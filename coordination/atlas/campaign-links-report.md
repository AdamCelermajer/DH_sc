# Campaign command relationships

This report describes the read-only Original Data Atlas campaign-link stage. A
relationship proves a serialized source link; it does not prove that a branch
ran, a source object was admitted at runtime, or the port reproduces the native
consumer.

## Investigation evidence

- **Source corpus:** `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`, `port/level-loader/command_c1_descriptors_v59.inc`, per-level PyData, and exact scene-referenced Android MGP files. `tools/atlas/table_payloads.py` decodes selected `v2conditions`, `v2quests`, `dialogs`, and `worldmap` payloads from their recovered readers.
- **Object definitions:** the resolver parses each `GameObject` element from MGP files named by a scene's direct module references or procedural choices. Records preserve scene ID, full `.local-inputs` source file, zero-based GameObject ordinal, stable source-definition ID, gametype, and every authored property. A procedural-only module is marked `CONDITIONAL_PROCEDURAL_CHOICE`; presence in the file does not imply that a runtime instance was created. Command consumers that do `ObjectManager::GetObjectByName` are related to a `scene_object_definition`, never relabeled as a runtime instance. Duplicate exact names remain ambiguous.
- **Character source properties:** the resolver links `charpropsname` to a CharacterProperties row only when the same GameObject has no `char_template` override. A present `char_template` is joined independently to `character_templates`; its presence suppresses any inferred CharacterProperties edge. These are static authored relations, not spawn/admission claims.
- **Command operand consumers:** `Script_SpawnCharacter::Execute` 0x45f400 resolves the authored name with `ObjectManager::GetObjectByName`, converts it to `Character`, and calls `CharStateMachine::SM_SetSpawnState` (`pseudocode-all.c:248020`). `Script_MoveActor::Execute` 0x45f0a8 distinguishes its Character operand at +24 from destination GameObject at +12 (`pseudocode-all.c:247897`). `Script_SetActorPosition::Execute` 0x45e5c8 resolves waypoint +12 and Character +20 (`port/windows-foundation/features/campaign_host/campaign_host.cpp:619-635`). Other reviewed mappings cite their concrete operand consumers in `campaign_links.py`. Static presence does not establish the runtime lookup's result.
- **ExecScript indexes:** `Script_ExecScript::Execute` 0x4607f4 chooses a random field +20 array element when count +16 is nonzero, otherwise field +12; it adds `dword_9A5FE4` only when byte +8 is false, then calls `ScriptManager::StartScript(..., a3, 1)` (`pseudocode-all.c:248640-248674`). `Level::_LoadScripts` first loads `data/pydata/scripts_pyscripts.bin` and its names, then `_LoadScriptFile` transforms the configured level `.pyscript` into a sibling per-level command/name pair and appends it (`pseudocode-all.c:176890-176990`; `ScriptManager::LoadScriptFile` / `LoadScriptFileNames` append rows at 245678-245891). The resolver maps true-flag values as absolute ScriptManager indexes across the global bank and any admitted current-level rows; false-flag values map to scene-local decoded script indexes, with the manager index offset by the global name count. Local targets are only admitted when the scene has a `DONE` per-level name/command pair and the target script row is `DONE`. `StartScript` still applies ID bounds, player-death/online admission, and—because ExecScript passes true—the already-running guard (`pseudocode-all.c:248583-248638`). These source edges do not assert that the call executes.
- **ExecScript uncertainty:** IDA exposes the `dword_9A5FE4` addition and the global-then-level append order, but the recovered pseudocode did not expose the symbol's assignment itself. The local-bank interpretation is consistent with this loader order and the admitted script indexes; that unresolved storage assignment is retained here as a source limitation.
- **Condition operands:** `ObjectBase::DeclareProperties` binds `activate_cond` to ConditionData +144 and `deactivate_cond` to +180 (`pseudocode-all.c:41550-41562`). `TestEnableCondition` (+140) and `TestDisableCondition` (+176) evaluate those condition lists (`0x33e6d4` and `0x33e61c`, lines 41138-41199). Exact authored names join only to unique `v2conditions#group0` rows. The condition table decoder's typed quest/level links remain a separate hop; for example, a condition relation does not itself claim a quest state or trigger outcome. No source-proven direct WorldMap-row operand was identified in this condition path, so no WorldMap edge is fabricated. Names absent from the supplied condition rows stay unresolved.
- **Dialog and table links:** `Script_StartDialog::Execute` 0x460fe8 passes +8 actor, +12 style, and +16 text IDs to `DialogMsg` (`coordination/claude-preview16/OPENING2-report.md:17`); actor +8 indexes `Arrays::DialogActors` (`pseudocode-all.c:220177-220193`). `Script_StartDialogID::Execute` 0x461114 passes +8 to `GetDialogMsgListById` 0x433f04, which bounds-checks `Arrays::Dialogs` and indexes rows by ID (`pseudocode-all.c:220213-220280, 248959-248985`). The operand roles are proven, but text/style/actor payload rows are not all represented by the supplied Atlas tables. Numeric sound, effect, and animation links use their reviewed consumers and exact source table/index joins. `Script_ChangeLevel::Execute` 0x45cba0 compares +16 with `Arrays::LevelList::m_memberNames` before loading the selected level (`pseudocode-all.c:246813-246877`); joins use `levels#group1` and do not substitute scene filenames.
- **Visual evidence:** this stage indexes static source relationships and has no direct visual gameplay counterpart. It does not infer execution, timing, animation continuity, or runtime object admission from a still frame. Existing campaign behavior handoffs remain the visual/gameplay evidence owners.

## Resolver behavior

`resolve(ctx, refs, tables, assets)` returns `(updated_refs, proven_relations)`.
It adds source-backed roles only for reviewed command kind/field pairs, exact
scene-object definitions, table indexes, script-name indexes, or authored
condition names. Object targets carry `related_field_refs`; definition records
retain full source identity and properties. Script target references include
their namespace, target index, and whether runtime selection is random or
single. A missing source row stays unresolved, and duplicate names/index rows
stay ambiguous. `assets` remains accepted for Atlas stage API compatibility;
MGP object names are parsed from the exact scene-referenced source modules.

`global_records(ctx)` returns fully decoded original global scripts plus their
commands. Each script carries `graph_id=campaign:global:<index>` and each
command carries `graph_id=command:global:<script-index>:<command-index>`;
stable source IDs remain in `id`. The records preserve the name-row and command
payload ordinals, both source files, script/command source sites, command
labels, decoded fields, and counts. The API returns no partial bank if either
stream or the recovered command schemas fail bounded decoding. The supplied
Android bank has 15 scripts and 81 commands.

`object_records(ctx, tables)` returns the complete stable-order flat list of
named scene `GameObject` source definitions indexed from exact MGPs referenced
by scene records. Each record retains its scene, full source path, ordinal,
gametype, complete authored properties, membership (direct or conditional
procedural choice), and any explicitly proven static CharacterProperties or
character-template association. It does not create runtime instances or infer
execution. The supplied corpus yields 6,701 definitions across 15 gametypes.

`global_references(ctx, tables)` returns `(refs, relations)` for the global
bank's command fields whose role is covered by the resolver's recovered
consumer mappings. The reference owner metadata uses `scene_id=global` plus
the script/command indexes, so graph ownership can attach to
`command:global:<script-index>:<command-index>`. A global object-name operand
has no unique level scene context and is deliberately left unjoined to MGP
definitions; recognized runtime selector symbols and exact table/index
operands may resolve. This API is separate from `resolve` and does not load or
re-enter the scene-script target resolver. In the supplied corpus it emits 37
proven references; 3 resolve (two `Player` runtime-selector operands and sound
ordinal 127), while the other 34 retain explicit unresolved reasons or
payload-table gaps.

For graph integration, resolved object-command targets use
`type=scene_object_definition`, with `id` stable for scene + full source MGP +
GameObject ordinal; `related_field_refs` points back to the command operand.
The helper also emits association rows whose `source_definition_id` is that
same object-definition ID and whose target is a `type=table-row` with exact
table/index: `character_properties` (`character_properties#group0`) or
`character_template` (`character_templates#group0`). `SCENE_OBJECT_CONDITION`
references carry `source_definition_id`, `source_file`, `object_ordinal`, and
`property`; their exact condition target is a `v2conditions#group0` table row.
Graph should use the definition as the condition-reference owner, rather than
the containing scene. A nested `char_template` prevents the competing
CharacterProperties association.

## Focused verification

Before editing, the focused checks were defined around Mud Lord, Witch Cave,
Darkwood and a later level: exact source object and level target, a repeated
object name, an absent target, script target namespace/admission, and a
`char_template` override. These exercise exact joins and failure/ambiguity
branches without treating a helper run as full Atlas or runtime verification.

- **Investigation:** operand offsets were checked against the generated C1 command reader and the native or recovered consumer listed above. Condition and script namespaces were checked against the native property bindings, script loader, `ExecScript`, and `StartScript` callers/gates.
- **Implementation:** all referenced MGP GameObjects are indexed as source definitions. The Witch Cave `_prim_Waypoint_doorport` is now uniquely linked to a `Dummy` definition in `bossroom_witch_.mgp`, ordinal 20. Mud Lord `_prim_Monster_05_325` remains a `Character` definition with a separate `Swamp_LizadMan_Type3` CharacterProperties edge. Procedural-only source modules are marked conditional.
- **Implementation:** `Script_ExecScript` produced 1,004 proven target references: 891 absolute manager indexes currently targeting global-bank rows and 113 scene-local targets resolved to their decoded source script rows. It adds no execution claim. Exact scene-object condition names produced 472 references: 462 resolve to a unique condition row and 10 remain unresolved; the resolved rows produce 447 activation and 15 deactivation condition-row edges. `DARKWOOD` resolves to `levels#group1[4]`; later `UNDERWORLD_HUB` references resolve to `levels#group1[45]`.
- **Focused checks:** `py -m py_compile tools/atlas/campaign_links.py` passed. `global_records(ctx)` decoded all 15 global scripts and all 81 command payloads, preserving ordinal pairs and graph IDs. `object_records(ctx, tables)` returned 6,701 definitions across 15 gametypes in stable source order; Witch's `_prim_Waypoint_doorport` appears at ordinal 20 in `bossroom_witch_.mgp`, with separate direct-scene and conditional-procedural memberships where two scene records reference that same source file. `global_references(ctx, tables)` returned 37 proven field references, including exact `Player` runtime symbols and sound ordinal 127; it left global character names without a scene context unjoined and preserved unresolved dialog payload IDs. An isolated synthetic source fixture verified two same-name GameObjects stay ambiguous, a missing name remains unresolved, `char_template` suppresses a CharacterProperties join even when `charpropsname` is present, an exact condition name links to its condition row, an absolute manager index selects a global script row or a local row after the global-bank offset, a local index selects the admitted per-level script row, and a non-admitted local pair stays unresolved. The test also asserted Witch's full source file/object ordinal/gametype and the Mud Lord definition/property association.
- **Corpus counts:** with the supplied 9,208 campaign references and table rows, this helper returned 14,308 references and 9,126 proven relations. The added references were 3,624 role-proven dialog operands (their missing payload joins remain unresolved), 1,004 ExecScript targets, and 472 MGP condition references. These are helper-stage counts, not full Atlas regeneration results.
- **Full Atlas regeneration/runtime verification:** integration-owned and not claimed by this report.
- **Remaining gaps:** localized dialog text/style rows remain unresolved; source assignment of `dword_9A5FE4` was not recovered; unknown condition names and any direct WorldMap condition mapping remain unresolved; scripts, procedural modules, conditions, object creation, runtime spawn/admission, and command execution remain conditional/unverified static facts. No claim of end-to-end gameplay fidelity follows from these joins.
