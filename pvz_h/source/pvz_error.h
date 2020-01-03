/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \pvz_h\source\pvz_error.h
 * @Date: 2019-10-10 23:03:40
 * @LastEditTime : 2020-01-02 22:07:35
 * @Description: error 
 */

#ifndef CVZ_ERROR_H
#define CVZ_ERROR_H

#include <windows.h>
#include <string>
#include "pvz_global.h"

namespace pvz
{
#ifndef GBK
//UTF-8 转 GBK
void UTF8ToGBK(std::string &strUTF8);

#endif

inline void SetColor(unsigned short color)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, color);
}

//弹出异常，提示错误信息
template <typename... Args>
void PrintError(const std::string &content, Args... args)
{
    if (g_examine_level == 3)
        return;
        
    std::string error_content = "wave %d : " + content + "\n\n";

#ifndef GBK

    UTF8ToGBK(error_content);

#endif
    //暂停游戏
    PostMessage(g_hwnd, WM_KEYDOWN, VK_SPACE, 0);
    PostMessage(g_hwnd, WM_KEYUP, VK_SPACE, 0);

    //pvz窗口置底
    RECT pvz_rect;
    GetWindowRect(g_hwnd, &pvz_rect);
    SetWindowPos(g_hwnd, HWND_BOTTOM, pvz_rect.left, pvz_rect.top, pvz_rect.right - pvz_rect.left, pvz_rect.bottom - pvz_rect.top, SWP_SHOWWINDOW);

    SetColor(30 | 40);
    g_mu.lock();
    std::printf(error_content.c_str(), wave, args...);
    g_mu.unlock();
    SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    std::system("pause");
}
} // namespace pvz

#endif