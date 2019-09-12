
// 此文件是第五单元的一个练习
// 利用内存信息获取接口来实现一些智能的操作
// 自动种植原来的植物

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
    if (plant_index == -1)
    {
        printf("error\n");
        return;
    }

    PlantMemory plant(plant_index);

    // 获取相应的卡片/种子的对象序列
    int seed_type = GetSeedIndex(plant.type());

    SeedMemory seed(seed_type);

    while (1)
    {
        Sleep(100);
        if (seed.isUsable())
        {
            // 如果植物没了
            if (plant.isDisappeared() || plant.isCrushed())
            {
                Card(seed_type + 1, row, col);
                //需要延迟 1 cs ，此处十分重要！！！！
                Delay(1);
                // 种植植物之后要更新对象序列
                plant.setIndex(GetPlantIndex(row, col, type));
            }
        }
    }
}