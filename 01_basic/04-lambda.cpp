/// ラムダ式

// 基本的な書き方：
// [キャプチャするオブジェクト](仮引数の一覧)->戻り値の型 { 複合文 }
// - キャプチャするオブジェクト(必須)：
//   - ラムダ式の中に持ってくるオブジェクトを指定する。複数指定可。コピー・参照どちらにするかに注意
//   - メンバ関数内で記述する場合は`[this]`を書くこと（寿命に注意）
// - 仮引数リスト(省略可)：
//   - 関数宣言の仮引数リストと同じ。省略した場合は`()`(=引数なし)となる
// - 戻り値の型(省略可)
//   - 戻り値の型。省略した場合は以下のルールで扱われる。
//     - 単一のreturn式になっていれば自動的に推論
//     - それ以外はvoid
// - 複合文(必須)

#include <iostream>

int main()
{
    using namespace std;

    cout << "-------------------------------------" << endl;
    {
        // ラムダ式のいろいろな書き方

        // 引数を２つ持つ場合
        auto f = [](int a, const char *b)
        {
            cout << a << " & " << b << endl;
        }; // セミコロンを忘れないように
        f(193, "Hello");

        // 引数２つとint型の返り値を持つ場合
        auto g = [](int a, int b) -> int
        {
            return a + b;
        };
        cout << "5 + 8 = " << g(5, 8) << endl;
        // この書き方でもOK
        // 返り値の型を推論してくれている
        auto g2 = [](int a, int b)
        {
            return a + b;
        };
        cout << "4 + 3 = " << g2(4, 3) << endl;

        // (中身の話)↑のラムダ式は、コンパイル時に次のような形で
        // 関数オブジェクトとしてコンパイルされている
        struct some_G /*名前はコンパイラが勝手に決める*/
        {
            int operator()(int a, int b) const
            {
                return a + b;
            }
        };
        some_G g3{};
        cout << "4 + 3 = " << g3(4, 3) << endl; // 上のg2と同じ結果
        cout << "type of g2:" << typeid(g2).name() << endl; // macOSだと「Z4mainE3$_4」のように出力される
    }
    cout << "-------------------------------------" << endl;
    {
        // もっとも小さなラムダ式
        auto f = []{}; // 引数・返り値が省略されている
        f(); // 呼べる(何も起きないけれど)

        // 作成したラムダ式はその場で呼び出せる
        auto x = [](int a, int b) -> int {
            return a * b;
        }(2, 10); // その場で2&10と共に呼んでいる。
        cout << "x is " << x << endl; // "x is 20"
    }
    cout << "-------------------------------------" << endl;
    {
        // キャプチャ

        // ラムダ式はそのままだと、外の変数を参照できない
        auto x = 42;
        // auto result = [](int y){ return x + y; }(3); // コンパイルエラー！xに触れない

        // 参照できるようにするためには、変数をキャプチャする必要がある

        // []の中に、キャプチャしたい変数を書けばOK。これがコピーキャプチャ
        auto result = [x](int y){ return x + y; }(3); // xを参照できる
        cout << "x(42) + 3 = " << result << endl;

        // 変更したい場合は、変数に&を付けて参照キャプチャする
        // [x](int y){ x+=y; }(5); // コンパイルエラー！xを変更できない
        [&x](int y){ x+=y; }(5);
        cout << "current x is " << x << endl; // 47(=42+5)が表示される

        // キャプチャする変数は複数まとめて指定できる
        auto y = 81;
        // xをコピーキャプチャ、yを参照キャプチャ
        auto result2 = [x, &y](int z){ y = x + z; return y; }(5);
        cout << "result of `[x, &y](int z){ y = x + z; return y; }(5)` is " << result2 << endl;

        // オブジェクトのメンバー変数を使用したい場合は、キャプチャに
        // thisを指定する
        // auto f = [this]() {
        //     ここでthisを参照し処理を実行できる。private変数等でもOK
        // }
    }
    cout << "-------------------------------------" << endl;
    {
        // まとめてキャプチャ
        // コピーと参照
    }
    cout << "-------------------------------------" << endl;
    {
        // まとめてキャプチャ
    }
    cout << "-------------------------------------" << endl;
    {
        // 関数引数に渡したり、後で使うようにする場合
    }
}