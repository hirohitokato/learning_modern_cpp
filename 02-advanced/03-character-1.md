# 文字コード

## C++の文字列型と文字コード

|型|説明|文字コード(符号化方式)|
|---|---|---|
|std::string|char型の文字列|規定なし|
|std::u8string|char8_t型の文字列|UTF-8|
|std::u16string|char16_t型の文字列|UTF-16|
|std::u32string|char32_t型の文字列|UTF-32|
|std::wstring|wchar_t型の文字列|規定なし

## 文字集合と符号化方式

### 文字集合(Character Set)

各文字にユニークな番号を振ったもの。Unicodeの文字集合の符号空間は0～10FFFF16で111万4,112の符号位置を持つ。

* 代表例: ASCII, JIS, Unicode

### 符号化方式(Encoding)

文字集合を実際のバイト列/データに変換する方式。古い環境との互換性やデータサイズを考慮しつつ決められる。

* 代表例: ShiftJIS, UTF-8, UTF-16

## cl.exe(VC++コンパイラ)

VC++2023のcl.exeはソースファイルがSJISで書かれている・SJISのエンコーディング環境で実行することを想定したコンパイル処理が行われる。

* → UTF-8, UTF-16で保存したファイルをコンパイルできない

cl.exeにはソース文字セット(入力)と実行文字セット(出力)を指定するオプションがある。

* `/source-charset:utf-8`: ソースコードがUTF-8で書かれているとみなしてコンパイルする
* `/execution-charset:utf-8`: 実行時に使う文字列をUTF-8として出力する
* `/utf-8`: `source-～`と`execution-～`の同時指定

たとえば`/source-charset:utf-8`だけを指定した場合、ソースに書かれた文字列リテラルはバイナリの中ではSJISに変換され定義されることになる。

* 厳密にはUTF-8で保存する際にBOMをつければ認識してくれる・・・が、使わないよね


* ファイル
    * CP932
    * UTF-8
* 文字列
    * string
    * wstring
    * u8string
