# v21.8 MASTER HANDOFF (written when the owner's usage limit ran out)

Resume point for the next session. Read this first, then `AGENTS.md`, then the per-branch reports.

## Owner rules (do not drop)
- Haiku first for implementation/bugs; Sonnet ONLY as escalation after a Haiku agent fails or reports DID NOT WORK. If a Haiku agent spawns helpers it must pass `model: "haiku"` explicitly (the default is Sonnet).
- For every item: investigate the ORIGINAL first (IDA via rea MCP / offline IDA export `.local-inputs/ida-apk-export-2026-10-07/` / original Lua and tables / video notes), then our code, then fix. Mark inferences as inferences.
- One worktree + one branch per task; the lead merges; nothing is pushed. After each agent, report `WORKED / DID NOT WORK` per item to the owner.
- The owner wants EVERY skill (3 classes + 2 subclasses each: Knight -> Berserker, Paladin; Rogue -> Archer, Assassin; Mage -> Illusionist, Necromancer), every weapon type, every faery, verified for sound + animation + effect. The full-code audit runs in a separate Fable 5.1 session: `coordination/v21.8/AUDIT-HANDOFF-FABLE.md` (paste-ready prompt inside), to start AFTER the branches below are merged.
- Never recursive-delete under `.local-inputs` or inside worktrees' `.local-inputs` (directory junctions): remove junctions only with `cmd /c rmdir`. Use PowerShell for `subst`/`New-Item -ItemType Junction`. Run ctest from bash. `D:` is a real disk, never use it as a subst letter. Shared rules file: `C:/dh218/COMMON-RULES.md` (also has "Skill batch rules"); if it is lost, the content is summarised here.
- Stale task-notifications from finished agents keep arriving; stop finished agents with TaskStop.

## State of `windows-foundation` (primary checkout `C:\Users\adamc\Desktop\workspace\DH_sc`)
- `50b6cfaf` "end of act1" (Mud Lord merged, Gothicus->Darkwood load fixed).
- Merged since: character batch 1 (coverage matrix, faery/skill research, staging script), `v21.8-skills-a` (skill use-profile table replacing the 4-name whitelist, RuntimeTimedBuffOwnerV1, Rogue Quickness, ViciousStrike per-marker volleys; CTest 190/190), `v21.8-skillmatrix`, `v21.8-weaponmatrix`.
- Staged v21.8 asset input folder (untracked, local): `.local-inputs/windows-source-clock-v21.8-staged` (made by `coordination/v21.8/stage_assets_v21_8.py`; not a launchable release).

## Unmerged branches (each in `.claude/worktrees/<name>`; each has its own report in coordination/v21.8/ on the branch)
| branch | state at hand-off | next |
|---|---|---|
| `v21.8-mudlord2` (head 64dfe8ef + WIP from a Sonnet escalation) | Review findings #4/#7/#8/#10 done (CTest 187/187); kill->banner->Darkwood WORKED from post-intro seed | Mud Lord damage calibration from `CF__CalcDamage` 0x3b1fb8 and AOE pushback (`SwampKingSkillAOEPushback`), kill from one continuous afterwitch run (B158: hero cannot follow the relocated boss); B159/B160 gaps. Read `mudlord2-report.md` for what the Sonnet escalation reached. |
| `v21.8-mage` (head efe18b3a + Sonnet WIP) | Staff basic attack admitted as ranged; shared projectile executor launches/flies the fire bolt (ProjectileTable row 20); Tab select reproduced once only; no damage on contact; Mage skills not started | contact+damage+impact FX/sound; reliable Tab; stage `elemental_bolt_fire.bdae` + PyData set so `--projectile-source-assets` is unnecessary; ColdRay/ChainLightning/StoneEscape/Cyclone on the skills-a table (merge `windows-foundation` first) |
| `v21.8-faery` (head 4b0b1bf9 + Sonnet WIP) | Registry keyed by faery table script; all five faeries cast with `--test-faery <name|slot>`; Hotty FX bound; no target hits verified; no passive buffs; own-faery model/follow missing | target hits/FX/damage for each faery; own-faery visuals; passive resistance buffs on `RuntimeTimedBuffOwnerV1` (already merged); `sfx_faerie_ice.wav` absent in original data |
| `v21.8-skills-knight`, `v21.8-skills-rogue`, `v21.8-skills-mage-sub` (Haiku skill batches, started late) | WIP; see each branch's `coordination/v21.8/skills-*-report.md` | continue per-skill: profile row in own `runtime_skill_use_profile_<group>_v1.hpp`, real-run proof of animation + FX + sound + effect; escalate to Sonnet only for repeated failures; Archer/Mage projectile skills wait on the Mage executor merge |
Merge order suggestion: mudlord2, faery, mage (resolve the cast-coordinator/marker-drain merge point in main.cpp after `combatSession->update`), then the skill batches; run the full ctest after every merge (expect >=190).

## Open work list (priority)
1. Merge the branches above (review each first with an independent reviewer; Haiku reviewer first).
2. Every-skill verification: `coordination/v21.8/SKILL-VERIFICATION-MATRIX.md` (72 skills: 35 active, 37 passive, 48 subclass-only; 29 actives had no cast path). Runner template `skill_cast_runner_template.ps1`. Passives: none has an effect yet (37). Missing sound links: ChainLightning, Figments, GrapplingHook (WAV candidates exist).
3. Specialization flow (level-12 `IsSpecTime`, class change, SkillList reload) so subclass skills are reachable in the game, not only via test paths.
4. Weapons: `WEAPON-VERIFICATION-MATRIX.md`. Weapon sound id rule resolved (638-row `sdd_dungeon_hunter_2_iphone_pyarray.bin`, port is correct). Gaps: stance attack banks are built once at startup and not rebuilt on equip; Mage/2H/staff/wand/bow attacks not wired in production; no off-hand test flag; shield block unverified; staff projectile (fire bolt) lives on the mage branch.
5. Stun/knockback state for the player-skill tails and Mud Lord pushback (needs an original-backed state machine; not invented yet).
6. Sound: 45 `sounds.xml` names have no file in the original data (wolf/demon/dark-queen/statue/assassin cues, level music banks, etc.); report only, never invent. Nothing has ever been verified by ear.
7. Act 1 with Rogue and Mage end to end; tracker rows B158-B160 and I008.
8. Full code audit on Fable 5.1 (`AUDIT-HANDOFF-FABLE.md`), after the merges.
9. Package v21.8: add the 10 Mud Lord boss files (list in `coordination/v21.7/B155-B156-HANDOFF.md` section 8) plus the staged skill/faery assets via `stage_assets_v21_8.py`.

## Recipes
- Build/test/harness recipes: `coordination/v21.7/B155-B156-HANDOFF.md` section 4 and `coordination/v21.8/skills-a-report.md` (hidden-window method, seeds in `C:/dh217/seeds`, harness `C:/dh217/tools/run-case.ps1`, harness scripts for v21.8 in `C:/dh218/tools/`).
- Tests that read `.local-inputs/<dir>` need junctions to the primary's dirs in each worktree (`publication`, `windows-camera-assets`, `windows-melee-bindings`, ... list in the mage/skills reports).
- Baselines: CTest 190 on windows-foundation after skills-a; `private_save_transport_win32` and `source_level_config` need the `publication` and `windows-camera-assets` junctions.

## UPDATE (session 2, 2026-10-13): all six v21.8 branches are MERGED into `windows-foundation`
- Order: mudlord2, faery, mage (conflicts resolved against the skills-a buff arm), skills-knight, skills-rogue, skills-mage-sub. Full CTest 199/199 after the last merge. Nothing pushed.
- The skill use-profile registry is now `runtime_skill_use_profile_registry_v1.hpp` (`find_skill_use_profile_registered_v1`: shared table, then Knight, Rogue, Mage-sub group headers). A new group = one header + one line there. Profile struct field order (positional init!): ... shield_variant, buff, target_hit_fx_name, external_owner, rolls_combat, fixed_cooldown_ms.
- KEY FINDING: `.local-inputs/windows-melee-bindings/original-melee-bindings.xml` only has base actors (KnightPlayerBase, RoguePlayerBase, MagePlayerBase, KingPlayer). There is NO combat binding (model/animation) for Berserker, Paladin, Archer, Assassin, Illusionist, Necromancer. The prepare step also requires session actor definition_id == CharacterState.class_id. The subclass activation fixtures (Blindness, PoisonSting, DeathClaw, Figments) therefore print `SKIP` (not PASS) in `runtime_skill_activation_session_v1_tests.cpp` until subclass bindings exist. Extending the bindings export (`export.py` in that dir) is the first step for "every subclass working".
- Mage external owner (ChainLightning/Cyclone/StoneEscape rows have `external_owner=true`) is NOT wired in main.cpp yet: casting them reports "needs host-supplied external owner". Staff fire bolt has no contact damage yet.
- Remaining work list in the table above is still valid; the "Unmerged branches" table is now historical (those branches are merged; their reports in `coordination/v21.8/*-report.md` still list next steps).
