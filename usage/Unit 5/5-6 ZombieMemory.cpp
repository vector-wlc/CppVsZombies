/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \CppVsZombies\functions\Unit 5\5-6 ZombieMemory.cpp
 * @Date: 2019-09-13 17:10:18
 * @LastEditTime : 2020-01-17 17:01:13
 * @Description: 
 */

// 此文件介绍僵尸内存信息读取类

#include "pvz.h"
#include <iostream>

int main()
{
    using namespace std;
    // 首先实例化一个僵尸内存信息读取对象
    pvz::ZombieMemory zombie;
    while (1)
    {
        Sleep(100);
        system("cls");
        // 获取最大僵尸数量
        cout << "index  type    abscissa  ordinate  row\n";
        int zombie_count_max = zombie.countMax();
        // 遍历所有僵尸
        for (int i = 0; i < zombie_count_max; i++)
        {
            zombie.setIndex(i); // 设置对象序列
            // 如果僵尸还"活着"
            if (!zombie.isDead() && zombie.isExist())
            {
                cout << i << "\t"                     //栈位/对象序列
                     << zombie.type() << "\t"         //类型
                     << zombie.abscissa() << "\t  "   //横坐标
                     << zombie.ordinate() << "\t    " //纵坐标
                     << zombie.row() + 1 << "\n";     //所在行
            }
        }
    }

    return 0;
}