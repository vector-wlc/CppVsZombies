// 包含一些全局变量

#include "libpvz.h"
#include "pvz_preparation.h"
#include "pvz_error.h"

namespace pvz
{
// global
int g_scene;               //游戏场景
float g_DPI;               //DPI
int g_examine_level = 1;   //检查等级 : CVZ_ERROR
bool g_auto_exit = true;   //是否自动退出
bool g_error_exit = false; //是否因错误退出
std::mutex g_mu;           //互斥锁
HWND g_hwnd;
HANDLE g_handle;
int g_pvzbase;      //基址
int wave = 1;       //记录波数
int g_mainobject;   //游戏对象地址
int g_mouse_offset; //鼠标偏移地址

//  time
int zombie_refresh_time = 0;

//  card
std::map<std::string, int> seed_name_to_index;
bool is_get_seed_index = false;
std::string seed_name[11][8] =
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

// class
std::vector<std::pair<char, std::function<void()>>> key_operations;
FillIce ice_filler;
FixNut nut_fixer;
NvPuMiJi nv_pu_mi_ji;
PlaceDianCai dian_cai_placer;
PaoOperator pao_cvz(false);
CollectItem item_collector;

//此对象会一直在游戏退出战斗界面时才会析构
CvZPreparation cvz_preparation;

void KeyConnect(char key, const std::function<void()> &operate)
{
    if (key >= 'a' && key <= 'z')
        key -= 32;

    for (const auto &key_operation : key_operations)
        if (key_operation.first == key)
            PrintError("按键 %c 绑定了多个操作", key);

    key_operations.push_back(std::pair<char, std::function<void()>>(key, operate));

    static bool running = false;
    if (!running)
    {
        running = true;
        RunningInThread([&]() {
            while (true)
            {
                Sleep(10);
                for (const auto &key_operation : key_operations)
                    if (GetAsyncKeyState(key_operation.first) & 0x8001 == 0x8001)
                        key_operation.second();
            }
        });
    }
}
} // namespace pvz