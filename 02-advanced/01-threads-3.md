# 排他処理

## ロックの種類


|型|ver.|説明|
|---|---|---|
|`std::mutex`||ロック/非ロックで排他領域を作り出せる|
|`std::recursive_mutex`||同じスレッドであれば何回でも`lock()`できる|
|`std::shared_mutex`|C++17|リーダーライターロックを提供
|`std::shared_timed_mutex`|C++14|タイマー付のリーダーライターロックを提供
|`std::binary_semaphore`|C++20|バイナリセマフォ。`counting_semaphore<1>`相当|
|`std::counting_semaphore`|C++20|計数セマフォ|

* `std::recursive_mutex`はスレッドがロックを所有するため、別のスレッドから`unlock()`はできない点がセマフォと異なる

# ロック獲得/解放処理の便利メソッド

RAIIあるいは[Scoped Locking Pattern](https://www.dre.vanderbilt.edu/~schmidt/PDF/ScopedLocking.pdf)と呼ばれる方法で、スコープの寿命にロック獲得/解放を同期させるための管理メソッド群が用意されている。

|型|ver.|説明|
|---|---|---|
|`std::lock_guard`||シンプルなロック獲得/解放処理|
|`std::unique_lock`||`std::lock_guard`の高機能版|
|`std::shared_lock`|C++14|`std::shared_mutex`を共有ロック/解放する|
|`std::scoped_lock`|C++17|複数のミューテックスをまとめてロック/解放する

※ `std::shared_mutex`を占有してロックしたい場合は他の`std::lock_guard`/`unique_lock`/`scoped_lock`を使えば良い