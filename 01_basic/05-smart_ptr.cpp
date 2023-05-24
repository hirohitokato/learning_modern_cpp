// スマートポインタ

#include <iostream>
#include <memory>
#include <string>
using namespace std::literals::string_literals; // "～"sを使えるようにするための宣言

struct MyClass
{
    MyClass() : value(1) {}
    MyClass(int x, double y) : value(x) {}
    ~MyClass() { std::cout << "destruct" << std::endl; }

    int value;
    // ここでunique_ptrのメンバ変数を作った場合の生存期間はクラスと一緒になる
    void method()
    {
        std::cout << "Hello smart pointer!\n";
    }
};

void learn_unique_ptr();
void learn_shared_ptr();
void learn_weak_ptr();

int main()
{
    // 前提： 設計において、人間はポインタが指す先(リソース)に対し「誰の持ち物か」を意識している
    {
        using namespace std;

        int *a = new int{100};
        int *b = a; // bもaと同じリソースを参照できるようになった

        // 問題1：
        // ヒープに確保した100を保存している領域は誰のもの？
        // a?b?それとも両方？

        // 問題2：
        // 不要になったときにaとbどちらをdeleteすべき？残ったbはどうなる？
        delete a;
    }

    // 所有権を明示し強制できて、安全に使えるようになると嬉しい
    // → スマートポインタの登場。ポインタを生で使わないですむため、以下に心配しなくて良い
    // * 不正アクセス
    // * 解放忘れ
    // * メモリリーク

    // モダンなC++プログラミングではnew/deleteを使わない

    // 1. std::unique_ptr
    std::cout << "--- 1. std::unique_ptr ---" << std::endl;
    learn_unique_ptr();

    // 2. std::shared_ptr
    std::cout << "--- 2. std::shared_ptr ---" << std::endl;
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
    // 作成
    // コンストラクタを使う。ただしできるだけmake_uniqueを使う(C++14～)
    std::unique_ptr<int> x1 = std::make_unique<int>(100);
    std::unique_ptr<int> x2{new int{193}}; // これも可
    std::unique_ptr<MyClass> obj1 = std::make_unique<MyClass>();
    std::unique_ptr<MyClass> obj2 = std::make_unique<MyClass>(200, 3.14); // コンストラクタへの引数も渡せる

    // 配列も作れる
    std::unique_ptr<int[]> array = std::make_unique<int[]>(10); // C++20～。10個の要素を持つ未初期化の配列
    std::unique_ptr<int[]> old_array(new int[10]);              // ～C++17。10個の要素を持つ未初期化の配列

    // あとからセットしたい場合はreset()を使う
    {
        auto x = new MyClass();
        std::unique_ptr<MyClass> obj3;
        obj3.reset(x) // obj3.reset();だとnullptrが渡される
    }

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
        std::cout << "x1 is " << (x1 ? "active"s : "inactive"s) << std::endl; // x1 is active
        // delete raw_pointer; // だめ！
    }

    // どうしてもdeleteしたい場合は・・・
    // release()の返り値を使う。
    {
        int *raw_pointer = x1.release(); // 一度releaseするとx1が破棄されるときにdeleteしなくなる
        std::cout << "x1 is " << (x1 ? "active"s : "inactive"s) << std::endl; // x1 is inactive
        delete raw_pointer;                                                   // 呼んであげないとメモリリーク
        std::cout << "x1.release()" << std::endl;
    }

    // 応用：
    // 解放時に独自関数を実行させることもできる（カスタムデリータ）。
    // ※カスタムデリータを定義するときにmake_uniqueは使えないので注意
    {
        auto custom_deleter = [](int *x)
        {
            // リソース解放処理をここで書いてあげる（ファイルクローズとか）
            std::cout << "custom_deleter for " << *x << " is called." << std::endl;
            if (x != nullptr)
                delete x;
        };
        std::unique_ptr<int, void (*)(int *)> pointer{new int(4649), custom_deleter};
    } // ここで"custom_deleter for 4649 is called."が出力される
      // 注意: release()を途中で呼び中のポインタを取り出すとカスタムデリータも呼ばれなくなる

    // メリット：
    // * 所有権が１つであることを明確にできる
    // * スコープを外れたら確実に解放してくれる
    // * ポインタのように使える。既存ポインタからも作れる
    // * メモリ解放の安心感(スタック変数のように使える＆スタック領域を消費しない)
    // * カスタムデリータを使えば後始末のようなこともできる
}

//
// 2. std::shared_ptr
// 所有者(複数)がいなくなったときにリソース解放を保証してくれるスマートポインタ
//
void learn_shared_ptr()
{
    // 作成: unique_ptrとほとんど同じ
    //
    // コンストラクタを使う。ただしできるだけmake_sharedを使う
    std::shared_ptr<int> x1 = std::make_shared<int>(100);
    std::shared_ptr<int> x2{new int{193}}; // これも可
    std::shared_ptr<MyClass> obj1 = std::make_shared<MyClass>();
    std::shared_ptr<MyClass> obj2 = std::make_shared<MyClass>(200, 3.14); // コンストラクタへの引数も渡せる

    // 配列も作れる
    std::shared_ptr<int[]> array = std::make_shared<int[]>(10);
    std::shared_ptr<int[]> old_array(new int[10]);

    // あとからセットしたい場合はreset()を使う
    {
        std::shared_ptr<MyClass> obj3;
        obj3.reset(new MyClass{15, 1.41});
    }
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
}

void learn_weak_ptr()
{
}

// scoped/autoは使わない
