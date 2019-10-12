
// cvz_error.h

#ifndef CVZ_ERROR_H
#define CVZ_ERROR_H

#include <windows.h>
#include <sstream>
#include "pvz_global.h"

namespace pvz
{

#ifndef GBK

//UTF-8 转 GBK
void UTF8ToGBK(std::string &strUTF8);

#endif

inline void SetColor(unsigned short forecolor = 40, unsigned short backgroudcolor = 30)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);             //获取缓冲区句柄
    SetConsoleTextAttribute(hCon, forecolor | backgroudcolor); //设置文本及背景色
}

//弹出异常，提示错误信息
template <typename... Args>
void PrintError(const std::string &content, Args... args)
{

    std::string error_content = content;

#ifndef GBK

    UTF8ToGBK(error_content);

#endif

    SetColor();
    g_mu.lock();
    std::printf(error_content.c_str(), args...);
    std::printf("\n");
    g_mu.unlock();

    //暂停游戏
    PostMessage(g_hwnd, WM_KEYDOWN, VK_SPACE, 0);
    PostMessage(g_hwnd, WM_KEYUP, VK_SPACE, 0);

    //pvz窗口置底
    RECT pvz_rect;
    GetWindowRect(g_hwnd, &pvz_rect);
    SetWindowPos(g_hwnd, HWND_BOTTOM, pvz_rect.left, pvz_rect.top, pvz_rect.right - pvz_rect.left, pvz_rect.bottom - pvz_rect.top, SWP_SHOWWINDOW);

    std::system("pause");
    g_error_exit = true;
    std::exit(0);
}
} // namespace pvz

#endif