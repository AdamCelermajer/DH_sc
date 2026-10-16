# progress-app (slice: app / game-app)

Worklist: `dh2-deepseek-audit/index/queue_app.tsv` (1326 rows).
Ground truth: `recovered/native/hexrays-all/chunk_*.c` (+ `recovered/native/ida-asm/asm_chunk_*.asm` where Hex-Rays
was ambiguous — used to settle the two `Application::_Draw` findings and to confirm signed compares / field offsets).
Reconstruction: `port/decomp-reconstruction/game-app/src/*.cpp`, headers under `port/decomp-reconstruction/game/include/dh2`.
Findings CSV: `reports/decomp-reconstruction/deepseek-audit/_parts/findings-app.csv`.

## Audited counts

| prio | rows in slice | rows audited | notes |
|------|---------------|--------------|-------|
| 0    | 15            | 15           | all read; 13 are the GetRandom clone family |
| 1    | 421           | 47           | 46 prio-1 rows + 1 prio-4 row picked up incidentally; 240 prio-1 rows carry a `const:`/`float:` lead — 46 of those were covered, the rest were not reached |
| 2    | 6             | 6            | all read |
| 3    | 76            | 0            | not reached |
| 4    | 808           | 1 (0x43d288, incidental) | not reached |

Total: 69 rows audited (15 prio-0, 46 prio-1, 6 prio-2, 1 prio-4).

## Findings by severity

* **medium 1** — `0x32ade8 Application::_Draw`: `gui->drawAll()` is called *before* `driver->beginScene2D()`
  (reconstruction line 776 vs 778); the original calls driver vtable `+0x14` first and the gui slot `+0x0C` second
  (`chunk_002.c:9046-9047`, asm `LDR PC,[R3,#0x14]` then `LDR PC,[R3,#0xC]`). The gui/Flash layer is therefore drawn
  outside the 2D pass.
* **low 2** —
  * `0x32ade8 Application::_Draw`: the `IsRenderingGlitchBoundingBoxes` branch is missing the whole material setup
    (driver+0xDC material renderer, renderer index, `getMaterialInstance`, `getTechnique`/255, `setMaterial`, and the
    `intrusive_ptr` release) that the original runs before `DBG_TraverseAllGlitchNode` (`chunk_002.c:9015-9035`,
    asm 0x32B110-0x32B180). Debug-switch-only path.
  * `0x4556c4 Script_ShowFlash::IsBlocking`: `static_cast<std::uint32_t>(args->Loop) > m_menu->m_animOverCount` is an
    unsigned compare; the original's ARM code is `CMP R3,R0` with `MOVLE/MOVGT` (signed), as the sibling
    `Script_HideFlash::IsBlocking` (0x455708) is reconstructed. Differs only when `m_animOverCount >= 0x80000000`
    (`chunk_014.c:13663`).

No high-severity finding.

## Rows audited (all judged clean unless marked)

prio 0: `0x33ff90` GetRandom (canonical body, verified arithmetic/streams/counters), the 10 clone rows whose synced
flag is folded to false (`0x37bc2c 0x388c58 0x38e578 0x3af6d8 0x3c26a0 0x3ca708 0x3e8dac 0x3ec5d4 0x401afc
0x493780`, all documented folds into the one definition) and `0x38bc3c` (clone.2, range folded to 99, synced flag in
r0), `0x455848` Script_OpenDoor::IsBlocking (DOOR_OPENED=1 / DOOR_OPENING=3
and Door+0x3a8 verified in `dh2/Door.hpp`), `0x4556c4` (finding), `0x455708`, `0x31f4f4` (documented deviation).

prio 1: `0x32ade8*` (2 findings), `0x32bdc8 0x32d79c 0x31f6b0 0x320774 0x320808 0x31f5b4 0x323ae0 0x32a9c8 0x32ccc4`
(Application), `0x3b1fb8 0x3b0ad4 0x3b0c64 0x3b0d78 0x3b0e8c 0x3b0fa0` (CF__Calc* / damage & status rolls),
`0x488134 0x48f954` (rnd generator/rule step), `0x31d194 0x31ce84 0x37b23c 0x319ca8 0x37df30 0x37e2d8` (Lua value
pool, LuaManager, Binder, _Rand/_RandF), `0x4179c8 0x33a590 0x34d124 0x404710 0x34c91c 0x34c98c` (state machine,
GS_InterruptLoading, Gamepad/Keyboard/Mouse input), `0x45ec50 0x460004 0x45d95c 0x45ded0 0x460e68 0x460fe8 0x461114`
(script commands), `0x43e734 0x43d2c8 0x43fdbc 0x43fa58 0x43cb24 0x43f5c8 0x444c68 0x43b2d8 0x43ccec` (natives),
`0x41f548 0x52a504`.

prio 2: `0x37b5a0 0x449e70 0x427d50 0x48ad54 0x43c098 0x44c444`.

Checks that mattered (RNG / timing / input, since a wrong divisor or bound changes every roll):

* `Random::GetRandom` (0x33ff90): `(59051*seed + 177149) % 0xDAF26B`, `abs32(seed % range)`, seed untouched for
  range 0, counter `[1]`/`dword_99F8A8` for the synced stream and `[0]` otherwise — matches the original exactly, and
  every clone's folded body was compared individually (clone.2 = range 99 with the synced flag in r0).
* `LuaScript::_Rand` / `_RandF` (0x37df30 / 0x37e2d8): `GetRandom(max - min) + min` (int) and
  `lo + (float)(int)GetRandom((int)(hi - lo))` — bounds and cast order match.
* `NativeGetLoadingTipStrID` (0x43cb24) inlines one ordinary-stream step; the reconstruction's
  `GetRandom(HintPages::size, false)` is equivalent including the counter behaviour for an empty table.
* `CF__CalcDamage` (0x3b1fb8): mode 0/1 (properties 79/80/97 or 81/82/100, DOT 174/175), mode 2 (with the element
  argument overwritten on the non-DOT path), mode 3 (`out.damage = fixedDamage`), the `>> 8` scalings, the
  `/100` value-132/133 splits, the `s_isBlocked`/`s_isCritical` extras, the second element roll
  (`GetRandom(v28 - v27)` gated on both being >= 0) and the 25/51 mitigation constants all match.
* `CF__CalcSlow/Fear/Push/Hurt/Stun`: the shared `StatusEffect`/`SlowEffectPasses` helpers reproduce
  `a4 == 2 ? prop2 : prop0`, the `5*19 - defence + gate - 5*19 > a3` formula and the slow variant's single-precision
  `+0.125 * 19` terms, with the right property ids per function (188/145+144, 186/142+141, 183/137+136, 182/135+134,
  184/139+138).
* `Application::Update` (0x32ccc4): 2000 ms frame gate, 4000 ms reload timer, `1000/(float)m_dt` FPS, and the
  `avgFrameCount == 10` pacing ladder (`16-v13`, `33-v13`, `50-v13`, clamp to 5 when `v14 <= 4`) match exactly.
* `Gamepad::UpdateFrame` (0x34d124): `AdvanceChannel` reproduces the original's `previous`/`previous_minimum`/
  `previous_maximum` copies and the `previous >= (prevMax + prevMin + 1) * 0.5` flag; `UpdateStick` reproduces the
  axis pairing (x = 2k, y = 2k+1 with the sign flip), `0.65`/`0.95` dead-zone scaling, the 200-tick press stamp and
  the 400-tick repeat window with the `acos(dot) > 0.31416` (same literal) cutoff.
* `GS_InterruptLoading::Update` (0x4179c8): the three device-class branches really are identical code
  (`chunk_012.c:11146-11326`) with the same splash names, so collapsing them is safe; the `firstframe` 0/1/>=2 ladder
  and the language 4 (jp) / 5 (kor) splash match.

## Deliberately NOT reported (documented deviations, assert simplifications, defensive guards)

* `0x31f4f4 Application::InitIPhone`: `std::abort()` instead of the deliberate `STRB` to address 0 — named
  DEVIATION in the source and the ledger.
* All 11 `Random::GetRandom` clone rows: the queue's prio-0 `armdiff:A-clone` mismatches are the documented fold of
  the clones into one definition (source header comment + one ledger row per clone); the folded bodies are
  equivalent, so nothing is reported.
* `0x320774 Application::GetVersionBuild`: the reconstruction adds a `token == nullptr` guard before `atoi`; the
  original calls `atoi(NULL)` (crash) on the INGAME build. Documented in the source comment; assert/crash class.
* `0x44c444 NativeGetQuestDetails`: the reconstruction adds `obj == nullptr` to the early return; the original only
  checks the player character and would dereference a null output object. Defensive guard, no non-crashing
  behaviour change.
* `0x32ade8 Application::_Draw`: the extra `m_stateMachine != nullptr` / `gui != nullptr` guards (the gui one is
  documented as host-necessitated in the source comment); the original dereferences both.
* `0x32ccc4 Application::Update`: the `m_touchScreen != nullptr` guard, and the device timer being replaced by the
  header-only `glitch::os::Timer::getRealTime()` (documented in the source comment).
* `0x4179c8 GS_InterruptLoading::Update`: the `driver == nullptr` guard (documented).
* `0x48f954 rnd::Rule::Impl::Step`, `0x31ce84 Value::allocValueList`, `0x31d194 Value::freeValueList`,
  `0x33a590 StateMachine::_pushState`: hand-rolled STLport storage/pool bookkeeping replaced by `std::vector`
  operations and bounds checks, each with a "Rebuild note" in the source (explicitly out of scope per the brief).
* `0x3b0*` `CF__Calc*`: `a3 - 1` wrap-around kept via `Dec32`, the `>> 31` clamps kept — no deviation.
* `0x32ccc4 Application::Update`: the FPS value is computed as `1000.0f / frameTimeMs` where the original divides a
  `double` literal by a float and stores to float. Same stored type, only a rounding nuance — not reported.
* `0x319ca8 Binder::__methodCallback`, `0x37df30 _Rand`: a discarded `Arguments::operator[](1)` call (no side
  effects; documented in the source) is not repeated.

## Coverage gaps / not matched

* 194 of the 240 prio-1 `const:`/`float:` rows were **not** read (all prio-3 76 rows and 807 prio-4 rows also not
  read). Prepared dumps for 202 of those prio-1 rows sit in
  `dh2-deepseek-audit/_parts/app-cf0.txt` … `app-cf10.txt` (12-27 rows each, original + reconstruction side by side)
  so the work can be picked up directly; batch generator `app_batches.py`, dump tool `dump.py`.
* No address failed to resolve: every row I selected was found in the queue and both spans were readable.
* `0x32a9c8 Application::_CheckGamepad` (926 lines), `0x446afc NativeGetPlayerStats` (1124),
  `0x450b78 NativeGetMultiplayerRoomInfo` (856), `0x445a10 NativeGetSkillDetails` (431),
  `0x44ca5c NativeInvGetItemDetails` (409), `0x44aa28 NativeGetSaveSlotDetails` (405),
  `0x44e5cc NativeGetPlayerHUDInfos` (359), `0x44f27c NativeGetSaveIDsList` (329),
  `0x444168 NativeMerchantGetItemsListForSlot` (323), `0x443b18 NativeGetQuestIDsInRange` (288),
  `0x44d770 NativeInvitePlayer` (280), `0x44b604 NativeGetBuffs` (269), `0x4451ac NativeGetMultiplayerGameRooms` (232),
  `0x490bdc rnd::Rule::LoadFromXml` (278), `0x48fd64 rnd::Path::Impl::OneStep` (267),
  `0x48f47c rnd::ForceBlock::Impl::FilterExits` (224), `0x48cac0 rnd::RoomPool::LoadFromXml` (174),
  `0x490904`, `0x488e50`, `0x489508`, `0x48e8ac`, `0x48f25c`, `0x4435e4`, `0x44e270`, `0x44de4c`, `0x441b60`,
  `0x4408d4`, `0x69f768 PVRTCDecompress`, `0x52ad4c AlgoAStar`, `0x40de48 PlayerLightTweaker::onSetValue`,
  `0x382bdc/0x382040 ZoomHandler::onEvent`, `0x433f04 GetDialogMsgListById`, `0x3374e4/0x337b4c DebugSwitches`,
  `0x479e5c GameEventManager::Load`, `0x459b04 Script_AwardEndGameTrophies` and the remaining `Script_*` /
  `IStreamBase` / `StreamReader` / `sfc::script::lua::*` push helpers: not read (size/time), no verdict.
