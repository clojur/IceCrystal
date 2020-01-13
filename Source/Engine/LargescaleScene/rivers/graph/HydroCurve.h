

#ifndef _LargescaleScene_HYDROCURVE_H_
#define _LargescaleScene_HYDROCURVE_H_

#include "LargescaleScene/graph/graph/Curve.h"

namespace LargescaleScene
{

/**
 * A Curve with additional, river specific data.
 * An HydroCurve can be of 2 different types:
 * - A River, which is the same as a Curve
 * - A Bank, which contains links to Rivers and potential values.
 * @ingroup rivergraph
 * @author Antoine Begault
 */
class LargescaleScene_API  HydroCurve : public Curve
{
public:
    /**
     * HydroCurve Type.
     */
    enum HYDRO_CURVE_TYPE
    {
        AXIS,
        CLOSING_SEGMENT,
        BANK
    };

    /**
     * Creates a new HydroCurve.
     *
     * @param owner the graph containing this curve.
     */
    HydroCurve(Graph *owner);

    /**
     * Creates a new HydroCurve, with parameters copied from another Curve.
     *
     * @param owner the graph containing this curve.
     * @param c the copied curve, from which this curve takes its data.
     * @param s the start node.
     * @param e the end node.
     */
    HydroCurve(Graph *owner, CurvePtr c, NodePtr s, NodePtr e);

    /**
     * Deletes this HydroCurve.
     */
    virtual ~HydroCurve();

    /**
     * Returns the width of this curve.
     */
    float getWidth() const;

    /**
     * Returns this Curve's potential.
     * Returns -1 if this Curve is a river axis.
     */
    float getPotential() const;

    /**
     * Sets this curve's potential.
     */
    void setPotential(float potential);

    /**
     * Returns the id of the river axis associated to this HydroCurve.
     * NULL_ID if this Curve is a river axis.
     */
    CurveId getRiver() const;

    /**
     * Returns the river axis associated to this HydroCurve.
     * NULL if this Curve is a river.
     */
    CurvePtr getRiverPtr() const;

    /**
     * Sets the river axis associated to this HydroCurve.
     *
     * @param river the Curve's Id.
     */
    void setRiver(CurveId river);

    /**
     * Display method. For debug only.
     */
    virtual void print() const;

protected:
    /**
     * River axis associated to this HydroCurve.
     */
    CurveId river;

    /**
     * Value used to determine the flow between two banks.
     */
    float potential;

    friend class HydroGraph;
};

}

#endif
