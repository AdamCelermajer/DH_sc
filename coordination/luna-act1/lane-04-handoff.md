# Lane 04 handoff: preview rendering and camera

Status: source review complete; corrected camera parameters. Actual preview-character GPU draw remains for root integration because the draw loop and `model_renderer.cpp` are root-owned.

## Lane-local change

In `port/android-native/app/src/main/cpp/native_menu_preview_v121.cpp`, fixed `CameraProceduralNodeV16::set_data(fov, aspect, ...)` argument order. IDA assembly for `MenuMainMenu::CreateAvatarCamera` at `0x42bf3c` calls vtable offset `0x138` with `0x3FD578E9` (FOV 1.67), then `0x13c` with `0x3F3579C8` (aspect 0.71), grabs the source static reference, and sets near/far to 10/2000. V121 had those first two float arguments reversed. Position/target words, up vector, scene-manager attachment, active-camera publication, reference grab/drop, and removal paths agree with the source call order.

No builds or tests were run, per lane instructions.

## Existing V121 hooks and retained owners

- `native_menu_preview_camera_view_v121(app, view, present, error)` returns the real V121 camera view only when that camera is alive and is the same active `SceneManager` camera. The renderer should use its `projection` and `view` matrices for the preview draw. It must not step a second camera/scene clock.
- `native_menu_preview_camera_v121(app, camera, error)` loans the retained camera node; its view is maintained by the same `CameraProceduralNodeV16`.
- `native_menu_preview_prepare_main_v121`, `native_menu_preview_select_hide_v121`, `native_menu_preview_main_hide_v121`, and the destroy adapters already route through one process preview owner and the actual SceneManager/ObjectManager/AnimSetManager. Hide removes the retained scene root, flushes source owners, frees textures/cleans Glitch, removes/drops the camera, clears the real slot, and stops music.
- `renderer_native_menu_preview_v121.inc` constructs the authored `main_menu_charactere_swamp.bdae` as a retained scene root on that existing scene manager. It does not construct a World or Level.

## Root-owned draw wiring

IDA `MenuMainMenu::RenderCharacterPane` at `0x42bb44` is only `BX LR`; it does not draw the character. The source `Show` at `0x42c62c` creates the avatar camera, calls `SetupScene`, calls `SetupCharacter` if the static Character pointer is null, then registers that callback. Actual scene rendering therefore belongs in the renderer's normal preview frame.

The current root renderer still selects its synthetic `menu_camera()` for `menu_background` and calls `renderer_front_draw_v87.inc`, which draws `class_preview_actors` only. Root should:

1. During the MainMenu preview frame, query `native_menu_preview_camera_view_v121` and use its real `projection * view` with the surface viewport before drawing.
2. Resolve the live preview Character using `borrow_native_menu_preview_domain_v121` plus `borrow_native_menu_preview_character_v122(domain.owner, character.identity, record, matched, error)`; require a matched record, its actual `record->visual->visual()`, and a ready retained visual. Keep this visual on the same retained root and sampled scene clock.
3. For a non-modular base visual, use the existing `dh2::world::RetainedVisualDrawV27` path (`bind`/`refresh`) and the retained part materials/transforms. For modular saved equipment, take the same record's actual `equipment->draw_views(...)` and pass them through existing `sync_visual_draws_v64`; do not substitute class-selection actors or a second character.
4. Keep root scene/camera teardown paired with the existing V121 Hide adapters. Do not add a second renderer-side owner or make the SWF callback responsible for drawing.

The V121 camera view API already exists; no additional lane-local camera interface is needed. `RetainedGameObjectVisualV1` exposes modular draw views but keeps base mesh graph internals private, so root should reuse `RetainedVisualDrawV27` for the base. Shared renderer edits are outside lane 04's writable paths.

## IDA evidence and ABI boundary

- `0x42bf3c`: `CreateAvatarCamera`; original ARM32 constructor signature is `CCameraSceneNode(int, vector3d<float> const&, vector3d<float> const&, bool)`. The three vectors are 32-bit source floats; the native implementation converts the captured words before calling its typed host wrapper.
- `0x42c510`: `SetupScene`; loads physics with `(0,0,1,1)`, constructs `data/3d/menu/main_menu_charactere_swamp.bdae`, adds the returned scene node, drops its constructor reference.
- `0x42bd08`: `SetupCharacter`; guard requires slot != -1 and a non-null scene node; calls `SG_Exists`, `Character::CreatePlayer(... "PlayerCharacter_0" ...)`, conditionally `SG_Load(4)`, attaches Character `visual2d8`, sets position `(0,-200,-20)`, quaternion from `3.1416 * -0.125`, then visible=true.
- `0x42bcbc`: `DestroyCharacter` flushes ObjectManager, flushes AnimSetManager, then nulls the static character.
- `0x42bebc`: `DestroyScene` removes and clears the main root, calls `SceneManager.RemoveAll`, destroys the character, frees textures, and cleans Glitch.
- `0x42baec`: `DestroyAvatarCamera` removes the camera from its root, drops the source static reference, then clears the static.
- `0x42c128`: `Hide` calls DestroyScene, DestroyAvatarCamera, sets destroyed=true and slot=-1, then `VoxSoundManager::StopMusic(1000)`.
- `0x42bb44`: callback is a one-instruction return stub.

## Blockers and runtime

Character draw submission and use of the real camera in the shared renderer remain unintegrated until root changes the shared `model_renderer` draw selection and connects the lane 03 canonical Character record. The character's draw payload must remain the original retained preview visual and saved Gear owner. No runtime verification was attempted. The model/reasoning setting requested by the coordinator is GPT-6 Luna/high; this worker cannot inspect its runtime model configuration through available tools.
