// マルチスレッド
//
// 第2部：C++ でのマルチスレッドプログラミング

// `<thread>`ヘッダをインクルードして使うAPIを使用する。
// C++ 11以前で使われていたOS/環境依存のライブラリは、必要なとき以外は使わない。

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <thread>
#include <mutex>
#include <future>

namespace
{
    // 排他変数はスレッド間で共有できるスコープに置いておく
    std::mutex _printMutex;
}

void print_threadsafe(const std::string &msg)
{
    // std::coutはスレッドセーフではないため、他のスレッドとで出力が混じることがある
    // ここではmutexを使って排他的な出力処理を実現している。
    // → 全員がprint_threadsafe()を使っている場合は安全
    std::lock_guard lock{_printMutex};
    std::cout << msg << std::endl;
}

void my_function()
{
    print_threadsafe("  | Runs on thread.");
}

struct TrivialClass
{
    int y = 10;
};

void my_function2(int x, std::unique_ptr<TrivialClass> obj)
{
    std::stringstream ss;
    ss << "  | Runs on thread with value(" << x << " & " << obj->y << ")";
    print_threadsafe(ss.str());
}

struct ThreadableClass
{
    int _x;

    ThreadableClass(int x) : _x(x) {}

    void operator()()
    {
        // 関数オブジェクトとしてobj()で呼べるようにしておくと、スレッドの
        // エントリポイントとしてメンバー関数を渡せる
        //
        std::stringstream ss;
        ss << "  | Runs on thread with value(" << _x << ")"; // メンバー変数にもアクセス可
        print_threadsafe(ss.str());
    }
};

struct ThreadableClass2
{
    int _x;

    ThreadableClass2(int x) : _x(x) {}

    void member_function()
    {
        std::stringstream ss;
        ss << "  | Runs on thread with value(" << _x << ")"; // メンバー変数にもアクセス可
        print_threadsafe(ss.str());
    }
};

void my_function3(std::promise<int> p, int arg)
{
    // (時間のかかる計算)
    auto result = arg * 10;

    std::stringstream ss;
    ss << "  | Runs on thread: arg(" << arg << ") * 10 = " << result;

    // 結果をセット
    p.set_value(result);
}

int main()
{
    // 1. シンプルなスレッド起動と待ち合わせ
    {
        // スレッドの作成と実行開始
        std::thread my_thread{my_function};

        // 【注意!】
        // 下の出力処理はスレッドセーフではないため、他のスレッドとで出力が混じることがあります
        std::cout << "Output on main thread." << std::endl;

        // スレッド終了の待ち合わせ
        my_thread.join();
        std::cout << "thread joined." << std::endl;
    }

    // スレッドを引数付きで起動する
    {
        auto ptr = std::make_unique<TrivialClass>();

        // スレッドの作成と実行開始。
        // このとき関数の引数で値を渡せる。unique_ptrの場合はstd::move()で渡す
        std::thread my_thread{my_function2, 193, std::move(ptr)};

        print_threadsafe("Output on main thread.");

        my_thread.join(); // もしjoinしないままスコープを抜けるとSIGABRT発生
    }

    // スレッドをオブジェクトのメソッドとして起動する
    // (メンバー関数はそのままだと渡せないので、ちょっとしたテクニックが必要)
    {
        // 1.関数オブジェクトとして用意する
        ThreadableClass myobj{42};

        // スレッドの作成と実行開始
        // このときmyobj()、つまりmyobjの`void operator()()`の処理が呼ばれる
        std::thread my_thread{myobj};

        print_threadsafe("Output on main thread.");

        my_thread.join();

        /*----------------------------------------*/

        // 2.lambdaを介して渡す
        ThreadableClass2 myobj2{193};

        // スレッドの作成と実行開始
        // このときmyobj()、つまりmyobjの`void operator()()`の処理が呼ばれる
        std::thread my_thread2{[&myobj2]()
                               { myobj2.member_function(); }};
        my_thread2.join();

        // 他にも https://q7z.hatenablog.com/entry/2014/05/24/092127
        // にあるようなthisポインタを渡す方法もあるが、lambdaがあるなら
        // それを使った方がよいと思う
    }

    // スレッドの実行結果を受け取る
    {
        // promiseオブジェクトを渡しておき、そのfutureオブジェクト経由で値を受け渡しする

        std::promise<int> p{};                    // 1) 受け渡し役になるオブジェクトを準備
        std::future<int> future = p.get_future(); // 2) スレッドに渡す前にfutureを入手しておく

        // 3) スレッドの作成と実行開始
        // promiseオブジェクトはstd::move()で所有権を手放しておくと安全
        std::thread my_thread{my_function3, std::move(p), 193};

        // 4) 終了待ち
        my_thread.join();

        // 5) 結果の受け取り
        int result = future.get(); // 1930が得られる
        std::cout << "result is " << result << std::endl;
    }

    // 補足:スレッド情報の入手
    {
        // ハードウェアでサポートされているCPUスレッドの数
        // (M2 MacBook/ DAIV 4Nだと8が返る)
        std::cout << "concurrency = " << std::thread::hardware_concurrency() << std::endl;
    }
    return 0;
}