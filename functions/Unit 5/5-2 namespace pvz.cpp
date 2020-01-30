/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \CppVsZombies\functions\Unit 5\5-2 namespace pvz.cpp
 * @Date: 2019-09-12 21:41:19
 * @LastEditTime : 2020-01-13 16:27:31
 * @Description: 
 */

// 此文件用于说明名称空间 pvz

#include "pvz.h"

// 我们先定义一个名称空间

namespace test
{

int x;
float y;
void fun()
{
    printf("我是一个函数");
}

} // namespace test

int main()
{
    test::x = 1;    // 将名称空间 test 内的 x 变量赋值为 1
    test::y = 3.14; // 将名称空间 test 内的 y 变量赋值为 3.14
    test::fun();    // 调用名称空间 test 内的 fun 函数

    // 如果觉得以上每次都要写 test:: 太麻烦，你也可以使用 using
    using namespace test; // 将 test 内的内容全部导入
    x = 2;
    y = 14.37;

    // 但是上述做法有个缺点，就是将 test 内的内容全部导入了，有时候我们并不想这样，所以可以写成下面这样
    using test::fun; // 直接使用 test 中的 fun     这种做法是最为推荐的

    fun(); // 调用名称空间 test 内的 fun 函数

    return 0;
}