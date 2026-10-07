# Same-player original inventory preview V4

Root integration includes `renderer_character_inventory_preview_v4.inc`
immediately after `renderer_front_exports_v87.inc` inside `model_renderer`.
The public API is `draw_character_inventory_preview_v4(width,height,error)`.
The actual cached and source-named callback target is
`_root.menu_InventorySheetMain.avatarpane`. Attach the retained movie's
`menu_display_callback`; call the existing `SwfGpu::scene_pane` with the supplied
actual `SwfDraw.rect`, and invoke the new API from its pane callback. Existing
scene-pane submission owns viewport/scissor/depth-clear handling.

The renderer borrows `prince_visual.root`, the current same-player equipment
owner and its live posed `VisualDrawPartV6` records. It never loads a second
player, copies Gear/Save, advances simulation or writes gameplay transforms.
It refreshes the same GPU equipment draws from the same live pose.

Source `MenuCharMenu_InvMain::CreateAvatarCamera4528d0` initializes position
`(0,-800,200)`, target `(0,0,200)`, up `(0,0,1)`, near10, far1000 and FOV
float bits3f0efb1a. RenderCharacterPane452468 changes aspect to actual pane
width/height. Its source root preview places the player at the origin, reads
the actual quaternion Euler angles, converts X/Y with bits3c8efa35 and supplies
Z=-0.5 before rendering. The adapter performs this as a draw-matrix rebase of
the actual current posed geometry; it does not mutate the live root and then
hope to restore it. The root scale and all relative animated/equipment
transforms remain borrowed from the current player.

The GL adapter preserves program, array/element buffers, all three submitted
vertex attribute descriptors/enables, texture0/1 bindings and active unit,
depth function/mask/enable, cull mode/front/enable and separate blend factors.
It preserves the existing source-backed material and mesh draw implementation;
this does not reconstruct original LightScene or whole Inventory receiver
Update/rotation-event lifecycle.

Validation: strict complete copied renderer TU syntax check using the current
actual x86_64 compile database passed. The include was inserted in a temporary
probe; the shared renderer source was not edited by this task. Root owns both
ABI linked-build and live pane rendering validation. No live GPU acceptance is
claimed by this syntax result.

## Portrait audit

Actual Android SWF HudChar sprite41 has source frame labels Warrior0/Rogue1/
Mage2 and shape IDs38/39/40. The raw class-row mapping in HudManager remains
290..292→2, 325..327→1, otherwise0; these numeric row ranges must not be
renamed by their order. Current actual Warrior screenshot selects frame0.

The Warrior shape bounds are `[-222,837,-161,869]` twips. Bitmap1 fill matrix
has scale15.10906982421875, translation(-10293,-10052). Its HudChar parent
has authored translation327,324; btimg has authored scale.8295745849609375,
translation95,18. These are actual source offsets and atlas coordinates,
not grounds for a visual nudge. Current screenshot alone does not establish
a causal portrait-placement error.

New `authored_hud_portrait_v4` is a read-only same-movie measurement: it returns
the actual button/btimg/HudChar frame and local/world transforms, actual child
shape ID and virtual transformed bounds, plus retained source display rectangle
and viewport. Root can log these on the current bound HUD to compare the live
source path before applying an alignment correction. Its TU passes strict
ARM64 syntax and is in engine-ui CMake. Portrait alignment is not claimed fixed
until the actual current movie and viewport are measured and visually verified.
