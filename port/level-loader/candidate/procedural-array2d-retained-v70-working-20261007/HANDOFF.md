# V70 actual Array2d and retained generated graph

Source-only coherent three-file successor over privately adopted V68/V69. Previous handoffs remain unchanged. No tests, compiler, execution, hashes, fixtures or proof cycles were run.

Replace `procedural_layout_v1.hpp/.cpp` and `procedural_generator_stream_v66.cpp` on the SAME existing owner target. No new TU, World, parser, cache, manager or alternate generation provider.

The original Array2d C1 is created inside the actual generator occurrence before config/rule allocation. Source EnsurePosition48b578 grows columns first across every existing row, then grows front/back rows with null cells, retaining x/y origins and shared row width. Each cell query invokes EnsurePosition, including unsuccessful FitsInMap probes. Fits follows original x-outer/y-inner order; placement/removal follows original y-outer/x-inner order. Clearing empties rows and resets origins/width; removal leaves extents intact. Host deque ABI/allocation sizes are not asserted as original STLport/ARM byte parity.

The same loader-owned Tile/root114/list/grid body stays under generator14c through Save. A successful/no-layout result publishes immutable row projection without disposing that body. Root114 publishes only after source RootRule::Generate completes; checked projection failure still retains an already published root. Active roots before publication are retained as reached checked failure prefixes, never abandoned with raw grid references. No completed/failed source occurrence is replayed.

Original D0 order is implemented: UnloadTiles4843a0 calls Unspawn while block/list sources remain live, clears114 after completion, then unloads rule/list/pool resources and config; embedded grid storage is destroyed last. Reverse neighbor removal and swap-last child removal preserve their original count-before-recursion prefix. Detached children stay owned through partial checked cleanup; failures retain the actual generator/Level14c owner and temporary stream. Teardown work does not trip a previously reached generation work-budget limit. No post-D0 API inspects native source cells or the borrowed generator32 pointer.

Shared libc48 allocation remains separate from generator32 as V68. Clear precedes source seed reset/RootRule::Impl C1. Serialization requires the same live body/root114 and still uses V69 original retained malloc scratch, source property map and actual XML output.

Remaining explicit boundaries: original allocation-failure diagnostics/unwind after partial Array2d growth, unsupported raw arithmetic/capacity domains, and V69 no-NUL scratch edge. A partial allocation failure preserves its storage prefix and stops cleanup; it is not reported as successful D0. Historical row equality is not new current-source runtime proof. Integrated all35 generation/serialization/parity remains pending the Root milestone.
