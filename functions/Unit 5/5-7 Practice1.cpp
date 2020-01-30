/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \CppVsZombies\functions\Unit 5\5-7 Practice1.cpp
 * @Date: 2019-09-13 17:46:12
 * @LastEditTime : 2020-01-20 15:11:00
 * @Description: 此文件是第五单元的一个练习
 *               利用内存信息获取接口来实现一些"智能"的操作
 *               自动种植原来的植物
 */

#include "pvz.h"

using namespace pvz;

// 自动种植原来的植物
void AutoPlant(int row, int col, int type = -1);

int main()
{
    //自动种植位于(2,4)的植物
    AutoPlant(2, 4);
    return 0;
}

void AutoPlant(int row, int col, int type)
{
    // 获取植物内存对象序列
    int plant_index = GetPlantIndex(row, col, type);

    // 如果植物不存在，退出函数
    if (plant_index < 0)
    {
        printf("error\n");
        return;
    }

    PlantMemory plant(plant_index);

    // 获取相应的卡片/种子的对象序列
    int seed_index = GetSeedIndex(plant.type());

    SeedMemory seed(seed_index);

    while (1)
    {
        Sleep(100);
        if (seed.isUsable())
        {
            // 如果植物没了
            if (plant.isDisappeared() || plant.isCrushed())
            {
                Card(seed_index + 1, row, col);
                //需要延迟 1 cs ，此处十分重要！！！！
                Delay(10);
                // 种植植物之后要更新对象序列
                plant.setIndex(GetPlantIndex(row, col, type));
            }
        }
    }
}