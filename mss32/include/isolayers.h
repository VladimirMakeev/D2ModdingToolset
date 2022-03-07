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

#ifndef ISOLAYER_H
#define ISOLAYER_H

#include <cstdint>

namespace game {

struct CIsoLayer
{
    std::uint32_t value;
};

/** Addresses of iso layer values read from IIsoLay.dbf. */
struct IsoLayers
{
    CIsoLayer* fog;              /**< FOG */
    CIsoLayer* fogBorder;        /**< FOG_BORDER */
    CIsoLayer* trees;            /**< TREES */
    CIsoLayer* mountains;        /**< MOUNTAINS */
    CIsoLayer* roads;            /**< ROADS */
    CIsoLayer* capitals;         /**< CAPITALS */
    CIsoLayer* villages;         /**< VILLAGES */
    CIsoLayer* riots;            /**< RIOTS */
    CIsoLayer* flags;            /**< FLAGS */
    CIsoLayer* shields;          /**< SHIELDS */
    CIsoLayer* stacks;           /**< STACKS */
    CIsoLayer* stacksShadow;     /**< STACKS_SHADOW */
    CIsoLayer* landmarks;        /**< LANDMARKS */
    CIsoLayer* lmarkGround;      /**< LMARK_GROUND */
    CIsoLayer* sites;            /**< SITES */
    CIsoLayer* bags;             /**< BAGS */
    CIsoLayer* tombs;            /**< TOMBS */
    CIsoLayer* ruins;            /**< RUINS */
    CIsoLayer* rods;             /**< RODS */
    CIsoLayer* crystals;         /**< CRYSTALS */
    CIsoLayer* locations;        /**< LOCATIONS */
    CIsoLayer* locationsTop;     /**< LOCATIONS_TOP */
    CIsoLayer* stackDeath;       /**< STACK_DEATH */
    CIsoLayer* stackCreate;      /**< STACK_CREATE */
    CIsoLayer* stackOwner;       /**< STACK_OWNER */
    CIsoLayer* spellsGround1;    /**< SPELLS_GROUND1 */
    CIsoLayer* spellsGround2;    /**< SPELLS_GROUND2 */
    CIsoLayer* spellsUnder1;     /**< SPELLS_UNDER1 */
    CIsoLayer* spellsUnder2;     /**< SPELLS_UNDER2 */
    CIsoLayer* spellsFront1;     /**< SPELLS_FRONT1 */
    CIsoLayer* spellsFront2;     /**< SPELLS_FRONT2 */
    CIsoLayer* spellsOver1;      /**< SPELLS_OVER1 */
    CIsoLayer* spellsOver2;      /**< SPELLS_OVER2 */
    CIsoLayer* spellsTop1;       /**< SPELLS_TOP1 */
    CIsoLayer* spellsTop2;       /**< SPELLS_TOP2 */
    CIsoLayer* mountainRemove;   /**< MOUNTAIN_REMOVE */
    CIsoLayer* symLocationSel;   /**< SYM_LOCATION_SEL */
    CIsoLayer* symStackSel;      /**< SYM_STACK_SEL */
    CIsoLayer* symStackBattle;   /**< SYM_STACK_BATTLE */
    CIsoLayer* symMovePath;      /**< SYM_MOVE_PATH */
    CIsoLayer* symObjective;     /**< SYM_OBJECTIVE */
    CIsoLayer* symShadow;        /**< SYM_SHADOW */
    CIsoLayer* symInvisible;     /**< SYM_INVISIBLE */
    CIsoLayer* symStackHighlite; /**< SYM_STACK_HIGHLITE */
    CIsoLayer* symSpellArea;     /**< SYM_SPELL_AREA */
    CIsoLayer* symSpellFog;      /**< SYM_SPELL_FOG */
    CIsoLayer* grid;             /**< GRID */
    CIsoLayer* smoothScrolling;  /**< SMOOTH_SCROLLING */
    CIsoLayer* tileHighlight;    /**< TILE_HIGHLIGHT */
    CIsoLayer* cloudImage;       /**< CLOUD_IMAGE */
    CIsoLayer* cloudShadow;      /**< CLOUD_SHADOW */
    CIsoLayer* banners;          /**< BANNERS */
};

IsoLayers& isoLayers();

} // namespace game

#endif // ISOLAYER_H
