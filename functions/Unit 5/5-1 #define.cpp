/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \CppVsZombies\functions\Unit 5\5-1 #define.cpp
 * @Date: 2019-09-12 20:34:19
 * @LastEditTime : 2020-01-30 12:52:16
 */

// 此文件将介绍一个非常重要的指令 #define

#include "pvz.h"

// 所有 ARRAY_SIZE 都会被 10 替换
#define ARRAY_SIZE 10

#define NUM_7 3 + 4

int main()
{
    int array_[ARRAY_SIZE]; // 这条语句与 int array_[10]; 等价

    printf("%d\n", 3 + 4 * 3);
    system("pause");
    return 0;
}