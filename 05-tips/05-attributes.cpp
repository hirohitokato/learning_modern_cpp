// 属性を使った分かりやすいAPI開発
// * C++11~C++17

#include <iostream>

// 「属性」とは
// 型や変数、翻訳単位のようなプログラムの構成要素に対して追加情報を指定したいときに使う。
// （Pythonのannotationに似ているが、独自の属性を定義することはできない）


// 1. deprecated属性(C++14)
//
// 提供した関数や変数、型の仕様が非推奨であることを表す。
// バージョンが古く、将来サポートされなくなる可能性があるものに使う
//
// （…ただし、MSVC++だと警告レベルを最大にしないと警告が出てくれないみたいで辛い）

int OLD_CONSTANT_VALUE [[deprecated]] = 0xDEADBEEF; // 変数の非推奨化
struct [[deprecated]] OldClass {};                  // 型の非推奨化

[[deprecated]] // 関数の非推奨化
void old_function() {}
[[maybe_unused]]
[[deprecated("この関数はもう使われません。代わりにnew_func()を使ってください")]] // 関数の非推奨化(属性には独自の警告メッセージもつけられる)
void old_function2() {}


// 2. maybe_unused属性(C++17)
//
// 変数や関数を意図的に使用しない可能性があることを開発者やコンパイラに伝える。
// 未使用の変数がある旨の警告を抑制でき、場合によってはコンパイル時最適化のヒントにもなる
//
// 型や変数、メンバー変数、関数、enumやその列挙子に使えるが、下のように変数に使うことが多い(個人の感想)
//
// （…ただし、MSVC++だと警告レベルを最大にしないと警告が出てくれないみたいで辛い）
int function_without_attribute(int a, int b) // コンパイル時警告：'b': 引数は関数の本体部で 1 度も参照されません。
{
    return a + 1; // bを使っていない
}

int function_with_attribute(int a, [[maybe_unused]] int b)
{
    return a + 1; // bを使っていないが、警告が出ない
}


// 3. nodiscard属性(C++17)
//
// 関数の戻り値を無視してはならないことを開発者やコンパイラに伝える
// ヒープにメモリを確保して返すような処理を作った場合に、必ず返り値を保持しないといけないことを明示できる
//
// 関数の戻り値以外にも、クラスや列挙型の宣言、コンストラクタの宣言に使える

[[nodiscard]]
int* function_returns_pointer()
{
    return new int{193};
}


int main()
{
    // [[deprecated]]
    {
         [[maybe_unused]] OldClass old_a; // コンパイル時警告: 'OldClass': が古い形式として宣言されました。
        old_function2(); // コンパイル時警告: 'old_function2': この関数はもう使われません。代わりにnew_func()を使ってください
    }

    // [[maybe_unused]]
    {
        function_without_attribute(1,2); // 警告が関数本体側で出る
        function_with_attribute(1,2);
    }

    // [[nodiscard]]
    {
        function_returns_pointer();          // コンパイル時警告: 'nodiscard' 属性を持つ関数の戻り値を破棄しています
        [[maybe_unused]] auto x = function_returns_pointer(); // OK。(maybe_unusedによりx未使用の警告を抑制している)
    }

    return 0;
}


/*
他にも以下があるが割愛

- [[noreturn]] : 関数が決して返らないことをコンパイラに伝える(C++11)
- [[carries_dependency]] : データの依存性を持たせる or 維持する(C++11)
- [[fallthrough]] : switch-caseでbreakを書かないときに意図したフォールスルーであることをコンパイラに伝える(C++17)
- [[no_unique_address]] : 非静的メンバ変数に対して指定し、そのメンバがユニークなアドレスを持つ必要がないことをコンパイラに伝える(C++20)
- [[likely]], [[unlikely]] : if,switchなどの分岐制御構文で、条件分岐先のうちどれが実行される可能性が高いかを最適化ヒントとしてコンパイラに伝える(C++20)
- [[assume]] : 式が指定された時点で 常にtrueと評価されることを最適化ヒントとしてコンパイラに伝える(C++23)

*/


// 質問: 属性を複数同時に指定したい場合は？
// → `[[nodiscard, deprecated]]` または `[[nodiscard]][[deprecated]]`の２通りの方法がある。行が変わってもOK
// → 参考URL: https://en.cppreference.com/w/cpp/language/attributes
