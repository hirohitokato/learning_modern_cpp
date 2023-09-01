// enumにusingが使えるので、冗長なコードを書かずに済むようになりました。(C++20)
#include <iostream>

enum class EnumWithAVeryVeryLongName
{
    FirstValue,
    SecondValue,
    YetAnotherValue
};

int main()
{
    {
        // 従来の書き方
        auto value = EnumWithAVeryVeryLongName::FirstValue;
        switch (value)
        {
        case EnumWithAVeryVeryLongName::FirstValue:
            // なにかの処理
        case EnumWithAVeryVeryLongName::SecondValue:
            // なにかの処理２
        case EnumWithAVeryVeryLongName::YetAnotherValue:
            // ...毎回EnumWithALongName書くのめんどくさい
            break;
        }
    }
    {
        auto value = EnumWithAVeryVeryLongName::FirstValue;

        // C++20ではenumをusing出来るのでこう書ける
        switch (value)
        {
            using enum EnumWithAVeryVeryLongName; // ←new!
        case FirstValue:
            // なにかの処理
        case SecondValue:
            // なにかの処理２
        case YetAnotherValue:
            // ...楽！
            break;
        }
    }

    {
        // switch以外でも書ける。(スコープ内で有効)
        using enum EnumWithAVeryVeryLongName;
        auto value = FirstValue;
    }

    // ポイント:
    // 影響が大きくなるので、できるだけスコープを狭めること！

    return 0;
}