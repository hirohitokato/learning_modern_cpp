// スマートポインタ

#include <iostream>
#include <memory>
#include <string>
#include <vector>
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

void func_with_unique_ptr(std::unique_ptr<int> ptr);
void func_with_unique_ptr_ref(std::unique_ptr<int> &ptr);
void func_with_shared_ptr(std::shared_ptr<int> ptr);
void func_with_shared_ptr_ref(std::shared_ptr<int> &ptr);

//
// 1. std::unique_ptr
// 所有者が常に1人であることを保証してくれるスマートポインタ
//
void learn_unique_ptr()
{
    // 作成
    // コンストラクタを使う。ただしできるだけmake_uniqueを使う(C++14～)
    std::unique_ptr<int> x1 = std::make_unique<int>(100);
    auto x1_ = std::make_unique<int>(193);
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
        obj3.reset(x); // obj3.reset();だとnullptrが渡される
    }

    // 取り扱い
    // *  : 中身にアクセスする
    // -> : ポインタの要素にアクセスする
    {
        std::cout << x1 << "/" << *x1 << "/" << *x2 << std::endl; // → {アドレス}/100/193
        *x1 = 4649;                                               // 書き換えられる
        std::cout << x1 << "/" << *x1 << "/" << *x2 << std::endl; // → {アドレス}/4649/193
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
        std::cout << raw_pointer << "/" << *raw_pointer << std::endl;         // → {アドレス}/100
        std::cout << "x1 is " << (x1 ? "active"s : "inactive"s) << std::endl; // x1 is active
        // delete raw_pointer; // だめ！
    }

    // どうしてもdeleteしたい場合は・・・
    // release()の返り値を使う。
    {
        int *raw_pointer = x1.release();                                      // 一度releaseするとx1が破棄されるときにdeleteしなくなる
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

    // 関数に渡す時の注意
    {
        auto obj = std::make_unique<int>(42);
        // func_with_unique_ptr(obj);            // コンパイルエラー！コピーが発生するので値渡しでは呼べない
        func_with_unique_ptr_ref(obj);        // これは参照渡しなので呼べる
        func_with_unique_ptr(std::move(obj)); // これは呼べるが、値を手放しているのでobjは無効になってしまう
    }

    // メリット：
    // * 所有権が１つであることを明確にできる
    // * スコープを外れたら確実に解放してくれる
    // * ポインタのように使える。既存ポインタからも作れる
    // * メモリ解放の安心感(スタック変数のように使える＆スタック領域を消費しない)
    // * カスタムデリータを使えば後始末のようなこともできる
}

//
// 2. std::shared_ptr
// 所有者が複数。全員手放したときにリソース解放を保証してくれるスマートポインタ
//
void learn_shared_ptr()
{
    // 作成: unique_ptrとほとんど同じ
    //
    // コンストラクタを使う。ただしできるだけmake_sharedを使う
    std::shared_ptr<int> x1 = std::make_shared<int>(100);
    auto x1_ = std::make_shared<int>(100);
    std::shared_ptr<int> x2{new int{193}}; // これも可
    std::shared_ptr<MyClass> obj1 = std::make_shared<MyClass>();
    std::shared_ptr<MyClass> obj2 = std::make_shared<MyClass>(200, 3.14); // コンストラクタへの引数も渡せる

    // 配列も作れる: unique_ptrとは配列作成が少し異なる
    std::shared_ptr<int[]> array{new int[10]};
    std::shared_ptr<int[]> old_array(new int[10]);
    // std::shared_ptr<int[]> array = std::make_shared<int[]>(10); // この書き方はC++20から

    // あとからセットしたい場合はreset()を使う
    {
        auto *t = new MyClass{15, 1.41};
        std::shared_ptr<MyClass> obj3;
        obj3.reset(t); // obj3.reset()も可能
        t->value = 5;
        std::cout << obj3->value << std::endl; // 5

    } // t(new MyClass{15, 1.41}で作ったやつ)はobj3が破棄してくれる。delete t不要

    // 取り扱い: unique_ptrと同じ
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

    // 共有：
    // unique_ptrと違い、複数のオブジェクトで共有できる
    // =演算子 : 所有者の追加
    {
        auto owner1 = std::make_shared<MyClass>(1, 2.0);

        // 現在の所有者数を出力できる
        // ※注意: マルチスレッド下では値を信用しないこと！
        std::cout << owner1.use_count() << std::endl; // 1

        // 共有パターン１：
        {
            // コピーにより所有者が追加される
            auto owner2 = owner1;
            // owner1とowner2は同じポインタを持つ
            std::cout << "address >> owner1:" << owner1.get()
                      << " owner2:" << owner2.get() << std::endl; // address >> owner1:0xZZZZZZ owner2:0xZZZZZZ
            std::cout << owner2.use_count() << std::endl;         // 2
        }                                                         // ここでowner2が破棄され、所有者数が -1 される（が、まだ活きている）

        std::cout << owner1.use_count() << std::endl; // 1

        // 共有パターン２：
        {
            // コンストラクタに渡すことでも所有者が追加される
            auto owner3 = std::shared_ptr<MyClass>(owner1);
            // owner1とowner3は同じポインタを持つ
            std::cout << "address >> owner1:" << owner1.get()
                      << " owner3:" << owner3.get() << std::endl; // address >> owner1:0xZZZZZZ owner3:0xZZZZZZ
            owner3->value = 5;
            std::cout << owner1->value << std::endl;              // owner1でも5が出力される
            std::cout << owner1.use_count() << std::endl;         // 2
        }                                                         // ここでowner3が破棄され、所有者数が -1 される（が、まだ活きている）

        std::cout << "address >> owner1:" << owner1.get() << std::endl; // address >> owner1:0xZZZZZZ

        // 共有パターン３：
        {
            auto ptr = std::make_shared<int>(42);
            func_with_shared_ptr(ptr);                                  // 関数に値として渡すと参照カウンタ+1
            func_with_shared_ptr_ref(ptr);                              // 関数に参照として渡すと参照カウンタはそのまま
            std::cout << "use_count: " << ptr.use_count() << std::endl; // 1
        }
    } // ここでowner1も所有者から外れ、保持していたMyClassオブジェクトは破棄される

    // 所有権の放棄： unique_ptrにはあるrelease()はない
    {
        // 所有権の強制解除１
        auto owner1 = std::make_shared<MyClass>(1, 2.0);
        std::cout << "address >> owner1:" << owner1.get() << std::endl;               // address >> owner1:0xZZZZZZ
        owner1.reset();                                                               // owner.reset(nullptr);と同じ
        std::cout << "address >> owner1:" << owner1.get() << std::endl;               // address >> owner1:0x0
        std::cout << "owner1 is " << (owner1 ? "active"s : "inactive"s) << std::endl; // owner1 is inactive

        // 所有権の強制解除２
        auto owner2 = std::make_shared<MyClass>(1, 2.0);
        std::cout << "address >> owner2:" << owner2.get() << std::endl;               // address >> owner:0xZZZZZZ
        owner2 = nullptr;                                                             // これでもOK
        std::cout << "address >> owner2:" << owner2.get() << std::endl;               // address >> owner2:0x0
        std::cout << "owner2 is " << (owner2 ? "active"s : "inactive"s) << std::endl; // owner2 is inactive
    }

    // レアケース
    // make_～によるstd::vectorの初期化について。unique_ptrも同じ
    {
        // 問題：次の処理は何をする処理でしょうか？
        // a) 10と20を要素に持つ配列の作成
        // b) 値が20となる要素を10個持つ配列の作成
        auto v_unique = std::make_unique<std::vector<int>>(10, 20);
        auto v_shared = std::make_shared<std::vector<int>>(10, 20);

        {
            // 正解はb。std::vector<int> a{10,20};ではない

            // → 10と20を要素に持つ配列のshared_ptrを作りたい場合は次のようにする
            auto initializer_list = {10, 20};
            auto v_shared = std::make_shared<std::vector<int>>(initializer_list);
        }
    }

    // 使用上の注意
    // レファレンスカウント方式あるある：循環参照
    {
        struct Sample
        {
            std::shared_ptr<Sample> ref;
            ~Sample()
            {
                std::cout << "destructed!" << std::endl; // 呼ばれない
            }
        };

        auto s1 = std::make_shared<Sample>();
        auto s2 = std::make_shared<Sample>();
        s1->ref = s2;
        s2->ref = s1;
    } // 本来ここでSampleのデストラクタが呼ばれるはずだが、呼ばれない!!
    // 解消するためには、次に示すstd::weak_ptrを使う必要がある

    // 使い所
    // - 大きなオブジェクト
    // - マルチスレッド間で共有
    // - その他
    // 関数引数に使ったときに参照カウンタは増える？
    // - 値として渡す場合は増える。引数で渡すときにコピーコンストラクタが呼ばれるため
    // - 参照/ポインタとしてスマートポインタを渡すと増えない
}

void learn_shared_ptr_extra()
{
    // unique_ptrからshared_ptrにする
    // shared_ptrからunique_ptrはしないほうが良い（設計の問題）

    // １つのポインタに対して異なるグループのshared_ptrを作らない
}

void learn_weak_ptr()
{
    // 問題
    // 循環参照せざるを得ない場合にどうする？
    // 所有者の都合で破棄したときに、参照している人は破棄されたことをどうやって確認する？

    shared_ptr<aircraft> wingMan = pMaverick->myWingMan.lock();
    if (wingMan)
    {
        cout << wingMan->m_flyCount << endl;
    }
}

// scoped/autoは使わない

void func_with_unique_ptr(std::unique_ptr<int> ptr)
{
}

void func_with_unique_ptr_ref(std::unique_ptr<int> &ptr)
{
}

void func_with_shared_ptr(std::shared_ptr<int> ptr)
{
    std::cout << "shared_ptr in function: " << ptr.use_count() << std::endl; // +1した値が出力される
}

void func_with_shared_ptr_ref(std::shared_ptr<int> &ptr)
{
    std::cout << "shared_ptr& in function: " << ptr.use_count() << std::endl; // 参照なので+1されない
}
