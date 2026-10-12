# B132 — postfight BashDown registry / fatal transition

Implementation owner `/root/b132_bash_registry`; reviewer `/root/batch_gameplay_review`; integration `/root/batch_integration`. OPEN pending frozen normal runtime proof.

## Investigation before implementation

- Original visual evidence inspected: `.local-inputs/claude-preview13/E/bd30_sheet.png`, original reference 282.60–283.37 s (c01–c24 at 30 fps), and existing `coordination/claude-preview13/E-report.md` provenance. Knight proceeds through melee/impact poses beside Bogwomp; c01–03 have a short orange dust burst, later frames show target light/Miss and continuing animation. Attribution to BashDown is inherited inference; pressed skill key is absent. This proves no synthetic persistence visual behavior; the relevant invariant is continuous authored cast completion, not invented original exception recovery.
- Current failure sequence is independent reviewer normal physical-input run `.local-inputs/v21.3/gameplay-review/postfight-controls-trace`: frozen EDFB187F2B172BED8DD79152232FF681A4EA59C7D0BB991D3D83CA5D1F26DF6E / source63d7e97f. Real two-moth kills1109/1232, GoldStack01 pickup1388, key2 at28s, failure1508. Before input state1500 is alive HP169.195, Idle3, no pose ownership, unlocked controls and EquipmentReady1. Phase5 query completeness error precedes locomotion fatal. This is not a death restore failure.
- Current visual inspection: reviewer captures f1300.png shows hero standing with weapon, Priest, folded moth and visible4GOLD; f1450.png shows hero farther into water and same HUD key2/stock5. These two frames support displacement and visible drop presence across postfight movement; they do not show a successful BashDown (failure1508 has no capture) or independently prove kill/admission. Exact kill and pickup outcomes come from the run trace above.
- Recovered logic: `port/level-world/reference/character-skill-state-v4/cast-lifecycle-v46.md` original plaintext Lua provenance: BashDown Pre Enemy/AttackableOnly/FrontalFirst/range160, local target/LookAt; Use attacks captured target once; Post unconditional ClearTarget. CSSkill Focus raises SkillPre at0x3c4500 before SM_SetAnim at0x3c4510. Admission HasMana precedes departure (`runtime_skill_cast_coordinator_v1.cpp:821–847`); reached Pre query occurs after departure (`:855–870`) before actual mana debit. Strict same-world object size/order guard `runtime_skill_target_query_v1.cpp:307–309` is correct and remains required.
- Caller/admission source cross-check from reviewer handoff and `coordination/v20.1/B097-report.md`, `features/generic_skills/README-runtime-skill-cast-prepare-v1.md`: `_DoSkill`0x3b8bd8 → `AI_UseSkill`0x3d8868; `AI_IsSkillUsable`0x3d8358 requires loaded instance, not casting, and interruptibility before usable callback. BashDown Pre consumes mana even with empty result; Use class215 and retained Lua-local target roll once. No target-required gate may be invented.
- Producer fault: `main.cpp:6758–6762` enumerates only authored population definitions, while main3376 with campaignTriggers calls `CampaignHost::bind_lifecycle_object`; `campaign_host.cpp:860–883` explicitly adds synthetic neutral ID0x43414d504c494650 / name `campaign.lifecycle` / visiblefalse. It is a persistence adapter, not an authored interactive GameObject, and omitted from query order. World-item pickup/drop adapter owns a separate registry; temporal proximity to GoldStack pickup does not establish causation.
- Failure recovery audit: `CombatSession::play_actor_source_sequence` accepts physical departure then Focus Pre (2239–2300); failed Focus sets transitionFailure and retains reached source6 prefix. Coordinator receiptpartial_failure (918–923) intentionally does not roll back source departure/committed effects. Existing focused order tests explicitly require this behavior. Forcing Idle or weakening complete-registry validation would hide the producer error. No recovery rule is inferred from original footage or guessed.

## Expected behavior and planned minimal repair

Keep authored target order and attackable predicates. Complete the caller's typed registry with the exact CampaignHost-owned invisible persistence entry, excluded from gameplay attack query by its explicit owner type. Unknown extra objects must still fail completeness/typed admission. No target cone, mana, cooldown, action, source callback order or generic exception recovery changes.

## Focused test defined before coding

Exercise the real connected skill query with: authored attackable container; invisible exact campaign persistence entry; malformed same-ID/name entry; unknown extra object; duplicate order. Verify exact campaign entry is excluded, existing selected actor/container order unchanged, omitted/unknown/duplicate entries still reject. Then integrated frozen normal physical S/Tab/Space two-moth fight, pickup, BashDown and subsequent movement/potion/Equipment; successful normal Post and zero fatal required. Preserve source-phase failure guard; do not pretend a helper is runtime resolution.

## Gaps

Original clip does not prove key attribution or original error recovery. Latest frozen4C60 uses new46-actor roster while saved seed44 requires lead migration. 6B2B predeath locomotion fatal remains separately under investigation; no causal connection asserted.

## Implementation proposal

Private `main.patch` and `campaign-header.patch` under `.local-inputs/v21.3/B132/` sent to integration. Exact CampaignHost predicate requires synthetic ID, exact name, hidden visual and empty model/material/animation; it never broadly excludes invisible objects. Main appends the owner-exact entry after authored declarations, declares interaction_type=-1 in its same-world typed fact callback. Source query/coordinator/physical guards unchanged.

Focused compilation initially exposed stale root `.local-inputs/windows-foundation-build` libraries missing current death/runtime APIs. Retried against current integration `DH_wt/build-v201int` libraries; no source guard change was made to accommodate stale libraries.

## Focused results

PASS `.local-inputs/v21.3/B132/test.log` via private `run-tests.ps1`, current real connected Session/query/coordinator sources and integration libraries. Missing campaign entry reproduces exact complete-registry error. Complete typed entry preserves original Character/container order. Exact owner rejects malformed prefix/name and visible variants; arbitrary extra hidden omitted entry fails completeness, explicitly ordered unknown entry fails typed ownership. Existing linked Warrior/Rogue source Use/Post, MP/HP/RNG, mana/empty-target refusal, pre/post-debit partial failure and duplicate checks all pass. This is focused proof; normal frozen production remains pending lead/reviewer.

Second focused run additionally keeps the exact campaign persistence entry present through actual empty-target and attackable-object-target BashDown Pre/Use/Post. Both source mana spend and once-only Interact/marker/duplicate suppression pass; cast returns Idle normally. Output explicitly reports `PASS B132 campaign persistence present across nil/object BashDown Pre/Use/Post and duplicate rejection`. No source action recovery bypass was needed.

## Integrated frozen normal runtime

Owner test `.local-inputs/v21.3/B132/normal-4b42/`: frozen `dh-foundation-integration-4b42c44b.exe`, SHA2564B42C44BB64707276153B101B2927DAA9758E5DAF2E4870BBF373E0B5D0B7AFD. Own copied seed from v21.1, hidden isolated desktop, real Win32 S12–17s/Tab19s/Space20–23.5s/W26–27s/key2at28s/key5at30s/C32s/C34s. EXIТ0; no focus, registry or physical transition errors.

Actual moth lethal hits1011/1107; alive Idle3 admits physical key2 at1399 (phase1, MP28.5→19.5); authored Use1430 has empty targets; retained Post1473 is phase3, ClearTarget applied. Frame1500 trace Idle3/unlocked/EquipmentReady1. Potion1516 quantity5→4, MPraw4992→7296. Character menu physically opens1634 and closes normally. Inspected PNGs1450 (BashDown impact pose/groundFX),1550 (upright with potionstock4),1700 (real Stats menu). This verifies the normal postfight nil-target cast and continued potion/menu controls. It is not an Equipment-tab-specific check.

Run timing differs from original reviewer trace because load/start timing is wall-clock-relative. Current RNG produced no lootdrop; no GoldStack pickup is claimed in this owner run. Reviewer independently owns exact postfight/pickup replay on same freeze. Object-target cast remains focused connected consumer evidence; no normal physical container selection claim.

Additional `object-4b42` directfresh Swamp starting-coordinate fixture near authored urn exited0 but opening source cinematic remained controllerBlocked1 (inspected f320.png shows original SKIP cinematic). Physical key2 was correctly not admitted. This is a refusal/gate result, not object-cast proof. No gate/flags/target override was made.
