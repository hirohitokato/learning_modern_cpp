# RAII (Resource Acquisition Is Initialization)

「リソースの確保は初期化時に行う」の意味。ただしこの用語が使われる文脈では **「リソースの確保と解放を変数の寿命に一致させる」** の意味で用いられる。安全・確実にリソースを管理するのに役立つテクニック。


## 余談：C言語でRAII

GCCを使用しているのであれば以下のattributeを使うとそれっぽいことができる(らしい。未確認)。

（https://stackoverflow.com/a/368731 より）

```c
#include <stdio.h>

void cleanup_func(int * pvariable) {
    printf("variable (%d) goes out of scope\n", *pvariable);
}

int main(void) {
    printf("before scope\n");
    {
        int watched __attribute__((cleanup (cleanup_func)));
        watched = 42;
    }
    printf("after scope\n");
}
/* 実行結果:
before scope
variable (42) goes out of scope ← after scopeの前に呼ばれている
after scope
*/
```