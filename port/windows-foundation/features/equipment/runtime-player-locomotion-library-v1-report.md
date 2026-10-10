# Runtime player locomotion library V1 audit

## Original consumer and visible invariant

This is a source-data/clip-bank adapter, so its behavior has no independent
screen to render. The supplied original MainMenu still
`.local-inputs/dh2-final-mainmenu.png` shows an equipped Warrior on the original
swamp/statue stage, but a still cannot establish how the Character chooses or
advances walking, running, or equipment-specific idle clips. The feature does
not claim that visual or playback behavior. Its visible invariant is that the
same Character's normal source animation bank must preserve the class's
AnimationTable, equipped ItemTable IDs, state sequence IDs, aliases, `Loop`,
`Type`, phase speed/blend/movement flags, and clip resource references.

The original consumer path is documented in
`port/level-world/reference/live-animation-selection/NOTES.md`, derived from
the local ARM32 library export. `Character::GetAnimStance` (`0x3a53e0`)
derives the equipment stance from the current source equipment predicates and
clamps against `AnimStances/COUNT_IPHONE`. `Character::GetCharAnimTableId`
(`0x3a3228`) chooses the Character's table. `CSIdle::OnFocus` (`0x3c3020`)
chooses the source Idle base plus stance when the `AnimStancedAnim/SL_IDLE`
mask enables it. `CSMove::UpdateType` (`0x3c0f18`) makes the corresponding
Walk/Run decisions using the source list mask; the Android source mask enables
Idle and Walk stance variants but leaves Run at its base. The parenthesized
caller facts matter: the library cannot infer live item IDs, `flag1324`, or the
source constants from a displayed class name.

The Android cache/source trace records mask `210`, stance count `5`, Idle bit
`2`, Walk bit `16`, Run bit `32`; player stance priority is staff `3`, bow `4`,
offhand/dual `2`, effective two-hander `1`, no mainhand candidate `5`, default
`0`, with candidates at or above count clamped to `0`. For Android profile
rows Knight/Rogue/Mage use animation tables `48/50/49`; Idle and Walk roots
can vary by the actual stance while Android Run remains the authored base.
Those are source facts in the notes, not constants substituted by this loader.

## URI resolution and admission

`RuntimePlayerLocomotionLibraryV1::build` consumes the caller's
`AssetCatalog`, `profile_library_uri` (default `actor-profiles-v2.xml`),
profile ID, role, same-character `CharacterVisualConfig`, and current
main/offhand ItemTable IDs. It loads the exact profile record and checks its
AnimationTable and resolved model reference against the source profile and
same visual. It reads the source AnimationTables, animation clip dictionary,
ItemTable and `animations_pycst.bin` through `read_content` from that same
catalog. `content_paths.cpp` applies the existing one-file flattened `data/`
candidate for a requested `data/pydata/*.bin`; AssetCatalog confinement remains
authoritative.

The library forms the requested pair plus each null-slot combination, stable
deduplicates the pairs, and rejects invalid actual item IDs or a caller budget
above the hard eight-program limit before publishing output. Each pair goes
through `resolve_runtime_player_locomotion_v1` and
`build_runtime_player_locomotion_program_v1`. The latter validates reachable
sequence/redirect/phase fields against those same tables and resolves exact
clip URI paths inside that same catalog. Selection returns a shared immutable
program lease; `merge_named_clips` atomically adds the exact receipts to the
caller visual config before CharacterVisual initialization. A path collision,
missing profile/table/clip, source-graph mismatch, or over-budget request
leaves the previous library/configuration unchanged.

The helper does not create or clone a Character, own a clock/animator/FSM,
select random Type-2 variants, mutate Gear, tick movement, or apply camera,
root-motion, pinning, or state-machine transitions. The native menu's own
`MenuMainMenu::Update` (`0x42c1c8`) updates its same `mCharacterToRender`
animator and SceneManager; this helper supplies no replacement for that owner.

## Evidence, test, and limits

Focused verification is the strict LLVM-MinGW C++17 test in
`runtime_player_locomotion_library_v1_tests.cpp`, built by
`run_runtime_player_locomotion_library_v1_tests.ps1` against the caller's
original asset root. It passed against
`.local-inputs/windows-source-clock-v19-preview-8/assets`. The test loads the
actual Knight profile and source tables/constants, proves ordinary/dual/staff
stances `0/2/3`, checks null tuple deduplication, exact Idle/Walk/Run program
fields and named clip receipt/step facts, validates same-config clip merge and
collision atomicity, exercises selection lease lifetime, and checks caller
budget plus missing custom-profile URI failure atomicity. This proves the
source data projection and helper contract; it is not an integrated gameplay,
movement, animator, native menu, or visual acceptance test.

Evidence versions are not fully aligned: the decompilation is from the local
2026-10-07 Android ELF export, while the cache fixture is the v19 source-asset
snapshot and the available gameplay capture set includes a v1.0.3 video while
the recovered target is identified as v1.0.2. The Android constant/table
values are independently present in the supplied source cache, but no
continuous original video sequence has been matched to the library's
ordinary/dual/staff transitions. The caller's live equipment-owner IDs,
same-Character `flag1324`, and normal native animator consumption remain
integration responsibilities. Parent-reported Knight→Rogue→Knight main-menu
clip/pose execution is separate evidence for that current owner path; this
library's standalone pass does not establish it.
