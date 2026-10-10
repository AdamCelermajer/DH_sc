# Act 1 remaining-work allocation and live roster — 2026-10-10

This is a staffing allocation, not a claim that every role is running. The accepted build remains Preview9. Recent Preview10 components and captures are development evidence; release acceptance still requires the normal integrated route and fresh checks. The root-readable [reported bug and implementation tracker](../docs/BUGS-AND-IMPLEMENTATION.md) is the single bug-status list; this file tracks owners, capacity and handoffs.

## Actual live roster

| Agent | Current scope | State | Dependency/next handoff |
| --- | --- | --- | --- |
| /root | Shared Session/core and release acceptance | Active | Core source terminal; no blanket compile hold. |
| /root/integration_lead | Windows main/CMake, coherent builds and production caller/UI verification | Active | Owns normal executable integration gates; no CUA. |
| /root/workforce_dispatcher | Tracker, ownership and fresh dispatch followups | Active | Maintains stable IDs, editable user inbox, exact roster and handoffs. |
| /root/b008_profile_models_fresh | B008 selected avatar/equipped weapon projection | Active | Normal captures reveal weapons missing on Knight/Rogue/Mage; patching the feature-owned slot-name mapping, then lead recaptures. |
| /root/workforce_dispatcher/b002_hud_art | B002 Faery/Potion source artwork | Active | Exact SWF/resource investigation and feature-only resolver/tests; no substitute art, lead owns renderer integration. |
| /root/b035_state10_runtime_sol | B035 source Push state-10 runtime consumer | Active | Source-backed modern state10/OnFocus/Blur and physical composition; feature-only, lead owns main/runtime capture. |

**Actual roster: 6 running tasks total (3 feature workers and 3 coordination roles); no independent reviewer is currently running.** I003 enrollment, I005/B023 and I006 feature consumers have finished and moved to handoff status below. B025 failure-prefix hardening is feature-tested and terminal; normal scene enrollment/click capture remains with the lead. B035 feature consumer and independent review pass; a fresh state10/physical implementation is active, while normal capture remains open. B036 label/circle alignment is resolved on the lead-confirmed E826D7CC normal capture; exact Faery/Potion artwork remains B002. I018/I019 menu cue adapter is feature-tested and terminal; audible main integration remains with the lead. B003, scoped B006 crash return, B034 player-death reward suppression, and scoped B036 label alignment are resolved for frozen E826D7CC; Preview9 remains accepted. B019 feature matrix and independent strict review are terminal PASS; normal renderer capture remains lead-owned. B007/B032 and other feature fixes remain normal-executable gates. B026 is resolved only for frozen 227F; Preview9 remains the only accepted release.
## Completed feature handoffs awaiting integration

- Frontend creation/profile: actual 13-input source route, cancel/refusal byte preservation, remove/recreate and same-profile start pass; lead owns main route and normal executable check.
- Audio menu cues: exact Single Player MenuConfirm and MenuSelect actions have a feature-owned adapter, authentic sample decoding, QPC/focus checks, and strict isolated tests. Lead owns actual callback/output enrollment and audible normal-executable verification. Missing Lizard WAVs remain absent.
- Persistence: separate-process SaveStore schema3/GameSave v1 matrix passes with legacy schema1 unknown-field and atomic-failure cases; lead must compose live profile/world owners.
- Death return: feature return/save helper passes current Session/save tests; no actual in-flight cast in this test, production main wiring and full active-cast rejection integration remain lead gates.
- PC HUD: semantic projection maps physical circles left/middle/right to keys1/2/3; B036 corrected label placement and passed multi-viewport layout tests. Potion count projection and potion-use provider also pass focused tests. Lead still owns normal viewport/render/hit and action verification; no playable HUD claim until capture passes.
- Target retention: same-session regression passes Space release, next attack, BashDown Post clear/reacquire and death invalidation; source caller/normal UI remains.
- BashDown FX: source root347/FX164 makes 3 mesh and 2 particle packets; duplicate suppressed. Main must register observer in same frame update/render order; no GPU/pixel claim.
- B032 feature fix and independent review are terminal PASS; lead’s normal DA4C direct-preload/first-grant retest is still open.
- B025 urn binder plus test hardening verifies urn RNG advancement, `loot_table==9`, and missing RNG/store rejection before visual enrollment; B030 intro consumer also has independent focused passes. Normal main enrollment/trigger integration remains open.
- I003 authored-scene enrollment is feature-terminal: actual decoded chest/urn declarations pass through the modern enrollment API, including probability-prefix rejection and same-session marker/store flow. Main still must build current authored candidates and provide actual live policy services.
- B005 exact-URI asset resolver test passes against actual effect and animation-decoy bytes; normal asset-root/caller capture remains open.
- B009 lifetime provider guard passes destroyed-session callback test; main page registration/visual route remains open.
- B023 current-Session zone candidate producer joins actual decoded ordered module bounds to the same-player lease, checks inclusive XY containment and leaves visitation unknown. Strict C++17/11 Python tests pass; live Level+36 membership, RoomZone backlink/visited state, camera planes and MapSheet caller remain.
- I006 follow consumer composes the same-session planner and movement executor and follows an actor to its master's updated position. The focused source/asset test passes; production event wiring, actual RENE_FOLLOW/master owner and WarpBehind destination remain.
- B034 focused no-reward fix and independent review pass with production UINT64_MAX player actor; frozen E826D7CC same-process death→Single Player preserves XP and emits no SourceDeathReward while an NPC reward control remains active.
- B007 invalid-slot callback guard passes 91 focused assertions; integration review is active and full route must rebuild against current archive.
- B011 fresh Knight/Rogue test/report is terminal: measured lateral arc matches recovered source order; no fix justified. Normal capture remains open.
- B035 independent review PASS: actual 0x8 no-dispatch and 0x98/0x88 source Push cases pass; production state10, physical effects and normal visual verification remain.

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
















