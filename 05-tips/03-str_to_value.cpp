// 数値と文字列の相互変換
// * https://speakerdeck.com/cpp/cedec2020?slide=107
// * C++17

#include <iostream>
#include <charconv>

int main()
{
    //
    // 1) 文字列→数値
    //
    std::cout << "1) chars -> value" << std::endl;

    // 与えられたイテレータ範囲[first, last)内の文字列からパターンにマッチする
    // "最初の数字文字列"を見つけて、数値へ変換する。
    // 変換にはメモリ確保を行わず、例外も投げない

    // 結果(from_chars_result型)の見方。from_chars_result型はconst char *ptrとerrc ecの２つを持つ
    // * 成功した場合
    //   - const char *ptr : 指定されたパターンに一致しなかった最初の文字の位置。全てが一致した場合はptr == last
    //   - errc ec : ec == errc{}
    // * パターンにマッチする文字列が見つからない場合
    //   - ptr : ptr == first
    //   - ec : ec == errc::invalid_argument
    // * 変換した結果の値がvalueの型では表現できない場合
    //   - ptr : 指定されたパターンに一致しなかった最初の文字の位置
    //   - ec : ec == errc::result_out_of_range
    {
        const char str[] = "00000123456789 is decimal"; // 先頭に空白があったりすると失敗するので注意
        int value{};

        //(1) 10進数文字列からintへ変換
        if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value); ec == std::errc{})
        {
            std::cout << value << std::endl; // 123456789
        }
    }

    {
        const char str[] = "1111111111111111 is (65535)_10";
        int value{};

        //(1) 2進数文字列からintへ変換
        if (auto [ptr, ec] = std::from_chars(std::begin(str), std::end(str), value, 2); ec == std::errc{})
        {
            std::cout << value << std::endl; // 65535
        }
    }

    //
    // 2) 数値→文字列
    //
    std::cout << "2) value -> chars" << std::endl;

    // 与えられた数値（value）を文字列へ変換し、[begin, end)内へ出力する
    {
        char out[50]{};
        auto begin = std::begin(out);
        auto end = std::end(out);

        //(1) 10進数文字列へ変換
        if (auto [ptr, ec] = std::to_chars(begin, end, 10); ec == std::errc{})
        {
            std::cout << std::string_view(begin, ptr - begin) << std::endl; // 10
        }
        //(1) 2進数文字列へ変換
        if (auto [ptr, ec] = std::to_chars(begin, end, 65535, 2); ec == std::errc{})
        {
            std::cout << std::string_view(begin, ptr - begin) << std::endl; // 1111111111111111
        }
    }
    return 0;
}