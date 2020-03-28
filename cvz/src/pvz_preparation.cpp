/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \pvz_h\source\pvz_preparation.cpp
 * @Date: 2019-10-10 23:47:03
 * @LastEditTime: 2020-03-26 08:54:25
 * @Description: 程序进入 main 和退出 main 的一些准备
 */

#include "libpvz.h"
#include "pvz_global.h"
#include "pvz_error.h"
#include "pvz_preparation.h"

namespace pvz
{
CvZPreparation::CvZPreparation()
{
    printf("To get the CvZ latest update message, please visit : https://vector-wlc.github.io/CppVsZombies/\n\n");
    //获得游戏窗口句柄及进程
    find_pvz_window();
    //将游戏窗口设为前景窗口
    SetForegroundWindow(g_hwnd);
    //获取游戏窗口缩放比例
    get_pvz_window_dpi();
    //做一些进入游戏前的准备
    some_preparations();
}

CvZPreparation::~CvZPreparation()
{
    //恢复游戏原来的内存信息
    pao_cvz.setResolveConflictType(PaoOperator::COLLECTION);
    StopMaidCheats();

    if (g_handle != nullptr)
    {
        CloseHandle(g_handle);
    }
}

//获得游戏窗口及进程
void CvZPreparation::find_pvz_window()
{
    //等待游戏运行
    for (int i = 0; (g_hwnd = FindWindowW(L"MainWindow", L"Plants vs. Zombies")) == 0; i++)
    {
        if (0 == i)
            std::printf("\aOpen Plants vs. Zombies, please...\n\n");
        Sleep(500);
    }
    DWORD pid;
    GetWindowThreadProcessId(g_hwnd, &pid);
    g_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    //读取游戏基址
    g_pvzbase = ReadMemory<int>(0x006a9ec0);
}

//获取窗口缩放比例
void CvZPreparation::get_pvz_window_dpi()
{
    RECT pvz_rect;
    GetClientRect(g_hwnd, &pvz_rect);
    g_DPI = (1.0 * 800) / (pvz_rect.right - pvz_rect.left);
}

void CvZPreparation::some_preparations()
{
    bool is_print = true;
    //等待游戏进入选卡或战斗界面
    while (GameUi() != 2 && GameUi() != 3)
    {
        if (is_print)
        {
            std::printf("Waiting to enter the card selection or combat interface...\n\n");
            is_print = false;
        }
        Sleep(10);
    }

    is_print = true;
    //等待点击继续游戏
    while (ReadMemory<bool>(g_pvzbase + 0x320, 0x94, 0x54))
    {
        if (is_print)
        {
            std::printf("Waiting for the click to continue the game...\n\n");
            is_print = false;
        }
        Sleep(10);
    }

    g_mainobject = ReadMemory<uintptr_t>(g_pvzbase + 0x768);
    g_scene = ReadMemory<uintptr_t>(g_mainobject + 0x554C);
    item_collector.start();
    pao_cvz.resetPaoList();

    //恢复游戏原来的内存信息
    pao_cvz.setResolveConflictType(PaoOperator::COLLECTION);
    StopMaidCheats();

    std::printf("\t##############################\n\n\t\tGame Start!\n\n\t##############################\n\n");
}
} // namespace pvz