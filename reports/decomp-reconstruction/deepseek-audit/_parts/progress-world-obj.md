# progress — slice `world-obj` (category game-world)

Auditor: single agent (sub-agent delegation was refused: `subagent depth 2 exceeds maxDepth 1`).
Repo: `...\DH_sc\.worktrees\track-b` (read-only use). No git, no build, nothing edited outside
`reports/decomp-reconstruction/deepseek-audit/_parts/` and
`...\dh2-deepseek-audit\_parts\` (scratch).

Deliverable: `reports/decomp-reconstruction/deepseek-audit/_parts/findings-world-obj.csv` (1 row).

## Worklist reality check

`index/queue_world-obj.tsv` has **855** data rows, not the 22/335/7/59/432 split assumed in the task
text: **22 prio0, 335 prio1 (178 `const:`, 34 `float:`, 123 without a lead), 7 prio2, 59 prio3,
432 prio4**. (The TSV must be parsed with `csv.QUOTE_NONE`; the `symbol` column contains `"` and
default quoting silently merges ~half the rows.)

## Audited (by priority)

| prio | rows in slice | read in full + compared | notes |
|---|---|---|---|
| 0 | 22 | **22** | every row read on both sides; all clean |
| 2 | 7 | **7** | every row; all clean |
| 1 (`const:`/`float:`) | 212 | **31** in full, **181** machine-triaged only | see the triage caveat below |
| 1 (no lead) | 123 | 0 | not started |
| 3 | 59 | 0 | not started |
| 4 | 432 | 0 | not started |

prio0 rows verified clean (armdiff class `C-stl` / `A-fault-order` / `U-unresolved` were harness
artifacts in every case): 0x525188, 0x5252c8, 0x5252ec, 0x470c84, 0x522250, 0x52231c, 0x46e6bc,
0x46e864, 0x4a19f8, 0x46e918, 0x38d4e0, 0x4a1814, 0x4a18a8, 0x46e978, 0x38d4a4, 0x470ac4, 0x470bd0,
0x3402f4, 0x39b26c, 0x38d4d0, 0x38d530, 0x38d55c.
Layout claims were cross-checked against the headers (`PFRoom` +0x3c/+0x48, `PFFloor` +0x6c/+0x78,
`GameObject::m_rotation` +0x16c, `ObjectBase` +0x29/+0x80/+0x108, `ISceneNode::flags` +0x11c,
`RootSceneNode::unk_208`, `TriggerZone` +0x85/+0x7b4, `b2Body::WakeUp` = clear `e_sleepFlag` 0x0008
+ `m_sleepTime` at +0x8c) — all matched.

prio2 rows: 0x38d7ec createBindings (86 bindings, same order, same function pointers and same
`this`/nullptr third argument — checked mechanically), 0x3496b8 Flush, 0x349b14 + 0x349e8c both
`~ObjectManager`, 0x342f30 IsPacketValid, 0x34552c InitPost, 0x471f1c SetMeshVisible. All clean
(the two dtors are `Flush()` + container teardown that the implicit member destruction performs —
the 3rd argument of `CPacketManager::SequenceMoreRecent` is unused per its own ledger row).

prio1 `const:`/`float:` rows read in full and compared: 0x348ea4, 0x349240, 0x39eedc, 0x39e198,
0x39ea9c, 0x818dac, 0x8186ac, 0x818544, 0x8183e0, 0x4a20f0, 0x4a2f34 (with its helpers), 0x4a2858,
0x46e768, 0x46eb70, 0x46ebe4, 0x46ec6c, 0x46ece8, 0x46e818, 0x3f94f4, 0x347dec, 0x390f1c, 0x393444,
0x39672c, 0x471368, 0x527660 (with `ObstaclePhysicsPasses`), 0x38b300, 0x38b630, 0x3e763c, 0x3e786c,
0x3a2bcc, 0x3a2c54, 0x3a2098, 0x38ba04, 0x39f3cc. All clean except the one finding below.

## Findings

1 row, severity **low**, kind `branch`:

* **0x348ea4 `ObjectManager::Remove(ObjectHandle)`** — `RemoveNoRoomObject(object)` and
  `EraseFirst(unk_90, object)` are outside the `if (object != nullptr)` guard
  (`g1-game-world-ObjectManager-5017c7-2.cpp:480-481`), where the original performs both only inside
  `if (v5)` (`chunk_003.c:11761-11788`). Same function/root cause: the `unk_2fc` test is also
  null-guarded in the port while the original dereferences the object. Impact limited to an
  unresolvable handle whose id is still in `m_objectMap`.

## Deliberately NOT reported (verified, but out of scope by the brief or documented)

* **Documented deviations in `game-world/ledger.csv`** (checked before judging, per the brief):
  * 0x390f1c `_IsInRange` — ledger: "Deviation: a missing target pushes false, where the listing
    would fault." The port's extra `if (target == nullptr) { pushBoolean(false); return; }` is that deviation.
  * 0x34b064 `GetHighestThreatPlayer` — ledger: the original dereferences a null Character for an
    unresolved name; the port takes the empty-handle path.
  * 0x3a2098 `GetLootFixedNumPowers` — ledger: the original reads one word in front of the entry
    array when `m_slotIndex == -1`; the port returns -1 for that case (the added `m_slotIndex < 0`).
  * 0x471368 `VisualObject::SetVisible` — ledger (on 0x4713d0): the callee "is partial only in its
    ForceRegister call"; the port null-guards the scene-manager pointer.
  * 0x3496b8 `Flush` — ledger/comment: the original calls `CharAI::_UpdatePointers(nullptr)` for a
    null list entry; the character list never holds null, so the port omits that case.
  * 0x818544 / 0x8183e0 / 0x8186ac / 0x818dac (CRoomAttributes) — the `off_XXXX`/stack-temporary
    NetStruct `SetChanged` sequences are the Hex-Rays synthetic-helper pattern the brief excludes;
    the file comment documents it.
* **Added null guards on paths where the original faults** (a systematic port style, not reported as
  findings): `Remove`'s guards, `SetVisible`'s scene-manager guard, `IsHostTouching`/`GetHighestThreatPlayer`.
* **False-alarm lead classes** (each checked on at least one side): struct offsets in
  `+ 0xNN` form, `& ~N` masks rendered as 429496729x (e.g. `~1u` = 4294967294, `~8` = 4294967287,
  `~0x400` = 4294966271), float bit patterns (`0.2f` = 858993459, `1.0f` = 1065353216,
  `100.0f`), `std::list`/`_Rb_tree`/`std::vector` growth and traversal code, `__stl_throw_out_of_range`
  bounds asserts, `COERCE_FLOAT`/`LODWORD` accessors, inlined `std::string` ctor/dtor in object
  constructors, and the `magic * ((end - begin) >> 4)` rendering of `std::vector` size (Hex-Rays
  prints the division magic instead of the count).
* 0x4a2f34 `TargetList::Search`: the port writes the range test as `!(distance > maxRange)` where the
  listing prints `v19 <= a3` — the brief's `x >= a` vs `!(x < a)` rule; also handled correctly for the
  cone (`ConeAdmits`), so not a finding.

## Coverage gaps (honest)

* **Not reached at all: all 59 prio3 rows and all 432 prio4 rows of this slice.**
* **181 of the 212 prio1 `const:`/`float:` rows were only machine-triaged**, i.e. compared by
  signature (integer constants outside `+ N` offset contexts, comparison-operator counts,
  `if/else/for/while/switch/case/break/continue/goto/return` counts, callee sets) plus a
  comment-stripped skeleton, not by a full hand comparison of both bodies. Their leads are in
  `_parts/triage-cf.txt` (full bodies, risk-ordered) and `_parts/triage-cf-summary.txt`; the 123
  prio1 rows without a lead were not triaged at all.
* No address in the worklist was unreadable or unmatched; the `orig_chunk`/`orig_span` and
  `recon_file`/`recon_span` fields matched the current sources for every row I opened (the source
  tree did not move under me during this pass), so all cited line numbers are current.
* **Rows whose reconstruction delegates to a shared template that I did not audit** (so they stay
  unverified rather than clean): 0x52cb90 and 0x52b560 `PFWorld::_SearchGraph` — the original inlines
  the whole `AlgoAStar<PFGInnerGraph,DijkstraHeuristic>::findNode` A* loop (476 / 395 lines;
  `chunk_024.c:15937-16283`), while `g1-game-world-PFWorld-d9994a-2.cpp:287-301` calls
  `search.findNode(...)`. Equivalence hinges on that shared `findNode` body (used by the 0x52ad4c
  DiabloIPhoneHeuristic row as well), which is outside this slice's worklist.
* The four largest prio1 rows were **not** read: 0x347fd0 `ForceFullUpdate` (orig 513 lines),
  0x346510 `SerializeGameObjectNetData` (692), 0x34735c `LoadGameObjectNetData` (533),
  0x3f82d8 `Level::Update` (296) — these are the highest-risk remaining rows of the slice.
  (`_CalcObstaclesForce` 0x527660 and `TargetList::Search`/`SearchRect` were read with their
  helpers and are clean.)

## Scratch artefacts (in `...\dh2-deepseek-audit\_parts\`, re-usable)

`dump.py` (side-by-side dump for queue rows), `split_batches.py`, `triage.py` (+`triage-cf.txt`,
`triage-cf-summary.txt`), `focus.py` (+`focus1..6.txt`), `conds.py` (control-flow-only view for big
functions), `AUDIT-SUBAGENT-INSTRUCTIONS.md` (batch instructions written for delegation that could
not be started), `write_findings.py`.
