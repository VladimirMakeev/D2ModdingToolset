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

#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include "d2string.h"

namespace game {

/** Holds settings read from 'Disciple.ini' file. */
struct GameSettings
{
    bool settingsCorrect;
    char padding[3];

    String gameFolder;

    String interfFolder;
    String imagesFolder;
    String imageDataFolder;

    String scensFolder;
    String globalsFolder;

    String videoFolder;

    String soundsFolder;
    String musicFolder;
    String customMusicFolder;
    String soundsPFolder;
    String musicPFolder;

    String campaignFolder;
    String exportsFolder;
    String saveGameFolder;

    String briefingFolder;
    String scenDataFolder;

    String globalsFolder2;
    String musicFolder2;
    String imageDataFolder2;
    String briefingFolder2;

    bool fullscreenDisplayMode;
    bool random;
    bool displayErrors;
    bool showIntro;
    bool soundFx;
    bool musicOn;
    char padding2[2];

    int fxVolume;
    int musicVolume;

    int acknoledgements;

    int playerSpeed;
    int opponentSpeed;
    int scrollSpeed;
    int battleSpeed;

    bool battleAnimations;
    bool confirmEndOfTurn;
    bool autoSave;
    bool smoothScroll;
    bool playIsoAnim;
    bool stackSmoothScroll;
    bool includeCampaign;
    bool scenEditDatabase;
    bool animatedIso;
    bool lowEndVideoCard;
    bool highAlpha;
    char padding3;

    int audioRegions;

    bool pauseAudioRegionsForAi;
    bool isoHighlight;
    bool defaultPathBattle;
    char padding4;

    int refreshRate;

    bool isoBirds;
    bool inversedPalMap;
    bool displayPathTurn;
    bool queueCommands;
    bool useD3d;
    bool forceD3dPow2;
    char padding5[2];

    int displayWidth;
    int displayHeight;

    bool scrollInterfBorder;
    bool showInterfBorder;
    bool enableZoom;
    bool snapCursor;

    int bonusPoolBlocks;

    bool stickyEncy;
    char padding6[3];

    int eventPopupDelay;
    int secondarySize;
    int mixAhead;
    int fragmentSize;

    bool flippedBattle;
    bool showTurnSummary;
    char unknown;
    char volumeLetter[2];
    char padding7[3];

    String defaultPlayerName;
};

assert_size(GameSettings, 468);

} // namespace game

#endif // GAMESETTINGS_H
