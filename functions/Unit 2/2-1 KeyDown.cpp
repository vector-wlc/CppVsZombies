#include "pvz.h"

using namespace pvz_memory; // 直接使用名称空间 pvz_memory 内的所有标识符

////////////////////// 函数声明

//当同时按下多个按键时，一次循环只会执行一个键盘的敲击
void KeyOperate1();

//当同时按下多个按键时，一次循环会执行多个键盘的敲击
void KeyOperate2();

////////////////////// main

int main()
{
    //运行时需要将其中一个注释掉
    KeyOperate1();
    KeyOperate2();

    return 0;
}

////////////////////// 函数定义

//当同时按下多个按键时，一次循环只会执行一个键盘的敲击
void KeyOperate1()
{
    while (1)
    {
        Sleep(10);
        //按下 Q 在 (2, 3) 种植第一张卡片
        if (KeyDown('Q'))
            Card(1, 2, 3);
        //按下 W 在鼠标所在的位置种植第二张卡片
        else if (KeyDown('W'))
            Card(2, MouseRow(), MouseCol());
        //按下 E 在铲除鼠标位置的植物
        else if (KeyDown('E'))
            Shovel(MouseRow(), MouseCol());
    }
}

//当同时按下多个按键时，一次循环会执行多个键盘的敲击
void KeyOperate2()
{
    while (1)
    {
        Sleep(10);
        //按下 Q 在 (2, 3) 种植第一张卡片
        if (KeyDown('Q'))
            Card(1, 2, 3);
        //按下 W 在鼠标所在的位置种植第二张卡片
        if (KeyDown('W'))
            Card(2, MouseRow(), MouseCol());
        //按下 E 在铲除鼠标位置的植物
        if (KeyDown('E'))
            Shovel(MouseRow(), MouseCol());
    }
}