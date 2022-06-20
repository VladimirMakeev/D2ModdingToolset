/*
 * This file is part of the modding toolset for Disciples 2.
 * (https://github.com/VladimirMakeev/D2ModdingToolset)
 * Copyright (C) 2022 Vladimir Makeev.
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

#ifndef MIDSTREAMFILE_H
#define MIDSTREAMFILE_H

#include "midgardstream.h"

namespace game {

struct CMidStreamEnvFile;

/** Implementation of IMidgardStream that works with files. */
struct CMidStreamFile : public IMidgardStream
{
    CMidStreamEnvFile* streamEnv;
    bool insideRecord;
    char padding[3];
};

assert_size(CMidStreamFile, 12);

} // namespace game

#endif // MIDSTREAMFILE_H
