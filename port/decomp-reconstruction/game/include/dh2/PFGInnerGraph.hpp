// PFGInnerGraph: the inner path-finding graph of a floor, a GraphSparse of PFGInnerEdge. It adds no members
// of its own: both destructors reset the vtable pointer and destroy the GraphSparse base (the deleting variant
// also frees the object). No constructor address was found, so none is declared. The original size is the
// GraphSparse base size and is not recorded here.
// Reconstructed from libDungeonHunter2.so pseudocode (dtors 0x523480 and 0x5234b4, DBG_GetMemUsage 0x52ddd0).
#pragma once

#include "dh2/sfc/math/graph/GraphSparse.hpp"

class PFGInnerEdge;

class PFGInnerGraph : public sfc::math::graph::GraphSparse<PFGInnerEdge> {
public:
    ~PFGInnerGraph() override;      // slot 0: 0x523480 (D1), slot 1: 0x5234b4 (D0, frees)

    // Debug estimate of the graph's memory: 44 bytes per node plus 24 bytes per incoming edge of each node.
    int DBG_GetMemUsage() const;    // 0x52ddd0
};
