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

#ifndef GENERATIONRESULTINTERF_H
#define GENERATIONRESULTINTERF_H

#include "popupdialoginterf.h"

namespace hooks {

struct CMenuRandomScenario;

using OnGenerationResultAccepted = void (*)(CMenuRandomScenario* thisptr);
using OnGenerationResultRejected = void (*)(CMenuRandomScenario* thisptr);
using OnGenerationResultCanceled = void (*)(CMenuRandomScenario* thisptr);

struct CGenerationResultInterf : public game::CPopupDialogInterf
{
    OnGenerationResultAccepted onAccept;
    OnGenerationResultRejected onReject;
    OnGenerationResultCanceled onCancel;
    CMenuRandomScenario* menu;
};

CGenerationResultInterf* createGenerationResultInterf(CMenuRandomScenario* menu,
                                                      OnGenerationResultAccepted onAccept,
                                                      OnGenerationResultRejected onReject,
                                                      OnGenerationResultCanceled onCancel);

} // namespace hooks

#endif // GENERATIONRESULTINTERF_H
