# Native graph-node search

`navigation_search.cpp` reconstructs the original specialized
`AlgoAStar<PFGInnerGraph, DiabloIPhoneHeuristic>::findNode` at `0x52ad4c`
and `markNode` at `0x529da4`. Captured heap, map, list and graph helpers are
listed in `original-functions.json`; the original engine SHA is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The original ELF is a local validation input, never an APK runtime dependency.

## Recovered behavior

- Outgoing edges traverse GraphSparse's map in destination-node-ID order.
- Distance is single-precision `edge.weight + popped_entry.distance`; the
  priority adds zero in this specialization. No guessed heuristic is inserted.
- A seen node changes only when the new distance is strictly smaller. Stale
  queue entries remain; there is no additional closed set.
- The heap orders by lower priority. Original SGI pop chooses the right child
  on equal priorities; native code preserves that ordering explicitly.
- Goal destinations bypass edge/node validity checks. Repeated goal predicate
  calls and their short-circuit ordering are preserved. When a newly marked
  goal is accepted, the heap drains, that goal is queued, and the remaining
  outgoing edges of the current node are skipped.
- The limit counts expansions. Exhausting it leaves the current node in place
  before the final goal query. A zero limit still queries the start twice.
- Success walks incoming edges back to the start and inserts each at the front
  of the output list, producing source-to-target edge order. Failure enumerates
  the mark map ascending by node ID and inserts at the front, producing a
  descending-ID forest. Failed output is not a usable route.
- Internal output clears before every request. Supplied external output keeps
  its old contents after the newly prepended segment, even for missing starts.

## Modern ownership boundary

SearchGraph supplies a read-only contiguous node/edge graph and a sorted outgoing
index. SearchTest supplies boolean goal, edge-valid and node-valid callbacks.
SearchWorkspace and SearchResult own bounded caller storage. Malformed requests,
nonfinite/negative input edge weights, invalid ranges and insufficient storage
reject before mutation or callbacks. The original producer supplies nonnegative
distance weights; this API validates its caller contract rather than imitating
legacy allocator exhaustion. Scratch is reused sequentially, not concurrently.

`floors::post_load` builds the index for the actual sewn native graph;
`floors::search_nodes` invokes the recovered search. Android startup runs a
development probe of all 64 ordered floor pairs: all succeed, emit 1,598 segments,
and match the original result/statistics checksum `5454073390c7d916`. The probe
uses caller all-valid predicates and chooses graph-node IDs directly.

## Validation and remaining scope

The differential executes full original search/mark/map/list/vector/heap bodies;
graph input trees, predicates, allocator and deallocator ownership are fixtures.
It compares 1,084 requests: 256 authored Crypt cases across all floor pairs and
four expansion limits, explicit missing/start-is-goal/queue-purge/prefix/tie/
zero-cost/stale-entry/validity cases, and 600 seeded synthetic cases. Returned
edges, all statistics, 96,279 search-node records and 101,090 predicate calls
match the compiled ARM64 source. A separate ASan/UBSan replay also checks the
actual asset-loader graph and its search adapter against the same original gold.
The two APK ARM64 libraries are audited separately; x86_64 emulator startup
checks the same 64-route checksum through live level loading.

The original PFWorld endpoint selection, temporary source/target nodes,
radius/obstacle validity producers, complete FindPath, smoothing and movement
controller remain pending. The search is not connected to enemy pursuit or
player movement yet. Current movement remains the existing supported-floor
adapter. Metadata, rotated/generated rooms and full lifecycle also remain
outside this checkpoint; no physical ARM64 test or full playable game is claimed.
