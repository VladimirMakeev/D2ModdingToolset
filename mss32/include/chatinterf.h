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

#ifndef CHATINTERF_H
#define CHATINTERF_H

#include "imagepointlist.h"
#include "lovechat.h"
#include "midpopupinterf.h"
#include "notifyplayerlist.h"
#include "playerlistentry.h"
#include "textmessage.h"

namespace game {

struct CChatInterfData
{
    Vector<TextMessage> chatMessages;
    Vector<PlayerListEntry> playerListEntries;
    int unknown9;
};

assert_size(CChatInterfData, 36);

/** Represents DLG_CHAT from Interf.dlg. */
struct CChatInterf
    : public ILoveChat
    , public INotifyPlayerList
    , public CMidPopupInterf
{
    CChatInterfData* chatData;
};

assert_size(CChatInterf, 44);

namespace CChatInterfApi {

struct Api
{
    using ListBoxDisplayHandler = void(__thiscall*)(CChatInterf* thisptr,
                                                    ImagePointList* contents,
                                                    const CMqRect* lineArea,
                                                    int index,
                                                    bool selected);
    ListBoxDisplayHandler listBoxDisplayHandler;
};

Api& get();

} // namespace CChatInterfApi

} // namespace game

#endif // CHATINTERF_H
