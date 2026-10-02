# Recovered kill and clear objective compilation

The source reconstructs two world-population helpers and four original quest
`Compile` methods. It uses owned scalar state and resolved character-ID snapshots;
original Character loading, list/cache lifecycle and full world ownership remain
external. This is a source gameplay component, not a complete playable game.

## Original execution evidence

The differential driver executes the actual original functions:

| Function | ELF address |
| --- | --- |
| Property-ID population helper | `0x47c7f4` |
| Template-ID population helper | `0x47cc20` |
| Property counted-kill Compile | `0x47ed70` |
| Property clear Compile | `0x47f070` |
| Template counted-kill Compile | `0x47ee18` |
| Template clear Compile | `0x47ef50` |
| Cached property quantity lookup | `0x3a2f48` |
| Resolved property/template ID accessors | `0x3b3d38` / `0x3b36ec` |
| Current-level getter / SetIsCompleted | `0x31f594` / `0x47ba10` |

All 27,516 population and 21,372 compilation comparisons match original ARM32,
host C and source ARM64, including 204 compile cases from 34 actual counted-kill
records. The cache has no clear objective records; clear paths use synthetic
records. There are 3,348 observed completion requests. Original instructions are
unchanged, including loaded-list traversal, cached-map lookup, resolved ID getters
and level/flag decisions. Native virtual completion is an observed fixture;
save ID -1 avoids external persistence. Source guards and unchanged fields pass.

The loaded list includes null entries and signed-short property/template IDs.
SafeGet name/template resolution is excluded: fixtures supply already resolved
IDs and equal template-name pointers. Property ID -1 unresolved cases are excluded
from native character fixtures. IDs absent from the list can still exercise cache
lookups, including int32 boundary IDs and negative/zero/positive quantities.

Host ASAN/UBSAN checks run 12,000 iterations with recovery disabled. Original
ELF and source/library/test hashes are in `differential-validation.json`.
ARM64 load segments align to 16 KiB; ARM64 execution uses Unicorn.

## Semantics

Population counts non-null loaded entries whose resolved property or template ID
matches. The helpers do not filter dead actors. A positive loaded count takes
precedence over cached quantities. With no loaded property match, the native map
returns its stored signed quantity, or zero when absent. Template fallback checks
comparator function identity; in the original ELF the template/property comparators
are different, so a missing loaded template returns zero.

Counted-kill compilation always copies the recorded required count. It activates
only when level is -1/current, population is positive and required count is positive.
Otherwise it clears active. Clear compilation preserves active on failure; it
preserves required on level mismatch and otherwise stores the world population
even when zero/negative. Positive eligible population activates it. Both paths
call SetIsCompleted and request completion whenever current >= required, including
when already completed. Completed flags accept native byte values 0..255.

The source C context takes an already resolved population. `dh2_quest_population`
computes that population from portable resolved-ID and unique property-cache
arrays, bounded to 4096 entries each. Validation and alias failures preserve
state and outputs. Match IDs/levels/quantities are signed int32.

## Owned Lua controls

- `DH2CreateQuestWorld(level, characters, cache)` copies an immutable snapshot.
  Character entries have signed-short `property_id`/`template_id` and optional
  boolean `present` (default true). Cache entries have `property_id` and signed
  `quantity`; IDs must be unique. Raw table access avoids caller metamethods.
- `world:GetPopulation(kind, match_id)` uses 0 property / 1 template.
  `world:GetLevel()` returns the copied level.
- `DH2CreateCompiledQuestObjective(row, objective, current, completed, active,
  world)` accepts zero-based indexes and booleans, imports native types 0/1/10/11,
  and returns the owned objective plus its compilation result.
- `objective:CompileAgainst(world)` repeats the original decision using retained
  record scalars. Results have `eligible`, `required_updated`,
  `completion_requested`, `newly_completed` and `progress`.
- `GetProgress()` also exposes `active` and `compiled_record`. Existing authored
  counter constructors and `ConsumeKillEvent` remain available. Consume runs an
  already-dispatched event handler; it does not enforce active. Automatic event
  routing/activation filtering belongs to the future world dispatcher.

Objective record generations are retained in userdata environments. Worlds copy
caller input. Compilation allocates the complete result before committing state;
invalid world/arguments and allocation failures preserve progress. These controls
are authored APIs, not recovered original Lua registration.

World collection/load/unload, native ID resolution, conditions, automatic event
dispatch, markers/observers, persistence/rewards, actual loot/killer credit,
complete combat/AI/world loop and complete source gameplay remain unfinished.

## Reproduce

```sh
python port/quest-compile/build.py --host --report port/quest-compile/host-build-validation.json
python port/quest-compile/build.py --ndk /path/to/windows-ndk --report port/quest-compile/android-build-validation.json
python port/quest-compile/tests/differential.py \
  --original /private/libDungeonHunter2.so --oracle port/skin-payloads/build/oracle.so \
  --cache /private/cache/files --host port/quest-compile/build/compile-host.so \
  --arm64 port/quest-compile/build/compile-arm64.so --report port/quest-compile/differential-validation.json
```

Host uses Linux/WSL. Differential execution requires Unicorn and pyelftools.
Addresses use the original ELF zero base.
