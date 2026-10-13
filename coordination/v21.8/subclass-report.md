# v21.8 subclass report (branch v21.8-subclass)

Base: windows-foundation @ 0eea5e65. Full CTest on the final source: **200/200** (baseline 199 + `class_family_v21_8`).

## 1. Original investigation

- CharacterTable (`.local-inputs/assets-extra/android/data/pydata/character_properties_*`): the six subclass rows are
  `KnightPlayerBase_Berserker` (264), `KnightPlayerBase_Paladin` (265), `RoguePlayerBase_Archer` (326),
  `RoguePlayerBase_Assassin` (327), `MagePlayerBase_Illusionist` (291), `MagePlayerBase_Necromancer` (292).
- Compared field by field with their base rows (224 fields): ModelFile (`prince_modular`), AnimTable (48/50/49), AI, Sounds,
  Scale_X/Y/Z, ClassID, AIFaction, ModularVisual are **identical**. Only ClassString, ClassDescString, Loot and SkillTree differ.
  So the original subclass actors reuse the base model and clip set. There is no separate subclass model.
- IDA (`libDungeonHunter2.so/pseudocode/003f/003fa330.c`, `ItemInstance::IsEquippableBy`): the item class field selects
  the subclass row by name (Item[34] 2 = Berserker, 3 = Paladin, 5 = Assassin, 6 = Archer, 8 = Necromancer, 9 = Illusionist). A
  subclass is therefore a separate CharacterTable row, and `SG_GetPlayerClass` returns it.
- Strings confirm the nine row names in `strings.jsonl`. The subclass is chosen by the class-spec flow, which is not in this branch.
- Inference: the original actor's visuals come from the base row. This is the reason the bindings reuse the base clip set.

## 2. Bindings (step 2)

- `original-melee-bindings.xml` is now generated with the six subclass actors appended after the existing 25 actors.
  Generator: `coordination/v21.8/export_melee_bindings_v21_8.py`. Without the subclass flag it reproduces the old file
  byte-for-byte (checked).
- Check and manifest: `coordination/v21.8/check_subclass_bindings_v21_8.py` and `subclass-bindings-manifest.json`. The check
  verifies all 25 pre-existing actor subtrees are identical to the backup, that each subclass actor's model/AI/states equal its
  base, and the CharacterTable fields. `--regenerate` reproduces the installed file byte-for-byte. Result: `ok=true`.
- Backup of the old file (not committed, `.local-inputs` is not versioned):
  `.local-inputs/windows-melee-bindings/original-melee-bindings.base4-backup.xml`.
- Staged package copy `.local-inputs/windows-source-clock-v21.8-staged/assets/original-melee-bindings.xml` was an older export
  with a different base-actor structure. It was left unchanged except for six subclass actors cloned from their base actor in the
  same file (ids/propertyRow changed). Backup: `original-melee-bindings.pre-subclass.xml`. The production EXE reads this copy.

## 3. Fixtures (step 3)

`features/generic_skills/runtime_skill_activation_session_v1_tests.cpp`: the SKIP path is removed. Each row now needs its own
binding. The fixture uses the subclass row as its profile (required by the production `definition_id == class_id` check).
Fixes that were needed:
- Blindness: the Lua OnPostSkill_ is empty, so the caster keeps its target (new `post_clears_target` flag).
- Fixture sheets (HP/MP) are resolved at the fixture level, not level 1 (DeathClaw needs level-21 MP).
- Figments: no pre-query and an area use, so no target order is required. Damage is **not** asserted: the base Mage rows
  have Damage_*_Main_Hand = 0, so damage comes from an equipped staff, which the fixture does not equip. Only "applied" is checked.

Results: Blindness PASS (buff 10000 ms), PoisonSting PASS (buff 3750 ms), DeathClaw PASS (2 results), Figments PASS (applied only,
PARTIAL).

## 4. Playable subclass path (step 4)

Production changes (`main.cpp`, `original_class_family.hpp`, `original_actor_properties.cpp`):
- `playable_base_class_row()` maps a subclass row to its family base for presentation lookups (creation class, actor-profiles-v2
  library, locomotion request, attack-bank family check). The actor definition and CharacterState.class_id keep the subclass row.
- `resolve_original_fresh_player` accepts subclass rows through the family rule.
- The startup `--combat-*` policy keyed by the base row is copied to the subclass row. The attack bank's profile is an alias of
  the family library profile under the subclass id.

Test-only tools: `tests/subclass_save_writer_v21_8.cpp` (target `subclass_save_writer_v21_8`, not a CTest) makes one
subclass save from a seed. Saves are in `C:/dh218/subclass-saves/`.

## 5. Real hidden-window runs (dh-foundation.exe, staged package, run-skill.ps1)

Flags for every run: `--move-segment 60:180:1,0 --move-from-frame 60 --attack-start-frame 220 --attack-frames 40`.
Rogue and Mage runs also use `--campaign-skip-frame 20` (the swamp intro cinematic otherwise holds control).
Captures at frames 150 and 230. Action codes from `Timed buff probe action frame=N action=K` (0 idle, 1 move, 2 attack).

| Subclass | Status | Evidence |
|---|---|---|
| Berserker (Knight family) | PARTIAL | Run `sub-berserker-4`: loads as `KnightPlayerBase_Berserker`, HP 165/210 projected, source stance 1 main-hand 1178, skill bank 16 roots. Action 0 -> 1 (frame 60) -> 0 -> 2 (frame 220). Player position changes 203,557 -> 684,765 by frame 150. f230 shows the sword swing pose, but the seed's "Quest Completed" panel covers the view. Model is the base prince_modular. |
| Paladin (Knight family) | PARTIAL | Run `sub-paladin-1`: loads as `KnightPlayerBase_Paladin`, stance 1, action 2 at frame 220. Frames not inspected. |
| Archer (Rogue family) | PARTIAL | Run `sub-archer-3`: loads as `RoguePlayerBase_Archer`, stance 2 (dual dagger), action 0 -> 1 -> 0 -> 2 -> 0. f230 shows the Rogue model with daggers. The bow attack is **not** implemented (Rogue stance is the startup dagger gear). SKIP prompt still on screen. |
| Assassin (Rogue family) | PARTIAL | Run `sub-assassin-3`: loads as `RoguePlayerBase_Assassin`, stance 2, action 0 -> 1 -> 0 -> 2 -> 0. Frames not inspected. Assassin skill casts are not in the EXE (whitelist is 4 skills). |
| Illusionist (Mage family) | PARTIAL | Run `sub-illusionist-3`: loads as `MagePlayerBase_Illusionist`, stance 3 (staff), action 0 -> 1 -> 0 -> 2 -> 0. Frames not inspected. The staff attack is the ranged projectile path; the projectile hit is not verified in this run. |
| Necromancer (Mage family) | PARTIAL | Run `sub-necromancer-3`: loads as `MagePlayerBase_Necromancer`, stance 3, action 0 -> 1 -> 0 -> 2 -> 0. f150 shows the Mage model holding a staff. |

Overall: every subclass now loads as its own class row, binds its own bindings, and plays the idle/move/attack state sequence in a
real run. None is verified visually on a clean frame (the seed quest overlay or the SKIP cinematic prompt is on screen).

## 6. Gaps (not done)

- Real class-spec flow (level 12 choice, class change): not implemented. The saves are created by the test writer.
- Subclass skills other than the four whitelisted skills cannot be cast in the EXE (Blindness, PoisonSting, DeathClaw, Figments
  only run in the fixture).
- Archer bow attack and projectile: missing (Rogue dagger stance is used).
- Mage projectile contact: not verified in these runs.
- Clean visual confirmation of walk/attack clips per subclass: needs runs without the seed overlay or intro.

## 7. Files

Committed: `coordination/v21.8/{export_melee_bindings_v21_8.py,check_subclass_bindings_v21_8.py,subclass-bindings-manifest.json,subclass-report.md}`,
`port/windows-foundation/{main.cpp,original_actor_properties.cpp,original_class_family.hpp,CMakeLists.txt,tests/class_family_v21_8_tests.cpp,tests/subclass_save_writer_v21_8.cpp,tests/original_melee_bindings_tests.cpp,features/generic_skills/runtime_skill_activation_session_v1_tests.cpp}`.

Not committed (untracked `.local-inputs`): the regenerated `windows-melee-bindings/original-melee-bindings.xml`, the staged
bindings copy, and the backups listed in section 2.
