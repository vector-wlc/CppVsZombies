
// 此文件介绍植物内存信息读取类

#include "pvz.h"
#include <iostream>

int main()
{
    using namespace std;
    // 首先实例化一个植物内存信息读取对象
    pvz::PlantMemory plant;
    cout << "index  type   abscissa ordinate  row   col\n";
    while (1)
    {
        Sleep(100);
        // 获取最大植物数量
        int plant_count_max = plant.countMax();
        // 遍历所有植物
        for (int i = 0; i < plant_count_max; i++)
        {
            plant.setIndex(i); // 设置对象序列
            // 如果植物还"活着"
            if (!plant.isCrushed() && !plant.isDisappeared())
            {
                cout << i << "\t"                  //栈位/对象序列
                     << plant.type() << "\t"       //类型
                     << plant.abscissa() << "\t  " //横坐标
                     << plant.ordinate() << "\t  " //纵坐标
                     << plant.row() + 1 << "\t"    //所在行
                     << plant.col() + 1 << "\n";   //所在列
            }
        }
    }

    return 0;
}