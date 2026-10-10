# CONTAINERS survey (read-only, 2026-10-10) - condensed from the survey agent's final message

## State
Container logic is source-only/isolated-test code; NOTHING is wired into the Windows EXE (no container identifiers in main.cpp, no interact path; population builds actors only; zero container log lines in 9 quiet runs on the Preview 13 EXE; `visuals=33 declarations=61 skipped=28`).
Existing: `features/interactions/session_authored_container_enrollment_v1`, `session_admitted_destructible_v1`, `session_openable_interaction_v1`, `session_destructible_interaction_v1`, `session_source_object_admission_v1`, `session_container_modern_drop_v1`, `source_container_loot_v1`, `session_container_retained_visual_v1`, `world_object_container_state_v1` (7-byte OBJS state in GameSave), `breakable_asset_recovery_v1.json`; level-world canonical_*container*, level-loader catalog_containers_v67.

## Original (IDA addresses)
- `Container::Interact` 0x3a0b38 (skip if state 3/4; sets state 4; state 3 + `activate` clip; no visual -> DoOpen at once; 3D sound), `DoOpen` 0x3a0a98 (`DropLootTable(row.loot, opener, -1, false)` then Lua `OnOpen`), `__EventCallback` 0x3a0c64 (`opened`), Serialize 0x39f8e0 / Deserialize 0x39f6f0.
- `DestructibleContainer::Interact` 0x3a0da0 (each hit decrements, plays stage clip + sound; at zero DestroyGameObject then base Interact), `__EventCallback` 0x3a1274.
- `ItemObject::DropAndAwardLoot` 0x3ec8a0, `DropLootTable` 0x3ecba0/0x3eccc8, `GameObject::_Summon` 0x39193c (not implemented in EXE), `CheckSpawnProbability` 0x38bd64, `Character::Interact` 0x3a4d78.
- Data: `Swamp_Normal_Chest` row 58 (visual 47, loot 227, sound 33); cave chest loot 223; `Swamp_Normal_DestructibleBarrel` row 30 (visual 70 `go_swamp_urn_breakable`, loot 9, sound 325, script `moth_spawn_container` = 25% Summon(Swamp_Moth_Minions); DealDamages is commented out in the file). plant_spawn_container: two 25% rolls; zombie_spawn_container: 25% InfectedBurned + fire damage.
- Swamp: 5 chests and 9 destructible barrels (no pots/vases in the scanned modules; `UnderworldHub_DestructiblePot` exists on another map). Translated coordinates listed in the agent report (e.g. chest _prim_OpenableContainer_2 (-2198.67, 935.19, 250)).

## Reference video (Part 1, v1.0.3)
196-205 s chest tutorial captions (HUD hidden, chest green/white outline); 205.5-208 HUD returns, player walks to chest, outline stays, bottom-right action button shows a chest icon in range; ~208.5-210 lid rises, light column, sparkle, fully open at 210.0 (about 1-1.5 s); ~211-212.5 floating item label ('Useless Mace') and gold glint; ~213-222 inventory/equipment menus. Camera isometric follow, no cut. Ring colour/tooltip not verified.

## Gaps
No container entity, no interact input, admission not called, clips never played, drops not wired (need Preview 14 DROPS), no save reload exercised, summon/trap absent, targeting range/marker unknown, pots/vases not found, key/trapped chests not investigated.

## Design (portable)
New `features/containers/container_entity_v1.*` (data-driven from declarations); population hook after the actor loop in main.cpp (~918); interaction through the shared nearest-interactable/OOI owner with an abstract press/tap event; animations through the retained clip owner (`activate` -> `opened` marker -> DoOpen -> `idleactive`; destructibles: stage clips -> `opened`); rewards through `session_container_modern_drop_v1` into the DROPS world store; persistence via the existing OBJS component (no schema bump for open state).

## Work breakdown
T1 (M) population + closed visuals; T2 (M) interaction (range, duplicate/out-of-range rejection, marker); T3 (L) open/break timing; T4 (S, after DROPS) rewards; T5 (S) persistence; T6 (L, deferred) summon/trap scripts.

## Open questions for the user
1. Scope: chests and barrels only, or also summon/trap behaviour (moths from barrels)?
2. Interaction: tap on chest, or key press within range? Range value?
3. Open-state persistence via existing OBJS (no schema bump)?
4. Pots/vases in Swamp: scan all modules?
5. Tutorial captions (196-205 s) belong to the cinematic stream.
