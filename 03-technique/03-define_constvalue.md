# 定数を定義するときのポイント

* まずは`#define`による定義を避けられないか考える
* `extern`で宣言と定義とを分ける
    * ヘッダファイルに直接値を書かない
    * ヘッダファイルには `extern const int FOOBAR;` と書いておく(宣言)
        * .cppファイルに `const int FOOBAR = 4649;` と書く(定義)

## `#define`の問題点

便利なんだけれど・・・

* ただ置き換えるだけなので危険がいっぱい
* 変数マクロの場合、型が分からない
* namespaceに入れられないのでグローバル名前空間を汚染しまくる

## constを使った方法

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
    const std::string LOG_FILENAME;
}

//-------------------------

// main.cppファイル
  ：
auto value = MyLibrary::FooValue;
std::cout << MyLibrary::LOG_FILENAME << std::endl;
  ：
```

### 実現方法2

クラスの静的変数として宣言する。

```cpp
// my_library.hファイル
#include <string>

class MyLibrary {
public:
    static const int FooValue;
    static const int MaxNameLength;
    static const std::string LOG_FILENAME;
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

## メリット

* ビルド時間を短くできる
    * ヘッダファイルの更新 → includeしている全ファイルで再ビルドが走る
    * cppファイルの更新だけなら同cppファイルの再コンパイルだけでOK
* バイナリサイズが小さくなる(塵も積もれば…)
    * 各objファイルが共通の値を見に行くため

## デメリット

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

* `constexpr`/`consteval`や`inline`変数の使用も考える
    * 宣言と実装を分けたときの定数表現には対応していないけれど、今後の標準になるので
    * とりあえずは`const`を`constexpr`に変えてみよう

### 補足

`<limits>`ヘッダによって使える`std::numeric_limits`では、constexprを使って定数表現の評価もできるようになっているようす。しかし自分のコードでの実現方法が分かりませんでした・・・。
