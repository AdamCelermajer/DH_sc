# Chapter coverage v36 handoff

Completed this bounded task: reproducible original-source inventory and Chapter 1 acceptance cases. Whole runtime acceptance remains pending.

Files:
- `tools/audit_chapter_coverage_v36.py`: standalone Python CLI, standard library only.
- `reports/chapter-coverage-v36-summary.json`: compact 51-row machine-readable status table; exact byte count and hash in validation receipt.
- `reports/chapter-coverage-v36.md`: human-readable 51-row map/constructor/script/acceptance table and nine Chapter 1 cases, including exact late-game exit and returning entrypoint.
- `reports/chapter-coverage-v36.json`: full source hierarchy, object identities, exact attributes, module contexts, provenance, scripts/conditions/NPC/data/transition references; about 19.8 MB. Read the compact table first.
- `reports/chapter-coverage-v36-validation.json`: independent inventory agreement, deterministic rerun, malformed-input rejection and source snapshots of the main save providers.

Verified source facts:
- 51 design identities: 16 fixed / 35 procedural; 890 distinct XML source files examined.
- SWAMP: 9 module pairs, 195 module objects, 50 Characters, 5 chests. Counts exactly match independent original-object inventory v32. Config + module controllers are counted separately in the per-level total 205.
- Original Chapter 1 Swamp script pair has 55 names. SWAMP_02 has 6; witch cave has 24; troll cave has 8. All authored scriptFile references inspected have existing compiled binary pairs. The `.pyscript` reference is not a literal missing file.
- SWAMP -> SWAMP_02: `_prim_ExitLevelZone_toSwamp02`, destination `SWAMP_02`, entrypoint `0`, gate `IsAfter_Gothicus2Survivors`.
- SWAMP_02 -> SWAMP: `_prim_ExitLevelZone_toSwamp`, destination `SWAMP`, entrypoint `3`. Preserve original identity and entrypoint; sharing swamp art does not merge campaign state.
- SWAMP scripts include original `Swamp_Intro`, `Movement_Tuto`, `CombatTuto`, `Camp_Intro`, `tutorial_treasure`, `chest_tuto`, `cinematic_Tuto_levelUp`, `cinematic_Tuto_potionUse`, `FirstQuest_Start`, and named NPC dialogue sequences. These are registration/acceptance inputs, not evidence of successful execution.

Source gaps remain explicit:
- Current selected loader constructor families lack `TriggerTrap` in Abbey/Crypt candidate sources. Authored `link` appears in several sources and is absent from the original registry: determine original helper/declaration policy before treating it as a new gameplay class.
- Sixteen strict XML diagnostics are recorded. Fifteen rule trees come from existing original TinyXML captures with current-cache source hash verification; original parser errors remain explicit. One MGP duplicate-attribute case is left unresolved by this strict audit.
- Twenty-three rule candidate source references fail this audit's lookup. These are candidate-file observations, not proof a selected native map fails; existing original validity filters, explicit repairs and fallback behavior require separate runtime proof.
- Historical native assembly statuses are joined with receipt hashes and seeds. They are not fresh current-build render or activation results.
- Exact local/global script-name lookup does not find SWAMP authored `OpenWall` and `script_name`, or SWAMP_02 `enterLocation_Swamp` and `script_name`. Resolve original dispatcher/template/inactive-object policy; do not fabricate scripts or suppress errors without source evidence.

Save integration uses existing main source owners:
- `PlayerSavegameV1::location()` exposes three difficulty-specific levels/seeds/current acts, entrypoints and spawn selection. Original named sections are LNAM, LEPT and LUSP.
- `QuestSavegameV1` requires genuinely initialized quest identities; QEST restores regular/volatile owners. Loader consumes those services and stable original object/module identity.
- This evidence establishes saved location/seed/progression fields; it does not establish whole-scene serialization or a chest/trigger persistence policy. Main must expose/prove those policies.

Validation performed: archive SHA identity; exact 51/16/35 catalog dimensions; SWAMP full type-count agreement with independent v32; 55 local script names and matching script payload count; two identical full audit runs; four malformed/truncated names rejected. No engine, emulator, ADB or other session process launched, and no main/menu source modified.

Regeneration arguments: `--cache C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip --receipt-root <private-loader>/reports --output <private-loader>/reports/chapter-coverage-v36.json`.
