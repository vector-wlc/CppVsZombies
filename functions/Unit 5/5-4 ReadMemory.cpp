/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \CppVsZombies\functions\Unit 5\5-4 ReadMemory.cpp
 * @Date: 2019-09-12 21:57:37
 * @LastEditTime : 2020-01-16 15:53:07
 * @Description: 
 */

// 此文件介绍一个非常重要的函数 ReadMemory

#include "pvz.h"
#include <iostream>

int main()
{
    // 在使用 ReadMemory 之前，你需要看懂地址

    // 6A9EC0
    // ├┬768    // 当前游戏信息和对象
    // │├―5564  // 本轮总波数
    // │├―5568  // 游戏时钟
    // │├―556C  // 游戏时钟(包括选卡时)
    // │├―557C  // 已刷新波数
    // │├―5594  // 刷新触发血量
    // │├―5598  // 本波总血量
    // │├―559C  // 刷新倒计时
    // │├―55A0  // 刷新倒计时初始值
    // │├―55A4  // 旗帜波刷新倒计时
    // │├─┬AC   // 植物属性(+14C下一个)
    // ││ ├─8   // 植物横坐标
    // ││ ├─C   // 植物纵坐标
    // ││ ├─1C  // 所在行数
    // ││ ├─24  // 植物类型
    // ││ ├─28  // 所在列数
    // ││ ├─3C  // 植物状态
    // ││ ├─40  // 当前血量
    // ││ ├─44  // 血值上限

    using pvz::ReadMemory;

    uintptr_t x = ReadMemory<uintptr_t>(0x6A9EC0, 0x768, 0x138);

    while (true)
    {
        Sleep(10);
        system("cls");
        std::cout << ReadMemory<int>(0x28 + x);
    }
    system("pause");
    return 0;
}