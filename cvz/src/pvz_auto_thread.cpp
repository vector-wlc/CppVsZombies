/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \pvz_h\source\pvz_auto_thread.cpp
 * @Date: 2019-10-10 23:46:10
 * @LastEditTime: 2020-03-22 15:30:11
 * @Description: 自动操作类的实现
 */

#include "libpvz.h"
#include "pvz_error.h"
#include "pvz_global.h"

// #define __CVZ_DEBUG__

namespace pvz
{
/////////////////////////////////////////////////
//    BaseAutoThread
/////////////////////////////////////////////////

void BaseAutoThread::error_messages()
{
	PrintError("自动操作类不允许一个对象创建多个线程");
}

/////////////////////////////////////////////////
//    FillIce
/////////////////////////////////////////////////

void FillIce::start(const std::vector<GRID> &lst)
{
	if (is_stoped)
	{
		is_stoped = false;
		is_paused = false;
		resetFillList(lst);
		RunningInThread(&FillIce::use_ice, this);
	}
	else
	{
		error_messages();
	}
}

void FillIce::use_ice()
{
	WaitGameStart();
	SeedMemory ice_seed_memory;
	std::vector<int> ice_seed_index_vec;

	// 初始化
	for (int i = 0; i < 2; ++i)
	{
		int ice_index;
		if (i == 0)
		{
			ice_index = GetSeedIndex(HBG_14);
		}
		else
		{
			ice_index = GetSeedIndex(HBG_14, true);
		}

		if (ice_index != -1)
		{
			ice_seed_index_vec.push_back(ice_index);
		}
	}

	coffee_seed_index = GetSeedIndex(KFD_35);
	std::vector<int> ice_plant_index_vec;
	std::vector<int> usable_iseed_index_vec;
	decltype(ice_plant_index_vec.begin()) ice_plant_index_it;
	decltype(grid_lst.begin()) grid_it;
	bool is_get_indexs = false;

	while (!is_stoped)
	{
		Sleep(time_interval);
		if (GamePaused() || GameUi() != 3 || is_paused)
		{
			continue;
		}
		grid_it = grid_lst.begin();
		usable_iseed_index_vec.clear();
		for (const auto &index : ice_seed_index_vec)
		{
			ice_seed_memory.setIndex(index);
			if (5000 - 1 <= ice_seed_memory.CD() ||
				ice_seed_memory.isUsable()) // 精准种植
			{
				usable_iseed_index_vec.push_back(index);
			}
		};

#ifdef __CVZ_DEBUG__
		system("cls");
		printf("%d\n", usable_iseed_index_vec.size());
#endif
		if (usable_iseed_index_vec.size() == 0)
		{
			continue;
		}

		GetPlantIndexs(grid_lst, HBG_14, ice_plant_index_vec);

#ifdef __CVZ_DEBUG__
		for (const auto &each : ice_plant_index_vec)
		{
			printf("%d ", each);
		}
#endif
		ice_plant_index_it = ice_plant_index_vec.begin();

		for (auto &index : usable_iseed_index_vec)
		{
			for (; ice_plant_index_it != ice_plant_index_vec.end();
				 ++grid_it, ++ice_plant_index_it)
			{
				if ((*ice_plant_index_it) == -1)
				{
					// 保证种上去
					ice_seed_memory.setIndex(index);
					while (!ice_seed_memory.isUsable())
						;
					Card(index + 1, grid_it->row, grid_it->col);
					++grid_it;
					++ice_plant_index_it;
					break;
				}
			}
		}
	}
}

void FillIce::resetFillList(const std::vector<GRID> &lst)
{
	g_mu.lock();
	grid_lst = lst;
	g_mu.unlock();
}

//使用咖啡豆，必须配合自动存冰函数使用
void FillIce::coffee() const
{
	if (coffee_seed_index == -1)
	{
		PrintError("您没有选择咖啡豆卡片!");
	}

	g_mu.lock();
	RightClick(1, 1);
	LeftClick(70, 50 + 50 * (coffee_seed_index + 1));
	auto grid_it = grid_lst.end();
	do
	{
		--grid_it;
		SceneClick(grid_it->row, grid_it->col);
	} while (grid_it != grid_lst.begin());
	RightClick(1, 1);
	g_mu.unlock();
}

/////////////////////////////////////////////////
//    FillPlant
/////////////////////////////////////////////////

void FixPlant::autoGetFixList()
{
	PlantMemory plant;

	int plant_cnt_max = PlantMemory::countMax();
	GRID grid;

	for (int index = 0; index < plant_cnt_max; ++index)
	{
		plant.setIndex(index);
		if (!plant.isCrushed() && !plant.isDisappeared() && plant.type() == plant_type)
		{
			grid.col = plant.col() + 1;
			grid.row = plant.row() + 1;
			grid_lst.push_back(grid);
		}
	}
}

bool FixPlant::use_seed(int seed_index, int row, float col, bool is_need_shovel)
{
	if (is_use_coffee)
	{
		if (coffee_seed_index == -1)
		{
			return false;
		}
		SeedMemory coffee_seed(coffee_seed_index);
		if (!coffee_seed.isUsable())
		{
			return false;
		}
	}
	if (is_need_shovel)
	{
		if (plant_type == NGT_30)
		{
			Shovel(row, col, true);
		}
		else
		{
			Shovel(row, col);
		}
	}
	Card(seed_index + 1, row, col);
	if (is_use_coffee)
	{
		Card(coffee_seed_index + 1, row, col);
	}
	return true;
}

void FixPlant::get_seed_list()
{
	int seed_index;
	seed_index = GetSeedIndex(plant_type);
	if (-1 != seed_index)
	{
		seed_index_vec.push_back(seed_index);
	}
	seed_index = GetSeedIndex(plant_type, true);
	if (-1 != seed_index)
	{
		seed_index_vec.push_back(seed_index);
	}
	leaf_seed_index = GetSeedIndex(HY_16);
	coffee_seed_index = GetSeedIndex(KFD_35);
}

void FixPlant::resetFixHp(int _fix_hp)
{
	fix_hp = _fix_hp;
}

void FixPlant::start(int _plant_type, const std::vector<GRID> &lst, int _fix_hp)
{
	if (is_stoped)
	{
		is_paused = false;
		is_stoped = false;
		if (_plant_type >= JQSS_40)
		{
			PrintError("修补植物类仅支持绿卡");
		}
		plant_type = _plant_type;
		fix_hp = _fix_hp;
		get_seed_list();

		//如果用户没有给出列表信息
		if (lst.size() == 0)
		{
			autoGetFixList();
		}
		else
		{
			grid_lst = lst;
		}

		RunningInThread(&FixPlant::update_plant, this);
	}
	else
	{
		error_messages();
	}
}

void FixPlant::update_plant()
{
	//等待游戏开始
	WaitGameStart();

	SeedMemory seed_memory;
	SeedMemory leaf_seed_memory(leaf_seed_index);
	PlantMemory plant;
	std::vector<int> plant_index_vec;
	GRID need_plant_grid; //记录要使用植物的格子
	int min_hp;			  //记录要使用植物的格子
	bool is_seed_used;	  //种子是否被使用
	decltype(seed_index_vec.begin()) usable_seed_index_it;
	decltype(plant_index_vec.begin()) plant_index_it;
	decltype(grid_lst.begin()) grid_it;

	while (!is_stoped)
	{
		Sleep(time_interval);
		if (is_paused || GamePaused() || GameUi() != 3)
			continue;

		usable_seed_index_it = seed_index_vec.begin();
		do
		{
			seed_memory.setIndex(*usable_seed_index_it);
			if (seed_memory.isUsable())
			{
				break;
			}
			++usable_seed_index_it;
		} while (usable_seed_index_it != seed_index_vec.end());

		// 没找到可用的卡片
		if (usable_seed_index_it == seed_index_vec.end())
			continue;

		GetPlantIndexs(grid_lst, plant_type, plant_index_vec);

		is_seed_used = false;
		need_plant_grid.row = need_plant_grid.col = 0; //格子信息置零
		min_hp = fix_hp;							   //最小生命值重置

		for (grid_it = grid_lst.begin(), plant_index_it = plant_index_vec.begin();
			 grid_it != grid_lst.end();
			 ++grid_it, ++plant_index_it)
		{
			//如果此处存在除植物类植物的植物
			if (*plant_index_it == -2)
			{
				continue;
			}

			if (*plant_index_it == -1)
			{
				//如果为池塘场景而且在水路
				if ((g_scene == 2 || g_scene == 3) &&
					(grid_it->row == 3 || grid_it->row == 4))
				{ //如果不存在荷叶
					if (GetPlantIndex(grid_it->row, grid_it->col, 16) == -1)
					{
						//如果荷叶卡片没有恢复
						if (leaf_seed_index == -1 || !leaf_seed_memory.isUsable())
							continue;

						Card(leaf_seed_index + 1, grid_it->row, grid_it->col);
					}
				}
				is_seed_used = use_seed((*usable_seed_index_it), grid_it->row, grid_it->col, false);
				break;
			}
			else
			{
				plant.setIndex(*plant_index_it);
				int plant_hp = plant.hp();
				//如果当前生命值低于最小生命值，记录下来此植物的信息
				if (plant_hp < min_hp)
				{
					min_hp = plant_hp;
					need_plant_grid.row = grid_it->row;
					need_plant_grid.col = grid_it->col;
				}
			}
		}

		//如果有需要修补的植物且植物卡片能用则进行种植
		if (need_plant_grid.row && !is_seed_used)
		{
			//种植植物
			use_seed((*usable_seed_index_it), need_plant_grid.row, need_plant_grid.col, true);
		}
	}
}

/////////////////////////////////////////////////
//    PlaceDianCai
/////////////////////////////////////////////////

PlaceDianCai::PlaceDianCai() : BaseAutoThread()
{
	place_message_list = {{0, 1000.0, 9, false},
						  {0, 1000.0, 9, false},
						  {0, 1000.0, 9, false},
						  {0, 1000.0, 9, false},
						  {0, 1000.0, 9, false},
						  {0, 1000.0, 9, false}};
}

void PlaceDianCai::resetSetDianCaiList(const std::vector<GRID> &lst)
{
	if (lst.size() == 0)
	{
		resetSetDianCaiList();
		return;
	}

	for (auto &message : place_message_list)
	{
		message.is_plantble = false;
	}

	for (const auto &grid : lst)
	{
		place_message_list[grid.row - 1].is_plantble = true;
		place_message_list[grid.row - 1].place_max_col = grid.col;
	}
}

void PlaceDianCai::resetSetDianCaiList()
{
	// 不填参数就是初始化成所有行都要种
	for (auto &message : place_message_list)
	{
		message.is_plantble = true;
		message.place_max_col = 9;
	}
}

void PlaceDianCai::get_min_absci_zombie()
{
	//重置最小的僵尸坐标
	for (auto &message : place_message_list)
		message.zombie_min_abscissa = 900.0;

	int zombies_count_max = zombie.countMax();

	for (int i = 0; i < zombies_count_max; i++)
	{
		zombie.setIndex(i);
		//如果僵尸存在
		if (zombie.isExist() && !zombie.isDead())
		{
			//记录僵尸类型
			int Ztype = zombie.type();
			for (const auto &e : prevent_zombie_type_list)
			{
				if (Ztype == e)
				{
					int Zrow = zombie.row();
					float Zabsci = zombie.abscissa();
					//如果是橄榄球
					if (Ztype == 7)
						Zabsci += 20;
					//如果现在僵尸的坐标值小于已记录的坐标值
					if (Zabsci < place_message_list[Zrow].zombie_min_abscissa)
						place_message_list[Zrow].zombie_min_abscissa = Zabsci;
					break;
				}
			}
		}
	}
}

void PlaceDianCai::resetProtectedPlantList(const std::vector<GRID> &lst)
{
	//如果没有输入任何值，读取内存相关信息进行初始化
	if (lst.size() == 0)
	{
		//初始化
		for (auto &message : place_message_list)
		{
			message.plant_max_abscissa = 0;
		}

		int plants_count_max = plant.countMax();
		for (int i = 0; i < plants_count_max; i++)
		{
			plant.setIndex(i);
			//如果植物存在
			if (!plant.isDisappeared() && !plant.isCrushed())
			{
				int Prow = plant.row();
				int Pabsci = plant.abscissa();
				int Ptype = plant.type();
				//如果不是炮
				if (place_message_list[Prow].plant_max_abscissa < Pabsci && Ptype != 47)
					place_message_list[Prow].plant_max_abscissa = Pabsci;
				//如果是炮
				if (place_message_list[Prow].plant_max_abscissa < (Pabsci + 80) && Ptype == 47)
					place_message_list[Prow].plant_max_abscissa = Pabsci + 80;
			}
		}
	}
	//如果输入了值
	else
	{
		// 遍历参数列表
		for (const auto &e : lst)
		{
			place_message_list[e.row - 1].plant_max_abscissa = int((e.col - 0.5) * 80);
		}
	}
}

//得到需要种植垫材的格子
GRID PlaceDianCai::need_diancai_grid() const
{
	float min_distance = 1000;
	float now_distance = 0;
	int row = 1;
	int mindistance_row = 0;
	for (const auto &message : place_message_list)
	{
		if (message.is_plantble)
		{
			now_distance = message.zombie_min_abscissa - message.plant_max_abscissa;
			// 找到距离更小的行数，记录信息
			if (min_distance > now_distance)
			{
				min_distance = now_distance;
				mindistance_row = row;
			}
		}
		row++;
	}
	GRID diancai;
	//如果场上存在僵尸
	if (mindistance_row != -1)
	{
		diancai.row = mindistance_row;
		int zombie_col = int((place_message_list[mindistance_row - 1].zombie_min_abscissa + 70) / 80);
		diancai.col = zombie_col < place_message_list[mindistance_row - 1].place_max_col ? zombie_col : place_message_list[mindistance_row - 1].place_max_col;
	}
	else
	{
		diancai.col = diancai.row = -1; //不存在时将两个值赋为-1
	}
	return diancai;
}

//自动种植垫材函数
//原理：每次选取离植物最近的巨人种植垫材
void PlaceDianCai::auto_plant_diancai()
{
	WaitGameStart();

	//开启自动种植垫材
	is_stoped = false;

	SeedMemory seed;

	while (!is_stoped)
	{
		Sleep(time_interval);
		if (is_paused || GamePaused() || GameUi() != 3)
			continue;
		//遍历垫材植物列表
		for (const auto &e : diancai_plant_type_list)
		{
			seed.setIndex(e - 1);
			if (seed.isUsable())
			{
				//得到每行最小的巨人僵尸横坐标
				get_min_absci_zombie();

				//得到需要种植垫材的格子
				GRID diancai = need_diancai_grid();
				//如果僵尸存在
				if (diancai.row > 0)
				{
					//如果需要种植垫材的格子无法种植
					while (!IsPlantable(diancai.row, diancai.col))
						--diancai.col;
					//要种垫材的地方如果有植物(垫材)
					while (GetPlantIndex(diancai.row, diancai.col) != -1 || GetPlantIndex(diancai.row, diancai.col, 33) != -1)
						--diancai.col;
					//如果种植垫材的格子为需要保护的植物或有巨人举锤和小丑爆炸等
					if ((int((diancai.col - 0.5) * 80) <= place_message_list[diancai.row - 1].plant_max_abscissa) ||
						IsHammering(diancai.row, diancai.col) ||
						IsExplode(diancai.row, diancai.col))
						break;

					Card(e, diancai.row, diancai.col);
				}
				break;
			}
		}
	}
}

//开启放置垫材线程
void PlaceDianCai::start(const std::vector<int> &lst_plant,
						 const std::vector<GRID> &lst_grid,
						 const std::vector<int> &lst_zombie)
{
	if (is_stoped)
	{
		plant.getOffset();
		zombie.getOffset();
		is_paused = false;
		is_stoped = false;
		resetSetDianCaiList(lst_grid);
		resetDianCaiSeedList(lst_plant);
		//得到每行最大的植物横坐标
		resetProtectedPlantList();
		resetZombieTypeList(lst_zombie);
		RunningInThread(&PlaceDianCai::auto_plant_diancai, this);
	}
	else
	{
		error_messages();
	}
}

/////////////////////////////////////////////////
//    CollectItem
/////////////////////////////////////////////////

CollectItem::CollectItem() : BaseAutoThread() {}

void CollectItem::start_auto_collect() const
{
	WaitGameStart();

	ItemMemory item;

	uintptr_t mouse_attribution_offset = ReadMemory<uintptr_t>(g_mainobject + 0x138); // 鼠标属性偏移

	while (!is_stoped)
	{

#ifdef __CVZ_DEBUG__
		printf("CollectItem\n");
#endif

		Sleep(time_interval);
		if (is_paused || GamePaused() || GameUi() != 3)
			continue;
		int item_count_max = item.countMax();
		if (!item.count())
		{
			Sleep(100);
			continue;
		}
		int uncollect_item_count = 0;
		for (int i = 0; i < item_count_max; i++)
		{
			item.setIndex(i);
			if (!item.isDisappeared() && !item.isCollected())
				++uncollect_item_count;
		}

		if (!uncollect_item_count)
		{
			Sleep(time_interval);
			continue;
		}

		for (int i = 0; i < item_count_max; i++)
		{
			if (GameUi() != 3)
				break;
			// 游戏暂停或鼠标上面有物品
			while (GamePaused() || ReadMemory<int>(mouse_attribution_offset + 0x30) != 0)
				Sleep(time_interval);
			item.setIndex(i);
			if (!item.isDisappeared() && !item.isCollected())
			{
				float item_x = item.abscissa();
				float item_y = item.ordinate();
				if (item_x >= 0.0 && item_y >= 70)
				{
					g_mu.lock();
					RightClick(1, 1);
					int x = static_cast<int>(item_x + 30);
					int y = static_cast<int>(item_y + 30);
					LeftClick(y, x);
					RightClick(1, 1);
					g_mu.unlock();
					Sleep(time_interval);
				}
				int x = 0xFFFFFFF;
			}
		}
	}
}

void CollectItem::start()
{
	if (is_stoped)
	{
		is_paused = false;
		is_stoped = false;
		RunningInThread(&CollectItem::start_auto_collect, this);
	}
	else
	{
		error_messages();
	}
}
} // namespace pvz