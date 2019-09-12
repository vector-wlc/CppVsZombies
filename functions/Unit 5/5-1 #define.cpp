
// 此文件将介绍一个非常重要的指令 #define

#include "pvz.h"

// 所有 ARRAY_SIZE 都会被 10 替换
#define ARRAY_SIZE 10

int main()
{
    int array_[ARRAY_SIZE]; // 这条语句与 int array_[10]; 等价

    // #define 广泛用于 CvZ 框架中
    // 主要用于函数与对象的过度
    return 0;
}