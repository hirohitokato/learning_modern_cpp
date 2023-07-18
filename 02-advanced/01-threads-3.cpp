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

    if (shared_value > 1)
    {
        return;
    }
    shared_value++;
    std::cout << "function_1a: " << shared_value << std::endl;
    m.unlock();
}

void function_1b()
{
    // このスコープの中でmによる排他を実現する
    std::lock_guard lock{m};

    if (shared_value > 1)
    {
        return; // 途中で抜けてもmをunlockしてくれる
    }
    shared_value++; // 共有している情報に安全にアクセスできる
    std::cout << "function_1b: " << shared_value << std::endl;
}

void func2()
{
    std::lock_guard lock1{rm};
    std::lock_guard lock2{rm}; // 同一スレッドからであれば複数回ロック可能
}

int main()
{
    {
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
    }

    return 0;
}