/// autoによる型の自動推論と取得
/// * `auto`を使うと変数の型を自動的に決定できる。

#include <map>
#include <iostream>
#include <memory>
#include <functional>
#include <vector>

using namespace std;

int foo() { return 0; }

struct X
{
    int a;

    X(int a) : a(a) {}
    ~X()
    {
        cout << a << ": デストラクタが呼ばれました" << endl;
    }
    X(const X &other)
    {
        a = other.a + 1;
        cout << a << ": コピーコンストラクタが呼ばれました" << endl;
    }
};

class Interface
{
    virtual void foo() = 0;
};

class Implement : public Interface
{
public:
    void foo()
    {
        cout << "foo" << endl;
    }
};

///////////////////////////////////////////////
int main()
{
    {
        auto x = foo(); // xはint型
        auto fx = foo;  // xはint型を返す関数ポインタ `int (*)()`
        auto y = 1.0;   // yはdouble型
    }
    cout << "-------------------------------------" << endl;
    {
        // 指定子を付けてヒントにも出来る
        const auto cx = foo(); // cxはconst int型
        static auto y = 1.0;   // yは静的なdouble型
        int i = 100;
        auto p_i1 = &i;  // p_i1はint*型
        auto *p_i2 = &i; // p_i2はint*型
        // ↓はエラーになる。型が値/ポインタ/参照のどれなのか意識して書くとコンパイルエラーにしてくれて良い
        // auto *p_i3 = i;
    }
    cout << "-------------------------------------" << endl;
    {
        auto a = make_unique<int>(5);
        auto b = make_unique<int>(7);

        // ラムダ式はコンパイラしか型情報が分からない → autoで推論する変数でしか束縛できない(*)
        auto some_func = [](const std::unique_ptr<int> &p1, const std::unique_ptr<int> &p2)
        {
            return *p1 < *p2;
        };
        cout << "lambda ver.: " << some_func(a, b) << endl;
        // (正確にはstd::functionでラップできるがメモリ消費もスピードも悪化する)
        std::function<bool(const std::unique_ptr<int> &p1, const std::unique_ptr<int> &p2)>
            some_func2 = [](const std::unique_ptr<int> &p1, const std::unique_ptr<int> &p2)
        {
            return *p1 < *p2;
        };
        cout << "std::function ver.: " << some_func2(a, b) << endl;
    }
    cout << "-------------------------------------" << endl;
    {
        using namespace std;

        // ラムダ式の仮引数にもautoを使える(ジェネリックラムダ)。
        auto f2 = [](auto a, auto b)
        { cout << "a is " << a << ", b is " << b << endl; };
        f2(2.4, 3);   // aはdouble型、bはint型とみなしてくれる。テンプレート関数もどきになる
        f2(1, "abc"); // aはint型、bはconst char* const型とみなしてくれる。テンプレート関数もどきになる
    }
    cout << "-------------------------------------" << endl;
    {
        // イテレータに使うと便利＆安全になる(1)
        using namespace std;

        // 型の宣言を間違えると不要なコピー処理が発生してしまうので積極的にautoを使う
        map<string, int> m = {{"a", 1}, {"c", 3}}; // ※memo:ここでは`auto m2 = {...};`とは書けない。コンパイラが推測してくれない

        for (const auto &e : m) // このautoの実体は`const std::pair<const std::string, int>&`
        {
            cout << e.first << " : " << e.second << endl;
        }
        // これをautoを使わずfor (const std::pair<std::string, int>& &e : m) {...}`と書くと不要なコピー処理が走ってしまう！！（あとで補足）
    }
    cout << "-------------------------------------" << endl;
    {
        // イテレータに使うと便利＆安全になる(2)
        // autoを書くときの工夫
        vector<X> x_vector = {X(1), X(2), X(3)};

        cout << "----" << endl;
        cout << "コピーコンストラクタが呼ばれてしまう書き方(1)" << endl;
        for (X x : x_vector)
        {
            cout << "X(" << x.a << ")" << endl;
        }
        cout << "----" << endl;

        cout << "コピーコンストラクタが呼ばれてしまう書き方(2)" << endl;
        for (auto x : x_vector)
        {
            cout << "X(" << x.a << ")" << endl;
        }
        cout << "----" << endl;

        cout << "コピーコンストラクタが呼ばれない良い書き方" << endl;
        for (const auto &x : x_vector)
        {
            cout << "X(" << x.a << ")" << endl;
        }
        cout << "----" << endl;
        cout << "コピーコンストラクタは呼ばれないが面倒くさい書き方" << endl;
        for (vector<X>::iterator it = x_vector.begin(), end = x_vector.end(); it != end; ++it)
        {
            cout << "X(" << (*it).a << ")" << endl;
        }
        cout << "----" << endl;
    }
    cout << "-------------------------------------" << endl;
    {
        // autoが使えない場面、、ではなく使わない方が良い場面

        auto *obj1 = static_cast<Interface *>(new Implement()); // Interface*型
        auto *obj2 = new Implement();                           // Implement*型。

        // 以下の記述は不正確なので削除。注意点は https://pc98.skr.jp/post/2020/0616/ を参照してほしい
        // Interface &ref = *obj;
        // // Interface obj = Implement();
        // vector<Interface &> a{};
        // a.push_back(ref);
    }
}

// Q1: autoを使うとスピード/メモリは落ちる？
//  A1: No. 上で挙げたstd::functionの記述は、std::functionでラップするので追加のリソース消費があるという話
// Q2: autoでスピードが落ちることはある？
//  A2: コンパイル時に決まるので変わらない（コンパイル時間は伸びる）
// Q3: イテレータで配列にアクセスするときはコンテナの要素を参照しているのでコピーコンストラクタが走らない？
//  A3: Yes
// Q4: コピーコンストラクタは勝手に作られる？あえて書くことでオーバーライドしてくれる？
//  A4: Yes
