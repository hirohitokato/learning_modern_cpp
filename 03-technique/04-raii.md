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

GCCのドキュメントにある記述も転記しておく。

> The cleanup attribute runs a function when the variable goes out of scope. This attribute can only be applied to auto function scope variables; it may not be applied to parameters or variables with static storage duration. The function must take one parameter, a pointer to a type compatible with the variable. The return value of the function (if any) is ignored.
>
> `cleanup`属性は変数がスコープから外れたときに関数を実行する。この属性は自動関数スコープの変数にのみ適用でき、パラメータ(?関数引数のこと?)やstaticな変数には適用できない。属性にわたす関数は、変数と互換性のある型へのポインタをパラメータとして1つ取る。関数の戻り値は書いたとしても無視される。
