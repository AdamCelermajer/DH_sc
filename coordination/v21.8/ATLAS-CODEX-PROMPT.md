# Original Data Atlas: paste-ready Codex prompt

Purpose: map the original game data and scripts so that nobody has to rediscover where an animation, sound or effect resolves, and so that every boss/level script has a known engine-function readiness. Read-only on game code. Written 2026-10-13.

```
You are building the ORIGINAL DATA ATLAS for the Dungeon Hunter II Windows reconstruction.
Repo: C:\Users\adamc\Desktop\workspace\DH_sc (branch windows-foundation). Create a worktree and work ONLY there:
  git worktree add .claude/worktrees/atlas -b v21.8-atlas windows-foundation
Never push. Never recursive-delete anything under .local-inputs (directory junctions live there).
Read first: AGENTS.md, RECONSTRUCTION-HANDOFF.md, coordination/v21.8/MASTER-HANDOFF.md, docs/BUGS-AND-IMPLEMENTATION.md,
coordination/v21.7/B155-B156-HANDOFF.md (Mud Lord worked example), coordination/v21.8/*-report.md.
You may use subagents (one per category below; each owns its own output files and writes its counts into PROGRESS.md).

WHY: most of our lost time was a reference to an animation / sound / effect that we could not find, or did not know where it
resolves, and Lua boss/level scripts calling engine functions we did not know were unported. The atlas must make both answerable.

SOURCES (read-only):
- EXPLICIT SOURCE ROOTS (compare them, do not treat one as complete):
  * ANDROID data: .local-inputs/assets-extra/android/data (3d, scene, scripts, pydata) and .local-inputs/assets-extra/android/original-cache/data (3d, scene)
  * ANDROID APK analysis pack: .local-inputs/ida-apk-export-2026-10-07/ (HANDOFF-TO-MAIN-SESSION.txt, apk-inventory.json, databases/libDungeonHunter2.so.i64 and
    libStormGLOFT.so.i64 = the IDA databases of the native code, classes.dex)
  * iPHONE data: .local-inputs/assets-extra/ios/data (3d, sounds). THE ONLY AUDIO FILES LIVE HERE (239 .wav + 14 .vxn level-music banks, e.g.
    m_level_darkwood_sfx_*.vxn); the Android dump has no audio files, so every sound reference must be resolved against the iOS sounds folder.
  * iPhone pyarray tables: .local-inputs/items-discovery/sdd_dungeon_hunter_2_iphone_pyarray.bin (+ pyarraynames.bin) = the 638-row sound array
  * There is NO iPhone app binary (IPA/Mach-O) in the repo; only the APK/native Android code is available for IDA. If you find one, say so.
  * Compare android vs ios for every asset kind: files present on one side only, differing contents, differing names/case, and record it in RULES.md.
- .local-inputs/assets-extra/android/data/ : 3d (~5.5k files: .bdae models/clips), scene (levels *.mlx, *.rule.xml), scripts (ai/, level/, objects/,
  skills/ *.luac, ~226), pydata (*_pyarray.bin + *_pyarraynames.bin tables); also assets-extra/ios and original-cache (compare platforms)
- sounds.xml, the 638-row sdd_dungeon_hunter_2_iphone_pyarray.bin (row index; isEvent -> random member, else the sounds.xml filename) and every audio file
- Lua readers: .local-inputs/lua514-source, .local-inputs/lua_read_v13.py (some luac files are plain Lua source)
- IDA: offline export .local-inputs/ida-apk-export-2026-10-07/ (and the rea MCP tools if you have them) for native handlers
- Our code: port/windows-foundation/ (grep to decide port status)

OUTPUT 1 (top priority): SCRIPT -> ENGINE MAP
- For every Lua script: each engine function it calls (PlayFX, SpawnSkillProjectile, SetStunState, TargetListSearch, ... ) with call sites (file:line)
  and the literal strings/ids passed.
- For every engine function: native handler address (IDA), meaning, our implementation (file:line) and status PORTED / PARTIAL / MISSING.
- Roll up per script and per boss/level script a readiness score (ported calls / total calls) and a ranked "implement these engine functions next" list.
- Bosses and custom scripts (swampking_core, witch, rene, madruk*, darkqueen*, edwardboss, flaminite*, roottroll*, sandworm*, winddragon*, balrog,
  assassin*, emissary, resurrectinggod, miniboss, ...) also get a detail section: phases, skills, timers, FX/sound/anim ids, cutscene hooks.

OUTPUT 2 (top priority): REFERENCE RESOLUTION LEDGER (animations, sounds, effects, projectiles)
- One row per REFERENCE SITE: script line, table cell, animation marker/event, item/skill/character row, level rule.
- Columns: site, referenced name/id, kind (anim / sound / effect set / animated effect / projectile / model / script), resolves-to (actual file or table row),
  rule used (e.g. sound row index into the 638-row array, AnimatedEffectTable name vs EffectsTables set, CharAnimTable root + template clip,
  data/3D path normalisation and case), status RESOLVED / AMBIGUOUS / UNRESOLVED, for UNRESOLVED a reason code (FILE_MISSING, NAME_NO_FILE, PATH_CASE,
  ID_OUT_OF_RANGE, NOT_IN_CACHE, OTHER) plus nearest candidates, and port status (does our code resolve it the same way: file:line).
- Record the resolution RULES once, in coordination/atlas/RULES.md, with the evidence for each rule.
- Provide a lookup CLI: `atlas find <name-or-id>` listing every referrer and every resolution of an asset or id.

OUTPUT 3: ACT-ORGANISED VIEWS (generated from outputs 1 and 2, not hand-written)
- coordination/atlas/general/ : game state, characters/classes, skills, faeries, items, projectiles/effects, audio, common scripts, native map.
- coordination/atlas/act_N/ : levels (load chain, spawns, triggers, quests, music), mobs (model, anims, sounds, AI script, skills, loot), bosses,
  act-specific scripts, dialogues. Assets used by 2+ acts live in coordination/atlas/shared/ and are referenced from each act.
  Debug levels (100-130) and backups (x02_...) go to coordination/atlas/unplaced/.
- Act boundaries: derive from the level transition chain / level scripts and mark each as INFERENCE until the owner confirms. Act 1 = what our port finished
  (swamp + witch cave, ending at the Mud Lord and the Darkwood load; verify from B157/B155 notes).
- Per-asset detail pages ONLY for act 1 and the next act; the rest stays as generated tables.

COMPLETION RULES (this is how you know how much is done and when you are finished):
1. STEP 0 = enumerate. Before mapping, build the full inventory with totals (scripts, engine functions called, anim refs, sound refs, effect refs,
   projectile refs, asset files, levels, mobs, bosses) and write the counts to coordination/atlas/PROGRESS.md. Every inventory row starts as TODO.
2. Work = move each row to DONE, UNRESOLVED(reason) or NOT-NEEDED(reason). Percent done = rows not TODO / total. Update PROGRESS.md in every commit.
3. `atlas check` (one command, you write it) must exit non-zero if: a script/function/reference has no row; a row has no status; an UNRESOLVED row has
   no reason code and candidates; a referenced id does not exist; a file has no referrer and is not marked ORPHAN. You are finished only when it is green
   and its coverage numbers are printed in the final report.
4. KNOWN-ANSWER TESTS (the atlas must reproduce these or you are not done): 45 sounds.xml names have no file in the original data; sfx_faerie_ice.wav is absent;
   the sound-row rule above; swampking_core.luac and witch.luac call lists match the port's implemented behaviour (Mud Lord worked example); Knight skill rows
   (BashDown, GroundSlam, Charge) resolve anim/FX/sound as in coordination/v21.8/SKILL-VERIFICATION-MATRIX.md. Put these checks in the check command.
5. Resumable: commit after each batch; if you are restarted, read PROGRESS.md and continue from the first non-DONE row. All outputs must regenerate from the
   source data with one command (tools/atlas/regen.* ; python is NOT on PATH in the Git Bash shell here: use PowerShell, C++, or a portable interpreter you
   place under tools/atlas, nothing global).

RULES:
- Original first: derive from data and native code; never invent behaviour. Anything not proven by data or IDA is marked INFERENCE; confidence PROVEN / LIKELY / GUESS.
- Read-only on game code, CMake and tests. Write only under tools/atlas/ and coordination/atlas/.
- Do not check the original data into git (it is .local-inputs, untracked); commit only the generator, the generated atlas and the reports.

FINAL REPORT (coordination/atlas/README.md): per category totals / resolved / unresolved by reason / orphans / ported / partial / missing; the ranked list of
unported engine functions blocking bosses; the ranked list of unresolved references blocking each act; the top 20 gaps for acts 2+. Commit on v21.8-atlas.
```
