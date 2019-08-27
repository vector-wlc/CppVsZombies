
// 本文件主要介绍炮操作类的成员函数

#include "pvz.h"

int main()
{
    UpdatePaoList({}); //使用炮操作类之前不要忘了把 CvZ 的炮操作对象的列表清零

    PaoOperator pao_list; //首先我们先创建一个炮列表

    /////////////////// PaoOperator Member Introduction

    // 更新/重置 炮列表
    // 将对象的炮列表设置为 {2, 3}, {3, 4}
    pao_list.resetPaoList({{2, 3}, {3, 4}});

    // 设定解决冲突方式
    // 注意：此函数的设定对于创建的所有对象均生效
    // 将冲突设置为优先解决炮落点的冲突
    // 这样写后将不存在因为点炮位置与落点较近而发不出去炮的现象
    // 但是有可能会因为炮身旁有一堆收集物导致无法成功发炮
    pao_list.setResolveConflictType(PaoOperator::DROP_POINT);

    // 将冲突设置为优先解决收集物的冲突
    // 这样写后将不存在因为点炮位置周围有较多的收集物干扰而无法成功发炮的现象
    // 但是有可能会出现因为点炮位置与落点较近而发不出去炮的现象
    pao_list.setResolveConflictType(PaoOperator::COLLECTION);

    // 设定炮序限制
    // 限定炮序，这样写后依赖炮序的 pao 系列函数可用，
    // 不依赖炮序的 tryPao 系列函数不可使用
    // 而且不允许 fixPao 铲种炮列表内的炮
    // rawPao 系列炮仍可用
    pao_list.setLimitPaoSequence(true);

    // 解除炮序限制，这样写后依赖炮序的 pao 系列函数不可用，
    // 不依赖炮序的 tryPao 系列函数可用
    // 而且允许 fixPao 铲种炮列表内的炮
    // rawPao 系列炮仍可用
    pao_list.setLimitPaoSequence(false);

    //一些非常熟悉的函数不再介绍其用法

    pao_list.rawPao(1, 1, 2, 2);

    pao_list.pao(2, 9);

    pao_list.tryPao(5, 9);

    pao_list.skipPao(2);

    pao_list.setNextPao(1);
    pao_list.setNextPao(2, 4);

    pao_list.fixPao(4, 5);         // 铲种位置为(4, 5)的炮
    pao_list.fixPao(4, 5, -1);     // 位移铲种位置为(4, 5)的炮，种的炮将向左位移一格
    pao_list.fixPao(4, 5, 1);      // 位移铲种位置为(4, 5)的炮，种的炮将向右位移一格
    pao_list.fixPao(4, 5, 0, 100); // 铲种位置为(4, 5)的炮，当炮被铲后，延迟 100cs 再进行相关种植操作

    pao_list.recoverPao(3, 9); // 等待下一门炮恢复立即发往(3, 9)

    pao_list.rawRoofPao(1, 1, 2, 2);

    pao_list.roofPao(2, 9);

    pao_list.tryRoofPao(5, 9);

    // 改变炮的信息
    // 请在手动或使用基础函数例如 Card 改变炮的信息后立即使用此函数
    // 当然在实际写代码的时候请尽量避免自己用基础函数例如 Card 进行种炮
    pao_list.changePaoMessage(2, 3, 2, 3); // 在手动铲种(2,3)位置的炮后，更改相关炮的信息
    pao_list.changePaoMessage(2, 3, 2, 4); // 手动位移铲种(2,3)位置的炮后，更改相关炮的信息
    pao_list.changePaoMessage(0, 0, 2, 3); // 手动增加(2,3)位置的炮后，更改相关炮的信息

    return 0;
}