# Original menu/HUD resource and layout inventory

This is a source-discovery handoff, not a native UI parity or live-rendering claim. It changes no renderer, CMake, existing runtime, or queued-startup module. The original ARM32 ELF is a desktop reference only.

## Authority and reproducible artifacts

* ELF `.local-inputs/libDungeonHunter2.so`: SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
* Cache ZIP `C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip`: previously bound SHA256 `3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
* `swf-inventory.json`: all 30 cache SWFs, exact member names/hashes, checked decompressed declared lengths, authored stage/frame rate, recursively bounded tag inventory, exported resource names and frame labels.
* `menu-resource-table.json`: all 28 pointer entries read from original table `0x956a64`.
* `source-global-bindings.json`: original GOT references resolved through ELF relocations, including `Width_Screen`, `HTC_DEVICES`, `NO_IGP`, `gameswf::s_render_handler`, and the texture-loader callback.
* Complete symbol-sized routine bytes/hashes and ASM are in the manifest/`reference` files here and the five subdirectories. There are 45 captured entries in total. `MenuManager::Init` has embedded literal pools; the common capture disassembler stops at invalid data. Its whole-byte hash still covers the full function. Discovery `.local-inputs/ui-layout-discovery/annotated.asm` uses Capstone skip-data to traverse the remaining branch destinations; disassembled literal words are data, not executed instructions.
* Discovery scripts: `.local-inputs/ui-layout-discovery/{inventory,swf_probe,path_probe,annotate,bindings}.py`. `swf_probe` is a bounded inspection tool; it is not a production movie/ActionScript interpreter. `first-frame-paths.json` and the four `*-tree.json` files preserve the inspected placements/matrices/clip depths. Static first-frame traversal does **not** run initialization actions or imply that every placed menu is visible.

## Actual menu path

The game's primary UI is **GameSWF/RenderFX**, not the separate `glitch::gui::CGUIEnvironment` XML infrastructure. Original source names include `src_gameswf_gameswf_render_handler_glitch.cpp`; no precise upstream GameSWF version or drop-in compatibility is established.

| Source | Exact behavior / connection |
|---|---|
| `MenuManager::Init` `0x42f304` | Incremental loader/registration state machine; slots 0 shared, 1 character/inventory, 2 main menus, 3 HUD. It also registers the game's native ActionScript callbacks. |
| `MultiMenuManager::LoadSWFFile` `0x437d68` | Reuses an existing loaded slot; otherwise owns a new `MenuFX` and `MenuFlash2DCamera`, enables text buffering. |
| `RenderFX::Load` `0x7ab784` | Owns `gameswf::player`, sets its working directory from the file path, calls `player::load_file` `0x7736f4`, retains root and sets context to the root character. |
| `MenuManager::PostLoad` `0x42efb8` | Discovers named `menu_` contexts and registers menus after resource loading. |
| `RenderFX::Update` `0x7ad68c` | Converts the integer elapsed milliseconds to float and divides by 1000 before `root::advance(float,bool)` `0x775304`; then processes cursor state. There is no wall-clock-modulo substitute. |
| `MenuManager::Draw` `0x42e818` | Checks actual DebugSwitches `IsDeactivatingFlashMenus` and `IsDeactivatingFlashMenusRender` before drawing. |

Resource selection is proved by the Android ELF, not by assuming the meaning of filename suffixes. `MenuManager::Init` reads actual `Width_Screen`: width 854 selects table offset `0x40`, hence the `_droid` quartet. Width 800 uses `HTC_DEVICES` / `NO_IGP` branches and the i9000/LG quartet; width 960 uses the base quartet. Other paths include saved-language decisions. Exact device/global producers still require their initialization/caller audit before general runtime selection is implemented.

| Slot | Base resource | Width-854 resource | Authored content |
|---|---|---|---|
| 0 | `data/menus/dqshared.swf` | `data/menus/dqshared_droid.swf` | Shared controls, icons, text styles, HSlider/button classes. |
| 1 | `data/menus/dqcharmenu.swf` | `data/menus/dqcharmenu_droid.swf` | Inventory, equipment, character sheet, skills, faery, quest/map and merchant screens. |
| 2 | `data/menus/dqmenus.swf` | `data/menus/dqmenus_droid.swf` | `menu_MainMenu`, `menu_StartGame`, class selection, splash/name/language and multiplayer screens. |
| 3 | `data/menus/dqhud.swf` | `data/menus/dqhud_droid.swf` | Four HUD layouts, HP/MP/XP, potion/skill/spell controls, minimap/travel/dialog/death/fade/SCT clips. |

The droid files are ordinary uncompressed `FWS`, version 8, authored **480×320**, 30fps except `dqshared_droid` at 12fps. The main-menu root has two frames; HUD/character/shared roots each have one. Base large resources often use compressed `CWS` and 1024×768. Resource sizes and hashes are in the inventory. The assets contain vector shape records as well as external bitmap placeholders, nested clip timelines, text definitions, ActionScript `DoAction`/`DoInitAction`, masks and color transforms. Rendering a single atlas without these producers cannot reproduce these screens.

## Hierarchy / layout / aspect / input

Placement records are packed `PlaceObject2`/`PlaceObject3`; their depth, move/replace flags, referenced dictionary ID, optional instance name, matrix, color transform and clip depth define the authored hierarchy. Matrix scale/skew are signed 16.16; translations and shape bounds are signed **twips (20 per pixel)**. This is not an invented pixel/anchor schema.

The parsed droid HUD root contains the exact instances `menu_HUD_0` (dictionary 470), `_1` (462), `_2` (466), `_3` (453). These placements are distinct authored layouts. Original `HUDControls::initCachedChars` `0x419b4c` and `InfoHUDManager::initCachedChars` `0x41d880` read saved `HUDStyle`, format `_root.menu_HUD_%d`, and resolve relative children, including:

* `HUDelements.HealthBars.player.bar_hp`, `bar_mp`, `bar_xp`
* `HUDelements.HealthBars.btn_potion.cnt.value`, `DistressGlow`
* `HUDelements.controls.controls.Joystick`, `.Joystick.stick`, `btn_interact`, `btn_spell`, `btn_skill1/2/3`
* `HUDelements.btn_charactermenu`, `HUDelements.btn_mainmenu`

Those exact paths also occur in the parsed authored dictionary; no replacement layout needs to be guessed. Example: player clip 157 contains `bar_hp` 90, `bar_mp` 147, `bar_xp` 152. Static first-frame hierarchy is preserved in discovery; actions can subsequently change frames, visibility, matrices, and callbacks.

`HUDControls::SetHUDPos` `0x4187a0` divides the supplied float positions by 20, converts through the original float-to-integer helper, and calls `GameSWFUtils` positioning services. The original authored placements and this native customization path are distinct from an Android view anchor system.

`MenuFlash2DCamera::Update` `0x42cd84` is the concrete viewport owner: signed integer camera offsets ease toward desired offsets by `1 + trunc(distance/10)` per update; optional captured clip bounds clamp them. It passes the actual driver dimensions to `RenderFX::SetViewport(0,0,w,h)` `0x7a9bac`, followed by `RenderFX::SetBounds(offset_x,offset_y,w,h,scale_mode=0)` `0x7a9b30`. This is frame-call-driven easing, not an elapsed-time interpolation. `SetLimitClip` `0x42cf30` captures the clip rect when set; it does not recompute it every frame.

`root::set_display_viewport` `0x775d38` stores the viewport only on change, then invokes `set_display_bounds` `0x7755f4`. That routine contains separate scale modes, renderer-orientation branches and an ActionScript bounds-change notification. `root::screen_to_logical` `0x773dc0` and inverse `logical_to_screen` `0x773f50` use the actual stored bounds plus movie rect and orientation; orientations 0/2 differ from the other orientations. A universal centered aspect-fit matrix has **not** been proved. Port these coupled producers together and retain the native renderer orientation service.

Hit testing is also authored and source-ordered: `sprite_instance::get_topmost_mouse_entity` `0x77f00c` tests visibility, transforms coordinates by the inverse local matrix, traverses child entries from the end, invokes child virtual hit tests and mouse-capability gates, and includes a name sentinel branch. `shape_character_def::point_test_local` `0x77a570` first rejects points outside shape bounds, then tests actual paths `0x779ff4`; an axis-aligned button rectangle alone is insufficient. `display_list::display` `0x7552ac` traverses drawing entries forward and executes clip-depth mask transitions. `RenderFX::UpdateInput` `0x7ac4bc` sends the native event before subsequent button handling and honors event consumption. Its `UpdateCursor` backend and original mouse-event generation remain required for full control parity.

## Draw and texture boundary

The original concrete backend factory is `gameswf::create_render_handler_irrlicht` `0x7d6658`, producing `render_handler_glitch`. Captured entries include `begin_display` `0x7d73d0`, `fill_style_bitmap` `0x7d4558`, and `draw_bitmap` `0x7d8df4`. Typed shape tessellation/mesh submission, cxform, bitmap fill matrices/wrap, clipping, text, blending and cache ownership must feed the native 2D shader backend. The effects worker has verified authored `GameSWFVS.glsl`, `GameSWFFS.glsl`, and `GameSWFFS_blend.glsl` in the original shader pack; this inventory does not claim they are wired to live draws.

`export_loader` `0x75c5f8` can call `load_external_texture` `0x759cfc`; it asks the registered callback for the exported name and bitmap dimensions, then replaces the placeholder texture if supplied. `GameSWFUtils::SwfTextureLoader` `0x416c14` is captured completely. It recognizes special splash/dhalliance names and selects exact language-dependent `data/3d/textures/...` paths; the generic path is **`data/%s`**, passed to `CTextureManager::getTexture` `0x5ed210`.

Actual droid exports include `menus/MenusGraphics_droid.tga`, `menus/splash_final_droid.tga`, `menus/iPhone_Table_DH2.tga`, `map_top.tga` and `map_bottom.tga`. The cache physical texture paths commonly live under `data/3d/textures`, while generic requested names are `data/menus/...` or `data/...`. The texture manager's alias/search/resource ownership must be recovered; mapping by basename is presently only a hypothesis, **not** a source-backed resolver. The effect/texture worker was notified of this concrete boundary.

`MenuMainMenu::SetupScene` `0x42c510` separately loads `data/3d/menu/main_menu_charactere_swamp.bdae`; `Show` `0x42c62c` resolves `menu_bg`. Existing native assets already contain the corresponding backdrop BDAE and Prince menu idle animation. The 3D avatar/background is separate from the Flash UI layer and should keep its actual camera/ownership path.

## Recommended first connected visible slice

Render the **authored HUDStyle-0 player status panel over the current native scene**, using `dqhud_droid` dictionary 470 → `HUDelements.HealthBars.player` and its exact HP/MP/XP/potion subtree. This gives a useful visible bridge to existing genuine native character/property/item data with fewer menu/session dependencies than a complete multiplayer/main menu. The main menu remains the next complete-screen target, with its already available 3D background/Prince assets.

Implementation order:

1. Owned bounded SWF dictionary/display-list reader: preserve raw ActionScript bytes and unimplemented tags explicitly; decode exact signed RECT/MATRIX/CXFORM, shapes/fills, nested timelines/placements, exports and masks. Validate against actual original readers, not only this Python inventory. Do not flatten away reused clip identities or per-instance frame/visibility state.
2. Original viewport/layout and shape tessellation → native GameSWF draw sink, consuming source shaders, authored atlas resolver and glyph producer. Initially label isolated status-subtree presentation truthfully; selecting it is an explicit port integration scope until full initialization actions execute.
3. Source-backed `InfoHUDManager`/`NativeGetPlayerHUDInfos` adapter updates the exact retained clips from genuine player data. The latter `0x44e5cc` emits `HP_PCT`, `HP_LOWPCT`, `MP_PCT`, `XP_PCT`, `SPELL_PCT`, `SKILL1_PCT`/2/3 and `NB_POTIONS`; it is captured but not reconstructed here. No constant percentages or invented skill availability.
4. Original input transform, path hit tests, cursor events and native callback dispatch. Only then connect character menu, inventory and start/load-screen actions to their real service owners.

Suggested ownership contract: immutable `MovieResource` owns decompressed bytes/dictionary/track/action records; `MovieInstance` owns retained display-list instances, per-clip frame/play state, variables and masks; borrowed `UIDriver` supplies orientation/dimensions/draw sink/textures/glyphs; borrowed native callback registry supplies real game methods. Keep script callbacks synchronous and honor source event consumption/reloads. These are proposed interfaces, not completed code.

The next consequential milestone is an authored, correctly transformed HP/MP panel visibly attached to the genuine player state. This discovery alone supplies no native ARM64 UI execution, sanitizer corpus, complete ActionScript VM, menus/inventory interactivity, or screenshot proof.
