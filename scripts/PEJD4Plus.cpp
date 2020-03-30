/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \CppVsZombies\functions\Unit 3\3-5 Practice.cpp
 * @Date: 2019-09-11 20:36:49
 * @LastEditTime: 2020-03-30 10:17:31
 * @Description: 经典四炮 +
 */

#include "pvz.h"

int main()
{
    using namespace pvz;
    SetZombies({PJ_0, TT_4, WW_8, QS_11, BC_12, XC_15, KG_17, HT_14, FT_21, BY_23, HY_32});
    // 选择寒冰菇 模仿寒冰菇 咖啡豆 窝瓜 樱桃 毁灭菇 荷叶 南瓜头 三叶草 小喷菇
    SelectCards({{"hbg"}, {"Mhbg"}, {"kfd"}, {"wg"}, {"ytzd"}, {"hmg"}, {"hy"}, {"ngt"}, {"syc"}, {"xpg"}});

    // 使用自动存冰对象进行存冰
    ice_filler.start({{3, 5}, {1, 4}, {6, 4}, {1, 5}, {6, 5}});

    // 使用自动修补坚果对象进行修补南瓜头 {1, 4}, {6, 4} 用来保护存冰
    nut_fixer.start(30, {{1, 4}, {6, 4}, {3, 5}, {3, 6}, {4, 5}, {4, 6}});

    dian_cai_placer.start({10}, {{2, 7}, {5, 7}});

    KeyConnect('Q', [&]() {
        Card("wg", pvz::MouseRow(), pvz::MouseCol());
    });

    for (wave = 1; wave < 21; wave++)
    {
        if (wave_in(1, 5, 9, 14, 18))
        {
            Prejudge(-95, wave);
            Pao({{2, 9}, {5, 9}});
            if (wave == 9)
                RecoverPao({{2, 8.5}, {5, 8.5}});
            else
            {
                //僵尸在 601 刷新，白天冰的生效时间为 298，所以当参数填为 601 - 298 时
                //就是僵尸刚一刷新冰就会生效，但是所有的僵尸并不是在这一时刻全部刷新
                //所以可以延迟 50 来达到冰冻全部僵尸的效果，这里 50 是随便取的，
                //你可以取得更多或更少，根据实际操作需求来定
                Until(601 - 298 + 50);
                ice_filler.coffee();
            }
        }
        else if (wave_in(2, 6, 11, 15, 19))
        {
            Prejudge(1800 - 373 - 200, wave);
            Pao({{2, 8.5}, {5, 8.5}});
            if (wave == 19)
            {
                RecoverPao({{2, 8.5}, {5, 8.5}});
                //停止自动存冰线程
                StopAutoFillIceThread();
            }
            else
            {
                Delay(373 + 210 - 298);
                ice_filler.coffee();
                Ice3(298);
            }
        }
        else if (wave_in(3, 7, 12, 16))
        {
            Prejudge(1800 - 373 - 200, wave);
            Pao({{2, 8.5}, {5, 8.5}});
            Delay(373 + 210 - 298);
            ice_filler.coffee();
            Ice3(298);
        }
        else if (wave_in(4, 8, 13, 17))
        {
            //使用核
            Prejudge(1200 - 200 - 298, wave);
            if (wave_in(4, 17))
                Card({{"hy", 3, 8}, {"hmg", 3, 8}, {"kfd", 3, 8}});
            else if (wave == 8)
                Card({{"hy", 3, 9}, {"hmg", 3, 9}, {"kfd", 3, 9}, {"ngt", 3, 9}});
            else
                Card({{"hy", 4, 9}, {"hmg", 4, 9}, {"kfd", 4, 9}, {"ngt", 4, 9}});
        }
        else if (wave == 10)
        {
            Prejudge(-55, wave);
            Pao({{2, 9}, {5, 9}});
            //种植樱桃消除延迟
            Until(-55 + 373 - 100);
            Card({{"ytzd", 2, 9}});
            Until(601 - 298 + 50);
            ice_filler.coffee();
        }
        else //if(wave == 20)
        {
            Prejudge(150, wave);
            ice_filler.coffee(); //冰杀小偷
            Delay(298 + 500);
            RecoverPao({{2, 9}, {5, 9}, {2, 8.5}, {5, 8.5}});
        }
    }

    pvz::WaitGameEnd();
    return 0;
}