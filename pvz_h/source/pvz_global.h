/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \pvz_h\source\pvz_global.h
 * @Date: 2019-10-10 23:49:51
 * @LastEditTime : 2020-01-02 22:11:01
 * @Description: 包含 CvZ 用到的所有全局变量               
 */
#ifndef PVZ_GLOBAL_H
#define PVZ_GLOBAL_H

#include <mutex>
#include <windows.h>
#include <vector>
#include <map>

namespace pvz
{

extern int wave; //记录波数

//  all file
extern int g_scene;         //游戏场景
extern float g_DPI;         //DPI
extern int g_examine_level; //检查等级 : CVZ_ERROR
extern std::mutex g_mu;     //互斥锁
extern HWND g_hwnd;

//  memory
extern HANDLE g_handle;
extern int g_pvzbase;      //基址
extern int g_mainobject;   //游戏对象地址
extern int g_mouse_offset; //鼠标偏移地址

//  time
extern int g_zombie_refresh_time;

//  card
extern std::map<std::string, int> g_seed_name_to_index;
extern bool g_is_get_seed_index;
extern std::string g_seed_name[11][8];

//  keyboard
extern std::vector<std::pair<char, std::function<void()>>> g_key_operations;
} // namespace pvz

#endif