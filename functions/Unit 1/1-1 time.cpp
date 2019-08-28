
// 此文件用于说明关于时间操作函数的用法

// Prejudge : 主要用于获取僵尸刷新时间点，调用此函数获得僵尸僵尸刷新时间后，
//            此函数会后续调用 Until, 注意：Prejudge 函数每一波只能调用一次，而且每波必须调用

// Until : 等待现在时间点与 Prejudge 获取的僵尸刷新时间点达到指定差值

// Delay : 延迟一个时间段

#include "pvz.h"

////////////////////////// 使用示例

int main()
{
    // Prejudge
    {
        // 读取第一波僵尸刷新时间点，读取完毕后等待现在的时间与僵尸刷新时间点差值达到 -95 时，
        // 进行此函数接下来的操作
        Prejudge(-95, 1);

        // 读取第三波僵尸刷新时间点，读取完毕后等待现在的时间与僵尸刷新时间点差值达到 200 时，
        // 进行此函数接下来的操作
        Prejudge(200, 3);
    }

    // Until
    {
        // 等待现在时间点与 Prejudge 获取的僵尸刷新时间点达到-55时，进行此函数接下来的操作
        Until(-55);

        // 等待现在时间点与 Prejudge 获取的僵尸刷新时间点达到300时，进行此函数接下来的操作
        Until(300);

        // 注意：使用 Until 不允许不符合时间先后的逻辑，例如以下代码是错误的
        // It's not allowed !
        {
            Until(500);
            Until(200);
        }
    }

    // Delay
    {

        //延迟 300cs 后进行接下来的操作
        Delay(300);
    }

    return 0;
}