# includeの順序のガイドライン

## そもそもincludeは何をしているのか

* 書いた場所にそのファイル内容をコピペしているだけ
    * コンパイラによるコンパイル前の、プリプロセッサの仕事
    * プリプロセッサは「コメントの削除」「`#`から始まるプリプロセッシングディレクティブの処理」の２つを行う
        * `#include …`は後者の仕事
* だからこんな書き方もできる

```cpp
//  data.csvの中身
1,2,3,4

// main.cpp
#include <vector>

int main() {
    std::vector<int> array {
        #include "data.csv"
    };
    return 0;
}
```

### `#include<〜>`と`#include ""`の違い

どのディレクトリを探すかの違い

|ディレクティブ|ディレクトリの検索順序|
|`#include<〜>`|処理系指定のインクルードパスからのみ検索|
|`#include ""`|まずカレントディレクトリを検索、次に処理系指定のインクルードパスを検索|

* 処理系指定のインクルードパス: Visual Studioだと「プロジェクトプロパティ」→「C/C++」→「追加のインクルードディレクトリ」

### インクルードガード

`#ifndef/#define/#endif`を組み合わせることで、複数のファイルから同一のヘッダファイルをインクルードしても二重定義にならないで済む「インクルードガード(#define guard)」が実現できる。

```cpp
#ifndef FOO_BAR_BAZ_H_
#define FOO_BAR_BAZ_H_

...

#endif  // FOO_BAR_BAZ_H_
```

* 今の大抵のコンパイラでは`#pragma once`が使えるのでそちらを使った方が簡単で安全
    * 上の`FOO_BAR_BAZ_H_`が別ファイルでも同じ名前で定義されているかもしれない

```cpp
// 上と同じ機能(＆より安全)
#pragma once

...
```

## includeの順序/書き方

以下は[Google C++ スタイルガイド](https://ttsuki.github.io/styleguide/cppguide.ja.html)を参考にしている。

### `.h`ファイル

* ヘッダファイルだけでコンパイルができるよう、自己完結型にしておくこと
    * → ヘッダファイルに書かれている型は、すべてヘッダファイルでincludeする

[Google C++ スタイルガイドの「自己完結型ヘッダー」項](https://ttsuki.github.io/styleguide/cppguide.ja.html#Self_contained_Headers)によると、ヘッダファイル(.h)におけるインクルードの順序は以下を推奨している。


### `.cpp`ファイル

[Google C++ スタイルガイドの「インクルードの名前と順序」項](https://ttsuki.github.io/styleguide/cppguide.ja.html#Names_and_Order_of_Includes)によると、ソースコード(.cpp)におけるインクルードの順序は以下を推奨している。

1. 紐付くヘッダー(`myclass.cpp`なら`myclass.h`)
2. C言語のシステムヘッダー(`cstdio`, `cmath`など)
3. C++標準ライブラリヘッダー(`vector`, `string`など)
4. その他のライブラリのヘッダー(`boost/asio`, `sqlite3.h`など)
5. 自分のプロジェクト内のヘッダー(`otherclass.h`など)

それぞれのグループの間には空行を1行入れると良い。

```cpp
#include "myclass.h"

#include <cstdio>
#include <cmath>

#include <vector>
#include <string>

#include "boost/asio"
#include "sqlite3.h"

#include "app_entity.h"
#include "otherclasss.h"

...
```

この順番にしておけば、

* 上で記した自己完結型のヘッダーでないときにコンパイルエラーで気付ける
    * なんのファイルでエラーになったかがわかりやすくなる

というメリットが得られる。

## 必要なヘッダを直接インクルードする

* 必要なヘッダファイルをできるだけ明確に、冗長で良いのでインクルードする
    * 他ファイルを経由してインクルードしない
    * ヘッダーファイルの作者がインクルードを除外してしまうかもしれないため
