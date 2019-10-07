

#include "libpvz.h"
#include "pvz_error.h"
#include "pvz_global.h"

namespace pvz
{
void BaseBaseAutoThread::error_messages()
{
	PrintError("错误", "自动操作类不允许一个对象创建多个线程");
}

BaseAutoThread::BaseAutoThread() : BaseBaseAutoThread() {}

void BaseAutoThread::base_reset_list(const std::vector<GRID> &lst)
{
	grid_lst.clear();
	if (g_examine_level == CVZ_NO)
		grid_lst = lst;
	else
	{
		for (const auto &e : lst)
		{
			//如果为池塘场景而且在水路
			if ((g_scene == 2 || g_scene == 3) &&
				(e.row == 3 || e.row == 4))
				//如果没有选择荷叶卡片且需要荷叶
				if (leaf_seed_index == -1 && GetPlantIndex(e.row, e.col, 16) == -1 && GetPlantIndex(e.row, e.col, 43) == -1)
					PrintError("错误", "您没有选择荷叶卡片，(#, #)需要荷叶", e.row, e.col);
			grid_lst.push_back(e);
		}
	}
}

FillIce::FillIce() : BaseAutoThread() {}

void FillIce::start(const std::vector<GRID> &lst)
{
	if (stop_)
	{
		stop_ = false;
		pause_ = false;
		//获得荷叶的对象序列
		leaf_seed_index = GetSeedIndex(16);
		resetFillList(lst);
		RunningInThread(&FillIce::use_ice, this);
	}
	else
	{
		error_messages();
	}
}

void FillIce::resetFillList(const std::vector<GRID> &lst) { base_reset_list(lst); }

void FillIce::use_ice()
{
	std::vector<int> ice_seed_index; //定义一个动态数组，存储寒冰菇卡片的对象序列

	WaitGameStart();

	int hbg = GetSeedIndex(14);
	int Mhbg = GetSeedIndex(14, true);
	//得到咖啡豆卡片对象序列
	coffee_index = GetSeedIndex(35);

	//获得寒冰菇卡片对象序列
	if (hbg != -1)
		ice_seed_index.push_back(hbg);
	//获得寒冰菇模仿者卡槽对象序列
	if (Mhbg != -1)
		ice_seed_index.push_back(Mhbg);

	SeedMemory ice_seed;
	SeedMemory leaf_seed(leaf_seed_index);

	while (!stop_)
	{
		Sleep(interval_);
		//等待继续命令
		if (pause_ || GamePaused() || GameUi() != 3)
			continue;

		for (int i = 0; i < ice_seed_index.size(); i++)
		{
			ice_seed.setIndex(ice_seed_index[i]);
			//如果卡片不能使用
			if (!ice_seed.isUsable())
				continue;
			//遍历所有存冰位
			for (int j = 0; j < grid_lst.size(); j++)
			{
				//如果此存冰坐标没有植物，使用寒冰菇
				if (GetPlantIndex(grid_lst[j].row, grid_lst[j].col) == -1)
				{
					//如果为池塘场景而且在水路
					if ((g_scene == 2 || g_scene == 3) &&
						(grid_lst[j].row == 3 || grid_lst[j].row == 4))
						//如果不存在荷叶
						if (GetPlantIndex(grid_lst[j].row, grid_lst[j].col, 16) == -1)
						{
							//如果荷叶卡片没有恢复
							if (!leaf_seed.isUsable())
								continue; //为了不影响其他地点的寒冰菇种植，跳过循环
							//种植荷叶
							Card(leaf_seed_index + 1, grid_lst[j].row, grid_lst[j].col);
						}

					//由于花盆比较特殊，在此不考虑天台自动补花盆情况

					//种植寒冰菇
					Card(ice_seed_index[i] + 1, grid_lst[j].row, grid_lst[j].col);
					break;
				}
			}
			int time = GameClock();
			while (GameClock() <= time + 1)
				Sleep(1);
		}
	}

	return;
}

//使用咖啡豆，必须配合自动存冰函数使用
void FillIce::coffee() const
{
	if (grid_lst.size() == 0)
		PrintError("警告", "存冰列表未初始化！");
	else if (coffee_index == -1)
		PrintError("警告", "您没有选择咖啡豆卡片!");
	else
	{
		g_mu.lock();
		RightClick(1, 1);
		LeftClick(70, 50 + 50 * (coffee_index + 1));
		for (int j = grid_lst.size() - 1; j >= 0; j--)
			SceneClick(grid_lst[j].row, grid_lst[j].col);
		RightClick(1, 1);

		if (g_examine_level == CVZ_INFO)
			std::printf("	Use coffee\n\n");
		g_mu.unlock();
	}
}

FixNut::FixNut() : BaseAutoThread() {}

void FixNut::start(int nut_type, const std::vector<GRID> &lst, int fix_hp)
{
	if (stop_)
	{
		pause_ = false;
		stop_ = false;
		if (nut_type != 3 && nut_type != 23 && nut_type != 30)
			PrintError("警告", "维修坚果类只能用于维修坚果类植物");
		//得到对应类型的坚果卡片对象序列
		nut_seed_index = GetSeedIndex(nut_type);
		//获得荷叶卡片的对象序列
		leaf_seed_index = GetSeedIndex(16);
		nut_type_ = nut_type;

		if (nut_seed_index == -1)
			PrintError("错误", "您没有选择对应的坚果卡片");

		//如果用户没有给出列表信息
		if (lst.size() == 0)
		{
			PlantMemory plant;
			int PCM = plant.countMax();
			GRID fix_grid;
			for (int i = 0; i < PCM; i++)
			{
				plant.setIndex(i);
				if (!plant.isCrushed() && !plant.isDisappeared() &&
					plant.type() == nut_type_)
				{
					fix_grid.row = plant.row() + 1;
					fix_grid.col = plant.col() + 1;

					if (g_examine_level != CVZ_NO)
					{
						//如果为池塘场景而且在水路
						if ((g_scene == 2 || g_scene == 3) &&
							(fix_grid.row == 3 || fix_grid.row == 4))
							//如果没有选择荷叶卡片且需要荷叶
							if (leaf_seed_index == -1 && GetPlantIndex(fix_grid.row, fix_grid.col, 16) == -1 && GetPlantIndex(fix_grid.row, fix_grid.col, 43) == -1)
								PrintError("错误", "您没有选择荷叶卡片，(#, #)需要荷叶", fix_grid.row, fix_grid.col);
					}
					grid_lst.push_back(fix_grid);
				}
			}
		}
		else
			base_reset_list(lst);

		resetFixHp(fix_hp);
		RunningInThread(&FixNut::update_nut, this);
	}
	else
	{
		error_messages();
	}
}

void FixNut::resetFixList(const std::vector<GRID> &lst) { base_reset_list(lst); }

void FixNut::resetFixHp(int fix_hp)
{
	if (nut_type_ == 23)
		fix_hp_ = fix_hp > 5332 ? 5332 : fix_hp;
	else
		fix_hp_ = fix_hp > 2665 ? 2665 : fix_hp;
}

//更新坚果
void FixNut::update_nut() const
{
	//等待游戏开始
	WaitGameStart();

	SeedMemory nut_seed(nut_seed_index);
	SeedMemory leaf_seed(leaf_seed_index);

	PlantMemory plant;
	//记录要使用坚果的格子
	GRID need_nut_grid;
	//记录坚果的最小生命值
	int min_nut_hp;

	while (!stop_)
	{
		Sleep(interval_);
		if (pause_ || GamePaused() || GameUi() != 3)
			continue;
		//格子信息置零
		need_nut_grid.row = need_nut_grid.col = 0;
		//最小生命值重置
		min_nut_hp = fix_hp_;
		//等待卡片回复
		if (!nut_seed.isUsable())
			continue;

		for (const auto &e : grid_lst)
		{
			//这里使用了多个 GetPlantIndex，开销大，待优化......

			int nut_index = GetPlantIndex(e.row, e.col, nut_type_);

			//如果此处存在除坚果类植物的植物
			if (GetPlantIndex(e.row, e.col) != -1 && nut_index == -1 && nut_type_ != 30)
				continue;

			//如果此处不存在坚果植物
			if (nut_index == -1)
			{
				//如果为池塘场景而且在水路
				if ((g_scene == 2 || g_scene == 3) &&
					(e.row == 3 || e.row == 4))
					//如果不存在荷叶
					if (GetPlantIndex(e.row, e.col, 16) == -1)
					{
						//如果荷叶卡片没有恢复
						if (leaf_seed_index == -1 || !leaf_seed.isUsable())
							continue;

						Card(leaf_seed_index + 1, e.row, e.col);
					}
				//种植坚果
				Card(nut_seed_index + 1, e.row, e.col);
				//延迟一定时间使得内存中的信息得以变化
				Sleep(50);
				break;
			}
			else //如果此处存在坚果类植物
			{
				plant.setIndex(nut_index);
				int nut_hp = plant.hp();
				//如果当前生命值低于最小生命值，记录下来此坚果的信息
				if (nut_hp < min_nut_hp)
				{
					min_nut_hp = nut_hp;
					need_nut_grid.row = e.row;
					need_nut_grid.col = e.col;
				}
			}
		}

		//如果有需要修补的坚果且坚果卡片已恢复则进行种植
		if (need_nut_grid.row && nut_seed.isUsable())
			Card(nut_seed_index + 1, need_nut_grid.row, need_nut_grid.col);
	}
}

NvPuMiJi::NvPuMiJi() : BaseBaseAutoThread() {}

void NvPuMiJi::stop_dancer_advance() const
{
	WaitGameStart();

	//启动女仆秘籍线程
	while (!stop_)
	{
		if (pause_ || GameUi() != 3)
			continue;
		//暂停判定条件
		while ((((DancerClock() + 10) % (20 * 23)) / 20) > 11)
			Sleep(1);
		//模拟敲击空格
		g_mu.lock();
		PressSpace();
		g_mu.unlock();
		Sleep(50);
		while (((DancerClock() % (20 * 23)) / 20) <= 11)
			Sleep(1);
		g_mu.lock();
		PressSpace();
		g_mu.unlock();
	}
}

void NvPuMiJi::start()
{
	if (stop_)
	{
		pause_ = false;
		stop_ = false;
		RunningInThread(&NvPuMiJi::stop_dancer_advance, this);
	}
	else
	{
		error_messages();
	}
}

PlaceDianCai::PlaceDianCai() : BaseBaseAutoThread()
{
	place_message_ = {{0, 1000.0, 9, false},
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

	for (auto &message : place_message_)
	{
		message.is_plantble = false;
	}

	for (const auto &grid : lst)
	{
		place_message_[grid.row - 1].is_plantble = true;
		place_message_[grid.row - 1].place_max_col = grid.col;
	}
}

void PlaceDianCai::resetSetDianCaiList()
{
	// 不填参数就是初始化成所有行都要种
	for (auto &message : place_message_)
	{
		message.is_plantble = true;
		message.place_max_col = 9;
	}
}

void PlaceDianCai::get_min_absci_zombie()
{
	//重置最小的僵尸坐标
	for (auto &message : place_message_)
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
			for (const auto &e : lst_zombie_)
				if (Ztype == e)
				{
					int Zrow = zombie.row();
					float Zabsci = zombie.abscissa();
					//如果是橄榄球
					if (Ztype == 7)
						Zabsci += 20;
					//如果现在僵尸的坐标值小于已记录的坐标值
					if (Zabsci < place_message_[Zrow].zombie_min_abscissa)
						place_message_[Zrow].zombie_min_abscissa = Zabsci;
					break;
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
		for (auto &message : place_message_)
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
				if (place_message_[Prow].plant_max_abscissa < Pabsci && Ptype != 47)
					place_message_[Prow].plant_max_abscissa = Pabsci;
				//如果是炮
				if (place_message_[Prow].plant_max_abscissa < (Pabsci + 80) && Ptype == 47)
					place_message_[Prow].plant_max_abscissa = Pabsci + 80;
			}
		}
	}
	//如果输入了值
	else
	{
		// 遍历参数列表
		for (const auto &e : lst)
		{
			place_message_[e.row - 1].plant_max_abscissa = int((e.col - 0.5) * 80);
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
	for (const auto &message : place_message_)
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
		int zombie_col = int((place_message_[mindistance_row - 1].zombie_min_abscissa + 70) / 80);
		diancai.col = zombie_col < place_message_[mindistance_row - 1].place_max_col ? zombie_col : place_message_[mindistance_row - 1].place_max_col;
	}
	else
		diancai.col = diancai.row = -1; //不存在时将两个值赋为-1
	return diancai;
}

//自动种植垫材函数
//原理：每次选取离植物最近的巨人种植垫材
void PlaceDianCai::auto_plant_diancai()
{
	WaitGameStart();

	//开启自动种植垫材
	stop_ = false;

	SeedMemory seed;

	while (!stop_)
	{
		Sleep(interval_);
		if (pause_ || GamePaused() || GameUi() != 3)
			continue;
		//遍历垫材植物列表
		for (const auto &e : lst_plant_)
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
						diancai.col--;
					//要种垫材的地方如果有植物(垫材)
					while (GetPlantIndex(diancai.row, diancai.col) != -1 || GetPlantIndex(diancai.row, diancai.col, 33) != -1)
						diancai.col--;
					//如果种植垫材的格子为需要保护的植物或有巨人举锤和小丑爆炸等
					if ((int((diancai.col - 0.5) * 80) <= place_message_[diancai.row - 1].plant_max_abscissa) ||
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
	if (stop_)
	{
		plant.getOffset();
		zombie.getOffset();
		pause_ = false;
		stop_ = false;
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

CollectItem::CollectItem() : BaseBaseAutoThread() {}

void CollectItem::start_auto_collect() const
{
	WaitGameStart();

	ItemMemory item;

	while (!stop_)
	{
		Sleep(interval_);
		if (pause_ || GamePaused() || GameUi() != 3)
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
				uncollect_item_count++;
		}

		if (!uncollect_item_count)
		{
			Sleep(200);
			continue;
		}

		for (int i = 0; i < item_count_max; i++)
		{
			if (GameUi() != 3)
				break;
			while (GamePaused() || MouseInGame())
				Sleep(100);
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
					Sleep(100);
				}
			}
		}
	}
}

void CollectItem::start()
{
	if (stop_)
	{
		pause_ = false;
		stop_ = false;
		RunningInThread(&CollectItem::start_auto_collect, this);
	}
	else
	{
		error_messages();
	}
}
} // namespace pvz