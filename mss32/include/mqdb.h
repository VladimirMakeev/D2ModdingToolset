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

#ifndef MQDB_H
#define MQDB_H

#include "d2list.h"
#include "d2map.h"
#include "d2pair.h"
#include "d2string.h"
#include "d2unorderedmap.h"
#include "d2vector.h"
#include "mq_c_s.h"

namespace game {

struct MQDBPackedImage;

/** Table of contents (ToC) record inside of .ff file. */
struct MQDBTocRecord
{
    /** Unique record id */
    std::uint32_t recordId;
    /** Size of contents in this record, in bytes */
    std::uint32_t size;
    /** Total record size, in bytes */
    std::uint32_t allocatedSize;
    /** Offset from the start of the file to associated MQRC record */
    std::uint32_t recordContentsOffset;
};

assert_size(MQDBTocRecord, 16);

/** Implementation of std::unordered_map<std::uint32_t, MQDBTocRecord> used in game */
using MQDBRecordMap = UnorderedMap<std::uint32_t /* record id */,
                                   MQDBTocRecord,
                                   Pair<Pair<char, char[3]>, int>>;
assert_size(MQDBRecordMap, 44);

struct MQDBFile
{
    String filePath;
    String error;
    void* fileHandle; /**< Actual type is HANDLE */
    int unk8;
    int fileOffset;
    int freeId;
    MQDBRecordMap tocRecords; /**< Table of Contents records */
    MQDBRecordMap notUnused;
};

assert_size(MQDBFile, 136);

struct quick_string
{
    char string[257]; /**< 256 characters + null terminator */
    char padding[3];
};

using mq_o_c_s = mq_c_s<Pair<quick_string /* record name */, std::uint32_t /* record id */>>;

assert_size(mq_o_c_s, 20);

struct MQDBData
{
    MQDBFile** mqdb;
    mq_o_c_s* stringPairs; /**< For fast ToC record access by its name */
};

assert_size(MQDBData, 8);

struct MQDBDataWrapper
{
    MQDBData* data;
};

assert_size(MQDBDataWrapper, 4);

using IndexOptOffsetSizePair = Pair<std::uint32_t /* offset */, std::uint32_t /* size */>;

using IndexOptRecordIdFlippedPair = Pair<std::uint32_t /* record id */, bool /* flipped */>;

struct MQDBImageData
{
    MQDBDataWrapper* imagesData;
    /** Actual type is HANDLE */
    void* imagesOptHandle;
    /** Offset to '-images.opt' record data from the beginning of file. -1 if there is no record. */
    int imagesOptDataOffset;
    /** Actual type is HANDLE */
    void* animsOptHandle;
    /** Offset to '-anims.opt' record data from the beginning of file. -1 if there is no record. */
    int animsOptDataOffset;

    // Next 3 vectors have the same length and should be accessed using same index,
    // they describe images
    /** Valid record ids from '-index.opt' contents */
    Vector<std::uint32_t> indexOptRecordIds;
    /** Image names corresponding to valid record ids from '-index.opt' contents */
    Vector<String> indexOptStrings;
    Vector<IndexOptOffsetSizePair> indexOptPairs;

    // Next 2 vectors have the same length and should be accessed using same index,
    // they describe animation frames
    // Animation frames are marked with invalid record id (-1) inside of '-index.opt' contents.
    Vector<String> indexOptStringsNoIdx;
    Vector<IndexOptOffsetSizePair> indexOptPairsNoIdx;

    Map<IndexOptRecordIdFlippedPair, SmartPtr<MQDBPackedImage>> loadedImages;
    List<Pair<int /* indexOptRecordId */, int /* flipped */>> list;
    char unknown5[72];
};

assert_size(MQDBImageData, 216);

struct MQDBImageDataWrapper
{
    MQDBImageData* data;
};

assert_size(MQDBImageDataWrapper, 4);

} // namespace game

#endif // MQDB_H
