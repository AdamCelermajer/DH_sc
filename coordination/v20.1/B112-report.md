# B112 — generic NPC green targeting report

**Ready for fresh read-only review and root integration verification.** User clarified the missing green circle only. The failure is reproduced and the existing B087/B100 implementations restore it in a private frozen executable at both tested aspects. No additional gameplay behavior is needed. Root owns combined production/package verification and keeps B112 open until then.

## Investigation (before behavioral edits, 2026-10-11)

User: "in npc in general we dont get the green targeting". Fresh owner: b112_npc_green_target; root owns integration/review. Private branch codex/b112 at389c8806; no behavior patch made yet.

### Visual evidence
Inspected original P1 v1.0.3 local mp4 through extracted 2fps contact sheets `.local-inputs/v20.1/b112/video/npc-400-4075.png` and `merchant-580-5875.png`, plus existing404–411 sheet and585 merchant still. NPC sequence:404–404.5 hero leaves cage;405–406.5 Hungry Prisoner retains pale-blue ! and green ground circle; dialogue/removal begins approximately407s (independent reviewer correction, review-t406-407.png). Merchant sequence:580–582.5 green ring is already visible under distant merchant at upper-right while Bogwalker dialogue runs;583–585 remains through NEW QUEST banner;585.5 hero approaches merchant with money-bag and ring;586 onward shop opens. Directly observed persistence while distant: green circle is NOT proof of selected target. Input events are not visible.

Inspected supplied shots8 and18. Shot8 Hungry Prisoner lacks ! and green circle. Shot18 merchant lacks bag and green circle. Both match missing status marker assets/path already investigated in B087/B100, not evidence for a separate friendly attack-target ring.

Existing integrated root NPC frames40/120/300/650 were inspected: green circle appears with B087 marker before accept; both disappear after accept. The green part is the ground mesh of quest_marker(.bdae/_blue), whose bounds span ground z2 through ! z568. That fixture does NOT establish mouse NPC selection.

### Logic evidence and admission gates
- Original `Character::UpdateObjectOfInterest`0x3abb9c validates stored OOI each frame; refresh500ms; TargetList mask0x59, frontal Character priority. `_IsCharacterValid`0x4a1ab8 admits merchant bit0x10, registered quest NPC Character+762 via0x40, hostile bit1; follower alone does not satisfy these.
- B088 recovered handoff: Objective_TalkToNPC::Register0x47db34 sets+762, Unregister0x47dbc4 clears. Quest::SetState0x480c78 registers available accept/objectives/completed end slots. B087 marker helper derives these state/objective facts.
- Original NPC ground green circle is part of quest_marker assets driven by `Objective_TalkToNPC::InstallObjectiveMarker`0x47d8e8 / Remove0x47da6c. Merchant `Character::Update`0x3abe98 merchant visible +138 / quest+762 gate grabs effect139 quest_marker_merchant; B100 handoff preserves it. Field+129 is the distinct disabled admission checked by AnimatedFX.
- Separate target circles: Character::InitPost3b5214..5358 loads9 consecutive FX sets; Character::Update3ac758..81c/8b4..8fc resolves last-target first then OOI and calls actual GetInteractionType. Recovered `character_target_marker_v28.cpp` and `reports/target-art-v28/independent-source-review.md` inspected. `target-set-rows.json`: type3 NPC set265 has file=-1, i.e. no separate circle art. `hud_geometry.cpp:30` confirms empty model URI. Adding a new green type3 selection circle would invent behavior.
- Current main UI surface returns no world receiver at main.cpp4754, and uiInput.target_point_requested has no main consumer. SemanticInput can emit it, but only helper live_dispatch consumes it. Thus world mouse targeting is unwired; distinct from green status marker visibility.
- Current quest NPC registry uses static authored position and registered predicate; it omits enabled/alive gates. This is a genuine target eligibility risk but does not yet reproduce B112 itself. Any proposal must use owner/live NPC state without promoting companions to attack actors.

### Expected behavior and minimal fix
NPCs with registered TalkToNPC markers show the authored ! plus green floor circle; merchant non-quest markers show authored money-bag plus green floor circle while alive/enabled. Marker visibility is independent of frontal OOI/pointer selection and follows quest eligibility. Use B087+B100 marker owners/assets, without per-NPC names or invented type3 art. Mouse selection is a deliberate PC input route; any new pointer implementation needs a separately justified bounded scope.

### Uncertainties
Reference v1.0.3 vs ELF/data v1.0.2. Pointer/touch inputs not visible. B100 not yet in this baseline389c8806; B110 owns merchant registry/shop and inherits B100. Marker static meshes do not yet loop original animations; billboard work belongs B105. Exact user intent beyond missing circles in supplied screenshots is not resolved by guessing.

### Focused verification plan (defined before code)
Run accepted-v20 vs frozen fix112 executable with fresh isolated saves/quiet silent desktop. Quest NPC before/after accepting, repeated Space, distant marker vs frontal selection, real10ms mouse press. Merchant comparison using B100/B110-owned path once available. Verify marker persists independently of selection, disappears after objective unregistration, and does not remain for dead/disabled NPC. Capture at1280x720 and1024x768. Keep attack/red marker regression separate; no new friendly attack target.

## Implementation
B087 quest-marker renderer is already in base 389c8806. B100 commits 1093c24c / 7474d2f0 / a66b7f9c were cherry-picked as a **verification dependency** into this private branch (local equivalents ab510e17 / f5d4848f / f4dc2cf7). They apply cleanly and are the only game-source diff. B110 already inherits them and owns the actual merchant/shop merge. Root should take the B112 report/fixture commits separately, without applying the merchant source hunks twice. No CMake, core targeting, map-specific code, attack eligibility, or B105/B109-owned file was changed.

## Verification
Build wrapper `p14_build.ps1 -Name fix112 -Jobs 2`: initial 1,042 steps exit 0; after B100 import, incremental main compilation/link exit 0. Source frozen at f4dc2cf7; executable `.local-inputs/v20.1/b112/dh-foundation-b112-frozen.exe`, SHA256 **B4668D588B99B5BE7FA4D64D60CE74C616E25769D61D754BD78DF946782F5888**. Full PowerShell ctest: 167/168 pass, only permitted `session_skill_binding` fails, 16.19 s; `.local-inputs/v20.1/b112/ctest.log`. Existing quest-runtime and marker-state tests pass. Additional existing OOI/context tests were privately compiled with C++17/O2/Wall/Wextra/Werror/static into `.local-inputs/v20.1/b112/tests/`: both pass; no shared test scratch overwritten.

Normal EXE comparison uses quiet_run, hidden/silent, independent fresh saves/cwds, fixed step 1/60, no campaign cinematic replay. **16/16 jobs exit 0**: accepted v20 and frozen B112, each at 1280x720 and 1024x768, quest NPC near/distant and merchant near/distant. Only seeded starting position is a test aid; OOI, quest registration, Space dispatch and drawing are the normal paths. Summaries `summary-before.json` / `summary-after.json`; args, logs, PPM and inspected PNG frames in `.local-inputs/v20.1/b112/runtime/`.

| Case (both aspects) | Observed private production result |
|---|---|
| Quest NPC pending, near, frame 260 | Accepted v20 has no ring/!. B112 has original green ground circle and pale-blue !; bubble button. |
| Quest NPC pending, distant, frames 260/340 | Green circle/! remains while player is farther away; sword button, no talk. Confirms circle does not depend on OOI selection. |
| Quest NPC accepted via Space 300, frame 340 | Ring/! disappear; NEW QUEST banner and sword button. |
| Same NPC, repeated Space 550, frame 580 | NPC remains visible with no green circle/!; ordinary swing. Exactly one row365 talk/accept in log, so no stale ring or duplicate talk. |
| Merchant near, frames 260/340/580 | Accepted v20 has no bag/ring. B112 displays original money-bag and green ground circle throughout; bubble button, merchant event on Space 300 and 550. Shop is outside this B100-only private build (B110 owner). |
| Merchant distant, frames 260/340 | Money-bag/green circle persists on distant merchant while a different quest NPC also shows its !/green circle. No merchant interaction is logged. |

All near and distant frame260 PNGs plus quest frame580 at both aspects were visually inspected; quest frame340 and merchant frame580 at 1280x720 also inspected. Matched source status/visibility behavior, not pixel-perfect animation. C++ `scene_probe` on all three original marker assets confirms 3 instances / 12 vertices / 6 triangles / 3 ranges, ground minimum z1.99995; quest maximum z568.555, merchant z558.477. The same complete asset root is used on both sides of A/B, proving the accepted-code omission independently of package contents.

## Match statements
- Matches video: green circles are visible with pending quest/merchant markers, persist independent of selected/frontal OOI, and the quest ring disappears on accepting its talk objective. Private frame sequence reproduces these visible outcomes at both aspects.
- Matches IDA: pending quest/merchant FX rule, merchant quest-registration suppression, and absent separate type3 selection effect. Existing general predicate/renderer paths are reused.
- Not verified: marker animation/billboard pixel fidelity (static renderer, B105-owned continuation), generic dead/disabled transition in a normal NPC session, hostile red-ring runtime transition (no enemy rendering change), merged B110 shop path, and final frozen package. World-pointer transport is a separate discovered TODO, expressly outside user's circle-only clarification.

## Clarification and baseline reproduction (2026-10-11)
Root relayed user clarification: **green circle missing ONLY**. Full pointer targeting is outside B112. Marker fixes B087/B100 are the implementation dependencies; B110 inherits B100 and owns merchant/shop integration. No additional friendly target circle will be invented.

The reusable fixture generator `coordination/v20.1/b112_runtime_fixtures.py` starts from the existing normal root NPC startup arguments, replaces assets/position/window/captures, and creates a new per-case working directory and save paths; it does not write accepted packages/saves. All eight accepted-v20 runs exit0 (quiet/silent), two aspects1280x720 and1024x768, near/distant quest NPC and merchant. Accepted executable SHA256 remains8BA1E537D33D4D4FD5B7DD9822C4CAB5080917FE2427A3903AB7C247827E8CCE. Runtime files: `.local-inputs/v20.1/b112/runtime/before-*`, summary `summary-before.json`.

Inspected before quest-near1280x720 frame260: prisoner visible with speech-bubble HUD, no ! or green ring. Inspected before merchant-near1280x720 frame260: standing merchant visible, sword HUD, no bag or green ring. These reproduce supplied shots8/18. Assets point at the same read-only B100 asset root for both comparisons, so missing rings are source-path omissions in acceptedv20, not absence of art in the comparison. Repeated Space300/550 offers no merchant action in acceptedv20.

Additional original anchor consumer inspected: `AnimatedFX::Update`0x492f68 (pseudocode declaration line288795) removes its anchor for virtual+52 dead or anchor+129 disabled. Merchant update checks enabled+138 (raw SetEnable0x33ddc8); visible is +128 via SetVisible0x33dcf8. This corrects the earlier visibility interpretation using the actual setters. Existing renderer actor alive/enabled guards support that visible invariant; full disabled/dead production branch not yet exercised here.


## Required package content and root handoff
Existing primary quest marker `data/3d/interface/quest_marker.bdae` SHA256 E49CD0FB966F3B8F83E0ED1E2865EFDB392E4BC98E42EACC6B68A1508117F10C (19,832 bytes). Add the already required B087 secondary `quest_marker_blue.bdae` SHA256 5CF0E72C691999677D4F92402F81F24BDA71953FEE9DDF049B2AA2BA17766E54 (19,868 bytes) and B100 `quest_marker_merchant.bdae` SHA256 54ACBBCE5269AB28162E0A472FF92ED8F737F8BFD154C012DC9BBF2A2540C417 (21,972 bytes), sourced from `.local-inputs/assets-extra/android/data/3d/interface/`. Preserve their shared original textures. Without the merchant resource no circle/bag can render; without blue the quest renderer falls back to primary yellow !/green circle. Accepted package EXE hash was rechecked unchanged after all 16 isolated runs.

No active tracker or resolved history edited by this worker. Root must retain stable B112 as the user-visible status-circle verification gate linked to B087/B100, integrate B110's dependency once, include marker assets in the new package, and perform final combined production visual acceptance. Follow-up implementation/integration ownership remains root/B110; this report is not release acceptance.
