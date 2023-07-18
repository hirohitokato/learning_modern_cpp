#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

std::mutex m;
std::recursive_mutex rm;

int shared_value = 0;

void function_1a()
{
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
        std::thread t2{function_2a};

        t2.join();
        t1.join();
    }

    return 0;
}