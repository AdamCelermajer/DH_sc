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

## 2026-10-10 B002 same-state potion count audit

**Visual/source evidence.** I inspected the supplied Android/native gameplay
capture at
`.local-inputs/publication/checkpoint/port/android-native/reports/native-skill-world-final/gameplay.png`.
The bottom row visibly shows the original skill circles, a Faery control, and a
potion control captioned `× 5`; it confirms count visibility for the authored
HUD, not a PC-specific key legend or PC circle geometry. The recovered-source
handoff `port/engine-ui/authored_gameplay_hud_v1_handoff.md` records the actual
handlers: `NativeHUDSkill(slot)` receives logical source slots 0/1/2,
`NativeHUDSpell()` handles Faery, and `NativeUsePotion(0)` handles potion use.
The current PC mapper is
`features/platform_input/semantic_input.hpp::Bindings` (keys `1/2/3/4/5`)
and `features/platform_input/semantic_input.cpp::button_index` / `pointer`;
`main.cpp::applyPlayerFrameControls` calls
`pc_skill_number_to_source_slot_v1(pcKeyIndex+1, ...)` exactly once for each
pressed PC skill edge. Thus the printed physical key is not a saved slot and
pointer release does not issue a second cast.

**Expected behavior and test defined before this change.** Keep the five
physical hits ordered skills 1/2/3, Faery 4, potion 5. If a caller has the
current HUD frame's exact `CharacterState` and source actor, render the source
Potion0 quantity beside key 5; if no count provider exists, retain the plain
key 5 label. Reject a supplied count bound to another character or actor
without replacing the prior output. Each down edge yields one corresponding
SemanticInput control, with skill-slot permutation deferred to the existing
single pre-cast mapper; saved slot identities and Android 0/1/2 semantics stay
unchanged. The focused test will cover same-state count, absent count, stale
identity rejection, all five hits, duplicate/edge behavior, and atomic failure.

**Uncertainty.** The screenshot is Android/native and is not evidence for the
PC layout. The reconstructed Windows Session's Potion0 count is read from its
current same-character inventory projection; potion use remains subject to
the existing runtime potion transaction and source controller gates. It does
not establish omitted original HUD styling or activation tails.

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

## 2026-10-10 B036 PC HUD label alignment

**Visual evidence.** The reported production capture is
`.local-inputs/b003-production-space/semanticinput-aac57d8f/knight-space220.png`
(P6, 1201x720; the capture name identifies frame 220). It directly shows the
three PC skill circles at approximately screen x=424/509/594, while their
`1/2/3` labels are at x=255/304/352. The Faery and `Potion: 0` caption also
collide left of their two circles. The circle centers agree with the caller's
stage positions 162/200/238 after its single 720/320 uniform scale and
letterbox offset; this localizes the visible defect to text placement. The
original Android/native reference
`.local-inputs/publication/checkpoint/port/android-native/reports/native-skill-world-final/gameplay.png`
is a 2400x1080 frame with five bottom controls in order: three skill cells,
Faery, potion; it visibly shows original skill artwork and potion count `× 5`.
That image establishes source control roles and artwork, not the deliberate PC
circle positions or keyboard legends. No original PC-specific screenshot or
pixel layout was found.

**Source and logic evidence.** The recovered-source handoff
`port/engine-ui/authored_gameplay_hud_v1_handoff.md` records
`NativeHUDSkill(slot)` for logical saved slots 0/1/2, `NativeHUDSpell()` for
Faery, and `NativeUsePotion(0)` for Potion0. The same handoff identifies the
original authored HUD's Faery frame selection through
`NativeHUDGetActiveFaery(0)` and its count field at
`HealthBars.btn_potion.cnt.value.text`. The PC adaptation is the existing
physical key-to-slot mapping `[2,0,1]`, followed by the main caller's one
pre-cast conversion; Android logical slots remain `[0,1,2]`.

The integration caller in `port/windows-foundation/main.cpp` builds circles in
480x320 coordinates (around lines 1982-2003), transforms their triangles once
to the fitted viewport (around lines 2931-2936), and maps pointer coordinates
back to the stage once (around lines 2079-2084). `FrontendText::rebuild` uses
`layout_original_target_text`, which computes alignment within
`TextField.local_bounds` and then applies `TextField.matrix`; it subsequently
projects 480x320 text sprites to the viewport in
`features/frontend/rich_text.cpp`. `append_key_label` had supplied its global
stage rectangle as `local_bounds` with an identity matrix. Center alignment
therefore treated the stage-left coordinate as a local inset, shifting labels
left before the caller's viewport transform. This is a text-local coordinate
bug, not a reason to change circle placement or map skill slots again.

**Expected behavior and pre-code check.** Each PC key legend must center under
its own physical circle; the three skill labels and distinct Faery4/Potion5
labels must have non-overlapping rectangles. Potion count remains from the
same projected CharacterState/actor. Original SkillIcon pixels and the
`[2,0,1]` mapper remain unchanged. Before implementation, the focused check was
defined to exercise field-local origin/translation, viewport projection at
800x600, 1201x720 and 1920x1080, off-center/overlapping label rejection, all
five hit actions, stale count identity and mapper identity.

**Implementation and focused verification.** `append_key_label` now stores a
zero-origin local rectangle and translates it to the caller's stage-space
label rectangle. Composition requires centered labels below their circles and
rejects overlapping label rectangles. The strict C++17 runner passes. It checks
the production viewport equations for all three sizes, the five action hits,
same-state potion count and stale-identity rejection, and rejection of
off-center/overlapping labels while preserving the previous output.

**Uncertainty and remaining verification.** The original source HUD art
contains Faery and potion artwork, but this PC packet currently resolves only
source SkillIcon art; the main caller still supplies the active Faery/potion
captions and same-state Potion0 count. The B036 coordinate fix does not add
invented replacement icons or prove those original sprites are wired on PC.
The integrated normal executable must still verify visual label alignment and
clicks, including real Faery/potion art availability, after the lead applies
the source change. No user live window or save was touched.

## 2026-10-10 B002 source Faery/potion PC packet extension

**Visual evidence.** I inspected the Android/native gameplay reference
`.local-inputs/publication/checkpoint/port/android-native/reports/native-skill-world-final/gameplay.png`.
It directly shows three skill circles, a Faery control with its source emblem
and `Locked` text, and a potion bottle with `× 5`; this is an authored-source
art reference, not a PC-layout specification. I also inspected the frozen
normal PC capture
`.local-inputs/b003-production-space/semanticinput-aac57d8f/knight-space220.png`
(frame 220, 1201×720): all five PC circles are present, only the assigned
skill has an icon, and Faery/potion are blank; their long captions overlap the
lower-left area. This confirms the reported art gap in that executable but
does not test the new packet or its live rendering.

**Logic and exact asset evidence.** Original asset
`port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf`
has SHA-256 `a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238`.
The decoded HUDStyle2 display list places `HealthBars.btn_potion` as sprite121;
its `btframe` child sprite106 places bitmap shape105, the potion bottle.
`controls.controls.btn_spell` is sprite398; its `btimg` child is sprite397,
whose 13 authored image frames contain shapes
`[383,385,387,389,391,367,393,395,empty,363,357,359,361]`. The source
prototype `onPush` at SWF action offset `0x320d4..0x3211b` calls
`NativeHUDGetActiveFaery()`, adds 1, then calls `btn_spell.btimg.gotoAndStop`.
The `+1` is the ActionScript one-based frame convention: the generic source
transport at `port/engine-ui/authored_gameplay_hud_v1.cpp` adds one to a
zero-based frame, and bundled GameSWF converts one-based values back to
zero-based frames in `vendor/gameswf1714/gameswf/gameswf_sprite.cpp` near
line529. Therefore source IDs0..12 select frames0..12. A `-1` sentinel passes
frame0, which bundled GameSWF treats as invalid; the already-placed `btimg`
remains on its initial frame0. That last statement is an inference from the
initial placement and invalid-frame handling, not a source rule that `-1`
means the first Faery. Frame8 is actually empty and remains empty in this
adapter. The source
`onLoad` click closures at `0x30dd5..0x30df8` route Faery release to
`NativeHUDSpell()`; the potion handler is on
`HealthBars.btn_potion.onRelease` and calls `NativeUsePotion(0)` per
`port/engine-ui/authored_gameplay_hud_v1_handoff.md`. The art still uses
source bitmap1, `MenusGraphics_droid.tga`. This is decoded SWF/display-list and
ActionScript evidence, not a claim that every input callback is integrated.

**Expected behavior and pre-implementation check.** Preserve PC slot mapping
`[2,0,1]` and saved/Android slots `[0,1,2]`. Keep the circles, key positions
and keyboard labels as an explicit PC adaptation; draw only source Faery and
potion triangles/UVs inside the caller's PC circles. The pre-code focused
check was to validate the source frame/shape table, active IDs0..12 to the
same-numbered zero-based frame, the no-active initial-frame behavior, source-
empty frame8, potion shape105 UVs, rejection of IDs outside the recovered
frame domain, and the existing five-control/key/count/input tests.

**Implementation and verification.** Added reproducible exporter
`export_pc_gameplay_hud_source_art_v1.py` and generated
`pc_gameplay_hud_source_art_v1.cpp` from the pinned original SWF using the
repository's bounded shape/timeline decoder. The feature packet now accepts an
optional same-frame active Faery ID and appends its exact original sprite397
frame art; it always appends shape105 potion artwork. Existing physical order,
circle hit regions, labels/count, and the single pre-cast `[2,0,1]` mapping are
unchanged. The strict C++17 runner passes. It checks all 13 frames and shape
IDs, the source-empty frame, shape105/atlas UV preservation, invalid-ID atomic
rejection, all five hit actions, same-state count and the existing key edge
behavior. Command: `& .\port\windows-foundation\features\generic_skills\run_pc_gameplay_hud_v1_tests.ps1`.

**Limits.** This is generated source art plus isolated packet/input tests; it
is not wired to the normal PC executable and does not prove PC pixels, pointer
hits, or NativeHUDSpell/NativeUsePotion dispatch. Caller must supply the real
current `NativeHUDGetActiveFaery` result; absent identity remains no Faery
icon, and frame8 stays blank as authored. The source-locked overlay/cooldown
appearance is not added by this packet. The integration lead owns normal
renderer and hit-surface wiring, followed by a normal frozen-executable
capture; no claim of complete B002 resolution is made.
