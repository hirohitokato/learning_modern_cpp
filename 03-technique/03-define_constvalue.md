# 定数を定義するときのポイント
# 参考書籍： マーティン・レディ著、ホジソンますみ訳『C++のためのAPIデザイン』

![https://www.sbcr.jp/product/4797369151/](https://www.sbcr.jp/wp-content/uploads/product/4797369151-480x596.jpg)

パブリック/インターナルな定数を定義し、コード中に値のハードコードを避けたいときのポイント。

```cpp
// これらをどう書くか
#define SOMETHING_CONSTANT (42)
const int FOO_MESSAGE_SIZE = 64;
```

* まずは`#define`による定義を避けられないか考える
* `extern`で宣言と定義とを分ける
    * ヘッダファイルに直接値を書かない
    * ヘッダファイルには `extern const int FOOBAR;` と書いておく(宣言)
        * .cppファイルに `const int FOOBAR = 4649;` と書く(定義)

## 【課題】`#define`の問題点

便利なんだけれど・・・

* ただ置き換えるだけなので危険がいっぱい
* ライブラリの場合、実装の詳細を漏らしてしまうことになる
* 使用している変数の型が分からない・型チェックもない
* ただの置き換えなのでデバッグ時のシンボル情報が残らない
* namespaceに入れられないのでグローバル名前空間を汚染しまくる

## 【解決策】constを使った方法

### 実現方法1

ヘッダファイルで`extern`宣言をする。

```cpp
// my_library.hファイル
#include <string>

// namespaceに入れておくと名前空間を汚染しない
namespace MyLibrary {
    extern const int FooValue;
    extern const int MaxNameLength;
    extern const std::string LOG_FILENAME;
}

//-------------------------

// my_library.cppファイル
#include "my_library.h"

namespace MyLibrary {
    const int FooValue = 42;
    const int MaxNameLength = 255;
    const std::string LOG_FILENAME = "Hello!"s;
}

//-------------------------

// main.cppファイル
  ：
using namespace MyLibrary;
auto value = FooValue;
std::cout << LOG_FILENAME << std::endl;
  ：
```

### 実現方法2

namespaceの代わりにクラスの静的変数として宣言する。

```cpp
// my_library.hファイル
#include <string>

class MyLibrary {
public:
    static const int FooValue;
    static const int MaxNameLength;
    static const std::string LOG_FILENAME = "Hello!"s;
};

//-------------------------

// my_library.cppファイル
#include "my_library.h"

const int MyLibrary::FooValue = 42;
const int MyLibrary::MaxNameLength = 255;
const std::string MyLibrary::LOG_FILENAME;

//-------------------------

// main.cppファイル。使い方は同じ
  ：
auto value = MyLibrary::FooValue;
std::cout << MyLibrary::LOG_FILENAME << std::endl;
  ：
```

クラスの静的変数にした場合は `using namespace MyLibrary` と省略させられなくなるので、namespaceとどちらにするかはケースバイケース。

## 宣言と実装を分けるメリット

* ビルド時間を短くできる
    * ヘッダファイルの更新 → includeしている全ファイルで再ビルドが走る
    * cppファイルの更新だけなら同cppファイルの再コンパイルだけでOK
* バイナリサイズが小さくなる(塵も積もれば…)
    * 各objファイルが共通の値だけれど、各ファイルで実体が定義され、それを見に行くことになるため

## 宣言と実装を分けるデメリット

* 宣言と実装を分けると定数表現を評価できなくなる

```cpp
// other.cpp
#include "my_library.h"

// コンパイルエラー！値が分からない
char name1[MyLibrary::MaxNameLength];

const int SIZE = 10;
char name2[SIZE]{}; // 定義が見えている状態ならOK
```

## その他のアイデア

* お互いに独立した数値で、同じグループにあるものは`enum`で定数定義する。

```cpp
// Not good
#define ALIGNMENT_CENTER 0
#define ALIGNMENT_LEFT   1
#define ALIGNMENT_RIGHT  2

// better
enum Alignment: int {
    Center,
    Left,
    Right,
};
```

* `constexpr`/`consteval`や`inline`変数の使用も考える
    * 宣言と実装を分けることはできないけれど、今は積極的にconstexpr/constevalを使っていくのがスタイル
    * とりあえずは`const`を`constexpr`に変えてみることから慣れてみよう

```cpp
namespace MyLibrary {
    // constexpr(C++20はconstevalも)を付ける
    constexpr int GetSize(int count) { return count * 3; }
}

int main() {
    using namespace MyLibrary;

    // 関数の戻り値を定数定義に使える
    char myarray[GetSize(3)]{};
    std::cout << sizeof(myarray) / sizeof(char) << std::endl; // 9

    return 0;
}
```

## 補足

* `<limits>`ヘッダによって使える`std::numeric_limits`では、constexprを使った定数表現を駆使しています。一度見てみると面白いかも
* `#define`が完全な悪というわけではないので（シンプル/手っ取り早い）、棲み分けができるようにしておくと良い
