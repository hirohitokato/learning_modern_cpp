// プログラムの終了方法４選
// * C++11

#include <iostream>
#include <cstdlib>   // exit() & quick_exit()
#include <exception> // terminate()

struct GlobalClass
{
    ~GlobalClass()
    {
        // 方法1と２でのみ呼ばれる。3,4では呼ばれない
        std::cout << "destructor is called." << std::endl;
    }
};

GlobalClass myobj;

int main()
{
    // システム終了には４種類の方法がある

    // 1) main()から戻る。グローバルオブジェクトのデストラクタが呼ばれる
    // 2) std::exit(status)。1と同じ
    // 3) std::quick_exit(status)。デストラクタは呼ばれない
    // 4) std::terminate()。システムを異常終了(SIGABRT)させる。デストラクタは呼ばれない

    int condition = 1;
    switch (condition)
    {
    case 1: // 方法１
        return 0;
    case 2: // 方法２
        std::exit(0);
    case 3: // 方法3
        // 注意：macOSではquick_exit()は定義されていないので使えない
        // https://teratail.com/questions/67366
        std::quick_exit(0);
    case 4: // 方法4
        std::terminate();
    default:
        return -1;
    }
    return 0;
}
