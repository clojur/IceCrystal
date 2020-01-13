

/*
 * Authors:jiangsi.
 */

#ifndef _LargescaleScene_COMPOSEDMARGIN_H_
#define _LargescaleScene_COMPOSEDMARGIN_H_

#include "LargescaleScene/common/LargescaleSceneMacro.h"
#include "LargescaleScene/graph/graph/Margin.h"

namespace LargescaleScene
{

/**
 * A Margin is used to determine what to clip in a graph.
 * It enables to take into account the width of a curve when clipping a box.
 * For example, when drawing a road that would go along a border, the curve would be in only one tile,
 * but considering the curve's width, the drawing would be cut on the tile border. That is why we need margins.
 * This class enables to check multiple Margins at the same time.
 * @ingroup graph
 * @author Antoine Begault
 */
class LargescaleScene_API ComposedMargin : public Margin
{
public:
    /**
     * Creates a new ComposedMargin.
     */
    ComposedMargin();

    /**
     * Deletes this ComposedMargin.
     */
    virtual ~ComposedMargin();

    /**
     * Returns the maximum margin from contained margins (max borderSize).
     *
     * @param clipSize size of the clipping box.
     */
    virtual double getMargin(double clipSize);

    /**
     * Returns the maximum margin from contained margins.
     *
     * @param clipSize size of the clipping box.
     * @param p the curve to get the margin from.
     */
    virtual double getMargin(double clipSize, CurvePtr p);

    /**
     * Returns the maximum margin from contained margins.
     *
     * @param clipSize size of the clipping box.
     * @param a the area to get the margin from.
     */
    virtual double getMargin(double clipSize, AreaPtr a);

    /**
     * Adds a margin.
     *
     * @param m a margin.
     */
    void addMargin(Margin *m);

    /**
     * Removes a margin.
     *
     * param m a margin.
     */
    void removeMargin(Margin *m);

private:
    /**
     * list of margins.
     */
    vector<Margin*> margins;
};

}

#endif
