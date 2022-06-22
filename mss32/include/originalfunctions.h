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

#ifndef ORIGINALFUNCTIONS_H
#define ORIGINALFUNCTIONS_H

#include "attackimpl.h"
#include "battlemsgdata.h"
#include "citystackinterf.h"
#include "commandmsg.h"
#include "condinterf.h"
#include "d2osexception.h"
#include "ddcarryoveritems.h"
#include "effectinterf.h"
#include "effectresult.h"
#include "enclayoutspell.h"
#include "enclayoutstack.h"
#include "enclayoutunit.h"
#include "exchangeinterf.h"
#include "game.h"
#include "gameimages.h"
#include "menubase.h"
#include "menuload.h"
#include "menunewskirmishmulti.h"
#include "menuphase.h"
#include "menuprotocol.h"
#include "midevcondition.h"
#include "mideveffect.h"
#include "midevent.h"
#include "midunit.h"
#include "netmsg.h"
#include "pickupdropinterf.h"
#include "sitemerchantinterf.h"
#include "testcondition.h"

namespace hooks {

struct OriginalFunctions
{
    game::ComputePlayerDailyIncome computePlayerDailyIncome;
    game::CreateBatAttack createBatAttack;
    game::GetAttackClassWardFlagPosition getAttackClassWardFlagPosition;
    game::AttackClassToString attackClassToString;
    game::MarkMapPosition markMapPosition;
    game::CCityStackInterfApi::Api::Constructor cityStackInterfCtor;
    game::CExchangeInterfApi::Api::Constructor exchangeInterfCtor;
    game::CPickUpDropInterfApi::Api::Constructor pickupDropInterfCtor;
    game::CSiteMerchantInterfApi::Api::Constructor siteMerchantInterfCtor;
    game::CDDCarryOverItemsApi::Api::Constructor carryOverItemsCtor;
    game::CEncLayoutSpellApi::Api::Constructor encLayoutSpellCtor;
    game::os_exceptionApi::Api::ThrowException throwException;
    game::CMidUnitApi::Api::AddRemoveModifier removeModifier;
    game::BattleMsgDataApi::Api::FindAttackTarget findAttackTarget;
    game::BattleMsgDataApi::Api::Constructor battleMsgDataCtor;
    game::GetSoldierImmunityAiRating getSoldierImmunityAiRating;
    game::GetAttackSourceWardFlagPosition getAttackSourceWardFlagPosition;
    game::BattleMsgDataApi::Api::AddUnitToBattleMsgData addUnitToBattleMsgData;
    game::CNetMsgVftable::Serialize stackBattleActionMsgSerialize;
    game::CNetMsgVftable::Serialize cmdBattleStartMsgSerialize;
    game::CNetMsgVftable::Serialize cmdBattleChooseActionMsgSerialize;
    game::CNetMsgVftable::Serialize cmdBattleResultMsgSerialize;
    game::CNetMsgVftable::Serialize cmdBattleEndMsgSerialize;
    game::CCommandMsgApi::Api::Destructor commandMsgDtor;
    game::CNetMsgApi::Api::Destructor netMsgDtor;
    game::CAttackImplApi::Api::Constructor2 attackImplCtor2;
    game::IAttackVftable::GetData attackImplGetData;
    game::CMidEvConditionApi::Api::CreateFromCategory createEventConditionFromCategory;
    game::CMidEvConditionApi::Api::GetInfoString eventConditionGetInfoString;
    game::CMidEvConditionApi::Api::GetDescription eventConditionGetDescription;
    game::CMidEvConditionApi::Api::GetDescription eventConditionGetBrief;
    game::editor::CCondInterfApi::Api::CreateFromCategory createCondInterfFromCategory;
    game::ITestConditionApi::Api::Create createTestCondition;
    game::CMidEventApi::Api::CheckValid checkEventValid;
    game::BattleMsgDataApi::Api::BeforeBattleRound beforeBattleRound;
    game::CMidUnitApi::Api::AddModifiers addModifiers;
    game::CMidUnitVftable::InitWithSoldierImpl initWithSoldierImpl;

    game::CMidEvEffectApi::Api::CreateFromCategory createEventEffectFromCategory;
    game::CMidEvEffectApi::Api::GetInfoString eventEffectGetInfoString;
    game::CMidEvEffectApi::Api::GetDescription eventEffectGetDescription;
    game::CMidEvEffectApi::Api::GetDescription eventEffectGetBrief;
    game::editor::CEffectInterfApi::Api::CreateFromCategory createEffectInterfFromCategory;

    game::IEffectResultApi::Api::Create createEffectResult;

    game::CMenuPhaseApi::Api::SetTransition menuPhaseSetTransition;
    game::CMenuProtocolApi::Api::DisplayCallback menuProtocolDisplayCallback;
    game::CMenuProtocolApi::Api::ButtonHandler menuProtocolContinueHandler;

    game::CMenuBaseApi::Api::ButtonCallback menuNewSkirmishLoadScenario;
    game::CMenuNewSkirmishMultiApi::Api::CreateServer menuNewSkirmishMultiCreateServer;

    game::CMenuLoadApi::Api::ButtonCallback menuLoadSkirmishMultiLoadScenario;
    game::CMenuLoadApi::Api::CreateHostPlayer menuLoadSkirmishMultiCreateHostPlayer;

    game::GameImagesApi::Api::GetCityPreviewLargeImageNames getCityPreviewLargeImageNames;
    game::GameImagesApi::Api::GetCityIconImageNames getCityIconImageNames;

    game::CEncLayoutUnitApi::Api::Initialize encLayoutUnitInitialize;
    game::CEncLayoutUnitApi::Api::Update encLayoutUnitUpdate;

    game::CEncLayoutStackApi::Api::Update encLayoutStackUpdate;
};

OriginalFunctions& getOriginalFunctions();

} // namespace hooks

#endif // ORIGINALFUNCTIONS_H
