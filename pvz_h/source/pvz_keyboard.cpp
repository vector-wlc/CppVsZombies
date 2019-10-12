

#include "libpvz.h"
#include "pvz_preparation.h"
#include "pvz_error.h"

namespace pvz
{

//将对象创建在 cvz_preparation 之后， 保护一些对象的内存
std::vector<std::pair<char, std::function<void()>>> key_operations;
FillIce ice_filler;
FixNut nut_fixer;
NvPuMiJi nv_pu_mi_ji;
PlaceDianCai dian_cai_placer;
PaoOperator pao_cvz(false);
CollectItem item_collector;

//此对象会一直在游戏退出战斗界面时才会析构
CvZPreparation cvz_preparation;

void KeyConnect(char key, const std::function<void()> &operate)
{
    if (key >= 'a' && key <= 'z')
        key -= 32;

    for (const auto &key_operation : key_operations)
        if (key_operation.first == key)
            PrintError("按键 %c 绑定了多个操作", key);

    key_operations.push_back(std::pair<char, std::function<void()>>(key, operate));

    static bool running = false;
    if (!running)
    {
        running = true;
        RunningInThread([&]() {
            while (true)
            {
                Sleep(10);
                for (const auto &key_operation : key_operations)
                    if (GetAsyncKeyState(key_operation.first) & 0x8001 == 0x8001)
                        key_operation.second();
            }
        });
    }
}
} // namespace pvz