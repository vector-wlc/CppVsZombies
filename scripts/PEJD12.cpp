/*
 * @coding: utf-8
 * @Author: vector-wlc
 * @Date: 2019-08-30 19:47:11
 * @Description: PE 经典十二炮
 */

#include "pvz.h" // 包含 CvZ 库

int main()
{
    for (wave = 1; wave < 21; wave++)
    {
        if (10 == wave)
        {
            Prejudge(-55, wave);
            Pao({{2, 9}, {5, 9}});
            Until(-55 + 373 - 100);
            Card("ytzd", 2, 9);
        }
        else if (wave == 20)
        {
            Prejudge(-135, wave);
            Pao(4, 7);
            Until(-55);
            Pao({{2, 9}, {5, 9}});
            Delay(500);
            RecoverPao({{2, 9}, {5, 9}, {2, 9}, {5, 9}});
        }
        else
        {
            Prejudge(-95, wave);
            Pao({{2, 9}, {5, 9}});
            if (wave_in(9, 19))
            {
                // UpdatePaoList(....)
                //手动
                //SkipPao(4);

                //自动
                RecoverPao({{2, 9}, {5, 9}, {2, 9}, {5, 9}});
            }
        }
    }

    pvz::WaitGameEnd();

    return 0;
}
