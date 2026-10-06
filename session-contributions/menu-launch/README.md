# Isolated launch and menu contribution

Source snapshot: `../dh2`. Original checkout: `C:\Users\adamc\Desktop\workspace\DH_sc`.
All implementation and emulator changes from this contribution are confined to the snapshot and dedicated emulator. The original checkout and emulator-5554 were not modified by this contribution.

## Verified media milestone

`media-tests-v7/media-validation.json` records PASS on emulator-5580, using its own ADB server 5038. Tested landscape surfaces: 1920x1080, 2400x1080, 2184x1968. Real original intro video changes visibly, fits without stretching/cropping, and retains position across Home/resume. The full 50.292-second intro naturally transitions to the authored main menu; Android Back also transitions to it. Title music plays its original 5.832375-second intro followed by its 113.214-second loop.

Screenshots, per-scenario logcat, and AudioFlinger dumps are in `media-tests-v7/`. These tests establish media behavior; they do not establish menu functionality or audible output quality by listening.

Reuse candidates are `CinematicActivity.java`, `FrontAudio.java`, `decode_title_music.py`, and `assets/original-media/`, together with the manifest registration and relevant MainActivity lifecycle/intent/audio hooks. Source locations are under `../dh2/port/android-native/`. The activity is private, reached through MainActivity's boolean `cinematic` extra. Intro playback is currently opt-in, not the default launcher path.

The MP4 is byte-identical to the original cache resource. Music is decoded from original VXN segment metadata to PCM; provenance and sample counts are in `title-music-decode.json`. Menu sound effects are now connected through NativePlaySoundFX and the Android playback owner; see the v23 milestone below.

The source-frame build also passed the complete media check in `media-tests-v19/media-validation.json`, including natural video completion and Home/resume. Latest APK checkpoint v20 has a teardown-only owner-release change after the movie is destroyed; main text and loading checks are tied to that exact APK hash.

## Menu work remains experimental

The main and loading clips render from original SWFs with a centered 3:2 authored canvas. Main `onShow` executes. The main button labels now display Start game, Options, Info, and green MORE GAMES! through a connected original `text_layout_v1` path. `text-tests-v20/text-validation.json` and screenshots establish those labels and genuine FT face metrics at all three sizes. Font metrics (2048 EM units, 2341 ascender-minus-descender) match the exact original Fontin SmallCaps font tables. Callbacks for saves/settings/navigation are not bound, and touch now activates the authored main button handlers through a direct native event stage; navigation is still unbound. Full scene background and game menu integration are also outstanding. Do not treat this as a functional menu or merge the entire MainActivity/native changes without review.

The text property overlay leaves frozen vendor bytes unchanged. It adds htmlText dispatch, routes the retained field to the original layout/parser, converts actual glyphs to display records, and retains cloned font lifetimes. Glyphs use the real existing provider and source bitmap pipeline; metrics come from the real FT_Face. Inline image and underline display remain required-provider errors, rather than substituted glyphs. Stock matrix-triggered reformatting now preserves field HTML mode. The final display body and every text-format setter are not yet proven equivalent to the original engine. See `MENU-TEXT-FINDING.md` for the original ARM setter evidence.

MainActivity currently defaults to the main menu in this snapshot, changing the original Crypt launch behavior. The isolated Gradle configuration builds x86_64 only. CMake job limits and short-drive mappings are local build accommodations. Current native diagnostic logging is investigative, not a finished engine change. The isolated Android build now links the original frame-v1 and input-v1 overlays. An owned frame/history receiver is bound before any shared/root construction and retained through movie teardown. Menu construction and advancement now execute that source scheduler. The v25 input milestone connects a direct native main event stage and actual touch activation. MenuManager/HUDControls forwarding and navigation ownership still require integration. Other users of this experimental engine build must install their frame/history owners before creating characters.

`loading-tests-v20/loading-validation.json` proves visible motion and pre-construction frame ownership at three sizes. Visual inspection shows only a small rotating texture fragment on an otherwise empty surface. Correct texture-region and native loading background/progress ownership are not verified; this is not a finished loading screen.

## Isolation and snapshot receipts

The emulator window is `Android Emulator - DH2_Launch:5580`, with dedicated fresh userdata under `../emulator/`. It is visible and audio enabled. Use only `adb -P 5038 -s emulator-5580`. Do not stop or reuse emulator-5554. Each size test restores this dedicated emulator's prior display override.

The full working copy includes assets, .git, untracked files, and build caches. `snapshot-copy.log`, `source-head.txt`, `source-status.txt`, and `snapshot-source-sha256.json` record the captured baseline. One unreadable generated original file could not be copied: `port/android-native/build/reports/configuration-cache/2c878sb7255wjoyqdpf6a8rtq/8s8ls8znkqu0so15e58ky07b2/configuration-cache-report.html`. No reconstruction input was identified as missing.

The DeepSeek directory was inspected read-only. `deepseek-comparison.json` found an older audit/copy, with no unique source implementation to reuse. Original file baselines for all modified files are preserved in `baseline/`.

The main chat has not been interrupted or sent messages. It can review this directory when the user directs it here. Work remains in progress; the reconstruction goal is not complete.

## Menu sound service milestone v23

`launch-build-v23.log` is a successful x86_64 build. `sound-tests-v23/sound-validation.json` ties verification to its exact APK hash. The actual authored Options `onRelease` handler requests MenuConfirm and reaches Android playback at all three test sizes. MenuBack, MenuConfirm, MenuSelect, MenuSpending and MenuTab all start and complete. Invalid argument shapes and an unknown sound name produce no sound request. Pausing during an effect releases its MediaPlayer. The emulator is returned to the main menu and its previous size override.

`menu-sound-assets.json` records the source table hashes, sound count and menu mappings; the generated lookup retains all 183 original Arrays::Sounds names. `recover_menu_sounds.py` parses the full 11295-byte sound array, preserves original IDs, extracts exact menu WAV bytes, and generates the name/file lookup. NativePlaySoundFX uses the typed AS provider and the original one-string argument/name-miss gate (original ARM entry 0x43ae10). The pinned upstream core represents strings as UTF-8; it has no distinct original wide-string tag. A known sound without an implemented backend remains an explicit required-provider failure.

The shell-only, DUMP-protected DEBUG_MENU_SOUND probe invokes the authored Options handler or the real AS NativePlaySoundFX global. It does not emulate a touchscreen or establish navigation. NativePushMenu, source menu-manager/input ownership, save/settings callbacks and main scene rendering are still missing. Effects currently use the original menu records' default volume of 100; integration with persistent user sound settings and full Vox channel/mixing semantics is outstanding. Title music still starts from activity screen selection, rather than authored NativePlayMusic. No listening-based audio quality claim is made.

Reuse files include `swf_menu_sound.{hpp,cpp}`, `original_menu_sound_data.hpp`, FrontAudio/MainActivity/NativeBridge and original_ui_session/native_app changes, plus the generator and smoke tool. The engine CMake source list adds the thin typed argument adapter. Review the updated contribution patch against preserved baselines before integrating.

The exact v23 APK also passed the full media regression in `media-tests-v23/media-validation.json`: video fit/motion, Back-to-main, title intro-to-loop at three sizes, Home/resume (2456 to 2561 ms), and natural intro completion. `media-tests-v23/restored-main-settled.png` is the final visible main menu after its authored entry animation settles.

## Main touch milestone v25

`input-tests-v25/input-validation.json` and screenshots verify real Android tap/swipe input, source hit testing and button press/release/clicked events, authored MenuConfirm playback at three resolutions, margin/outside-release rejection, live resize in the same process, and held-touch cancellation across Home/resume. `checkpoints/front-v25.apk` preserves the tested build. It still does not navigate to Options: original NativePushMenu/NativePopMenu and the MultiMenuManager stack/lifecycle owner remain missing. See FRAME-INPUT-INTEGRATION.md for exact owner scope and reuse requirements. The loading appearance and 3D main scene are also still incomplete. Media regression evidence remains tied to v23; the v25 input/lifecycle tests verify this changed touch integration.

## Timing and original input initialization v28

`menu-frame-clock-audit.log` verifies the compiled C++ clock on emulator-5580: ten seconds deliver exactly 10000 milliseconds at simulated 60, 90, 120, 144 and 240 Hz, preserving fractional elapsed time, with a 100 ms gap cap and clean reset. Previous per-frame truncation delivered only 8640 ms at 144 Hz. This tests time conversion, not measured emulator refresh rate.

`audit_menu_stack.py` independently extracts original ELF strings and source disassembly into `menu-stack-source-v27/`. LoadMainMenu explicitly selects input flags 0x84 on renderer slot 2 (0x4325ac); these replace the prior constructor-zero flags. PostLoad finds all names containing `menu_`, and RegisterState hides those real characters before activation. The new facade connects that visibility substage only; native state Create, stack ownership and lifecycle callbacks are still unconnected.

`input-tests-v28/input-validation.json` records PASS for the exact v28 APK: real tap/press/release/clicked, sound, rejected margin and blank-area release, held-pointer focus transfer Options-to-Info, live resize and Home/resume at all three sizes. Original 0x80 behavior can transfer focus to a non-mouse blank shape, so blank release need not emit Options kind 7; the test now asserts actual focus loss and no activation. A settled explicit DOWN/MOVE/UP sequence verifies cross-button focus transfer; a continuous swipe traversing animated intermediate controls did not meet that test's assumed Info endpoint. Earlier failure logs are retained as investigation evidence. Main is left visible after tests. The loading fragment, 3D background, save/settings, game menu and destination navigation remain incomplete.

The same v28 APK passed `media-tests-v28/media-validation.json`: real video motion and aspect fit, skip/Back-to-main and title intro-to-loop at three sizes, Home/resume (2282 to 2380 ms), and natural video completion. The visible dedicated emulator is restored to main with no display override; `media-tests-v28/restored-main.png` was inspected. `checkpoints/front-v28.apk` and `front-v28-source/` preserve the tested APK and contribution sources. These checks do not establish listening quality, the full audio settings backend, or complete menus.

## Native option reader v29 (not connected to the displayed menu)

`swf_menu_options.{hpp,cpp}` adds the source NativeGetOptionParameters body (0x44a298), using actual OwnedHudSettingsV1 getters and the supplied ActionScript object. It writes NumOptions, CurrentOption and OptionString in original order, preserves object identity and member-setter behavior, converts the first argument through the real AS string conversion, ignores extra arguments, preserves the result on a null object, and retains the Sharp Language maximum/observation branch. Missing reached StringManager or Sharp observation providers fail explicitly. Short/null-stack host calls are rejected rather than reading invalid stack memory. The adapter is linked, but is not registered as a production native action yet.

OwnedHudSettingsV1 now exposes original getOptionMax (type 2 maximum minus one, genuine miss -1) and getOptionString (descriptor value-string base plus current, genuine miss -1), with source 32-bit wrap arithmetic. Original getter disassembly and decoded property names are retained in options-source-v29/.

`options-tests-v29/option-validation.json` records an Android executable audit against the actual 16-record design table and real AS stack, root and objects. Its StringManager and Sharp callbacks are explicit fixtures, not application implementations. The real file provider and a deliberately unavailable scene cause load rejection; the test uses the genuinely retained partially initialized option map, and does not claim loaded settings. Environment construction initially failed because the test player had no root; the corrected test creates an explicit empty AVM1 fixture with real pre-construction frame/history owners. This fixture is not an application scene. The final audit passes identity, setter order/ignored return, null receiver, source Language maximum 7, Sharp maximum 5, numeric name/miss, ignored extra arguments, short-call rejection and the no-string volume branch.

Options is a real root clip of dqshared_droid.swf, while MainMenu is in dqmenus_droid.swf. The current facade activates only the latter. Next integration needs real shared-renderer activation, MultiMenuManager push/pop lifecycle, private settings/language/audio ownership and persistence. Merely showing the shared clip would not prove a functional Options menu. Navigation, loading and gameplay menus remain unfinished; full media evidence is still tied to v28.

The v30 adapter additionally enforces the original destination OBJECT tag, so a PROPERTY argument does not invoke its getter. `options-tests-v30/option-validation.json` passes that case with a real property on the actual AS argument stack. A bound property is evaluated by the core's copy constructor; the test therefore binds it after pushing, rather than mistakenly attributing pre-call copying to native dispatch. `launch-build-v30.log` and `input-tests-v30/input-validation.json` confirm the exact current APK still passes the three-size touch/focus/sound, live-resize and held-touch Home/resume checks. The folded and restored main screenshots were inspected. The visible emulator stays on main, with no resolution override. This is still the same incomplete main-menu flow; Options is not opened by the new reader.

## Shared renderer and resize ordering v32-v34

The displayed main menu now retains a second, independent `dqshared_droid.swf` player with its own construction history, frame owner and viewport. Original RenderFX.Load allocates a new player for every renderer (0x7ab7c0..0x7ab7e0); importing shared definitions into the main player cannot replace that renderer. Actual player identities are logged and checked as nonzero and distinct. The shared root contains nine hidden menu states; Options is clip 121 with 23 frames. Its state remains inactive while native stack ownership is pending.

The render path updates both viewports and the main input rectangle before the frame processes input. The previous order processed input once with the old rectangle on a surface change. The independent shared timeline advances before main, using the same retained integer milliseconds: original MenuManager.Update iterates all loaded slots 0..3 (0x42ecc0..0x42ed28). This is the shared root timeline stage, not a claim that its native state stack/input path is connected.

`input-tests-v32/input-validation.json` and `input-tests-v33/input-validation.json` pass three-size real input, shared-player identity/construction, inactive Options visibility, held focus transfer, live resize and Home/resume checks. v33 is the build with viewport-before-input ordering and shared timeline advancement. Tests restore the visible emulator to the main menu with its prior display size. Screenshots are retained alongside the logs.

`swf_menu_navigation.{hpp,cpp}` adds the original NativePushMenu and NativePopMenu argument/dispatch adapters (0x43b1b4 and 0x43b158). They use the real AS `to_xstring` conversion, ignore extra arguments, preserve the result, and distinguish named pop from zero-argument virtual top-pop. Reached missing/rejected stack owners fail explicitly. They require synchronous real menu-manager services and are linked but deliberately not registered until that owner exists. No deferred request queue stands in for source delivery.

`options-tests-v34/option-validation.json` verifies the option reader plus navigation adapters using real AS arguments and explicit stack fixtures. It covers string/numeric/object conversions, extra arguments, zero-argument pop without an environment, result preservation and synchronous rejection. It does not prove actual navigation or a functional Options screen. Source disassembly for player allocation, loaded input flags, renderer update order, push/pop and callback environments is preserved in `menu-stack-source-v33/`.

Native menu creation/registration, full push/pop lifecycle, settings persistence/language/audio ownership, the 3D main scene and loading artwork remain incomplete. The full goal is still active.

The exact v34 APK passed `media-tests-v34/media-validation.json`: original video motion/fit at 1920x1080, 2400x1080 and 2184x1968, Back-to-main, title intro/loop, Home/resume (2563 to 2617 ms), and natural completion to main. The final screenshot was inspected and the dedicated visible emulator was restored to main with no display override. v34 only adds the linked, unregistered navigation adapters beyond v33; the full input evidence remains explicitly tied to v33, and the full media evidence is now tied to v34. This does not establish menu navigation or listening quality.

## Synchronous menu renderer dispatch v35-v36

`SwfMovie::menu_action_script` is an explicit synchronous path for native menu-manager calls to another retained renderer, or back to the active renderer. Ordinary facade calls still reject recursion with `SWF core busy`. Scope entry serializes global core provider access, installs the selected renderer/glyph providers, and restores the caller's renderer/glyph/active dispatch on return or exception. Same-renderer nesting does not clear an existing required-provider failure; only a new top-level scope clears it. File/log dispatch use the same active-owner trampolines, and bitmap dispatch is restored for an outer active renderer. The method retains the exact graph and AS connection through callback return.

`renderer-scope-tests-v35-native/scope-validation.json` passes on emulator-5580 against two actual independent AVM1 players. Tiny generated SWFs and glyph/draw services are explicit fixtures, not application replacements. Checks cover A->B->A, a genuine typed native callback synchronously entering its peer renderer, root/native player/provider identities, renderer/glyph restoration after exceptions, ordinary recursion rejection, preservation of a same-renderer failure, and recovery on the next top-level call. This removes a necessary integration obstacle; the production native stack is still unconnected and Options/Info do not open yet.

The first v35 real-input regression failed because its first-tap log contained four Options press/release pairs and four confirmations, while the test expected one. Its original failure log/report remain in `input-tests-v35/`; no cause is claimed from those logs alone. v36 adds a debug-only, opt-in `trace_menu_input` intent to record Android motion action/device/source/time before queuing native touch delivery, and the test enables that trace. The exact one-confirmation assertion is retained, with a clearer failure message. The trace is disabled for the final restored main view. No production touch behavior was changed to hide the failure.

`input-tests-v36/input-validation.json` passes the unchanged three-size touch/focus/sound checks, live resize and held-touch Home/resume for the exact v36 APK. Android input traces are retained in each scenario log. This clean run does not prove the cause of the prior v35 failure. The v36 engine-library SHA256 exactly matches the scope audit's v35-native tested library (`ad8888eee95c3801a08660dba9283da1602815c8d2992b9b892121928ea21b3f`). Media evidence remains explicitly tied to v34. The visible emulator is restored to main with no size override.

## Visible main-menu scene v38-v39

The original `models/main_menu_charactere_swamp.bdae` now renders behind the authored main-menu SWF. Camera values come from original `MenuMainMenu::CreateAvatarCamera` at 0x42bf3c: eye (0,-900,150), target (0,0,225), up (0,0,1), aspect bits 0x3fd578e9, FOV bits 0x3f3579c8, near 10, far 2000. The concrete CCameraSceneNode vptr is vtable+28 (constructor 0x5837dc); slots 0x130/134/138/13c resolve to setNearValue/setFarValue/setAspectRatio/setFOV. The port converts this view to its GLES matrix convention and fits the scene into the same centered 480x320 menu area.

This is a connected visible scene, not full original scene-manager/rendering parity. It uses the existing model material shader and animation loader; original lighting, scene callback/layer ordering, dynamic viewport camera behavior, save data and the separate player avatar remain unfinished. No replacement background artwork was generated. The source model and textures were already present in the snapshot.

v38's first 2400x1080 screenshot is `background-tests-v38/main.png`. Its first input test was interrupted when the emulator process disappeared. The relaunched input test failed because it tapped after input binding but before the heavy model upload completed; its saved before screenshot is blank. The test now waits for an actually submitted main-menu frame before tapping. v39 also reloads the scene after context recreation while retaining the SWF graph, and clears menu-camera selection when entering a world. See the exact version's input receipt for verification status; previous input/audio receipts do not verify this new scene.

The bottom-left black center belongs to the authored btn_exit subtree (sprite509, bitmap-filled shape507), not the player portrait. Its final texture/timeline behavior still needs tracing. Save-slot text/controls remain disconnected from their required persistence services. The original swamp is now visible, but these missing parts are not claimed fixed.

Verified v39: input-tests-v39/input-validation.json PASS for the exact APK SHA256 0be99519546c883e3ff02ae76ed3d1a84ca17a804b3f007353604827298ff980. Three-size input checks, live resize, held-touch cancellation and post-resume tap pass. The final restored-main screenshot shows the original swamp. Navigation/save/avatar/Exit artwork/lighting/loading artwork remain incomplete.


## Native loading background v40-v42

v42 draws the original startup artwork separately from the authored loading overlay. `GSInit::Draw` 0x384a4c supplies source rectangle (0,0,1280,752), and `GSInit::SwitchBackground` 0x384f64 selects data/3d/textures/splash_final.tga for the default language. `original-startup-draw-v42.asm` and `loading-resources-v42.json` preserve those source inputs. The modern adapter aspect-fits the source rectangle without stretching. This connects the missing artwork; it does not reconstruct the complete startup state machine or campaign loading stages.

The source ZIP base and Droid splash textures are byte-identical (SHA256 2a296425a7072f22b6ac0ec9344b441599ebc7b5497dc22e671c791f455df2a8, 8388652 bytes). The runtime catalog now retains the base logical name using the already bundled identical bytes. `audit_loading_resources.py` reproduces source/cached-resource hashes, the native background rectangle, source default filename, loading fill tables, and Exit shape507's fill table.

The v41 base loading movie experiment did not fix the spinner. Its initial bitmap fill table contains five styles; examining only its first matrix gave a misleading apparent match to ring artwork. The actually drawn bitmap style still samples an artwork fragment. Its passing test established background pixels and timeline motion only, not spinner correctness. The final v42 retains the Droid overlay while adding the separate native startup background. The spinner remains visibly incorrect; no guessed coordinate relocation was made. Exit artwork also remains incomplete.

`loading-tests-v42/loading-validation.json` passes for the exact APK: original startup layer/logical rectangle, three aspect-fit sizes, four original pixel samples per size (maximum error 2 bytes), and visible authored timeline motion. It explicitly records spinner_artwork_correct=false and full_menu_functionality=false. Screenshots were inspected. The launch/menu save/navigation/avatar work remains active.


### v43: initial authored Main/Info/Back navigation

Installed only on visible DH2_Launch/emulator-5580 using private ADB server 5038.
Main Info button now synchronously opens menu_info; its authored top-left Back
button returns to Main. Uses the typed original NativePushMenu/NativePopMenu
argument wrappers, retained same-player stack, authored onHide/onShow/onPush,
visible-state changes, generic MenuBase input for Info, and the source SetContext
pointer replacement (0x7a7ee8). Live cursors/focus are preserved by SetContext.

This is an initial owned navigation slice, not original MultiMenuManager parity.
Only Main and Info are registered in this slice. Other destinations emit
`Menu navigation not connected` and remain incomplete. Cross-renderer
navigation/settings, source touch reset/process ordering, transition animations,
native state flags/GotFocus/Show side effects and arbitrary named-pop semantics
are still pending. Info Help/About/Twitter controls are visible, not functional.

Exact APK SHA256: 3d8b936fe455486ab8c74f5ee926ee050b92d30cbce2a9a2dad91843be33c975
Validation: navigation-tests-v43-retry/navigation-validation.json, repeated
Main/Info/Back cycles at three surfaces and Info live-resize/back in one process.
First navigation-tests-v43 failure was an incorrect test Back location at the
bottom left; screenshot showed the actual authored Back at the top left.
Test coordinates were corrected; no application hit-zone workaround was added.

Current visible main has the swamp background. Save panel/avatar, Exit atlas
artwork, loading spinner artwork and complete settings/game paths remain open.
No main-session checkout or emulator was changed.


### v44: shared Help and About navigation; HTML clipping and text metrics

The isolated visible emulator now uses retained independent main/shared SWF
players for Main -> Info -> Help topics and About -> Back. Each exact root
advances once; only the selected renderer processes input. Navigation callbacks
keep their original caller player/renderer scope. Seven original Help topics
and Other Stats pages a/b/c/b/a are connected.

Original Help HTML extends beyond its authored single-line text rectangle.
The overlay follows the ordinary original non-border display branch without
stock's extra rectangle mask, keeping ancestor masks. This exposes all five
Controls rows and longer Help pages. The original textHeight/textWidth getters
now return retained formatter bounds in pixels, enabling the authored credits
scroll and wrap condition. The frozen formatter kernel/vendor remain unchanged.
Original source captures: text-display-source-v44.asm and
original-text-get-member-v44.asm.

Credits ^t resolves the real MENU_GAME_TITLE constant and string ID. ^v uses
the original 1.0.2 ordinary Android operator branch. English is the explicit
front inspection language; saved settings/language selection and Japanese title
branch are not connected. Other varargs directives still fail explicitly.
Credits source capture: credits-application-source-v44.asm.

Shared navigation validation: shared-tests-v44-motion/shared-validation.json.
Seven topics, pagination, About text-region motion, Back navigation at three
surface shapes; Help live resize, pause/resume and return in the same process.
Earlier failures are preserved: retry (credits directives unavailable), credits
(textHeight absent and stationary text). Motion was not considered passing
until real screenshot comparison succeeded.

Still incomplete: Options settings/persistence/audio volume, New Game/save
selection/character panel/avatar, online paths, full native stack transitions,
original background lighting/material fidelity, Exit atlas and loading spinner
artwork. Existing cinematic/audio tests refer to earlier exact APKs.
No complete-menu claim. Main checkout and its emulator were untouched.

Exact APK SHA256: a38cae2123ef483ce0ffae8b7d739976938d3ff3ac7d143b34af5d7e8ba885a9


### v45: authored Options, saved settings and audio volumes

Options now uses the original GameOptions table, original SWF sliders/selectors,
localization IDs, and ordered 16-option plus 14-tutorial-byte save format.
The private settings file is written through a checked temporary file and rename.
Music and effect controls deliver actual MediaPlayer volume changes; new audio
players also receive the saved volume. Language reload follows the authored
SaveSettings/onPush/LoadSettings path with a retained front scene registry.
The front scene has no gameplay characters/items; attaching gameplay still
requires its genuine actor and HUD traversal. Selector saving is verified;
gameplay effects of Controls, HUD Style and Auto Transmute are not certified.
Full source Savegame file-manager wrappers, Sharp flags, ResetFonts, native
audio mix/fade, and full MenuManager focus/transition behavior remain pending.

Exact APK settings validation: settings-tests-v45-utf8/settings-validation.json.
Three surfaces: 1920x1080, 2400x1080, 2184x1968. Real drags, audio delivery,
selector changes, file-byte checks, app restart persistence, Back navigation,
same-process resize and pause/resume passed. Prior private settings were restored.
The cache file menu.french actually contains Ukrainian text, identical to the
supplied ZIP. The initially mislabeled French receipt is preserved separately;
the corrected receipt includes a fresh native Ukrainian title delivery check.
Shared Help/About regression also passed on this exact APK at all three sizes,
including seven topics, pagination, text rows, scrolling credits and lifecycle.

Still incomplete: New Game/save/character panel, background material and lighting
fidelity, Exit/loading artwork, game loading flow, online actions, Asian font
switching and gameplay selector effects. Earlier cinematic/audio playback
receipts refer to their earlier exact APKs. No complete-menu claim.
All changes, testing and emulator operations stayed in the isolated snapshot.

Exact APK SHA256: f844b4c56612aa807be6cd9b10b5d6eec4d442874e086b04da25e2b40850d635


### v46: real absent-save branch and authored name initialization

NativeGetSaveSlotDetails now connects the original all-files-absent branch.
It checks all four dh2_NNN.savegame files and their .bak files in the private
files directory. Existing campaigns explicitly require the complete
PlayerSavegame loader; they are never represented as empty. No campaign
save is created, removed, or replaced by this change. An absent save now
takes the original Start Game -> menu_EnterName branch.

The name transition follows the relevant original MultiMenuManager Push
ordering: onPush, RenderFX PlayAnim("show"), then onShow. The authored frame
15 clears the name field. Its absence had left authoring HTML in the field
and the eight-character check incorrectly rejected every key. The actual
text getter was correct and was not changed. Original keyboard handlers
now produce text, enforce eight characters, and Back returns to Main.

Name verification on this exact APK: name-tests-v46/name-validation.json.
Three surfaces, real taps, entered "adam", rejected a ninth character,
Back, empty field after reopening, and same-process resize preserving
the current name all passed. Screenshots and actual native field inspection
are retained. The earlier failing probes remain in their own output files.
v45 Options/Help/About receipts remain exact-v45 evidence; they are not
represented as fresh v46 certifications. This change is confined to the
new name transition and absent-save callback.

Still incomplete: class selection and full New Game/game-loading flow,
existing save loading, main background material and lighting fidelity,
keyboard/Exit/loading artwork, online actions and prior v45 limitations.
The supplied splash atlas decodes, but authored keyboard shape 70 samples
pixels x49..89/y891..932 containing spinner fragments; shape 507 samples
black artwork. Atlas/native previews and fill tables are preserved in
keyboard-fill-source-v46.json and *-v46.png. No speculative texture-coordinate
or asset substitution was made. This is a work-in-progress checkpoint.
The visible emulator and all work remain isolated from the main checkout.

Exact APK SHA256: 1d0e1273230f1c99d5c67dbe88420f9e78e296a67434e831dd5ac4cf659d2d96


### v47: class selection controls and descriptions (preview incomplete)

The authored Name -> Choose a Class flow now reaches menu_SelectClass.
Original MenuCharacterSelect Update/OnEvent branches supply Warrior
(KnightPlayerBase), Rogue and Mage order, arrow boundaries, source string
IDs, HTML descriptions and the actual CurrentClass ActionScript callback.
The retained original HTML parser renders title/description styling; no
replacement class artwork or rewritten authored ActionScript was added.
The selector compares the actual cached button character identities on
original event kind 2. Show retains the current singleton class index.

Exact-APK receipt: class-tests-v47-controls/class-validation.json. Class
arrows and boundaries, class-to-name Back/reopening, actual root PlayerClass
inspection, three surfaces and retained-process class resize passed.
Source, patch, APK and screenshots are preserved for review. A separate
earlier test, class-tests-v47-final, failed to reopen Name from Main after
two back cycles and the third resize. A fresh third-size launch works
(class-tests-v47-third); that main re-entry/resizing path is unresolved.
Do not treat the class-only passing receipt as complete menu certification.

Still incomplete: class selection scene/actors/animations/lighting, swipe,
Confirm -> game creation/loading, full existing-save load, main materials,
keyboard/Exit/loading icon artwork and previously documented limitations.
The empty central class preview is an acknowledged missing native owner.
Earlier Options/Help/Name receipts certify their recorded APKs only.
This remains an isolated work-in-progress contribution; main checkout and
main emulator were not modified and the full user goal remains active.

Exact APK SHA256: 8c785108a39c9ef4526cfc3c1c7efa51be85e1e4b6b52deaa9ab29a2dddbd23c


### v48: original foliage alpha channel and new full navigation checks

Scene materials now retain their actual external effect filename/URI and
GLES2 CurrentTechnique. The exact GL_Diffuse_L1_VC_iPhone.bdae technique
L1_Vc_Al_----_----_----_---- replaces opacity with AlphaSampler's BLUE channel,
as authored GL_Diffuse_L1_iPhone_FS.glsl specifies; the old generic renderer
multiplied RED instead. This is a bounded material correctness fix. The
generic preview shader remains in use: full original lighting, effect/pass
render states, normals, profile COMMON and other shader variants are pending.

Exact-APK render receipt: material-tests-v48/material-validation.json.
Baseline v47 hash was read from installed base.apk. Before/after screenshots
show a small difference, mean RGB difference around 0.04 byte across the frame.
Three surface sizes and same-process Home/resume passed; this is NOT proof
of faithful overall background appearance. Native material binding logs and
the decoded source textures/channel statistics are retained.

Exact-APK navigation receipt: main-reentry-tests-v48/class-validation.json.
Main -> Name -> Class -> Name -> Main loops, all class arrows/boundaries,
three surface sizes and retained-process resize passed with Android touch
logging enabled. These checks wait 2.5 seconds after each WM resize. The
older v47 failure with a shorter wait remains preserved and is not erased
or claimed resolved by source changes. Simulator resize readiness remains
a timing concern; arbitrary resolution and transition timing are not fully
certified. The emulator still uses a visible, separate instance.

Class-scene investigation confirmed original CLASS_SELECTION.bdae has a
camera rig, seven lol_* animation segments and three dummy actor anchors.
It and the original effect bytes are preserved in handoff analysis files.
They have NOT yet been integrated into the application. Class previews,
source camera/light owner, complete game creation/loading, icon artwork,
existing-save loading and prior documented menu limitations remain pending.
This checkpoint is work in progress; the full user goal remains active.

Exact APK SHA256: 4650ca584c2bfd87ea2df2351162ccfcf1d2c720cbbbedc0f590b40fb72a37c3


### v49: authored animated class-selection environment and real display hook

The actual menu_SelectClass.class_select character now owns a GameSWF display
callback, invoked after its children. The retained movie pins the callback
character, removes callbacks before teardown, and propagates rendering errors.
The callback maps its real world bounds through the current stage projection;
the GLES adapter draws into that pane and restores the SWF viewport/depth/cull
state. Its coverage and stencil-query targets now share packed depth/stencil
storage, required for GLES framebuffer completeness with a 3D pane.

The exact original CLASS_SELECTION.bdae is now bundled. All 27 authored
position/rotation/scale tracks bind without unsupported channels. The seven
lol_* clips drive the four adjacent camera transitions and three class idle
views. Camera target direction follows Collada CCameraSceneNode at 0x6e5300;
FOV conversion follows its constructor; near/far and Z-up follow Show.
The scene animator replaces Show's initial camera position, so the rendered
camera retains its sampled position. Class arrow input is locked during the
transition, as native field fc/IsAnimOver specifies, including queued taps.
Back restores the main-menu scene; context rebuilds retain class selection.

Verified exact-APK receipts:
* class-scene-tests-v49-gated/class-validation.json: real callback draw, all
  seven clip names, 27 bound tracks, rapid arrow tap lock, three surface sizes,
  class text/identity/boundaries, Back, retained selection on resize and Home,
  same-process resume, and main-scene restoration.
* shared-mask-tests-v49/shared-validation.json: packed framebuffer regression
  for seven Help topics, masked Controls text rows, Skills pagination, moving
  credits, Home/resume and Back at 1080x2400. This is one surface only.

Screenshots were visually inspected. This delivers the missing environment;
it does not finish the class preview. Three Characters, LoadPropertiesForClassSelect,
class equipment, actor idle/selection animation and source lighting/material
passes remain incomplete. The generic unlit mesh renderer remains in use.
Icon/keyboard atlas problems, swipe, Confirm/game creation/loading, existing
save handling, arbitrary-resolution certification and previously documented
menu limitations remain pending. Earlier cinematic/audio/settings receipts
remain evidence for those earlier APKs, not a new blanket certification.
The full user goal remains active. No main checkout/emulator was changed.

Use tools/front_class_scene_smoke.py for this checkpoint: the older class
controls test uses shorter waits incompatible with source camera input locks.
The handoff connect_class_scene_v49.py is a one-time staging record, not a
current repair/rebuild command; later camera/framebuffer fixes are in source.

Exact original ZIP scene member: com.gameloft.android.GAND.GloftD2SS/files/data/3d/optimizedmaxfiles/class_selection.bdae
Exact APK SHA256: ddcaeaec3b2bed281b0fb08490316a22684be7b9ef7f9faea3eeb0903b433d04


### v50: original class preview inputs and corrected loot-cache reader

Corrected Arrays::ItemTypeList: nine variable-length byte lists, not a flat short array. Original ARM reader section offsets are recorded in loot-reader-offsets-v50.json. ClassPreviewDefinition loads actual base/class properties, unpowered singleton starting-item definitions and actual MenuIdle/MenuOnSelect/Template clip paths for the three source class names. It preserves the Rogue dagger duplicate. These are authored inputs; inventory delivery, equip decisions, modular visual binding, actors and native animation state machines remain pending.

Actual-cache native test class-definition-test-v50.log passed on emulator-5580: 3 classes, 16 entries, duplicate daggers, animation paths, failed-load output preservation, truncated ItemTypeList and owned-cache lifetime. Actual Android class-menu loading now invokes the same resolver and logs all three definitions. Exact original loot cache files are bundled and their APK bytes verified against loot-assets-v50.json.

class-scene-tests-v50-packaged/class-validation.json passed against this exact APK: three tested surfaces, all seven environment clips, arrow bounds/input lock, Back, resize selection, Home/resume and main-scene restoration. No new actor, lighting, icon, cinematic/audio or gameplay completion is claimed. The first attempted APK lacked loot assets; that failure is preserved in class-scene-tests-v50. The corrected installed build is this packaged checkpoint. Full requested goal remains active; main checkout and emulator remain untouched.
APK SHA256: cd2077d30f892eae35e2c4dc3ac0362ba25e8f289b371116e9e710aaf67dd921


### v51: three class body previews and original idle clips

The class display callback now draws three independently owned modular body resources. The original starting-item visual names select their exact -mesh-skin controllers. The empty helmet uses MC_Head__naked, following INV_UpdateSkin category + __naked fallback. These are scene/body resources, not a completed Character/ItemInventory factory. Each body uses its class MenuIdle clip from the original animation table. All skeletal targets bind: Warrior 29 tracks, Rogue 26, Mage 31, zero skipped or unbound. Placement follows each authored dummy anchor and original base-property visual scale. Body poses and GPU buffers are updated during the real class display callback. Scene changes release resources; GL context rebuild discards stale handles and recreates resources.

modular-resource-v51.log: native emulator test passed for the exact 4 controllers per class, 1492 skin vertices, finite animated positions, changed idle poses and missing/duplicate module rejection. class-scene-tests-v51-textures/class-validation.json: exact APK passed existing class navigation, camera clips, three surfaces, resize selection, Back and Home/resume. This navigation test still labels full actors incomplete, correctly: equipment inventory, weapons, selection state machine, and full light/shader passes remain pending. Screenshots show actual distinct bodies. The first v51 app attempt lacked Rogue/Mage atlases; its failure is preserved in class-scene-tests-v51. The corrected exact original atlases and all seven original menu/template clip files are bundled and hash checked against class-animation-assets-v51.json and class-texture-assets-v51.json.

This is visible progress, not menu completion. Weapon attachments, original actor state transitions, full lighting, broken icons, swipe, Confirm/game creation/loading, existing saves and other documented remaining menu/audio/lifecycle requirements remain open. Main checkout and emulator untouched; full goal remains active.
APK SHA256: 58f217aa482ea339d781229e87aca5ad11b87d4c1dced84badb81011f87f151d


### v52 keyboard compatibility correction

The supplied Android keyboard samples loading-spinner fragments from its splash atlas. Generic corresponding key shapes provide matching original letter/digit, pressed-glow and Delete artwork. Shift and Space use identified original atlas graphics. Their mappings are compatibility choices; the original Android atlas mapping remains unresolved. No replacement artwork was generated. Original cache bytes stay intact and are validated before the separate compatibility SWF is loaded.

repair_keyboard_atlas.py generates front-compat/dqmenus_droid.swf and its provenance manifest. Seven keyboard shapes change, including bitmap matrices introduced by internal NewStyles records. verify_keyboard_atlas.py confirms every other tag, action, placement, shape edge and bound is byte preserved. This fixes visible keyboard fragments without claiming full original visual fidelity. Space currently stretches the original blank capsule; preservation of original border proportions remains a fidelity limitation.

keyboard-tests-v52-settled covers the exact APK on three surfaces: 1080x1920, 1080x2400, 1968x2184. Real taps verify lower/upper case, Space, Delete, digits, eight-character limit, Back and retained-process resize. Held-pointer screenshots show the actual pressed-letter glow. The final visible keyboard uses physical 2400x1080, no wm override. Earlier keyboard-tests-v52-final failed after resizing 0.4 seconds after Back; its blank main screen and logs remain preserved. The successful run waits 1.5 seconds after Back and verifies the actual pop. Resize during that transition remains an unresolved runtime case. No campaign save writes. The main checkout/session/emulator were untouched. Full menu goal remains active: loading/other icons, full actor equipment/lighting and Confirm/game creation remain open.

APK SHA256: e96412b1da97ce4495a8c6dd4e40c53285ba36abca50a8e171d5979e1dc629e0


### v53: retain the native surface across size changes

The rapid Name-menu Back/resize path previously produced a blank Android presentation. resize-back-v53 captures this: native SWF visibility, frame progression and viewport were valid, and GLES readback contained the menu pixels, while Android screenshots were blank. Returning to the physical size could restore presentation. This is evidence of a surface-presentation/lifecycle problem; the specific Android compositor fault is not established. SurfaceSyncGroup timeouts were logged.

MainActivity now handles orientation, screenSize, screenLayout and smallestScreenSize configuration changes. The existing GLSurfaceView and native retained graph resize through onSurfaceChanged. No Android density or locale handling was overridden. onConfigurationChanged records the retained activity identity. A read-only inspect-front probe exposes graph visibility/positions/frame numbers and GPU pixels/binding/viewport for diagnosis.

resize-back-v53-fixed repeats the real failing path: actual name entry, Back, size change after 0.1 seconds, 2184x1968 presentation, then physical 2400x1080 restoration. Visible center colors >3200 at every captured stage, one GL initialization and retained-configuration callbacks. Earlier blank captures and GPU evidence remain preserved. front_resize_transition_smoke.py is the reproducible test in the source handoff.

class-scene-tests-v53-retained verifies the exact APK at three surfaces, all seven original camera clips, 27 animation tracks, class boundaries, transition input locking, Back, retained Rogue selection on resize, and Home/resume in the same process. This does not certify arbitrary resolutions or complete characters/lighting/game creation. Keyboard actions were tested on v52; its assets are unchanged. Current v53 screenshots also show the repaired keyboard on Back. The full menu objective remains active. Main checkout, main session and emulator5554 were untouched.

APK SHA256: 101f4bb03da60a4a1848d62b9329f17fb3622559009dce376eabcbe057251ae4


### v54: connect the Confirm-flow stack callback

NativePopAllAbove is now registered on the retained front/shared renderers and synchronously delivered to their genuine stack owner. The source wrapper 0x43ac28 accepts exactly one STRING/OBJECT, converts with the actual GamesWF to_xstring, and leaves the AS result untouched. Wrong arity/type is a source no-op. The original vtable slot0x3c resolves to MultiMenuManager::PopMenu(name,true) at0x439270. Its loop tests target stack membership and exits when that target becomes current. Original bounded disassembly and literal/vtable receipts are retained in confirm-source-v54.asm and menu-owner-literals-v54.json. This implementation uses the already-connected per-pop retained menu lifecycle.

front_pop_above_smoke.py verifies the real installed callback through the graph, with actual Name/Class entry, one/two-level unwinding, malformed argument rejection, missing/already-current target no-ops, real Back after unwinding and a rapid resize after returning to Main. Screenshots were inspected at physical2400x1080 and2184x1968. No campaign saves were written. This is a prerequisite for the original Confirm flow, not a completed game launch: NativeCreateSaveSlot, NativeAssignSaveSlotToPlayer and menu_StartGame still require their actual native owners. The source frame29 callback order is documented in CONFIRM-GAME-LAUNCH-TRACE.md. Main checkout, chat and emulator5554 remain untouched; v54 is installed in visible DH2_Launch:5580.

The full objective remains active. Equipment, lighting, remaining icons and keyboard Space proportions also remain incomplete. Previous v53 keyboard/surface checks are retained and apply to that exact APK, not a blanket v54 certification.

APK SHA256: 25c513a82c7cb7813d16625532f8f3a111e7b461d3550caff7c483d0fe2dfd34


### v55: native fresh-player profile encoder

fresh_player_profile_v1 produces the seven initial metadata sections registered by original SG_Load(1): PNAM, PLVL, PCLS, PDFL, LNAM, LEPT and LUSP. These represent name, level1, the genuine CharacterTable base identity, difficulty0, level41/three seeds and source flags, zero entry points and three enabled spawn-point flags. The source seed arithmetic is unsigned real-time plus21371 and86186. Character IDs are resolved from the actual CharacterTable: Knight263, Rogue325, Mage290. The older standalone metadata reader probe's smaller ClassTable fixture is not the appropriate playable character dictionary; the new creation probe loads the original CharacterTable names from the ZIP and asserts these identities.

Original creation instructions0x43f718..0x43f7e0, the parameterized constructor, SG_GenerateSeeds and all seven metadata serializers executed under Unicorn. Class/name input conversion and free-slot inventory are caller inputs; SG_Load is recorded but not executed, SG_SetSaveDate and actual SG_Save/filesystem delivery are external services. Loaded difficulty count is3. These omissions are explicit and do not establish live Confirm, filename/backup handling, full profile loading or filesystem fidelity. The seven payloads compare byte-for-byte with native C++ execution on emulator5580: nine cases,63 sections, three classes, ASCII/UTF-8 names and wraparound timer. Native section-index/name/level/class readers also round-trip the result. Null, non-playable and absent CharacterTable failures preserve output.

The outer count/size/four-byte-tag/payload and ascending tag order follow the recovered Savegame::saveAll source and the native profile-index reader. The entire original saveAll/file-job chain has not yet executed in this probe; original byte comparisons cover section payloads. The encoder is compiled into dh2_game_data and the Android build passes, but no production callback invokes it yet. No campaign files were written. Installed visible app remains the verified v54 build. v55 APK below is a build-only artifact, not a live game-launch claim.

Runtime follow-up: implement genuine free-slot/file delivery, occupied NativeGetSaveSlotDetails, AssignSaveSlotToPlayer storage and menu_StartGame/level loading. Full objective remains active, including menus, loading, assets, actor equipment/lighting, cinematics/audio and dynamic-resolution verification. Main checkout and emulator5554 remain untouched.


v57 keyboard texture and Space capsule correction

The separate front-compat movie uses original cached artwork for letter/digit keys, their pressed states, Shift and Delete. The Space normal/glow states now preserve curved end proportions and stretch only their middle. Three bitmap rectangles retain the original Space bounds and bitmap pixels. NewStyles separates their tessellation layers. Explicitly clearing FillStyle0 is required after each layer for the existing GamesWF point test; leaving its sentinel would make the middle/right slices unclickable. No engine/vendor changes were needed.

The structural verifier confirms all 618 tags remain, with every action, placement and non-keyboard tag byte preserved. Five shapes change only matrices; Space shapes80/82 intentionally change their geometry to three adjacent rectangles within original bounds. Original cache assets remain intact. These mappings are compatibility recovery from original artwork; exact original Android atlas coordinates remain unresolved.

The exact installed APK passed real typing, Shift, Delete, digits, eight-character limit and Back on three surfaces:1080x1920,1080x2400,1968x2184. Space was tapped at left, middle and right on each. Screenshots cover normal/pressed Space and held letter artwork. Text survives retained-process resize. The final visible emulator is DH2_Launch:5580 at physical2400x1080 with no wm override, left on Enter Name. Failed development screenshots/receipts remain in keyboard-tests-v57 and keyboard-tests-v57-final; the authoritative final run is keyboard-tests-v57-verified.

The full menu/game-launch/loading/media/resolution objective remains active. Confirm/game creation, occupied save slots, full equipment/lighting and other menu artwork remain unfinished. Keyboard verification on three sizes does not establish all-resolution correctness for the whole application. Main checkout, chat and emulator5554 were untouched.

APK SHA256: b414f21ca5ea266b7fb83bfe3785a72cf80a8b2cf80cd64dc8f0f8868d0a16e8


v60 fresh-profile save date and location reader

The first LNAM word is the save timestamp stored by SG_SetSaveDate at PlayerSavegame+0x38, not a constant zero. v55/v58 fixtures used an explicitly stubbed date service and only certified that fixture. The production encoder now accepts separate real-time seed input and time() seconds, stores saved_date and serializes it. Original SG_SetSaveDate instructions now execute with time() fixture seconds; original creation, seven serializers and complete Savegame::saveAll produce nine exact native comparisons across three classes, ASCII/UTF-8 names, timer wraparound and date word boundaries. The file writer traverses genuine borrowed section nodes and performs all seek/backpatch/header operations; filesystem, async job processing, backup delivery and SG_Load remain borrowed/unverified services.

LNAM also owns three level IDs, three seeds and three current acts. The third value in each triple is the current act, not a generic spawn flag. Original __LoadLevelName copies each act into both regular and volatile quest owners (+0xfc/+0x15c arrays). The new PlayerSavegameV1::load_location follows those stores. Blank-constructor date/levels/seeds are zero and acts are one, proven by executing the original constructor. Original/native reader comparisons cover128 arbitrary and signed-boundary spans, constructor defaults and trailing input; bounded native checks cover six truncation prefixes. Nine fresh-file results round-trip through the new native location reader as well as name/level/class/index readers.

Android build passes. These APIs are compiled but CreateSaveSlot, occupied NativeGetSaveSlotDetails, AssignSaveSlotToPlayer and menu_StartGame are still unconnected. No campaign files were written. Installed visible app remains the verified v57 keyboard/menu APK; v60 APK is a build-only checkpoint. Filesystem ownership and occupied-slot AS projection are the next implementation steps. Main checkout/chat/emulator5554 are untouched. The full menu/loading/game-launch/media/dynamic-resolution objective remains active.


v61 source menu-load dispatch and entry/spawn readers

Executing the original PlayerSavegame(slot,17,false) constructor and SG_Load shows the menu dispatches PNAM, PLVL, PCLS, PDFL, LNAM, LEPT, LUSP and QEST in that order, initializing both quest owners before QEST. Nine original dated fresh profiles load through the real dispatch and seven real readers. Cache lookup/Savegame::load dispatch delivery, offline singleton and quest initialization are explicit borrowed services. This is not full filesystem or occupied-ActionScript proof. A present QEST section must be loaded: QuestSavegame::UnpackQuests stores current acts and can overwrite LNAM acts; ignoring it would misreport progressed characters.

PlayerSavegameV1 now owns three entry points and three raw spawn bytes. Native LEPT/LUSP readers reproduce original three-int32/three-byte reads and retain reached stores on bounded truncation. Original/native comparisons on isolated emulator5580 pass128 cases including signed limits, noncanonical bool bytes and trailing input;15 native truncation prefixes also pass. The date/location128-case suite and nine full fresh-profile buffers/63 sections were rebuilt and pass with the enlarged owner. Fresh files also round-trip all three starting entry points0 and spawn flags1.

Android build passes. v61 APK is build-only; visible emulator remains verified v57. CreateSaveSlot, occupied-slot AS projection, quest ownership, campaign files/backup, assignment and StartGame are still unconnected. No campaign files or main-session files/emulator were changed. Full menu/loading/game-launch/media/dynamic-resolution goal remains active. Next work must implement QEST ownership and occupied projection before enabling campaign writes in Confirm.


v63 Exit power icon

Main menu btn_exit sprite509 contains icon sprite508/shape507. The supplied Android shape samples bitmap1 at x48.72..77.22,y697.17..725.67; its embedded bitmap tag has only a seven-byte format0 placeholder. The supplied generic menu owns the complete bitmap atlas and corresponding btn_exit sprite496/icon shape494 uses the actual power glyph at x656.76..694.16,y774.62..812.17. The separate compatibility movie now maps shape507 to that original glyph while preserving Android shape bounds, edges, placements and all actions. No artwork was generated and original cache bytes remain intact. Exact Android atlas mapping remains unresolved; this is recovery from the corresponding original menu asset.

The structural verifier passes all618 tags: five keyboard shapes plus Exit change matrices only; two Space states keep their existing verified capsule slices. Exit normal/pressed artwork, confirmation opening and No cancellation pass on three retained-process surfaces1080x1920,1080x2400,1968x2184 with screenshots visually inspected. Yes/application shutdown is not exercised. Keyboard typing, Shift, Space at left/middle/right, Delete, digits, eight-character limit, Back and retained resize pass again on the exact installed APK. Physical dimensions are restored with no override. Visible emulator5580 remains separate; main session/emulator5554 are untouched.

The build also contains the verified v62 native QuestSavegame dispatch/progress API. Authored quest construction and actual condition/objective data are still required services. Campaign persistence, occupied AS display, Confirm/assignment/StartGame, full equipment/lighting and complete menu/media/resolution verification remain unfinished. Full goal remains active.

APK SHA256: 93040894d9c1356ca91a611a3d2929af75c49fb5c1230b4b6c515c2b9a94f9cd

Latest build-only increment: see SAVE-SLOT-DATE-v64.md for recovered original date presentation and1,028 native checks. Latest visually verified installed menu remains v63. Save ownership coordination is pending; campaign flow is incomplete.

Latest installed increment v65: see SAVE-SLOT-PROPERTIES-v65.md. Shared empty/occupied AS writer passes1,408 original/native typed setter comparisons; occupied campaign loading is still unconnected. Three-surface keyboard/menu regression passes.

Latest installed v66: see SAVE-SLOT-SERVICES-v66.md for source slot ordering and canonical profile callbacks. 64 original selections/192 native AS calls and three-surface live absent-menu regression pass. Occupied campaign loader and game handoff remain pending.

Latest build-only v67: MENU-PROFILE-METADATA-v67.md describes actual metadata-section loading,128 source/native comparisons and required canonical QEST/difficulty callbacks. Original undefined absent-field backing is explicitly qualified. Visible tested menu remains v66; occupied profile and game handoff are incomplete.

Latest build-only v68: MENU-SAVE-SLOT-PROJECTION-v68.md covers localized class/level/location/date presentation, explicit regular/volatile quest selection and432 native integration cases. Live occupied menus and canonical game/save ownership agreement remain pending. Installed visual menu remains v66.

Latest v69: CAMPAIGN-FILES-MENU-v69.md documents source backup reads and actual occupied fresh-profile menu screenshots at3sizes. Main ownership split agreed. Act label missing NativeGetParsedString; create/assignment/start/delete and canonicalQEST still pending. Test campaign files removed; installed emulator5580 now v69.

Latest v70: PARSED-MENU-STRINGS-v70.md; source/native formatter comparison 176 PASS and visible Act 1 on occupied profiles at three sizes. Canonical Online/QEST and assignment/start/create/delete remain pending. Production no longer defaults quest selection to offline.

Latest v71: CAMPAIGN-ERASE-v71.md documents actual authored erase/cancel file effects and menu refresh at three sizes. Confirmation text clipping and assignment/start remain pending.

Latest v72: PLAIN-CONFIRMATION-CLIP-v72.md; complete erase/exit confirmation text at three sizes, installed APK verified. Preview callback is separate from canonical assignment/start.

Latest v74: WEAPON-PREVIEWS-v74.md. Starting sword, two Rogue daggers and Mage staff connected in main/class previews; live installed build checked, snapshots retained. Canonical gameplay still pending.

Latest v75: START-MENU-v75.md. Authored Start screen, Back and explicit pending gameplay-loading confirmation verified; canonical launch and online remain pending.

Latest v76: DEVICE-MENU-v76.md. Original graphics-support producer connected to real Android Build facts/GLES2; authored full Start layout verified. Online/gameplay providers remain incomplete.

Latest v77: MENU-MUSIC-v77.md. Authored title/pause/stop callbacks connected; retained playback/lifecycle/fades/context recreation verified. Broader goal incomplete.

## v78 normal-launch cinematic

See LAUNCH-CINEMATIC-v78.md and launch-cinematic-v78/validation.json. Normal launch now plays the original intro before main. Explicit inspection and loading entries remain available. Full game-loader transition remains pending.

## v79 loading ring and launcher resume

See LOADING-CINEMATIC-v79.md. Corrected original loading ring artwork and retained original timeline, fitted within startup art. Existing-task launcher resume no longer replays intro. Final APK full cinematic and loading verification retained; canonical loading transitions remain pending.

## v80 authored Exit and same-process relaunch

See EXIT-RELAUNCH-v80.md. Exit Yes now releases the front task; No returns to main. Two SWF engine teardown/reinitialization defects fixed. Verified three sizes, same process, preserved campaign/settings. Full game/loading integration remains pending.

## v81 original loading hints and state kernels

See LOADING-HINTS-v81.md. All16tips verified through actual AS/localization callbacks. Original progress/completion kernels compared with145executedARMcases and Androidnative checks. Full Level readiness integration remains pending; no artificial progress/timer added.

## v82 Twitter/browser handoff

See TWITTER-BROWSER-v82.md. Authored release opens original URL through Android ACTION_VIEW once; same-process Info/main return verified at three sizes. Remote redirect page unverified; More Games/GLive and gameplay loading remain pending.

## v83 original More Games Activity

See MORE-GAMES-v83.md. Authored native release now launches original catalog WebView with source language/URL/encryption. Current server returns404; error/Back and live resize verified. Gameloft Live and actual gameplay loading remain pending.

## v84 retained loading-state callbacks

See LOADING-STATE-CALLBACKS-v84.md. Real progress/EndLoading AS wrappers now take retained Level/Online providers;289Androidchecks PASS, main/tips unchanged. Canonical binding and visible gameplay-loading panel remain pending.

## v86 actual shared gameplay loading panel

See LOADING-PANEL-v86.md. Original panel mounted and inspected at3sizes plus resize/resume; source loader-driven entry/refresh exposed. Real gameplay owner/completion remains pending.

## v86 actual shared gameplay loading panel

See LOADING-MULTIPLAYER-v87.md. Original panel mounted and inspected at3sizes plus resize/resume; source loader-driven entry/refresh exposed. Real gameplay owner/completion remains pending.

## v87 original loading readiness callbacks

See LOADING-MULTIPLAYER-v87.md.972originalARM cases and31139Android AS checks passed; real owner bindings remain pending.
