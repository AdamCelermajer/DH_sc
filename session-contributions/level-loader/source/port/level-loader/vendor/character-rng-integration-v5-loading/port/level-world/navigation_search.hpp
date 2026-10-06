#pragma once
#include "navigation.hpp"
namespace dh2::navigation {
// Outgoing edges are ordered by destination node ID, as in GraphSparse's map.
struct SearchGraph {const Graph* graph;const std::uint32_t* edge_order;const std::uint32_t* offsets;};
using SearchPredicate=std::uint32_t (*)(void*,std::uint32_t);
struct SearchTest {SearchPredicate goal,edge_valid,node_valid;void* user;};
struct SearchNode {std::uint32_t edge;float distance,priority;std::uint32_t seen;};
struct SearchEntry {std::uint32_t edge;float distance,priority;};
struct SearchWorkspace {SearchNode* nodes;SearchEntry* heap;std::uint32_t node_capacity,heap_capacity;};
struct SearchRequest {const SearchGraph* graph;const SearchTest* test;std::uint32_t start,limit,external_path,reserved;};
struct SearchResult {
 std::uint32_t found,expanded,edges_examined,candidate_relaxations,non_goal_enqueues,path_count;
 std::uint32_t* path;std::uint32_t path_capacity,reserved;
};
static_assert(sizeof(SearchNode)==16&&sizeof(SearchEntry)==12);
}
extern "C" {
// Recovered AlgoAStar<PFGInnerGraph,DiabloIPhoneHeuristic>::findNode.
// Successful paths prepend edges in source-to-target order. On failure the
// original routine prepends marked incoming edges in descending node-ID order
// (a forest). external_path=1 preserves caller contents after the new segment,
// including a missing start;
// zero uses the original algorithm's cleared internal-output mode.
// 0 completed, 1 invalid request, 2 insufficient bounded storage; rejects are
// atomic. Callbacks and graph/container ownership are supplied caller services.
int dh2_nav_search(dh2::navigation::SearchResult*,const dh2::navigation::SearchRequest*,dh2::navigation::SearchWorkspace*);
}
