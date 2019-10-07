
// 模式选择

#include "libpvz.h"
#include "pvz_global.h"

namespace pvz
{

void OpenExamine(int level)
{
    g_examine_level = level;
}

//开关函数：开启高精度
//使用此函数会导致较高的 CPU 占用
void OpenHighPrecision()
{
    g_high_precision = true;
}

void CancelAutoExit()
{
    g_auto_exit = false;
}

} // namespace pvz