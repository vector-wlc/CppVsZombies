
// 此文件用来说明多炮列表的用法

#include "pvz.h"

// 利用多个炮列表用来避免天台复杂的调节炮序的问题
// 阵型：RE 十炮

PaoOperator ground_pao; // 平地炮炮列表
PaoOperator wind_pao;   // 风炮炮列表

// 定义一个发射炮的函数
void MyRoofPao()
{
    ground_pao.roofPao(4, 9); // 平地炮打下半场
    wind_pao.roofPao(2, 9);   // 风炮打上半场
}

int main()
{
    //选择寒冰菇，咖啡豆，花盆，樱桃炸弹，忧郁菇，香蒲，冰西瓜投手，吸金磁，地刺王，玉米加农炮
    SelectCards({{"hbg"}, {"kfd"}, {"hp"}, {"ytzd"}, {"yyg"}, {"xp"}, {"bxgts"}, {"xjc"}, {"dcw"}, {"ymjnp"}});
    
    // 使用炮操作类必须把 CvZ 自带的炮操作对象内的所有炮清零
    UpdatePaoList({}); 
    // 为对象设定炮列表
    ground_pao.resetPaoList({{1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}});
    wind_pao.resetPaoList({{{1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}});

    // 接下来只需要无脑的使用 MyRoofPao 即可，由于这个阵型没有落点冲突
    // 所以完全不需要考虑炮序的问题

    for (wave = 1; wave < 21; wave++)
    {
        if (wave == 10)
        {
            //MyRoofPao炮飞行时间为387cs
            Prejudge(-55 + 373 - 387, wave);
            MyRoofPao();
            Delay(387 - 100);
            //消除刷新延迟
            Card({{"hp", 2, 9}, {"ytzd", 2, 9}});
            Delay(118);
            Shovel(2, 9);
        }
        else if (wave == 20)
        {
            Prejudge(-150, wave);
            //冰消空降
            Card({{"hp", 1, 8}, {"hbg", 1, 8}, {"kfd", 1, 8}});
            Delay(300);
            Shovel(1, 8);
            Delay(500);
            MyRoofPao();
            Delay(3480 / 5);
            MyRoofPao();
            Delay(3480 / 5);
            MyRoofPao();
        }
        else
        {
            Prejudge(3475 / 5 - 200 - 387, wave);
            MyRoofPao();
            //自动收尾
            if (wave_in(9, 19))
            {
                Delay(3480 / 5);
                MyRoofPao();
                Delay(3480 / 5);
                MyRoofPao();
            }
        }
    }
    return 0;
}