#include <iostream>
#include <memory>
#include <functional>
#include <vector>

using namespace std;

// autoによる変数定義は初期化が必要
// auto i; ← NG。何の型か分からない

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
        a = other.a;
        cout << a << ": コピーコンストラクタが呼ばれました" << endl;
    }
};

int main()
{

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
        // autoを書くときの工夫
        vector x_vector = {X(1), X(2), X(3)};
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
}