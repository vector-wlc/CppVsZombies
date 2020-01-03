/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \CppVsZombies\pvz_h\include\pvz.h
 * @Date: 2019-08-31 23:04:00
 * @LastEditTime : 2019-12-31 16:24:54
 * @Description: PvZ Script Framework C++ vs. Zombies
 *               This framework references Python vs. Zombies
 *               @lmintlcx https://pvz.lmintlcx.com/scripts/pvz.py/
 */
#pragma once
#ifndef PVZ_H
#define PVZ_H

#include "libpvz.h"

using pvz::Card;            //用卡
using pvz::Delay;           //时间延迟
using pvz::dian_cai_placer; //放置垫材对象
using pvz::FixNut;          //修补坚果类
using pvz::Ice3;            //冰三
using pvz::ice_filler;      //存冰对象
using pvz::item_collector;  //自动收集
using pvz::KeyConnect;      //键盘指定操作
using pvz::KeyDown;         //检测键盘是否按下
using pvz::nut_fixer;       //修补坚果对象
using pvz::nv_pu_mi_ji;     //女仆秘籍对象
using pvz::OpenExamine;     //开启检查
using pvz::PaoOperator;     //炮操作类
using pvz::Prejudge;        //时间预判
using pvz::RunningInThread; //创建子线程
using pvz::SelectCards;     //选卡
using pvz::Shovel;          //铲除
using pvz::Until;           //等待时间到
using pvz::wave;            //波数
using pvz::wave_in;         //波数判定
using pvz::wave_not_in;     //波数判定

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//以下的宏是为了兼容 CvZ 之前的版本，现已不推荐使用

#define UpdatePaoList pvz::pao_cvz.resetPaoList
#define SetLimitPaoSequence pvz::pao_cvz.setLimitPaoSequence
#define SetResolveConflictType pvz::pao_cvz.setResolveConflictType
#define ChangePaoMessage pvz::pao_cvz.changePaoMessage
#define SetNextPao pvz::pao_cvz.setNextPao
#define RawPao pvz::pao_cvz.rawPao
#define SkipPao pvz::pao_cvz.skipPao
#define Pao pvz::pao_cvz.pao
#define TryPao pvz::pao_cvz.tryPao
#define RecoverPao pvz::pao_cvz.recoverPao
#define RoofPao pvz::pao_cvz.roofPao
#define TryRoofPao pvz::pao_cvz.tryRoofPao
#define RawRoofPao pvz::pao_cvz.rawRoofPao
#define FixPao pvz::pao_cvz.fixPao
#define TryFixPao pvz::pao_cvz.tryFixPao
#define ShovelPao pvz::pao_cvz.shovelPao
#define PlantPao pvz::pao_cvz.plantPao

#define StartAutoFillIceThread pvz::ice_filler.start
#define StopAutoFillIceThread pvz::ice_filler.stop
#define Coffee pvz::ice_filler.coffee

#define StartPreventDancerAdvance pvz::nv_pu_mi_ji.start
#define StopPreventDancerAdvance pvz::nv_pu_mi_ji.stop

#define StartAutoSetDianCaiThread pvz::dian_cai_placer.start
#define StopAutoSetDianCaiThread pvz::dian_cai_placer.stop

#define StartAutoFixNutThread pvz::nut_fixer.start
#define StopAutoFixNutThread pvz::nut_fixer.stop

#define StartAutoCollectThread pvz::item_collector.start
#define StopAutoCollectThread pvz::item_collector.stop

#endif // PVZ_H