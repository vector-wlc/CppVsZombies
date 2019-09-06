
// 此文件主要说明自动放置垫材对象的用法

#include "pvz.h"

////////////////////// 使用示例

int main()
{
    KeyConnect('Q', [&]() {
        dian_cai_placer.start({1, 2, 3}); // 开始放置垫材
    });

    KeyConnect('W', [&]() {
        dian_cai_placer.resetDianCaiSeedList({2, 3}); //重置垫材植物列表
    });

    KeyConnect('E', [&]() {
        dian_cai_placer.pause(); // 暂停
    });

    KeyConnect('R', [&]() {
        dian_cai_placer.goOn(); // 继续
    });

    KeyConnect('T', [&]() {
        dian_cai_placer.resetProtectedPlantList({{2, 6}, {5, 7}}); // 重置受保护植物的位置
    });

    KeyConnect('Y', [&]() {
        dian_cai_placer.resetSetDianCaiList({{1, 6}, {6, 8}}); // 重置放置垫材位置
    });

    KeyConnect('U', [&]() {
        dian_cai_placer.resetZombieTypeList({23}); //重置要垫的僵尸的种类
    });

    KeyConnect('I', [&]() {
        dian_cai_placer.stop(); // 停止垫垫材
    });

    return 0;
}