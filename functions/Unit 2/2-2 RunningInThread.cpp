
// 此文件介绍子线程的创建

#include "pvz.h"

// 假如我们想每隔 110cs 种第一个卡槽的植物
// 然后每隔 130cs 种第二个卡槽的植物，这两个动作是厅室进行的
// 如果想把这两个操作写在一个线程是比较复杂的，所以此时我们需要创建子线程

///////////////////////////// 使用示例

// 首先我们需要写两个函数

//每隔 110cs 种一个植物
void Plant110()
{
    int col = 1;
    while (col < 5)
    {
        Delay(110);
        Card(2, 2, col++);
    }
}

//每隔 130cs 种一个植物
void Plant130()
{
    int col = 1;
    while (col < 5)
    {
        Delay(130);
        Card(1, 1, col++);
    }
}

//每隔 150cs 种一个植物
void Plant150(int row, int col)
{
    while (col < 10)
    {
        Delay(150);
        Card(3, row, col++);
    }
}

int main()
{
    // 首先如果直接调用上面两个函数肯定达不到我们预期的效果
    // 因为肯定是一个种完了另一个才种
    // 所以此时我们需要创建子线程

    // 将 Plant110 设定在子线程里运行
    RunningInThread(Plant110);

    // Plant130 在主线程里运行   main 即是主线程
    Plant130();

    // 下面是带参数的函数书写形式
    RunningInThread(Plant150, 5, 2); // 如果 Plant150 在主线程运行，书写形式为 Plant150(5, 2);

    return 0;
}