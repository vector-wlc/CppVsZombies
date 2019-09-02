
// 此文件将介绍自动修补坚果类和自动修补坚果类对象，并且进一步介绍使用自己创建的对象时的一些细节

// 自动修补坚果类 ： FixNut
// 自动修补坚果类对象 ： nut_fixer

#include "pvz.h"

int main()
{
    // 在第二单元我们已经说过，一个自动线程函数至多只能创建一个线程，
    // 就是因为停止线程函数只有一个，如果创建线程函数能开多个线程
    // 那么停止线程函数将无法确定要关闭哪些线程，现在是时候解决这个问题了
    // 我们可以用修补坚果类创建多个对象，让每个对象单独修补一种植物，此问题就得到解决了

    // nut_fixer 是 CvZ 自带的修补坚果类对象，如果在一局游戏中我们需要同时修补两种坚果植物
    // 那么我们就需要再创建一个对象

    FixNut pumpkin_fixer; //修补南瓜对象

    // 下面为对象指定修补的种类和位置
    nut_fixer.start(23);
    pumpkin_fixer.start(30, {{1, 2}, {1, 3}});

    //下面用 KeyConnect 配合使用可以更方便的观察现象

    KeyConnect('Q', [&]() {
        nut_fixer.pause(); // 修补暂停
    });

    KeyConnect('A', [&]() {
        pumpkin_fixer.pause();
    });

    KeyConnect('W', [&]() {
        nut_fixer.goOn(); // 修补继续
    });

    KeyConnect('S', [&]() {
        pumpkin_fixer.goOn();
    });

    KeyConnect('E', [&]() {
        nut_fixer.resetFixList({{2, 1}, {2, 2}}); //重置修补位置
    });

    KeyConnect('D', [&]() {
        pumpkin_fixer.resetFixList({{2, 3}, {2, 4}, {2, 5}});
    });

    KeyConnect('R', [&]() {
        nut_fixer.resetFixHp(600); // 重置修补血线
    });

    KeyConnect('F', [&]() {
        pumpkin_fixer.resetFixHp(900);
    });

    KeyConnect('T', [&]() {
        nut_fixer.stop(); // 停止修补
    });

    KeyConnect('G', [&]() {
        pumpkin_fixer.stop();
    });

    pvz::WaitGameEnd(); // 这条语句必须有，否则用户自己创建的对象将被析构

    return 0;
}