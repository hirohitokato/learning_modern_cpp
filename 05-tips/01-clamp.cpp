// 値を範囲内に丸める
// * https://speakerdeck.com/cpp/cedec2020?slide=14
// * C++17

#include <algorithm>
#include <iostream>

int main()
{
    auto x = 300;

    // x をmin～maxの範囲に丸めるときは std::clamp(x, min, max)が便利
    // 古いやり方のように不注意で範囲をひっくり返してしまうことも減る
    {
        // 古いやり方(0-100に丸める)
        auto clipped = std::min(std::max(x, 0), 255); // → 255
    }
    {
        // 今のやり方
        auto clipped = std::clamp(x, 0, 255); // → 255
    }
}