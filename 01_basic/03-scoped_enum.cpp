/// スコープ付き列挙型 (aka enum class)

#include <iostream>
#include <cstdint>
#include <tuple>

// 古(いにしえ)の書き方
enum Color
{
    red,
    green,
    blue = 2
};

// 今の書き方
enum struct /*classも可*/ ScopedColor
{
    red,
    green = 1,
    blue
};

enum struct /*classも可*/ Diretion
{
    up,
    down,
    left,
    right
};

// 値として持つときの型も古い方法と同様に指定できる
// (旧:デフォルトなし/新:デフォルトint)
enum class Status : std::uint8_t
{
    good,
    bad = 127
};

std::tuple<int, int, int> get_rgb();

// 説明
int main()
{
    using namespace std;
    {
        // 古い書き方

        // 問題1: 古い書き方はグローバルな名前空間を汚染してしまう
        // (Colorが通用する場面ではRed/green/Blueの名前が使えない)
        cout << "red   : " << red << endl;
        cout << "green: " << green << endl;
        cout << "blue  : " << blue << endl;

        auto red = 5;                              // コンパイラによっては再定義エラーになる。
                                                   // ならない場合も以降このスコープでredが使えない
        cout << "| red is " << red << endl;        // 5
        cout << "| red is " << Color::red << endl; // こうすれば 0

        // 問題2: ただの数値なのでおかしなことができてしまう
        cout << "blue + 5 = " << blue + 5 << endl;                                         // ?!
        cout << "blue is bigger than red : " << ((blue > red) ? "true" : "false") << endl; // ?!しかもバグ
    }
    cout << "-------------------------------------" << endl;
    {
        // 新しい書き方
        auto direction = Diretion::right; // `型名::列挙子`で書く

        // メリット1: 名前空間を汚染しない
        auto up = 3; // これはDirection::upとは別物

        // メリット2: 数値ではなく型で区別してくれる(本来のenumの意図に沿う)
        // ↓以下はコンパイルエラー！
        // cout << "left + right = " << Diretion::left + Diretion::right << endl; // コンパイルエラー
        cout << "up > down ? " << (Diretion::up > Diretion::down) << endl; // これはできてしまうので注意
        // cout << "up : " << Diretion::up << endl;

        // 強引に数値にしたい場合はstatic_cast<>を使う
        cout << "up   : " << static_cast<int>(Diretion::up) << endl;
        cout << "down : " << static_cast<int>(Diretion::down) << endl;
        cout << "left : " << static_cast<int>(Diretion::left) << endl;
        cout << "right: " << static_cast<int>(Diretion::right) << endl;
    }
    cout << "-------------------------------------" << endl;
    {
        // 古い書き方の方が良い唯一の例

        // タプルからの値の入手
        auto rgb = get_rgb();
        auto r = std::get<0>(rgb); // この0-2がわかりにくい
        auto g = std::get<1>(rgb);
        auto b = std::get<2>(rgb);
        cout << "r/g/b : " << r << "/" << g << "/" << b << endl;

        // enumを使って名前でアクセスできる
        auto r2 = std::get<Color::red>(rgb);
        auto g2 = std::get<Color::green>(rgb);
        auto b2 = std::get<Color::blue>(rgb);

        // スコープ付き列挙型だとけっこう面倒
        auto r3 = std::get<static_cast<std::size_t>(ScopedColor::red)>(rgb);
        auto g3 = std::get<static_cast<std::size_t>(Color::green)>(rgb);
        auto b3 = std::get<static_cast<std::size_t>(Color::blue)>(rgb);

        // でもC++17以降であれば構造化束縛が使えるのでもはや使われない
        auto [r4, g4, b4] = get_rgb();
        cout << "r/g/b : " << r4 << "/" << g4 << "/" << b4 << endl;
    }
}

std::tuple<int, int, int> get_rgb()
{
    return std::make_tuple(38, 64, 47);
}
