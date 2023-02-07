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

#ifndef IMGFACTORY_H
#define IMGFACTORY_H

#include "d2string.h"
#include "d2vector.h"
#include "mqimage2.h"
#include "smartptr.h"

namespace game {

struct CImgFactory
{
    struct ImageVftable;

    struct Image
    {
        ImageVftable* vftable;
    };

    struct IdManagerVftable;

    struct IdManager
    {
        IdManagerVftable* vftable;
        int value;
    };

    struct FactoryLoaderVftable;

    struct FactoryLoader
    {
        FactoryLoaderVftable* vftable;
    };
};

using IdManagerSmartPtr = SmartPtr<CImgFactory::IdManager>;

struct CImgFactory::ImageVftable : public IMqImage2Vftable
{
    using GetImage = IMqImage2*(__thiscall*)(const Image* thisptr, std::uint32_t imageIndex);
    GetImage getImage;

    using IsUnknownReset = bool(__thiscall*)(const Image* thisptr);
    IsUnknownReset isUnknownReset;
};

assert_vftable_size(CImgFactory::ImageVftable, 11);

struct CImgFactory::IdManagerVftable
{
    using Destructor = void(__thiscall*)(CImgFactory::IdManager* thisptr, char flags);
    Destructor destructor;

    /** Returns sum of IdManager::value and a2. */
    using Method1 = int(__thiscall*)(CImgFactory::IdManager* thisptr, int a2);
    Method1 method1;

    /**
     * Returns difference between a2 and IdManager::value if a2 is greater.
     * Otherwise, returns a2 as is.
     */
    using Method2 = int(__thiscall*)(CImgFactory::IdManager* thisptr, int a2);
    Method2 method2;
};

assert_vftable_size(CImgFactory::IdManagerVftable, 3);

struct CImgFactory::FactoryLoaderVftable
{
    using Destructor = void(__thiscall*)(CImgFactory::FactoryLoader* thisptr, char flags);
    Destructor destructor;

    using LoadImageNames = void(__thiscall*)(CImgFactory::FactoryLoader* thisptr,
                                             const char* imageName,
                                             Vector<String>* imageNames);
    LoadImageNames loadImageNames;
};

assert_vftable_size(CImgFactory::FactoryLoaderVftable, 2);

struct CImgFactoryAnimLoaderWeakDup : public CImgFactory::FactoryLoader
{
    CImgFactory::FactoryLoader* factoryLoader;
};

assert_size(CImgFactoryAnimLoaderWeakDup, 8);

} // namespace game

#endif // IMGFACTORY_H
