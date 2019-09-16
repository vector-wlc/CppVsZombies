
// 此文件主要介绍炮操作类成员函数之间的相互配合
// 所有的成员根据模式分大概可以分为四类

#include "pvz.h"

int main()
{
    pvz::pao_cvz.resetPaoList({}); // 使用炮操作类之前不要忘了把 CvZ 的炮操作对象的列表清零，或者你可以直接使用它
    auto &other_name = pvz::pao_cvz; // 为 pao_cvz 取别名

    PaoOperator pao_list;

    /////////////////// 限制炮序  完全由我们自己排炮序
    pao_list.setLimitPaoSequence(true);
    pao_list.pao(2, 9);
    pao_list.recoverPao(2, 9);
    pao_list.setNextPao(3);
    pao_list.skipPao(2);
    pao_list.roofPao(2, 9);

    /////////////////// 不限制炮序  由计算机帮助我们排炮序
    pao_list.setLimitPaoSequence(true);
    pao_list.tryPao(2, 9);
    pao_list.tryFixPao();
    pao_list.tryRoofPao(2, 9);

    ///////////////////  一些不受模式设置限制的成员
    pao_list.setResolveConflictType(PaoOperator::DROP_POINT);
    pao_list.rawPao(1, 2, 2, 9);
    pao_list.rawRoofPao(1, 2, 2, 9);
    pao_list.resetPaoList({});
    pao_list.changePaoMessage(1, 1, 1, 1);

    /////////////////// 这个函数比较特殊，他的使用条件是不破坏炮序就行，因此两种模式下都可使用
    pao_list.fixPao(3, 4);

    return 0;
}