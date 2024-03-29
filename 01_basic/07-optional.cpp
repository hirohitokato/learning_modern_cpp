/// std::optionalを使うと、値または無効値の２種類の状態を格納できる(C++17)
#include <iostream>
#include <optional>

/// @brief a/bで除算をした結果を返す関数。
/// @param a 割られる数
/// @param b 割る数
/// @return a/bの値。bがゼロの場合、無効値が返る
std::optional<int> safe_divide(int a, int b)
{
    if (b == 0)
        return std::nullopt; // 無効値を表す値

    return a / b;
}

/// @brief std::optionalクラスを使うと値が無効かどうかチェックする記述を簡潔に書ける。
///  記述量が減ってシンプルな表記にできることがメリット。またエラーであることを明示的に
///  示す値を利用出来るので、不具合も減るはず
int main()
{
    {
        std::optional<int> result = safe_divide(9, 3); // 3
        if (result) // 無効値が入っていないか検査。値が入っている場合はtrueを返す
        {
            int x = result.value();                            // ここで値を取り出す
            std::cout << "result of 9/3 : " << x << std::endl; // 3
        }
    }

    {
        // 9/0の結果を求める（ゼロ除算なので無効値 std::nullopt が返る）
        std::optional<int> result = safe_divide(9, 0);
        if (!result) // 無効値が入っている場合はfalseを返す
        {
            std::cout << "result of 9/0 : error" << std::endl;
        }
    }

    {
        // 値を取り出す際に無効値の代替値を使うことも出来る
        std::optional<int> result = safe_divide(9, 0);
        int result_ = result.value_or(-1); // 無効値std::nulloptの場合は-1を代わりに使う
        // ↑は↓の文と同じ意味
        // int result_ = result.has_value() ? result.value() : static_cast<int>(std::forward<int>(-1));

        std::cout << "dividing result : " << result_ << std::endl; // dividing result : -1
    }

    return 0;
}