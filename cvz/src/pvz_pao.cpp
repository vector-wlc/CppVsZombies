/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \pvz_h\source\pvz_pao.cpp
 * @Date: 2019-10-10 23:44:07
 * @LastEditTime: 2020-03-07 22:00:51
 * @Description: 炮操作类的实现
 */

#include "libpvz.h"
#include "pvz_error.h"
#include "pvz_global.h"

namespace pvz
{
std::map<GRID, PaoOperator::PAO_INFO> PaoOperator::all_pao_list;
const PaoOperator::FLY_TIME PaoOperator::fly_time_data[8] = {
    {515, 359}, {499, 362}, {515, 364}, {499, 367}, {515, 369}, {499, 372}, {511, 373}, {511, 373}};
int PaoOperator::conflict_resolution_type = PaoOperator::COLLECTION;

//得到炮的所有信息
void PaoOperator::get_all_pao_message()
{
    PlantMemory cannon;
    std::pair<GRID, PAO_INFO> pao_message;
    int plant_count_max = cannon.countMax();
    int cannon_status;
    int now_time = GameClock();

    //遍历场上所有炮的信息
    for (int i = 0; i < plant_count_max; ++i)
    {
        cannon.setIndex(i);
        if (cannon.type() == 47 && !cannon.isCrushed() && !cannon.isDisappeared())
        {
            pao_message.first.row = cannon.row() + 1;
            pao_message.first.col = cannon.col() + 1;
            pao_message.second.index = i;
            cannon_status = cannon.state();

            //计算炮恢复时间
            if (cannon_status == 37)
                pao_message.second.recover_time = now_time;
            else if (cannon_status == 38) //如果正在发射则认为该炮的
                pao_message.second.recover_time = now_time + 3475;
            else //如果炮不能用，则恢复时间为现在时间 + 倒计时 + 125
                pao_message.second.recover_time = now_time + cannon.stateCountdown() + 125;

            all_pao_list.insert(pao_message);
        }
    }
}

void PaoOperator::getAllPaoStatusList(std::vector<PAO_STATUS> &lst_out_)
{
    lst_out_.clear();
    int now_time = GameClock();
    int pao_cd;
    PAO_STATUS pao_status;
    for (const auto &pao_msg : all_pao_list)
    {
        pao_status.row = pao_msg.first.row;
        pao_status.col = pao_msg.first.col;
        pao_status.index = pao_msg.second.index;
        pao_cd = pao_msg.second.recover_time - now_time;
        pao_status.cd = pao_cd > 0 ? pao_cd : 0;
        pao_status.is_shoveled = pao_msg.second.is_shoveled;
        lst_out_.push_back(pao_status);
    }
}

void PaoOperator::getAllRecoveredPaoList(std::vector<GRID> &lst)
{
    lst.clear();
    int now_time = GameClock();
    for (const auto &pao_msg : all_pao_list)
    {
        if (pao_msg.second.recover_time < now_time && !pao_msg.second.is_shoveled)
        {
            lst.push_back(pao_msg.first);
        }
    }
}

PaoOperator::PaoOperator()
{
    limit_pao_sequence = true;
    next_pao = 0;
}

PaoOperator::PaoOperator(const std::vector<GRID> &lst)
{
    resetPaoList(lst);
    limit_pao_sequence = true;
    next_pao = 0;
}

PaoOperator::~PaoOperator()
{
    GRID pao_grid;
    //清除所有有关于本对象的炮列表
    for (const auto &e : pao_list)
    {
        e->second.is_in_list = false;
        e->second.is_in_sequence = false;
    }
}

void PaoOperator::setResolveConflictType(int type)
{
    conflict_resolution_type = type;

    if (type == DROP_POINT)
        WriteMemory<short>(0, 0x4663F9);
    else
        WriteMemory<short>(30, 0x4663F9);
}

void PaoOperator::setLimitPaoSequence(bool limit)
{
    limit_pao_sequence = limit;
    for (auto it : pao_list)
        it->second.is_in_sequence = limit;
}

void PaoOperator::add_pao_message(int row, int col)
{
    PlantMemory cannon;
    int cannon_status = 0;
    std::pair<GRID, PAO_INFO> new_pao;
    new_pao.second.index = GetPlantIndex(row, col, 47);

    int now_time = GameClock();
    cannon.setIndex(new_pao.second.index);
    cannon_status = cannon.state();
    //计算炮恢复时间
    if (cannon_status == 37)
    {
        new_pao.second.recover_time = now_time;
    }
    else if (cannon_status == 38) //如果正在发射则认为时该炮的全部CD
    {
        new_pao.second.recover_time = now_time + 3475;
    }
    else //如果炮不能用，则恢复时间为现在时间 + 倒计时 + 125
    {
        new_pao.second.recover_time = now_time + cannon.stateCountdown() + 125;
    }
    new_pao.first.row = row;
    new_pao.first.col = col;

    // 替换掉被铲除的炮
    for (auto it = all_pao_list.begin(); it != all_pao_list.end(); ++it)
    {
        if (it->second.is_shoveled)
        {
            new_pao.second.is_in_list = it->second.is_in_list;
            new_pao.second.is_in_sequence = it->second.is_in_sequence;
            all_pao_list.erase(it);
            break;
        }
    }

    all_pao_list.insert(new_pao);
}

void PaoOperator::plant_pao(int row, int col)
{
    int ymjnp = GetSeedIndex(47) + 1;
    int ymts = GetSeedIndex(34) + 1;
    SeedMemory cannon_seed(ymjnp - 1);
    SeedMemory pitcher_seed(ymts - 1);

    int now_time = GameClock();
    int plant_index;
    //种植玉米投手
    for (int i = col; i < col + 2; ++i)
    {
        plant_index = GetPlantIndex(row, i, 34);

        // 等待其他植物消失
        while (plant_index == -2)
        {
            Sleep(1);
            plant_index = GetPlantIndex(row, i, 34);
        }

        //如果有需要种植玉米炮的地方
        if (plant_index == -1)
        {
            //种植玉米投手
            while (!pitcher_seed.isUsable())
                Sleep(1);
            Card(ymts, row, i);

            while (pitcher_seed.isUsable())
                Sleep(1);
        }
    }

    //种植玉米加农炮
    while (!cannon_seed.isUsable())
        Sleep(1);
    Card(ymjnp, row, col);

    Sleep(20);
    add_pao_message(row, col);
}

void PaoOperator::changePaoMessage(int origin_row, int origin_col, int now_row, int now_col)
{
    // 参数为 0 不进行删除操作
    if (origin_row != 0 && origin_col != 0)
    {
        auto change_pao = all_pao_list.find(GRID(origin_row, origin_col));

        if (change_pao != all_pao_list.end())
        {
            change_pao->second.is_shoveled = true;
        }
        else
        {
            PrintError("请检查原来位置 (%d, %d) 是否为炮", origin_row, origin_col);
        }
    }

    //参数为 0 不进行增添操作
    if (now_row != 0 && now_col != 0)
    {
        RunningInThread([=]() {
            Sleep(20);
            add_pao_message(now_row, now_col);
        });
    }
}

//自动识别炮列表
void PaoOperator::resetPaoList()
{
    static bool is_get_all_pao_list_message = false;
    if (!is_get_all_pao_list_message)
    {
        get_all_pao_message();
        is_get_all_pao_list_message = true;
    }

    //清除所有有关于本对象的炮列表
    for (const auto &e : pao_list)
    {
        e->second.is_in_list = false;
        e->second.is_in_sequence = false;
    }

    //重置炮列表
    pao_list.clear();

    //寻找不在炮列表中的炮
    for (auto it = all_pao_list.begin(); it != all_pao_list.end(); ++it)
    {
        if (!(it->second.is_in_list) || !(it->second.is_shoveled))
        {
            it->second.is_in_list = true;
            it->second.is_in_sequence = limit_pao_sequence;
            pao_list.push_back(it);
        }
    }

    if (g_examine_level == CVZ_INFO)
    {
        std::printf("resetPaoList:\n\n");
        int i = 1;
        for (const auto &pao_grid : pao_list)
        {
            std::printf(" [%d, %d]", pao_grid->first.row, pao_grid->first.col);
            if (i % 4 == 0)
                std::printf("\n");
            ++i;
        }
        std::printf("\n\n");
    }
}

//用户重置炮列表
void PaoOperator::resetPaoList(const std::vector<GRID> &lst)
{
    //清除所有有关于本对象的炮列表
    for (const auto &pao_it : pao_list)
    {
        pao_it->second.is_in_list = false;
        pao_it->second.is_in_sequence = false;
    }

    //重置炮列表
    pao_list.clear();
    PaoIterator it;
    for (const auto &pao_grid : lst)
    {
        //在总炮信息里面找不到则报错
        it = all_pao_list.find(pao_grid);
        if (it == all_pao_list.end() || it->second.is_shoveled)
        {
            PrintError("请检查 (%d, %d) 是否为炮", pao_grid.row, pao_grid.col);
        }
        //如果此炮在其它炮列表中
        if (it->second.is_in_list)
        {
            PrintError("(%d, %d) 在其它炮列表中", pao_grid.row, pao_grid.col);
        }
        it->second.is_in_list = true;
        it->second.is_in_sequence = limit_pao_sequence;

        pao_list.push_back(it);
    }
}

//得到本列表中能用的炮
void PaoOperator::getRecoveredPaoList(std::vector<GRID> &lst_out_)
{
    lst_out_.clear();
    int now_time = GameClock();
    for (const auto &pao_it : pao_list)
    {
        if (pao_it->second.recover_time < now_time && !pao_it->second.is_shoveled)
        {
            lst_out_.push_back(pao_it->first);
        }
    }
}

//得到本炮列表中炮的状态
void PaoOperator::getPaoStatusList(std::vector<PAO_STATUS> &lst_out_)
{
    lst_out_.clear();
    int now_time = GameClock();
    int pao_cd;
    PAO_STATUS pao_status;
    for (const auto &pao_it : pao_list)
    {
        pao_status.row = pao_it->first.row;
        pao_status.col = pao_it->first.col;
        pao_status.index = pao_it->second.index;
        pao_cd = pao_it->second.recover_time - now_time;
        pao_status.cd = pao_cd > 0 ? pao_cd : 0;
        pao_status.is_shoveled = pao_it->second.is_shoveled;
        lst_out_.push_back(pao_status);
    }
}

void PaoOperator::setNextPao(int temp_next_pao)
{
    if (temp_next_pao > pao_list.size())
    {
        PrintError("本炮列表中一共有 %d 门炮，您设的参数已溢出", pao_list.size());
    }
    next_pao = temp_next_pao - 1;
}

void PaoOperator::setNextPao(int row, int col)
{
    int temp_next_pao = 0;
    for (const auto &pao_grid : pao_list)
    {
        if (pao_grid->first.row == row && pao_grid->first.col == col)
        {
            next_pao = temp_next_pao;
            return;
        }
        ++temp_next_pao;
    }

    PrintError("请检查(%d, %d)是否在本炮列表中", row, col);
}

//对炮落点进行一些检查
bool PaoOperator::is_drop_conflict(int pao_row, int pao_col, int drop_row, float drop_col)
{
    int pao_x, pao_y, drop_x, drop_y;
    pao_x = 80 * pao_col;
    drop_x = int(80 * drop_col);
    //如果为泳池和迷雾
    if ((g_scene == 2) || (g_scene == 3))
    {
        pao_y = 55 + 85 * pao_row;
        drop_y = 55 + 85 * drop_row;
    }
    //如果为天台
    else if ((g_scene == 4) || (g_scene == 5))
    {
        if (pao_col > 5)
        {
            pao_y = 45 + 85 * pao_row;
        }
        else
        {
            pao_y = 45 + 85 * pao_row + (120 - 20 * pao_col);
        }
        if (drop_col > 5)
        {
            drop_y = 45 + 85 * drop_row;
        }
        else
        {
            drop_y = 45 + 85 * drop_row + (120 - 20 * drop_col);
        }
    }
    //其他场景
    else
    {
        pao_y = 40 + 100 * pao_row;
        drop_y = 40 + 100 * drop_row;
    }
    return ((drop_x - pao_x) * (drop_x - pao_x) + (pao_y - drop_y) * (pao_y - drop_y)) < 10000;
}

//对炮进行一些检查
void PaoOperator::pao_examine(PaoIterator it, int now_time, int drop_row, float drop_col)
{
    if (conflict_resolution_type == COLLECTION &&
        is_drop_conflict(it->first.row, it->first.col, drop_row, drop_col))
    {
        PrintError("位于 (%d，%d) 的炮与落点(%d，%f)冲突", it->first.row, it->first.col, drop_row, drop_col);
    }
    if (it->second.recover_time > now_time)
    {
        PrintError("位于 (%d, %d) 的炮还有 %dcs 恢复", it->first.row, it->first.col, it->second.recover_time - now_time);
    }
}

void PaoOperator::base_fire_pao(PaoIterator it, int now_time, int drop_row, float drop_col)
{
    g_mu.lock();
    RightClick(1, 1);
    switch (conflict_resolution_type)
    {
    case DROP_POINT: //点击一次即可
        SceneClick(it->first.row, it->first.col);
        SceneClick(drop_row, drop_col);
        break;

    case COLLECTION: //点击炮身三次
        for (int i = 0; i < 3; ++i)
        {
            SceneClick(it->first.row, it->first.col);
        }
        SceneClick(drop_row, drop_col);
        break;
    default:
        break;
    }

    it->second.recover_time = now_time + 3475;

    if (g_examine_level == CVZ_INFO)
    {
        std::printf("	Fire cannon from (%d, %d) to (%d, %3.2f)\n\n", it->first.row, it->first.col, drop_row, drop_col);
    }
    g_mu.unlock();
}

//用户自定义炮位置发炮：单发
void PaoOperator::rawPao(int pao_row, int pao_col, int drop_row, float drop_col)
{
    int now_time = GameClock();

    auto it = all_pao_list.find(GRID(pao_row, pao_col));

    if (it == all_pao_list.end())
    {
        PrintError("请检查 (%d, %d) 是否为炮", pao_row, pao_col);
    }
    if (g_examine_level != CVZ_NO)
    {
        pao_examine(it, now_time, drop_row, drop_col);
    }
    base_fire_pao(it, now_time, drop_row, drop_col);
}

//用户自定义炮位置发炮：多发
void PaoOperator::rawPao(const std::vector<RAW_PAO> &lst)
{
    int now_time = GameClock();
    PaoIterator it;

    for (const auto &e : lst)
    {
        it = all_pao_list.find(GRID(e.pao_row, e.pao_col));

        if (it == all_pao_list.end())
        {
            PrintError("请检查 (%d, %d) 是否为炮", e.pao_row, e.pao_col);
        }
        if (g_examine_level != CVZ_NO)
        {
            pao_examine(it, now_time, e.drop_row, e.drop_col);
        }
        base_fire_pao(it, now_time, e.drop_row, e.drop_col);
    }
}

//发炮函数：单发
void PaoOperator::pao(int row, float col)
{
    int now_time = GameClock();
    if (!limit_pao_sequence)
    {
        PrintError("解除炮序限制，Pao系列函数不可使用！");
    }
    g_mu.lock();
    int temp_next_pao = next_pao;
    skipPao(1);
    g_mu.unlock();

    if (g_examine_level != CVZ_NO)
    {
        pao_examine(pao_list[temp_next_pao], now_time, row, col);
    }

    base_fire_pao(pao_list[temp_next_pao], now_time, row, col);
}

//发炮函数：多发
void PaoOperator::pao(const std::vector<PAO> &lst)
{
    int now_time = GameClock();
    if (!limit_pao_sequence)
        PrintError("解除炮序限制，Pao系列函数不可使用！");

    g_mu.lock();
    int temp_next_pao = next_pao;
    skipPao(lst.size());
    g_mu.unlock();

    for (const auto &drop_grid : lst)
    {
        if (g_examine_level != CVZ_NO)
        {
            pao_examine(pao_list[temp_next_pao], now_time, drop_grid.row, drop_grid.col);
        }
        base_fire_pao(pao_list[temp_next_pao], now_time, drop_grid.row, drop_grid.col);
        temp_next_pao = (temp_next_pao + 1) % pao_list.size();
    }
}

bool PaoOperator::tryPao(int row, float col)
{
    if (limit_pao_sequence)
        PrintError("由于炮序限制，tryPao系列函数不可使用！");

    int now_time = GameClock();

    //寻找符合条件的炮
    for (const auto &pao_it : pao_list)
    {
        if (pao_it->second.is_shoveled)
            continue;
        //如果炮可用
        if (pao_it->second.recover_time <= now_time)
        {
            if (conflict_resolution_type == DROP_POINT || //克服落点冲突模式
                (conflict_resolution_type == COLLECTION &&
                 !is_drop_conflict(pao_it->first.row, pao_it->first.col, row, col))) //克服收集物冲突模式
            {
                base_fire_pao(pao_it, now_time, row, col);
                return true;
            }
        }
    }

    return false;
}

bool PaoOperator::tryPao(const std::vector<PAO> &lst)
{
    if (limit_pao_sequence)
        PrintError("由于炮序限制，tryPao系列函数不可使用！");

    int now_time = GameClock();
    int success_num = 0;

    for (const auto &drop_grid : lst)
    {
        //寻找符合条件的炮
        for (const auto &pao_it : pao_list)
        {
            if (pao_it->second.is_shoveled)
                continue;
            //如果炮可用
            if (pao_it->second.recover_time <= now_time)
            {
                if (conflict_resolution_type == DROP_POINT ||                                                                                          //克服落点冲突模式
                    (conflict_resolution_type == COLLECTION && !is_drop_conflict(pao_it->first.row, pao_it->first.col, drop_grid.row, drop_grid.col))) //克服收集物冲突模式
                {
                    base_fire_pao(pao_it, now_time, drop_grid.row, drop_grid.col);
                    ++success_num;
                    break;
                }
            }
        }
    }

    return success_num == lst.size();
}

//等待炮恢复立即用炮：单发
void PaoOperator::recoverPao(int row, float col)
{
    if (!limit_pao_sequence)
    {
        PrintError("解除炮序限制，Pao系列函数不可使用！");
    }
    g_mu.lock();
    int temp_next_pao = next_pao;
    skipPao(1);
    g_mu.unlock();

    if (conflict_resolution_type == COLLECTION && is_drop_conflict(pao_list[temp_next_pao]->first.row, pao_list[temp_next_pao]->first.col, row, col))
    {
        PrintError("位于(%d，%d)的炮与落点(%d，%f)冲突", pao_list[temp_next_pao]->first.row, pao_list[temp_next_pao]->first.col, row, col);
    }
    while (pao_list[temp_next_pao]->second.recover_time - 1 > GameClock())
    {
        Sleep(1);
    }

    // 确保发炮
    PlantMemory cannon_memory(pao_list[temp_next_pao]->second.index);
    while (cannon_memory.state() != 37)
        ;
    base_fire_pao(pao_list[temp_next_pao], GameClock(), row, col);
}

//等待炮恢复立即用炮：多发
void PaoOperator::recoverPao(const std::vector<PAO> &lst)
{
    for (const auto &drop_grid : lst)
    {
        recoverPao(drop_grid.row, drop_grid.col);
    }
}

//获取屋顶炮飞行时间
int PaoOperator::get_roof_fly_time(int pao_col, float drop_col)
{
    //得到落点对应的横坐标
    int drop_x = static_cast<int>(drop_col * 80);
    //得到该列炮最小飞行时间对应的最小的横坐标
    int min_drop_x = fly_time_data[pao_col - 1].min_drop_x;
    //得到最小的飞行时间
    int min_fly_time = fly_time_data[pao_col - 1].min_fly_time;
    //返回飞行时间
    return (drop_x >= min_drop_x ? min_fly_time : (min_fly_time + 1 - (drop_x - (min_drop_x - 1)) / 32));
}

//用户自定义炮位置，屋顶修正时间发炮：单发
void PaoOperator::base_fire_roof_pao(PaoIterator it, int fire_time, int drop_row, float drop_col)
{
    //时间矫正
    while (GameClock() < fire_time - 2)
        Sleep(1);

    while (GameClock() < fire_time)
        ;

    base_fire_pao(it, fire_time, drop_row, drop_col);
}

//用户自定义炮位置，屋顶修正时间发炮：多发
void PaoOperator::base_fire_roof_paos(std::vector<RP> lst)
{
    //进行发射时间的排序
    std::sort(lst.begin(), lst.end(), [](const RP &r1, const RP &r2) {
        return r1.fire_time < r2.fire_time;
    });

    for (const auto &rp : lst)
    {
        //时间矫正
        while (GameClock() < rp.fire_time - 2)
            Sleep(1);
        while (GameClock() < rp.fire_time)
            ;

        base_fire_pao(rp.it, rp.fire_time, rp.drop_row, rp.drop_col);
    }
}

//防止线程冲突
void PaoOperator::roofPao(int row, float col)
{
    if (!limit_pao_sequence)
        PrintError("解除炮序限制，Pao系列函数不可使用！");
    if (g_scene != 4 && g_scene != 5)
        PrintError("RoofPao函数只适用于 RE 与 ME ");

    g_mu.lock();
    int temp_next_pao = next_pao;
    skipPao(1);
    g_mu.unlock();

    int fire_time = 387 - get_roof_fly_time(pao_list[temp_next_pao]->first.col, col) + GameClock();
    if (g_examine_level != CVZ_NO)
        pao_examine(pao_list[temp_next_pao], fire_time, row, col);
    pao_list[temp_next_pao]->second.recover_time = fire_time + 3475;
    RunningInThread(base_fire_roof_pao, pao_list[temp_next_pao], fire_time, row, col);
}

void PaoOperator::roofPao(const std::vector<PAO> &lst)
{
    if (!limit_pao_sequence)
        PrintError("解除炮序限制，Pao系列函数不可使用！");
    //检查函数运行环境
    if (g_scene != 4 && g_scene != 5)
        PrintError("RoofPao函数只适用于RE与ME");

    std::vector<RP> roof_pao_list(lst.size());
    int num = 0;
    int fire_time;
    int now_time = GameClock();

    g_mu.lock();
    int temp_next_pao = next_pao;
    skipPao(lst.size());
    g_mu.unlock();

    for (const auto &drop_grid : lst)
    {
        fire_time = 387 - get_roof_fly_time(pao_list[temp_next_pao]->first.col, drop_grid.col) + now_time;
        if (g_examine_level != CVZ_NO)
        {
            pao_examine(pao_list[temp_next_pao], fire_time, drop_grid.row, drop_grid.col);
        }
        pao_list[temp_next_pao]->second.recover_time = fire_time + 3475;
        roof_pao_list[num] = {pao_list[temp_next_pao], drop_grid.row, drop_grid.col, fire_time};
        ++num;
        temp_next_pao = (temp_next_pao + 1) % pao_list.size();
    }

    RunningInThread(base_fire_roof_paos, roof_pao_list);
}

bool PaoOperator::tryRoofPao(int row, float col)
{
    if (limit_pao_sequence)
        PrintError("由于炮序限制，tryPao系列函数不可使用！");
    //检查函数运行环境
    if (g_scene != 4 && g_scene != 5)
        PrintError("RoofPao函数只适用于RE与ME");

    int fire_time;
    int now_time = GameClock();
    //寻找符合条件的炮
    for (const auto &pao_it : pao_list)
    {
        if (pao_it->second.is_shoveled)
            continue;
        fire_time = 387 - get_roof_fly_time(pao_it->first.col, col) + now_time;
        //如果炮可用
        if (pao_it->second.recover_time <= fire_time)
        {
            if (conflict_resolution_type == DROP_POINT ||                                                                      //克服落点冲突模式
                (conflict_resolution_type == COLLECTION && !is_drop_conflict(pao_it->first.row, pao_it->first.col, row, col))) //克服收集物冲突模式
            {
                pao_it->second.recover_time = fire_time + 3475;
                RunningInThread(base_fire_roof_pao, pao_it, fire_time, row, col);

                return true;
            }
        }
    }

    return false;
}

bool PaoOperator::tryRoofPao(const std::vector<PAO> &lst)
{
    if (limit_pao_sequence)
    {
        PrintError("由于炮序限制，tryPao系列函数不可使用！");
    }

    if (g_scene != 4 && g_scene != 5)
    {
        PrintError("RoofPao函数只适用于RE与ME");
    }
    std::vector<RP> roof_pao_list(lst.size());
    int now_time = GameClock();
    int fire_time;
    int success_num = 0;

    for (const auto &drop_grid : lst)
    {
        //寻找符合条件的炮
        for (const auto &pao_it : pao_list)
        {
            if (pao_it->second.is_shoveled)
                continue;
            fire_time = 387 - get_roof_fly_time(pao_it->first.col, drop_grid.col) + now_time;
            //如果炮可用
            if (pao_it->second.recover_time <= fire_time)
            {
                if (conflict_resolution_type == DROP_POINT ||                                                                                          //克服落点冲突模式
                    (conflict_resolution_type == COLLECTION && !is_drop_conflict(pao_it->first.row, pao_it->first.col, drop_grid.row, drop_grid.col))) //克服收集物冲突模式
                {
                    pao_it->second.recover_time = fire_time + 3475;
                    roof_pao_list[success_num] = {pao_it, drop_grid.row, drop_grid.col, fire_time};
                    ++success_num;
                    break;
                }
            }
        }
    }

    //如果全部找到
    if (success_num == lst.size())
    {
        RunningInThread(base_fire_roof_paos, roof_pao_list);
        return true;
    }

    return false;
}

//屋顶修正时间发炮，单发
void PaoOperator::rawRoofPao(int pao_row, int pao_col, int drop_row, float drop_col)
{
    if (g_scene != 4 && g_scene != 5)
        PrintError("RawRoofPao函数只适用于RE与ME");

    auto it = all_pao_list.find(GRID(pao_row, pao_col));
    if (it == all_pao_list.end())
        PrintError("请检查 (%d, %d) 是否为炮");
    int fire_time = 387 - get_roof_fly_time(pao_col, drop_col) + GameClock();
    if (g_examine_level != CVZ_NO)
        pao_examine(it, fire_time, drop_row, drop_col);
    it->second.recover_time = fire_time + 3475;
    RunningInThread(base_fire_roof_pao, it, fire_time, drop_row, drop_col);
}

//屋顶修正时间发炮 多发
void PaoOperator::rawRoofPao(const std::vector<RAW_PAO> &lst)
{
    if (g_scene != 4 && g_scene != 5)
        PrintError("RawRoofPao函数只适用于RE与ME");

    std::vector<RP> roof_pao_list(lst.size());
    int num = 0;
    int fire_time;
    PaoIterator it;

    for (const auto &rp : lst)
    {
        if ((it = all_pao_list.find(GRID(rp.pao_row, rp.pao_col))) == all_pao_list.end())
        {
            PrintError("请检查 (%d, %d) 是否为炮");
        }
        fire_time = 387 - get_roof_fly_time(rp.pao_col, rp.drop_col) + GameClock();
        if (g_examine_level != CVZ_NO)
        {
            pao_examine(it, fire_time, rp.pao_row, rp.pao_col);
        }
        it->second.recover_time = fire_time + 3475;
        roof_pao_list[num] = {it, rp.drop_row, rp.drop_col, fire_time};
    }

    RunningInThread(base_fire_roof_paos, roof_pao_list);
}

//铲种炮
void PaoOperator::shovel_and_plant_pao(int row, int col, int move_col, PaoIterator it, int delay_time)
{
    PlantMemory shovel_pao(it->second.index);

    //等待炮使用
    while (shovel_pao.state() != 38)
        Sleep(1);

    int time = GameClock();
    while (GameClock() <= time + 206)
        Sleep(1);
    //铲炮
    Shovel(row, col);

    //在这里不能直接释放 it 的内存，虽然炮已经不存在了，
    it->second.is_shoveled = true;

    int now_time = GameClock();

    //等待一些时间
    while (GameClock() - now_time < delay_time)
        Sleep(1);
    if (delay_time == 0)
        Sleep(20);

    plant_pao(row, col + move_col);
}

void PaoOperator::shovelPao(int row, int col, bool is_forced)
{
    auto it = all_pao_list.find(GRID(row, col));

    //是否为炮？
    if (it == all_pao_list.end())
    {
        PrintError("请检查(%d, %d)是否为炮", row, col);
    }
    if (it->second.is_in_sequence && !is_forced)
    {
        PrintError("(%d, %d)被炮序限制，不允许对其进行铲除操作", row, col);
    }
    //在这里不能直接释放 it 的内存，虽然炮已经不存在了，
    it->second.is_shoveled = true;

    Shovel(row, col);
}

void PaoOperator::fixPao(int row, int col, int move_col, int delay_time, bool is_forced)
{
    if (move_col > 1 || move_col < -1)
        PrintError("铲种炮不允许位移量超过 1");

    auto it = all_pao_list.find(GRID(row, col));

    //是否为炮？
    if (it == all_pao_list.end())
        PrintError("请检查(%d, %d)是否为炮", row, col);

    if (it->second.is_in_sequence && !is_forced)
        PrintError("(%d, %d)被炮序限制，不允许对其进行铲种操作", row, col);

    RunningInThread(shovel_and_plant_pao, row, col, move_col, it, delay_time);
}

void PaoOperator::tryFixPao()
{
    if (limit_pao_sequence)
    {
        PrintError("由于炮序限制，tryPao系列函数不可使用！");
    }
    RunningInThread([&]() {
        pvz::PlantMemory cannon;
        //记录内存中炮的状态(炮的状态为可以发射)
        std::vector<PaoIterator> original_pao_message_list;

        //进行遍历并记录符合条件的炮
        for (const auto pao_message : pao_list)
        {
            cannon.setIndex(pao_message->second.index);
            if (cannon.state() == 37)
            {
                original_pao_message_list.push_back(pao_message);
            }
        }

        while (1)
        {
            Sleep(5);
            for (const auto &pao_message : original_pao_message_list)
            {
                cannon.setIndex(pao_message->second.index);
                if (cannon.state() == 38)
                {
                    shovel_and_plant_pao(pao_message->first.row, pao_message->first.col, 0, pao_message, 0);
                    return;
                }
            }
        }
    });
}

void PaoOperator::tryRecoverPao(int row, float col)
{
    if (limit_pao_sequence)
    {
        PrintError("由于炮序限制，tryPao系列函数不可使用！");
    }

    // 寻找第一个未被铲的炮
    auto pao_it_it = pao_list.begin();
    while ((*pao_it_it)->second.is_shoveled)
    {
        ++pao_it_it;
    }

    // 寻找 cd 最小的炮
    int now_time = GameClock();
    auto min_time_it_it = pao_it_it;
    do
    {
        if (!(*pao_it_it)->second.is_shoveled &&                                          // is_shoveled?
            (*min_time_it_it)->second.recover_time > (*pao_it_it)->second.recover_time && // is_recoverd?
            (conflict_resolution_type == DROP_POINT ||                                    // model_examine
             (conflict_resolution_type == COLLECTION && !is_drop_conflict((*pao_it_it)->first.row, (*pao_it_it)->first.col, row, col))))
        {
            min_time_it_it = pao_it_it;
        }
        ++pao_it_it;
    } while (pao_it_it != pao_list.end());

    while ((*min_time_it_it)->second.recover_time - 1 > GameClock())
    {
        Sleep(1);
    }

    // 确保发炮
    PlantMemory cannon_memory((*min_time_it_it)->second.index);
    while (cannon_memory.state() != 37)
        ;

    base_fire_pao(*min_time_it_it, GameClock(), row, col);
}

void PaoOperator::tryRecoverPao(const std::vector<PAO> &lst)
{
    if (limit_pao_sequence)
    {
        PrintError("由于炮序限制，tryPao系列函数不可使用！");
    }

    for (const auto &grid : lst)
    {
        tryRecoverPao(grid.row, grid.col);
    }
}

} // namespace pvz