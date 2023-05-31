# Pimplイディオム

The pimpl idiom is a modern C++ technique to hide implementation, to minimize coupling, and to separate interfaces. Pimpl is short for "pointer to implementation." You may already be familiar with the concept but know it by other names like Cheshire Cat or Compiler Firewall idiom.

## Pimpleのメリット

* コンパイル依存関係の最小化
* インターフェースと実装の分離
* ポータビリティ

## ゴール

```cpp
// my_class.h
class my_class {
   //  ... all public and protected stuff goes here ...
private:
   class impl;
   unique_ptr<impl> pimpl; // opaque type here
};
```

> Opaque typeとは

```cpp
// my_class.cpp
class my_class::impl {  // defined privately here
  // ... all private data and functions: all of these
  //     can now change without recompiling callers ...
};
my_class::my_class(): pimpl( new impl )
{
  // ... set impl values ...
}
```
