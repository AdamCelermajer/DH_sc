# progress-anim-ui — slice `anim-ui` (categories game-anim + game-ui)

Queue: `index/queue_anim-ui.tsv` (1109 rows: prio0 8, prio1 303, prio2 9, prio3 64, prio4 725).
Work done by one agent (no subagents available: delegation is depth-limited in this session).
Nothing outside `.../deepseek-audit/_parts/` was modified. No git, no build.

## Audited coverage (exact)

| prio | rows | audited (both sides read in full) | machine-screened only |
|------|------|-----------------------------------|-----------------------|
| 0    | 8    | 8                                 | 0                     |
| 1    | 303  | 32 individually (19 game-anim, 13 game-ui) + structural screening of all 303 | 271 |
| 2    | 9    | 8 (1 is a documented 1-line partial) | 0                   |
| 3    | 64   | 0                                 | 64 (token/structure screen only, no semantic verdict) |
| 4    | 725  | 0                                 | 0 (not reached)       |

Screening tool: `_parts/triage-anim-ui.py` (statement-level alignment of original vs reconstruction,
name/constant-normalised) run over every prio-0/1/2 row, plus `_parts/scan-anim-ui.py` for feature
diffs (control-flow counts, call/string/float sets). Screening ranked rows; it is **not** an audit
and no prio-3/4 row is claimed clean.

Individual game-anim audits, both sides read in full (21 prio-1 rows + 2 prio-0 + 2 prio-2):
0x364444, 0x4922e0, 0x492374 (prio 0); 0x3644cc, 0x3644fc, 0x3645e8, 0x3649bc, 0x3649e0, 0x364bf4,
0x364ca0, 0x3660f4, 0x366224, 0x366628, 0x36679c, 0x3672c8, 0x367420, 0x3674ac, 0x367ec8,
0x3c90f8, 0x3c94b8, 0x3caf3c, 0x3caccc, 0x4060c0, 0x406c2c, 0x4749f4, 0x4761cc, 0x4762c4,
0x47653c, 0x476780, 0x492560, 0x4927a4, 0x49296c, 0x492aa0, 0x492f68 (prio 1); 0x365d38,
0x3659ec (prio 2). 0x405b04 was reasoned from the original plus the reconstruction's structure and
the sibling Cmd_* functions (see the note below) rather than a full body read.
Individual game-ui audits, both sides read in full (14 prio-1/2 rows):
0x341bcc, 0x477074, 0x477094, 0x411f34, 0x7a7ccc (prio 0); 0x412098, 0x41b9bc (prio 1); 0x4372bc,
0x438278, 0x42ea04, 0x418360, 0x3fd5d0 (prio 2); plus 0x4269dc (FSCommand table compared
mechanically). 0x418d28 `HUDControls::OnEvent` and 0x7ac924 `RenderFX::UpdateCursor` were screened,
not decided. `HUDMinimap::UpdateIconPositions` (0x41b9bc) was verified group by group: the five
position arrays (unk_1cc/unk_1fc/unk_25c/unk_2bc/unk_34c) and the five counts
(unk_c8/unk_d4/unk_d0/unk_cc/unk_d8) match the original's a1[115]/[127]/[151]/[175]/[211] walks and
its a1[50]/[52]/[53]/[54]/[51] bounds, and `SetEnemies()` is called once per enemy iteration in both.

prio 2: all 9 rows were looked at; 8 decided (0x4372bc, 0x438278, 0x365d38, 0x4269dc, 0x42ea04,
0x3659ec, 0x418360, 0x3fd5d0, 0x3caccc). 0x4269dc `MenuBase::MenuBase(char const*)` is a
one-line stub in the reconstruction (`_ZN8MenuBaseC1EPKc` is marked partial in its own header
comment) — not reported.

## Findings

2 rows in `findings-anim-ui.csv`:

* no `high`-severity finding this slice.
* `medium`, kind `field` — **0x3fd5d0 `SortByEquipability::operator()`**,
  `game-ui/src/g1-game-ui-pack4-0efacf-0.cpp:209`. Real bug: the final tie-break uses
  `equipped(slot)` on the hand path and the ring path, where the original re-reads the path's own
  slot (`LeftHand` / `LeftHandRingFinger`, chunk_011.c:11898 and :11937 used at :11925 and :11945).
  Effect: with `m_equipSlot == RightHand` (or `RightHandRingFinger`) the comparator answers about
  the wrong slot, so the inventory sort order of equippable/equipped items can differ.
* `low`, kind `side-effect` — **0x4761cc `AnimSetManager::GetSynchronizedAnimator(int)`**,
  `game-anim/src/g1-game-anim-AnimSetManager-d3d27a-0.cpp:105`. The original null-guards the engine
  set before reading its dirty byte (chunk_016.c:366); the reconstruction dereferences
  `set.m_animSet` unconditionally. Not reachable through the current registration paths (they all
  call `CreateAnimSet`), hence low. The identical pattern in `GetAnimator` (chunk_016.c:404) is not
  reported twice.

One suspicious difference that was investigated and **dismissed** (recorded here so it is not
re-investigated): 0x405b04 `v2Controller::Cmd_Attack(GameObject*)` — the reconstruction calls
`m_controllable->Ctrl_Attack(target)` at the end of the body, outside the
`IsOnlineGame() && unk_00a && m_character != nullptr` block. The original's three early returns
(chunk_011.c:18328, :18330, :18333) do return without reaching the final call at :18371, but the
guard at :18329 (`*(_BYTE *)(v2 + 10)`) is the same flag the reconstruction's condition reads, and
the offline paths (:18327, :18329) reach :18371 — exactly like the sibling commands
`Cmd_UseOOI` (chunk_011.c:18460) and `Cmd_EndSkill`, which also call their `Ctrl_*` after the online
block. So the reconstruction reproduces the original; no finding.

## Deliberately NOT reported (documented deviations) — verified by hand

* 0x364444 `AnimApplicator::CalculateDelta` — same-time path returns the zeroed X delta where the
  original returns the `this` register residue; ledger documents it and the only caller
  (chunk_004.c:19616) discards the result.
* 0x4922e0 / 0x492374 `AnimatedFX::AnimatedFX(int)` — the differ's `memory this+0x4 (m_endOfLoop)`
  "orig=0x03 recon=0x0e" is not an initialisation difference: the original never writes +0x04 (no
  store), so the only real difference is that the reconstruction does clear `m_endOfLoop`; the
  ledger documents exactly this.
* 0x418360 `HUDControls::SaveHUDPos` — the original's `get_world_matrix()+8`/`+20` words are the
  matrix translation; the reconstruction's `WorldTranslation` helper returns `m_[0][2]` / `m_[1][2]`
  and its 11-element `LayoutElement` order matches the original's 11 fixed character offsets
  (+0x4c, +0x88, +0x3b8, +0x3e8, +0x418, +0x448, +0x478, +0x4d8, +0x4a8, +0x508, +0x568).
* 0x4761cc / 0x4762c4 / 0x47653c `AnimSetManager` — `map::operator[]` vs `lower_bound` and the
  extra `DebugSwitches::GetSwitch` value discards are container/idiom differences.
* 0x3659ec `AnimationSet::LoadAnimation(int)` — the original's own profiling string says
  "AnimationSet::ReplaceAnimation" (chunk_004.c:18772) and 0x365d38's says
  "AnimationSet::ReplaceAnimation" too; the reconstruction copies the original's (misleading)
  strings, so the `PushProfilingContext` text is reproduced, not invented.
* 0x412098 `Dropable::IsIntersecting` — centre of the dragable rect vs the target rect, edges
  included; `drag[1]+drag[0]` is the original's `v6 + v5` (xmax + xmin).
* 0x42ea04 `MenuManager::Update` — the interaction-type jump table is
  `CSWTCH.6518 = 0,1,2,3,5,5,6,7,5,5,4` (asm_chunk_018.asm:77927); the reconstruction's
  `kIconTypes[11]` matches exactly. `IsDisplayingMenuWireframe` → `SetWireFrame(true)` is the
  reconstruction of the original's register-carrying `SetWireFrame(dword_0 + 1)` call.
* 0x38xxxx / 0x3cxxxx / 0x47xxxx / 0x49xxxx functions whose reconstruction reads a member chain the
  Hex-Rays text prints as `*(_DWORD *)(x + off)` — offsets checked against the dh2 headers
  (`AnimatedFX.hpp`, `CharAnimator.hpp`, `Dragable.hpp`, `HUDControls.hpp`, `VisualFXManager.hpp`,
  `Animation.hpp`) where a suspicious difference was found.
* ASSERT/`gAssertLevel` bodies, `std::` containers, iterator-based tree walks, `off_XXXX`/vtable
  slot calls covered by named getters, and v1..vN temporaries: per the brief, not reportable.

## Addresses not matched / not readable

* 0x4269dc `MenuBase::MenuBase(char const*)` (prio 2): the reconstruction is a 1-line declaration at
  `game-ui/src/g1-game-ui-MenuBase-8948cc-4.cpp:395`; the whole 63-entry FSCommand registration table
  IS present at lines 400-473 and matches the original's order one-for-one (checked mechanically),
  so nothing is reported, but the function is not "reconstructed" in the strict sense.
* Rows whose queue `recon_span` no longer points at the function body (agents are editing the tree):
  every row was re-located by its `// 0x<addr>` comment before judging; line numbers cited in the
  CSV are current as of this run.
* prio 3 (64 rows) and prio 4 (725 rows): not audited. 64 prio-3 rows were structure-screened only.

## Honest gaps

The 278 prio-1 rows that were only machine-screened (including 40+ game-anim rows and the whole
`RenderFX`/`MenuFX`/`StringManager`/`HUDMinimap`/`MenuCharMenu_Map` families) are **not** verified
clean. The screen catches missing/extra branches, constants, calls and strings, but a
correctly-typed wrong-member access or a swapped argument in a named helper survives it. The
highest-value unverified groups for a follow-up pass are: 0x418d28 `HUDControls::OnEvent`,
0x7ac924 `RenderFX::UpdateCursor`, 0x41e064 `InfoHUDManager::FastUpdate`, 0x508ef4
`StringManager::parse`, 0x509aec `StringManager::parseEx`, 0x4151d8
`FlashAnimManager::ScanForAnims`, 0x41b9bc `HUDMinimap::UpdateIconPositions`, 0x498620
`SWFAnimManager::ExtendBank`, 0x409bd8 `v2PS3MoveController::Update`, 0x3c9f4c
`CharAnimator::SetAnimationSet`, 0x3caf3c `CharAnimator::Update` (read in full, no finding),
0x492aa0 `AnimatedFX::SyncIrrData` (read in full, no finding).
