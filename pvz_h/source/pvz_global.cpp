
#include "pvz_global.h"

namespace pvz
{
int g_scene;                   //游戏场景
float g_DPI;                   //DPI
bool g_high_precision = false; //是否开启高精度
int g_examine_level = 1;       //检查等级 : CVZ_ERROR
bool g_auto_exit = true;       //是否自动退出
bool g_error_exit = false;     //是否因错误退出
std::mutex g_mu;               //互斥锁
HWND g_hwnd;
HANDLE g_handle;
int g_pvzbase;      //基址
int wave = 1;       //记录波数
int g_mainobject;   //游戏对象地址
int g_mouse_offset; //鼠标偏移地址

} // namespace pvz