// マルチスレッド
//
// 第2部：C++ でのマルチスレッドプログラミング

// `<thread>`ヘッダをインクルードして使うAPIを使用する。
// C++ 11以前で使われていたOS/環境依存のライブラリは、必要なとき以外は使わない。

#include <iostream>
#include <string>
#include <thread>
#include <mutex>

namespace
{
    std::mutex _printMutex;
}

void print_threadsafe(const std::string &msg)
{
    // std::coutはスレッドセーフではないため、他のスレッドとで出力が混じることがあります
    // ここではmutexを使って排他的な出力処理を実現しています。
    std::lock_guard lock{_printMutex};
    std::cout << msg << std::endl;
}

void my_function()
{
    print_threadsafe("  | Runs on thread.");
}

int main()
{
    // スレッドの作成と実行開始
    std::thread my_thread{my_function};

    // 【注意!】
    // 下の出力処理はスレッドセーフではないため、他のスレッドとで出力が混じることがあります
    print_threadsafe("Output on main thread.");

    // スレッド終了の待ち合わせ
    my_thread.join();

    return 0;
}