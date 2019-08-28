
// 12炮 实战，此程序源代码与 B站 视频一致

#include "pvz.h"

// PP   PP   PP
// 601

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

    return 0;
}
