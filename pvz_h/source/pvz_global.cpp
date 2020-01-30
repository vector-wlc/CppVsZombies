/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \pvz_h\source\pvz_global.cpp
 * @Date: 2019-10-10 23:49:51
 * @LastEditTime : 2020-01-27 15:52:32
 * @Description: 包含 CvZ 用到的所有全局变量               
 */

#include "libpvz.h"
#include "pvz_preparation.h"
#include "pvz_error.h"

namespace pvz
{

int wave = 1; //记录波数

int g_scene;             //游戏场景
float g_DPI;             //DPI
int g_examine_level = 1; //检查等级 : CVZ_ERROR
std::mutex g_mu;         //互斥锁
HWND g_hwnd;
HANDLE g_handle;
uintptr_t g_pvzbase;    //基址
uintptr_t g_mainobject; //游戏对象地址

//  time
int g_zombie_refresh_time = 0;

//  card
std::map<std::string, int> g_seed_name_to_index;
bool g_is_get_seed_index = false;
std::string g_seed_name[11][8] =
    {
        //豌豆射手，向日葵，樱桃炸弹，坚果，寒冰射手，大嘴花，双重射手
        {"wdss", "xrk", "ytzd", "jg", "tddl", "hbss", "dzh", "scss"},
        //小喷菇，阳光菇，大喷菇，墓碑吞噬者，魅惑菇，胆小菇，寒冰菇，毁灭菇
        {"xpg", "ygg", "dpg", "mbtsz", "mhg", "dxg", "hbg", "hmg"},
        //荷叶，窝瓜，三发射手，缠绕海藻，火爆辣椒，地磁，火炬树桩，高坚果
        {"hy", "wg", "sfss", "crhz", "hblj", "dc", "hjsz", "gjg"},
        //水兵菇，路灯花，仙人掌，三叶草，裂荚射手，杨桃，南瓜头，磁力菇
        {"sbg", "ldh", "xrz", "syc", "ljss", "yt", "ngt", "clg"},
        //卷心菜投手，花盆，玉米投手，咖啡豆，大蒜，叶子保护伞，金盏花，西瓜投手
        {"jxcts", "hp", "ymts", "kfd", "ds", "yzbhs", "jzh", "xgts"},
        //机枪射手，双子向日葵，忧郁菇，香蒲，冰西瓜投手，吸金磁，地刺王，玉米加农炮
        {"jqss", "szxrk", "yyg", "xp", "bxgts", "xjc", "dcw", "ymjnp"},

        //以下为模仿者卡片名称
        //豌豆射手，向日葵，樱桃炸弹，坚果，寒冰射手，大嘴花，双重射手
        {"Mwdss", "Mxrk", "Mytzd", "Mjg", "Mtddl", "vhbss", "Mdzh", "Mscss"},
        //小喷菇，阳光菇，大喷菇，墓碑吞噬者，魅惑菇，胆小菇，寒冰菇，毁灭菇
        {"Mxpg", "Mygg", "Mdpg", "Mmbtsz", "Mmhg", "Mdxg", "Mhbg", "Mhmg"},
        //荷叶，窝瓜，三发射手，缠绕海藻，火爆辣椒，地磁，火炬树桩，高坚果
        {"Mhy", "Mwg", "Msfss", "Mcrhz", "Mhblj", "Mdc", "Mhjsz", "Mgjg"},
        //水兵菇，路灯花，仙人掌，三叶草，裂荚射手，杨桃，南瓜头，磁力菇
        {"Msbg", "Mldh", "Mxrz", "Msyc", "Mljss", "Myt", "Mngt", "Mclg"},
        //卷心菜投手，花盆，玉米投手，咖啡豆，大蒜，叶子保护伞，金盏花，西瓜投手
        {"Mjxcts", "Mhp", "Mymts", "Mkfd", "Mds", "Myzbhs", "Mjzh", "Mxgts"}};

// keyboard
std::vector<std::pair<char, std::function<void()>>> g_key_operations;

// class
FillIce ice_filler;
FixPlant nut_fixer;
PlaceDianCai dian_cai_placer;
PaoOperator pao_cvz;
CollectItem item_collector;
CvZPreparation cvz_preparation;
} // namespace pvz