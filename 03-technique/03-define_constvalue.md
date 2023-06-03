# 定数を定義するときのポイント

## コツ

* インターフェースと実装とを分ける
    * ヘッダファイルに直接値を書かない
    * ヘッダファイルには `extern const int FOOBAR;` と書いておく
        * .cppファイルに `const int FOOBAR = 4649;` と書く

## 実現方法

### 実現方法1

ヘッダファイルで`extern`宣言をする
```cpp
// my_library.hファイル
#include <string>

namespace MyLibrary {
    extern const int FooValue;
    extern const int MaxNameLength;
    extern const std::string LOG_FILENAME;
}

// my_library.cppファイル
#include "my_library.h"

namespace MyLibrary {
    const int FooValue = 42;
    const int MaxNameLength = 255;
    const std::string LOG_FILENAME;
}
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

// my_library.cppファイル
#include "my_library.h"

const int MyLibrary::FooValue = 42;
const int MyLibrary::MaxNameLength = 255;
const std::string MyLibrary::LOG_FILENAME;
```

## メリット

* ビルド時間を短くできる
    * ヘッダファイルの更新 → includeしている全ファイルで再ビルドが走る
    * cppファイルだけなら当該ファイルの再コンパイルだけでOK
* バイナリサイズが小さくなる(塵も積もれば…)
    * 各objファイルが共通の値を見に行くため

## デメリット

* 宣言と実装を分けると定数表現を評価できなくなる

```cpp
// other.cpp
#include "my_library.h"

char name1[MyLibrary::MaxNameLength]; // コンパイルエラー！値が分からない
```

