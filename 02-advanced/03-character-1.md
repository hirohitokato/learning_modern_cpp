# 文字コード

## 1. C++の文字列型と文字コード

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

### 符号化方式(エンコーディング/Encoding)

文字集合を実際のバイト列/データに変換する方式。古い環境との互換性やデータサイズを考慮しつつ決められる。

* 代表例: ShiftJIS, UTF-8, UTF-16

## ソースファイルからはじまるエンコーディング指定

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

MSVCの場合、UTF-8で保存する際にファイルにBOMをつければ認識してくれる・・・が、BOMをつけるのはあまり好きではない(個人の意見です)。


* ファイル
    * CP932
    * UTF-8
* 文字列
    * string
    * wstring
    * u8string
    * ATL::CString
    * LPCSTR
    * LPWSTR

## Windowsの文字列

### Unicodeとマルチバイト文字

VC++等では、

* ユニコード文字列を処理する文字をワイド文字と呼び、wchar_t型で表す
    * MSVCではwchar_tのサイズは2、UTF-16で格納
    * macOS, linuxではwchar_tのサイズは4、UTF-32で格納
* ユニコード以前のShift_JISやascii文字をマルチバイト文字と呼び、char型で表す



## TCHAR型(MSVCのみ)

TCHAR、LPCTSTRやLPTSTRなどのTCHAR系列の型は、ソースコードをユニコードと非ユニコード(=マルチバイト)のどちらにも対応できるようにするための型。

```cpp
#ifdef UNICODE
	typedef WCHAR    TCHAR;
#else // MBCS(Multi-Bytes CharSet)
	typedef char     TCHAR;
#endif
```

となっていてコンパイル時に「UNICODE」が定義されているとTCHARはWCHAR、定義されていないとTCHARはchar型として定義される。

|型名|MBCSでの実際の型|UNICODE定義時の実際の型|
|---|---|---|
|TCHAR|char|WCHAR(wchar_t)|
|LPCTSTR|const char*|const WCHAR*|
|LPTSTR|char*|WCHAR*|

```cpp
// 以下は同じ文
const char* str = "日本語";
LPCSTR str = "日本語";
```

```cpp
// 以下は同じ文
const wchar_t* wstr  = L"ワイド文字";
LPCTSTR wstr = L"ワイド文字";
```

参考: https://www.usefullcode.net/2006/11/tcharlpctstrlptstr.html

## その他補足

### ATL(Active Template Library)

コンポーネントオブジェクトモデル (COM) オブジェクトのプログラミングを簡素化する、テンプレートベースの C++ クラスのセット。  
(COMは、Windowsでのソフトウェアコンポーネントの作成および使用を目的としたバイナリ仕様)