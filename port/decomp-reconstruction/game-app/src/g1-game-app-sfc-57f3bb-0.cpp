// Batch g1-game-app-sfc-57f3bb-0 (owner sfc, part 1 of 2). Rebuilt from the Hex-Rays pseudocode and checked against
// the assembly (tools/decomp_asm.py).
//
// Most rows of this batch are already in the sfc graph headers as inline template members, each with its address
// comment: Node.hpp (Node<unsigned int>), Edge.hpp (Edge<PFGInnerNode, float>) and GraphSparse.hpp (_InNode). Their
// ledger rows point at those headers. The bodies below are the ones the headers only declare:
// GraphSparse<PFGInnerEdge>::addNode and addEdge. GraphSparse<PFGOuterEdge>::clear is partial: the walk is fully
// checked against the asm (see the note at the end of this file) but the outer edge and node types are not in the
// tree, so the delete expressions cannot be written.
#include "dh2/sfc/math/graph/GraphSparse.hpp"

#include "dh2/PFGInnerEdge.hpp"
#include "dh2/PFGInnerNode.hpp"
#include "dh2/global/Vec3f_Origin.hpp"

namespace sfc {
namespace math {
namespace graph {

// Inline in game/include/dh2/sfc/math/graph/Node.hpp (rows 0x51bc40, 0x51bc58, 0x51c0fc):
//   Node<unsigned int>::~Node() (D1) is empty, getID() returns m_id (+0x04), isValid() returns true.
// Inline in game/include/dh2/sfc/math/graph/Edge.hpp (rows 0x51bc44 to 0x51bc70, 0x51c104, 0x51c10c):
//   Edge<PFGInnerNode, float>::~Edge() (D1) is empty, getFromNode() and getToNode() return m_from (+0x04) and
//   m_to (+0x08) in both const and non-const forms, setWeight() stores m_weight (+0x0c), getWeight() returns it,
//   isValid() returns true.
// Inline in game/include/dh2/sfc/math/graph/GraphSparse.hpp (rows 0x51c520, 0x523100 and 0x5231d4):
//   GraphSparse<PFGInnerEdge>::_InNode::~_InNode() (D1) is the defaulted destructor, which erases the edge map
//   (+0x2c) and keeps the edges, as the original does. GraphSparse<PFGOuterEdge>::~GraphSparse() (D1) is the inline
//   destructor { clear(); }, followed by the destruction of the node map (+0x04).

// 0x51dca4 sfc::math::graph::GraphSparse<PFGInnerEdge>::addNode(unsigned int)
// Returns the node with this id. When there is none, a node is created and inserted in the map. The original searches
// the map twice (a lookup, then the insert position), which here is one find() and one emplace(). The new node is
// set as the original does: the id (+0x04), position (+0x08) and direction (+0x14) copied from the Vec3f_Origin
// global, width (+0x20) and length (+0x24) zero, and no floor (+0x28). The edge map (+0x2c) starts empty.
template <>
GraphSparse<PFGInnerEdge>::_InNode* GraphSparse<PFGInnerEdge>::addNode(unsigned int id) {
    const auto found = m_nodes.find(id);
    if (found != m_nodes.end()) {
        return found->second;
    }
    auto* node = new _InNode();
    node->m_id = id;                   // +0x04
    node->m_position = Vec3f_Origin;   // +0x08
    node->m_direction = Vec3f_Origin;  // +0x14
    node->m_width = 0.0f;              // +0x20
    node->m_length = 0.0f;             // +0x24
    node->m_floor = nullptr;           // +0x28
    m_nodes.emplace(id, node);
    return node;
}

// 0x51e758 sfc::math::graph::GraphSparse<PFGInnerEdge>::addEdge(unsigned int, unsigned int)
// Returns the edge from node "from" to node "to", creating it when it is missing. Returns nullptr when either id has
// no node. The original looks up the to node's id through its virtual getID() (slot 0) and searches the from node's
// edge map by that id. An existing edge is returned. Otherwise a new PFGInnerEdge (operator new(0x18), no
// constructor) is set as the original does: from (+0x04) and to (+0x08) are the two node pointers, the weight
// (+0x0c) is 1.0f, and m_distance (+0x10) and m_clearance (+0x14) are zero. The edge is then inserted keyed by the
// to id. The original reads getID() a second time for that insert. getID has no side effects and returns the same
// value, so one read is used here.
template <>
PFGInnerEdge* GraphSparse<PFGInnerEdge>::addEdge(unsigned int from, unsigned int to) {
    const auto fromEntry = m_nodes.find(from);
    const auto toEntry = m_nodes.find(to);
    if (fromEntry == m_nodes.end() || toEntry == m_nodes.end()) {
        return nullptr;
    }
    _InNode* fromNode = fromEntry->second;
    _InNode* toNode = toEntry->second;
    const unsigned int toId = toNode->getID();
    auto& edges = fromNode->m_edges;
    const auto existing = edges.find(toId);
    if (existing != edges.end()) {
        return existing->second;
    }
    auto* edge = new PFGInnerEdge();
    edge->m_from = fromNode;    // +0x04
    edge->m_to = toNode;        // +0x08
    edge->m_weight = 1.0f;      // +0x0c (0x3f800000 in the original)
    edge->m_distance = 0.0f;    // +0x10
    edge->m_clearance = 0.0f;   // +0x14
    edges.emplace(toId, edge);
    return edge;
}

// 0x522f70 sfc::math::graph::GraphSparse<PFGOuterEdge>::clear()
// The body is the same two-level walk as the PFGInnerEdge instantiation (0x52328c, defined in
// g1-game-app-sfc-57f3bb-1.cpp): for every node in key order, delete each edge of the node's edge map through its
// virtual deleting destructor, empty that map, delete the node through its own deleting destructor, then empty the
// node map. The asm (0x522f70, 100 instructions) confirms every step and the orders:
//   522F8C  R6 = node (outer map node's value at +0x14)      522F90/522F94  edge map root / header at node+8
//   522FA0  edge = node+0x14 of the map node  522FB8  LDR PC,[R3,#0x20] = the edge's deleting destructor (slot 8)
//   52309C  _Rb_tree<...,PFGOuterEdge*>::_M_erase(node+8, node+0x0c) then the +0x14/+0x10/+0x0c/+0x18 header reset
//   5230BC  LDR PC,[R3,#0x0c] = the node's deleting destructor (slot 3) on the _InNode object
//   523044  _Rb_tree<...,_InNode*>::_M_erase + the a1+0x14/+0x10/+0x0c/+0x08 header reset (m_nodes)
// The binary's destructor slots are the ones a C++ `delete` emits: _ZTV13PFGOuterGraph (0x96C7B8) holds the D1
// 0x523164 and the D0 0x523198, and _ZTVN...GraphSparseI12PFGOuterEdgeEE (0x96C7C8) the D1 0x5231d4 / D0 0x523238,
// so deleting through the GraphSparse base or through the derived type reaches the same pair.
// PARTIAL: not written. It cannot be: the delete expressions need the complete types of the outer graph's edge and
// node, and neither exists in the tree. grepping the whole decomp-reconstruction tree finds no definition of
// PFGOuterEdge or PFGOuterNode (only the forward declaration in dh2/PFGOuterGraph.hpp), and the ELF has no such
// symbols either: recovered/native/symbols/libDungeonHunter2.so has a .symtab STT_FILE entry for PFGOuterEdge.cpp
// and PFGOuterNode.cpp but no vtable, destructor or method symbol for either class, so the recoverable evidence
// (the vtable slot used, 0x20, and the node's 0x44-byte allocation) is already recorded above and the classes
// themselves are unreconstructed. Missing item: dh2/PFGOuterEdge.hpp and dh2/PFGOuterNode.hpp (or the class bodies
// in whatever header owns them). The instantiation `template void GraphSparse<PFGOuterEdge>::clear();` cannot even
// be declared here until then. The row stays partial.

}  // namespace graph
}  // namespace math
}  // namespace sfc
