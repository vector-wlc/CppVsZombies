
// 此文件主要介绍按键绑定和鼠标位置读取用法

// KeyConnect : 按键绑定
// MouseRow MouseCol : 鼠标位置读取

#include "pvz.h"

using namespace pvz; //直接使用名称空间 pvz 的所有的标识符

// 上句对于此程序来说亦可以使用下列语句
// using pvz::MouseCol;
// using pvz::MouseRow;

////////////////// 使用示例

int main()
{
    //将 Q键 与操作 "在(2, 3) 放置第一张卡片" 绑定起来
    //即 按下 Q键 就会在(2, 3) 放置第一张卡片
    //此函数第二个参数形式为 ：[&](){ // Write code here ;}
    //[&]()为固定形式，不用了解其具体含义，只要写上就行，你需要注意的是{}里写的操作！！！
    KeyConnect('Q', [&]() { Card(1, 2, 3); });

    //按下 W键 就会将第二张卡片放置在鼠标光标所在的位置
    //并且会将炮发射在鼠标光标所在位置
    KeyConnect('W', [&]() {
        Card(2, MouseRow(), MouseCol());
        Pao(MouseRow(), MouseCol());
    });

    //按下 E键 就会铲除鼠标光标所在位置的植物
    KeyConnect('E', [&]() { Shovel(MouseRow(), MouseCol()); });

    return 0;
}