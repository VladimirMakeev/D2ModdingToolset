/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2021 Stanislav Egorov.
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

#ifndef COMMANDMSG_H
#define COMMANDMSG_H

#include "idlist.h"
#include "netmsg.h"

namespace game {

/** Network command messages identifiers. */
enum class CommandMsgId : int
{
    EraseObject = 0,
    UpdateObject = 1,
    MoveStack = 2,
    StackVisit = 3,
    StackDestroyed = 4,
    StackAppear = 5,
    StackIllusion = 6,
    StackAction = 7,
    BattleStart = 8,
    BattleChooseAction = 9,
    BattleResult = 10,
    BattleEnd = 11,
    NobleAction = 12,
    NobleActionCancel = 13,
    NobleResult = 14,
    ProposeTrade = 15,
    TradeAccept = 16,
    ProposeAlliance = 17,
    AcceptAlliance = 18,
    BreakAlliance = 19,
    GiveGold = 20,
    BeginTurn = 21,
    TurnInfo = 22,
    EndTurn = 23,
    BreakRod = 24,
    DropRod = 25,
    CastSpell = 26,
    TerrainChange = 27,
    MapChange = 28,
    RemoveLandmark = 29,
    OccupyCity = 30,
    CityGrow = 31,
    LootRuin = 32,
    PickupBag = 33,
    OpenBag = 34,
    CloseBag = 35,
    DiscoverSpy = 36,
    GameSaved = 37,
    ScenarioEnd = 38,
    // 39 ?
    CreateStack = 40,
    ChangeOwner = 41,
    ChangeOrder = 42,
    GiveSpell = 43,
    GiveItem = 44,
    ChangeObjective = 45,
    ChangeFog = 46,
    ChatMessage = 47,
    Popup = 48,
    RemoveRiot = 49,
    UpgradeLeader = 50,
    // 51 ?
    CaptureCapital = 52,
    CaptureResources = 53,
    QueueCommands = 54,
    UnpauseAi = 55,
    Eem = 56,
    TurnSummary = 57,
    ExportLeader = 58,
    ChangeLandmark = 59,
    ProposeExchangeMap = 60,
    AcceptExchangeMap = 61,
};

struct CCommandMsgVftable;
struct IMidgardObjectMap;

struct CCommandMsg : public CNetMsgT<CCommandMsgVftable>
{
    CMidgardID playerId;
    int unknown;
    IdList* ids;
};

assert_size(CCommandMsg, 16);

enum class CommandMsgParam : int
{
    Value0,
    Value1,
    Value2,
};

template <CommandMsgId ID, CommandMsgParam P>
struct CCommandMsgTempl : public CCommandMsg
{ };

struct CCommandMsgVftable : public CNetMsgVftable
{
    using GetId = CommandMsgId(__thiscall*)(CCommandMsg* thisptr);
    GetId getId;

    using GetParam = CommandMsgParam(__thiscall*)(CCommandMsg* thisptr);
    GetParam getParam;

    /** Returns true if command can be ignored by current player. */
    using CanIgnore = bool(__thiscall*)(const CCommandMsg* thisptr,
                                        const IMidgardObjectMap* objectMap,
                                        const IdList* playerIds,
                                        const CMidgardID* currentPlayerId);
    CanIgnore canIgnore;
};

assert_vftable_size(CCommandMsgVftable, 5);

namespace CCommandMsgApi {

struct Api
{
    using Destructor = void(__thiscall*)(CCommandMsg* thisptr);
    Destructor destructor;
};

Api& get();

} // namespace CCommandMsgApi

} // namespace game

#endif // COMMANDMSG_H
