# Lane 12: XP and level progression

Status: source implementation reviewed; no integrated build or gameplay verification performed.

## Existing source implementation

The assigned source files already contain a connected, source-derived progression slice:

- `port/level-world/player_progression_v1.cpp` implements level-scaled XP, modified XP (property 201), integer award conversion, `GiveXP`, single-level `LevelUp`, XP carry clamping, authored class recalculation (which derives attribute/skill awards from the real class row), HP/MP regeneration, actual Save delivery, stat-manager lookup, party distance scaling, local XP text routing, and a per-character death receipt.
- `port/level-world/player_xp_text_v1.cpp` composes source victim position/bounds, `anim_sct_xp`, the localized `StrID.GAMEPLAYMENUS_REWARD_XP` format, actual color and enqueue callbacks. It requires the real formatter and queue; no numeric substitute is emitted.
- `port/android-native/app/src/main/cpp/renderer_player_progression_v1.inc` borrows the existing `PlayerGameplayBinding` Gear, property view, save and class cache. It creates no parallel player.
- `renderer_character_progression_bindings_v44.inc` binds actual Character/NPC properties, Level150, DesignSettings, Debug switches, difficulty, Save14e8, PlayerManager stats lookup, real RegenHP/MP, source save writer, and scrolling XP text into `CharacterProgressionWorldV23`.
- `renderer_death_reward_fields_v56.inc` provides the actual player Character+14e8 save borrow and same-player Kill fields used by the existing whole-Kill production path.

The authored class cache and formulas award stat and skill points during base-property recalculation. The implementation intentionally does not add points by hand or recursively level a large award: the original grants one level and clamps carry. Signed XP math, fixed-point property 33/34, difficulty XP, OneKillLevelUp, range gates, party split, and the source's `scaledXP + 1` rounding are retained.

## Original-source path checked

Used the IDA export and assembly under `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so`:

- `Character::Kill` at `0x3a5b18` calls `Character::DistributeXP` at `0x3a5e18` after the source's early DropLoot branch and contributor/credit checks.
- `Character::DistributeXP` at `0x3bf828` enumerates `PlayerManager::GetPlayer(i, true)`, obtains shares from `GetLevelScaledXP` (`0x3bd918`), and calls `_GiveXP` (`0x3bf498`) for nonnegative shares. Local XP display applies `PROPS_GetModifiedXP` (`0x3de7ec`) and calls `F_ApplyScrollingCombatTextXP` (`0x3af0c8`).
- `Character::LevelUp` at `0x3beb88` increments level, clears XP, recalculates base properties from the actual actor/class data, regenerates HP/MP, stores the Save level and saves before running player presentation, then carries at most the source-allowed XP.
- `Character::_GiveXP` applies the maximum-level, player/remote and current-level gates, property-201 modifier, threshold check and stats lookup. `_GiveXP` does not make `IncreaseStat` award points; the class recalculation owns the authored awards.

The existing whole `CharacterKillProductionV23` owner preserves source kill ordering. `CharacterKillRewardsLiveV31` routes only the reached `kill_drop_loot` and `kill_distribute_xp` calls into the existing loot and progression owners. Do not add another death hook or retry ledger.

## Required integration providers

Root owns shared renderer, CMake and application wiring. To activate the path, the integrator must retain `RendererCharacterProgressionBindingsV44` beside the real Kill runtime, call `bind(error)`, pass `owner()` as `RendererKillProvidersV43.progression`, and release it before player/VM/profile teardown. Include the V44 binding only after its combat-text and actual player-manager helper definitions. The lane does not change the shared renderer or CMake files.

The V44 adapter intentionally refuses to claim readiness until it receives:

1. The actual player enumeration and recipient Character borrows. Single-player must come from the real selected-player producer; do not force the player count to one.
2. A real current-Level provider and actual Application/SavegameManager difficulty. Do not replace either with fabricated defaults or Level mode.
3. A complete `level_presentation` provider for the localized `MENU_LEVEL_UP` status/menu path, `PlayerLevelUp`, the same source VisualFX event 87, level-2 tutorial/campaign flags, level-12 `IsSpecTime`, and local `epic_lvl10` through `epic_lvl100` trophies.
4. The actual positive profile Save writer for `SG_Save`; preserve source NULL/profile-disabled early exits. Do not report a successful save for a slot sentinel alone.
5. The existing Kill owner with genuine DropLoot and contributor routing so XP is delivered exactly at `kill_distribute_xp`.

A missing reached provider fails after any already-applied source mutation and before the remaining tail; retain and report that partial prefix. Do not stub missing callbacks as success, invent unlock flags/rewards, reset the receipt except on source Revive/new lifetime, or issue duplicate awards.

## Verification boundary

This handoff records static source/IDA review only. No build, host tests, emulator, APK or gameplay run was performed in this lane. Source delivery does not prove a lethal Swamp kill grants XP or that level-up presentation/save works in the integrated game. Integrated acceptance remains with the root milestone owner.
