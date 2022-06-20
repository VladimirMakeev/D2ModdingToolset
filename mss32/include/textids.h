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

#ifndef TEXTIDS_H
#define TEXTIDS_H

#include <string>

namespace hooks {

struct TextIds
{
    struct Interf
    {
        std::string sellAllValuables;
        std::string sellAllItems;
        std::string infiniteAttack;
        std::string infiniteText;
        std::string critHitAttack;
        std::string critHitDamage;
        std::string ratedDamage;
        std::string ratedDamageEqual;
        std::string ratedDamageSeparator;
        std::string splitDamage;
        std::string modifiedValue;
        std::string modifiersCaption;
        std::string modifiersEmpty;
        std::string modifierDescription;
        std::string nativeModifierDescription;
        std::string drainDescription;
        std::string drainEffect;
        std::string overflowAttack;
        std::string overflowText;
        std::string dynamicUpgradeLevel;
        std::string dynamicUpgradeValues;
    } interf;

    struct Events
    {
        struct Conditions
        {
            struct OwnResource
            {
                std::string tooMany;
                std::string mutuallyExclusive;
            } ownResource;

            struct GameMode
            {
                std::string tooMany;
                std::string single;
                std::string hotseat;
                std::string online;
            } gameMode;

            struct PlayerType
            {
                std::string tooMany;
                std::string human;
                std::string ai;
            } playerType;

            struct VariableCmp
            {
                std::string equal;
                std::string notEqual;
                std::string greater;
                std::string greaterEqual;
                std::string less;
                std::string lessEqual;
            } variableCmp;
        } conditions;
    } events;

    struct Lobby
    {
        std::string serverName;
        std::string serverNotResponding;
        std::string connectAttemptFailed;
        std::string serverIsFull;
        std::string computeHashFailed;
        std::string requestHashCheckFailed;
        std::string wrongHash;
        std::string wrongRoomPassword;
    } lobby;
};

const TextIds& textIds();

} // namespace hooks

#endif // TEXTIDS_H
