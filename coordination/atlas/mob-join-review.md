# Mob join review

Status: read-only source-first review of `tools/atlas/mobs.py`, `mob_sounds.py`, `mob_drops.py`, and the generated `coordination/atlas/general/mobs` profiles. No canonical data was regenerated or changed during this review. This report is the only file written by the review.

## Findings

No row-space mismatch or exact-name lookup ambiguity was found in the reviewed output. A non-mutating in-memory rebuild joined all 448 actor records to exactly one `CharacterProperties` row and one profile by stable row ID and index. The actor names were unique in this source set. All 448 profiles explicitly have `complete: false`; the index and pages describe the result as partial.

The source rows checked were:

| Actor | CharacterProperties row | Authored roots joined |
| --- | ---: | --- |
| SwampKing | 358 | model 99, animation 70, effects 1, sounds row 2, loot 229 `SwampKing_Loot` |
| Witch | 435 | model 113, animation 76, effects 0, sounds row 2, loot 338 `Witch_Loot` |
| Swamp_LizadMan_Type1 | 360 | model 60, animation 34, effects 1, sounds row 2, loot 230 `Swamp_Basic_Loot` |
| RootTroll | 328 | model 80, animation 58, effects 1, sounds row 2, loot 89 `ET_Boss_Loot` |

For each, `actors.json`, its `CharacterProperties` table row, and the generated profile agree on row index and stable row ID. SwampKing, Witch, and RootTroll receive `BOSS_FLAGGED` from the source-backed boss marker. The lizard is `MOB_AI_TYPE_4`. SwampKing and Witch are assigned Act 1 with inference status; RootTroll remains unplaced. These labels do not claim encounter placement.

## Source and behavior boundaries

This is static atlas infrastructure, so there is no direct visual counterpart to the join itself. The existing drop report reuses the Mud Lord encounter at `coordination/v21.7/B155-B156-HANDOFF.md`, frames `f3320–f3440` / source video `20:48`; it explicitly says those frames show a quest-completion banner and do not prove a loot item appeared. No visual/runtime claim is made here.

Sound row spaces are separated correctly. `CharacterProperties.Sounds` indexes the legacy `CharSoundsTable` (`sounds#group0`), registered as `[i][i][i][i]bb` in `port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp:113–114`. `Structs::CharSounds::read` at IDA `0x4eade4` reads `Die`, `Hurt`, `ImpactVsFlesh`, `ImpactVsMetal`, `IsFlesh`, and `IsMetallic`; `Character::GetCharSoundsId` at `0x3a3294` uses row 2 for a negative/out-of-range actor index, then `GetCharSounds` at `0x3a32d0` reads that row. The vectors contain SoundAutoGen ordinals into the distinct registered 638-row `sdd_dungeon_hunter_2_iphone` table, not `Sounds_bak` or XML UIDs. The combat sound consumer handoff (`port/level-world/character_combat_sound_v1.cpp`, source anchor `0x3afee0`) gates playback on attacker/target rows, `MP_MinimalRandoms`, death or positive damage, attacker kind, target material, and nonempty vectors. Thus the sample actors' row 2 `Default` lists being empty means this specific CharSounds combat path contributes no die/hurt/impact sound for them; it does not imply that the actor is silent. Animation, projectile, and script sound sites remain separate and the profiles now union those reached audio candidates.

The drop graph preserves source fields while following loot group-5 fixed/random entries and subloots, group-2 item-list members, terminal item rows, and power-list rows (`mob_drops.py:135–190`). Its consumer boundaries are documented in `mob-drops-report.md`: normal `CharacterLootDropV8::drop_table` resolves victim and killer and admits AI flag bit 4 or bit 2; otherwise the credited actor must be a player or the victim itself. Explicit `drop_explicit_v108` bypasses that wrapper gate. `LootCreationV8::add` checks `MP_MinimalRandoms` before selection, and table/item selection applies separate recursive, probability/weight, player-count, difficulty, class, and item-power rules. The dossier therefore presents authored possibilities, not guaranteed drops or unconditional percentages.

## Graph coverage and limitations

The current `Index.walk()` does follow reached source-file references transitively. `mobs.py:94–124` queues newly reached assets, then consumes every reference whose `context.file` matches that asset. A read-only in-memory closure for the four sample actors included their model BDAE plus referenced textures; the SwampKing model section includes `swampking.bdae` and four texture atlas files. This confirms the updated asset queue addresses the earlier concern that asset-internal references were only listed.

The traversal still records script associations rather than proving actor execution. Includes are followed only for an exact string and unique script stem; dynamic or ambiguous includes remain gaps (`mobs.py:140–166`). Script references can describe conditional/shared branches, as the generated pages say. Boss flag and Act membership are also not a complete scene-spawn graph; RootTroll's `unplaced` status is preserved. Mob pages are a joined atlas view, not a runtime encounter inventory.

## Verification and remaining status

Read-only checks passed: `mobs.coverage_check(ctx)` and `mob_sounds.coverage_check(ctx)` returned no errors; an in-memory `mobs.build(ctx)` audit confirmed 448 unique actor/profile/CharacterProperties joins, exact sample loot-root joins, and `complete: false` for every profile. The reviewed drop roots had no dangling graph gaps or cycles: SwampKing 182 nodes / 58 terminal item rows, Witch 182 / 58, lizard 93 / 30, RootTroll 115 / 42. The existing drop helper report additionally records synthetic dangling-edge and cycle cases; the sound report records truncated-input/boundary failures and invalid ordinal handling.

These are source/data-join and isolated coverage checks. They do not verify gameplay execution, audible output, a real death/drop outcome, or the normal UI/runtime consumer. Those remain integration/runtime verification work, not a gap hidden by the generated profile status.
