#include <thread>
#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <vector>
#include <chrono>

std::mutex m;
std::recursive_mutex rm;
std::shared_mutex sm;

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
    // 参考function_2bと同じ処理をlock_guardを使わないで書いた場合
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
        std::vector<std::thread> threads;

        // (大量の)リーダースレッド
        for (size_t i = 0; i < 10; i++)
        {
            std::thread rdr{[&]()
                            {
                                // 読み出す場合にはstd::shared_lockを使う（他スレッドと同時に入れる）
                                std::shared_lock lock{sm};
                                std::cout << shared_value << std::endl; // この処理は非スレッドセーフなので注意
                            }};
            threads.push_back(std::move(rdr));
        }

        std::thread writer{[&]()
                           {
                               for (size_t i = 0; i < 10; i++)
                               {
                                   // 書き込む場合にはstd::lock_guardで排他する（rdrスレッドは入れない）
                                   std::lock_guard lock{sm};
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

    return 0;
}