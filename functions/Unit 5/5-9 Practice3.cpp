
// 此文件是第五单元的一个练习
// 此程序是一个读取内存进行全自动挂机的入门示范
// 阵型 ：传统四炮.改
// 打法 ：非冰即炸
// 在此程序中，我们不适用框架自带的修补坚果类，来自己编写自动修补坚果函数
// 有时，橄榄红眼可能会啃到大喷，这时需要我们使用倭瓜来减压
// 对于用炮、灰烬和冰，我们要利用 TryPao 的返回值来规划
// 对于气球，直接读取内存使用三叶草即可

#include "pvz.h"

using namespace pvz;

// 枚举卡片的对象序列
enum SEED
{
    ICE_SHROOM = 0,      //寒冰菇
    ICE_SHROOM_IMITATOR, //模仿寒冰菇
    COFFEE_BEEN,         //咖啡豆
    SQUASH,              //倭瓜
    CHERRY,              //樱桃炸弹
    CHILI,               //火爆辣椒
    PUMPKIN,             //南瓜头
    CLOVER,              //三叶草
    FUME_SHROOM,         //大喷菇
    PUFF_SHROOM          //小喷菇
};

// 使用三叶草
void UseClover();

// 修补南瓜头
void FixPumpkin();

// 修补大喷菇
void FixFumeShroom();

// 应对波数主要逻辑函数
void DealWave();

// 使用倭瓜
void UseSquash();

// 这个全局变量是用来控制补种大喷菇的，因为咖啡豆不能随便用
// 当然这里可以使用指针进行线程之间的数据传递，但是方法比这个要复杂一些
// 所以此处使用全局变量的方法
// 如果使用指针，那么 FixFumeShroom 函数原型需要写为 void FixFumeShroom(bool *);
// 当然这里还可以继承自动线程类来解决这个问题
bool coffee_been_usable = false;

int main()
{
    SelectCards({{"hbg"}, {"Mhbg"}, {"kfd"}, {"wg"}, {"ytzd"}, {"hblj"}, {"ngt"}, {"syc"}, {"dpg"}, {"xpg"}});

    WaitGameStart();

    ice_filler.start({{3, 6}, {4, 6}});
    dian_cai_placer.start({10}, {{1, 7}, {6, 7}});
    dian_cai_placer.resetProtectedPlantList({{1, 5}, {6, 5}});

    // 得到第一波出怪类型
    // 如果有舞王再进行女仆秘籍
    std::vector<int> zombie_type;
    GetWaveZombieType(zombie_type, 1);
    if (find(zombie_type.begin(), zombie_type.end(), 8) != zombie_type.end())
        nv_pu_mi_ji.start();

    RunningInThread(FixPumpkin);
    RunningInThread(UseClover);
    RunningInThread(UseSquash);
    RunningInThread(FixFumeShroom);

    SetLimitPaoSequence(false);

    for (wave = 1; wave < 21; wave++)
    {
        Prejudge(0, wave);
        // 冰杀小偷
        if (wave_in(10, 20))
        {
            Until(375 - 298 + 50);
            ice_filler.coffee();
        }
        else
            DealWave();

        // 第 9 19 波进行炮炸收尾
        if (wave_in(9, 19, 20))
        {
            coffee_been_usable = true;
            // 得到对应波数的僵尸类型
            GetWaveZombieType(zombie_type);
            // 如果发现有红眼再进行炮击
            if (find(zombie_type.begin(), zombie_type.end(), 32) != zombie_type.end())
                while (!TryPao({{2, 8.5}, {5, 8.5}}))
                    Sleep(100);
            // 等待倒计时结束
            while (pvz::Countdown() > 150)
                Sleep(5);
            if (wave_in(9, 19))
            {
                while (pvz::Countdown() > 4)
                    Sleep(5);
                while (pvz::HugeWaveCountdown() > 150)
                    Sleep(5);
            }
            coffee_been_usable = false;
        }
    }

    WaitGameEnd();
    return 0;
}

void UseClover()
{
    SeedMemory clover_seed(CLOVER); // 首先创建一个读取三叶草卡片内存信息的对象
    ZombieMemory ballon_zombie;     // 再创建一个读取气球内存信息的对象

    int index, zombie_count_max;
    while (1)
    {
        Sleep(100);
        if (clover_seed.isUsable() && !GamePaused())
        {
            // 遍历全场僵尸
            zombie_count_max = ballon_zombie.countMax();
            for (index = 0; index < zombie_count_max; index++)
            {
                ballon_zombie.setIndex(index);
                // 如果气球僵尸快飞到家了
                if (ballon_zombie.isExist() &&
                    ballon_zombie.type() == 16 &&
                    !ballon_zombie.isDead() &&
                    ballon_zombie.abscissa() <= 0.5 * 80)
                {
                    // 种植三叶草
                    Card(CLOVER + 1, 2, 7);
                    break;
                }
            }
        }
    }
}

void FixPumpkin()
{
    SeedMemory pumpkin_seed(PUMPKIN); // 首先创建一个读取倭瓜卡片内存信息的对象
    PlantMemory pumpkin;              //再创建一个读取植物内存信息的对象

    int index, plant_count_max;
    while (1)
    {
        Sleep(100);
        if (pumpkin_seed.isUsable())
        {
            // 遍历全场植物
            plant_count_max = pumpkin.countMax();
            for (index = 0; index < plant_count_max; index++)
            {
                pumpkin.setIndex(index);
                // 如果发现南瓜该修补了
                if (!pumpkin.isCrushed() &&
                    !pumpkin.isDisappeared() &&
                    pumpkin.hp() < 2 * 4000 / 3 &&
                    pumpkin.type() == 30)
                {
                    // 种植南瓜
                    Card(PUMPKIN + 1, pumpkin.row() + 1, pumpkin.col() + 1);
                    break;
                }
            }
        }
    }
}

void FixFumeShroom()
{
    SeedMemory fume_shroom_seed(FUME_SHROOM);
    SeedMemory coffee_been_seed(COFFEE_BEEN);
    while (1)
    {
        Sleep(100);
        if (coffee_been_usable && fume_shroom_seed.isUsable() && coffee_been_seed.isUsable())
        {
            if (GetPlantIndex(1, 5) == -1)
                Card({{FUME_SHROOM + 1, 1, 5}, {COFFEE_BEEN + 1, 1, 5}});
            else if (GetPlantIndex(6, 5) == -1)
                Card({{FUME_SHROOM + 1, 6, 5}, {COFFEE_BEEN + 1, 6, 5}});
        }
    }
}

void DealWave()
{
    // 此处模拟人为反应
    Until(50);

    // 辣椒和樱桃总是同时使用，所以只需要检测一个的就行
    static SeedMemory cherry_seed(CHERRY);
    // static SeedMemory chili_seed(CHILI);

    // 如果尝试发炮不成功，即现在没有可以发的炮
    if (!TryPao({{2, 8.5}, {5, 8.5}}))
    {
        // 此时还有 440 - 50 - 100 = 290 cs 的时间
        // 如果灰烬能够使用
        if (cherry_seed.isUsable() || 5000 - cherry_seed.CD() < 280)
        {
            Until(440 - 10);
            // 使用辣椒樱桃
            Card(CHILI + 1, 5, 7);
            Card(CHERRY + 1, {{2, 8}, {2, 7}});
        }
        else // 灰烬没有恢复只能冰杀
            ice_filler.coffee();
    }
}

void UseSquash()
{
    SeedMemory squash_seed(SQUASH);
    ZombieMemory zombie;

    struct
    {
        float min_abscissa;
        int min_row;
    } zombie_min_position{800, 0};

    int index = 0, zombie_count_max = 0, zombie_type = 0, zombie_row = 0;
    float zombie_abscissa = 0.0;
    GRID use_squash_grid{0, 0};

    while (1)
    {
        Sleep(100);
        zombie_min_position = {800, 0};
        if (squash_seed.isUsable())
        {
            zombie_count_max = zombie.countMax();
            for (index = 0; index < zombie_count_max; index++)
            {
                zombie.setIndex(index);

                if (zombie.isExist() &&
                    !zombie.isDead() &&
                    zombie.hp() + zombie.oneHp() > 500)
                {
                    zombie_type = zombie.type();
                    zombie_abscissa = zombie.abscissa();
                    zombie_row = zombie.row() + 1;
                    if (zombie_type == 7)
                        zombie_abscissa += 40;
                    if ((zombie_type == 23 || zombie_type == 32 || zombie_type == 7) &&
                        (zombie_row == 1 || zombie_row == 6))
                    {
                        if (zombie_abscissa < zombie_min_position.min_abscissa)
                        {
                            zombie_min_position.min_abscissa = zombie_abscissa;
                            zombie_min_position.min_row = zombie_row;
                        }
                    }
                }
            }

            // 如果检测到了僵尸
            if (zombie_min_position.min_row != 0)
            {
                use_squash_grid = {zombie_min_position.min_row, int(zombie_min_position.min_abscissa / 80)};

                // 规定种植范围
                if (use_squash_grid.col > 9)
                    use_squash_grid.col = 9;
                if (use_squash_grid.col < 6)
                    use_squash_grid.col = 6;
                if (IsPlantable(use_squash_grid.row, use_squash_grid.col))
                {
                    Shovel(use_squash_grid.row, use_squash_grid.col);
                    Card(SQUASH + 1, use_squash_grid.row, use_squash_grid.col);
                }
            }
        }
    }
}