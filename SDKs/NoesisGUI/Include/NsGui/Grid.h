////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRID_H__
#define __GUI_GRID_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Panel.h>
#include <NsGui/GridLength.h>

#include <NsCore/Vector.h>
#include <NsCore/Set.h>
#include <NsCore/HashMap.h>


namespace Noesis
{

class ColumnDefinition;
class RowDefinition;
template<class T> class TypedCollection;

typedef Noesis::TypedCollection<Noesis::ColumnDefinition> ColumnDefinitionCollection;
typedef Noesis::TypedCollection<Noesis::RowDefinition> RowDefinitionCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a flexible grid area that consists of columns and rows.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.grid.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Grid: public Panel
{
public:
    /// Constructor
    Grid();
    ~Grid();

    /// Gets column definitions collection
    ColumnDefinitionCollection* GetColumnDefinitions() const;

    /// Gets a value that indicates which column child within a Grid should appear in
    static uint32_t GetColumn(const DependencyObject* element);
    /// Sets a value that indicates which column child within a Grid should appear in
    static void SetColumn(DependencyObject* element, uint32_t column);

    /// Gets a value that indicates the number of columns that child spans within a grid
    static uint32_t GetColumnSpan(const DependencyObject* element);
    /// Sets a value that indicates the number of columns that child spans within a grid
    static void SetColumnSpan(DependencyObject* element, uint32_t columnSpan);

    /// Gets a value that indicates that multiple Grid elements are sharing size information
    static bool GetIsSharedSizeScope(const DependencyObject* element);
    /// Sets a value that indicates that multiple Grid elements are sharing size information
    static void SetIsSharedSizeScope(DependencyObject* element, bool value);

    /// Gets row definitions collection
    RowDefinitionCollection* GetRowDefinitions() const;

    /// Gets a value that represents the distance between the top of an element and the top
    /// of its parent Canvas
    static uint32_t GetRow(const DependencyObject* element);
    /// Sets a value that represents the distance between the top of an element and the top
    /// of its parent Canvas
    static void SetRow(DependencyObject* element, uint32_t row);

    /// Gets a value that represents the distance between the top of an element and the top
    /// of its parent Canvas
    static uint32_t GetRowSpan(const DependencyObject* element);
    /// Sets a value that represents the distance between the top of an element and the top
    /// of its parent Canvas
    static void SetRowSpan(DependencyObject* element, uint32_t rowSpan);

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ColumnProperty; // attached property
    static const DependencyProperty* ColumnSpanProperty; // attached property
    static const DependencyProperty* IsSharedSizeScopeProperty; // attached property
    static const DependencyProperty* RowProperty; // attached property
    static const DependencyProperty* RowSpanProperty; // attached property
    //@}

protected:
    /// From DependencyObject
    //@{
    void OnInit();
    //@}

    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const;
    Visual* GetVisualChild(uint32_t index) const;
    void OnVisualChildrenChanged(Visual* added, Visual* removed);
    //@}

    /// From FrameworkElement
    //@{
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const;
    uint32_t GetLogicalChildrenCount() const;
    BaseComponent* GetLogicalChild(uint32_t index) const;
    Size MeasureOverride(const Size& availableSize);
    Size ArrangeOverride(const Size& finalSize);
    //@}

private:
    // Simple measure of children
    Size SimpleMeasure(const Size& availableSize);

    // Simple arrange of children
    Size SimpleArrange(const Size& finalSize);

    // Updates private cell structures
    void UpdateCellStructure();
    void UpdateElementCache();

    // Gets element column and row info
    //@{
    void GetColumnInfo(const UIElement* child, uint32_t numCols, uint32_t& col, uint32_t& colSpan) const;
    void GetRowInfo(const UIElement* child, uint32_t numRows, uint32_t& row, uint32_t& rowSpan) const;
    //@}

    struct DefinitionsInfo;
    struct DefinitionCache
    {
        GridUnitType type;
        float size;
        float minSize;
        float maxSize;

        DefinitionCache();
        DefinitionCache(GridUnitType t, float s, float mins, float maxs);
    };

    typedef NsVector<DefinitionCache> DefinitionCacheVector;
    typedef NsVector<float> FloatVector;

    // Measure helpers
    //@{
    Size DoMeasure(const Size& availableSize);
    void MeasureElements(uint32_t start, uint32_t end);
    float MeasureSizes(const DefinitionCacheVector& defVec, FloatVector& sizes);
    void UpdateSizes(const DefinitionCacheVector& defVec, uint32_t index, uint32_t span,
        bool allAutoSpan, float size, float starTotal, FloatVector& sizes);
    void SolveDef(float availableSize, const DefinitionsInfo& info, const FloatVector& sizes,
        FloatVector& constraints, float& usedSize);
    //@}

    // Arrange helpers
    //@{
    void DoArrange(const Size& finalSize);
    void ArrangeDef(float finalSize, float desiredSize, bool isDesiredSize,
        const DefinitionsInfo& info, const FloatVector& measureSizes,
        FloatVector& arrangeSizes, FloatVector& arrangePositions);
    void UpdateArrangeSize(uint32_t index, FloatVector& arrangeSizes, float size,
        float& usedSize, float star, float& usedStar);
    //@}

    // Determines if grid size is determined by children
    void IsDesiredSize(const Size& size, bool& isDesiredWidth, bool& isDesiredHeight);
    bool IsDesiredSize(float finalSize, float desiredSize, float localSize) const;

    // Distributes desired size within the cell span
    void DistributeSize(uint32_t index, uint32_t span, bool allAuto, float size,
        float starTotal, const DefinitionCacheVector& defVec, FloatVector& vec) const;

    // Calculates the size of a range of cells
    float GetSpanSize(const FloatVector& vec, uint32_t index, uint32_t span) const;

    // Indicates if all cells in the span are auto
    void GetAllAutoSpanAndOrder(const DefinitionCacheVector& defVec, uint32_t index, uint32_t span,
        bool& allAutoSpan, uint32_t& orderType) const;

    // Notifies when a collection of column or row definitions changed
    void OnCollectionChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);

    // Notifies when the definition of a column or row changes
    void OnDefinitionChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);

    // Invalidates grid cell cache
    void InvalidateCellCache();

    // Creates column/row definitions if necessary
    void EnsureColumns() const;
    void ConnectColumns() const;
    void DisconnectColumns();
    void EnsureRows() const;
    void ConnectRows() const;
    void DisconnectRows();

    // Detects changes in cell properties of children elements
    static void StaticCellChanged(DependencyObject* obj,
        const DependencyPropertyChangedEventArgs& args);

private:
    mutable Ptr<ColumnDefinitionCollection> mColumnDefinitions;
    mutable Ptr<RowDefinitionCollection> mRowDefinitions;

    bool mDirtyCellStructure;
    bool mDirtyElementCache;

    struct ElementInfo
    {
        UIElement* element;
        uint32_t col;
        uint32_t colSpan;
        uint32_t row;
        uint32_t rowSpan;
        uint32_t order;
        bool allAutoRowSpan;
        bool allAutoColSpan;
    };

    typedef NsVector<ElementInfo> ElementInfoVector;
    typedef NsVector<uint32_t> IndexVector;

    struct OrderedSize
    {
        float size;
        uint32_t index;

        OrderedSize(float s = 0.0f, uint32_t i = 0);
        bool operator<(const OrderedSize& other) const;
    };

    typedef NsMultiset<OrderedSize> OrderedSizes;

    struct DefinitionsInfo
    {
        DefinitionCacheVector definitions;
        IndexVector pixelDefinitions;
        IndexVector autoDefinitions;
        IndexVector starDefinitions;
        float starTotal;

        DefinitionsInfo();
        void Reset();
        void Reserve(uint32_t count);
        void AddDefinition(const DefinitionCache& def);
    };

    struct MeasureInfo
    {
        // Contains the constraints applied to each cell element
        FloatVector colConstraints;
        FloatVector rowConstraints;

        // Stores the desired size of each cell
        FloatVector colSizes;
        FloatVector rowSizes;

        // During measure process stores the used size by Pixel and Auto cells, to calculate the
        // remaining space for Star cells. After measure represents the desired size of the grid
        float usedWidth;
        float usedHeight;
    };

    struct ArrangeInfo
    {
        FloatVector colSizes;
        FloatVector rowSizes;
        FloatVector colPositions;
        FloatVector rowPositions;
    };

    typedef NsHashMap<uint32_t, IndexVector> OrderedIndices;

    struct CellStructureInfo: public BaseComponent
    {
        DefinitionsInfo colInfo;
        DefinitionsInfo rowInfo;
        ElementInfoVector elements;
        OrderedIndices orderedIndices;
        IndexVector orderedElements;
        OrderedSizes orderedSizes;
        uint32_t solveColsIndex;
        uint32_t solveRowsIndex;
        MeasureInfo measureInfo;
        ArrangeInfo arrangeInfo;

        void ResetCellInfo();
        void ResetElementCache();
        void ResetMeasureInfo();
        void ResetArrangeInfo();

        uint32_t GetNumOrderedIndices(uint32_t cell);
        void AddOrderedElements(uint32_t cell);

    private:
        void ResetMeasureDef(const DefinitionsInfo& info, FloatVector& constraints,
            FloatVector& sizes, float& usedSize);
        void ResetArrangeDef(const DefinitionsInfo& info, FloatVector& sizes,
            FloatVector& positions);
    };

    Ptr<CellStructureInfo> mCellInfo;

    NS_DECLARE_REFLECTION(Grid, Panel)
};

NS_WARNING_POP

}

#endif
