# Other-branch findings — 2026-10-05

Compared current `bbc1e78` with engine branch `e6da25b`.

- **Useful:** camera/frustum producers, GLES pass-state mapping, corrected mesh-cache ownership, and BRES reader/block research. Imported 23 evidence/tool files.
- **Already retained:** Noam main/continuation branches. Adam native-textures remains at the imported baseline.
- **Source fixes:** corrected five audio chunk tags; recovered camera planes, culling and visibility callers. Host tests pass; native wiring remains open. The reader-context helper is still untested.
- **New Adam main `c3ae7973`:** player/HUD, skill/loot owners, menu and loader snapshots. Reused GFNT/viewport source: 8,532/5,200 host comparisons pass with sanitizers. Native skill/loot integration and full enemy pursuit remain open. [Import pins](../reports/branch-audit-2026-10-05/adam-ui-source-import.json).
- **DH2Work:** newer commits add seven startup/roadmap documents; no newer game/engine source changes in that delta.

Verification: 1,733 recorded ELF hashes match (1,430 distinct ranges). All 2,901 historical BDAE rows and 2,577,206 fixup classifications reproduce locally. The local directory's three extra BDAEs are listed separately. Five Python bytecode files were excluded. These are research checks, not completed gameplay.

[Import ledger](../reports/branch-audit-2026-10-05/selective-import.json) · [ELF checks](../reports/branch-audit-2026-10-05/engine-branch-evidence.json) · [Corpus replay](../reports/branch-audit-2026-10-05/resource-corpus-replay.json) · [Project checklist](PROJECT-CHECKLIST.md)

## Adam reconciliation at `c3ae797`

Both repositories refreshed; local camera checkpoint is `41e75b7`. Read the three-session checkpoint and player/UI, item-effects, and player-skills/loot milestones. Those staged snapshots and historical receipts do not validate our selected build.

| Contribution | Decision | Concrete boundary |
|---|---|---|
| GFNT/viewport subset | Already present | Fifteen imported files; current host gates already pass. |
| Property/class/timer kernels | Already present | Equivalent after newline normalization; retain our selected owners. |
| `fresh_inventory_owned_v4` | Adapted; host tested | All 144 owner cases pass with borrowed Character properties and the existing RNG core; 676 draws and 3,584 property aliases checked. Legacy save/inventory owner excluded. |
| `player_equipment_v3` | Superseded | V4 supplies the authoritative equip/remove operations; selecting both would duplicate ownership. |
| `item_gear_properties_v5`, `item_power_tables_v5` | Imported; host/actual ARM64 kernel tested | APK library matches original on 4,037 gear cases and 937 power rows/1,224 properties. Fresh gold matches public fixtures; older parser receipts remain historical. |
| `player_gear_effects_v5` | Adapted; host tested | Same V4 properties; six freshly replayed original-ELF cases/112 steps match. Text/debug are fixtures; skin only exercises null Visual. |
| `item_presentation_v5` | Deferred | Require genuine localization and one AddPower/destruction path on V4 items. |
| `character_player_skills_v3` owner/session | Incompatible wholesale | Its owner contracts would overlap our Prince/VM/property/timer/frame owners. Reuse individual callers through adapters. |
| Skill preparation/selection/update/state queries | Adapted; selected and bounded live | One native Knight VM loads 13 unchanged scripts; eight updates now complete before missing faery info. Full-array returns preserve both usable/active results. Full Player AIS remains open. |
| Saved skills and current-skill callbacks | Adapted; host and bounded live | Sole save owner initialized from live SkillTree; 64 original sessions/1,536 snapshots match. Native 16 rows start at source level0; starter grants/profile load remain open. |
| Temporary skill properties | Adapted; selected and bounded live | Borrow existing Character properties and one shared temporary sheet. Original Bashdown computes mana cost without debiting live MP. Buff groups/full property setters remain open. |
| Player callback membership | Adapted; host/ARM and native preparation tested | Default/OnKill membership uses the selected shared Lua core. Native initialization is bounded skill preparation, not full Player AIS construction/OnInit. |
| Skill cooldown callbacks | Adapted; host/ARM and live fixture tested | Borrow existing slot/timer fields; 545 ARM cases match. Original framework timer expiry/rearm survives native reload/rotation. Full skill use and nontrivial numeric providers remain open. |
| Check/use/session and outer skill dispatch | Adapted; selected host tested | Original caller ordering, full returns, error prefixes and Character identity guards pass. 273 outer-dispatch ARM cases match; native CSSkill/input activation remains open. Buffs deferred. |
| `loot_power_resources_v7` | Imported; host tested | Pins the same V5 power rows; 121 power and 39 quantity lists, 13 guards. |
| `loot_power_creation_v7` | Deferred runtime | Adapt RNG and real text/AddPower services first; not the complete AddLoot/drop/pickup caller. |
| Menu `verified-v39` | Deferred live | Tested partial SWF front screen; missing navigation/save/world lifecycle. Bridge UI as a gameplay-state consumer. |
| Menu `current-source` delta | Deferred | New loading art lacks an exact tested APK receipt. |
| Level-loader snapshot | Useful research | TinyXML/inventory probe; every factory is unimplemented. Inferred aliases are not verified loader behavior. |

The live renderer still has separate combat/animation RNG projections. The shared two-stream core and borrowed Ghost input are selected prerequisites, not a unified live application RNG. Consolidate those producers before powered-loot activation. Lua gameplay and GameSWF AVM1 are separate runtimes; preserve source-required AVM1 player/history lifetimes without duplicating the GameSWF core.

Next live milestone: original Bashdown use damages a real Ghost through the current Prince/FSM/timer/property owners. Finish starter grants/progression, faery info, buffs, mana/targets and SkillCombatRoll; then deliver loot to the sole V4 inventory. Source Ghost pursuit, full skill effects, presentation and pickup remain disconnected.

Prior reconciliation evidence: [six selected game-data suites](../reports/branch-audit-2026-10-05/adam-integrated-host.json), [Player/cooldown/shared-VM host and ARM reports](../reports/branch-audit-2026-10-05/adam-reconciliation-validation.json). Both ABIs contain all nine reused module groups; nine live camera snapshots match original ARM.

Latest integration: [selected host gates, actual APK/source audit and live receipts](../reports/reconstruction-2026-10-05/native-player-skill-checks/validation.json). ARM64/x86_64 compile; API37/x86_64/16 KiB completes eight Knight updates, Bashdown/passive two-result checks, cooldown expiry/rearm and the existing Crypt ambush regression. The same VM/save/timer owners survive reload/rotation. Next required provider is `GetCurrentSpellInfo`. A stronger Ghost-pursuit gate fails and is recorded separately. Full skill combat, physical ARM64 and loot pickup remain open. The published camera APK/source identity is unchanged.
