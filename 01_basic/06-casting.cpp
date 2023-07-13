// キャスト
// - 変数を表す型を変換して扱うときに用いる

// C++のキャストはCスタイルのキャストを加えると以下の５つが使える。
//
// * Cスタイルのキャスト: 安全性のチェックを伴わない、明示的な型の変換。実行時判定を行わない
// * static_cast: intからfloatへの変換のように、明示的に型を変換。実行時判定を行わない
// * dynamic_cast: クラスの実行時型情報(RTTI)を利用して、参照またはポインタの方を変換。実行時判定を行う
// * const_cast: const修飾/volatile修飾を変更
// * reinterpret_cast: ポインタもしくは整数値を任意の型に変換

#include <iostream>