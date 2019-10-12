
#include "libpvz.h"
#include "pvz_global.h"
#include "pvz_error.h"

namespace pvz
{

static int zombie_refresh_time = 0;

//预判时间
void Prejudge(int t, int w)
{
    if (g_examine_level == CVZ_INFO)
    {
        g_mu.lock();
        std::printf("  ################## wave:%d ##################\n\n", wave);
        g_mu.unlock();
    }
    wave = w;
    int base_time;
    if (wave == 1)
        base_time = 550;
    else if (wave == 10 || wave == 20)
        base_time = 750;
    else
        base_time = 200;
    int now_wave = NowWave();
    //预判的目标波已刷出(这种情况在wave==10||wave==20时基本不可能出现，所以不考虑）
    if (now_wave == wave)
        zombie_refresh_time = Countdown() + GameClock() - InitialCountdown();
    //预判的目标波未刷出
    else if (now_wave < wave)
    {
        //等待下一波
        while (NowWave() < (wave - 1))
            if (!g_high_precision)
                Sleep(1);

        //如果是大波
        if (wave == 10 || wave == 20)
        {
            while (Countdown() > 4)
                if (!g_high_precision)
                    Sleep(1);

            while (HugeWaveCountdown() > base_time)
                if (!g_high_precision)
                    Sleep(1);
            zombie_refresh_time = HugeWaveCountdown() + GameClock();
        }
        else
        {
            while (Countdown() > base_time)
                if (!g_high_precision)
                    Sleep(1);
            zombie_refresh_time = Countdown() + GameClock();
        }
    }

    else
        PrintError("wave #，Prejudge无法预判出僵尸刷新时间戳，请调整其他时间函数的使用", wave);
    //如果时间已超过预定时间戳
    if ((GameClock() - zombie_refresh_time) > t)
        PrintError("wave #，预判的时间为#，现在的时间已到#，请检查其他函数的使用", wave, t, GameClock() - zombie_refresh_time);

    if (g_examine_level == CVZ_INFO)
    {
        g_mu.lock();
        std::printf("	Prejudge time :%dcs...\n\n", t);
        g_mu.unlock();
    }
    //等待时间到达预定时间戳
    while ((GameClock() - zombie_refresh_time) < t)
        if (!g_high_precision)
            Sleep(1);
}

//延迟一定时间
void Delay(int time)
{
    if (time >= 0)
    {
        //获取当前时钟
        int in_time = GameClock();

        if (g_examine_level == CVZ_INFO)
        {
            g_mu.lock();
            std::printf("	Delay time :%dcs...\n\n", time);
            g_mu.unlock();
        }

        while (GameClock() - in_time < time)
            if (!g_high_precision)
                Sleep(1);
    }
    else
        PrintError("wave #，Delay函数参数不能为负数,当前的参数为#", wave, time);
}

//等待时间到
void Until(int time)
{

    if ((GameClock() - zombie_refresh_time) > time)
        PrintError("wave #，Until目标时间戳为#，现在的时间已到#，请检查其他函数的使用",
                   wave, time, GameClock() - zombie_refresh_time);
    if (g_examine_level == CVZ_INFO)
    {
        g_mu.lock();
        std::printf("	Until time arrive :%dcs...\n\n", time);
        g_mu.unlock();
    }
    while ((GameClock() - zombie_refresh_time) < time)
        if (!g_high_precision)
            Sleep(1);
}
} // namespace pvz