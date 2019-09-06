
// 此文件将介绍自动存冰对象和女仆秘籍对象

// 自动存冰对象 ：ice_filler
// 女仆秘籍对象 ：nv_pu_mi_ji
// 以上的对象是 CvZ 自带的，就是已经被创建好的，用户可以直接使用

#include "pvz.h"

//////////////////// 使用示例

int main()
{
    // 下面为了更清楚的观察现象，将配合 KeyConnect 进行使用

    // 将 ice_filler 的所有接口与 KeyConnect 相结合

    KeyConnect('Q', [&]() {
        ice_filler.start({{1, 2}, {2, 3}, {2, 4}}); //开始存冰
    });

    KeyConnect('W', [&]() {
        ice_filler.pause(); //暂停存冰
    });

    KeyConnect('E', [&]() {
        ice_filler.goOn(); //继续存冰
    });

    KeyConnect('R', [&]() {
        ice_filler.resetFillList({{2, 2}, {2, 5}, {2, 6}}); //重置存冰位置
    });

    KeyConnect('T', [&]() {
        ice_filler.coffee(); //使用咖啡豆
    });

    KeyConnect('Y', [&]() {
        ice_filler.stop(); //停止存冰
    });

    // 将 nv_pu_mi_ji 的所有接口与 KeyConnect 相结合

    KeyConnect('A', [&]() {
        nv_pu_mi_ji.start(); //开始女仆秘籍
    });
    
    KeyConnect('S', [&]() {
        nv_pu_mi_ji.pause(); //暂停女仆秘籍
    });

    KeyConnect('D', [&]() {
        nv_pu_mi_ji.goOn(); //继续女仆秘籍
    });

    KeyConnect('F', [&]() {
        nv_pu_mi_ji.stop(); //停止女仆秘籍
    });

    return 0;
}