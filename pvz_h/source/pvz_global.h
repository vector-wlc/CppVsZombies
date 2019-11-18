
// pvz_global.h 一些全局变量的设定

#ifndef PVZ_GLOBAL_H
#define PVZ_GLOBAL_H

#include <mutex>
#include <windows.h>

namespace pvz
{
extern int g_scene;           //记录游戏场景
extern float g_DPI;           //记录DPI
extern bool g_auto_exit;      //是否自动退出
extern bool g_error_exit;     //因错误退出
extern std::mutex g_mu;       //互斥锁
extern int wave;
extern HWND g_hwnd;
extern int g_examine_level; //检查等级 : 默认CVZ_ERROR
} // namespace pvz

#endif