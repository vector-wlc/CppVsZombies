
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
    using std::cout;
    using std::endl;
    int pvz_mainobject = ReadMemory<int>(0x6A9EC0, 0x768); // 记录游戏主要对象地址

    cout << ReadMemory<int>(pvz_mainobject + 0x5564) << endl; // 打印总波数
    cout << ReadMemory<int>(pvz_mainobject + 0x5568) << endl; // 打印游戏时钟

    int plant_offset = ReadMemory<int>(pvz_mainobject + 0xAC); // 记录植物内存地址偏移

    cout << ReadMemory<int>(plant_offset + 0x14C * 35 + 0x24); // 打印栈位为 2 的植物类型

    return 0;
}