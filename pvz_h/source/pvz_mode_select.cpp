
// 模式选择

#include "libpvz.h"
#include "pvz_global.h"

namespace pvz
{

void OpenExamine(int level)
{
    g_examine_level = level;
}

void CancelAutoExit()
{
    g_auto_exit = false;
}

} // namespace pvz