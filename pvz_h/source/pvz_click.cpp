
#include "libpvz.h"
#include "pvz_global.h"

namespace pvz
{
void LeftClick(int x, int y)
{
    x /= g_DPI;
    y /= g_DPI;
    int position = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
    PostMessage(g_hwnd, WM_LBUTTONDOWN, 0, position);
    PostMessage(g_hwnd, WM_LBUTTONUP, 0, position);
}

//模拟鼠标右键点击操作
//主要用于安全点击
void RightClick(int x, int y)
{
    x /= g_DPI;
    y /= g_DPI;
    int position = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
    PostMessage(g_hwnd, WM_RBUTTONDOWN, 0, position);
    PostMessage(g_hwnd, WM_RBUTTONUP, 0, position);
}

//模拟鼠标选卡点击操作
//延迟200ms后鼠标抬起
void SeedClick(int x, int y)
{
    x /= g_DPI;
    y /= g_DPI;
    int position = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
    PostMessage(g_hwnd, WM_LBUTTONDOWN, 0, position);
    Sleep(200);
    PostMessage(g_hwnd, WM_LBUTTONUP, 0, position);
}

//模拟敲击空格键
void PressSpace()
{
    PostMessage(g_hwnd, WM_KEYDOWN, VK_SPACE, 0);
    PostMessage(g_hwnd, WM_KEYUP, VK_SPACE, 0);
}

//点击函数
//此函数自动识别场景，只用于战斗界面
//0: 白天, 1 : 黑夜, 2 : 泳池, 3 : 浓雾, 4 : 屋顶, 5 : 月夜, 6 : 蘑菇园, 7 : 禅境花园, 8 : 水族馆, 9 : 智慧树.
//使用示例：
//Click(3, 4)------点击场地上3行4列
//Click(3, 4, 10)-----点击场地上3行4列再向下偏移10px
void SceneClick(float row, float col, int offset)
{
    int x, y;
    y = 80 * col;
    //如果为泳池和迷雾
    if ((g_scene == 2) || (g_scene == 3))
        x = 55 + 85 * row + offset;
    //如果为天台
    else if ((g_scene == 4) || (g_scene == 5))
        if (col > 5)
            x = 45 + 85 * row + offset;
        else
            x = 45 + 85 * row + (120 - 20 * col) + offset;
    //其他场景
    else
        x = 40 + 100 * row + offset;
    x /= g_DPI;
    y /= g_DPI;
    int position = ((x & 0xFFFF) << 16) | (y & 0xFFFF);
    PostMessage(g_hwnd, WM_LBUTTONDOWN, 0, position);
    PostMessage(g_hwnd, WM_LBUTTONUP, 0, position);
}

//铲除植物，单株
void Shovel(int row, float col, bool pumpkin)
{
    g_mu.lock();
    RightClick(1, 1);
    LeftClick(70, 50 * 13);
    if (!pumpkin)
        SceneClick(row, col);
    else
        SceneClick(row, col, 10);
    RightClick(1, 1);

    if (g_examine_level == CVZ_INFO)
        if (pumpkin)
            std::printf("	Shovel pumpkin (%d, %3.2f)\n\n", row, col);
        else
            std::printf("	Shovel plant (%d, %3.2f)\n\n", row, col);

    g_mu.unlock();
}

//铲除植物，多株
void Shovel(const std::vector<SHOVEL> &lst)
{
    for (const auto &e : lst)
        Shovel(e.row, e.col, e.pumpkin);
}
} // namespace pvz