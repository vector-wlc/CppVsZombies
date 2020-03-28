/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \pvz_h\source\pvz_card.cpp
 * @Date: 2019-10-10 23:37:48
 * @LastEditTime: 2020-03-22 16:13:37
 * @Description: 卡片操作函数的实现
 */

#include "libpvz.h"
#include "pvz_error.h"
#include "pvz_global.h"

namespace pvz
{

//选择单张卡片
//使用示例：
//ChooseCard(1,2)----选择1行2列的卡片（不是模仿者）
//ChooseCard(1,2,T)--选择1行2列的卡片（是模仿者）
void ChooseCard(const SEED_INDEX &seed)
{
    int x, y;
    if (seed.imitator)
    {
        while (!ReadMemory<bool>(g_pvzbase + 0x320, 0x94, 0x54)) //保证点上模仿者
        {
            LeftClick(550, 490);
            Sleep(200);
        }
        x = 160;
        y = 215;
    }
    else
    {
        x = 160;
        y = 50;
    }
    x += (seed.row - 1) * 70;
    y += (seed.col - 1) * 50;
    SeedClick(x, y);
}

void LetsRock()
{
    Sleep(200);
    SeedClick(565, 234); //点击LET'S ROCK
    Sleep(500);
    //出现警告框时
    while (ReadMemory<bool>(g_pvzbase + 0x320, 0x94, 0x54))
    {

        LeftClick(400, 320);
        RightClick(1, 1);
        Sleep(500);
    }
}

//进行一些对模仿者误点的处理
void DealWrongClickFromImitator()
{
    int zombie_cnt_max = pvz::ZombieMemory::countMax();
    pvz::ZombieMemory zombie_memory;
    for (int index = 0; index < zombie_cnt_max; ++index)
    {
        zombie_memory.setIndex(index);
        // 如果僵尸在模仿者旁边，将其移开
        if (zombie_memory.isExist() &&
            zombie_memory.abscissa() < 930 &&
            zombie_memory.ordinate() > 370)
        {
            pvz::WriteMemory<float>(930, pvz::g_mainobject + 0x90, 0x2C + 0x15C * index);
        }
    }
}

//选择多张卡片,根据卡片位置选择
void SelectCards(const std::vector<SEED_INDEX> &lst)
{
    if (g_examine_level == CVZ_IGNORE_TIME)
        return;

    while (ReadMemory<bool>(g_mainobject + 0x140, 0x88))
    {
        Sleep(1);
    }

    if (GameUi() != 2)
    {
        return;
    }

    DealWrongClickFromImitator();

    while (ReadMemory<int>(g_mainobject + 0x15C, 0x8) != 4250)
    {
        Sleep(1);
    }

    SeedMemory seed;
    int slots_count = seed.slotsCount();
    if (slots_count != lst.size())
    {
        PrintError("选择的卡片数目与卡槽位数目不符");
    }
    //开始选卡
    while (1)
    {
        for (const auto &e : lst)
            ChooseCard(e);
        Sleep(500);
        //检查Let's Rock的按钮是否亮起
        if (ReadMemory<int>(0x6A9EC0, 0x774, 0x88, 0x1C) == 255)
            break;
        //没有全部选上重选
        else
        {
            Sleep(500);
            for (int i = slots_count; i > 0; i--)
            {
                RightClick(1, 1);
                LeftClick(70, 100);
                Sleep(100);
            }
        }
    }
    LetsRock();

    //等待游戏开始
    WaitGameStart();
}

//选择多张卡片，根据卡片名称选择
void SelectCards(const std::vector<SEED_NAME> &lst)
{
    //将卡片的名称转换成卡片所在位置的形式

    std::vector<SEED_INDEX> seed_lst(lst.size());
    bool success = false;
    //记录卡片位序
    int i = 0;
    for (const auto &e : lst)
    {
        for (seed_lst[i].row = 0; seed_lst[i].row <= 11; seed_lst[i].row++)
        {
            //如果i超过边界
            if (seed_lst[i].row > 10)
                PrintError("SelectCards第 %d 个卡片的名称'%s'未被录入，请参考pvz_data.h的卡片命名", i, e.seed_name.c_str());
            for (seed_lst[i].col = 0; seed_lst[i].col < 8; seed_lst[i].col++)
            {
                if (e.seed_name == g_seed_name[seed_lst[i].row][seed_lst[i].col])
                {
                    //如果是模仿者卡片
                    if (seed_lst[i].row > 5)
                    {
                        //对信息进行处理
                        seed_lst[i].imitator = true;
                        seed_lst[i].row -= 6;
                    }
                    seed_lst[i].row++;
                    seed_lst[i].col++;
                    success = true;
                    break;
                }
            }

            //转换成功后直接进行下一个卡片的转化
            if (success)
            {
                success = false;
                break;
            }
        }
        i++;
    }

    //进行选卡
    SelectCards(seed_lst);
}

// 基础用卡函数
void UseSeed(int x, int row, float col)
{
    g_mu.lock();
    RightClick(1, 1);
    LeftClick(70, 50 + 50 * x);
    SceneClick(row, col);
    RightClick(1, 1);
    if (g_examine_level == CVZ_INFO)
        std::printf("	Plant seed(%d) at (%d, %3.2f)\n\n", x, row, col);
    g_mu.unlock();
}

void UseSeed(int x, const std::vector<CARD> &lst)
{
    g_mu.lock();
    RightClick(1, 1);
    LeftClick(70, 50 + 50 * x);
    if (g_examine_level == CVZ_INFO)
        std::printf("	Plant seed(%d) at ", x);
    for (const auto &e : lst)
    {
        SceneClick(e.row, e.col);
        if (g_examine_level == CVZ_INFO)
            std::printf("(%d, %3.2f) ", e.row, e.col);
    }
    if (g_examine_level == CVZ_INFO)
        std::printf("\n\n");
    RightClick(1, 1);
    g_mu.unlock();
}

//为卡片名称变量获取卡片对象序列
int GetSeedIndexFromSeedName(const std::string &seed_name)
{
    if (!g_is_get_seed_index)
    {
        SeedMemory seed;
        //等待游戏开始
        while (GameUi() != 3)
            Sleep(1);

        int seed_counts = seed.slotsCount();
        int seed_type;
        std::pair<std::string, int> seed_info;

        for (int i = 0; i < seed_counts; ++i)
        {
            //得到卡片类型
            seed.setIndex(i);
            seed_type = seed.type();
            //如果是模仿者卡片
            if (seed_type == 48)
            {
                seed_type = seed.imitatorType();
                seed_info.first = g_seed_name[seed_type / 8 + 6][seed_type % 8];
                seed_info.second = i;
            }
            else //if(seed_info != 48)
            {
                seed_info.first = g_seed_name[seed_type / 8][seed_type % 8];
                seed_info.second = i;
            }
            g_seed_name_to_index.insert(seed_info);
        }
        g_is_get_seed_index = true;
    }

    auto it = g_seed_name_to_index.find(seed_name);
    if (it == g_seed_name_to_index.end())
        PrintError("卡片名称'%s'未被录入pvz.h,或者您没有选择该卡片", seed_name.c_str());

    return it->second;
}

//使用卡片：单张 单位置
void Card(int x, int row, float col)
{
    if (g_examine_level != CVZ_NO)
    {
        SeedMemory seed(x - 1);
        int seed_CD = seed.CD();
        if (seed_CD)
            PrintError("第 %d 张卡片还有 %dcs 才能使用", x, seed.initialCD() - seed_CD);
    }
    UseSeed(x, row, col);
}

//使用卡片：多张 每张卡片只定义一个位置
void Card(const std::vector<CARD_INDEX> &lst)
{
    for (const auto &each : lst)
    {
        Card(each.x, each.row, each.col);
    }
}

//使用卡片：单张 多位置
void Card(int x, const std::vector<CARD> &lst)
{
    if (g_examine_level != CVZ_NO)
    {
        SeedMemory seed(x - 1);
        int seed_CD = seed.CD();
        if (seed_CD)
            PrintError("第 %d 张卡片还有 %dcs 才能使用", x, seed.initialCD() - seed_CD);
    }

    UseSeed(x, lst);
}

//根据名称使用卡片：单张单位置
void Card(const std::string &seed_name, int row, float col)
{
    int seed_index = GetSeedIndexFromSeedName(seed_name);
    Card(seed_index + 1, row, col);
}

//根据名称使用卡片：多张 每张卡片只定义一个位置
void Card(const std::vector<CARD_NAME> &lst)
{
    for (const auto &each : lst)
    {
        Card(each.name, each.row, each.col);
    }
}

void Card(const std::string &seed_name, const std::vector<CARD> &lst)
{
    int seed_index = GetSeedIndexFromSeedName(seed_name);
    Card(seed_index + 1, lst);
}

// 等待种子可用
void WaitSeedUsable(int seed_index)
{
    SeedMemory seed_memory(seed_index);
    while (!seed_memory.isUsable())
    {
        Sleep(1);
    }
}

void RecoverCard(int x, int row, float col)
{
    WaitSeedUsable(x - 1);
    UseSeed(x, row, col);
}

void RecoverCard(const std::vector<CARD_INDEX> &lst)
{
    for (const auto &seed : lst)
    {
        WaitSeedUsable(seed.x - 1);
    }

    for (const auto &each : lst)
    {
        UseSeed(each.x, each.row, each.col);
    }
}

void RecoverCard(int x, const std::vector<CARD> &lst)
{
    WaitSeedUsable(x - 1);
    UseSeed(x, lst);
}

void RecoverCard(const std::string &seed_name, int row, float col)
{
    int seed_index = GetSeedIndexFromSeedName(seed_name);
    RecoverCard(seed_index + 1, row, col);
}

void RecoverCard(const std::vector<CARD_NAME> &lst)
{
    std::vector<CARD_INDEX> card_indexs;
    CARD_INDEX card_index;
    for (const auto &each : lst)
    {
        card_index.x = GetSeedIndexFromSeedName(each.name) + 1;
        card_index.row = each.row;
        card_index.col = each.col;
        card_indexs.push_back(card_index);
    }
    RecoverCard(card_indexs);
}

void RecoverCard(const std::string &seed_name, const std::vector<CARD> &lst)
{
    int seed_index = GetSeedIndexFromSeedName(seed_name);
    RecoverCard(seed_index + 1, lst);
}

} // namespace pvz