
// 此文件将非常简单的介绍 C++类和对象 的相关知识

#include "pvz.h"

/////////////////////// 关于类和对象更进一步的知识，需要看一些 C++类和对象 的相关教程

int main()
{
    // 首先我们先创建一个自动修补坚果对象
    // 我的修补坚果对象
    FixNut my_nut_fixer;
    // 语法上类似于非常基础的定义变量 int x;

    // FixNut 是类
    // my_nut_fixer 是对象

    // 下面我们可以用创建的 my_nut_fixer 搞一些事情

    // 让 my_nut_fixer 开始在 {1, 3}, {1, 4} 修补小坚果
    my_nut_fixer.start(3, {{1, 3}, {1, 4}});

    // 让 my_nut_fixer 暂停修补小坚果
    my_nut_fixer.pause();

    // 让 my_nut_fixer 继续修补小坚果
    my_nut_fixer.goOn();

    // 重置 my_nut_fixer 的修补范围为 {2, 3}, {3, 4}
    my_nut_fixer.resetFixList({{2, 3}, {3, 4}});

    // 让 my_nut_fixer 停止修补小坚果
    my_nut_fixer.stop();

    return 0;
}