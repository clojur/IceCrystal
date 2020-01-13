

#ifndef _LargescaleScene_EDIT_ORTHO_CPU_PRODUCER_H_
#define _LargescaleScene_EDIT_ORTHO_CPU_PRODUCER_H_

#include "LargescaleScene/producer/OrthoCPUProducer.h"

namespace LargescaleScene
{

/**
 * An OrthoCPUProducer whose tiles can be edited at runtime.
 * Intended to be used with an EditOrthoProducer.
 * @ingroup edit
 * @authors Eric Bruneton, Antoine Begault
 */
class LargescaleScene_API EditOrthoCPUProducer : public OrthoCPUProducer
{
public:
    /**
     * Creates a new EditOrthoCPUProducer.
     * See #OrthoCPUProducer.
     */
    EditOrthoCPUProducer(ptr<TileCache> cache, const char *name);

    /**
     * Deletes this EditOrthoCPUProducer.
     */
    virtual ~EditOrthoCPUProducer();

    /**
     * Notifies this producer that a tile has been edited.
     *
     * @param level the level of the edited tile.
     * @param tx the logical x coordinate of the edited tile.
     * @param ty the logical y coordinate of the edited tile.
     * @param deltaColor the difference between the new and the previous
     *      color values of the edited tile. Does not include borders.
     */
    void editedTile(int level, int tx, int ty, int *deltaColor);

    /**
     * Updates the residual tiles produced by this producer to take into
     * account all the edited tiles since the last call to this method.
     */
    void updateTiles();

    virtual bool hasTile(int level, int tx, int ty);

    /**
     * Cancels all editing operations performed on this producer.
     */
    void reset();

protected:
    /**
     * Creates an uninitialized EditOrthoCPUProducer.
     */
    EditOrthoCPUProducer();

    /**
     * Initializes this EditOrthoCPUProducer.
     * See #EditOrthoCPUProducer.
     */
    virtual void init(ptr<TileCache> cache, const char *name);

    virtual bool doCreateTile(int level, int tx, int ty, TileStorage::Slot *data);

    virtual void swap(ptr<EditOrthoCPUProducer> p);

private:
    /**
     * True if there is no file associated with this producer.
     */
    bool empty;

    /**
     * The size of the color residual tiles, including borders.
     */
    int tWidth;

    /**
     * The size of the color residual tiles, without borders.
     */
    int tSize;

    /**
     * The number of channels of the color residual tiles.
     */
    int tChannels;

    /**
     * The id of the last color delta tile that was used in
     * #getDeltaColor. Optimization to avoid too many lookups in
     * the #deltaColors map.
     */
    TileCache::Tile::Id curId;

    /**
     * The last color delta tile that was used in #getDeltaColor.
     * Optimization to avoid too many lookups in the #deltaColors map.
     */
    int *curDeltaColor;

    /**
     * The color residual tiles that have been modified.
     */
    std::map<TileCache::Tile::Id, unsigned char*> modifiedTiles;

    /**
     * The color deltas from which to recompute the color residual tiles.
     */
    std::map<TileCache::Tile::Id, int*> deltaColors;

    /**
     * Returns the color delta at the given location.
     *
     * @param level a quadtree level.
     * @param n the numbers of tiles per row or column in the quadtree at
     *      this level (1 << level).
     * @param tx a logical tile x coordinate.
     * @param ty a logical tile y coordinate.
     * @param x a pixel coordinate relatively to the lower left corner of
     *      the (level,tx,ty) tile (without borders). Can be outside the
     *      tile itself (then the value will be looked up in an adjacent tile).
     * @param y a pixel coordinate relatively to the lower left corner of
     *      the (level,tx,ty) tile (without borders). Can be outside the
     *      tile itself (then the value will be looked up in an adjacent tile).
     */
    int *getDeltaColor(int level, int n, int tx, int ty, int x, int y);
};

}

#endif
