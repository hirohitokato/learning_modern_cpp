// 初期化付き条件分岐
// * https://speakerdeck.com/cpp/cedec2020?slide=47
// * C++17

#include <iostream>

int getResult()
{
    return 1;
}

int main()
{
    // if文とswitch文の()の中に初期化文を書けるようになった
    // 宣言した変数は同じif/switch文の中でしか使えないので、スコープを狭められる

    // 例１
    if (auto result1 = getResult(); result1 > 0)
    {
        std::cout << "result1 is " << result1 << std::endl; // result1 is 1
    }
    // resutlt1 += 1; // コンパイルエラー！ここではもうresult1は使えない

    // 例２
    switch (auto result2 = getResult(); result2)
    {
    case 1:
        std::cout << "result2 is " << result2 << std::endl; // result2 is 1
        break;
    default:
        break;
    }
    // std::cout << resutlt2 << std::endl; // コンパイルエラー！ここではもうresult2は使えない

    return 0;
}