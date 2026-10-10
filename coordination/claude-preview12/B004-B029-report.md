# B004 / B029 - target ring/HUD disappears after Space release or skills

Status: IMPLEMENTED AND ISOLATED-TESTED; NOT integrated-verified. The B004/B029 bug stays open until the
integrated EXE is checked in a live window (section 5). One regression in a sibling runner is caused by another
worker's uncommitted `combat_session.cpp` edit (section 4).

## 1. Evidence

### Visual (reference video, v1.0.3)
- Contact sheet `dh2-act1/target-retention-sequence-281-291.png` (20 frames, 4x5, 0.5 s apart, 281.0-290.5 s).
  Observed directly: a living Bogwomp carries the red ring and the "BOGWOMP" name plus red/black HP bar in most
  frames while the player swings the sword, with MISS/damage feedback. Frames at 281.0-281.5 s show no name frame
  around the nearby Bogwomp, so the name/HP frame is not visible in every early sample. The ring and name/HP frame
  appear and disappear together in the sampled frames (observation). Later in the sheet the marker moves to the
  next living enemy after the first is down (observation).
- The sampled frames do not show Space release or skill Post events. Post-skill and post-release timestamps are
  NOT yet extracted (see Uncertainties).

### Logic (IDA, ARM32 `libDungeonHunter2.so`, pseudocode-all.c)
- `Character::UpdateObjectOfInterest` at 0x3abb9c (pseudocode line 126078):
  - OOI slot = `this+0x14a4`; timer = `this+0x14aa` (WORD); refresh flag `this+0x14ad`/`0x14ac` not used here.
  - Per call: validity callback via vtable slot +140 on the stored OOI; if valid and `OOI+129` is set the OOI is
    cleared; if invalid the OOI is cleared (per-frame validation; the slot-+140 semantics are not decoded here).
  - Timer: `timer -= Application::GetDt()`; if timer > 0 return. Otherwise timer = 500, OOI = 0, then
    re-query.
  - Query: `ObjectSearcher::TargetList(this, 1, 0, 1)`, search position = `GameObject::GetTargetPosition(this)`,
    radius = `CharacterDesign.OOI_Distance` (= **200**, `port/script-runtime/reference/design-bindings/cache-constant-inventory.json:968`),
    angle flags `1086918619`, sorter `TargetSorter::_sortFrontal`.
  - Candidate loop: skip self; once OOI is set, a candidate is accepted only if its vtable+144 result
    (interaction type) is 1 and the flag bit is set; otherwise type 0 or 2 accepts it immediately; type 1 accepts
    only when the candidate's `+956` equals `this`; other types continue. At exhaustion the OOI is the last
    assigned candidate (LABEL_28), not necessarily one that matched.
  - Sets `this+5293 = 1` when OOI changed.
- Android-native recovered copy (`port/android-native/.../source_campaign_character_interest_v111.cpp`) confirms
  the same query: OOI_Distance converted to float, arc 2*pi, flags 89, one search per 500 ms refresh.
- `CharacterTargetMarkerV28::update` (per AUTO-TARGET review, `features/combat/auto-target-marker-v1-independent-review.md`):
  `Character+0x40c` (last_target) first, falls back to OOI when last target is null or self; no alive filter.
- Caller: `Character::Update` (pseudocode 126682) calls `UpdateObjectOfInterest` each update.
- Clears that match source and must NOT change: skill `ClearTarget` (BashDown/GroundSlam/JumpKick),
  combo `attack_anim_clear_nonsticky`, `_ClearNonStickyTarget` (0x3d8d70), HUD Update 0x41a780
  held-attack/no-OOI path (`Cmd_Attack(null)`, clear sticky).

### Port reproduction (from frozen-build reports in the independent review)
- Explicit Tab target: red marker/name/HP shown; implicit Space attack/release: none; player `target_id` 0 while
  Bogwomp HP unchanged. The port HUD and ring both read `combatSession->selectedactor()`:
  `main.cpp:2969` (world ring) and `main.cpp:3004` (target HUD), `combat_session.cpp:1790`.

## 2. Expected behaviour and minimal implementation (planned)
- Four values kept separate: combat target (`player->target_id`), last/preferred target
  (`CombatSessionTargetPresentation.last_target` when known), sticky (`stickyPlayerTarget`, unchanged), rendered
  marker (new).
- Rendered marker = last target if known and non-null and not self; else OOI; then gated by eligibility
  (alive, enemy, targetable) at render time. Ineligible -> hidden.
- OOI owner: 500 ms timer, nearest eligible actor within 200 (horizontal distance) of the player, re-derived on
  each refresh.
- Changes to `ClearTarget` / clear-nonsticky / combat_session.cpp: none.

## 3. Changes
All new logic is in my own files; combat state (`ClearTarget`, clear-nonsticky, sticky, `combat_session.cpp`) is untouched.
- NEW `port/windows-foundation/features/combat/object_of_interest_owner_v1.{hpp,cpp}`: pure OOI owner (500 ms timer,
  nearest eligible candidate within 200 horizontal, ties to lower id, owner-change reset) and
  `resolve_rendered_target_marker_v1` (last target if known/non-null/non-self, else OOI; render-time eligibility
  gate; a rejected last target does not fall back to OOI).
- NEW `port/windows-foundation/features/combat/object_of_interest_world_v1.{hpp,cpp}`: read-only adapter over
  `CombatSession` (`world()`, `target_presentation_state()`, `eligible_target`). Exposes
  `update_object_of_interest_v1` and `rendered_target_marker_actor_v1`.
- NEW `features/combat/object_of_interest_owner_v1_tests.cpp` and `features/combat/run_object_of_interest_owner_v1_tests.ps1`.
- `port/windows-foundation/CMakeLists.txt`: one commented `target_sources(dh-foundation PRIVATE ...)` line after the
  `menu_return_v1.cpp` line (the sources belong to the exe, not `foundation_data`).
- `port/windows-foundation/main.cpp` (5 hunks, anchors):
  1. Include after `#include "features/frontend/rich_text.hpp"`: `object_of_interest_world_v1.hpp`.
  2. Declaration `objectOfInterest` after `std::shared_ptr<f::CombatSession> combatSession;` (~line 920).
  3. Tick after `if(!gameplayPaused&&!combatSession->update(gameplayDt,...))throw ... "Live combat: "` (~line 2795):
     `update_object_of_interest_v1(*combatSession, player_id, gameplayDt, objectOfInterest)` when not paused.
  4. Before the world ring block (`// B004/B029: rendered marker = ...`, ~line 2969): `markerTarget` = actor of
     `rendered_target_marker_actor_v1`; ring condition now `if(const auto* target=markerTarget;...)` (was `selectedactor()`).
  5. Target HUD (~line 3004): `if(const auto* target=markerTarget;target&&target->alive())` (was `selectedactor()`).
- Not touched: `combat_session.cpp`, `playable_actor_world.cpp`, `auto_target_marker_v1.*`, the B037 files.

## 4. Tests
Focused runner (new, its own build folder `.local-inputs/b004-b029-ooi-owner-v1-build`):
`powershell -File port/windows-foundation/features/combat/run_object_of_interest_owner_v1_tests.ps1`
Output (final run, exit 0):
```
PASS skill Post cleared last target keeps marker on eligible OOI
PASS unknown last target falls back to eligible OOI after release
PASS dead OOI hides marker immediately (render gate)
PASS dead OOI is dropped by the 500 ms refresh
PASS nearest eligible candidate is chosen
PASS out-of-range OOI kept until refresh (400 ms)
PASS out-of-range OOI replaced by in-range eligible one at 500 ms
PASS sticky/last target has precedence over nearer OOI
PASS ineligible candidate never chosen as OOI
PASS ineligible last target hides marker (no OOI fallback)
PASS self last target falls back to OOI
PASS unknown last target value is ignored (no OOI -> hidden)
PASS first update refreshes immediately
PASS 499 ms: no refresh yet
PASS 500 ms: refresh picks the nearer candidate
PASS t=0 OOI is the nearest in-range candidate
PASS 60 fps steps refresh after about 30 frames
PASS equal distance ties choose the lower actor id
PASS owner change refreshes for new owner
PASS candidate beyond 200 is not chosen
PASS negative dt does not block the first refresh
OOI owner and marker policy tests passed
```
One failure was found and fixed during development: the test fixture left the player as an eligible candidate, so
the owner-change check picked the player (test bug, not owner bug). Fixture now marks the player ineligible, as the
live adapter does.

Mapping to the brief's required cases (all at the pure-policy level, not a live Session):
- Skill Post clears last target, OOI eligible: marker stays on OOI (case 1).
- Space release: same (case 2).
- OOI dies inside the period: hidden immediately by the render gate, dropped at the 500 ms refresh (case 3).
- OOI leaves range: kept until refresh, replaced at 500 ms (case 4).
- Tab sticky: explicit last target survives ticks and beats a nearer OOI (case 5).
- Ineligible actors: never chosen as OOI; ineligible last target hides; self last target falls back; 200 range edge (case 6, 8).

Existing runners (run after my change, same working tree):
- `run_auto_target_marker_v1_tests.ps1`: exit 0, `PASS original target-marker candidate precedence, null/self fallback, and unfiltered source identity`.
- `run_target_facing_regression_tests.ps1`: exit 0, both `PASS` lines (Knight and Rogue).
- `run_target_retention_regression_tests.ps1`: **exit 1** on the final run. Baseline before my edits was exit 0 (`PASS source target/OOI retention across Space release ...`).
  Failure: `Space key-up or source attack completion cleared a living selected target`. The runner's inputs
  (`target_retention_regression_tests.cpp`, `session_source_object_interest_v1.cpp`, `combat_session.cpp`,
  `actor_combat_runtime.cpp`, `playable_actor_world.cpp`, ...) do not include `main.cpp` or any file I added. The
  only change to its inputs in that window is an uncommitted 21-line `combat_session.cpp` hunk
  (mtime 12:48:57, just before the rerun) adding `depart_source_attack` and a Space-release branch: that is the
  B037 worker's edit. I did not reproduce against a HEAD copy (would need a second tree), so this is an inference
  from timing and input list, not a proven bisect. It must be resolved with the B037 owner before integration.

Syntax check of `main.cpp` (exe's real command from `ninja -t commands dh-foundation.exe`, `-fsyntax-only`, no
`-o`, run from `.local-inputs/windows-foundation-build`): exit 0, no diagnostics. Same flags on the two new
feature `.cpp` files: exit 0 each. The exe was not built.

## 5. Uncertainties / not verified
- Post-skill and post-release video timestamps not yet extracted; name/HP frame vs ring in original not confirmed
  after skills.
- `_sortFrontal` order and vtable +140/+144 semantics not decoded; port uses nearest-by-horizontal-distance and
  an eligibility filter (alive, enemy, targetable) in place of the source interaction-type dispatch.
- Source OOI-validity per frame (vtable +140) vs port render-time eligibility gate: port hides immediately on death
  (stricter than the 500 ms period); to be verified in the integrated EXE.
- Explicit Tab target before `last_target_known` is observed: the marker falls to the OOI instead of the chosen
  target. The projection sets known on the first source sync; confirm in the EXE that Tab shows the Tab target.
- The HUD (name/health frame) now follows the same rendered actor as the ring. Whether the original frame follows
  the ring after skills is not confirmed by the video (see next).
- Video: only the 281.0-290.5 s contact sheet was examined (observation above). Post-skill and post-release
  timestamps were NOT extracted, so the claim that the ring persists after skills and releases is not
  video-verified. Verifier/fidelity must extract a run after a BashDown/GroundSlam Post and after a Space
  release, and check whether the name/HP frame persists with the ring.
- Real-Session coverage: the pure tests do not drive a `CombatSession`. Integrated checks needed: Rogue/Knight
  BashDown Post then idle (ring should stay on an in-range eligible enemy), Space release with no target, enemy
  death within 500 ms (ring hides at once), Tab then combo boundary (sticky holds). The frozen build DA4C
  reproduction (explicit Tab red ring vs implicit Space no ring) should now show the ring after implicit Space
  if an eligible enemy is within 200.
- Per-frame source validity (vtable +140 on the stored OOI) and the candidate sorter `_sortFrontal` are not decoded;
  the port uses nearest horizontal distance.
- Integration risk: the failing `run_target_retention_regression_tests.ps1` (section 4) must be resolved with the B037
  owner before the next preview. My change does not touch that path.
