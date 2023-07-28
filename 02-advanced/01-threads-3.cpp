#include <thread>
#include <mutex>
#include <shared_mutex>
#include <semaphore>
#include <latch>
#include <iostream>
#include <vector>
#include <chrono>

std::mutex m;
std::recursive_mutex rm;
std::shared_mutex sm;
std::counting_semaphore sem{0};
std::latch ltch{10};

int shared_value = 0;

void function_1a()
{
    // lock_guardを使わないで書いた場合
    m.lock();
    shared_value++;

    if (shared_value > 5)
    {
        m.unlock();
        return;
    }
    std::cout << "function_1a: " << shared_value << std::endl;
    m.unlock();
}

void function_1b()
{
    // lock_guardを使って書いた場合
    // このスコープの中でmによる排他を実現する
    std::lock_guard lock{m};
    shared_value++; // 共有している情報に安全にアクセスできる

    if (shared_value > 5)
    {
        return; // 途中で抜けてもmをunlockしてくれる
    }
    std::cout << "function_1b: " << shared_value << std::endl;
}

void function_2b()
{
    std::lock_guard lock1{rm};
    std::lock_guard lock2{rm}; // 同一スレッドからであれば複数回ロック可能

    std::cout << std::this_thread::get_id() << ": " << __func__ << std::endl;
}

void function_2b_dash()
{
    // 参考: function_2bと同じ処理をlock_guardを使わないで書いた場合
    rm.lock();
    rm.lock();

    std::cout << std::this_thread::get_id() << ": " << __func__ << std::endl;

    // 同じ回数のアンロックが必要
    rm.unlock();
    rm.unlock();
}

void function_2a()
{
    std::lock_guard lock{rm};
    std::cout << std::this_thread::get_id() << ": " << __func__ << std::endl;
    function_2b(); // 同じrmをロックする関数を呼んでも安全
}

void function_3(int x)
{
    // 許可が下りるまで（セマフォを獲得できるまで）待機
    std::cout << "[semaphore] Wait processing ... " << x << std::endl;
    sem.acquire();

    // 処理の開始
    std::cout << "[semaphore] Start processing ... " << x << std::endl;
}

void function_4(int x)
{
    // なにかの初期処理を先行して実施
    std::cout << "[latch] Start processing ... " << x << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{100});

    // 処理が完了したことを通達。ラッチをカウントダウンする
    std::cout << "[latch] Finished. " << x << std::endl;
    ltch.count_down();
}

int main()
{
    {
        // 単純なロック
        std::vector<std::thread> threads;

        for (size_t i = 0; i < 10; i++)
        {
            std::thread t1{function_1a};
            std::thread t2{function_1b};
            threads.push_back(std::move(t1));
            threads.push_back(std::move(t2));
        }

        for (auto &thread : threads)
        {
            thread.join();
        }
    }

    {
        // 再帰ロックを使うと、ロックを同じスレッドで連続して獲得できる
        std::thread t1{function_2a};
        std::thread t2{function_2a}; // 別スレッドになるのでt1とは排他する

        t2.join();
        t1.join();
    }

    {
        // リーダーライター(Reader-Writer)ロック
        // 読み書きの頻度が非対称(読み>書き)の場合にパフォーマンス低下を抑えられる
        // ※ここでいう読み書きは「読み：値の変更などを伴わない軽い処理」「書き：値の変更を伴う重い処理」を一般的に表す

        // (大量の)リーダースレッド
        std::vector<std::thread> threads;
        for (size_t i = 0; i < 10; i++)
        {
            std::thread reader{[&]()
                               {
                                   // 読み出す場合にはstd::shared_lockを使う（他スレッドと同時に入れる）
                                   std::shared_lock lock{sm};
                                   std::cout << shared_value << std::endl; // この処理は非スレッドセーフなので注意
                               }};
            threads.push_back(std::move(reader));
        }

        std::thread writer{[&]()
                           {
                               for (size_t i = 0; i < 10; i++)
                               {
                                   // 書き込む場合にはstd::lock_guardで排他する（readerスレッドも入れない）
                                   std::scoped_lock lock{sm};
                                   shared_value++;
                                   std::cout << "increment shared_value:" << shared_value << std::endl;
                                   std::this_thread::sleep_for(std::chrono::milliseconds{30});
                               }
                           }};

        for (auto &thread : threads)
        {
            thread.join();
        }
        writer.join();
    }

    std::cout << "----------------------------------" << std::endl;

    {
        // セマフォの使用例（昔ファーム開発で書いたコード） (C++20)
        const auto num_threads = 10;

        // 他のスレッドからロック量をコントロールできるので色々使いまわしやすい
        std::vector<std::thread> threads;
        for (size_t i = 0; i < num_threads; i++)
        {
            // スレッドを起動・・・ただしまだ処理を開始せず待ち合わせる
            std::thread t{function_3, i};
            threads.push_back(std::move(t));
        }

        // なにかのスレッド開始前に必要な初期処理
        std::this_thread::sleep_for(std::chrono::seconds{1});

        // 眠っているスレッドをすべて起こす
        std::cout << "Start workers." << std::endl;
        for (size_t i = 0; i < num_threads; i++)
        {
            sem.release();
        }

        for (auto &thread : threads)
        {
            thread.join();
        }
    }

    std::cout << "----------------------------------" << std::endl;

    {
        // ラッチの使用例 (C++20)
        const auto num_threads = 10;

        // セマフォ同様に他のスレッドからロック量をコントロールできるので色々使いやすい
        std::vector<std::thread> threads;
        for (size_t i = 0; i < num_threads; i++)
        {
            // 初期化処理(を模した)スレッドを起動
            std::thread t{function_4, i};
            threads.push_back(std::move(t));
        }

        // 全スレッドの終了を待つ
        ltch.wait();

        // 残りの処理を開始
        std::cout << "All tasks are finished." << std::endl;

        for (auto &thread : threads)
        {
            thread.join();
        }
    }

    return 0;
}