# Frontend handoff — 2026-10-09

The native frontend is now interactive and substantially closer to the supplied **original gameplay** images. Full original fidelity and complete new-game persistence remain unfinished. Stop the previous expensive workers; Luna replacements can resume the bounded ownership below. No frontend edits should touch root main/core/CMake, accepted releases or user profiles.

## Ownership and worker checkpoints

All six original children are completed and checkpointed; no running child needs to be interrupted.

| Scope | Last worker | State / next useful work |
|---|---|---|
| `features/frontend/art/` | `menu_art` | Complete generic-original movie/texture/stop-frame recovery. Wait for real AVM/mask/reflow work; do not repeat unchanged art QA. |
| `features/frontend/flow/` | `menu_flow` | Source navigation and borrow-only canonical service boundary. Bind only after a genuine indexed-slot and launch owner exists. |
| `features/frontend/creation/` | `creation_data` | Exact class metadata/text and full-factory dependency audit. Coordinate the new source-character factory and campaign owner; no fixture initialization. |
| `features/frontend/preview/` | `creation_preview` | Source showcases, camera, endpoint hold and exact light metadata. Next: actual SAME Character/FSM idle provider and real LightPoint/material ownership. |
| `features/frontend/input/` | `frontend_input` | PC/touch authored path controller and source name policy complete. Fresh input epoch required on actual actor/profile/level replacement. |
| `features/frontend/verification/` | `frontend_verify` | Original screenshot comparison and camera-independent body-motion checks complete. Recheck only after a real renderer/provider change. |

Lead-owned files: `interactive.cpp`, `native_host.{hpp,cpp}`, `rich_text.{hpp,cpp}`, standalone `CMakeLists.txt`, frontend README, `reports/feature-frontend.json`, and `.local-inputs/frontend-feature-build/`. The obsolete static prototype is archived only in that ignored build directory.

## Current build and checks

- EXE: `.local-inputs/frontend-feature-build/dh-frontend-diagnostic.exe`.
- Latest SHA256: `fb0652c2289985beb3e4baf1d22a9d78ea53805e02840ce7f49a6cd82eced0e5`.
- Latest build passes with canonical navigation, exact endpoint hold and `class_light_source.cpp` included automatically by the feature CMake glob.
- Latest non-GUI suite: **8/8 PASS, 0.75s**, `ctest-components-latest.log`.
- Prior isolated full suite: **8/8 PASS, 4.93s**, including actual Win32 event smoke, `ctest.log`. There are now nine registered tests after adding the canonical contract test; GUI smoke was not rerun after the final metadata/endpoint additions.
- Actual own-HWND smoke used mouse down/up, WM_CHAR `QA`, Return, Warrior→Rogue→Mage clicks after each source transition gate, and Confirm. It reached explicit `NativeCreateSaveSlot owner unavailable` without any profile write. One earlier concurrent GUI run failed to reach the boundary; isolated reruns passed. Coordinate root's GUI window before rerunning.
- Host, text renderer and input host passed `clang++ -std=c++17 -Wall -Wextra -Werror -fsyntax-only` before the final metadata update; the latest ordinary build also passes.

Toolchain cache already configured: llvm-mingw under `.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64`; CMake/Ninja from Android SDK `cmake/3.22.1/bin`. The feature target links existing `windows-foundation-build` archives and does not change root CMake.

```
cmake --build .local-inputs/frontend-feature-build -j 6
ctest --test-dir .local-inputs/frontend-feature-build -E frontend_native_event_smoke --output-on-failure
dh-frontend-diagnostic.exe --assets port/android-native/app/src/main/assets --verify-native-input --frames 500
dh-frontend-diagnostic.exe --assets port/android-native/app/src/main/assets --screen class --class 1 --frames 100 --fixed-step 0.016 --capture rogue.ppm
```

## Reference and source facts to preserve

- Original user images are retained unchanged under `.local-inputs/frontend-feature-build/verification/original-{main,name,class}.png`, with hashes. **Reconstructed Android output is not a reference.** Initial static diagnostic failed this gate.
- Current corrected captures: `name-corrected.png`, `rogue-corrected.png`, plus high-resolution `name-corrected-1080.ppm`. Name now has the genuine carved backdrop, red QWERTY/numbers, gold Confirm and framed Shift arrow; class has the ornate header, all three actors, selected Rogue crossblade showcase and complete colored descriptions.
- `showcase-knight/rogue/mage/` contain native PPM samples at 16…3840ms; corresponding MP4 previews are sampled at roughly 256ms, with no invented animation. `verification/check_motion.py` independently proves body changes after the camera settles, rather than claiming motion from camera movement.
- Actual source UI is **complete retained generic `dqmenus.swf`**, not the failed droid-placeholder/hybrid graph. Its repeated ExportAssets filenames refer to different bitmap definitions. Nine exact embedded textures are under `art/assets/`, with `source_bitmap_receipt.json`; staged copies live next to the EXE in `ui-assets/`. `--ui-assets` can select an explicit directory.
- Gold Name Confirm comes from source lowercase `idle` → real STOP frame4; Shift uses the recovered `released` state; ornate class heading requires mixed bitmap fills. There are no tints or replacement artwork. Original vector antialias callback is a no-op; compare matching raster resolutions.
- Original route: Main empty slot → EnterName → SelectClass → actual create → actual assign → pop above Main → StartGame; start submenu assigns before starting. Raw name is retained; ASCII trim-only empty rejection and original keyboard append cap apply.
- First class update must run the selected actor's **MenuOnSelect**, previous index starts `-1`. Knight/Rogue clips are 5000ms; Mage is 5366ms. Native camera routes by destination, not a guessed initial idle. Dummy transforms override the visual property scale, so draw `anchor`, not `anchor * actor.scale`.
- Camera's preserved **4:3** aspect is verified directly from the original ARM ELF constructor. Do not restore viewport-derived aspect0. Source class-select bounds are exactly fullscreen after normalization.

## Genuine missing root owners

1. **Complete source Character factory and indexed save-slot graph.** Read `creation/FULL-CREATION-OWNERS.md`: `FreshInventoryOwnedV4`, `CanonicalCharacterCandidateRecordV60`, `CharacterPlayerSkillsV6`, `CharacterProfileBootstrapV59`. Metadata-only creation is not an initialized Character. Root's new `/root/integration_lead/source_character_owner_factory` and `/root/acceptance_audit` have this evidence.
2. **Explicit compatibility projection with retained native authority.** Actual schema has Strength149, Dexterity150, Endurance151, Energy152 and no Intelligence field. Native saved data includes zero-ranked skills, slot maps, faery, difficulty and level/quest state. Do not silently flatten/drop these or invent an Intelligence value. Current CharacterState is a view, not the whole source save graph.
3. `creation::CompleteCreationService(request, ClassChoice, error) -> optional<CharacterState>` must supply complete genuine initialization before persistence. `flow::CanonicalNavigationOwner` borrows the SAME live CharacterState on every call; it stores no slot/profile graph. Actual `create_slot`, `assign_slot`, `start_game` owners remain absent. Ordinary GameSave checkpoint serialization is not indexed SG_Save/SG_Load.
4. **Actual idle continuation:** `CreationPreview::set_idle_transition_provider(actor,error)` must bind SAME `CharacterAnimationInstance`, `NativeFsm24`, `CharacterStateOwner`, stance facts and ordinary animation owner. CSAnim event34/0x22 → SM_SetIdleState is real; current providers are missing. Showcase endpoints now hold exact source geometry and expose `idle_transition_required()`. There is no stand-in MenuIdle fallback.
5. **Actual light/material chain:** first authored Omni node → spawned LightPoint/type19 → SAME CLight → AssignTweaker PlayerLight set0/slot0 → scene automatic light list → VisualObject light filter → SceneManager UpdateLightSet → SAME CMaterial cells/uniform program. `ClassPreviewScene.light_inputs()` contains exact source setters and normalized attenuation only; this is not a provider. `port/level-world/menu_class_select_light_owner_v1.cpp` was observed as an unfinished draft; identify its owner before using it.
6. **Actual gameplay input replacement:** `InputOwnerEpoch::before_replace` must deliver cancel/release edges to OLD command owners before advancing and borrowing the new actor/controller/level. Never retain gameplay/CharacterState receiver pointers across replacement.

## Remaining acceptance and next bounded steps

Independent QA now accepts the name/class **screen structure**, not pixel parity. All three genuine showcase motions are verified. Remaining visible class gaps: small finite-floor cutoff, oversized foreground edge/rock and darker material response. Source coverage audit proves no missing/animated backdrop: all visible instances decode, static world matrices remain exact at 15 timestamps, and projected triangles truly leave the corner uncovered. Fullscreen callback bounds are also proven. Do not fabricate terrain, camera offsets, tint or crop.

The supplied main image is an equipped Archer profile. No actual matching saved profile/campaign input exists; never fabricate David/level22 or substitute a starter model. Full AVM lifecycle, masks/reflow/device formatting and physical touch remain unaccepted.

Next replacement worker should integrate **a real owner** when the factory/campaign/FSM/material work is ready, then capture full 5s/5.366s endpoints and >endpoint hold/status at one shared millisecond clock. Root was asked to coordinate ~20s GUI time after its combat-text checks; do not launch conflicting windows. Existing four-second MP4 previews predate endpoint-hold GPU captures; the latest asset tests prove endpoints against direct original animation sampling.

No user profiles were written. Accepted gameplay releases and root main/core/CMake remain independent.
