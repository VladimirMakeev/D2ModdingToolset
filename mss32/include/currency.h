/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2020 Vladimir Makeev.
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

#ifndef CURRENCY_H
#define CURRENCY_H

#include <cstdint>

namespace game {

enum class CurrencyType : int
{
    Gold = 0,
    InfernalMana, /**< Mainly used by Legions of the Damned. */
    LifeMana,     /**< Mainly used by Empire. */
    DeathMana,    /**< Mainly used by Undead Hordes. */
    RunicMana,    /**< Mainly used by Mountain Clans. */
    GroveMana     /**< Mainly used by Elven Alliance. */
};

/** Represents prices as well as player resources. */
struct Bank
{
    std::int16_t infernalMana;
    std::int16_t lifeMana;
    std::int16_t deathMana;
    std::int16_t runicMana;
    std::int16_t groveMana;
    std::int16_t gold;
};

static inline bool operator==(const Bank& a, const Bank& b) noexcept
{
    // clang-format off
    return a.gold == b.gold
        && a.infernalMana == b.infernalMana
        && a.lifeMana == b.lifeMana
        && a.deathMana == b.deathMana
        && a.runicMana == b.runicMana
        && a.groveMana == b.groveMana;
    // clang-format on
}

static inline bool operator!=(const Bank& a, const Bank& b) noexcept
{
    return !(a == b);
}

/** Same as calling BankApi::get().less(a, b). */
static inline bool operator<(const Bank& a, const Bank& b) noexcept
{
    // clang-format off
    return a.infernalMana < b.infernalMana
        || a.lifeMana < b.lifeMana
        || a.deathMana < b.deathMana
        || a.runicMana < b.runicMana
        || a.gold < b.gold
        || a.groveMana < b.groveMana;
    // clang-format on
}

static inline bool operator<=(const Bank& a, const Bank& b) noexcept
{
    return (a < b) || (a == b);
}

static inline bool operator>(const Bank& a, const Bank& b) noexcept
{
    // clang-format off
    return a.infernalMana > b.infernalMana
        && a.lifeMana > b.lifeMana
        && a.deathMana > b.deathMana
        && a.runicMana > b.runicMana
        && a.gold > b.gold
        && a.groveMana > b.groveMana;
    // clang-format on
}

static inline bool operator>=(const Bank& a, const Bank& b) noexcept
{
    // clang-format off
    return a.infernalMana >= b.infernalMana
        && a.lifeMana >= b.lifeMana
        && a.deathMana >= b.deathMana
        && a.runicMana >= b.runicMana
        && a.gold >= b.gold
        && a.groveMana >= b.groveMana;
    // clang-format on
}

namespace BankApi {

struct Api
{
    /**
     * Performs pairwise addition of currencies of two banks.
     * Underflows and overflows of each currency are clamped in range [0 : 9999].
     */
    using Add = Bank*(__thiscall*)(Bank* bank, const Bank* other);
    Add add;

    /**
     * Performs pairwise subtraction of currencies, subtracting other from bank.
     * Underflows and overflows of each currency are clamped in range [0 : 9999].
     */
    using Subtract = Bank*(__thiscall*)(Bank* bank, const Bank* other);
    Subtract subtract;

    /**
     * Multiplies each resource in the bank by specified value.
     * Results are clamped in range [0 : 9999].
     */
    using Multiply = Bank*(__thiscall*)(Bank* bank, std::int16_t value);
    Multiply multiply;

    /**
     * Divides each resource in the bank by specified value.
     * Results are clamped in range [0 : 9999].
     */
    using Divide = Bank*(__thiscall*)(Bank* bank, std::int16_t value);
    Divide divide;

    /**
     * Returns true if bank is less than other in any currency.
     * Comparison sequence: infernal, life, death, runic, gold and the grove is the last one.
     */
    using Less = bool(__thiscall*)(const Bank* bank, const Bank* other);
    Less less;

    /** Copies currencies of other into this if they are not equal. */
    using Copy = Bank*(__thiscall*)(Bank* bank, const Bank* other);
    Copy copy;

    /** Sets all currencies to invalid value of 10000. */
    using SetInvalid = Bank*(__thiscall*)(Bank* bank);
    SetInvalid setInvalid;

    /** Sets all currencies to 0. */
    using SetZero = void(__thiscall*)(Bank* bank);
    SetZero setZero;

    /**
     * Sets bank currency to specified value.
     * Value are clamped in range [0 : 9999]. Nothing changes in case of incorrect currencyValue.
     * @returns value that was set.
     */
    using SetCurrency = std::int16_t(__thiscall*)(Bank* bank,
                                                  CurrencyType currencyType,
                                                  std::int16_t value);
    SetCurrency set;

    /** Returns true if all currencies in bank are zero. */
    using IsZero = bool(__thiscall*)(const Bank* bank);
    IsZero isZero;

    /** Returns true if all currency types are in [0 : 9999] range. */
    using IsValid = bool(__thiscall*)(const Bank* bank);
    IsValid isValid;

    /**
     * Converts bank currency values to string.
     * Same as @code{.cpp}
     * sprintf(string, "G%04d:R%04d:Y%04d:E%04d:W%04d:B%04d",
     *         gold, infernal, life, death, runic, grove);
     * @endcode
     */
    using ToString = char*(__thiscall*)(const Bank* bank, char* string);
    ToString toString;

    /** Initializes bank structure from string. */
    using FromString = Bank*(__stdcall*)(Bank* bank, const char* string);
    FromString fromString;
};

Api& get();

} // namespace BankApi

} // namespace game

#endif // CURRENCY_H
