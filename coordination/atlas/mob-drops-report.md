# Mob loot graph helper

## Evidence and behavior boundaries

- **Visual evidence:** Reused the Act 1 Mud Lord sequence documented in `coordination/v21.7/B155-B156-HANDOFF.md`: the boss dies during `boss12`; frames `f3320-f3440` show the quest-completion banner, and the handoff compares that banner with the source video at `20:48`. That note does not describe or prove a loot item appearing. The video is therefore encounter context only; it is not evidence that a particular table entry always drops. This helper has no gameplay or UI counterpart.
- **Logic evidence:** `tools/atlas/table_sites.py` decodes `CharacterProperties.Loot` as a `loot_table#group5` index and records the source cell reference. It decodes loot records from `port/game-data/loot_tables_v2.cpp` (schema `RollType`, `NumRandomItemProbs`, `RandomLootEntries`, `FixedLootEntries`, `SubLoots`), list rows from `loot_table#group2`, and item rows from `loot_table#group3`.
- The original consumer path is represented by the recovered source implementation and its native call-site anchors: `CharacterLootDropV8::drop_table` / `drop_explicit_v108` in `port/level-world/character_loot_drop_v8.cpp` wrap source `DropLootTable` `0x3ecba0` and `GetInventoryToDrop` `0x3ecae4`; the class header records that the wrapper is called from the ordered `CharacterKill` drop service. Normal drops require a resolved victim and killer, then admit victims with AI flag bit 4 or bit 2; otherwise the credited actor must be a player or the victim itself. Explicit reward `DropLoot` at `0x3eccc8` bypasses that wrapper gate. `LootCreationV8::add` in `port/game-data/loot_creation_v8.cpp` checks `MP_MinimalRandoms` before selection, then runs `LootTableSelectionV8` (`port/game-data/loot_table_selection_v8.cpp`) and `LootItemSelectionV8` (`port/game-data/loot_item_selection_v8.cpp`). Selection recursively visits fixed entries and subtables; random entries and nested candidates are governed by `NumRandomItemProbs`, NumProbArray lookup, percent-vs-weight interpretation, `InfiniteLootDrops`, and—where applicable—player-class counts. ItemList members also use per-item probabilities. The add path has further item/power/difficulty/player-count gates. These conditions mean a graph edge or positive authored field does not establish an unconditional drop or a standalone probability.
- **Expected behavior:** A mob dossier should expose the exact authored graph from character row to loot rows, item lists, terminal item rows, and power-list rows, preserving every source field and repeated edge. The helper joins rows only; it reports broken references and cycles without inventing runtime outcomes.
- **Uncertainties:** The reused video note does not establish loot appearance. The static graph cannot evaluate debug settings, runtime RNG, difficulty item variants, class composition, current player count, or reward-vs-kill call path. Android source tables are the parsed atlas input; other supplied platform caches may differ. No equality claim across Android/iOS is made here.

## Implementation and API

`tools/atlas/mob_drops.py` exposes:

```python
index = build_index(tables, references)
dossier = build(index, tables, references, root_index)
```

`root_index` accepts a loot group-5 integer index, a group-5 row, or an actor row containing `fields.Loot`. Passing the prebuilt index as `ctx` avoids rescanning the 8,863 decoded rows for each mob. The JSON-ready result contains root/actor identity, unique reachable nodes with copied fields and ordered edges, terminal item records including item-list probability/quantity and complete item fields, `gaps`, and explicit cycle paths. It preserves negative sentinel values in raw entry fields and skips them as absent references. Edge probabilities remain raw source values.

## Focused verification

Before implementation, the focused check was defined as traversal of the known Act 1 roots plus malformed graph fixtures: SwampKing (CharacterProperties 358 → Loot 229), Witch (435 → 338), and swamp lizard (360 → 230) must expose the declared subloot rows and terminal items; dangling references and cycles must terminate and be reported. The check ran against generated `coordination/atlas/data/tables.json` and `references.json` with Python 3. It returned:

| Root | Resolved loot nodes | Terminal item paths | Gaps | Cycles |
| --- | ---: | ---: | ---: | ---: |
| SwampKing_Loot 229 | 182 | 58 | 0 | 0 |
| Witch_Loot 338 | 182 | 58 | 0 | 0 |
| Swamp_Basic_Loot 230 | 93 | 30 | 0 | 0 |

SwampKing reached group-5 rows `229, 235, 234, 236, 237, 302`; Witch reached `338, 235, 234, 236, 237, 303`; the lizard reached `230, 233, 301`. A synthetic two-row cycle plus dangling item-list ID 99 returned two nodes, one cycle path, one dangling edge, and valid JSON. This is isolated helper verification only; root/CLI integration and integrated UI/gameplay verification remain unperformed and are owned by the integration lead.
