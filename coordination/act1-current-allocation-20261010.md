# Act 1 remaining-work allocation and live roster — 2026-10-10

This is a staffing allocation, not a claim that every role is running. The accepted build remains Preview9. Recent Preview10 components and captures are development evidence; release acceptance still requires the normal integrated route and fresh checks. The root-readable [reported bug and implementation tracker](../docs/BUGS-AND-IMPLEMENTATION.md) is the single bug-status list; this file tracks owners, capacity and handoffs.

## Actual live roster

| Agent | Current scope | State | Dependency/next handoff |
| --- | --- | --- | --- |
| `/root` | Shared Session/core and release acceptance | Active | Root owns core source and release; no dispatcher edits to combat_session or user-owned saves/process. |
| `/root/integration_lead` | Windows main/CMake, coherent builds and production caller/UI verification | Active | Owns main/build/GUI; DA4C reports 100/100 CTest; no CUA per user. |
| `/root/workforce_dispatcher` | Tracker, ownership and dispatch followups | Active | Maintains actual roster, bug movement and feature integration handoffs. |
| `/root/b007_creation_flow_fresh` | B007 creation/back/remove/recreate bug | Active | Fresh source-backed implementation owner; previous host test remains component evidence only. |
| `/root/b008_profile_models_fresh` | B008 profile/model/label mismatch | Active | Fresh bug owner; exact selected-profile reproduction and focused implementation/test pending. |
| `/root/b009_menu_navigation_fresh` | B009 character-page navigation bug | Active | Fresh bug owner; prove/reproduce page route, implement/focus-test, hand caller contract to lead. |
| `/root/b016_equipment_art_fresh` | B016 equipment text/art fidelity | Active | Fresh bug owner; source-linked Details/label verification and focused result pending. |
| `/root/b018_skill_text_fresh` | B018 empty skill button/label behavior | Active | Fresh owner; verify source labels/empty-cell art separately from saved-slot mapping. |
| `/root/b019_equipment_avatar_fresh` | B019 equipment avatar/item appearance | Active | Fresh owner; source-matched item/class preview regression pending. |
| `/root/b020_skill_level_locks_fresh` | B020 level locks/prerequisite chain | Active | Fresh owner; source/UI lock and chain regression pending. |
| `/root/workforce_dispatcher/b033_targetless_attack` | B033 targetless Attack5 source parity | Active | Fresh owner builds a focused same-session test/proposal; no shared Session/core edit. |
| `/root/workforce_dispatcher/b033_targetless_review` | Independent B033 source/test review | Active | Source proves conditional null-target Attack5 path after gates; awaiting owner artifact to rerun. |
| `/root/bug_gate_review` | Independent B006 reviewer | Active | Restart policy and focus-resume feature reviewed; current runtime/input tests pass 28 checks. Same-process caller/re-entry proof is still needed. |
| `/root/b003_post_skill_attack_fresh` | B003 post-skill Space attack continuity | Terminal | B003-only default runner and independent focused review pass. Normal EXE/controller/key proof remains open; targetless diagnostic is separate. |
| `/root/workforce_dispatcher/combat_anim_audit` | Independent B003 review | Terminal | Default runner passed with current PowerShell call operator; child invocation anchor discrepancy documented. |
| `/root/integration_lead/frontend_focus_resume_luna` | B006 same-process frontend focus return | Terminal | Feature/runtime/input tests pass 28 checks; integration lead still needs to wire caller and prove same-process route. |
| `/root/b004_auto_target_fresh` | B004 auto-target retention behavior | Terminal | Six-branch strict C++17 feature test and independent review pass; lead still needs real last_target/OOI projection and ring capture. |
| `/root/b006_death_restart_fresh` | B006 death→Single Player policy helper | Terminal | Helper and focused policy test pass; normal same-process route remains open at frontend focus/re-entry. |
| `/root/integration_lead/act1_loot_luna` | Potion transaction and death rewards | Terminal | Potion and current-vitals/XP feature tests pass; lead owns normal input/caller/render integration. |

**Current concurrency: 13 active agents total, 10 feature/verification workers.** Completed this dispatch wave (not active): Linux x86-64 package/backend, B032 rank-zero preload fix/review, B004 marker precedence helper/review, headless creation/profile route test, authentic MenuSelect cue adapter, profile persistence matrix, death-return helper, PC HUD draw/hit packet provider, potion-use transaction, target-retention regression, Knight BashDown FX packets, B003 implementation/review, and B006 focus-resume implementation. Their feature receipts are ready for lead’s main integration/normal-runtime checks; helper passes do not close active bugs. B003’s default runner passes in the current PowerShell process and remains distinct from the optional targetless Attack5 diagnostic; normal executable proof is open. Preview9 remains the only accepted release. ACB532 and 9C40 remain distinct candidate evidence; do not merge builds.

## Completed feature handoffs awaiting integration

- Frontend creation/profile: actual 13-input source route, cancel/refusal byte preservation, remove/recreate and same-profile start pass; lead owns main route and normal executable check.
- Audio MenuSelect: source ordinal143→UID3 authentic PCM16 WAV decode and no-device fail-closed submission pass; lead must provide action timestamp/emitter and audible runtime path. Missing Lizard WAVs remain absent.
- Persistence: separate-process SaveStore schema3/GameSave v1 matrix passes with legacy schema1 unknown-field and atomic-failure cases; lead must compose live profile/world owners.
- Death return: feature return/save helper passes current Session/save tests; no actual in-flight cast in this test, production main wiring and full active-cast rejection integration remain lead gates.
- PC HUD: semantic projection maps physical circles left/middle/right to keys1/2/3; source-art draw/hit packet provider passes a strict focused test. Main still needs viewport layout, draw-list and `SemanticInput::Surface::hit` wiring; potion provider also passes a strict linked Session test, with main binding outstanding. No playable HUD claim until integration and normal capture pass.
- Target retention: same-session regression passes Space release, next attack, BashDown Post clear/reacquire and death invalidation; source caller/normal UI remains.
- BashDown FX: source root347/FX164 makes 3 mesh and 2 particle packets; duplicate suppressed. Main must register observer in same frame update/render order; no GPU/pixel claim.
- B032 feature fix and independent review are terminal PASS; lead’s normal DA4C direct-preload/first-grant retest is still open.

## Newly tracked production bug gates

| Gate | Evidence and owner | Acceptance still required |
| --- | --- | --- |
| Target-facing while attacking | Source audit shows Attack update and event paths call LookAt; prior combo fixtures were aligned and did not prove facing. `combat_anim_audit` owns a non-collinear Knight/Rogue same-Session regression; root owns the shared Session change. | Focused non-aligned test after root’s core fix, then lead’s coherent normal-EXE check. No aligned fixture can close this gate. |
| Enemy advances after melee until next attack | Newly reported Celeste encounter behavior. `act1_enemy_luna` owns source AIS/RuntimeNavigation/physical timing investigation and a feature fix/test; lead owns the production validation path. | Source-backed update/timer ordering, focused regression, then a normal gameplay capture with no guessed route offset or attack timing. Build and timestamp remain unverified. |

## Capacity plan by workstream

| Workstream | Planned roles |
| --- | ---: |
| Main menu, creation and cinematics | 3 |
| Full character menu | 3 |
| Combat, animation and movement | 2 |
| Enemy population and AI | 2 |
| Pots/chests, drops/pickup and XP | 2 |
| Quests, NPCs, tutorials and cinematics | 2 |
| Companions | 1 |
| Skills, weapon effects and projectiles | 2 |
| Audio | 1 |
| Bosses and world barriers | 1 |
| Save regressions | 1 |
| Independent fidelity/playthrough | 1 |
| **Total planned feature/verification roles** | **21** |

These are capacity assignments. The live roster above is the only active-count claim.

## 21 bounded roles

| Slot | Allocation group / scope | Owner to reactivate | Current evidence/status | Next concrete result and dependency |
| --- | --- | --- | --- | --- |
| 01 | Frontend profiles and creation | `frontend_luna` | Current-main phase17 slot flow passes: selection, arrows, refusal, recoverable removal, repeated Confirm, create/load/start and metadata. | Fresh same-executable profile/model/label capture and full create→load→return verification. Lead owns the window; retain exact sandbox/profile provenance. |
| 02 | Menu scene, class showcase and camera | `frontend_luna` (terminal audit) | Source/current projection, FOV/aspect, viewport, camera parent and update order agree; current v1.0.2 scene has no geometry at settled hole points. | Decisive follow-up requires a v1.0.3 scene asset or matched native orientation/camera/cursor/viewport receipt. No camera or overlay change is justified by current evidence. |
| 03 | Pause and main-menu return flow | `act1_companions_luna` | Source pause page/action helper and Debug-hidden behavior are tested; save-guard audit says Level +0x130 is load-step, not a level kind. | Fresh current-main Escape→pause→Continue and Yes/No return-path capture with exact save behavior. Lead owns GUI; no process-exit substitution. |
| 04 | Equipment/inventory page and actions | `equipment_luna` | Details pane, glyph sizing, Auto-equip label/action, enabled Transmute, Rogue gear render and unequip→auto-equip pass; semantic-vitals regression passes. | Extend normal same-character checks to durability, other classes/items, drop/pickup and reload; keep source gear recurrence (property164) distinct. Lead owns main/GUI. |
| 05 | Skills page, source training and progression projection | `character_menu_luna` (terminal source audit) | Training/current-next is integrated and tested; prior rank-zero Grey anchor fix is feature-tested. Source `[Active]` color and Grey/lock display match the later capture; base Rogue specialization positions8–15 are intentional placeholders. | Lead’s fresh post-refresh Rogue Skills capture is the remaining visual comparison; no feature mismatch or patch is currently justified. |
| 06 | Faery page and saved spell selection | `faery_menu_luna` | Celest/Mage key4 source cast passes same-Session and current main receives two results; Hotty is correctly Mage list2 row7, not DEFAULT Fake_Hotty. | Verify ordinary selected Faery across save/reload and FX same-owner dispatcher/render delivery. Lead owns key4 callsite; source visual parity is separate. |
| 07 | Quest and Map page composition | `act1_quests_luna` + `act1_map_luna` (terminal adapters) | Exact Quest/Map providers pass registry/source-owner tests. Quest shape106 row and shape82 Activate resolver is source-backed in authored 480×320 stage space; Map intentionally provides family3 only. | Lead wires the pushed-menu symbol stack and viewport-to-stage conversion; canonical Map Level/RoomZone/camera/object providers remain separate source-backed inputs. No fifth Tab or inferred map families. |
| 08 | Player/weapon combat, targets and attack banks | `targetloss_audit` + `death_pose_audit` (terminal consumers) | Rogue JumpKick and same-session profile bank tests pass; projectile sensor/frame/render tests pass; transition consumer covers Attack5/Injury11/Dead12 and Skill6/Cast7 physical branches. | Lead wires production projectile launch/contact and RenderQueue; full OnProjectileHit/F_ApplyResult tails and GameObject.Stop path-target behavior remain. |
| 09 | Movement, physical contact and gameplay camera | root (shared runtime), `bug_gate_review` (terminal tests), `physical_filter_luna` (terminal primitive) | Mage/Lizard overlap quantified; physics-frame, enemy contact, body transitions and absent-body checkpoint save/restore pass. | Physical filter set/reset has a focused pass; lead owns frame/contact wiring and fresh separated Mage capture. No invented collision radius. |
| 10 | Enemy AI, authored population and spawn admission | `act1_enemy_luna` (terminal route-release follow-up) | Source population/monster policy, runtime controller, contact/buff/timer and tracked-target retention tests pass; route-only navigation release is tested. Authored LizardIntro trigger admission remains separate. | Accepted target-intent bridge still needs a source-backed setter at the actual selection event; lead owns production frame/contact wiring. Avoid a duplicate actor/AI registry. |
| 11 | Boss encounter phases and unlock conditions | `campaign_save_writer_luna` (terminal same-Session consumer) | Swamp Escape predicate, plan helper, and same-Session Swamp King phase/timer consumer pass. It uses the source Idle/native non-Skill/!canDive gates, 1500 ms timer and <=75% phase2 latch. | Main must compose frame-begin facts and source actor inputs; typed HP/native/script facts were fixture inputs, and full AI/phase visual behavior is still unverified. |
| 12 | Class skills 1–3 and cast lifecycle | `skill_menu_luna` | BashDown, GroundSlam, Charge and Rogue JumpKick have connected same-session tests; Charge current-heading/120° target regression passes. | Explain frozen PC3 empty-target result with marker heading/per-candidate diagnostics, then verify key1–3 in a fresh main. Do not widen source cone or reuse Pre target list. |
| 13 | Effects and projectiles | `targetloss_audit` + `act1_map_luna` (terminal feature bridges) | Projectile frame/render bridge and CPU OriginalScene mesh/pass packet pass for actual Staff01 row20 BDAE and radial texture; same-NativeWorld sensor producer now passes genuine contact/filter/expiry tests. | Lead wires launch/contact and RenderQueue. Typed CharAI::OnProjectileHit and full F_ApplyResult tails remain separate; no GPU/pixel-equivalence claim. |
| 14 | Rewards, leveling and durable save/reload | `act1_loot_luna` (terminal) | Same-session current HP/MP synchronization before XP, two non-level awards, true source level-up refill, and strict SaveStore reload pass; independent review passed. | Lead reruns a normal production kill→XP/save path on the refreshed coherent build; no new feature worker needed unless that run finds a defect. |
| 15 | Pots/chests and object interaction | `act1_interactions_luna` (active scene collection) | `admit_and_bind_session_openable_v1` covers candidate admission→row-backed Openable initialization→authored open/drop→GameSave state4 silent restore; strict source/C++ tests pass. | Add a retained authored-scene enrollment collection and actual chest/pot tests where source placements exist. Missing live policy providers remain explicit modern inputs; native OBJS import is optional legacy fidelity. |
| 16 | Quest objectives, dialogue and NPC interaction | `canonical_skill_context_luna` | Quest page registry is tested; Celeste dialogue join is explicitly unproven because available target resolves to SisterEllen and no script line join exists. | Route real current quest/NPC interaction only from an authored actor+script/StringID mapping. Crypt02 actor record and source StartDialog mapping are missing. |
| 17 | Cinematics/tutorials and authored triggers | `video_fidelity_audit` | v1.0.3 footage audit shows chest tutorial text; exact touch telemetry and trigger mapping are unavailable. | Match source camera/actor/timing and tutorial/trigger callbacks in a fresh route; identify source version and caller before behavior changes. |
| 18 | Companion AI and Faery follower | `act1_companions_luna` (terminal MoveTo consumer) | Same-Session MoveTo uses real same-actor PF/path bindings, live master ActorId/position and source-order ClearTarget; stop/mask/stale master/lease teardown tests pass. | Main still needs the production frame caller. WarpBehind fails closed until a source-produced destination exists; no continuous-follow claim. |
| 19 | Global audio | `audio_luna` (terminal asset provenance) | Exact Rogue Quickness/Roundhouse BDAEs match the visual-v6 manifest and pass strict decoder tests. Exact Lizard attack/hurt/death WAV bytes remain absent after 11 local APK/OBB archives plus caches/data.save search; source PCM metadata only. | Upstream source bytes or a version-matched package are required to validate playback; no substitutes. Missing-resource diagnostics remain source-honest. |
| 20 | World progression, checkpoints, doors and transitions | `inventory_luna` | Existing GameSave route adapter roundtrips level URI/character/actors/RNG; no native door/LNAM/LEPT/LUSP payload is claimed. | Connect accepted modern transition requests and same captured GameSave to canonical restore; separately decide whether legacy Door/PlayerSavegame parity is required. No guessed gate fields. |
| 21 | Fidelity and integrated regression/release | `bug_gate_review` (completed review), `video_fidelity_audit` | 20-lane ledger and source/frame reviews exist; many helper/runtime passes remain unreleased. | Review each frozen coherent build, current own-window captures and source route; replay Act1 creation→combat→menus→rewards→save/reload. Root alone accepts Preview10/Preview11. |

## Delivery rules

Keep owners exclusive at the file level. Root owns shared core/Session/world interfaces; integration lead owns `main`, CMake, shared builds and live GUI windows. Feature workers work in disjoint feature folders or private copied-archive runners. Reopen a completed worker only for a specific source-backed result and test; do not keep a lane active for status reporting. A feature test, helper, source audit or CPU packet test is not production integration or release acceptance.
















