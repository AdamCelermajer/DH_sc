# v20 release work — 2026-10-11

Requested result: one `windows-source-clock-v20` package, combining Preview 16,
`p16.1/integrate`, completed B068, and `p17/integrate`. No intermediate releases.

## Investigation and ownership

Required handoffs read in order. Main checkout was actually `windows-foundation`
at `b3db5dc0` (the requested `e0f9d23a` is its parent); both are documentation
handoffs beyond the Preview 16 code. Untracked `docs/ida-fidelity-audit-2026-10-11/`
is preserved. `v20/integrate` starts at `2f0030e2`, clean, in `DH_wt/v20int`.

- B068 implementation: fresh `/root/b068_cinematics`, GPT-6.1 Sol medium,
  exclusive `fix068` ownership. All cinematic problems stay in this session.
- Integration: `/root/v20_integration`, GPT-6 Luna high, exclusive `v20int`
  implementation/build ownership. Merge B068 before P17; retain both sides.
- Evidence review: `/root/v20_evidence_review`, GPT-6 Luna high, read-only source
  review with its own report.
- Frozen executable verification: fresh `/root/v20_frozen_verifier`, GPT-6 Luna
  high, waiting for the final package/hash. A/B baseline is accepted Preview 16,
  SHA256 `3ACF50D2A249962156583C3F13CE010443F6AB65A94A7E0DBE423D71CF4D3D1D`.
- Root owns acceptance, package metadata, tracker/history and `publish` push.

## Verification defined before changes

B068 must compare reference frame sequences first, then IDA/recovered logic,
then actual production frames for every authored Act 1 cinematic. Original
timing and animation continuity need sequences. Unknowns remain explicit.

The integration gate is a successful build and full CTest run (only
`session_skill_binding` may fail in a worktree), plus normal runtime tests of
conditions, doors, Space transitions, generated levels and preserved B069-B077
behavior. The frozen package is verified independently against Preview 16 with
isolated saves. Game runs are hidden and silent through `quiet_run.ps1` only.

Package content must include P17 assets required for normal transitions, rather
than relying on scratch overlays. The packager refuses existing destinations
and base packages containing saves; the manifest covers every other shipped
file, including receipts and notes.

## Current status

The B068 logical phase and P17 branches are merged in v20int. Final B068 completion, build/tests,
integrated runtime/visual verification, frozen package, independent verdict,
tracker resolution and remote push are **pending**. No v20 acceptance is claimed.
Red-door flame may be a known issue by the user's explicit release instruction.

## Root intermediate review (not release verification)

B068 logical step `a4f88744`: root inspected the code diff, runtime
`fix068/audit-opening-before/f1500.png`, `audit-opening-rigid/f1500.png` and
`fix068/v20audit/opening.jpg` reference sequence t85..142 s. The restored authored
rigid mesh visibly adds the missing gold halo; source evidence is VisualObject
constructor `0x472a0c`, with collider ancestry excluded. This supports that bounded
change, not a claim of full cinematic fidelity. Root verified that unqualified
trigger contact returns before modifying runtime edge state, and identified a
remaining stale pre-tick position edge when a script teleports/unmarks the actor;
the B068 owner is addressing it using current post-command actor facts and will
test full played/SKIP openings plus genuine free-play re-entry.

Integration investigation found inherited Swamp source-scope URI stays static
through level transitions, and P17 tests had used external asset overlays.
Integration owns current-level scope derivation and production tests; root will
include finite required asset dependencies in the frozen package. Inherited
`RENE_FOLLOW` forced condition remains under source investigation; deleting it
without a source-backed producer would knowingly disable the requested follower
fix. Any unresolved forced gate must be disclosed as a caveat.

The subsequent source review resolved that uncertainty: `RENE_FOLLOW` is authored
condition row 115, op 1 on quest 8 (`Darkwoods_GoToCathedral`), threshold 13.
Authored initial quest state is 0; the authentic predicate is `13 > live_state`.
Population binds the same profile's CQPG before applying it. Package-style tests
will therefore remove the inherited forced condition, verify fresh following and
the false resumed-state branch, then freeze those launcher arguments.

The finite asset plan now contains 264 P17 records plus nine cinematic FX records, all staged under
`.local-inputs/v20/asset-stage/assets` alongside copied Preview 16 assets. This
stage has no release EXE and is only for integration tests. Four Abbey-only art
references remain genuinely unavailable and explicitly documented; source aliases
for `pvr2_env_castle_alpha` and waterfall texture were verified in the resolver.

Core P17 door collision was confirmed unbound, separately from the permitted
red-flame issue. I027 owner is fresh `/root/v20_door_collision` (GPT-6 Luna high)
in `DH_wt/v20doors`, branch `codex/v20-door-collision` from `955d486b`. It owns
narrow door/physical adapters; integration owns main/CMake hooks. Source tracing
requires the actual ApplyMeshBox rectangle, authored collision/presence gates,
and filter changes at terminal door animation events. The production
closed-blocked/open-passage test remains a release gate.

## Combined runtime progress (candidate not frozen)

The staged package-style Swamp startup passed 160 rendered frames with both
inherited overrides removed: current-level root derivation loaded nine scopes,
16 floors and all three doors. The first door geometry startup exposed authored
comma-delimited rotation tuples; integration repaired that parser and reran its
three focused door checks. Closed-blocked/open-passage gameplay remains pending.

Witch Cave generation first exposed missing dynamic MGX/index dependencies,
then the corresponding MVX geometry. The finite plan now includes the index and
all 41 authored MGX/MVX pairs. After copying the 264 P17 records into the stage,
integration's hidden, silent seed-1234 Witch run generated seven modules, derived
the correct rule-root scopes, rendered 160 frames and shut down cleanly. This
supports packaged procedural startup, not full campaign transition acceptance.

Root inspected the B068 valid-trigger Moth and Captain runtime frame sequences
in `fix068/v20audit/trigger-moth/sheet.jpg` and `trigger-captain/sheet.jpg`.
Both moths remain framed during flight and return to authored ground placement;
the Captain sequence shows the victim falling and the camera returning to play.
The owner attributes the corrected result to source scripted animation state 14
and camera rig composition, with no guessed offsets. Exact final committed build,
reference/IDA comparison and combined regression remain pending before closure.

## User steering during integration

The user added the missing Gameloft launch buzz as B078, requested full original
player death/UI/respawn/world-state work (existing I024), and asked to investigate
remembered gate/teleport light effects (I028). Fresh owners are respectively
`/root/b078_gameloft_audio`, `/root/v20_death_flow`, and
`/root/v20_gate_teleport_fx`; reviewer `/root/v20_new_work_review`. Main/core/build
integration remains with `/root/v20_integration`. These extend v20; no final
package has been frozen. Audio checks remain silent and cannot prove audibility.

Door investigation traced the actual Witch `Door_Teleport` compiled sequence:
effect ordinal 132, sound 604, wait, source SetActorPosition to the door waypoint,
wait and repeat effect. The `is_door_closed` trigger gate suppresses processing
in door states 1/3 (opened/opening), rather than proving open walk-through.
Current consumer rejects that field and current Witch test did not activate the
campaign trigger path. Integration must bind the generic source gate and active
level script bank before claiming this original route works. Physical open-cross
attempts that clamp at missing source floor remain disqualified as passage proof.

## Continued integration gates (not acceptance)

The finite additions now total 310 unique package destinations: 267 P17 records,
28 source-derived actor dependencies, ten B068 assets including the original
dialogue speaker table, three teleport dependencies and two authentic Gameloft
movie files. Root validated every source file and duplicate destination hash;
the generated preflight is `.local-inputs/v20/package-assets-preflight.json`.
No package was created. Final metadata must use actual clean source, full CTest
and frozen executable results rather than these intermediate diagnostics.

Normal compiled common/active-level script banks are integrated. Source-derived
actor retention exposed an unused empty SwampKing injury clip and then a cached
target-node pointer invalidated by replacement of the posed scene graph. The
empty generated injury preload is excluded only for visual-only, non-damageable
profiles; the source state metadata remains. Stable node lifetime is being
repaired with pose rollback, foreign-token and reload tests. The failing runs
are diagnostic failures, not accepted cinematic or campaign tests.

The latest Witch first-overlap diagnostic emitted `questZoneEntered` and applied
its quest event once before that unrelated target-cache failure. This verifies
the bound producer in that fixture; it does not prove the complete Witch scene.
Source logical death is now distinct from HP and persisted in the V4 witness
tail. The death screen geometry/timeline has isolated tests, but actual lethal
hit, visible screen, Continue, revive and checkpoint/world restoration still
require production wiring and runtime verification. The authentic launch movie
has silent decoder and lifecycle checks; audibility remains unverified under
the user's silent-test rule. Actual Door_Teleport light-effect capture and all
final frozen package checks remain pending.

## Subsequent diagnostic results

Target-node lifetime fix `c853de9c` passed actual-model pose/reload/foreign-token
tests and the combined normal Camp run (frozen EXE `5D88EA41…`, 1500 frames,
exit 0, cinematic end 768). The next normal-bank eight-case batch on `6845E2EC…`
also exited 0: opening played/SKIP/movement, Serpent, Captain, Witch arena and
two WitchActivate contacts. Captain's source kill produced one reward/event
and End34 with physical retirement. Serpent reached its final PutLimbus commands.
These are intermediate runtime results, not frozen-release verification.
Hero framing in Serpent/Witch still failed: stale player motor facing overwrote
accepted scripted facing. Its source-backed publication fix is awaiting the
focused visual recheck. NPC05 source condition versus reference visibility is
still an explicit discrepancy; no blanket video/IDA match is claimed.

The compiled-bank diagnostic close/real-zone pair on `6559D139…` both exited 0.
At the authored teleport zone, opened BlueDoor suppressed `Door_Teleport`; the
actual compiled `Intro_Witch` command 63 changed it to closing and the ordinary
zone consumer started `Door_Teleport`. Both set132 commands dispatched and the
player moved to authored waypoint `(5515.36,-12643.6,88.1133)`. Root inspected
`i028-compiled-bank-replay-6559/close/f2.png`: localized brightness is visible,
but the still does not establish effect identity or lifetime independently of
lighting. This is diagnostic placement plus an original closure command, not
proof of natural campaign traversal. Natural Intro_Witch closing at frame739
was separately observed. Dense temporal FX and frozen verification remain open.

Root's supplementary full-video black-screen search decoded all 39,947 P1 and
27,482 P2 frames (silent output): only P1 opening 0–2.533333 seconds satisfied
the 95%-black, at least 0.5-second threshold. Logs are under
`.local-inputs/v20/death-video-scan/`. This finds no obvious opaque death-screen
candidate after the logo, but is not proof of absence or an observed death-flow
match. Exact source vtables establish the player OBJS callback is SetVisible,
not SG_Load; checkpoint quests and player profile are separate. Raw caller
assembly `0x396034–0x396044` passes false to CheckpointSave: the zone position is
the checkpoint key, while the actual entering character position is its revive
location. The source fade-from-black helper and selective checkpoint contracts
are implemented; main wiring and true lethal-hit/save/world verification remain
open.

Asset preflight now includes the original Witch portal and explicit gate model/
texture dependencies: 314 unique additions, all source/stage hashes matching.
Scripted PlaySound/StopSound/regular ScriptSaveGame consumers are being wired
from the B068 source audit; regular ScriptSaveGame is not checkpoint saving.
No final package, acceptance, tracker resolution or push has occurred.

## User-requested stop and handoff to Claude

The user stopped implementation and requested committed worktrees and a complete
handoff. Integration WIP is committed at `948f44a8`; B068 at `48c721f8`;
death at `afd9fca1`; gate FX at `60e60e85`; B078 at `a0062e71`. All those
worktrees are clean and their owners report no active game jobs. The latest
FB1EFF2F condition true/false runs each exited 0, but captures are uninspected.
See `coordination/HANDOFF-v20-CLAUDE.md`, the worktree inventory and v20int's
`coordination/v20/INTEGRATION-HANDOFF.md` for exact remaining work. No release
acceptance is implied by this checkpoint. Unrelated external session files
were preserved without committing them.
