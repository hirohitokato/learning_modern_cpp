// explicit指定子について
// 暗黙的な型変換を禁止でき、API提供者の「こう使ってほしい」を明確にできる
// 参考： https://komorinfo.com/blog/cpp-explicit-specifier/
#include <iostream>

struct A
{
    A() {}
    A(int x) {}
    A(const char *s, int x = 0) {}
};

struct A2
{
    explicit A2() { std::cout << "1\n"; }
    explicit A2(int x) { std::cout << "2\n"; }
    explicit A2(const char *s, int x = 0) { std::cout << "3\n"; }
};

struct B
{
    B(int, int) {}
};

struct B2
{
    explicit B2(int, int) {}
};

void FuncB(B b) {}
void FuncB2(B2 b) {}

int main()
{
    {
        // explicitがないとこう書ける(書けてしまう)
        A a1 = 1;       // A(int x)が呼ばれる
        A a2 = "Hello"; // A(const char *s, int x = 0)が呼ばれる

        // ちなみにこれらは明示的なのでOK
        A a3;                     // A()
        A a4 = A(1);              // A(int x)
        A a5{1};                  // A(int x)
        A a6 = A("Venditti");     // A(const char*,int)
        A *p = new A(1);          // A(int x)
        A a7 = (A)1;              // A(int x)。明示的型変換になる
        A a8 = static_cast<A>(1); // A(int x)
    }
    {
        // explicitをつけるとどちらも禁止できる
        // A2 a1 = 1;
        // A2 a2 = "Hello";

        // これらはexplicitでもOK
        A2 a3;                       // A()
        A2 a4 = A2(1);               // A(int x)
        A2 a40 = A2{1};              // A(int x)
        A2 a5(1);                    // A(int x)
        A2 a6 = A2("Venditti");      // A(const char*,int)
        A2 *p = new A2(1);           // A(int x)
        A2 a7 = (A2)1;               // A(int x)。明示的型変換になる
        A2 a8 = static_cast<A2>(1);  // A(int x)
        A2 a9 = (A2) "Hello";        // A(const char*,int)
        A2 a10 = (A2){"Hello", 193}; // A(const char*,int) ←MSVCだとコンパイルエラー(ClangはOK)
    }
    {
        B b1 = {0, 0}; // OK。これを良しとするかどうかはAPI提供者次第
        FuncB({1, 2}); // OK。わかりにくいけれど
    }
    {
        // B2 b1 = {0, 0}; // NG。コンパイルエラー
        // FuncB2({0, 0}); // NG。コンパイルエラー
        B2 b1{0, 0};      // OK
        FuncB2(B2(0, 0)); // OK
    }
}

// [Google Style Guide](https://ttsuki.github.io/styleguide/cppguide.ja.html#Implicit_Conversions)
// にかかれている内容を整理すると、
// * 暗黙的型変換を新たに定義してはダメ。引数を１つとるコンストラクタにおいてはexplicitキーワードを使用すること
//   * (型変換演算子もそうだが、今回は省略)
// * ただし以下のケースはexplicitはなくても良い
//   * std::initializer_list型の引数を１つとるコンストラクタで、
//     コピーによる初期化(例: MyType m = {1, 2};)をサポートするためにはexplicitを省略する。
