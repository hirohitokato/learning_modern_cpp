/// ラムダ式

// 基本的な書き方：
// [キャプチャするオブジェクト](仮引数の一覧)->戻り値の型 { 複合文 }
//
// - キャプチャするオブジェクト(必須)：
//   - ラムダ式の中に持ってくるオブジェクトを指定する。複数指定可。コピー・参照どちらにするかに注意
//   - メンバ関数内で記述する場合は`[this]`を書くこと（寿命に注意）
// - 仮引数リスト(省略可)：
//   - 関数宣言の仮引数リストと同じ。省略した場合は`()`(=引数なし)となる
// - 戻り値の型(省略可)
//   - 戻り値の型。省略した場合は以下のルールで扱われる。
//     - 中のreturn文から自動的に推論
//     - それ以外はvoid
// - 複合文(必須)

#include <iostream>
#include <functional>
#include <string>

std::function<int(int)> invalid_lambda_example();
void long_process(void (*completion_handler)(int));

int main()
{
    using namespace std;

    cout << "-------------------------------------" << endl;
    {
        // ラムダ式のいろいろな書き方

        // 引数を１つ持ちdouble型を返す場合
        auto f2 = [](double a) -> double
        {
            return a * 2;
        };
        f2(2.5); // → 5.0

        // 引数を２つ持つ場合
        auto f = [](int a, const char *b)
        {
            cout << a << " & " << b << endl;
        };
        f(193, "Hello"); // "193 + Hello"

        // 引数２つとint型の返り値を持つ場合
        auto h = [](int a, int b) -> int
        {
            return a + b;
        };
        cout << "5 + 8 = " << h(5, 8) << endl; // "5 + 8 = 13"

        // この書き方でもOK
        // 返り値の型を推論してくれている
        auto h2 = [](int a, int b)
        {
            return a + b;
        };
        cout << "4 + 3 = " << h2(4, 3) << endl; // "4 + 3 = 7"

        // [ラムダ式の中身の話]
        // ↑のラムダ式は、コンパイル時に次のような形で
        // 関数オブジェクトとしてコンパイルされている
        struct something_H /*名前はコンパイラが勝手に決める*/
        {
            int operator()(int a, int b) const
            {
                return a + b;
            }
        };
        something_H h3{};
        cout << "4 + 3 = " << h3(4, 3) << endl; // "4 + 3 = 7"。上のg2と同じ結果

        cout << "type of h2:" << typeid(h2).name() << endl;
        // ↑はwinだと「class <lambda_4fb8879bd4b8231e033ae71dead7e178>」、
        // macOSだと「Z4mainE3$_4」のように出力される。なのでautoでないと受けられない

        // ちなみにこの書き方もOK(C++14～)。
        // ジェネリックラムダと呼ばれる仕様。簡易的なテンプレート関数として使える
        auto h2_fullauto = [](auto a, auto b)
        {
            return a + b;
        };
        cout << "4 + 3.2 = " << h2_fullauto(4, 3.2) << endl;                   // "4 + 3.2 = 7.2"
        cout << "ab + c = " << h2_fullauto(string("ab"), string("c")) << endl; // "ab + c = abc"
    }
    cout << "-------------------------------------" << endl;
    {
        // もっとも小さなラムダ式
        auto f = [] {}; // 引数・返り値が省略されている
        f();            // 呼べる(何も起きないけれど)

        // 作成したラムダ式はその場で呼び出せる
        // UIの部品をパーツごとの初期化処理でまとめるのに使ったことあり
        auto x = [](int a, int b) -> int
        {
            // 長い初期化処理
            return a * b;
        }(2, 10);                     // その場で2&10と共に呼んでいる。
        cout << "x is " << x << endl; // "x is 20"
    }
    cout << "-------------------------------------" << endl;
    {
        // キャプチャ

        // ラムダ式はそのままだと外にある変数を参照できない
        auto x = 42;
        // auto result = [](int y){ return x + y; }(3); // コンパイルエラー！xに触れない

        // 参照/変更できるようにするためには、変数をキャプチャする必要がある。
        // キャプチャには「コピーキャプチャ」と「参照キャプチャ」の２種類ある。
        //
        // * コピーキャプチャ: 変数をコピーしてキャプチャ。コピーコンストラクタが呼ばれる
        // * 参照キャプチャ： 変数の参照をキャプチャ。キャプチャした変数の内容を変更できる

        // []の中に、キャプチャしたい変数を書けばOK。これがコピーキャプチャ
        auto result = [x](int y)
        { return x + y; }(3); // xを参照できる
        cout << "x(42) + 3 = " << result << endl;

        // 変更したい場合は、変数に&を付けて参照キャプチャする
        [&x](int y)
        { x += y; }(5);
        cout << "current x is " << x << endl; // "current x is 47"が表示される
        // [x](int y){ x+=y; }(5); // これはコンパイルエラー！xを変更できない

        // キャプチャする変数は複数まとめて指定できる
        //
        // * xをコピーキャプチャ、yを参照キャプチャ
        auto y = 81;
        auto result2 = [x, &y](int z)
        { y = x + z; return y; }(5);
        cout << "result of `[x, &y](int z){ y = x + z; return y; }(5)` is " << result2 << endl;
        // ↑ result of `[x, &y](int z){ y = x + z; return y; }(5)` is 52
    }
    cout << "-------------------------------------" << endl;
    {
        // ここから次回

        // クラスとラムダ式
        //
        // クラス内のラムダ式でメンバー変数を使用したい場合は、キャプチャに
        // thisを指定する
        // auto f = [this]() {
        //     // ここでthisを参照し処理を実行できる。private変数等でもOK
        //     this->x = this->get_value(); // 例
        // }
        class TmpClass
        {
        private:
            int x;

        public:
            TmpClass(int x) : x(x) {}

            void add(int y)
            {
                auto in_add = [this, y]()
                {
                    // thisのprivateにもアクセスできる
                    return this->x + y;
                };
                cout << "this->x + y = " << in_add() << endl; // "this->x + y = 12";
            }
        };
        auto obj = TmpClass{7};
        obj.add(5);
    }
    cout << "-------------------------------------" << endl;
    {
        // まとめてキャプチャ
        int a{3}, b{9}, c{7};
        // 定義したタイミングで見えるすべての自動変数をキャプチャしたいときは[=]または[&]と書く
        // * [=](…){…}  : 全ての外の変数をコピーキャプチャ
        // * [&](…){…}  : 全ての外の変数を参照キャプチャ
        auto f = [=]()
        {
            cout << a << "/" << b << "/" << c << endl; // a,b,cすべて参照できる
        };
        auto g = [&]()
        {
            a *= 2; // a,b,cすべて変更できる
            b *= 3;
            c *= 4;
        };
        f();
        g();
        cout << a << "/" << b << "/" << c << endl; // a,b,cすべて変更されている

        // 応用
        // * [&, x]  : デフォルトで参照キャプチャ、変数xのみコピーして、ラムダ式のなかで使用する
        // * [=, &x] : デフォルトでコピーキャプチャ、変数xのみ参照して、ラムダ式のなかで使用する
    }
    cout << "-------------------------------------" << endl;
    {
        // 関数引数に渡したり、後で使うようにする場合
    }
    cout << "-------------------------------------" << endl;
    {
        // キャプチャするときの注意
        // * キャプチャした変数の寿命に注意

        // ラムダ式が参照キャプチャしている変数の寿命が切れたあとで呼び出したときの動作は未定義
        auto foo = invalid_lambda_example()(2); // 未定義の振る舞い
        cout << "foo value is unspecified. : " << foo << endl;
    }
    cout << "-------------------------------------" << endl;
    {
        // キャプチャしないラムダ式であれば普通の関数ポインタのように使える
        // (キャプチャしているとコンパイルエラー)
        long_process([](int result)
                     { cout << result << endl; }); // OK
        // long_process([=](int result)
        //              { cout << result << endl; }); // コンパイルエラー!
    }
}

std::function<int(int)> invalid_lambda_example()
{
    int n = 3;
    return [&n](int i) -> int
    { return n + i; }; // ローカル変数nを参照キャプチャし利用したラムダ式を返している(NG!)
}

/// @brief 関数ポインタを引数に持つ関数
/// @param completion_handler 処理完了時に呼び出されるコールバック
void long_process(void (*completion_handler)(int))
{
    // 長時間の処理
    int long_process_result = 42;
    // コールバックの関数ポインタを呼び出し
    completion_handler(long_process_result);
}

// * ラムダ式のメリットは何？
//   * 名前を考えなくて良い
//   * その場でしか使わない関数で名前空間を汚染しなくて済む
// * ラムダ式の直後に()を付けた場合、ラムダ式の戻り値には何が入っている？
//   * ラムダ式の実行結果
//   * ラムダ式はその場で作られ、使われ、捨てられる(=再利用不可)
