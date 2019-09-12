
// 此文件是第五单元的一个练习
// 利用内存信息获取接口来进行"智能"操作

#include "pvz.h"

// 阵型：DE 双冰六炮
// 主要的"智能"的操作有判断是否有冰车来使用倭瓜
// 收尾自动选择红眼数目最少的行，其余行直接炸死

// 收尾
void Ending();

int main()
{
    SelectCards({{"hbg"}, {"Mhbg"}, {"kfd"}, {"hmg"}, {"wg"}, {"ytzd"}, {"ngt"}, {"xpg"}, {"ygg"}, {"dxg"}});

    ice_filler.start({{1, 1}, {5, 1}, {3, 7}});

    for (wave = 1; wave < 21; wave++)
        switch (wave)
        {
        case 1:
        case 2:
        case 5:
        case 8:
        case 13:
        case 16:
        case 19:
            Prejudge(-95, wave);
            Pao({{2, 9}, {4, 9}});
            if (wave != 1)
            {
                Until(601 - 298 + 50);
                ice_filler.coffee();
                //垫撑杆
                if (wave == 2 && pvz::ExamineZombieExist(3, 3))
                    Card("xpg", 3, 9);
                if (wave == 5)
                    ice_filler.resetFillList({{1, 1}, {5, 1}});
                if (wave == 19)
                    Ending();
            }
            break;

        case 3:
        case 6:
        case 9:
        case 11:
        case 14:
        case 17:
            Prejudge(50, wave);
            if (wave == 3 && pvz::ExamineZombieExist(12, 3))
            {
                Shovel(3, 9);
                Card("wg", 3, 9);
            }

            Until(1500 - 373 - 200);
            Pao({{2, 8.5}, {4, 8.5}});
            if (wave != 9)
            {
                Until(1500 - 298 + 50);
                ice_filler.coffee();
            }
            else
            {
                Ending();
            }
            break;

        case 10:
            Prejudge(-55, wave);
            Pao({{2, 9}, {4, 9}});
            Until(-55 + 373 - 100);
            Card("ytzd", 2, 9);
            Until(601 - 298 + 50);
            ice_filler.coffee();
            break;

        case 20:
            Prejudge(-55, wave);
            Pao({{2, 9}, {4, 9}});
            Ending();
            break;

        default:
            Prejudge(1500 - 373 - 200, wave);
            Pao({{2, 8.5}, {4, 8.5}});
            break;
        }

    return 0;
}

void Ending()
{
    if (wave == 20)
        RecoverPao({{2, 9}, {4, 9}});
    else
        RecoverPao({{2, 8}, {4, 8}});

    // 等待炮生效
    Delay(373 + 10);
    dian_cai_placer.start({8, 9, 10});

    pvz::ZombieMemory giant;

    int zombie_count_max = giant.countMax();

    int giant_row = 0;

    // 统计每行的红眼巨人的数量
    for (int i = 0; i < zombie_count_max; i++)
    {
        giant.setIndex(i);
        // 如果巨人不是残血
        if (giant.type() == 32 && giant.hp() > 600)
        {
            giant_row = giant.row() + 1;
            break;
        }
    }

    // 如果是第一行
    switch (giant_row)
    {
    case 1:
        RecoverPao({{4, 8}, {4, 8}});
        {
            Shovel(2, 7);
            Card("wg", 2, 7);
        }

        break;

    case 2:
        RecoverPao({{4, 8}, {4, 8}});
        {
            Shovel(1, 7);
            Card("wg", 1, 7);
        }
        break;

    case 4:
        RecoverPao({{2, 8}, {2, 8}});
        {
            Shovel(5, 7);
            Card("wg", 5, 7);
        }
        break;

    case 5:
        RecoverPao({{2, 8}, {2, 8}});
        {
            Shovel(4, 7);
            Card("wg", 4, 7);
        }

        break;

    default:
        printf(" cry \n");
        break;
    }

    // 等待倒计时结束
    while (pvz::Countdown() > 150)
        Sleep(5);
    if (wave_in(9, 19))
    {
        while (pvz::Countdown() > 4)
            Sleep(5);
        while (pvz::HugeWaveCountdown() > 150)
            Sleep(5);
    }

    dian_cai_placer.stop();
    Delay(10);
    //铲除所有垫材
    for (int row = 1; row < 6; row++)
        for (int col = 7; col < 10; col++)
            Shovel(row, col);
    Card("wg", giant_row, 7);
}
