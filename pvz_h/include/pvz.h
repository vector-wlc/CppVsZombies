/*
 * @coding: utf-8
 * @Author: Chu Wenlong
 * @FilePath: \CppVsZombies\pvz_h\include\pvz.h
 * @Date: 2019-08-31 23:04:00
 * @LastEditTime : 2020-01-27 15:45:31
 * @Description: PvZ TAS Framework C++ vs. Zombies
 *               This framework references Python vs. Zombies
 *               @lmintlcx https://pvz.lmintlcx.com/scripts/pvz.py/
 */
#pragma once
#ifndef PVZ_H
#define PVZ_H

#include "libpvz.h"

using pvz::Card;
using pvz::Delay;
using pvz::dian_cai_placer;
using pvz::FixPlant;
using pvz::RecoverCard;
using FixNut = FixPlant;
using pvz::Ice3;
using pvz::ice_filler;
using pvz::item_collector;
using pvz::KeyConnect;
using pvz::KeyDown;
using pvz::nut_fixer;
using pvz::OpenExamine;
using pvz::PaoOperator;
using pvz::Prejudge;
using pvz::RunningInThread;
using pvz::SelectCards;
using pvz::Shovel;
using pvz::StartMaidCheats;
using pvz::StopMaidCheats;
using pvz::Until;
using pvz::wave;
using pvz::wave_in;
using pvz::wave_not_in;

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

#define StartAutoSetDianCaiThread pvz::dian_cai_placer.start
#define StopAutoSetDianCaiThread pvz::dian_cai_placer.stop

#define StartAutoFixNutThread pvz::nut_fixer.start
#define StopAutoFixNutThread pvz::nut_fixer.stop

#define StartAutoCollectThread pvz::item_collector.start
#define StopAutoCollectThread pvz::item_collector.stop

#endif // PVZ_H