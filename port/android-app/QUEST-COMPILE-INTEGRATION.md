# Android 17 source quest activation development build

This source reconstruction milestone is not a complete playable game. It has
owned ARM64/x86_64 libraries, targets SDK 37 and aligns ELF/APK libraries at
16 KiB. Original cache assets are supplied separately for diagnostic imports.

APK: `DH2-source-quest-activation-Android17-debug.apk`
Size: 906,147 bytes. SHA-256: `7580405d76546728dc858a88f397a6ee854f91bc0c62483110f7c14d4abc1f2a`.
Package: `local.dh2.sourceviewer`.

## Tested

- Android 17 SDK 37 emulators with 4 KiB/16 KiB pages, both x86_64.
- Actual original population helpers/list traversal/cache lookup/ID accessors,
  level getter, four Compile methods and SetIsCompleted versus host/source ARM64:
  27,516 population and 21,372 compile cases, zero mismatches, 204 cases from 34
  real quest objectives, 3,348 observed completion requests. Original Character
  IDs are supplied resolved; save ID -1 and an observed virtual callback isolate
  completion observers/persistence. The real cache has no clear objectives;
  clear-path fixtures are synthetic.
- 12,000 strict compile/population sanitizer iterations and full runtime normal
  and strict host checks. Both Android emulators pass current runtime selftests.
- 23 imports per installed APK run, preserving earlier data/health/death/counter
  checks and all 64 real quest snapshots. All 34 counted-kill records compile
  against copied world snapshots and use the exact melee formula, two five-point
  hits per diagnostic ten-point actor, owned death events and recorded thresholds.
- Property cached-quantity fallback, template no-fallback, immutable world data,
  invalid argument rollback, four synthetic kill/clear records, level mismatch,
  empty population, original active/required preservation and repeated completion
  on recompile pass. Retained real/synthetic record and world generations survive
  replacement/rejection and collection.
- Original textured character and 25-track walk preview load afterward in the
  same process. Installed APK hashes match; no source-app fatal in either run.

## Development use

Import quest data with the existing control. DH2CreateQuestWorld copies a level,
resolved character-ID list and property quantity cache. GetPopulation takes kind
0 property / 1 template. DH2CreateCompiledQuestObjective takes row, objective,
current, completed, active and world; indexes are zero-based and flags boolean.
CompileAgainst repeats the retained record's original compile decision.
GetProgress exposes active/compiled_record in addition to count/completion.
ConsumeKillEvent remains an already-dispatched handler and does not gate active.
These are authored controls, not original Lua registration. See quest-compile
README for the checked API and native limits.

Original Character/world loading/ID resolution, collection/cache lifecycle,
conditions, automatic event dispatch, markers/observers, persistence/rewards,
loot generation, killer credit/XP, player death, full attack/world/AI,
progression/saves and complete source gameplay remain pending. ARM64 compares
in Unicorn; devices execute x86_64 emulator builds. No Fold7, Android 9, ARM64
hardware or full gameplay equivalence is claimed.

Browser permission denied the release upload. No release was published from this
continuation; the exact upload authorization question remains pending.

Evidence: `questcompile-4k-runtime-validation.json`, `questcompile-16k-runtime-validation.json`,
`../quest-compile/differential-validation.json` and `../lua-runtime/questcompile-*-validation.json`.
