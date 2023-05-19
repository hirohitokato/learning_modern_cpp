// スマートポインタ

#include <iostream>
#include <memory>
#include <string>
using namespace std::literals::string_literals; // "～"sを使えるようにするための宣言

struct MyClass
{
    MyClass() : value(1) {}
    MyClass(int x, double y) : value(x) {}

    int value;
    void method()
    {
        std::cout << "Hello smart pointer!\n";
    }
};

void learn_unique_ptr();
void learn_shared_ptr();
void learn_weak_ptr();

int main() {
    // 前提： 設計において、人間はポインタが指す先(リソース)に対し「誰の持ち物か」を意識している
    {
        using namespace std;

        int *a = new int{100};
        int *b = a; // bもaと同じリソースを参照できるようになった

        // 問題1：
        // ヒープに確保した100を保存している領域は誰のもの？
        // a?b?それとも両方？

        // 問題2：
        // 不要になったときにaとbどちらをdeleteすべき？
        delete a;
    }

    // 所有権を明示し強制できて、安全に使えるようになるといいな
    // → スマートポインタの登場。ポインタを生で使わないですむため、以下に心配しなくて良い
    // * 不正アクセス
    // * 解放忘れ
    // * メモリリーク

    // モダンなC++プログラミングではnew/deleteを使わない

    // 1. std::unique_ptr
    learn_unique_ptr();

    // 2. std::shared_ptr
    learn_shared_ptr();

    // 3. std::weak_ptr
    learn_weak_ptr();

    return 0;
}

//
// 1. std::unique_ptr
// 所有者が常に1人であることを保証してくれるスマートポインタ
//
void learn_unique_ptr()
{
    //
    // 1. std::unique_ptr
    // 所有者が常に1人であることを保証してくれるスマートポインタ
    //

    // 作成
    // コンストラクタを使う。ただしできるだけmake_uniqueを使う(C++14～)
    std::unique_ptr<int> x1 = std::make_unique<int>(100);
    std::unique_ptr<int> x2{new int{193}}; // これも可
    std::unique_ptr<MyClass> obj1 = std::make_unique<MyClass>();
    std::unique_ptr<MyClass> obj2 = std::make_unique<MyClass>(200, 3.14); // コンストラクタへの引数も渡せる

    // 配列も作れる
    std::unique_ptr<int[]> array = std::make_unique<int[]>(10); // C++20～
    std::unique_ptr<int[]> old_array(new int[10]);              // ～C++17

    // 取り扱い
    // *  : 中身にアクセスする
    // -> : ポインタの要素にアクセスする
    {
        std::cout << x1 << "/" << *x1 << "/" << *x2 << std::endl; // → {アドレス}/100/193
        obj1->method();                                           // → Hello smart pointer!
        std::cout << obj2->value << std::endl;                    // → 200

        // [] : 要素にもアクセス可能
        array[5] = 10;
        old_array[8] = 3;
        for (size_t i = 0; i < 10; i++)
        {
            // array[5]とold_array[8]で設定された値が出てくる（他は未初期化）
            std::cout << array[i] << "/" << old_array[i] << ", ";
        }
        std::cout << std::endl;
    }

    // 【Point】コピーはできない（＝所有権を共有できない）
    {
        // auto other_x1 = x1; // コンパイルエラー！
    }

    // 所有権の移動：
    // std::move()を使う
    {
        // std::move()を使う
        auto x2_newowner = std::move(x2);

        std::cout << *x2_newowner << std::endl;                               // 193 （所有権が移動している）
        std::cout << "x2 is " << (x2 ? "active"s : "inactive"s) << std::endl; // x2 is inactive
        // std::cout << *x2 << std::endl;                                     // 例外発生！もうアクセスできない
        std::cout << x2 << std::endl; // 「00000000」が出力される(winの場合)
        // ※ここでx2_newownerのスコープから外れるので、元x2は解放される
    }

    // 生ポインタを取得する（危険！）：
    // get()したデータを絶対deleteしないように！！
    {
        int *raw_pointer = x1.get();
        std::cout << raw_pointer << "/" << *raw_pointer << std::endl; // → {アドレス}/100
        // delete raw_pointer; // だめ！
    }

    // どうしてもdeleteしたい場合は・・・
    // release()の返り値を使う。
    {
        int *raw_pointer = x1.release(); // 一度releaseするとx1が破棄されるときにdeleteしなくなる

        delete raw_pointer;
    }
}

void learn_shared_ptr()
{
}

void learn_weak_ptr()
{
}

// scoped/autoは使わない
