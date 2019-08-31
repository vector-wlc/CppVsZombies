#include <pvz.h>

int main()
{
    UpdatePaoList({});
    PaoOperator pao_list;
    pao_list.resetPaoList({{2, 1}, {2, 3}, {2, 5}, {2, 7}, {3, 1}, {3, 3}, {3, 5}, {3, 7}, {4, 1}, {4, 3}, {4, 5}, {4, 7}});
    pao_list.setResolveConflictType(PaoOperator::DROP_POINT);
    pao_list.setLimitPaoSequence(true);
    StartAutoFillIceThread({{1, 5}, {1, 6}, {5, 5}, {5, 6}});

    for (wave = 1; wave < 21; wave++)
    {
        //首代
        if (wave_in(1, 10))
        {
            Prejudge(-60, wave);
            pao_list.pao({{2, 9}, {4, 9}});
            Until(-60 + 90);
            pao_list.pao({{2, 9}, {4, 9}});
            Until(-60 + 90 + 108);
            pao_list.pao({{1, 8.7}, {4, 9}});
            Until(314);
            Coffee();
        }
        else if (wave_in(2, 11, 4, 13, 6, 15, 8, 17)) //上
        {
            Prejudge(29, wave);
            pao_list.pao(3, 9);
            Until(397);
            pao_list.pao(3, 2.5);
            Until(627);
            pao_list.pao(1, 2.4);
            Until(700);
            Card({{"xpg", 2, 9}, {"ygg", 3, 9}, {"dxg", 4, 9}});
            Until(1013);
            Shovel({{2, 9}, {3, 9}, {4, 9}});
            Until(1057);
            pao_list.pao({{1, 8.5}, {4, 8.5}});
            Until(1343);
            Coffee();
        }
        else if (wave_in(3, 12, 5, 14, 7, 16, 18))
        {
            Prejudge(29, wave);
            pao_list.pao(3, 9);
            Until(397);
            pao_list.pao(3, 2.5);
            Until(627);
            pao_list.pao(1, 2.4);
            Until(700);
            Card({{"xpg", 2, 9}, {"ygg", 3, 9}, {"dxg", 4, 9}});
            Until(1013);
            Shovel({{2, 9}, {3, 9}, {4, 9}});
            Until(1057);
            pao_list.pao({{1, 8.5}, {4, 8.5}});
            Until(1343);
            Coffee();
        }
        else if (wave_in(9, 19))
        {
            Prejudge(29, wave);
            pao_list.pao(3, 9);
            Until(472);
            Card({{"hmg", 1, 4}, {"kfd", 1, 4}});
            Until(700);
            Card({{"xpg", 2, 9}, {"ygg", 3, 9}, {"dxg", 4, 9}});
            Until(1013);
            Shovel({{2, 9}, {3, 9}, {4, 9}});
            Until(1057);
            pao_list.pao(3, 9);
            Until(1257);
            pao_list.pao(3, 9);
            Until(1475);
            pao_list.pao({{2, 8.35}, {4, 8.35}});
            Until(1666);
            pao_list.pao({{2, 8.35}, {4, 8.35}});
            pao_list.skipPao(4);
        }
        else if (wave == 20)
        {
            Prejudge(-33, wave);
            pao_list.pao({{2, 9}, {4, 9}});
            Until(-33 + 100);
            pao_list.pao({{2, 9}, {4, 9}});
            Until(-33 + 100 + 100);
            pao_list.pao({{1, 9}, {4, 9}, {1, 9}, {4, 9}});
        }
    }
}