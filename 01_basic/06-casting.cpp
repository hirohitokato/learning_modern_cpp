// キャスト
// - 変数を表す型を変換して扱うときに用いる

// C++のキャストはCスタイルのキャストを加えると以下の５つが使える。
//
// * Cスタイルのキャスト: 安全性のチェックを伴わない、明示的な型の変換。実行時判定を行わない
// * static_cast: intからfloatへの変換のように、明示的に型を変換。実行時判定を行わない
// * dynamic_cast: クラスの実行時型情報(RTTI)を利用して、参照またはポインタの方を変換。実行時判定を行う
// * const_cast: const修飾/volatile修飾を変更
// * reinterpret_cast: ポインタもしくは整数値を任意の型に変換
// * bit_cast: 値のビットパターンを維持したまま別の型に変換

#include <iostream>
#include <cstdio>

#define dump_bitpattern(x) _dump_bitpattern(#x, x)
template <class T>
void _dump_bitpattern(const char *name, const T &x)
{
    printf("%s : ", name);
    unsigned char a[sizeof(T) + 2] = {0}; //
    std::memcpy(a, &x, sizeof(T));
    for (int i = 0; i < sizeof(a); i++)
    {
        printf("%x", a[i]);
    }
    printf("\n");
}

int main()
{
    {
        // Cスタイルのキャスト:
        // 安全性のチェックを伴わない、明示的な型の変換。実行時判定を行わない
        uint32_t x = 0xDEADBEEF;     // == 3735928559
        printf("%u / %08X\n", x, x); // 3735928559 / DEADBEEF
        dump_bitpattern(x);          // x : efbeadde (エンディアン)

        int x_int = (int)x;
        dump_bitpattern(x_int); // x_int : efbeadde

        uint16_t x_uint16 = (uint16_t)x; // == 48879
        dump_bitpattern(x_uint16);       // x_uint16 : efbe (切り捨てられる)

        float x_float = (float)x;
        dump_bitpattern(x_float); // x_float : bfad5e4f (浮動小数点数表現に変換)
    }
    {
        // static_cast:
        // intからfloatへの変換のように、明示的に型を変換。実行時判定を行わない
    }
    return 0;
}