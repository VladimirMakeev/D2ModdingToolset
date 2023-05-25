/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2023 Vladimir Makeev.
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

#ifndef WAVSTORE_H
#define WAVSTORE_H

#include "d2map.h"
#include "d2set.h"
#include "smartptr.h"

namespace game {

struct CSoundSystemSample;
struct Wdb;

/** Sound effects from midgard.wdb. */
enum class SoundEffect : int
{
    Appear,       /**< APPEAR */
    Boat,         /**< BOATSND */
    EnterSite,    /**< ENTRSITE */
    EnterRuin,    /**< ENTRRUIN */
    EnterCity,    /**< ENTRCITY */
    ExitCity,     /**< EXITCITY */
    Occupy,       /**< OCCUPY */
    SpellDis,     /**< SPELLDIS */
    LootRuin,     /**< LOOTRUIN */
    Enroll,       /**< ENROLL */
    Occupy2,      /**< OCCUPY */
    Beep,         /**< BEEP */
    Botreprt,     /**< BOTREPRT */
    Seebat,       /**< SEEBAT */
    Sbattle,      /**< SBATTLE */
    Snoble,       /**< SNOBLE */
    Stolen,       /**< STOLEN */
    Building,     /**< BUILDING */
    Openbook,     /**< OPENBOOK */
    Closbook,     /**< CLOSBOOK */
    Bkpopup,      /**< BKPOPUP */
    Openintr,     /**< OPENINTR */
    Closintr,     /**< CLOSINTR */
    Potionboost,  /**< PBOOST */
    Potionheal,   /**< PHEAL */
    Potionrevive, /**< PREVIVE */
    UseItem,      /**< USEITEM */
    BuyItem,      /**< BUYITEM */
    CityGrow,     /**< CITYGROW */
    TakeBag,      /**< TAKEBAG */
    SpinRock,     /**< SPINROCK */
    ChangeFace,   /**< CHNGFACE */
    SoundFx,      /**< SOUNDFX */
    GiveGold,     /**< GIVEGOLD */
    TradeSpell,   /**< TRADSPEL */
    Reftrspe,     /**< REFTRSPE */
    TradeItem,    /**< TRADITEM */
    Reftritm,     /**< REFTRITM */
    Alliance,     /**< ALLIANCE */
    Refallia,     /**< REFALLIA */
    Brkallia,     /**< BRKALLIA */
    AiChat,       /**< AICHAT */
    Aunn7778,     /**< AUNN7778 */
    Aunn7788,     /**< AUNN7788 */
    EndRiot,      /**< ENDRIOT */
    CreateStack,  /**< CREATSTK */
};

struct SoundEffectInfo
{
    void* data;              /** < Mqrc data from .wdb file. */
    std::uint32_t size;      /**< Data size in bytes. */
    std::uint32_t startTime; /**< Time when effect started playing. */
};

assert_size(SoundEffectInfo, 12);

struct CWavStoreData
{
    SmartPtr<CSoundSystemSample> soundSystemSample;
    SmartPointer memPool;
    SmartPtr<Wdb> wdb;
    Map<char[16] /* name */, SoundEffectInfo> effectCache;
    Set<int> list2;
};

assert_size(CWavStoreData, 80);

struct CWavStore
{
    CWavStoreData* data;
};

assert_size(CWavStore, 4);

} // namespace game

#endif // WAVSTORE_H
