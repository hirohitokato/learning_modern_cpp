# 文字コードとC++

## 0. (基礎知識)文字集合と符号化方式

### 2.1. 符号化文字集合(Character Set)

各文字にユニークな番号を振ったもの。ISO/IEC 646だと7bitで128の符号位置(=管理可能な文字数)、Unicodeの文字集合の符号空間は0～10FFFF16で111万4,112の符号位置を持つ。

* 代表例: ASCII(ISO/IEC 646), Latin-1(ISO/IEC 8859-1), JIS X 0208, Unicode

※今回のトピックでは符号化文字集合は取り扱わない。

### 2.2. 符号化方式(エンコーディング/Encoding)

文字集合を実際のバイト列/データに変換する方式。古い環境との互換性やデータサイズを考慮しつつ決められる。

* 代表例: ASCII, Shift_JIS, UTF-8, UTF-16, UTF-32, EUC-JP

JIS X 0208の符号化方式がShift_JIS、Unicodeの符号化方式がUTF-8, UTF-16, UTF-32。

Windowsでいう「コードページ」はエンコーディングのこと（と思っておけばだいたい合ってる）。

### 2.3. 「文字コード」とは？

普段の会話や記事でよく出てくる「文字コード」は広い意味（＝曖昧な意味）で使われる。

* 文字集合
* 符号化方式

のどちらの意味で使われることもあるので、自分が話すときにはどちらの意味で使っているかを意識すること。

## 1. C++で登場する文字列型とエンコーディング

### 1.1. C++標準の文字型

各エンコーディングの文字(≠文字列)を扱えるようにするための型が用意されている。

|型|説明|バージョン|
|---|---|---|
|char|昔からある文字型|-|
|wchar_t|ワイド文字を扱うための型|-|
|char8_t|UTF-8文字を表すときの型|C++20|
|char16_t|UTF-16文字を表すときの型|C++11|
|char32_t|UTF-32文字を表すときの型|C++11|

C++17でUTF-8を扱うときは`char`を使わなければならない（が問題が多すぎるので使いたくない）。

#### [memo] `wchar_t`型？

文字数が増えて１バイトでは文字を表せなくなったときに登場した型。複数バイトで１文字を表現することを狙って作られた。

この型で扱う文字を「ワイド文字」という。使い分けは以下のとおり。

* UTF-XXなUnicodeを出自とするエンコーディングを処理する文字を**ワイド文字**と呼び、wchar_t型で表す
    * MSVCではwchar_tのサイズは**2**、**UTF-16**で格納
    * macOSやLinuxではwchar_tのサイズは**4**、**UTF-32**で格納
    * Windowsではこれを「ユニコード文字」と言う（紛らわしい）
* ユニコード以前のShift_JISやASCII文字はchar型で表す
    * Windowsではこれを「マルチバイト文字」という。1バイトか2バイトなので、とのこと（紛らわしい！）

Windowsが言うユニコードは文字集合のUnicodeそのものとは異なり、UTF-XXなエンコーディングを表しているので注意。

最初はこの２つだけで済むと思っていたが、そうでは無いことが分かりここから`char16/32_t`や`char8_t`が追加されていったという歴史。

### 1.2. C++標準の文字列型

|型|説明|エンコーディング|
|---|---|---|
|std::string|char型の文字列|規定なし|
|std::wstring|wchar_t型の文字列|規定なし
|std::u8string|char8_t型の文字列(C++20)|UTF-8|
|std::u16string|char16_t型の文字列(C++11)|UTF-16|
|std::u32string|char32_t型の文字列(C++11)|UTF-32|

いずれも`string`ヘッダのインクルードで使用可。

C++17でUTF-8を扱うときは`std::string`を使う（が問題が多すぎるので使いたくない）。

#### [memo] 各エンコーディングのリテラル表現/出力ストリーム

|エンコーディング|リテラル表現|標準出力|
|---|---|---|
|-|`std::string s = "ABC"s`|`std::cout`|
|(ワイド文字)|`std::wstring s = L"ABC"`|`std::wcout`|
|UTF-8|`std::u8string s = u8"ABC"`|なし|
|UTF-16|`std::u16string s = u"ABC"`|なし|
|UTF-32|`std::u32string s = U"ABC"`|なし|

**u8/16/32stringはそのままでは標準出力に出力できない**ので、出力したい場合は変換ライブラリを経由するか、あるいはあきらめてデータとして使うしかない。  
（将来、`std::u8cout`みたいなのが出てくるかも…）

**注意**: 変換処理を行うcodecvtが標準ライブラリにあるが、[あまりにお粗末な設計](https://cpprefjp.github.io/reference/codecvt.html)のためC++17で非推奨になった。他のサードベンダーのライブラリを使うこと。


## 2.Windowsの(面倒くさい)C++の世界

### 2.1.Windows独自の文字型マクロ「TCHAR」

TCHAR/LPCTSTR/LPTSTRなどのTCHAR系列の型は、ソースコードをユニコードと非ユニコード(=マルチバイト)のどちらにも対応できるようにするための型。

```cpp
#ifdef _UNICODE
	typedef WCHAR    TCHAR;
#else // MBCS(Multi-Bytes CharSet)
	typedef char     TCHAR;
#endif
```

となっていてコンパイル時に「UNICODE」が定義されているとTCHARはWCHAR、定義されていないとTCHARはchar型として定義される。

TCHARは他にもいくつかバリエーションがある。中身はC++標準の型なので、ややこしい。

|型名|MBCSでの実際の型|UNICODE定義時の実際の型|
|---|---|---|
|TCHAR|char|WCHAR(wchar_t)|
|LPCTSTR|const char*|const WCHAR*|
|LPTSTR|char*|WCHAR*|

```cpp
// 以下はマルチバイト文字のときに同じ文
const char* str = "日本語";
LPCSTR str = "日本語";
```

```cpp
// 以下はユニコード文字のときに同じ文
const wchar_t* wstr  = L"ワイド文字列";
LPCTSTR wstr = L"ワイド文字列";
```

参考: https://www.usefullcode.net/2006/11/tcharlpctstrlptstr.html

#### UNICODE/マルチバイトを意識しない文字列リテラル

Windowsでは`_T("～")`という文字列リテラルを表すためのマクロがあり、これで普段から統一して書いておくと、マルチバイト文字でビルドするかUNICODEでビルドするかをコンパイル時のマクロで決められる。

```cpp
// コンパイル時にUNICODEが定義されていればwchar_t*型、そうでなければchar*型になる
const auto* tstr  = _T("ワイド文字列かユニコード文字列");
// 以下は同じ文
#ifdef _UNICODE
    const wchar_t* tstr  = L"ワイド文字列かユニコード文字列";
#else // MBCS(Multi-Bytes CharSet)
    const char_t* tstr  = "ワイド文字列かユニコード文字列";
#endif
```

**TODO**: MBCSとSBCSについてあとで加筆。正確には`#else`ではさらにMBCSとSBCSの２つに分かれていて、`_UNICODE`、`_MBCS`ともに未定義の場合はSBCS(Single Byte Character Set)になる。

### 2.2.Windows(ATL)独自の文字列型

WindowsのATL(Active Template Library)なるライブラリは、独自の文字列型 `CString` を用意している。

これは文字の管理にchar型とwchar_t型のどちらを使っているか意識しなくても済むよう設計された型。

CStringは_UNICODEマクロが有効なときにCStringの実体はCStringWとなり、無効なときにはCStringAが実体で使われる。

```cpp
// 正確ではない、概念説明のためのコード
#ifdef _UNICODE
	typedef CString    CStringW;
#else // MBCS(Multi-Bytes CharSet)
	typedef CString    CStringA;
#endif
```

---

## 3. エンコーディングを取り巻く環境

３つの要素がある。

* ソースファイルのエンコーディング
* 実行ファイルのエンコーディング
* 実行環境(ロケール)

### 3.1. ソースファイルのエンコーディング

VC++2023のcl.exeはソースファイルがSJISで書かれている・SJISのエンコーディング環境で実行することを想定したコンパイル処理が行われる。

つまり、デフォルトでは**UTF-8, UTF-16で保存したファイルをコンパイルできない**＆**UTF-8等の出力を期待した環境では文字化けする**

MSVC(cl.exe)やGCC,Clangにはソース文字セット(入力)と実行文字セット(出力)を指定するオプションがある。

|オプション|MSVC|GCC|
|---|---|---|
|ソースファイルのエンコーディング指定|`/source-charset:utf-8`|`-finput-charset=utf-8`|
|実行時に使う文字列のエンコーディング指定|`/execution-charset:utf-8`|`-fexec-charset=cp932`|
|入出力のエンコーディング同時指定(UTF-8の場合)|`/utf-8`|-|

**使用例:**

ソースファイルに書かれた文字列リテラルはUTF-8として解釈してコンパイルし、
コンパイル後のバイナリ中ではSJISに変換された文字列として定義する場合：

* `cl.exe /source-charset:utf-8 main.cpp`
* `gcc -finput-charset=utf-8 -fexec-charset=cp932 sample.cpp`

MSVCの場合、UTF-8で保存する際にファイルにBOMをつければ自動認識してくれるらしい・・・が、BOMをつけるのはあまり好きではない(個人の意見です)。

### 3.2. 実行環境(コマンドプロンプトやターミナル)

### 3.3. 出力ストリーム

出力ストリーム(標準出力やファイルストリームなど)でstd::coutとstd::wcoutを混在させると、あとから呼んだ方は出力されなくなる。

`byte-oriented`モードと`wide-oriented`モードのどちらかのモードを持ち、最初に呼んだ方で固定されるため。  
以下のどちらも後者が表示されない。

1. std::cout/printf()を呼んだあとにstd::wcout/wprintf()を呼ぶ
2. std::wcout/wprintf()を呼んだあとにstd::cout/printf()を呼ぶ



## その他補足

### ATL(Active Template Library)

コンポーネントオブジェクトモデル (COM) オブジェクトのプログラミングを簡素化する、テンプレートベースの C++ クラスのセット。  
(COMは、Windowsでのソフトウェアコンポーネントの作成および使用を目的としたバイナリ仕様)