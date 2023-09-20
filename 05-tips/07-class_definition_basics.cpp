/// クラス定義のテンプレート

/*
 * @see: https://qiita.com/qiita_kuru/items/95989a26f76678b7c3f1
 * C++11～でクラスを定義する場合の必要最低限のテンプレートが記載。
 * クラスを定義する際のファイル構成、ファイルの書き方、クラス定義の方法など。
 */

//////////////////////////////////////
// ヘッダファイル
//////////////////////////////////////
// 1.ヘッダファイルのインクルード
#include <iostream>
#include <mutex>

// 2.マクロ定義はここに記述

// 3.名前空間の定義はここに記述し、全体を包んでおく
// ※名前空間に包んだ場合は、実装ファイル側も同様にnamespaceを記述すること
namespace some_namespace
{
    // 必要に応じて多重に包む
    //    namespace project {
    class BaseClass
    {
    public:
        // デフォルトコンストラクタ
        BaseClass() {}

        // 引数付のコンストラクタ。１引数の場合は`explicit`をつけること
        explicit BaseClass(const std::string &value)
            : privateMember_(value) {}

        // コピーコンストラクタ
        // ※コピーさせたくない場合は`= delete`を記述
        BaseClass(const BaseClass &from)
            : privateMember_(from.privateMember_) {}
        // BaseClass(const BaseClass &from) = delete;

        // デストラクタ
        // ※基底クラスは必ずvirtualを付けること!!【必須】
        // 注:デストラクタにvirtualをつけた場合は、ムーブ系のメソッドは自動生成されなくなる
        virtual ~BaseClass() {}

        // コピー代入演算子のオーバーロード
        // ※代入させたくない場合は`= delete`を記述
        BaseClass &operator=(const BaseClass &from);
        // BaseClass &operator=(const BaseClass &from) = delete;

        // ムーブコンストラクタ。必要に応じて宣言。不要ならばdeleteを書く
        // 書かないと必要なときに自動生成されるが、このコードのようにデストラクタが付いている場合は自動生成されない
        BaseClass(BaseClass &&rhs) noexcept = delete;

        // ムーブ代入演算子。必要に応じて宣言。不要ならばdeleteを書く
        // 書かないと必要なときに自動生成されるが、このコードのようにデストラクタが付いている場合は自動生成されない
        BaseClass &operator=(BaseClass &&rhs) noexcept = delete;

        // パブリックなメンバー関数。
        // ※例外を出力しない場合は`noexcept`を書く(パフォーマンスに影響)
        int publicMethod(const std::string &input, std::string &output) noexcept;

        // 実行してもメンバ変数の状態を変更しない関数はconstを付ける(パフォーマンスに影響)
        // ※constを付ける場合はスレッドセーフになるように設計すること
        virtual std::string toString() const;

    private:
        // privateメンバー関数
        std::string privateMember_;
    };

    class SubClass: public BaseClass
    {
    public:
        SubClass(/* args */);
        virtual ~SubClass(); // さらに継承される可能性がある場合はvirtualをつける

        // 親クラスでdeleteしたものはサブクラスでもdeleteされる
    };
}
