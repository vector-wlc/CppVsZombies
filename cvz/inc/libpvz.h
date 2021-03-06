/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \CppVsZombies\pvz_h\include\libpvz.h
 * @Date: 2019-12-21 15:19:54
 * @LastEditTime: 2020-03-22 16:20:49
 * @Description: PvZ TAS Framework C++ vs. Zombies
 *               This framework references Python vs. Zombies
 *               @lmintlcx https://pvz.lmintlcx.com/scripts/pvz.py/
 */
#pragma once
#ifndef LIBPVZ_H
#define LIBPVZ_H

#include <cstdio>
#include <windows.h>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <initializer_list>
#include <algorithm>
#include <functional>

namespace pvz
{

// ########################  assist functions #################

extern int wave;

//如果波数在参数范围内
template <class... Args>
bool wave_in(Args... args)
{
    std::initializer_list<int> lst = {args...};
    for (auto e : lst)
        if (e == wave)
            return true;
    return false;
}

//如果波数在不参数范围内
template <class... Args>
bool wave_not_in(Args... args)
{
    std::initializer_list<int> lst = {args...};
    for (auto e : lst)
        if (e == wave)
            return false;
    return true;
}

//使函数在子线程中运行
template <class FUNC, class... Args>
inline void RunningInThread(FUNC func, Args... args)
{
    std::thread task(func, args...);
    task.detach();
}

//########## memory basic functions ###############

extern HANDLE g_handle;
extern uintptr_t g_mainobject;
extern uintptr_t g_pvzbase;

//读取内存函数
//ReadMemory<data type>(address);
//此函数有返回值，需要变量接收读取结果
//使用示例
//pvz::ReadMemory<int>(0x6A9EC0) -----读取基址
//pvz::ReadMemory<int>(0x6A9EC0, 0x768) ------读取当前游戏信息和对象
template <typename T, typename... Args>
T ReadMemory(Args... args)
{
    std::initializer_list<uintptr_t> lst = {static_cast<uintptr_t>(args)...};
    uintptr_t buff = 0;
    T result = T();
    for (auto it = lst.begin(); it != lst.end(); ++it)
        if (it != lst.end() - 1)
            ReadProcessMemory(g_handle, (const void *)(buff + *it), &buff, sizeof(buff), nullptr);
        else
            ReadProcessMemory(g_handle, (const void *)(buff + *it), &result, sizeof(result), nullptr);
    return result;
}

//改写内存函数
template <typename T, typename... Args>
void WriteMemory(T value, Args... args)
{
    std::initializer_list<uintptr_t> lst = {static_cast<uintptr_t>(args)...};
    uintptr_t buff = 0;
    for (auto it = lst.begin(); it != lst.end(); ++it)
        if (it != lst.end() - 1)
            ReadProcessMemory(g_handle, (const void *)(buff + *it), &buff, sizeof(buff), nullptr);
        else
            WriteProcessMemory(g_handle, (void *)(buff + *it), &value, sizeof(value), nullptr);
}

//########## memory classes ###############

//抽象内存类
class AbstractMemory
{
protected:
    uintptr_t offset; //地址偏移
    int index;        //对象序列/栈位/序号...

public:
    AbstractMemory() { offset = index = 0; }
    //设置对象序列
    void setIndex(int _index) { index = _index; }
    //重新得到地址偏移信息
    virtual void getOffset() = 0;
};

//卡片内存信息类
class SeedMemory : public AbstractMemory
{
public:
    SeedMemory(int _index);
    SeedMemory();

    virtual void getOffset();

    //返回卡槽数目
    int slotsCount()
    {
        return ReadMemory<int>(offset + 0x24);
    }

    //判断卡片是否可用
    bool isUsable()
    {
        return ReadMemory<bool>(offset + 0x70 + 0x50 * index);
    }

    //返回卡片冷却
    int CD()
    {
        return ReadMemory<int>(offset + 0x4C + index * 0x50);
    }

    //返回卡片总冷却时间
    int initialCD()
    {
        return ReadMemory<int>(offset + 0x50 + index * 0x50);
    }

    //返回模仿者卡片类型
    int imitatorType()
    {
        return ReadMemory<int>(offset + 0x60 + index * 0x50);
    }

    //返回卡片类型
    int type()
    {
        return ReadMemory<int>(offset + 0x5C + index * 0x50);
    }
};

class PlantMemory : public AbstractMemory
{
public:
    PlantMemory(int _index);
    PlantMemory();

    virtual void getOffset();

    //返回当前最大植物数目
    static int countMax()
    {
        return ReadMemory<int>(g_mainobject + 0xB0);
    }

    //返回下一个植物的栈位/编号/对象序列
    static int nextIndex()
    {
        return ReadMemory<int>(g_mainobject + 0xB8);
    }

    //返回植物所在行 数值范围：[0,5]
    int row()
    {
        return ReadMemory<int>(offset + 0x1C + 0x14C * index);
    }

    //返回植物所在列 数值范围：[0,8]
    int col()
    {
        return ReadMemory<int>(offset + 0x28 + 0x14C * index);
    }

    //判断植物是否消失
    bool isDisappeared()
    {
        return ReadMemory<bool>(offset + 0x141 + 0x14C * index);
    }

    //判断植物是否被压扁
    bool isCrushed()
    {
        return ReadMemory<bool>(offset + 0x142 + 0x14C * index);
    }

    //返回植物类型 与图鉴顺序一样，从0开始
    int type()
    {
        return ReadMemory<int>(offset + 0x24 + 0x14C * index);
    }

    //返回植物横坐标 数值范围：[0,800]
    int abscissa()
    {
        return ReadMemory<int>(offset + 0x8 + 0x14C * index);
    }

    //返回植物纵坐标
    int ordinate()
    {
        return ReadMemory<int>(offset + 0xC + 0x14C * index);
    }
    //返回植物血量
    int hp()
    {
        return ReadMemory<int>(offset + 0x40 + 0x14C * index);
    }

    //生效倒计时
    int activeCountdown()
    {
        return ReadMemory<int>(offset + 0x50 + 0x14C * index);
    }

    //发射子弹倒计时，该倒计时不论是否有僵尸一直不断减小并重置
    int bulletCountdown()
    {
        return ReadMemory<int>(offset + 0x58 + 0x14C * index);
    }

    //子弹发射倒计时，该倒计时只在有僵尸时才一直不断减小并重置
    int countdownBullet()
    {
        return ReadMemory<int>(offset + 0x90 + 0x14C * index);
    }

    //返回植物的状态
    //35：空炮
    //36：正在装填
    //37：准备就绪
    //38：正在发射
    int state()
    {
        return ReadMemory<int>(offset + 0x3C + 0x14C * index);
    }

    //植物属性倒计时
    int stateCountdown()
    {
        return ReadMemory<int>(offset + 0x54 + 0x14C * index);
    }
};

class ZombieMemory : public AbstractMemory
{
public:
    ZombieMemory(int _index);
    ZombieMemory();

    virtual void getOffset();

    //返回僵尸最大数目
    static int countMax()
    {
        return ReadMemory<int>(g_mainobject + 0x94);
    }

    //判断僵尸是否存在
    bool isExist()
    {
        return ReadMemory<short>(offset + 0x15A + 0x15C * index);
    }

    //返回僵尸所在行数 范围：[0,5]
    int row()
    {
        return ReadMemory<int>(offset + 0x1C + 0x15C * index);
    }

    //返回僵尸所在横坐标 范围：[0,800]
    float abscissa()
    {
        return ReadMemory<float>(offset + 0x2C + 0x15C * index);
    }

    //返回僵尸纵坐标
    float ordinate()
    {
        return ReadMemory<float>(offset + 0x30 + 0x15C * index);
    }

    //返回僵尸类型 与图鉴顺序一样，从0开始
    int type()
    {
        return ReadMemory<int>(offset + 0x24 + 0x15C * index);
    }

    //返回僵尸本体当前血量
    int hp()
    {
        return ReadMemory<int>(offset + 0xC8 + 0x15C * index);
    }

    //返回僵尸一类饰品当前血量
    int oneHp()
    {
        return ReadMemory<int>(offset + 0xD0 + 0x15C * index);
    }

    //返回僵尸二类饰品血量
    int twoHp()
    {
        return ReadMemory<int>(offset + 0xDC + 0x15C * index);
    }

    //判断僵尸是否啃食
    bool isEat()
    {
        return ReadMemory<bool>(offset + 0x51 + 0x15C * index);
    }

    //返回僵尸状态
    int state()
    {
        return ReadMemory<int>(offset + 0x28 + 0x15C * index);
    }

    //判断僵尸是否死亡
    bool isDead()
    {
        return state() == 1;
    }

    //判断巨人是否举锤
    bool isHammering()
    {
        return state() == 70;
    }

    //判断僵尸是否隐形
    bool isStealth()
    {
        return ReadMemory<bool>(offset + 0x18 + 0x15C * index);
    }

    //变化量(前进的舞王和减速的冰车等的前进速度)
    float speed()
    {
        return ReadMemory<float>(offset + 0x34 + 0x15C * index);
    }

    //僵尸已存在时间
    int existTime()
    {
        return ReadMemory<int>(offset + 0x60 + 0x15C * index);
    }

    //僵尸属性倒计时
    int stateCountdown()
    {
        return ReadMemory<int>(offset + 0x68 + 0x15C * index);
    }

    //判断僵尸是否消失
    bool isDisappeared()
    {
        return ReadMemory<bool>(offset + 0xEC + 0x15C * index);
    }

    //僵尸中弹判定的横坐标
    int bulletAbscissa()
    {
        return ReadMemory<int>(offset + 0x8C + 0x15C * index);
    }

    //僵尸中弹判定的纵坐标
    int bulletOrdinate()
    {
        return ReadMemory<int>(offset + 0x90 + 0x15C * index);
    }

    //僵尸攻击判定的横坐标
    int attackAbscissa()
    {
        return ReadMemory<int>(offset + 0x9C + 0x15C * index);
    }

    //僵尸攻击判定的纵坐标
    int attackOrdinate()
    {
        return ReadMemory<int>(offset + 0xA0 + 0x15C * index);
    }

    //僵尸减速倒计时
    int slowCountdown()
    {
        return ReadMemory<int>(offset + 0xAC + 0x15C * index);
    }

    //僵尸黄油固定倒计时
    int fixationCountdown()
    {
        return ReadMemory<int>(offset + 0xB0 + 0x15C * index);
    }

    //僵尸冻结倒计时
    int freezeCountdown()
    {
        return ReadMemory<int>(offset + 0xB4 + 0x15C * index);
    }
};

class PlaceMemory : public AbstractMemory
{
public:
    PlaceMemory(int _index);
    PlaceMemory();

    virtual void getOffset();

    //返回场景物品最大数目
    //包括：弹坑 墓碑 罐子等
    static int countMax()
    {
        return ReadMemory<int>(g_mainobject + 0x120);
    }

    //返回该格子物品的类型
    int type()
    {
        return ReadMemory<int>(offset + 0x8 + 0xEC * index);
    }

    //返回物品的行数 范围：[0,5]
    int row()
    {
        return ReadMemory<int>(offset + 0x14 + 0xEC * index);
    }

    //返回物品的列数 范围：[0,8]
    int col()
    {
        return ReadMemory<int>(offset + 0x10 + 0xEC * index);
    }

    //判断物品是否存在
    bool isExist()
    {
        return ReadMemory<short>(offset + 0xEA + 0xEC * index);
    }
};

class ItemMemory : public AbstractMemory
{
public:
    ItemMemory(int _index);
    ItemMemory();

    virtual void getOffset();

    //返回需要收集物品的数目
    //包括金币 钻石 礼盒等
    static int count()
    {
        return ReadMemory<int>(g_mainobject + 0xF4);
    }

    //返回需要收集物品的最大数目
    static int countMax()
    {
        return ReadMemory<int>(g_mainobject + 0xE8);
    }

    //判断物品是否消失
    bool isDisappeared()
    {
        return ReadMemory<bool>(offset + 0x38 + 0xD8 * index);
    }

    //判断物品是否被收集
    bool isCollected()
    {
        return ReadMemory<bool>(offset + 0x50 + 0xD8 * index);
    }

    //返回物品横坐标
    float abscissa()
    {
        return ReadMemory<float>(offset + 0x24 + 0xD8 * index);
    }

    //返回物品纵坐标
    float ordinate()
    {
        return ReadMemory<float>(offset + 0x28 + 0xD8 * index);
    }
};

//########## memory functions ###############

//返回鼠标所在行
inline int MouseRow()
{
    return ReadMemory<int>(g_mainobject + 0x13C, 0x28) + 1;
}

//返回鼠标所在列
inline float MouseCol()
{
    return 1.0 * ReadMemory<int>(g_pvzbase + 0x320, 0xE0) / 80;
}

//判断游戏是否暂停
inline bool GamePaused()
{
    return ReadMemory<bool>(g_mainobject + 0x164);
}

//获取游戏当前游戏时钟
inline int GameClock()
{
    return ReadMemory<int>(g_mainobject + 0x5568);
}

// 一个内部时钟, 可用于判断舞王/伴舞的舞蹈/前进
inline int DancerClock()
{
    return ReadMemory<int>(g_pvzbase + 0x838);
}

//返回刷新倒计时
inline int Countdown()
{
    return ReadMemory<int>(g_mainobject + 0x559c);
}

//返回大波刷新倒计时
inline int HugeWaveCountdown()
{
    return ReadMemory<int>(g_mainobject + 0x55A4);
}

//返回已刷新波数
inline int NowWave()
{
    return ReadMemory<int>(g_mainobject + 0x557c);
}

//返回初始刷新倒计时
inline int InitialCountdown()
{
    return ReadMemory<int>(g_mainobject + 0x55A0);
}

//返回一行的冰道坐标 范围：[0,800]
//使用示例：
//IceAbscissa(0)------得到第一行的冰道坐标
inline int IceAbscissa(int i)
{
    return ReadMemory<int>(g_mainobject + 0x60C + 4 * i);
}

//获取游戏信息
//1: 主界面, 2: 选卡, 3: 正常游戏/战斗, 4: 僵尸进屋, 7: 模式选择.
inline int GameUi()
{
    return ReadMemory<int>(g_pvzbase + 0x7FC);
}

//启动女仆秘籍
inline void StartMaidCheats()
{
    WriteMemory<byte>(0x71, 0x52DFE8);
}

//停止女仆秘籍
inline void StopMaidCheats()
{
    WriteMemory<byte>(0x7F, 0x52DFE8);
}

//等待游戏开始
void WaitGameStart();

//等待游戏结束
void WaitGameEnd();

//获取指定类型植物的卡槽对象序列 植物类型与图鉴顺序相同，从0开始
//返回的卡片对象序列范围：[0,9]
//GetSeedIndex(16)------------获得荷叶的卡槽对象序列
//GetSeedIndex(16,true)-------获得模仿者荷叶的卡槽对象序列
int GetSeedIndex(int type, bool imitator = false);

//得到指定位置和类型的植物对象序列
//当参数type为默认值-1时该函数无视南瓜花盆荷叶
//使用示例：
//GetPlantIndex(3,4)------如果三行四列有除南瓜花盆荷叶之外的植物时，返回该植物的对象序列，否则返回-1
//GetPlantIndex(3,4,47)---如果三行四列有春哥，返回其对象序列，如果有其他植物，返回-2，否则返回-1
int GetPlantIndex(int row, int col, int type = -1);

//用于储存位置信息
struct GRID
{
    int row;
    int col;

    GRID() : row(0), col(0) {}
    GRID(int row_, int col_) : row(row_), col(col_) {}
    friend const bool operator==(const GRID &grid1, const GRID &grid2)
    {
        return grid1.row == grid2.row && grid1.col == grid2.col;
    }

    friend const bool operator<(const GRID &grid1, const GRID &grid2)
    {
        if (grid1.row == grid2.row)
            return grid1.col < grid2.col;
        return grid1.row < grid2.row;
    }

    friend const bool operator>(const GRID &grid1, const GRID &grid2)
    {
        if (grid1.row == grid2.row)
            return grid1.col > grid2.col;
        return grid1.row > grid2.row;
    }
};

//得到一组指定位置的植物的对象序列
//参数1：填写一组指定位置
//参数2：填写指定类型
//参数3：得到对象序列，此函数按照位置的顺序填写对象序列
//注意：如果没有植物填写-1，如果有植物但是不是指定类型，会填写-2
void GetPlantIndexs(const std::vector<GRID> &grid_lst_in_,
                    int type_in_,
                    std::vector<int> &indexs_out_);

//检查僵尸是否存在
//使用示例
//ExamineZombieExist()-------检查场上是否存在僵尸
//ExamineZombieExist(23)-------检查场上是否存在巨人僵尸
//ExamineZombieExist(-1,4)-----检查第四行是否有僵尸存在
//ExamineZombieExist(23,4)-----检查第四行是否有巨人僵尸存在
bool ExamineZombieExist(int type = -1, int row = -1);

//判断该格子是否能种植物
//该函数不适用于屋顶场景
//此函数只能判断此格子能不能种植物，但并不能判断此格子是否有植物
//使用示例：
//IsPlantable(3,3)-----3行3列如果能种植物，默认为非灰烬植物(即使此格子有非南瓜花盆荷叶的植物)，返回true,否则返回false
//IsPlantable(3,3,true)----3行3列如果能种植物，为灰烬植物，返回true,否则返回false
//如果判定的植物是灰烬植物则会无视冰车
bool IsPlantable(int row, int col, bool hui_jin = false);

//检测巨人是否对该格子植物锤击
//IsHammering(3,4)------------如果该格子有巨人不同时举锤返回true，否则返回false.
bool IsHammering(int row, int col, bool pumpkin = false);

//检测该格子是否会被即将爆炸的小丑炸到
//使用示例：IsExplode(3,4)-------如果（3，4）会被小丑炸到返回true 否则返回false.
bool IsExplode(int row, int col);

//得到僵尸出怪类型
//注意：此函数最好在选卡界面使用
//参数为 vector 数组
//使用示例：
//std::vector<int>zombies_type;
//GetZombieType(zombies_type);
//僵尸的出怪类型将会储存在数组 zombies_type 中
void GetZombieType(std::vector<int> &zombie_type_list_out_);

//得到相应波数的出怪类型
//参数为 vector 数组
//使用示例：
//std::vector<int>zombies_type;
//GetWaveZombieType(zombies_type);-------得到当前波数出怪类型
//GetWaveZombieType(zombies_type, 1);-------得到第一波出怪类型
//僵尸的出怪类型将会储存在数组 zombies_type 中
void GetWaveZombieType(std::vector<int> &zombie_type_list_out_, int _wave_in_ = wave);

//设置出怪 此函数不管填不填蹦极都会在 wave 10 20 刷蹦极！！！！！！！！！！！！
//参数命名规则：前面两个字母与 PvZ Tools 出怪类型名称拼音首字母一致，后面接下划线和类型代号
//例如 铁桶命名为 TT_4
//使用示例：
//SetZombies({CG_3, TT_4, BC_12, XC_15, QQ_16, FT_21, TL_22, BY_23, HY_32, TT_18});
//设置出怪类型为：撑杆 铁桶 冰车 小丑 气球 扶梯 投篮 白眼 红眼 跳跳
//SetZombies({TT_4, BC_12, BC_12});
//设置出怪类型为：铁桶 冰车 并且两种僵尸的比例为 1：2
void SetZombies(const std::vector<int> &zombie_type);

//设置特定波出怪 此函数不管填不填蹦极都会在 wave 10 20 刷蹦极！！！！！！！！！！！！
//参数命名规则：前面两个字母与 PvZ Tools 出怪类型名称拼音首字母一致，后面接下划线和类型代号
//例如 铁桶命名为 TT_4
//使用示例：
//SetWaveZombies(1, {CG_3, TT_4, BC_12, XC_15, QQ_16, FT_21, TL_22, BY_23, HY_32, TT_18});
//设置第一波出怪类型为：撑杆 铁桶 冰车 小丑 气球 扶梯 投篮 白眼 红眼 跳跳
//SetWaveZombies(1, {TT_4, BC_12, BC_12});
//设置第一波出怪类型为：铁桶 冰车 并且两种僵尸的比例为 1：2
void SetWaveZombies(int _wave, const std::vector<int> &zombie_type);

// ########################### time #####################

//预判函数
//获取僵尸刷新时间戳
//除 wave 1 10 20 最小预判时间分别为 -550 -700 -700 之外，其他波次最小预判时间均为 -200
//使用示例：
//Prejudge(-95,wave)-----僵尸刷新前95cs进行接下来的操作
//Prejudge(100,wave)-----僵尸刷新后100cs进行接下来的操作
void Prejudge(int time, int wave);

//时间延迟函数（以游戏内部时钟为基准）
//使用示例：
//Delay(100)----延迟100cs
void Delay(int time);

//等待直至当前时间戳与本波刷新时间点的差值达到指定值
//使用该函数之前必须使用Prejudge函数
//使用示例：
//Until(-95)-----僵尸刷新前95cs进行接下来的操作
//Until(200)-----僵尸刷新后200cs进行接下来的操作
//注意：此函数使用顺序必须符合时间先后顺序！例如：
//Until(-95);
//Until(-100);
//这种用法是错误的！
void Until(int time);

//冰三函数
//注意：此函数在子线程运行
//使用示例：
//Ice3(298) --------- 修正冰三时间点至当前时刻的 298cs 后
void Ice3(int delay_time);

//######################## mouse and keyboard ###########################

//模拟鼠标左键操作
//使用示例：
//LeftClick(400,300)-----点击pvz窗口中央
//LeftClick(55+85,80*6)------点击泳池(1,6)
void LeftClick(int x, int y);

//模拟鼠标右键点击操作
//主要用于安全点击
void RightClick(int x, int y);

//模拟敲击空格键
void PressSpace();

//点击函数
//此函数自动识别场景，只用于战斗界面
//0: 白天, 1 : 黑夜, 2 : 泳池, 3 : 浓雾, 4 : 屋顶, 5 : 月夜, 6 : 蘑菇园, 7 : 禅境花园, 8 : 水族馆, 9 : 智慧树.
//使用示例：
//SceneClick(3, 4)------点击场地上3行4列
//SceneClick(3, 4, 10)-----点击场地上3行4列再向下偏移10px
void SceneClick(float row, float col, int offset = 0);

//模拟鼠标选卡点击操作
//延迟200ms后鼠标抬起
void SeedClick(int x, int y);

//铲除植物函数
//使用示例：
//Shovel(4, 6)--------铲除4行6列的植物,如果植物有南瓜保护默认铲除被保护植物
//Shovel(4, 6, true)---铲除4行6列的植物,如果植物有南瓜保护铲除南瓜
//Shovel({{3, 6},{4, 6}})
void Shovel(int row, float col, bool pumpkin = false);

//用于铲除函数
struct SHOVEL
{
    int row;
    float col;
    bool pumpkin = false;
};

//铲除植物函数
//使用示例：
//Shovel(4,6)--------铲除4行6列的植物,如果植物有南瓜保护默认铲除被保护植物
//Shovel(4,6,true)---铲除4行6列的植物,如果植物有南瓜保护铲除南瓜
//Shovel({{3, 6},{4, 6}})
void Shovel(const std::vector<SHOVEL> &lst);

//检测某键是否按下
//KeyDown('Q')--------若Q键按下，返回true，否则返回false
//while(1)
//{Sleep(1); if(KeyDown('Q')) Card("ytzd",2,9);}--------不断检测Q键状态，一旦Q键按下，则在二行九列使用樱桃炸弹
//注意此函数参数书写形式为单引号
inline bool KeyDown(int key)
{
    if (key > 96 && key < 123)
        key -= 32;
    return GetAsyncKeyState(key) & 0x8001 == 0x8001;
}

//键盘控制操作
//keyConnect('Q', [&](){ Pao(2, 9);}) 按下Q就炮击(2, 9)
//使用此函数时请在 main 函数结尾时尽量使用 pvz::WaitGameEnd()
void KeyConnect(char key, const std::function<void()> &operate);

//用于选卡函数(根据卡片对象序列选择)
struct SEED_INDEX
{
    int row;
    int col;
    bool imitator = false;
};

//用于选卡函数(根据卡片名称选择)
struct SEED_NAME
{
    std::string seed_name;
};

//选择卡片用来战斗
//使用示例：
//SelectCards({ {2,7},{2,7,true},{5,4},{2,8},{3,1},{3,2},{1,3},{4,4},{4,7},{2,1} });
//true为使用模仿者,默认不使用
//卡片名称为拼音首字母，具体参考pvz_data.h的命名
//选择 寒冰菇，模仿寒冰菇，咖啡豆，窝瓜，樱桃炸弹，南瓜头，小喷菇，阳光菇，胆小菇，向日葵 卡片
//SelectCards({{"hbg"}, {"Mhbg"}, {"kfd"}, {"wg"}, {"ytzd"}, {"ngt"}, {"xpg"}, {"yyg"}, {"dxg"}, {"xrk"}});
void SelectCards(const std::vector<SEED_INDEX> &lst);

//选择卡片用来战斗
//使用示例：
//SelectCards({ {2,7},{2,7,true},{5,4},{2,8},{3,1},{3,2},{1,3},{4,4},{4,7},{2,1} });
//true为使用模仿者,默认不使用
//卡片名称为拼音首字母，具体参考pvz_data.h的命名
//选择 寒冰菇，模仿寒冰菇，咖啡豆，窝瓜，樱桃炸弹，南瓜头，小喷菇，阳光菇，胆小菇，向日葵 卡片
//SelectCards({{"hbg"}, {"Mhbg"}, {"kfd"}, {"wg"}, {"ytzd"}, {"ngt"}, {"xpg"}, {"yyg"}, {"dxg"}, {"xrk"}});
void SelectCards(const std::vector<SEED_NAME> &lst);

//用于用卡函数
struct CARD
{
    int row;
    float col;
};

//用于用卡函数(根据对象序列用卡)
struct CARD_INDEX
{
    int x;
    int row;
    float col;
};

//用于选卡函数(根据名称用卡)
struct CARD_NAME
{
    std::string name;
    int row;
    float col;
};

//用卡函数
//使用示例：
//Card(1, 2, 3)---------选取第1张卡片，放在2行,3列
//Card({{1, 2, 3}, {2, 3, 4}})------选取第1张卡片，放在2行,3列，选取第2张卡片，放在3行,4列
//Card(1, {{2, 3}, {3, 4}})--------选取第1张卡片，优先放在2行,3列，其次放在3行,4列
//以下用卡片名称使用Card,卡片名称为拼音首字母，具体参考pvz_data.h的命名
//Card({{"ytzd", 2, 3}, {"Mhblj", 3, 4}})---------选取樱桃卡片，放在2行,3列，选取辣椒卡片，放在3行,4列
void Card(const std::vector<CARD_INDEX> &lst);
void Card(int x, int row, float col);
void Card(int x, const std::vector<CARD> &lst);
void Card(const std::string &seed_name, int row, float col);
void Card(const std::vector<CARD_NAME> &lst);
void Card(const std::string &seed_name, const std::vector<CARD> &lst);

//等待卡片恢复立即用卡
//使用方法与 Card 相同
//需要注意的是一次使用多张卡片时会等待卡片全部恢复时才开始种植
void RecoverCard(const std::vector<CARD_INDEX> &lst);
void RecoverCard(int x, const std::vector<CARD> &lst);
void RecoverCard(int x, int row, float col);
void RecoverCard(const std::vector<CARD_NAME> &lst);
void RecoverCard(const std::string &seed_name, int row, float col);
void RecoverCard(const std::string &seed_name, const std::vector<CARD> &lst);

// 炮操作类
class PaoOperator
{
private:
    //记录炮的信息
    struct PAO_INFO
    {
        int recover_time;            //恢复时间
        int index;                   //炮的对象序列
        bool is_in_list = false;     //记录是否在炮列表内
        bool is_in_sequence = false; //记录该炮是否被炮序限制
        bool is_shoveled = false;    //是否被铲除
    };

    using PaoIterator = std::map<GRID, PAO_INFO>::iterator;

    //用于发炮函数
    struct PAO
    {
        int row;
        float col;
    };

    //用于RAWPAO函数
    struct RAW_PAO
    {
        int pao_row;
        int pao_col;
        int drop_row;
        float drop_col;
    };

    //记录炮的位置和落点的位置及炮弹飞行时间
    struct RP
    {
        PaoIterator it;
        int drop_row;
        float drop_col;
        int fire_time;
    };

    //屋顶炮飞行时间辅助数据
    struct FLY_TIME
    {
        int min_drop_x;   //记录该列炮最小飞行时间对应的最小的横坐标
        int min_fly_time; //记录最小的飞行时间
    };

    static int conflict_resolution_type;          //冲突解决方式
    static std::map<GRID, PAO_INFO> all_pao_list; //所有炮的信息
    std::vector<PaoIterator> pao_list;            //炮列表，记录炮的迭代器信息
    int next_pao;                                 //记录当前即将发射的下一门炮
    bool limit_pao_sequence = true;               //是否限制炮序

    static const FLY_TIME fly_time_data[8];
    //禁用 = 运算符
    void operator=(PaoOperator) {}
    //对炮进行一些检查
    static void pao_examine(PaoIterator it, int now_time, int drop_row, float drop_col);
    //检查落点
    static bool is_drop_conflict(int pao_row, int pao_col, int drop_row, float drop_col);
    //基础发炮函数
    static void base_fire_pao(PaoIterator it, int now_time, int drop_row, float drop_col);
    //获取屋顶炮飞行时间
    static int get_roof_fly_time(int pao_col, float drop_col);
    //基础屋顶修正时间发炮
    static void base_fire_roof_pao(PaoIterator it, int fire_time, int drop_row, float drop_col);
    //用户自定义炮位置，屋顶修正时间发炮：多发
    static void base_fire_roof_paos(std::vector<RP> lst);
    //铲种炮
    static void shovel_and_plant_pao(int row, int col, int move_col, PaoIterator it, int delay_time);
    //增加炮的信息
    static void add_pao_message(int row, int col);
    //种炮
    static void plant_pao(int row, int col);
    //得到炮的所有信息
    static void get_all_pao_message();

public:
    //冲突类型
    enum ConflictType
    {
        DROP_POINT,
        COLLECTION
    };

    //改变炮的信息
    //请在手动或使用基础函数例如 Card 改变炮的信息后立即使用此函数
    //使用此函数后需要注意对炮列表的隐形改变
    //使用示例：
    //changePaoMessage(2,3,2,3)--------在手动铲种(2,3)位置的炮后，更改相关炮的信息
    //changePaoMessage(2,3,2,4)--------手动位移铲种(2,3)位置的炮后，更改相关炮的信息
    //changePaoMessage(0,0,2,3)--------手动增加(2,3)位置的炮后，更改相关炮的信息
    //changePaoMessage(2,3,0,0)--------手动删除(2,3)位置的炮后，更改相关炮的信息
    static void changePaoMessage(int origin_row, int origin_col, int now_row, int now_col);

    //设定解决冲突模式
    //使用示例：
    //setResolveConflictType(PaoOperator::DROP_POINT)---只解决落点冲突，不解决收集物点炮冲突
    //setResolveConflictType(PaoOperator::COLLECTION)---只解决收集物点炮冲突，不解决落点冲突
    static void setResolveConflictType(int type);

    //发炮函数：用户自定义位置发射，屋顶修正飞行时间发炮.
    //注意：尽量不要使用此函数操作位于炮列表中的炮，因为使用此函数后自动识别的炮序与UpdatePaolist更新的炮序将无效！
    //使用示例：
    //rawRoofPao(1,2,2,9)-----------------------将位置为（1，2）的炮发射到（2，9）
    //rawRoofPao({ {1,2,2,9},{1,3,5,9}})-------将位置为（1，2）的炮发射到（2，9），将位置为（1，3）的炮发射到（5，9）
    static void rawRoofPao(int pao_row, int pao_col, int drop_row, float drop_col);

    //发炮函数：用户自定义位置发射，屋顶修正飞行时间发炮.
    //注意：尽量不要使用此函数操作位于炮列表中的炮，因为使用此函数后自动识别的炮序与UpdatePaolist更新的炮序将无效！
    //使用示例：
    //rawRoofPao(1,2,2,9)-----------------------将位置为（1，2）的炮发射到（2，9）
    //rawRoofPao({ {1,2,2,9},{1,3,5,9}})-------将位置为（1，2）的炮发射到（2，9），将位置为（1，3）的炮发射到（5，9）
    static void rawRoofPao(const std::vector<RAW_PAO> &lst);

    //铲炮：该函数不能铲除有炮序限制的炮，除非强制
    //注意：使用此函数后需要注意对炮列表的隐形改变
    //使用示例
    //shovelPao(2, 3)-------铲除 (2, 3) 位置的炮，并且更新炮的信息
    //shovelPao(2, 3, true)-------无视炮序，强制铲除 (2, 3) 位置的炮，并且更新炮的信息
    static void shovelPao(int row, int col, bool is_forced = false);

    //种炮
    //注意：使用此函数后需要注意对炮列表的隐形改变
    //此函数不会检查目标位置是否有植物和是否可以种植植物
    //使用示例
    //plantPao(2, 3)--------在 (2, 3) 位置种植玉米投手及玉米炮，并录入炮的信息
    static void plantPao(int row, int col)
    {
        RunningInThread(plant_pao, row, col);
    }

    //铲种炮函数  已不推荐使用  注：shovelPao plantPao 灵活性更高
    //该函数不能铲除有炮序限制的炮，除非强制
    //使用示例：
    //fixPao(3,4)------------铲种(3,4)位置的炮
    //fixPao(3,4,1)--------铲掉(3,4)位置的炮，将炮种植在(3,4+1)位置上/位移铲种炮
    //fixPao(3,4,0,0,true)--------无视炮序，强制铲种(3,4)位置的炮
    static void fixPao(int row, int col, int move_col = 0, int delay_time = 0, bool is_forced = false);

    //发炮函数：用户自定义位置发射
    //注意：尽量不要使用此函数操作位于有炮序炮列表中的炮，因为使用此函数后自动识别的炮序与 resetPaolist 更新的炮序将无效！
    //使用示例：
    //rawPao(1,2,2,9)-----------------------将位置为（1，2）的炮发射到（2，9）
    //rawPao({{1, 2, 2, 9}, {1, 3, 5, 9}})-------将位置为（1，2）的炮发射到（2，9），将位置为（1，3）的炮发射到（5，9）
    static void rawPao(int pao_row, int pao_col, int drop_row, float drop_col);

    //发炮函数：用户自定义位置发射
    //注意：尽量不要使用此函数操作位于有炮序炮列表中的炮，因为使用此函数后自动识别的炮序与 resetPaolist 更新的炮序将无效！
    //使用示例：
    //rawPao(1,2,2,9)-----------------------将位置为（1，2）的炮发射到（2，9）
    //rawPao({{1, 2, 2, 9}, {1, 3, 5, 9}})-------将位置为（1，2）的炮发射到（2，9），将位置为（1，3）的炮发射到（5，9）
    static void rawPao(const std::vector<RAW_PAO> &lst);

    //炮的状态
    struct PAO_STATUS
    {
        int row;          //所在行
        int col;          //所在列
        int cd;           //剩余的冷却时间
        int index;        //对象序列
        bool is_shoveled; //是否被铲
    };

    //得到场上所有炮的状态
    static void getAllPaoStatusList(std::vector<PAO_STATUS> &lst_out_);

    //得到场上现在所有能用的炮的位置列表
    static void getAllRecoveredPaoList(std::vector<GRID> &lst_out_);

    PaoOperator(const std::vector<GRID> &lst);
    PaoOperator();
    ~PaoOperator();

    //////////////////////////////////////////// 模式设定成员

    //设置炮序限制 参数为 false 则解除炮序限制，true 则增加炮序限制
    //解除此限制后 fixPao 可铲种炮列表内的炮，tryPao 系列可使用， Pao 系列不可使用
    //增加此限制后 fixPao 不可铲种炮列表内的炮，tryPao 系列不可使用， Pao 系列可使用
    void setLimitPaoSequence(bool limit);

    /////////////////////////////////////////// 下面是关于限制炮序的相关成员

    //设置即将发射的下一门炮
    //此函数只有在限制炮序的时候才可调用
    //使用示例：
    //setNextPao(10)------将炮列表中第十门炮设置为下一门即将发射的炮
    //setNextPao(2, 8)------将炮列表中位于 (2, 8) 的炮设置为下一门即将发射的炮
    void setNextPao(int next_pao);

    //设置即将发射的下一门炮
    //此函数只有在限制炮序的时候才可调用
    //使用示例：
    //setNextPao(10)------将炮列表中第十门炮设置为下一门即将发射的炮
    //setNextPao(2, 8)------将炮列表中位于 (2, 8) 的炮设置为下一门即将发射的炮
    void setNextPao(int row, int col);

    //跳炮函数
    //使用示例：
    //skipao(2)---跳过按照顺序即将要发射的2门炮
    void skipPao(int x)
    {
        next_pao = (next_pao + x) % pao_list.size();
    }

    //发炮函数
    //使用示例：
    //pao(2,9)----------------炮击二行，九列
    //pao({ {2,9},{5,9} })-----炮击二行，九列，五行，九列
    void pao(int row, float col);

    //发炮函数
    //使用示例：
    //pao(2,9)----------------炮击二行，九列
    //pao({ {2,9},{5,9} })-----炮击二行，九列，五行，九列
    void pao(const std::vector<PAO> &lst);

    //发炮函数 炮CD恢复自动发炮
    //使用示例：
    //recoverPao(2,9)----------------炮击二行，九列
    //recoverPao({ {2,9},{5,9} })-----炮击二行，九列，五行，九列
    void recoverPao(int row, float col);

    //发炮函数 炮CD恢复自动发炮
    //使用示例：
    //recoverPao(2,9)----------------炮击二行，九列
    //recoverPao({ {2,9},{5,9} })-----炮击二行，九列，五行，九列
    void recoverPao(const std::vector<PAO> &lst);

    //屋顶修正飞行时间发炮. 此函数开销较大不适合精确键控.
    //此函数只适用于RE与ME 修正时间：387cs
    //使用示例：
    //roofPao(3,7)---------------------修正飞行时间后炮击3行7列
    //roofPao({ {2,9},{5,9} })---------修正飞行时间后炮击2行9列,5行9列
    void roofPao(int row, float col);

    //屋顶修正飞行时间发炮. 此函数开销较大不适合精确键控.
    //此函数只适用于RE与ME 修正时间：387cs
    //使用示例：
    //roofPao(3,7)---------------------修正飞行时间后炮击3行7列
    //roofPao({ {2,9},{5,9} })---------修正飞行时间后炮击2行9列,5行9列
    void roofPao(const std::vector<PAO> &lst);

    //////////////////////////////////////// 下面是不限制炮序能够使用的成员

    //自动找炮函数
    //使用示例
    //tryPao(2,9)----------------在炮列表中找到可用的炮后，炮击二行，九列
    //tryPao({{2, 9}, {5, 9}})-----在炮列表中找到可用的炮后，炮击二行，九列，五行，九列
    bool tryPao(int row, float col);

    //自动找炮函数
    //使用示例
    //tryPao(2,9)----------------在炮列表中找到可用的炮后，炮击二行，九列
    //tryPao({{2, 9}, {5, 9}})-----在炮列表中找到可用的炮后，炮击二行，九列，五行，九列
    bool tryPao(const std::vector<PAO> &lst);

    //自动找炮函数
    //使用示例
    //tryRoofPao(2,9)----------------在炮列表中找到可用的炮后，炮击二行，九列
    //tryRoofPao({{2, 9},{5, 9}})-----在炮列表中找到可用的炮后，炮击二行，九列，五行，九列
    bool tryRoofPao(int row, float col);

    //自动找炮函数
    //使用示例
    //tryRoofPao(2,9)----------------在炮列表中找到可用的炮后，炮击二行，九列
    //tryRoofPao({{2, 9},{5, 9}})-----在炮列表中找到可用的炮后，炮击二行，九列，五行，九列
    bool tryRoofPao(const std::vector<PAO> &lst);

    //尝试铲种炮函数
    //使用示例：
    //tryFixPao()------------尝试铲种即将发射的下一门炮，该函数不支持位移铲种，也不支持延迟时间，因为没有意义
    void tryFixPao();

    //发炮函数 炮CD恢复自动尝试发炮
    //使用示例：
    //tryRecoverPao(2,9)----------------炮击二行，九列
    //tryRecoverPao({ {2,9},{5,9} })-----炮击二行，九列，五行，九列
    void tryRecoverPao(int row, float col);

    //发炮函数 炮CD恢复自动尝试发炮
    //使用示例：
    //tryRecoverPao(2,9)----------------炮击二行，九列
    //tryRecoverPao({ {2,9},{5,9} })-----炮击二行，九列，五行，九列
    void tryRecoverPao(const std::vector<PAO> &lst);

    ////////////////////////////////// 下面是不受模式限制使用的成员

    //重置炮列表
    //使用示例:
    //resetPaoList({{3, 1},{4, 1},{3, 3},{4, 3}})-------经典四炮
    //resetPaoList()-------将场上所有剩余的未在炮列表的炮全部重置到此列表
    void resetPaoList(const std::vector<GRID> &lst);

    //重置炮列表
    //使用示例:
    //resetPaoList({{3, 1},{4, 1},{3, 3},{4, 3}})-------经典四炮
    //resetPaoList()-------将场上所有剩余的未在炮列表的炮全部重置到此列表
    void resetPaoList();

    //得到本列表中能用的炮
    void getRecoveredPaoList(std::vector<GRID> &lst_out_);

    //得到本炮列表中炮的状态
    void getPaoStatusList(std::vector<PAO_STATUS> &lst_out_);
};

//CvZ自定义炮类对象
extern PaoOperator pao_cvz;

// ################# auto thread ########################

//自动操作线程基类
class BaseAutoThread
{
protected:
    bool is_paused;        //暂停，线程仍在运行
    bool is_stoped;        //停止，线程不再运行
    int time_interval;     //检测间隔
    void error_messages(); //错误信息

public:
    BaseAutoThread() : is_paused(false), is_stoped(true), time_interval(100) {}
    virtual ~BaseAutoThread() {}
    //调用此函数使得线程停止运行
    void stop() { is_stoped = true; }
    //调用此函数使得线程暂停运行
    void pause() { is_paused = true; }
    //调用此函数使得线程继续运行
    void goOn() { is_paused = false; }
    //调用此函数改变检测间隔
    void setInterval(int interval) { time_interval = interval; }
};

//自动存用冰类
//public成员函数介绍：
//start：开始存冰，使用此函数会开辟一个线程用于存冰，使用方法与StartAutoFillIceThread相同
//stop：停止存冰，使用此函数会使得存冰线程停止运行
//pause：存冰暂停，使用此函数会暂停存冰任务，但是线程在不断运行
//goOn：继续存冰，使用此函数使得存冰线程继续工作
//resetList：重置存冰列表，使用方法与StartAutoFillIceThread相同
//coffee：使用咖啡豆，激活存冰，使用方法与Coffee相同
class FillIce : public BaseAutoThread
{
private:
    std::vector<GRID> grid_lst;
    void use_ice();
    int coffee_seed_index;

public:
    FillIce() : BaseAutoThread() { time_interval = 5; }
    ~FillIce() {}

    //重置存冰位置
    //使用示例：
    //resetFillList({{3,4},{5,6}})-----将存冰位置重置为{3，4}，{5，6}
    void resetFillList(const std::vector<GRID> &lst);

    //线程开始工作
    //使用示例：
    //start({{3,4},{5,6}})-----在{3，4}，{5，6}位置存冰
    void start(const std::vector<GRID> &lst);

    //使用咖啡豆函数
    //使用示例：
    //coffee()-----自动使用优先级低的存冰位
    void coffee() const;
};

//自动存用冰类对象：使用此对象可以使您更愉快的管理存冰
//public成员函数介绍：
//start：开始存冰，使用此函数会开辟一个线程用于存冰，使用方法与StartAutoFillIceThread相同
//stop：停止存冰，使用此函数会使得存冰线程停止运行
//pause：存冰暂停，使用此函数会暂停存冰任务，但是线程在不断运行
//goOn：继续存冰，使用此函数使得存冰线程继续工作
//resetList：重置存冰列表，使用方法与StartAutoFillIceThread相同
//coffee：使用咖啡豆，激活存冰，使用方法与Coffee相同
extern FillIce ice_filler;

class FixPlant : public BaseAutoThread
{
public:
    FixPlant() : BaseAutoThread() {}
    ~FixPlant() {}

    //重置植物修补位置
    //使用示例：
    //resetFixList({{2,3},{3,4}})-------位置被重置为{2，3}，{3，4}
    void resetFixList(const std::vector<GRID> &lst) { grid_lst = lst; } // 这里不再进行任何检查

    //自动得到修补的位置列表
    void autoGetFixList();

    //线程开始工作，此函数开销较大，不建议多次调用
    //第一个参数为植物类型
    //第二个参数不填默认全场
    //第三个参数不填默认植物血量为150以下时修补
    //使用示例：
    //start(23)-------修补全场的高坚果
    //start(30,{{1,3},{2,3}})-----修补位置为{1，3}，{2，3}位置的南瓜头
    //start(3,{{1,3},{2,3}},300)------修补位置为{1，3}，{2，3}位置的坚果，血量降至300开始修补
    void start(int _plant_type, const std::vector<GRID> &lst = {}, int _fix_hp = 150);

    //重置修补血量
    //使用示例：
    //resetFixHp(200)------将修补触发血量改为200
    void resetFixHp(int _fix_hp);

    //是否使用咖啡豆
    void isUseCoffee(bool _is_use_coffee) { is_use_coffee = _is_use_coffee; }

private:
    bool is_use_coffee = false;
    int plant_type;
    int fix_hp = 0;
    int coffee_seed_index;
    int leaf_seed_index;
    std::vector<int> seed_index_vec;
    std::vector<GRID> grid_lst;
    void get_seed_list();
    void update_plant();
    bool use_seed(int seed_index, int row, float col, bool is_need_shovel);
};

extern FixPlant nut_fixer;

//自动放置垫材类
//start：开始放置垫材，使用此函数会开辟一个线程用于放置垫材，使用方法与StartAutoSetDianCaiThread相同
//stop：停止放置垫材，使用此函数会使得放置垫材线程停止运行
//pause：放置垫材暂停，使用此函数会暂停放置垫材任务，但是线程在不断运行
//goOn：继续放置垫材，使用此函数使得放置垫材线程继续工作
//resetSetDianCaiList：重置垫材优先放置列表
//resetProtectedPlantList：重置保护植物位置列表
//resetDianCaiSeedList：重置被用来当作垫材的植物
//resetZombieTypeList：重置需要垫的僵尸
class PlaceDianCai : public BaseAutoThread
{
private:
    //记录一些放置垫材的信息
    struct PLACE_MESSAGE
    {
        int plant_max_abscissa;    //记录植物最大的横坐标值
        float zombie_min_abscissa; //记录巨人僵尸最小的横坐标值
        int place_max_col;         //记录放置的最大列数
        bool is_plantble;          //是否要种植物
    };

    std::vector<PLACE_MESSAGE> place_message_list;
    std::vector<int> diancai_plant_type_list;
    std::vector<int> prevent_zombie_type_list;
    PlantMemory plant;
    ZombieMemory zombie;
    void get_min_absci_zombie();
    GRID need_diancai_grid() const;
    void auto_plant_diancai();

public:
    PlaceDianCai();
    ~PlaceDianCai(){};

    //开启放置垫材线程
    //此函数开销较大，不建议多次调用
    //使用示例：
    //start({1,2,3,4})------将第 1 2 3 4 张卡片设为垫材，默认全场都从第九列开始垫，默认只垫 红眼 白眼
    //start({1,2,3,4},{{2,6},{4,5}})-----将第 1 2 3 4 张卡片设为垫材，只垫第 2 4 行，而且第二行从第六列开始垫，第四行从第五列开始垫
    //start({1,2,3,4},{{2,6},{4,5}},{23,32,7})----将第 1 2 3 4 张卡片设为垫材，只垫第 2 4 行，而且第二行从第六列开始垫，第四行从第五列开始垫,垫红白眼和橄榄
    void start(const std::vector<int> &lst_plant,
               const std::vector<GRID> &lst_grid = {},
               const std::vector<int> &lst_zombie = {23, 32});

    //重置保护植物列数
    //使用示例：
    //resetProtectedPlantList()-----默认将要保护的植物的位置重置为每行的最靠前位置
    //resetProtectedPlantList({{2,3},{3,4}})------将要保护的植物的位置重置为{2,3},{3,4}
    //注意：此函数必须配合要垫的行数，如果出现重置的行数不在要垫的范围内，就会报错
    void resetProtectedPlantList(const std::vector<GRID> &lst = {});

    //重置被当作垫材的植物
    //使用示例：
    //resetDianCaiSeedList({1,2,3})----将第1 2 3张卡片设置为垫材
    void resetDianCaiSeedList(const std::vector<int> &lst) { diancai_plant_type_list = lst; }

    //重置要垫的僵尸类型
    //使用示例：
    //resetZombieTypeList({23,32,0})-----将要垫的僵尸改为 白眼 红眼 普僵
    void resetZombieTypeList(const std::vector<int> &lst) { prevent_zombie_type_list = lst; }

    //重置垫材放置位置列表
    //使用示例：
    //resetSetDianCaiList({{2,8},{3,9}})-----将开始种垫材的位置重置为{2，8}，{3，9}
    void resetSetDianCaiList(const std::vector<GRID> &lst);
    void resetSetDianCaiList();
};

extern PlaceDianCai dian_cai_placer;

class CollectItem : public BaseAutoThread
{
public:
    CollectItem();
    ~CollectItem(){};
    void start();

private:
    void start_auto_collect() const;
};

extern CollectItem item_collector;

// ########################  mode select #################

enum ExamineType
{
    CVZ_NO,
    CVZ_ERROR,
    CVZ_INFO,
    CVZ_IGNORE_TIME
};

//开关函数：开启合理性检查（一些比较复杂的检查由此函数来控制）
//pvz::CVZ_NO : 不进行开销较大的检查
//pvz::CVZ_ERROR : 进行开销较大的检查
//pvz::CVZ_INFO : 输出操作信息
//pvz::CVZ_IGNORE_TIME : 进行无视时间调试
void OpenExamine(int level = CVZ_INFO);

// ################ enumeration of plants and zombies #########

// 植物类型
enum PlantType
{
    WDSS_0,   // 豌豆射手
    XRK_1,    // 向日葵
    YTZD_2,   // 樱桃炸弹
    JG_3,     // 坚果
    TDDL_4,   // 土豆地雷
    HBSS_5,   // 寒冰射手
    DZH_6,    // 大嘴花
    SCSS_7,   // 双重射手
    XPG_8,    // 小喷菇
    YGG_9,    // 阳光菇
    DPG_10,   // 大喷菇
    MBTSZ_11, // 墓碑吞噬者
    MHG_12,   // 魅惑菇
    DXG_13,   // 胆小菇
    HBG_14,   // 寒冰菇
    HMG_15,   // 毁灭菇
    HY_16,    // 荷叶
    WG_17,    // 倭瓜
    SFSS_18,  // 三发射手
    CRHZ_19,  // 缠绕海藻
    HBLJ_20,  // 火爆辣椒
    DC_21,    // 地刺
    HJSZ_22,  // 火炬树桩
    GJG_23,   // 高坚果
    SBG_24,   // 水兵菇
    LDH_25,   // 路灯花
    XRZ_26,   // 仙人掌
    SYC_27,   // 三叶草
    LJSS_28,  // 裂荚射手
    YT_29,    // 杨桃
    NGT_30,   // 南瓜头
    CLG_31,   // 磁力菇
    JXCTS_32, // 卷心菜投手
    HP_33,    // 花盆
    YMTS_34,  // 玉米投手
    KFD_35,   // 咖啡豆
    DS_36,    // 大蒜
    YZBHS_37, // 叶子保护伞
    JZH_38,   // 金盏花
    XGTS_39,  // 西瓜投手
    JQSS_40,  // 机枪射手
    SZXRK_41, // 双子向日葵
    YYG_42,   // 忧郁菇
    XP_43,    // 香蒲
    BXGTS_44, // 冰西瓜投手
    XJC_45,   // 吸金磁
    DCW_46,   // 地刺王
    YMJNP_47  // 玉米加农炮
};

enum ZombieType
{
    PJ_0 = 0,  // 普僵
    QZ_1,      // 旗帜
    LZ_2,      // 路障
    CG_3,      // 撑杆
    TT_4,      // 铁桶
    DB_5,      // 读报
    TM_6,      // 铁门
    GL_7,      // 橄榄
    WW_8,      // 舞王
    BW_9,      // 伴舞
    YZ_10,     // 鸭子
    QS_11,     // 潜水
    BC_12,     // 冰车
    XQ_13,     // 雪橇
    HT_14,     // 海豚
    XC_15,     // 小丑
    QQ_16,     // 气球
    KG_17,     // 矿工
    TT_18,     // 跳跳
    XR_19,     // 雪人
    BJ_20,     // 蹦极
    FT_21,     // 扶梯
    TL_22,     // 投篮
    BY_23,     // 白眼
    XG_24,     // 小鬼
    JB_25,     // 僵博
    HY_32 = 32 // 红眼
};

} // namespace pvz

#endif //!LIBPVZ_H