/// ファイル内でのみ有効な変数、関数を宣言する場合は無名名前空間を使う。
#include <iostream>
#include <vector>
#include <limits>

// 古い方法：
static int foo = 1;
static double getMaxValue(const std::vector<double> array)
{
    return 100;
}

// より良い方法
namespace
{
    int bar = 10;
    double getMinValue(const std::vector<double> array)
    {
        return 200;
    }
    int foo = 5; // 注意:この変数にはアクセスできない
}

int main()
{
    std::cout << ::foo << std::endl;             // 1
    std::cout << ::getMaxValue({}) << std::endl; // 100

    std::cout << ::bar << std::endl;             // 10
    std::cout << ::getMinValue({}) << std::endl; // 200

    {
        // 応用：同じ名前がそれぞれ別の名前空間にある場合
        int foo = 20;

        std::cout << foo << std::endl;   // 20 ←ローカル変数
        std::cout << ::foo << std::endl; // 1 ←グローバル名前空間のfoo
        // このとき無名名前空間内のfooにはアクセスできないので注意
    }

    return 0;
}
