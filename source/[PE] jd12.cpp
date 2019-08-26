
/*
 * Coding = utf-8
 * 日期：2019-08-10
 * 作者：向量
 * 阵型：【PE】经典十二炮
 * 编写难度：低
 * 目的：初步了解 C++ 语法，例如 for | if | if else | else ，
 * 学会使用一些基础函数：
 * ** Delay Prejudge
 * ** SelectCards
 * ** Pao Card RecoverPao
 */

//包括pvz.h头文件
#include "pvz.h"

////////////////////////////////// 函数声明

// 使用樱桃
// C++ 函数 声明|定义 规则：
// 返回类型 函数名(数据类型 参数名, 数据类型 参数名...)
void UseCherry(int row, int col)
{
    Delay(373 - 100); //延迟 273 cs
    Card("ytzd", row, col);
}

//////////////////////////////////// main C++ 主函数

// 接下来是入口函数 main, main 函数是 C++ 程序的入口函数，
// 每个程序必须有且仅只能有一个
// 程序将在 main 函数处开始执行
int main()
{
    //打开检查脚本机制，使用此函数会输出一些操作信息，增加一些错误提示
    OpenExamine();

    //选择 寒冰菇，模仿寒冰菇，咖啡豆，窝瓜，樱桃炸弹，南瓜头，小喷菇，阳光菇，胆小菇，向日葵 卡片
    SelectCards({{"hbg"}, {"Mhbg"}, {"kfd"}, {"wg"}, {"ytzd"}, {"jg"}, {"xpg"}, {"yyg"}, {"dxg"}, {"xrk"}});

    //对于1到20波的操作
    for (wave = 1; wave < 21; wave++)
    {
        //如果波数不是第十波和第二十波
        if (wave_not_in(10, 20))
        {
            Prejudge(-95, wave);   //预判95cs
            Pao({{2, 9}, {5, 9}}); //炮击{2, 9}, {5, 9}
            //如果是第九波和第十九波
            if (wave_in(9, 19))
            {
                //自动收尾
                //等待炮恢复，炮击{2, 9}, {5, 9}, {2, 9}, {5, 9}
                RecoverPao({{2, 9}, {5, 9}, {2, 9}, {5, 9}});

                //手动收尾：
                //SkipPao(4);
            }
        }
        else if (wave == 10)
        {
            Prejudge(-55, wave);
            Pao({{2, 9}, {5, 9}});
            //简单时间计算不提倡使用 RunningInThread,
            //此处是为示范当函数有参数时应如何使用 RunningInThread
            RunningInThread(UseCherry, 2, 9);
        }
        else //if(wave == 20)
        {
            Prejudge(-150, wave);
            Pao(4, 7);
            //自动收尾
            RecoverPao({{2, 9}, {5, 9}, {2, 9}, {5, 9}, {2, 9}, {5, 9}});
        }
    }

    //结束时需要写的一条语句，main函数返回值为0，程序正常退出
    return 0;
}

///////////////////////// 这里写函数定义(具体实现), 由于此程序较简单，所以这里没有任何内容