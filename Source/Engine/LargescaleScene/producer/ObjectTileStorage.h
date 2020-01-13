/*
 * LargescaleScene: a procedural landscape rendering library.
 * Copyright (c) 2008-2011 INRIA
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

/*
 * LargescaleScene is distributed under a dual-license scheme.
 * You can obtain a specific license from Inria: LargescaleScene-licensing@inria.fr.
 */

/*
 * Authors:jiangsi.
 */

#ifndef _LargescaleScene_OBJECT_TILE_STORAGE_H_
#define _LargescaleScene_OBJECT_TILE_STORAGE_H_

#include "LargescaleScene/producer/TileStorage.h"

namespace LargescaleScene
{

/**
 * A TileStorage that stores Object on CPU.
 * @ingroup producer
 * @authors Eric Bruneton, Antoine Begault
 */
class LargescaleScene_API  ObjectTileStorage : public TileStorage
{
public:
    /**
     * A tile managed by an ObjectTileStorage.
     */
    class ObjectSlot : public Slot
    {
    public:
        /**
         * The tile data.
         */
        ptr<Object> data;

        /**
         * Creates a new ObjectSlot. This constructor does NOT allocate any
         * object.
         *
         * @param owner the TileStorage that manages this tile.
         */
        ObjectSlot(TileStorage *owner);

        /**
         * Deletes this ObjectSlot. This deletes the reference to #data.
         */
        virtual ~ObjectSlot();
    };

    /**
     * Creates a new ObjectTileStorage.
     *
     * @param capacity the number of tiles managed by this tile storage.
     */
    ObjectTileStorage(int capacity);

    /**
     * Deletes this ObjectTileStorage.
     */
    virtual ~ObjectTileStorage();

protected:
    /**
     * Creates an uninitialized ObjectTileStorage.
     */
    ObjectTileStorage();

    /**
     * Initializes this ObjectTileStorage.
     *
     * @param capacity the number of tiles managed by this tile storage.
     */
    void init(int capacity);

    void swap(ptr<ObjectTileStorage> t);
};

}

#endif
