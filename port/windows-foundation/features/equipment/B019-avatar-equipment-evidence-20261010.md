# B019 equipment avatar, action, and profile evidence — 2026-10-10

## Original visual evidence

Reference: `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`.

- At 8:30.5 (510.5s) and 8:31.0 (511.0s), the original Torso page shows the `AUTO EQUIP` caption, a selected Ceremonial Garb row, and the full 3D character standing in the right preview pane with armor and a drawn sword. The avatar occupies the pane while the item controls and list remain separate.
- At 8:32.5 (512.5s), the page is Right Hand with Useless Blade selected. The same posed 3D avatar remains in the preview pane and a sword is visible. These sampled frames establish the page composition and visible equipped model; they do not show a change transaction or prove animation continuity.
- Extracted reference frames are retained in `.local-inputs/b019-original-equipment/510_5.png`, `511_0.png`, and `512_5.png`.

Directly observed: original equipment UI contains an avatar pane, equipped-looking body/weapon models, and a visible Auto-equip label. Inference: the avatar is the currently equipped player model, supported by the native render call path below and same-Scene equipment update path; the sampled video alone cannot prove item-instance identity.

## Original logic evidence

IDA-derived `port/engine-ui/reference/character-menu-flow-v1/native-stack-functions.json` and `.asm` identify:

- `MenuCharMenu_InvMain::Show` at `0x452b1c` calls `CreateAvatarCamera` (`0x4528d0`) and registers the avatar display callback. `Hide` (`0x4528a0`) destroys that camera.
- `CreateAvatarCamera` obtains the local player, creates and activates the avatar camera, and checks `CharStateMachine::SM_IsDead` (`0x3c03f0`); a living player is put into idle with `SM_SetIdleState` (`0x3c1a00`). The camera literals include the source pane look-at and FOV later captured by `runtime_equipment_preview_v1_report.json`.
- `MenuCharMenu_InvMain::RenderCharacterPane` at `0x452468` finds the SWF `avatarpane`, derives its absolute bounds, activates the avatar camera, obtains the current level and local player, calls `CharAnimator::Update` (`0x3caf3c`), reads `Application::GetDt` (`0x31f66c`), advances the root scene through `RootSceneNode::PlainOnAnimate` (`0x35c268`), and synchronizes visual rotation. This is a live character/pose path, not a still image.
- The SWF action for Details `btn_AutoEquip.onRelease` in `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt` passes the selected `InvSlotId` and operation `2` to `NativeInvAutoEquipSlot`, then regenerates the list. The bulk main-page button `btn_GAMEPLAYMENUS_AUTOEQUIP_ALL` separately sends `-1`; these two source commands must not be conflated. The Details title is supplied through the source `GAMEPLAYMENUS_AUTOEQUIP` symbol.
- The source item transaction uses `dh2_equipment_auto_v3`, mapped to `Inventory::EquipItemAuto` at `0x400c84` / `_EquipItemAuto` at `0x4009f8` in `port/game-data/reference/player-equipment-v3/NOTES.md`. The kernel reads the actual ItemTable slotting/type and live option flags; the adapter then stages gear properties, source appearance, vitals, and publication. Other useful source references already recorded in `reports/feature-equipment.json` are requirement checks `0x3a4930`, gear refresh `0x3e08a8`, ordered slot/power loading `0x3df480`, source slot-2 behavior `0x40022c`, class recalculation `0x3e0810`, and HP/MP clamp `0x3bd140`.

## Expected behavior and current reusable path

The Equipment pane should show the selected character's actual source model with its currently equipped armor and weapon, in the source avatar pose/camera. Equip, unequip, and Auto-equip must mutate that same character and owned item, refresh the model from the same equipment data, and preserve the stable inventory instance through profile save/reload.

The current production path is present in `port/windows-foundation/main.cpp`: the Details Auto-equip request is consumed through `RuntimeEquipmentBindingV1::auto_equip` around lines 2311–2326, after which the same-session render receipt is retained. Around line 2963, the Equipment avatar pane calls `with_preview_packets`, applies the source root rebase, and submits actual source packets through the existing renderer viewport. The feature borrows `CombatSession`'s current `CharacterVisual` and retained `Scene`; its `VisualSkinOwnerV6` resolves ItemTable modules and source weapon URIs/anchors. Source weapon rows are already in those packets and must not also be drawn from the attachment receipt.

The helper camera/rebase uses the composed SWF pane bounds `[155.05,87.70,321.10,282.75]`, source FOV bits `0x3f0efb1a`, eye `[0,-800,200]`, target `[0,0,200]`, near 10 and far 1000. It retains current source-root Euler X/Y and scale, applies the original `-0.5` radian Z, and rebases the real draw matrices. This keeps the current class model and pose in scope instead of creating a screenshot/model replacement.

## Focused verification

Before the test run, the observable checks were: a real source profile/item Auto-equip produces the same-session source weapon view and render receipt; save/reload retains the exact selected item instance; rebinding the actual preview owner yields that same weapon row without advancing the character animation clock; body suit equip changes source module rows and unequip restores the initial rows; a source Debug query failure reports the reached prefix and a later refresh restores the current gear projection.

`run_runtime_equipment_binding_v1_tests.ps1` passed against the staged original cache and actual `CombatSession` Knight player. It executes the original Details action surface, Auto-equip kernel, same-Scene receipt, source StartingSuit module change/unequip restoration, exact Longsword01 model/anchor, source packet provenance, failed source query recovery, missing-item/power-provider rejections, and the newly added isolated `save_character`/`load_character` round trip. The reloaded profile is rebound to the same current Session and its source weapon view is verified with the same `source-sword` instance ID.

`run_source_equipment_renderer_smoke.ps1` passed the isolated native WGL draw: 4 source body packets, 1 source weapon packet, 2 original textures, 1,288 changed framebuffer pixels, `GL_NO_ERROR`, and no source clock advance. Its runner needed `platform_context_identity.cpp` added to the link closure after the renderer began requiring those context identity providers.

These are focused feature and renderer results. They are not a normal frozen executable capture or a cross-class acceptance. The main callsites exist in the current source, but the tracker still owns the normal-executable run.

## Uncertainties and remaining coverage

- The direct visual samples are v1.0.3 and sparse; they cannot establish animation continuity or a visible Auto-equip before/after transition.
- The focused live-session fixture uses KnightPlayerBase with actual StartingSuit and Longsword01. The provider is driven from the same Session visual so it can follow the active class model, but Knight/Rogue/Mage model and pose variants are not yet all verified in the normal executable.
- The profile round trip verifies the SaveStore identity that the live equipment owner consumes, then rebinds the same Session fixture. A normal save/restart/reload across replaced Session and profile owners remains integration evidence to collect. Keep B019 open until that route and representative classes/items pass.

## Cross-class same-Session matrix (2026-10-10)

The focused matrix reuses the visual evidence above and the actual source class
asset audit in `runtime_creation_appearance_asset_audit.md`: all three classes
use the same authored `models/prince_modular.bdae` body with distinct warrior,
rogue, and mage modular skin aliases; Rogue's source weapon is
`MC_RWeapon_Dagger_01`, and Mage's is `MC_RWeapon_Quarterstaff_01`. The sampled
v1.0.3 footage remains evidence of avatar-pane composition only; it does not
show an equip transition.

`run_runtime_equipment_avatar_matrix_v1_tests.ps1` strictly builds and runs
`runtime_equipment_avatar_matrix_v1_tests.cpp` with the LLVM-MinGW C++17
`-Wall -Wextra -Werror` toolchain. It overlays the original shared pydata with
the full Android original visual assets in an isolated `.local-inputs` test
directory. Each profile is checked against its source CharacterTable row and
four exact starter ItemTable rows: Knight 263 / StartingSuit 1079,
StartingBoots 1073, StartingGloves 1076, Longsword01 664; Rogue 325 /
StartingSuitRogue 1081, StartingBootsRogue 1075, StartingGlovesRogue 1078,
Dagger01 370; Mage 290 / StartingSuitMage 1080, StartingBootsMage 1074,
StartingGlovesMage 1077, Staff01 1025. The runner passes all three profiles.

For each class, the same `CombatSession` actor's retained `CharacterVisual` and
Scene are checked through the preview borrow; a real starter torso equip changes
the source body module/material projection and unequip restores its original
module/material identity. The exact class starter weapon is then equipped and
the resulting source draw view and same-actor render receipt are checked against
its class-specific weapon geometry/material and stable instance ID. SaveStore
`save_character`/`load_character` preserves the class and exact equipped
instance; a fresh binding to that same live Session recreates its weapon view,
then unequip removes it without advancing the source pose clock. This validates
the feature's current-session SaveStore rebind path; it does not test native
GameSave/V60 persistence or a replaced Session after application restart.

### Exact production renderer handoff

Current root integration in `port/windows-foundation/main.cpp` binds the page
and runtime near lines 1761–1834. The main avatar display-list anchor calls
`RuntimeEquipmentBindingV1::with_preview_packets` around lines 3020–3035; its
callback derives the original source pane viewport, then draws every packet
through the root `Renderer::withViewport` while multiplying the authored
preview rebase by each source packet world matrix. Do not separately draw
starter weapon attachment receipts because their source weapon views are already
in the packet frame. After equip/unequip the input path at lines 2363–2371
consumes the typed source command/render receipt and rebinds source locomotion.
The lead's normal verification must use a frozen executable, record its SHA and
the selected class/profile, inspect the actual viewport for all three classes,
and exercise the source Details equip/unequip path in isolated profiles. The
focused matrix is not that executable/visual acceptance and does not close B019.
