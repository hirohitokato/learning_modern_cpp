/// 大きさが規定されている整数型

#include <cstdint>
#include <iostream>

using namespace std;

int main() {
    {
        // int8/16/32/64_t, uint8/16/32/64_t
        // - 指定したビット長ちょうどのサイズを持つ整数型
        int8_t a; // 未初期化で宣言
        int32_t b{}; // 値ゼロで初期化
        uint64_t c{193}; // 193で初期化
        cout << "int8_t:   [" << a << "] / size:" << sizeof(a) << endl;
        cout << "int32_t:  [" << b << "] / size:" << sizeof(b) << endl;
        cout << "uint64_t: [" << c << "] / size:" << sizeof(c) << endl;
    }
    {
        // int_least8/16/32/64_t, uint_least8/16/32/64_t
        // - 少なくともそのビット長以上あることが保証されている整数型

        // int_fast8/16/32/64_t, uint_fast8/16/32/64_t
        // - 少なくともそのビット長以上あることが保証されていて、もっとも高速に演算できる整数型
    }
    {
        // intmax_t, uintmax_t
        // - 処理系で最大の整数型。M2 Macだと64bit(８バイト)
        cout << "sizeof(intmax_t): " << sizeof(intmax_t) << endl;
    }
    {
        // intptr_t, uintptr_t
        // - 以下を保証するポインタサイズの整数型。
        // 1. 有効なvoidへのポインタから(u)intptr_t型への変換
        // 2. (u)intptr_t型のポインタ値からvoidへのポインタへの逆変換
        // 3. 変換前と逆変換のポインタ値が等値となる

        // 次の用途に利用できる。
        // - ポインタ値を、一意なIDとして使用する
        //   - ポインタ値はオブジェクトごとに一意であるため
        // - コールバック関数に渡す引数として、任意のオブジェクトを設定する

        // サンプルコード： https://cpprefjp.github.io/reference/cstdint/intptr_t.html
        int value = 42;
        // value変数へのポインタを、整数値として保持する
        intptr_t pointer_value = reinterpret_cast<intptr_t>(&value);
        cout << std::hex << "0x" << pointer_value << endl;
    }
}