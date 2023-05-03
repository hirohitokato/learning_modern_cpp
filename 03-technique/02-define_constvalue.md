# 定数を定義するときのポイント

## コツ

* ヘッダファイルに直接値を書かない
* ヘッダファイルには `extern const int FOOBAR;` と書いておく
    * .cppファイルに `const int FOOBAR = 4649;` と書く

```cpp
// .hファイル

namespace MyLibrary {
    extern const int FooValue;
}
```

```cpp
// .cppファイル

namespace MyLibrary {
    const int FooValue = 42;
}
```

## 理由

* ビルド時間を短くする
    * ヘッダファイルの更新 → includeしている全ファイルで再ビルドが走る