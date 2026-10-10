# PC gameplay skill HUD V1

## Source evidence and intended contract

The visible source comparison was inspected at
`.local-inputs/publication/checkpoint/port/android-native/reports/native-skill-world-final/gameplay.png`.
Its bottom HUD row contains three skill circles followed by Faery and potion
controls; the first skill control shows an icon/level and the next two are empty.
This is an Android/native capture, not a PC-renderer reference, and it does not
establish the new PC circle placement or PC key labels. The current Rogue Skills
page capture at `.local-inputs/v19-frontend-hotfix/current-gui/rogue-skills.png`
shows three separate assignment circles; its middle circle is assigned while
the left and right are empty.

The original source logic is recorded in
`port/engine-ui/authored_gameplay_hud_v1_handoff.md`: authored controls dispatch
`NativeHUDSkill(slot)` with logical slots 0/1/2, `NativeHUDSpell()` for Faery,
and `NativeUsePotion(0)` for potions. That source handoff also states the Android
HUD's whole artwork is available from the retained `AuthoredGameplayHudV1`
movie, while the PC HUD feature receives only projected skill data. The PC
physical `[2,0,1]` order and labels `1/2/3` are the intentional PC mapping-circle
adaptation documented at
`port/windows-foundation/features/generic_skills/README-runtime-skill-cast-prepare-v1.md`
and implemented by `pc_skill_hud_projection_v1`. They are not native keyboard
semantics and never rewrite saved slots. `SemanticInput::Bindings` already maps
PC `1/2/3/4/5` to `skill1/skill2/skill3/spell/potion`; pointer controls use the
same six button edge lanes.

The implemented packet therefore uses exact original `dqcharmenu_droid.swf`
SkillIcon meshes/UVs for assigned skills, with empty source assignments left
icon-free. The ring/fill mesh and key labels are deliberately marked as a PC
adaptation; Android ring artwork is not claimed. Placement comes from the
caller in authored 480x320 coordinates. Unknown cooldown/usability is not
visualized as a fabricated disabled/ready state.

## API and behavior

`pc_gameplay_hud_v1.hpp` exposes `compose_pc_gameplay_hud_v1` and
`pc_gameplay_hud_hit_test_v1`. The layout carries caller-provided centers,
radii and label rectangles for the three skills, Faery and potion. Composition
validates physical/source order `[source slots 2,0,1]`, appends the source icon
triangles, PC fill/ring draw batches, labels `1/2/3/4/5`, and matching circular
hit contours into a frontend `ScreenArt` packet. Icon batches select bitmap1,
the same original `MenusGraphics_droid.tga` atlas already used by the live HUD
and character-menu renderer. The key control returned by a
skill hit is always the physical PC control (`skill1`, `skill2`, `skill3`) with
key number in `Hit.item`; only the existing `pc_skill_number_to_source_slot_v1`
performs the later `[2,0,1]` conversion. Faery and potion hits return `spell`
and `potion` controls (items 4 and 5).

The helper is not wired into `run_frontend_v1`, the main gameplay renderer, or
the main input surface. Root must provide viewport-derived placement, merge
this packet into the live draw list, and return this hit-test result from the
existing `SemanticInput::Surface::hit` callback before claiming normal HUD UI.
The existing Android authored-HUD owner remains the reference for source
control behavior and is not replaced.

## Focused test

`run_pc_gameplay_hud_v1_tests.ps1` builds an isolated strict C++17 executable
from the new feature files, `original_skill_art.cpp`, and `semantic_input.cpp`.
It verifies source SkillIcon UV preservation and per-cell batch order, 48-segment
circle/ring pixel areas and hit contours, key labels, action order 1/2/3/4/5,
SemanticInput press/release edges for each circle, no outside hit, and atomic
rejection of a stale `[2,0,1]` source identity.

Command:

```powershell
& .\port\windows-foundation\features\generic_skills\run_pc_gameplay_hud_v1_tests.ps1
```

Result: PASS (`pc_gameplay_hud_v1_tests PASS: original SkillIcon UV/pixels,
48-segment PC circle geometry, physical hit order1/2/3, Faery4, potion5 and
SemanticInput press/release edges`). This is feature-level geometry/input
verification only; no integrated PC gameplay screenshot or live hit test has
been run.
