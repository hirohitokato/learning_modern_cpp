// 非同期(async。C++11)
//
// asyncは関数オブジェクトの非同期処理のしくみを提供する関数テンプレート。
// async()関数はstd::futureオブジェクトを返すことから、処理完了待ちと処理結果の受け渡しが行える
//
// 起動ポリシーも指定でき、非同期処理の動作メカニズムを指定できる。C++20時点では次の2ポリシーを指定可。
// * std::launch::async = 新しいスレッドを作成し、その上で非同期処理を実行
// * std::launch::deferred = 新しいスレッドは作成せず、wait()関数/get()関数が呼ばれたときに初めて実行

#include <chrono>
#include <iostream>
#include <vector>
#include <thread>

#include <future>

int shared_sum = 0;

int mytask1(const std::vector<int> &data)
{
    int sum = 0;

    for (int i : data)
    {
        // 時間がかかる処理のマネ
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
        sum += i;
    }

    return sum; // 値を戻せる
}

int main()
{
    auto data = std::vector<int>{1, 2, 3, 4, 5};

    // 1. std::launch::asyncによる新規スレッド起動＆実行
    {
        // dataを参照渡ししつつ、処理を新しいスレッドですぐに実行開始
        std::future<int> future = std::async(std::launch::async, mytask1, ref(data));

        try
        {
            // 別スレッドで実行された処理を待ち受ける
            auto sum = future.get();
            std::cout << sum << std::endl;
        }
        catch (...)
        {
            std::cerr << "somthing exception occurred." << std::endl;
        }

        // 補足:
        // MSVCにおけるlaunch::async指定した際のこの関数の実装は、Windowsの提供する
        // スレッドプールのスレッドを用いて処理を実行する。そのため新しいスレッドを起動せず、
        // 処理スレッドは終了されなくなりパフォーマンスには優れるが、規約違反なので注意。
    }

    // 2. std::launch::deferredによる遅延実行
    {
        // dataを参照渡ししつつ、処理を後で呼び出せる状態にしておく
        std::future<int> future = std::async(std::launch::deferred, mytask1, ref(data));

        try
        {
            // get()/wait()の呼び出しと同時に処理を実行開始する
            auto sum = future.get();
            std::cout << sum << std::endl;
        }
        catch (...)
        {
            std::cerr << "somthing exception occurred." << std::endl;
        }
    }

    // 3. その他
    {
        // a) ポリシーを２つ同時に付けた場合は、無指定と同じで「どちらのポリシーで実行しても良い」となる
        auto future1 = std::async(std::launch::async | std::launch::deferred, mytask1, ref(data));
        // auto future1 = std::async(mytask1, ref(data)); と同じ意味

        // ちなみに一度getしたfutureオブジェクトにもう1度get()すると例外が投げられる
        future1.get();
        // future1.get();ここで例外発生するのでコメントアウト
    }

    return 0;
}
