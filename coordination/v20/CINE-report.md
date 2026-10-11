# v20cine report (B068, single cinematic owner) - branch v20/cine

Base v20/integrate 107f2a86. Commits: e41f5d1c (scripted clip unit scale), 90ce8b51 (enemy controller skips animation-only actors), 0829f132 (witch portal FX speed).
Quiet runs only (`quiet_run.ps1`, hidden desktop, DH_AUDIO_SILENT), fresh private saves/cwd per run, normal compiled banks, `--campaign-triggers`, no named-start or condition override. Quest-gated scenes used the existing private CQPG fixtures (rows 8/50/52) from `v20audit`: a controlled fixture, not normal progression.
Reference: P1 `z_Zky7qQdYs`. Run folders/frames are local (`DH_wt/v20cine_work/runs/<name>`); committed evidence: `coordination/v20/cine-evidence/`.

## 1. Witch camera framing (root cause found, fixed)
Investigation
- Visual: P1 945-955 s (4 fps sheets). First shot: Prince LEFT, Rene RIGHT, both large from behind, purple floor ring and red wall ahead, witch rising between them at ~949; dialogue close-up 950-955: Rene left, helmeted Prince right, golden faery glow. Our E8F1 frame: Rene centred, Prince cropped at the left edge.
- Logic: CameraLevel::Update 0x410390: root = camera target anchor (+offset fields 152..160 = 0, HandleOffset gated by byte+36 = 0 by default, HandleCentering only for 2+ local players); applied zoom decays x0.75 to 0 within a few frames, long before our frame 30; PlayAnim 0x40f904 as in the brief. The port rig equals Waypoint_Center (6000,-12000,0) from frame 10 on, clip eye/target match native defaults, FOV 24.616/aspect 1.6678 untouched. The camera was NOT the cause.
- Measurement: the needed shift of Prince/Rene relative to the camera was a radial expansion of exactly 1/0.9 about the shared module origin. The hero clip root (local 373..535, 1278..1420) was drawn at 0.9 of that (CharacterInit 0x3b4d60: scale = base props 12/13 * 0.009 = 0.9,0.9,1, applied to the whole retained visual including the authored cutscene root translation). Drawing scripted-clip poses at unit scale reproduces the reference for the wide shot AND the close-ups (Rene left / Prince right).
- Uncertainty (explicit): the native reason (cutscene exemption / where the scale node sits) was NOT recovered in IDA; the evidence is the 1.0.3 video. Opening and Serpent comparisons are not contradicted but do not discriminate 0.9 vs 1.0.
Implementation: `source_actor_placement.hpp` `source_actor_draw_scale(scripted_clip_pose, scale)` used by the single `sourceActorWorld` lambda in main.cpp (also feeds the source target-node consumers, so bones/anchors stay consistent). Gameplay poses keep the actor scale. Test `source_actor_placement`.
Before/after: `cine-evidence/witch-framing-ref-before-after.jpg` (row 1 reference 945.5/946.5/948/949.4/951.5/955.5 s, row 2 before, row 3 after).

## 2. New findings fixed
- Crash after MothIntro (normal route at the trigger centre): `Foundation error: Live combat: Animation-only actor rejects combat commands`. RuntimeEnemyControllerV1 ran the monster AI for animation-only source actors (SwampKing, Snackman, ... have AI script "monster") and asked CombatSession::request_actor_attack. Fix: `CombatSession::admits_combat_commands` and the controller skips such actors. Verified: same recipe before = exit 1 at frame ~757, after = clean to 1500 frames.
- Intro_Witch portal ring invisible: FX set 121 (`cutscene_dh2_witch_intro_scene_01_portal`) has step speed 0.0 in the APK 1.0.2 table (captured reader 0x4bc504); AnimatedFX::SetSpeed 0x4924b0 applies it to the timeline, which then stays at key 0 where the diffuse-color tracks are black/transparent. Packets/geometry were already correct (floor discs at the WitchSpotter waypoint, column sheet). The 1.0.3 video shows the ring, so this one asset plays at speed 1.0 (`level-world/source_fx_version_overrides_v1.hpp`, test `source_fx_version_overrides`). This is a VERSION DIFFERENCE, not 1.0.2 behaviour; other zero-speed sets (mage mastery skills) are untouched.

## 3. Known B068 items
- Title re-fire: exactly one "Ancient Prison" title in played 4900, skip 900 and the final smoke run. Matches the reference.
- Moth near cage: present (played opening f900 vs P1 89-92 s: yellow moth lower left of the hero close-up).
- Faery flat white vs golden glow: NOT fixed, cause identified. `faeries_02_celeste.bdae` = opaque body + additive body copy + 8-vertex glow quad + embedded particle emitters GL_PCloud04/06/07. The runtime logs `Animation channel skipped (target is not a scene node) type=28 target=GL_PCloud*-emitter`: model-embedded particle emitters of retained character/decor visuals are not instantiated (the same gap hides decor dust motes, "Scene has no visible geometry"). The golden glow in the video is those emitters; needs a generic embedded-emitter owner (FX stream). No faery-specific recolour was added.
- Red wall behind the heroes in the Witch reference (945-949): not reproduced; the portal column sheet is present with alpha fade-in, the wall itself is not identified (candidate: _prim_BlueDoor fire-door model). Open.
- Prince helmet/bandaged arms in the Witch reference are later-game equipment; the fixture has the four starting items. Not a defect.

## 4. Coverage ledger (each scene run once; later code changes that could affect it are listed)
Final frozen EXE `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/v20/cine/dh-foundation-cine.exe`, SHA256 `83fdd91295c6271fd8fddc99c8c15a631b73c4c93c44b0e4029d495336d4b157`. Earlier passes used the build-dir EXE of the stated source state; intermediate hashes were not kept, so only the final EXE hash is a frozen identity.

| Scene (normal bank, trigger) | Source state at run | Frames inspected | Result vs reference | Later change that could affect it |
|---|---|---|---|---|
| Opening played (Swamp_Intro, 4900 f, auto-tap) | after scale fix | sheets 100..3800, 4100..4900 | sequence matches P1 75-205 s (troll/prisoner, cage, moth, halo, lightning, Rene/Celeste dialogue, New Quest); one title; faery white (gap) | none (enemy skip only touches animation-only AI; FX override is set 121 only) |
| Opening SKIP at 400 -> Movement tutorial | frozen EXE (smoke) | 300/600/900 | skip returns, New Quest, tutorial caption, one title, no abort | - |
| Opening with held move input (4900 f) | after scale fix | position log to 4890 | hero held at 0,0,0 during the cinematic, moves only after End | - |
| MothIntro | after enemy fix | 60..960 | moths fly, camera clip, follow return; crash fixed; moths idle afterwards (post-script behaviour of Swamp_MovieMoth AI not established; reference does not show this trigger) | - |
| LizardMan_Intro + CombatTuto | after scale fix | 60..1080 | lizards walk in, tutorial captions, completes; bounded match P1 218-244 s | - |
| chest_tuto | after scale fix | run finished (tutorial_treasure), sheet not viewed | earlier B068 match retained | - |
| Camp_Intro | after scale fix | 60..1080 | tour, Player/Rene plates, return | - |
| LizCaptain_Intro | after scale fix | 60..1080 | captive and captain walk, kill, follow return | - |
| TrollReturn | after scale fix | 60..1080 | troll walks in and roars | - |
| Swamp_Witch_Activate (north seed, rows 8/50=6, 52=3) | after scale fix | 60..1080 | NPC05 visible and framed (`Source condition lifecycle ... deactivate=true enabled=true`), captions correct; Rene/faery absent only because the fixture starts at the position without followers; blue gate FX not shown (I028 area) | - |
| Swamp2_Intro (overlap seed) | after scale fix | captions | interleaves with Witch_Activate in the overlap fixture | - |
| Swamp_Serpent_Intro | after scale fix | 60..1500 | water entry, dialogue, lizards, serpent, return | - |
| Intro_Witch (witch cave centre seed) | final code | 30..400 | framing matches P1 945-955; portal ring visible (`cine-evidence/witch-portal-final.png`); witch rises; dialogue | - |

Unobserved this session: Swamp_Serpent_Defeated, Door_Inspection_South, LizBlood_Activate, Swamp_Camp_Prisoner*, Tutorial_Merchant, cinematic_Tuto_levelUp/potionUse (callers need quest/events/gameplay that seeding does not reproduce), later maps 003-009 (explicit TODO, not normal-route acceptance).

## 5. Build and tests
Build exit 0 (`p14_build.ps1 -Name v20cine -Jobs 3`). ctest: 162/163 pass; only `session_skill_binding` fails (known worktree asset-root failure). New tests: `source_actor_placement`, `source_fx_version_overrides`.
Not touched: PlaySound/StopSound/SaveGame stubs still logged (other agent owns them); main.cpp edit limited to `sourceActorWorld` and an include.
