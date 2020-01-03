/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \pvz_h\source\pvz_keyboard.cpp
 * @Date: 2019-12-22 14:49:42
 * @LastEditTime : 2019-12-25 20:10:58
 * @Description: 键盘操作函数实现
 */
#include <functional>
#include "pvz_error.h"
#include "pvz_global.h"
#include "libpvz.h"

namespace pvz
{
void KeyConnect(char key, const std::function<void()> &operate)
{
    if (key >= 'a' && key <= 'z')
        key -= 32;

    for (const auto &key_operation : g_key_operations)
        if (key_operation.first == key)
            PrintError("按键 %c 绑定了多个操作", key);

    g_key_operations.push_back(std::pair<char, std::function<void()>>(key, operate));

    static bool running = false;
    if (!running)
    {
        running = true;
        RunningInThread([&]() {
            while (true)
            {
                Sleep(10);
                for (const auto &key_operation : g_key_operations)
                    if (GetAsyncKeyState(key_operation.first) & 0x8001 == 0x8001)
                        key_operation.second();
            }
        });
    }
}
} // namespace pvz