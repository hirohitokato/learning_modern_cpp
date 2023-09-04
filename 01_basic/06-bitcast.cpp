// 参考：
// 「（翻訳）C/C++のStrict Aliasingを理解する または - どうして#$@##@^%コンパイラは僕がしたい事をさせてくれないの！」
// https://yohhoy.hatenadiary.jp/entry/20120220/p1

#include <iostream>

uint32_t swap_endian_invalid(uint32_t a)
{
    uint32_t acopy = a;
    uint16_t *ptr = (uint16_t *)&acopy; // can't use static_cast<>, not legal.
                                        // you should be warned by that.
    uint16_t tmp = ptr[0];
    ptr[0] = ptr[1];
    ptr[1] = tmp;
    return acopy;
}
