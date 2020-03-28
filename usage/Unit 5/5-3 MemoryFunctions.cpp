
// 此文件将介绍此框架内的内存读取函数

#include "pvz.h"
#include <iostream>

int main()
{
    int now_time = pvz::GameClock(); // 获取当前游戏时钟
    int now_wave = pvz::NowWave();   // 获取当前波数
    std::vector<int> zombie_type;
    pvz::GetZombieType(zombie_type); // 获取出怪类型

    using std::cout;
    // 打印出怪类型信息
    for (int type : zombie_type)
        cout << type << " ";

    cout << "\n"; // 换行

    pvz::WaitGameStart(); // 等待游戏开始
    // 除此之外还有很多，具体请查看 libpvz.h 内容

    return 0;
}