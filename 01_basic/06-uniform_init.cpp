// 「一様初期化 (uniform initialization)」
// コンストラクタの呼び出しを、リスト初期化と合わせて波カッコ{ }で記述する構文
//
// 波括弧を使った変数初期化や、コンストラクタの呼び出しができる初期化方法とも言える

#include <iostream>
#include <string>
#include <vector>

struct S
{
    int a, b;
};

struct X
{
    X(int, int, int) {}
};

struct U
{
    int a, b, c;
};

U createU()
{
    return {1, 2, 3};
}

int main()
{
    {
        // 統一された書き方で初期化できる

        // 従来の初期化
        int x = 3;              // "=" を使った方法
        int a[] = {0, 1, 2, 3}; // "= {}" を使った方法
        S s = {0, 1};           // これも "= {}" を使った方法
        S s1(0, 1);
        std::vector<int> v;
        for (int i = 0; i < 4; ++i)
            v.push_back(a[i]); // ループを使ってコンテナを初期化

        // 一様初期化を使ったモダンな初期化
        int x_{3};
        int a_[]{0, 1, 2, 3};
        S s_{0, 1};
        std::vector<int> v_{0, 1, 2, 3}; // コンテナも一括初期化可能
    }
    {
        // 戻り値の型指定を省略できる
        // 上の`U createU()`関数を参照
        U u = createU();
    }
    {
        // ユーザー定義のコンテナクラスもリスト初期化できる
        // (std::initializer_list<T>を引数にとるコンストラクタを定義する)
        int a[] = {1, 2, 3};            // C++03でも出来た
        std::vector<int> v = {1, 2, 3}; // C++11から出来る
    }
    {
        // コンストラクタの引数に与えた関数の呼ばれる順序が保証される
        auto f = []()
        {
            std::cout << 1 << std::endl;
            return 1;
        };

        auto g = []()
        {
            std::cout << 2 << std::endl;
            return 2;
        };

        auto h = []()
        {
            std::cout << 3 << std::endl;
            return 3;
        };

        X x1(f(), g(), h()); // 呼び出し順序が不定
        X x2{f(), g(), h()}; // f→g→hで必ず初期化
    }
    {
        // Cの構造体と同様の初期化ができる
        struct T1
        {
            int a;
            double b;
        };
        struct T2
        {
            T2(int, double) {}
        };
        T1 v1 = {1, 1.0}; // 構造体として波カッコで初期化可能(C++03)
        T1 v2{1, 1.0};    // 構造体として波カッコで初期化可能(C++03)
        T2 v3 = {1, 1.0}; // コンストラクタ呼び出し(C++11)
        T2 v4{1, 1.0};    // コンストラクタ呼び出し(C++11)
    }
    {
        // メンバ変数の初期化のためだけにコンストラクタ定義しなくても良い

        class Foo
        {
            // Foo(int x, std::string str): _x{x}, _str(str) {} 不要
            int _x{100};
            std::string _str{"FOO"};
        };
        Foo foo{}; // 100 ＆”FOO"で初期化済み
    }
    {
        // 精度が落ちる暗黙的な型変換をエラーにしてくれる
        // int test {100.1}; // コンパイルエラー。整数型への変換で精度が落ちるため。
        int test{100}; // OK
        int test2 = 100.1; // 100として格納
    }
    {
        // 一様初期化を使わないほうが良いケース
        // @see https://qiita.com/h2suzuki/items/d033679afde821d04af8#3-使わない方が無難なケース

        // 1. 要素が１個のコンストラクタ
        std::vector<int> v(10);         // サイズ10の int のベクタ、全要素 0
        std::vector<int> w{10};         // サイズ1の int のベクタ、最初の要素 10
        std::vector<std::string> s{10}; // サイズ10の string のベクタ、全要素は空（コンパイルエラーにしてくれない）
    }

    return 0;
}
