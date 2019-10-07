
#include "libpvz.h"
#include "pvz_global.h"

namespace pvz
{
/* place */
PlaceMemory::PlaceMemory()
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0x11C);
    index = 0;
}

PlaceMemory::PlaceMemory(int i)
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0x11C);
    index = i;
}

void PlaceMemory::getOffset()
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0x11C);
}

/*  seed  */
SeedMemory::SeedMemory()
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0x144);
    index = 0;
}

SeedMemory::SeedMemory(int i)
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0x144);
    index = i;
}

void SeedMemory::getOffset()
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0x144);
}

/*  plant  */
PlantMemory::PlantMemory()
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0xAC);
    index = 0;
}

PlantMemory::PlantMemory(int i)
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0xAC);
    index = i;
}

void PlantMemory::getOffset()
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0xAC);
}

/* zombie */
ZombieMemory::ZombieMemory()
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0x90);
    index = 0;
}

ZombieMemory::ZombieMemory(int i)
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0x90);
    index = i;
}

void ZombieMemory::getOffset()
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0x90);
}

ItemMemory::ItemMemory()
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0xE4);
    index = 0;
}

ItemMemory::ItemMemory(int i)
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0xE4);
    index = i;
}

void ItemMemory::getOffset()
{
    offset = ReadMemory<int>(0x6A9EC0, 0x768, 0xE4);
}

/* 一些常用的函数 */

//等待游戏开始
void WaitGameStart()
{
    while (GameUi() != 3)
        Sleep(1);
    //为获取卡片对象序列函数争取时间
    Sleep(100);
}

//等待游戏结束
void WaitGameEnd()
{
    while (GameUi() == 3)
        Sleep(10);
}

//获取指定类型植物的卡槽对象序列 植物类型与图鉴顺序相同，从0开始
//返回的卡片对象序列范围：[0,9]
int GetSeedIndex(int type, bool imitator)
{
    SeedMemory seed;
    int slot_count = seed.slotsCount();
    for (int i = 0; i < slot_count; i++)
    {
        seed.setIndex(i);
        if (imitator)
        {
            if ((seed.type() == 48) && (seed.imitatorType() == type))
                return i;
        }
        else if (seed.type() == type)
            return i;
    }
    return -1;
}

//得到指定位置和类型的植物对象序列
//当参数type为默认值-1时该函数无视南瓜花盆荷叶
//使用示例：
//GetPlantIndex(3,4)------如果三行四列有除南瓜花盆荷叶之外的植物时，返回该植物的对象序列，否则返回-1
//GetPlantIndex(3,4,47)---如果三行四列有春哥，返回其对象序列，否则返回-1
//GetPlantIndex(3,4,33)---如果三行四列有花盆，返回其对象序列，否则返回-1
int GetPlantIndex(int row, int col, int type)
{

    PlantMemory plant;
    int plants_count_max = plant.countMax();
    for (int i = 0; i < plants_count_max; i++)
    {
        plant.setIndex(i);
        if (type == -1) //如果植物存在	且不为南瓜花盆荷叶
        {
            if ((!plant.isDisappeared()) && (!plant.isCrushed()) &&
                (plant.row() + 1 == row) && (plant.col() + 1 == col) &&
                (plant.type() != 16) && (plant.type() != 30) && (plant.type() != 33))
                return i; //返回植物的对象序列
        }
        else //如果植物存在且植物类型一致
        {
            if ((!plant.isDisappeared()) && (!plant.isCrushed()) &&
                (plant.row() + 1 == row) && (plant.col() + 1 == col) &&
                (plant.type() == type))
                return i; //返回植物的对象序列
        }
    }
    return -1; //没有符合要求的植物返回-1
}

//检查僵尸是否存在
//使用示例
//ExamineZombieExist()-------检查场上是否存在僵尸
//ExamineZombieExist(23)-------检查场上是否存在巨人僵尸
//ExamineZombieExist(-1,4)-----检查第四行是否有僵尸存在
//ExamineZombieExist(23,4)-----检查第四行是否有巨人僵尸存在
bool ExamineZombieExist(int type, int row)
{
    ZombieMemory zombie;
    int zombies_count_max = zombie.countMax();

    for (int i = 0; i < zombies_count_max; i++)
    {
        zombie.setIndex(i);
        if (zombie.isExist() && !zombie.isDead())
            if (type < 0 && row < 0)
                return true;
            else if (type >= 0 && row >= 0)
            {
                if (zombie.row() == row - 1 && zombie.type() == type)
                    return true;
            }
            else if (type < 0 && row >= 0)
            {
                if (zombie.row() == row - 1)
                    return true;
            }
            else //if (type >= 0 && row < 0)
                if (zombie.type() == type)
                return true;
    }

    return false;
}

//判断该格子是否能种植物
//该函数不适用于屋顶场景
//此函数只能判断此格子能不能种植物，但并不能判断此格子是否有植物
//使用示例：
//IsPlantable(3,3)-----3行3列如果能种植物(即使此格子有非南瓜花盆荷叶的植物)，返回true,否则返回false
bool IsPlantable(int row, int col, bool hui_jin)
{
    PlaceMemory place;

    int places_count_max = place.countMax();
    for (int i = 0; i < places_count_max; i++)
    {
        place.setIndex(i);
        //如果该位置存在弹坑或墓碑
        if (place.isExist() && (place.type() == 1 || place.type() == 2) &&
            place.row() == row - 1 && place.col() == col - 1)
            return false;
    }
    //如果该位置有冰道
    if (col * 80 + 30 > IceAbscissa(row - 1))
        return false;
    //如果不是灰烬
    if (!hui_jin)
    {
        ZombieMemory zombie;
        int ZCM = zombie.countMax();
        for (int i = 0; i < ZCM; i++)
        {
            zombie.setIndex(i);
            if (zombie.isExist() && !zombie.isDead())
            {
                int Ztype = zombie.type();
                int Zrow = zombie.row() + 1;
                float Zabsci = zombie.abscissa();
                //如果该行存在冰车
                if (Ztype == 12 && Zrow == row)
                {
                    if (Zabsci < col * 80 + 30)
                        return false;
                }
            }
        }
    }
    return true;
}

// !!!!!!!!!!，待修正
bool IsExplode(int row, int col)
{
    ZombieMemory zombie;
    int ZCM = zombie.countMax();
    for (int i = 0; i < ZCM; i++)
    {
        zombie.setIndex(i);
        if (zombie.isExist() && !zombie.isDead())
        {
            int Ztype = zombie.type();
            int Zrow = zombie.row() + 1;
            float Zabsci = zombie.abscissa();
            //如果存在小丑且开盒
            if (Ztype == 15 && zombie.status() == 16)
            {
                if (Zrow == row - 1 && Zabsci < col * 80 + 42 && Zabsci > col * 80 - 162)
                    return true;
                else if (Zrow == row && Zabsci < col * 80 + 61 && Zabsci > col * 80 - 181)
                    return true;
                else if (Zrow == row + 1 && Zabsci < col * 80 + 52 && Zabsci > col * 80 - 173)
                    return true;
            }
        }
    }

    return false;
}

// !!!!!!!!!!，待修正
bool IsHammering(int row, int col, bool pumpkin)
{
    int increased_distance = 0;

    if (pumpkin)
        increased_distance = 30;
    ZombieMemory zombie;
    int ZCM = zombie.countMax();
    for (int i = 0; i < ZCM; i++)
    {
        zombie.setIndex(i);
        if (!zombie.isDead() && zombie.isExist())
        {
            int Ztype = zombie.type();
            int Zrow = zombie.row();
            float Zabsci = zombie.abscissa();
            if (Zrow + 1 == row && (Ztype == 23 || Ztype == 32) &&
                Zabsci < (col + 0.60) * 80 + increased_distance && Zabsci > (col - 0.40) * 80)
            {
                //如果巨人举锤
                if (zombie.isHammering())
                    return true;
            }
        }
    }

    return false;
}

//得到僵尸出怪类型
void GetZombieType(std::vector<int> &type_list)
{
    type_list.clear();
    for (int i = 0; i < 33; i++)
        if (ReadMemory<bool>(g_mainobject + 0x54D4 + i))
            type_list.push_back(i);
}

//得到相应波数的出怪类型
void GetWaveZombieType(std::vector<int> &zombie_type, int _wave)
{
    int type = 0;
    zombie_type.clear();
    for (int i = (_wave - 1) * 50; i < _wave * 50; i++)
    {
        type = ReadMemory<int>(g_mainobject + 0x6B4 + i * 4);
        if (std::find(zombie_type.begin(), zombie_type.end(), type) == zombie_type.end())
            zombie_type.push_back(type);
    }
}
} // namespace pvz