# SPACEBTN report (Preview 16, branch p16/spacebtn, from p16/integrate 2d703432)

Status: Space context button is the production dispatcher for containers (open, destructible hit), NPC talk and enemies. Chest tutorial opens by Space after the script unlocks. The bottom-right action button is a labelled PLACEHOLDER (its authored art is not drawn). ctest: green except `session_skill_binding` (allowed).

## 1. Investigation (evidence)

### 1.1 Reference video (Part 1, v1.0.3), frames in `.local-inputs/p16-runs/spacebtn/ref/`
- 200.0, 204.0 s: chest tutorial caption band with SKIP, HUD hidden.
- 205.0-206.5 s: caption "Keep an eye out for Epic Chests, which contain superior equipment." (title "Tutorial"), SKIP, HUD hidden. Frame `at-206.5.png`.
- 207.0-208.0 s: HUD back. Bottom-right orange ring button shows a SWORD icon (attack).
- 208.5 s (`at-208.5.png`): full HUD; sword button; chest visible ahead of the player.
- 209.0-213.0 s: same sword button in the crops; a frame at 209.0-209.5 looks different (glow/face), not identified.
- **Discrepancy with the brief:** the brief says a chest icon appears in range at 205.5-208 s. In the crops of the bottom-right button (strip 205.0-213.0 s) I did NOT see a chest icon; the button stays the sword. Not resolved; the chest icon may need a finer frame pass.
- Pressed state: not identified in the frames I looked at.

### 1.2 Logic (IDA, from CONTEXT-report sections 1.1-1.5; not re-traced)
- OOI query `Character::UpdateObjectOfInterest` 0x3abb9c: 500 ms timer, `TargetList` mask 89, radius `OOI_Distance` 200, sorted frontal (`_sortFrontal` 0x38d5b4). Characters are queued first (flag), then ascending angle. Distance is NOT the rank. The brief said "nearest"; I followed the decoded frontal sort, which the brief also wrote in parentheses.
- Types: OpenableContainer 0 (0x3a16cc), DestructibleContainer 8 (0x3a0d60), enemy Character 8, friendly NPC 3, TriggerObject data row, items -1 (never OOI).
- Dispatch: HUD action button -> `Cmd_UseOOI` -> `AI_InteractWith` 0x3cff34 -> `SM_SetInteractState(type)`. Type 8 = attack state. NPC type 3 -> `Character::Interact` 0x3a4d78 (TalkToNPC event). `Container::Interact` 0x3a0b38 gates: unknown -> no visual -> state 3/4 reject -> activate. Range is the caller's query (200).
- Icon: `MenuManager::Update` 0x42eab4 table 0x8c9f28 = [0,1,2,3,5,5,6,7,5,5,4] (cached type, out-of-range -> 5). The same table is in `engine-ui/authored_gameplay_hud_v1.cpp` `AuthoredGameplayHudV1::action_icon`, which also drives the `btn_interact` movie (ID 374, btimg frames 0..11) through `update_action_icon`. Port table verified equal.

### 1.3 HUD art trace (ART/HUD rule)
- The authored button is a Flash movie (`btn_interact`, frames = icon index) driven by the authored HUD ActionScript. The EXE does NOT use `AuthoredGameplayHudV1` (not wired into the PC HUD). The movie's primitive draw path (bitmap fills / SWF shapes) was NOT traced to vertices/colours in this session. Therefore the drawn art is a PLACEHOLDER (section 6). The exact source to replicate: `AuthoredGameplayHudV1::update_action_icon` -> movie frame `btn_interact` frames 0..11.

## 2. Expected behaviour (preserved rules)
- Space is the single context button, press edge only:
  - Enemy OOI (type 8 actor): attack held (source held Cmd_Attack path), press sets the AI target (AI_SetTarget(OOI,0)). Enemy priority is the source queue order (Characters first).
  - Container OOI (type 0 chest): press -> `Container::Interact` -> activate -> `opened` -> DoOpen loot (existing container path).
  - Destructible OOI (type 8, non-actor): press -> one hit / break (not a held melee; PC adaptation).
  - NPC OOI (type 3): press -> TalkToNPC (quest event).
  - No OOI: held/press -> Cmd_Attack (existing).
- Controller lock (cutscene/tutorial LockCharacter, dead) refuses the press-edge use (new gate).

## 3. Implementation (commits on p16/spacebtn)
- `14a3a9ce` dispatcher: containers and talk NPCs are re-registered each frame into `InteractableRegistryV1` (containers: openable 0, destructible 8 except when opened; talk NPCs: type 3, `is_character` true). Space press edge routes to `containerRuntime.interact` (open or hit), `talkToNpcObject` (new helper, raises the same TalkToNPC event as `talkNearestNpc`), or the existing actor AI target. E no longer starts NPC talk; `--quest-talk-frame` (scripted) kept. `--interact-at` kept as debug.
- `context_button_v1`: `object_is_actor` (default true) so a non-actor type 8 is not combat (no held melee).
- `8962c6f6` controller-lock gate on the press-edge use; the context line prints `locked_global`/`locked_char`.
- `b3c1cc0b` PLACEHOLDER action button in the PC HUD: `PcGameplayHudLayoutV1::action` (bottom right, centre (440,270) radius 28, key-label bounds 400-480 x 300-313) + `action_label`. Ring = decoded `btn_spell` base (`original_pc_gameplay_hud_spell_base_v1`), text label from `action_button_label_v1(icon)`. Recomposed when the icon changes. No hit region (display only).
- `915416fc` isolated tests: barrel press / hold rule and label names. `run_context_p16_tests.ps1`: all PASS (65 checks).

## 4. Integrated runtime verification (quiet runs, EXE `DH_wt/build-p16spacebtn/dh-foundation.exe`, final build)
Logs and frames: `.local-inputs/p16-runs/spacebtn/`.

| Case | Setup | Result (log) |
|---|---|---|
| In range press | `final/inrange`: player 100 units from `_prim_OpenableContainer_2`, Space 20:3 | `Context container ... status=accepted state=2->3 distance=99.997 frame=20`; `opened frame=35 loot=227`; `Container loot ... selected=1 delivered=1 status=ok`. Frame `final/inrange/inrange.png` shows the placeholder "Chest" ring. |
| Out of range | `final/outrange`: player 600 units away | No OOI, no context line. Icon 5 "Attack" (`final/labels.png`). |
| Duplicate press | `b3/dup` (120 frames): presses 20 and 90 | First accepted, opened 35, loot once. Second `status=rejected_state state=3->3 frame=90`, no second loot. |
| No press (control) | `final/nopress` | Chest OOI present (icon 0), no interaction. |
| Enemy priority | `final/enemyprio`: player 120 units east of barrel B (`_prim_DestructibleContainer_05_..._47`), enemy also in range | `Context button ... type=8 use=1 actor=1` (enemy), no container line, barrel untouched. Icon 5 "Attack". |
| Barrel press | `final/barrelonly`: player 120 units south of barrel B | `accepted state=2->3`, `opened frame=36 loot=9 delivered=1`. One press, no held melee. |
| NPC talk via Space | `talk/`: Swamp talk NPC row 366 (same setup as QUESTUI talk-lizman), Space 40:3 | `Context talk npc row=366 id=... level=41`; `Quest event kind=2 applied=1`; `NEW QUEST row=51` banner. |
| Chest tutorial (LockCharacter) | `tut/`: start in `tutorial_treasure` zone next to the chest, tutorial starts (`script tutorial_treasure finished`, `chest_tuto` captions) | Press at 60 (during the caption): `status=refused_controller_locked`. Press at 1400 (after the script): `accepted state=2->3`, `opened frame=1414 loot=227 delivered=1`. |

Container/persistence/loot behaviour is the CONTAINERS2 path, unchanged; the new dispatch only supplies the press.

ctest (`DH_wt/build-p16spacebtn`, `ctest -j 6`, toolchain bin on PATH): 129/130 pass. Only `session_skill_binding` fails (allowed). Without PATH, `container_open_script_v1` and `winmm_pump_priority_v1` exit 0xc0000135 (DLL not found), an environment issue recorded in CONTAINERS2.

Visual check: `final/inrange/inrange.png` (bottom right "Chest" placeholder ring, label under it); `final/labels.png` ("Attack" for enemy, barrel, no OOI).

## 5. Not verified / gaps
- Chest icon in the reference at 205.5-208 s: not observed in the frames I looked at (sword in all). Needs a finer pass (brief's premise unconfirmed).
- The bottom-right button art (`btn_interact` movie) is NOT drawn. The SWF primitive path is not traced; the PC HUD has no equivalent art decode yet. Pressed state is not implemented (not identified in the frames).
- The placeholder placement (440,270, r 28) is not measured against the reference frame; it sits right of the potion. The ring is the `btn_spell` base (greyed look in the capture), not the orange `btn_interact` ring.
- After a chest opens its OOI stays a chest (icon "Chest") and a second press is `rejected_state`. Source OOI validity after open is not decoded; this is a guess that keeps the icon.
- Registry radius is 0 for containers and NPCs (source vt+148 for GameObjects not decoded). The OOI query uses 3D distance minus owner melee radius; `Container::Interact` uses the horizontal 200 check. A container just inside the OOI query can still return `out_of_range` from the container gate.
- NPC talk: every TalkToNPC row is a type-3 candidate regardless of NPC state (source friendly-NPC eligibility not decoded). Only the Swamp talk NPC was exercised.
- PC adaptations (explicit): (1) destructibles are one hit per press edge, never a held melee; (2) E no longer starts NPC talk; (3) enemy hold behaviour unchanged.
- The Space press is refused while the controller is locked (cutscene/tutorial). The attack path is unchanged.
- Chest opened-state clip rate and the reference lid timing are not re-measured here (CONTAINERS2 gap).
- Frontal sort used per the decoded source; the brief's "nearest" wording is not what the source does (CONTEXT-report correction).

## 6. Placeholders
1. PC HUD action button (bottom right): ring = decoded `btn_spell` base instead of the `btn_interact` art; icon replaced by a text label ("Chest", "Item", "Lever", "Talk", "Revive", "Attack", "Action"). Source to replicate: `AuthoredGameplayHudV1::update_action_icon` -> `btn_interact` frames 0..11 (SWF draw path not traced). Reference frames: `.local-inputs/p16-runs/spacebtn/ref/at-208.5.png` (sword, full HUD), crop strip 205-213 s. Placement is an estimate. Pressed state: not implemented.
2. Friendly NPC eligibility: all TalkToNPC rows are candidates (no state gate).

## 7. Package files required
- None new.

## 8. Verifier script
- Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16spacebtn -Jobs 6 [-Test]` (run ctest with the llvm-mingw bin on PATH for the two DLL tests).
- Isolated: `powershell -NoProfile -File port/windows-foundation/features/combat/run_context_p16_tests.ps1`.
- Quiet batch: `FRAMES=120 node .local-inputs/p16-gen-jobs-spacebtn.js <outdir> "name=x,y,z!--space-key-interval,20:3" ...` then `tools/quiet_run.ps1 -JobsFile <outdir>/jobs.json -Parallel 6`. Jobs used: `p16-runs/spacebtn/{b1,b2,b3,probe,final,tut,talk}`.
- Probe for the enemy/container overlap: `p16-runs/spacebtn/probe` (40 start points around five containers; `pB0` enemy + barrel, `pB3` barrel only).
