# B112 — generic NPC green targeting report

## Investigation (before behavioral edits, 2026-10-11)

User: "in npc in general we dont get the green targeting". Fresh owner: b112_npc_green_target; root owns integration/review. Private branch codex/b112 at389c8806; no behavior patch made yet.

### Visual evidence
Inspected original P1 v1.0.3 local mp4 through extracted 2fps contact sheets `.local-inputs/v20.1/b112/video/npc-400-4075.png` and `merchant-580-5875.png`, plus existing404–411 sheet and585 merchant still. NPC sequence:404–404.5 hero leaves cage;405–405.5 Hungry Prisoner has pale-blue ! and green ground circle;406 onward dialogue begins and both disappear. Merchant sequence:580–582.5 green ring is already visible under distant merchant at upper-right while Bogwalker dialogue runs;583–585 remains through NEW QUEST banner;585.5 hero approaches merchant with money-bag and ring;586 onward shop opens. Directly observed persistence while distant: green circle is NOT proof of selected target. Input events are not visible.

Inspected supplied shots8 and18. Shot8 Hungry Prisoner lacks ! and green circle. Shot18 merchant lacks bag and green circle. Both match missing status marker assets/path already investigated in B087/B100, not evidence for a separate friendly attack-target ring.

Existing integrated root NPC frames40/120/300/650 were inspected: green circle appears with B087 marker before accept; both disappear after accept. The green part is the ground mesh of quest_marker(.bdae/_blue), whose bounds span ground z2 through ! z568. That fixture does NOT establish mouse NPC selection.

### Logic evidence and admission gates
- Original `Character::UpdateObjectOfInterest`0x3abb9c validates stored OOI each frame; refresh500ms; TargetList mask0x59, frontal Character priority. `_IsCharacterValid`0x4a1ab8 admits merchant bit0x10, registered quest NPC Character+762 via0x40, hostile bit1; follower alone does not satisfy these.
- B088 recovered handoff: Objective_TalkToNPC::Register0x47db34 sets+762, Unregister0x47dbc4 clears. Quest::SetState0x480c78 registers available accept/objectives/completed end slots. B087 marker helper derives these state/objective facts.
- Original NPC ground green circle is part of quest_marker assets driven by `Objective_TalkToNPC::InstallObjectiveMarker`0x47d8e8 / Remove0x47da6c. Merchant `Character::Update`0x3abe98 merchant enabled +138 / quest+762 gate grabs effect139 quest_marker_merchant; B100 handoff preserves it.
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
Pending scope confirmation from root. No behavior edit or completion claim.

## Verification
Pending private build/runtime. Existing B087/B088/B100 helper reports and integrated frames are reused evidence, not a B112 completion claim.

## Match statements
- Matches video: the existing B087 quest ring path and B100 merchant ring path explain the observed status circles.
- Matches IDA: pending quest/merchant FX rule and absent separate type3 selection effect.
- Not verified: complete user pointer selection request, generic dead/disabled transition in production, merged merchant path, final frozen package.
