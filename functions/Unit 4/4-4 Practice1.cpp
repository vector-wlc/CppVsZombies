
// 此文件是第四单元的练习

#include "pvz.h"

// 此次使用 tryPao 来进行操作，使用 tryPao 用来避免调整炮序的工作
// 当不需要多个炮列表时，则不需要刻意使用炮操作类
// 炮操作类是为了方便用户编写代码的，具体写代码的时候是怎么简单怎么来！！！
// 阵型：万阵之王
// 代码借鉴于 https://pvz.lmintlcx.com/scripts/

int main()
{
    // 选卡
    SelectCards({{"Mhbg"}, {"hbg"}, {"kfd"}, {"ngt"}, {"jg"}, {"wg"}, {"hp"}, {"dxg"}, {"ygg"}, {"xpg"}});

    // 存冰
    StartAutoFillIceThread({{4, 9}});

    // 解除炮序限制/废掉炮序
    SetLimitPaoSequence(false);
    // 冲突解决方式为落点 在使用 tryPao 最好这样做
    SetResolveConflictType(PaoOperator::DROP_POINT);
    
    for (wave = 1; wave < 21; wave++)
    {
        if (wave_in(1, 4, 7, 11, 14, 17))
        {
            Prejudge(-15, wave);
            TryPao({{2, 9}, {5, 9}});
            Delay(107);
            TryPao({{1, 7.7}, {5, 7.7}});
        }
        else if (wave_in(2, 5, 8, 12, 15, 18))
        {
            Prejudge(-95, wave);
            TryPao({{2, 9}, {5, 9}});
            Delay(373 - 100 - 198);
            Coffee();
        }
        else if (wave_in(3, 6, 9, 13, 16, 19))
        {
            Prejudge(-95, wave);
            TryPao({{2, 9}, {5, 9}, {2, 9}, {5, 9}});
            Delay(108);
            TryPao({{1, 8.8}, {5, 8.8}});
            if (wave_in(9, 19))
            {
                Until(601 - 15);
                TryPao({{2, 9}, {5, 9}});
            }
        }
        else if (wave == 10)
        {
            Prejudge(-55, wave);
            TryPao({{2, 9}, {5, 9}, {2, 9}, {5, 9}});
            Delay(108);
            TryPao({{1, 8.8}, {5, 8.8}});
        }
        else if (wave == 20)
        {
            Prejudge(-150, wave);
            TryPao({{4, 6}, {4, 8}});
            Until(-60);
            TryPao({{1, 9}, {5, 9}, {2, 9}, {5, 9}});
            Delay(108);
            TryPao({{1, 9}, {5, 9}, {2, 9}, {5, 9}});
        }
    }
    return 0;
}