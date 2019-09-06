
/* coding=UTF-8 */

/*
 * 作者：向量
 * 日期：2019-08-11
 * 摘要：键控框架：C++ vs. Zombies 本文件包含常用接口，如需了解全部，请浏览 libpvz.h
 * 命名格式：类的私有成员函数命名方式全部为下划线，公有成员为小驼峰命名规则，普通函数为大驼峰命名规则
 */

#pragma once
#ifndef PVZ_H
#define PVZ_H

#include "libpvz.h"

using pvz::CancelAutoExit;		   //取消自动退出机制
using pvz::Card;				   //用卡
using pvz::Delay;				   //时间延迟
using pvz::dian_cai_placer;		   //放置垫材对象
using pvz::FixNut;				   //修补坚果类
using pvz::ice_filler;			   //存冰对象
using pvz::KeyConnect;			   //键盘指定操作
using pvz::KeyDown;				   //检测键盘是否按下
using pvz::nut_fixer;			   //修补坚果对象
using pvz::nv_pu_mi_ji;			   //女仆秘籍对象
using pvz::OpenExamine;			   //开启检查
using pvz::OpenHighPrecision;	  //开启高精度
using pvz::PaoOperator;			   //炮操作类
using pvz::Prejudge;			   //时间预判
using pvz::SelectCards;			   //选卡
using pvz::Shovel;				   //铲除
using pvz::StartAutoCollectThread; //自动收集
using pvz::StopAutoCollectThread;  //停止自动收集
using pvz::Until;				   //等待时间到
using pvz::wave;				   //波数

//如果波数在参数范围内
template <class... Args>
bool wave_in(Args... args)
{
	std::initializer_list<int> lst = {args...};
	for (auto e : lst)
		if (e == wave)
			return true;
	return false;
}

//如果波数在不参数范围内
template <class... Args>
bool wave_not_in(Args... args)
{
	std::initializer_list<int> lst = {args...};
	for (auto e : lst)
		if (e == wave)
			return false;
	return true;
}

//使函数在子线程中运行
template <class FP, class... Args>
void RunningInThread(FP fp, Args... args)
{
	std::thread task(fp, args...);
	task.detach();
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//以下函数是为了兼容 CvZ 之前的版本，现已不推荐使用

//更新炮列表
//使用示例:
//经典四炮：
//UpdatePaoList({ {3,1},{4,1},{3,3},{4,3} });
#define UpdatePaoList pvz::pao_cvz.resetPaoList

//设置炮序限制 参数为 false 则解除炮序限制，true 则增加炮序限制
//解除此限制后 fixPao 可铲种炮列表内的炮，tryPao 系列可使用， Pao 系列不可使用
//增加此限制后 fixPao 不可铲种炮列表内的炮，tryPao 系列不可使用， Pao 系列可使用
#define SetLimitPaoSequence pvz::pao_cvz.setLimitPaoSequence

//设定解决冲突模式
//使用示例：
//SetResolveConflictType(PaoOperator::DROP_POINT)---只解决落点冲突，不解决收集物点炮冲突
//SetResolveConflictType(PaoOperator::COLLECTION)---只解决收集物点炮冲突，不解决落点冲突
#define SetResolveConflictType pvz::pao_cvz.setResolveConflictType

//改变炮的信息
//请在手动或使用基础函数例如 Card 改变炮的信息后立即使用此函数
//使用示例：
//ChangePaoMessage(2,3,2,3)--------在手动铲种(2,3)位置的炮后，更改相关炮的信息
//ChangePaoMessage(2,3,2,4)--------手动位移铲种(2,3)位置的炮后，更改相关炮的信息
//ChangePaoMessage(0,0,2,3)--------手动增加(2,3)位置的炮后，更改相关炮的信息
#define ChangePaoMessage pvz::pao_cvz.changePaoMessage

//设置即将发射的下一门炮
//此函数只有在限制炮序的时候才可调用
//使用示例：
//SetNextPao(10)------将炮列表中第十门炮设置为下一门即将发射的炮
//setNextPao(2, 8)------将炮列表中位于 (2, 8) 的炮设置为下一门即将发射的炮
#define SetNextPao pvz::pao_cvz.setNextPao

//发炮函数：用户自定义位置发射
//注意：尽量不要使用此函数操作位于炮列表中的炮，因为使用此函数后自动识别的炮序与UpdatePaolist更新的炮序将无效！
//使用示例：
//RawPao(1,2,2,9)-----------------------将位置为（1，2）的炮发射到（2，9）
//RawPao({ {1,2,2,9},{1,3,5,9} })-------将位置为（1，2）的炮发射到（2，9），将位置为（1，3）的炮发射到（5，9）
#define RawPao pvz::pao_cvz.rawPao

//跳炮函数
//使用示例：
//Skipao(2)---跳过按照顺序即将要发射的2门炮
#define SkipPao pvz::pao_cvz.skipPao

//发炮函数
//使用示例：
//Pao(2,9)----------------炮击二行，九列
//Pao({ {2,9},{5,9} })-----炮击二行，九列，五行，九列
#define Pao pvz::pao_cvz.pao

//自动找炮函数
//使用示例
//TryPao(2,9)----------------在炮列表中找到可用的炮后，炮击二行，九列
//TryPao({ {2,9},{5,9} })-----在炮列表中找到可用的炮后，炮击二行，九列，五行，九列
//注意：此函数无视炮序，慎用
#define TryPao pvz::pao_cvz.tryPao

//发炮函数 炮CD恢复自动发炮
//使用示例：
//RecoverPao(2,9)----------------炮击二行，九列
//RecoverPao({ {2,9},{5,9} })-----炮击二行，九列，五行，九列
#define RecoverPao pvz::pao_cvz.recoverPao

//屋顶修正飞行时间发炮. 此函数开销较大不适合精确键控.
//此函数只适用于RE与ME 修正时间：387cs
//使用示例：
//RoofPao(3,7)---------------------修正飞行时间后炮击3行7列
//RoofPao({ {2,9},{5,9} })---------修正飞行时间后炮击2行9列,5行9列
#define RoofPao pvz::pao_cvz.roofPao

//自动找炮函数
//使用示例
//TryRoofPao(2,9)----------------在炮列表中找到可用的炮后，炮击二行，九列
//TryRoofPao({ {2,9},{5,9} })-----在炮列表中找到可用的炮后，炮击二行，九列，五行，九列
//注意：此函数无视炮序，慎用
#define TryRoofPao pvz::pao_cvz.tryRoofPao

//发炮函数：用户自定义位置发射，屋顶修正飞行时间发炮. 此函数开销较大不适合精确键控.
//注意：尽量不要使用此函数操作位于炮列表中的炮，因为使用此函数后自动识别的炮序与UpdatePaolist更新的炮序将无效！
//使用示例：
//RawRoofPao(1,2,2,9)-----------------------将位置为（1，2）的炮发射到（2，9）
//RawRoofPao({ {1,2,2,9},{1,3,5,9} })-------将位置为（1，2）的炮发射到（2，9），将位置为（1，3）的炮发射到（5，9）
#define RawRoofPao pvz::pao_cvz.rawRoofPao

//自动铲种炮函数
//使用示例：
//FixPao(3,4)------------铲种(3,4)位置的炮
//FixPao(3,4,1)--------铲掉(3,4)位置的炮，将炮种植在(3,4+1)位置上/位移铲种炮
#define FixPao pvz::pao_cvz.fixPao

//尝试铲种炮函数
//使用示例：
//FixPao()------------尝试铲种即将发射的下一门炮，该函数不支持位移铲种，也不支持延迟时间，因为没有意义
#define TryFixPao pvz::pao_cvz.tryFixPao


//自动存冰函数，该函数可以自动补荷叶
//使用示例：
//StartAutoFillIceThread({{1,3},{2,3}})--------------在1行3列，2行3列存冰
#define StartAutoFillIceThread pvz::ice_filler.start

//强制退出存冰线程
#define StopAutoFillIceThread pvz::ice_filler.stop

//使用咖啡豆函数（使用此函数之前必须使用StartAutoFillIceThread）
//使用示例：
//Coffee()-----自动使用优先级低的存冰位
#define Coffee pvz::ice_filler.coffee

//女仆秘籍
#define StartPreventDancerAdvance pvz::nv_pu_mi_ji.start

//停止女仆秘籍
#define StopPreventDancerAdvance pvz::nv_pu_mi_ji.stop

//自动种植垫材线程
//第一个参数：指定一组卡片作为垫材
//第二个参数：指定垫的位置，默认为全场都从第九列开始垫
//第三个参数：指定要垫的僵尸的类型，默认为{23,32}
//使用示例：
//StartAutoSetDianCaiThread({1,2,3,4})------将第 1 2 3 4 张卡片设为垫材，默认全场都从第九列开始垫，默认只垫 红眼 白眼
//StartAutoSetDianCaiThread({1,2,3,4},{{2,6},{4,5}})-----将第 1 2 3 4 张卡片设为垫材，只垫第 2 4 行，而且第二行从第六列开始垫，第四行从第五列开始垫
//StartAutoSetDianCaiThread({1,2,3,4},{{2,6},{4,5}},{23,32,7})----将第 1 2 3 4 张卡片设为垫材，只垫第 2 4 行，而且第二行从第六列开始垫，第四行从第五列开始垫,垫红白眼和橄榄
#define StartAutoSetDianCaiThread pvz::dian_cai_placer.start

//停止自动种植垫材
#define StopAutoSetDianCaiThread pvz::dian_cai_placer.stop

//自动维修坚果类植物函数(第一个参数为3则维修小坚果，为23则维修高坚果，为30则修补南瓜，
//不接受除此之外的任何参数，此函数自动补种荷叶
//使用示例：
//StartAutoFixNutThread(30)--------------维修全场南瓜
//StartAutoFixNutThread(3, {{1, 2}, {3, 4}, {4, 5}})-------维修一行二列，三行四列，四行五列的坚果
//StartAutoFixNutThread(23, {{1, 2}, {3, 4},1333)----------维修一行二列，三行四列的高坚果，等待血量降至1333开始修补
#define StartAutoFixNutThread pvz::nut_fixer.start

//强制退出修坚果线程
#define StopAutoFixNutThread pvz::nut_fixer.stop

#endif //PVZ_H