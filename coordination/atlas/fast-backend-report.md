# Fast Atlas backend handoff

## Scope and invariant

The server presents a searchable view over the generated typed graph. The
original graph, asset corpus, and decoded resources remain read-only. The
consumer invariant is that an entity and its recorded source relationships,
including direction, status, confidence, reason, source site, and reference
intermediaries, can be inspected without transferring the entire graph to the
browser. This is an internal viewer change, so there is no original gameplay
sequence to cite. The pre-existing source and document routes remain the
evidence endpoints for original bytes and indexed documents.

## Implementation

`graph_store.py` streams `data/graph.json` into an ignored SQLite cache under
`tools/atlas/.runtime`. A cache manifest records the graph SHA-256 and source
stat identity (resolved path, size, modification time, and file identity).
Warm startup validates the current identity and cache metadata. A changed graph
is hashed, then either reuses its content-addressed cache or rebuilds a
temporary database and atomically replaces the cache pointer. The source JSON
and source assets are never written. SQLite provides exact ID lookups,
forward/reverse edge indexes, category/name indexes, and an FTS5 search index
when available.

The server provides:

- `GET /api/meta`: counts and categories only.
- `GET /api/search?q=&category=&offset=&limit=`: FTS-backed, stable-ranked
  results. Exact name, display name, and alias matches sort ahead of prefix and
  partial matches. Known typed prefixes such as `skill:`, `item:`, and
  `actor:` scope the search; numeric typed suffixes match exact row indices.
  Android path aliases and known iPhone URI prefixes normalize for lookup, and
  an unknown prefix such as `q:` remains part of a path query. The default
  category omits raw References; choosing References exposes them. Pages cap
  at 100 rows.
- `GET /api/entity?id=`: one node and profile, bounded direct incoming and
  outgoing rows, endpoint nodes, one reference intermediary hop, and a resource
  packet capped at 1,800 nodes, 7,000 edges, and depth 10. Full edge records
  retain the source proof fields and original `from`/`to` orientation.
- `GET /api/closure?id=&direction=`: on-demand forward or reverse closure,
  capped at 5,000 nodes and 7,000 edges.
- `GET /api/native?id=&script_offset=&function_offset=&limit=`: bounded,
  independently paginated script and native-function evidence for one entity.
- `GET /api/native-source?function_id=&address_index=`: an excerpt lookup by
  indexed function ID and address slot. The optional `full=1` streams an
  individual pseudocode file only when its indexed path resolves inside the
  approved IDA pseudocode corpus and the file is at most 2 MB. Otherwise it
  returns a plain-text explanation and the indexed excerpt. It never accepts a
  caller-provided filesystem path.

The HTTP server serves the editable template at both `/` and `/index.html`.
Those routes replace the title, remove the offline payload placeholder, and
load the API. Generated `index.html` remains the portable offline snapshot.
Existing `/api/source/<asset-id>` raw and decoded-byte behavior and
`/api/document?path=` remain in place. Writes continue to return 405. When
`data/entity-native.json` is present, its SHA joins the cache signature and
matching entity responses add a bounded `entity_native` summary. Each native
page caps at 24 rows; returned functions carry at most 16 addresses and 1,200
snippet characters per address. The separate `data/resource-attributions.json`
maps and SHA join the same cache, and matching source/resource entities receive
their attribution rows. Canonical `entity:<table>:<index>` records join through
the graph node's exact table/index metadata, while existing actor/raw-ID
candidates remain available.

## Verification

The pre-code test plan was to check category and paging bounds, exact alias
ordering, malformed entity IDs, stale cache replacement, both edge directions,
entity plus reference-hop packet coverage, native/non-reference entity lookups,
closure truncation bounds, compact page serving, and the existing source,
decoded-source, document path, and write-rejection cases. `graph_store_checks.py`
implements those checks using a small mutated fixture and the current generated
graph through the real HTTP handler.

The final schema-4 full-corpus check passed **47/47 tests**. The 24 fixture checks
cover typed CLI queries, exact numeric kind/index lookup, physical source/site
paths, case-insensitive Android aliases, logical iPhone URIs, unknown `q:` path
prefixes, graph-row IDs, gameplay entities ahead of incidental exact Text
matches, explicit-category ordering, stale-cache replacement, edge orientation,
native pagination, canonical table-hash to table-index native/resource joins,
indexed excerpt and full-file paths, traversal rejection, ignored caller paths,
and write rejection. Live checks confirm BashDown and Bow12 native records and
animation/effect resource profiles appear in their entity packets; BashDown no
longer reports the false missing `CharacterProperties[172]` row. Handler
checks cover Mud Lord, indexed source bytes and decoder behavior, closure
bounds, compact page serving, document traversal, and read-only methods.

The final generation contains 228,667 nodes, 421,316 edges, and 6,852 profiles.
The native index contains 7,042 entity records, 219 scripts, 7,982 Lua calls
with source excerpts, and 288 function ledger rows; 271 functions have 498
native-address bindings. The resource index contains 3,579 profiles and 6,334
files. Detailed measurements and test results are saved in the ignored runtime
file `tools/atlas/.runtime/fast-final-verification.json`.

Measured on this worktree:

| Operation | Time | Response or cache size |
| --- | ---: | ---: |
| Cold SQLite cache build/open | 36.32 s | 918,540,288 byte SQLite cache |
| First check-process cache open | 84.53 ms | same cache |
| Warm cache open | 85.87 ms | same cache |
| `GET /` fast template | 15.35 ms | 70,792 bytes |
| `GET /api/meta` | 3.96 ms | 675 bytes |
| `GET /api/search?q=Mud%20Lord&limit=5` | 5.10 ms | 1,365 bytes |
| `GET /api/entity` for Mud Lord | 357.96 ms | 3,140,053 bytes |
| `GET /api/entity` for BashDown | 24.11 ms | 233,685 bytes |
| `GET /api/native` for Mud Lord | 10.82 ms | 69,813 bytes |
| Full indexed IDA source via HTTP | 5.24 ms | 224 bytes, exact file match |

The cold build reads and hashes the 466.7 MB graph and both side indexes once;
repeat server launches reuse the stat-validated cache. The largest tested
packet is Mud Lord's bounded entity/resource view. A real browser render and
navigation check remains with the integration owner; backend tests do not
establish visual correctness.

## Native attribution join

The native-attribution schema uses `entities_by_id`, `scripts_by_id`, and
`functions_by_id`. The backend tries exact graph IDs and explicit typed row
prefixes (`table:`, `scene:`, `object:`, `entity:`) when joining. Returned
summaries retain association status, unresolved links, IDs, native address,
pseudocode path and line, contract/port labels, and bounded snippets. A real
indexed individual source file was streamed through the handler and matched
the on-disk source bytes exactly; aggregate pseudocode fallback remains an
excerpt with an explicit full-file-unavailable explanation.
