/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \CppVsZombies\functions\Unit 2\2-3 ThreadFunctions.cpp
 * @Date: 2019-08-31 09:16:29
 * @LastEditTime : 2020-01-30 12:57:22
 * @Description: 此文件主要介绍 CvZ 自带的子线程函数
 *               注意一个自动线程函数只能创建一个线程，因为只有一个与之匹配的停止线程运行函数
 *               下一单元将介绍如何创建多个相似功能的线程，所以在这里根本不用慌，嘻嘻
 *               自动存冰 : StartAutoFillIceThread
 *               自动修坚果 : StartAutoFixNutThread
 *               自动种植垫材 : StartAutoSetDianCaiThread
 *               女仆秘籍 : StartPreventDancerAdvance
 *               自动收集 : StartAutoCollectThread
 */

#include "pvz.h"

/////////////////////////// 使用示例

int main()
{
    // 在 {1, 2}, {1, 3}, {1, 4} 进行自动存冰
    StartAutoFillIceThread({{1, 2}, {1, 3}, {1, 4}});

    // 修补南瓜，第二个参数不填默认修补全场
    StartAutoFixNutThread(30);
    // 修补 {2, 3}, {3, 4}, {3, 5} 位置的高坚果
    // 注意这里是错误的，因为一个函数不能创建多个线程，
    // 此函数在前面已经创建了修补全场南瓜的线程，所以这里是错误的
    StartAutoFixNutThread(23, {{2, 3}, {3, 4}, {3, 5}}); // It's not allowed!

    // 将第 1 张卡片设为垫材，默认全场都从第九列开始垫，默认只垫 红眼 白眼
    StartAutoSetDianCaiThread({1});
    // 将第 1 2 张卡片设为垫材，只垫第 5 6 行，而且第二行从第六列开始垫，第四行从第七列开始垫，默认只垫 红眼 白眼
    StartAutoSetDianCaiThread({1, 2}, {{5, 6}, {6, 7}}); // It's not allowed!
    // 将第 1 张卡片设为垫材，只垫第 5 6 行，而且第二行从第六列开始垫，第四行从第七列开始垫，垫 红眼 白眼 橄榄
    StartAutoSetDianCaiThread({1}, {{5, 6}, {6, 7}}, {23, 32, 7}); // It's not allowed!

    // 开启自动收集，此函数框架自动调用，不需要用户书写
    StartAutoCollectThread();

    Delay(200);

    /////////////////////////// 每个子线程创建函数都有一个相应的停止线程运行的配对函数

    StopAutoFillIceThread();
    StopAutoFixNutThread();
    StopAutoSetDianCaiThread();
    StopAutoCollectThread();

    return 0;
}