# DeepSeek session handoff — Track B, 2026-10-15

Handing back to the Claude agent. This covers one session: **rounds 13–40** of the persisted goal, from
commit `1217c766` to `915229f8`.

**Read this first, then `port/decomp-reconstruction/TRACK-B-HANDOVER.md` (which I kept updated in-tree),
then `TRACK-B-STATUS.md`.**

---

## 1. Final state — verified, committed, clean

| | value |
|---|---|
| HEAD | `915229f8` |
| worktree | **clean** |
| tests | **828 / 828 — 100%** (serial, quiet tree) |
| build | 0 compile errors, 157 undefined symbols, 0 multiple definitions |
| partial | **1,008** (was 1,237 at round 13) |
| reconstructed | **18,158** (was 18,059) |

Reproduce:

```
cd <ROOT>                                        # DH_sc/.worktrees/track-b
cmake -S port/decomp-reconstruction -B build/tb-int -G Ninja   # MinGW GCC 16.2.0
ninja -k 0 -C build/tb-int
ctest --test-dir build/tb-int                     # SERIAL - see §7
py -3 tools/decomp_progress.py
py -3 tools/trackb/link_report.py missing --build build/tb-int
```

The objective's four clauses: the engine-residue wave is **integrated**, the tree **builds and links
cleanly**, the wave is **committed** to `track-b/development`, and reconstruction **continues** — the
fourth is unfinished at 1,008 partial rows. **The goal was left active, not complete and not blocked.**

---

## 2. Commits produced (27, oldest first)

```
d815b22d  round 13 worker wave - suite 40 -> 14 failures of 818
58c15d15  the test suite is FULLY GREEN - 819/819, 0 failures
bac627c5  wave 14 - CImage blit family, CBatchDriver append fix, GLLive record, plus audit coverage
b573dcc8  link-stub cycle closed for the newly defined symbols; inert-stand-in audit
4685d714  fix the last real test failure; a clean build proves the tree green
5b664d1f  every ledger is now a valid 5-field CSV; 0 ignored rows
2cec9a4a  refresh TRACK-B-STATUS.md to the verified 2026-10-15 state
3f6ce686  re-test GetApplicator's blocker and record why it must NOT be written by slot offset
6353b2c6  record the two abandoned mid-edit incidents and preserve the stashed work
8ffd6db2  shared-string heap global + operator=, and the five missing GetNewInstance constructors
9d66ceb3  net-session callback batch, pydata reloadData, pixel_format copy/convert
15084c18  hazard-class audit (extern C drift, vtables-as-data), stub cycle closed
7a64f6d5  correct two glitch-video notes that had become false this round
41cd58bb  move a worker's audit report out of the reconstruction tree
e51d5397  serialize the two tests that share ./iglu.bin, removing a phantom failure
51d54f69  consolidate the CMatching vtable decision into one evidenced handover item
f645cba4  fix a use-after-free in CBatchDriver::initBuffer (0x58a920)
79a3ddeb  fix CBatchDriver::flush's gate offset (+0x44 -> +0x50 mappedVertices)
179b7271  settle the +0x234/+0x238 callback dispute - the header was wrong
2fbdf54b  write flush's end-of-batch callback call; 0x5a3a20 partial -> reconstructed
59908686  enable the initBuffer checks now that its use-after-free is fixed
f9eb2680  turn around the mirrored copyTo family; 3 rows reconstructed
e982928e  fix game_center_start_test (test stand-in, not a body bug)
7392ccf4  classify the scene_batch_visible_test flake - a process problem, not a test defect
3f53668b  registerSolidBatches written; CBatchDriver::compile deleted; convert arms; own members declared
915229f8  convert's luminance arm; repair the mid-edit buildPackedPlan I committed
```

---

## 3. CRITICAL — corrections that must not regress

These were each wrong in the tree before this session and are now right. Several were asserted in
multiple notes at once. **Re-read the register-level proof before changing any of them.**

| # | Correction | Where |
|---|---|---|
| 1 | **`CBatchDriver::compile` does not exist.** It was invented. `registerSolidBatches` reads `node+0x110` = `ISceneNode::sceneManager` and calls `CSceneManager::registerNodeForRendering` (0x58f348), already reconstructed. `CBatchDriver`'s vptr+0x24 (0x97712c) is the inherited `IVideoDriver::draw3DTriangle` (0x5a8bdc). The declaration was deleted (it was a rule-10 violation). | `glitch-video/include/glitch-video/batch_driver.hpp` |
| 2 | **`+0x234` = END-OF-BATCH, `+0x238` = SEGMENT-COMPILE.** Proof: `compileInternal` 0x58facc's `v14[5]` at `SP+0` has vptr `off_9762B8` = `_ZTV…SDefaultEndOfBatchCallback+8` and `SP` is stored to `driver+0x234` (0x58fb64) with `finalize` running on it (0x58fc00); `v20[6]` at `SP+0x2C` has `off_9762A0` = `…SDefaultSegmentCompileCallback+8` → `+0x238` (0x58fb70). **These names flipped three times; the four register-level facts above are the check, not any note.** | `batch_driver.hpp`, `batch_driver.cpp`, both ledgers |
| 3 | **`image.cpp`'s `copyTo` family was mirrored.** `this` is the pixel SOURCE, the `intrusive_ptr` argument is the DESTINATION — proven by `convert`'s mangled signature (param 2 `void const*`, param 5 `void*`) and the register flow at 0x600880. `a.copyTo(b)` used to target the wrong image. `0x60090c`'s rectangle was likewise from the target's dimensions instead of this image's. | `glitch-video/src/image.cpp` |
| 4 | **`hashString` sign-extends every byte** (`LDRSB`). The body previously marked `reconstructed` did not, so it was wrong for every non-empty string; its test survived only because `97 + 0x9E3779B9` happens to equal the correct `97 - 0x61C88647` mod 2^32. | `ps/IParticleSystem.hpp`, `src/ps/IParticleContext.cpp` |
| 5 | **`initBuffer` was a use-after-free**, measured as exit `0xC0000374` STATUS_HEAP_CORRUPTION: the port dropped the append buffer twice, deleting it inside `initBuffer`, then add-ref'd freed memory. `releaseAppendBuffer` helper is now dead in `batch_driver.cpp:55` (benign `-Wunused-function`). | `glitch-video/src/batch_driver.cpp` |
| 6 | **`flush`'s unmap gate is `+0x50` (`mappedVertices`), not `+0x44` (`indexUsedBytes`)** — they diverge when vertices are mapped but no indices are written. | `glitch-video/src/batch_driver.cpp` |
| 7 | **`CBatchDriver`'s `+0x23C/+0x240/+0x244/+0x248` are its OWN members**, not the base `IVideoDriver`'s (whose batch state is `+0x110/+0x120/+0x124/+0x128`). There is **no "0x11C offset bias"** — an earlier note invented one. | `batch_driver.hpp` |
| 8 | **The registration word is 1-based while the material index is 0-based.** `registerSolidBatches` does `SUB R2,R8,#1` for `getMaterial` (0x57e880) but passes `R8` (0x57e8d8); `render` 0x5808ac decodes it as `renderSolidBatch(word-1)`. Passing `batch` instead of `batch+1` draws the wrong batch. | `CBatchSceneNode.cpp` |
| 9 | **`SSharedStringData::useCount` exists at `+0x00`** — added on three independent listing reads (`operator=` 0x5daf94, `getParameterID` 0x5d308c, and `commit` 0x6a4cb8 zeroing the word before strcpying text to `+4`). It is load-bearing: without it two handles on one record double-free. | `glitch-shared/shared_string_heap.hpp` |
| 10 | **`flag-2 (float)` refusal in `convert` sits in the `0x5f9778` path (0x5f96d4)**, not on the luminance side. Several notes had it in the wrong arm. | `glitch-video/src/pixel_format_convert.cpp` |

**Two `CMatching` mis-overrides remain deliberately UNFIXED** — they change vtable layout. Fully
evidenced in `TRACK-B-HANDOVER.md` ("THE CMatching VTABLE REORDER"): `CMatchingGLLive::GenerateNewMemberId() const`
and `CMatchingLocal::IsInRoom() const` do not override, so virtual calls silently run the wrong body;
plus `CMatchingLocal::IsInRoom(int) const` is a dispatch bug **no compiler warning can see at any
verbosity**. The base also lacks the `+0x64 IsInRoom()` pure slot the binary has.

---

## 4. Rows flipped this session (partial → reconstructed)

`0x5daf94` `SSharedString::operator=` · `0x6a5538` `initSharedStringHeap` · `0x6a5074` `SData::get` ·
`0x5cef08` + `0x5d4714` `CMaterialRenderer::getParameterID`/`getTechniqueID` ·
`0x340fe0`/`0x34104c`/`0x3410a4`/`0x3410fc`/`0x342600` the five `GetNewInstance<T>` factories ·
`0x5d8b28` `createMaterialRenderer` · `0x5a0ec0` + `0x6a48cc` + `0x6bc2cc` bounding boxes ·
`0x58d908`/`0x58ddd0` `CSceneManager` ctors · `0x5a2e20`/`0x5a2ed8`/`0x5a3d50`/`0x5a8a80` `CBatchDriver` ·
`0x58a920` `initBuffer` · `0x5a3a20` `flush` · `0x57e800` `registerSolidBatches` ·
`0x63b44c`/`0x64d0bc`/`0x630a34`/`0x6309ac`/`0x63b560` etc. `IParticleContext` ·
`0x600708`/`0x60090c`/`0x60177c` `copyTo` · `0x601b10`/`0x601c24` sub-image ctors ·
`0x499f24`/`0x49aa60`/`0x49ac18`/`0x49ade0`/`0x49b3c8`/`0x49b47c` net-session callbacks ·
`0x4c540c` `PyDataConstants::reloadData` · `0x5ee40c` `pixel_format::copy` ·
`0x81e680`/`0x81d1f4`/`0x81d298` `CMatchingGLLive` record · `0x49a808` `HandlePlayerLeft` ·
13 mis-homed glitch-scene rows re-pointed at `src/scene/mesh_functions.cpp`.

**Some rows were DOWNGRADED too** (`0x58a920`→partial while its UAF was open, `0x60090c`→partial while
mirrored). Reconstructed moved *down* twice while trustworthiness moved up. That is correct behaviour.

---

## 5. Open items with EXACT blockers

**`CBatchDriver::thisAppendBatch` (0x5a6fe0)** — stays `partial`; full recovered structure is in a 90-line
comment in `batch_driver.cpp`. Missing: `spatialGridClustering` 0x5a5860 and `kdTreeClustering` 0x5a6568
(both `skipped`), `isVertexStreamHomolaced` 0x5a23f4 and `copyVertex` 0x5a2b18 (both `partial`, no bodies),
and the three callbacks' `operator()` declarations (neither interface declares one in
`scene_simple_accessors.hpp`).

**`registerTransparentBatches` (0x57ea70)** — the material handle comes from the segment's own stream
table (`segment+0x04` → `*(meshBuffer+0x14)` → vtable `+0x18`). `SSegment` `+0x04`/`+0x22` and
`CMeshBuffer +0x14` are unmapped padding, and `CBatchMesh::getSegmentCenter` 0x57e90c has no body
anywhere.

**`pixel_format::convert` (0x5f95ac)** — stays `partial`. Unwritten: `decompress` 0x5fd894 (body read but
it calls `PVRTCDecompress`, which lives in game-app, cross-category), the same-type switch 0x5f9778,
the 0x5f9ebc 24-bit arm, and `convertPacked`'s inline arms 1/2/4/6/8/9. **Two attempted generalisations of
those inline arms were reverted** because they could not reproduce the tree test's expected bytes for
key 6 (`0xfc5f`/`0x2188`) — that work needs a careful pass over 0x5f4fe8's per-channel shifts.

**`GetApplicator` (0x369160)** — stays `partial` and **must NOT be written by invoking the slot through
a function pointer**. Slot 9 is a BASE-class slot in the port, not a type getter; calling it would
dispatch to an unrelated function. The unnamed-slot trick that works elsewhere requires the slot to hold
the right function but be unnamed — it does not apply here.

**`CBatchSceneNode`'s visible-segment list (`+0x158`)** — unresolved. One `SSegment*` is one 32-bit word
in the original but 8 bytes on the host, so `postCompile` 0x57ef64 under-allocates and two non-empty
lists overlap. `addVisibleSegments` is still `skipped`; three options are named in the handover.

**`+0x24C`** — the one word of `CBatchDriver` still unclaimed, deliberately undeclared.

**CMatching vtable reorder** — see §3.

**Residual risk not enumerated**: 18,158 rows are marked `reconstructed`; several found this session were
wrong. That number carries unknown error that only executed coverage and audits can bound.

---

## 6. Audits written (in `default-workspace/dh2-mcp/`)

`INERT-STANDIN-AUDIT.md` · `HAZARD-CLASSES-AUDIT.md` · `WARN-AUDIT-overloaded-virtual.md` (also
`dh2-warn-audit/`) — plus the scripts: `repair_ledgers.py`, `fix_field_counts.py`, `find_bad_rows.py`,
`why_ignored2.py`, `drop_dup_stubs.py`, `drop_unscoped_rows.py`.

Headline results: **427 `-Woverloaded-virtual` warnings are 12 declarations** (GCC re-emits per TU), of
which **2 are genuine mis-overrides and 0 are enum/`int32_t` artefacts**. A null vtable **and null
typeinfo** were winning a link race (`Character-ee9946-4_test.cpp` declared `const char[256]` arrays
renamed onto three `_ZTV` symbols); fixed. **42 `asm()` abort stubs declare an arity different from the
symbol they rename onto**, so those tests cannot cover 104 functions they appear to stub.

---

## 7. Process warnings for the next agent

1. **Never run a full `ctest` while worktree builds are in flight.** `build/tb-int` is shared. This
   produces false failures: `scene_batch_visible_test` flaked three times under `-j 4` and passes
   serially; a worker saw `image_copyto_test` come back "Not Run" for a missing exe. Run **serial on a
   quiet tree**, or give each worker its own build dir.
2. **`link/library-duplicates` is RECONFIGURE-SENSITIVE and its failures are usually NOT defects.**
   `link_stubs.csv` is a `CMAKE_CONFIGURE_DEPENDS` input and `CMakeLists.txt` numbers the stubs **by CSV
   row index**, so editing the CSV after configure leaves stale `stub_NN.cpp` defining symbols real code
   now implements. **Always re-run `cmake` before believing that test.** This is also the mechanism behind
   the intermittent `CMake Generate step failed`.
3. **Incremental green is not proof.** An earlier "100% of 819" was stale — it ran against binaries built
   before a header change, while a clean build of the same tree gave a different answer.
4. **Two workers stopped mid-edit leaving the tree unbuildable.** Both are recorded in
   `TRACK-B-HANDOVER.md`; one worker's 276-line draft is preserved in `git stash` (`stash@{0}`) — treat it
   as a draft to check against the real headers, not code to reapply.
5. **Give every worker this as its last action:** `./tools/trackb/check_tu.sh <file>` on every file it
   touched, and confirm `OK` before reporting. Two incidents would have been prevented.
6. **Do not use `git add -A` on this tree.** Concurrent writers mean it sweeps in-flight worker edits and
   stray artifacts. I made this mistake three times, once committing a mid-edit `buildPackedPlan` that
   failed its test, and once committing a worker's audit report into the branch. **Use scoped adds.**
7. Several ledger rows were malformed by hand-editing (unquoted commas inside symbol names, text after a
   closing quote). The ledgers are currently valid — every row exactly 5 fields, 0 ignored rows. Use
   `csv.writer`, never string splitting.

---

## 8. The methodological finding worth carrying forward

**The `partial` count is not a measure of correctness.** Of the real defects found this session — a
use-after-free, a mirrored image copy, a missing sign-extension, swapped callback types, a null vtable
winning a link race, 42 wrong-arity stubs, an invented `CBatchDriver::compile` — **essentially none came
from querying `partial` rows.** They came from three different questions:

1. **Has this body ever actually executed?** (gdb breakpoints over every executable that contains the
   symbol) → found the `hashString` bug and the `initBuffer` use-after-free. Three of four freshly written
   bodies had never been entered despite the suite being green.
2. **Is this claim proven, or just written down?** (re-derive from `get_bytes`/`nm`/`objdump`, never from
   a note) → found the invented `compile`, the swapped callbacks, the null vtables, the inert
   `get_fontfile` stub.
3. **Does this test discriminate?** (restore the old body and confirm the test fails) → the `copyTo` test
   failed 9 checks on the mirrored body, which is why it can be trusted.

Stale notes were themselves a working defect: I corrected notes in `glitch-video`, `net-session` and the
status doc that asserted blockers which had already dissolved, and three separate workers independently
flagged `TRACK-B-STATUS.md` as misleading before I refreshed it.

**Recommendation for whoever continues:** the highest-value work is probably *not* the 1,008 remaining
`partial` rows (honest, mostly large bodies) but the bodies already marked `reconstructed` that nothing
exercises. That is where the unknown error lives.
