/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Vladimir Makeev.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MIDSERVERLOGIC_H
#define MIDSERVERLOGIC_H

#include "d2set.h"
#include "midmsgsender.h"
#include "midserverlogiccore.h"

namespace game {

struct CMidServer;

struct CMidServerLogicData
{
    int unknown;
    int unknown2;
    int unknown3;
};

struct CMidServerLogicData2
{
    CMidgardID unknownId;
    int unknown;
    int unknown2;
    Vector<void*> array;
    Vector<void*> array2;
    Vector<void*> array3;
    Vector<void*> array4;
    Vector<void*> array5;
    Vector<void*> array6;
    Set<CMidgardID> list;
    Set<void> list2;
};

static_assert(sizeof(CMidServerLogicData2) == 164,
              "Size of CMidServerLogicData2 structure must be exactly 164 bytes");

static_assert(offsetof(CMidServerLogicData2, array) == 12,
              "CMidServerLogicData2::array offset must be 12 bytes");

static_assert(offsetof(CMidServerLogicData2, array2) == 28,
              "CMidServerLogicData2::array2 offset must be 28 bytes");

static_assert(offsetof(CMidServerLogicData2, array3) == 44,
              "CMidServerLogicData2::array3 offset must be 44 bytes");

static_assert(offsetof(CMidServerLogicData2, array4) == 60,
              "CMidServerLogicData2::array4 offset must be 60 bytes");

static_assert(offsetof(CMidServerLogicData2, array5) == 76,
              "CMidServerLogicData2::array5 offset must be 76 bytes");

static_assert(offsetof(CMidServerLogicData2, array6) == 92,
              "CMidServerLogicData2::array6 offset must be 92 bytes");

static_assert(offsetof(CMidServerLogicData2, list) == 108,
              "CMidServerLogicData2::list offset must be 108 bytes");

static_assert(offsetof(CMidServerLogicData2, list2) == 136,
              "CMidServerLogicData2::list2 offset must be 136 bytes");

struct CMidServerLogic
    : public CMidServerLogicCore
    , public IMidMsgSender
{
    CMidServer* midServer;
    int unknown2;
    int unknown3;
    CMidServerLogicData data;
    Vector<void*> array;
    int unknown5;
    List<CMidgardID> playersIdList;
    int unknown6;
    int unknown7;
    CMidServerLogicData2 data2;
    int unknown8;
    int unknown9;
    bool turnNumberIsZero;
    char padding[3];
    List<void*> list;
    int unknown11;
    int unknown12;
    std::uint32_t aiMessageId;
    int unknown14;
    bool upgradeLeaders;
    char padding2[3];
    int unknown16;
    CMidgardID winnerPlayerId;
    List<void*> list2;
    char unknown17;
    char padding3[3];
    BattleMsgData* battleMsgData;
};

static_assert(sizeof(CMidServerLogic) == 324,
              "Size of CMidServerLogic structure must be exactly 324 bytes");

static_assert(offsetof(CMidServerLogic, CMidServerLogic::IMidMsgSender::vftable) == 8,
              "Vftable offset for IMidMsgSender in CMidServerLogic structure must be 8 bytes");

static_assert(offsetof(CMidServerLogic, data) == 24,
              "CMidServerLogic::data offset must be 24 bytes");

static_assert(offsetof(CMidServerLogic, array) == 36,
              "CMidServerLogic::array offset must be 36 bytes");

static_assert(offsetof(CMidServerLogic, playersIdList) == 56,
              "CMidServerLogic::playersIdList offset must be 56 bytes");

static_assert(offsetof(CMidServerLogic, data2) == 80,
              "CMidServerLogic::data2 offset must be 80 bytes");

static_assert(offsetof(CMidServerLogic, unknown8) == 244,
              "CMidServerLogic::unknown8 offset must be 244 bytes");

static_assert(offsetof(CMidServerLogic, unknown11) == 272,
              "CMidServerLogic::unknown11 offset must be 272 bytes");

static_assert(offsetof(CMidServerLogic, list2) == 300,
              "CMidServerLogic::list2 offset must be 300 bytes");

static_assert(offsetof(CMidServerLogic, unknown17) == 316,
              "CMidServerLogic::unknown17 offset must be 316 bytes");

namespace CMidServerLogicApi {

struct Api
{
    using GetObjectMap = IMidgardObjectMap*(__thiscall*)(CMidServerLogic* thisptr);
    GetObjectMap getObjectMap;
};

Api& get();

} // namespace CMidServerLogicApi

} // namespace game

#endif // MIDSERVERLOGIC_H
