
// cvz_error.h

#ifndef CVZ_ERROR_H
#define CVZ_ERROR_H

#include <windows.h>
#include <sstream>
#include "pvz_global.h"

namespace pvz
{
//完成内容转换
template <typename T>
int TransContent(std::string &content, T trans)
{
    auto replace = content.find_first_of("#");
    //如果字符串中无"#"则退出此函数
    if (replace == std::string::npos)
        return 0;

    //进行数字到字符串的转化
    std::stringstream s;
    s << trans;
    //将"#"替换成相应的内容
    content.replace(replace, 1, s.str());
    return 1;
}

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
void PrintError(const std::string &title, const std::string &content, Args... args)
{

    std::string error_content = content;
    //字符串内容更换，利用逗号表达式将参数包展开
    std::initializer_list<int>{(TransContent(error_content, args), 0)...};
    std::string error_title = title;

#ifndef GBK

    UTF8ToGBK(error_title);
    UTF8ToGBK(error_content);

#endif

    g_mu.lock();
    SetColor();
    std::printf("%s : %s\n\n", error_title.c_str(), error_content.c_str());
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