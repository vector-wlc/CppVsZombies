/*
 * @coding: utf-8
 * @Author: vector-wlc
 * @Date: 2020-03-30 10:00:19
 * @Description: RE 艹栋十六炮
 */

// 此文件是一个加深的练习

#include "pvz.h"

// 阵型 RE 16
// 充分利用炮操作类带来的模式选择来大大简化编写工作
// 这里简化的工作主要是异常复杂炮序调整

//设定两个炮列表，以第四列作为分界线
//设定成全局变量以方便程序编写
PaoOperator pao_col_in_5_and_7;
PaoOperator pao_col_in_1_and_3;

//分别使用一门 1 3 列炮和一门 5 7 列炮
//inline 是内联，对于短小的函数效率更高
inline void MyRoofPao(float col_1_3, float col_5_7)
{
    pao_col_in_1_and_3.tryRoofPao(2, col_1_3);
    pao_col_in_5_and_7.tryRoofPao(4, col_5_7);
}

int main()
{
    using namespace pvz;
    SetZombies({CG_3, TT_4, BC_12, XC_15, QQ_16, BJ_20, FT_21, TL_22, BY_23, HY_32, TT_18});
    //选卡
    SelectCards({{"kfd"}, {"hbg"}, {"Mhbg"}, {"hmg"}, {"ytzd"}, {"hblj"}, {"hp"}, {"ymts"}, {"ymjnp"}, {"gjg"}});

    //将CvZ自带的对象炮列表清零
    UpdatePaoList({});

    using pvz::MouseCol;
    using pvz::MouseRow;

    // 按下 Q 发射一枚炮
    KeyConnect('Q', [&]() {
        pao_col_in_5_and_7.tryRoofPao(MouseRow(), MouseCol());
    });

    // 按下 W 种高坚果
    KeyConnect('W', [&]() {
        auto mouse_row = MouseRow();
        auto mouse_col = MouseCol();
        Card({{"hp", mouse_row, mouse_col}, {"gjg", mouse_row, mouse_col}});
    });

    //调整炮列表
    pao_col_in_1_and_3.resetPaoList({{1, 1}, {1, 3}, {2, 1}, {3, 1}, {3, 3}, {4, 1}, {5, 1}, {5, 3}});
    pao_col_in_5_and_7.resetPaoList({{1, 5}, {1, 7}, {2, 5}, {3, 5}, {3, 7}, {4, 5}, {5, 5}, {5, 7}});

    //解除炮序限制
    pao_col_in_1_and_3.setLimitPaoSequence(false);
    pao_col_in_5_and_7.setLimitPaoSequence(false);

    //setResolveConflictType 对所有对象均生效，所以只需要调用一次
    PaoOperator::setResolveConflictType(PaoOperator::DROP_POINT);

    Card("hp", 2, 9);

    //启用存冰线程
    ice_filler.start({{2, 3}, {4, 3}, {2, 8}, {4, 8}});
    ice_filler.setInterval(30);

    //############# wave 1 ##################
    Prejudge(223 - 387, 1);
    MyRoofPao(9, 9);
    Until(330 - 387);
    MyRoofPao(8.8, 7.7);
    Until(223 - 100);
    Card("ytzd", 2, 9);
    Until(361 - 100);
    Card("hblj", 2, 9);

    //############# wave 2 ##################
    Prejudge(250 - 387, 2);
    MyRoofPao(9, 9);
    Until(250 - 298);
    Card({{"hmg", 2, 9}, {"kfd", 2, 9}});
    Until(361 - 387);
    MyRoofPao(8.8, 8.8);
    Until(0);
    Card("hp", 5, 9);
    Delay(2);
    Shovel(5, 9);

    //############# wave 3 ##################
    Prejudge(385 - 387, 3);
    MyRoofPao(9, 9);
    Until(513 - 387);
    MyRoofPao(8.75, 8.75);
    Until(601 + 42 - 387);
    MyRoofPao(8.75, 8.75);
    Until(601 + 19 - 298);
    Coffee();

    //############# wave 4 ##################
    Prejudge(1145 - 387, 4);
    MyRoofPao(8.8, 8.8);
    Delay(387 + 210 - 298);
    Coffee();
    Ice3(298);

    //############# wave 5 ##################
    Prejudge(570 - 387, 5);
    MyRoofPao(9, 8.8);
    Until(570 + 220 - 387);
    MyRoofPao(8.4, 8.4);
    Until(1200 - 387);
    MyRoofPao(8.8, 8.8);
    Delay(387 + 210 - 298);
    Coffee();
    Ice3(298);

    //############# wave 6 ##################
    Prejudge(410 - 387, 6);
    MyRoofPao(9, 8.4);
    Until(410 + 220 - 387);
    MyRoofPao(8.45, 8.45);
    Until(1200 - 387);
    MyRoofPao(8.9, 8.8);
    Delay(387 + 210 - 298);
    Coffee();
    Ice3(298);

    //############# wave 7 ##################
    Prejudge(410 - 387, 7);
    pao_col_in_5_and_7.tryFixPao();
    MyRoofPao(8.8, 8.8);
    Until(410 + 220 - 387);
    pao_col_in_1_and_3.tryRoofPao(2, 8.45);
    Until(410 + 220 - 100);
    Card("ytzd", 4, 8);
    Until(1190 - 298);
    Card({{"hp", 3, 9}, {"hmg", 3, 9}, {"kfd", 3, 9}});
    Until(1190 + 220 - 387);
    MyRoofPao(7.7, 7.7);

    //############# wave 8 ##################
    Prejudge(400 - 387, 8);
    pao_col_in_5_and_7.tryRoofPao({{4, 9}, {2, 9}});
    Until(495 - 387);
    MyRoofPao(8.9, 8.9);
    Until(601 + 36 - 387);
    MyRoofPao(8.75, 8.75);
    Until(601 + 150 - 298);
    Coffee();

    //############# wave 9 ##################
    Prejudge(900 - 387, 9);
    MyRoofPao(9, 9);
    Until(1070 - 387);
    MyRoofPao(9, 9);
    Until(1290 - 387);
    MyRoofPao(8.8, 8.8);
    Until(2376 - 387);
    pao_col_in_1_and_3.tryRoofPao({{2, 9}, {4, 9}});

    //############# wave 10 ##################
    Prejudge(223 - 387, 10);
    pao_col_in_5_and_7.tryFixPao();
    MyRoofPao(9, 9);
    Until(375 - 110 - 387);
    pao_col_in_5_and_7.tryRoofPao({{2, 9}, {4, 9}});
    Until(375 - 387);
    pao_col_in_1_and_3.tryRoofPao(2, 8.8);
    Until(375 - 100);
    Card({{"hp", 4, 9}, {"ytzd", 4, 9}});
    Delay(130);
    Shovel(4, 9);

    //############# wave 11 ##################
    Prejudge(385 - 387, 11);
    pao_col_in_5_and_7.tryRoofPao({{2, 9}, {4, 9}});
    Until(513 - 387);

    pao_col_in_1_and_3.tryRoofPao({{4, 8.75}, {2, 8.75}});
    Until(601 + 42 - 387);
    MyRoofPao(8.75, 8.75);
    Until(601 + 19 - 298);
    Coffee();

    //############# wave 12 ##################
    Prejudge(1145 - 387, 12);
    MyRoofPao(9, 8.8);
    Delay(387 + 210 - 298);
    Coffee();
    Ice3(298);

    //############# wave 13 ##################
    Prejudge(570 - 387, 13);
    MyRoofPao(9, 8.8);
    Until(570 + 220 - 387);
    MyRoofPao(8.4, 8.4);
    Until(1215 - 387);
    MyRoofPao(8.8, 8.4);
    Delay(387 + 210 - 298);
    Coffee();
    Ice3(298);

    //############# wave 14 ##################
    Prejudge(420 - 373, 14);
    MyRoofPao(9, 8.8);
    Until(420 + 230 - 387);
    MyRoofPao(8.45, 8.45);
    Until(1215 - 387);
    MyRoofPao(8.9, 8.9);
    Delay(387 + 210 - 298);
    Coffee();
    Ice3(298);

    //############# wave 15 ##################
    Prejudge(410 - 387, 15);
    MyRoofPao(9, 8.8);
    Until(410 + 220 - 387);
    MyRoofPao(8.45, 8.45);
    Until(1215 - 387);
    MyRoofPao(8.9, 8.9);
    Delay(387 + 210 - 298);
    Coffee();
    Ice3(298);

    //############# wave 16 ##################
    Prejudge(410 - 387, 16);
    MyRoofPao(9, 9);
    Until(410 + 220 - 387);
    MyRoofPao(8.45, 8.45);
    Until(1200 - 387);
    MyRoofPao(8.9, 8.8);
    Delay(387 + 210 - 298);
    Coffee();
    Ice3(298);

    //############# wave 17 ##################
    Prejudge(410 - 387, 17);
    pao_col_in_1_and_3.tryFixPao();
    MyRoofPao(8.8, 8.8);
    Until(410 + 220 - 387);
    pao_col_in_1_and_3.tryRoofPao(2, 8.45);
    Until(410 + 220 - 100);
    Card("ytzd", 4, 8);
    Until(1200 - 298);
    Card({{"hp", 4, 9}, {"hmg", 4, 9}, {"kfd", 4, 9}});
    Until(1200 + 220 - 387);
    MyRoofPao(7.7, 7.7);

    //############# wave 18 ##################
    Prejudge(400 - 387, 18);
    MyRoofPao(9, 9);
    Until(495 - 387);
    MyRoofPao(8.75, 8.75);
    Until(601 + 36 - 387);
    MyRoofPao(8.75, 8.75);
    Until(601 + 150 - 298);
    Coffee();

    //############# wave 19 ##################
    Prejudge(900 - 387, 19);
    MyRoofPao(9, 9);
    Until(1070 - 387);
    MyRoofPao(9, 9);
    Until(1290 - 387);
    MyRoofPao(9, 9);
    Until(2376 - 387);
    MyRoofPao(9, 9);

    //############# wave 20 ##################
    Prejudge(0, 20);
    MyRoofPao(9, 9);
    MyRoofPao(9, 9);
    Delay(100);
    MyRoofPao(9, 9);
    MyRoofPao(9, 9);

    pvz::WaitGameEnd(); // 等待游戏结束再退出 main

    return 0;
}
