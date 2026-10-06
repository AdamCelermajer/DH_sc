# Authored character panel native platform V4

The platform borrows the existing `AuthoredMenuApplicationFieldsV3` and
`AuthoredMenuTouchScreenV3`. It owns no Save, Gear, player or additional movie.
Keep `AuthoredCharacterPanelPlatformV4` at a stable address and destroy the
panel before the platform and its native owner lease.

## Root integration

Construct `AuthoredCharacterPanelPlatformServicesV4` with the native application
lease, existing fields/touch pointers, Debug producer and panel graph-scope
callback. The callback forwards `AuthoredCharacterPanelV2::scoped_graph`, which
reuses the active graph while startup ActionScript is executing. It works before
`movieLoaded`; it does not require `bound()`.

Retain one `AuthoredMenuCharacterProjectionV4` on the native platform. Bind
`deadzone_root` to its `root(graph, actualMenuName, ...)` and `absolute_bounds`
to its corresponding method. Projection nodes borrow actual display-list
characters for that Scope only. Their child order is the actual display list;
the menu deadzone collector uses source flags0 and does not read the missing
Gameloft focus field. Bounds use the actual virtual rectangle plus the ordered
parent-chain translations, then divide by20. They do not apply a second world
matrix to the rectangle.

Call `platform.bind(panelServices,error)` before panel initialization. The call
publishes the existing Application globals and installs lifecycle and remaining
stack callbacks. The normal path has concrete Debug, rollover/EC/IGM writes,
manager60 clearing, listener registration/unregistration and source touch
reset/process. Empty touchscreen work is an actual source branch; a nonempty
queue/release requires the real producer. Android mouse transport is not
represented as an original TouchScreen queue.

Supply actual HUD/base movie identities and leases with flags4/84. Use
`authored_menu_stack_character_v4(object, flags, focusProducer, out, error)` in
the owning Scope. A real source focus producer is required only when bit8 is
reached. No field is inferred from stock GameSWF `m_enabled`.
`authored_menu_movie_rect_v4` reads the actual root movie definition frame
rectangle in twips for the shared display/touch viewport seed.

`AuthoredMenuMovieStackV4` is available when genuine external MenuBase fields
exist. Bind its fields lookup to those same receiver fields and its scope lookup
to the actual retained movie by RenderFX identity. It implements Show/Hide,
literal MenuBase focus/blur, validity, visibility, AS callbacks and source
PlayAnim. Do not manufacture external menu states just to exercise this owner.
Unreached unsupported RenderFX reset/focus, settings/drag/language/Options and
license services remain explicit required continuations.

## Recovered source

`RenderFX::PlayAnim7aba04` forwards to `GotoFrame7ab924` with play=true. The
whole body checks sprite type2, invokes its actual label jump, and sets PLAY0
only after a successful result. Both the panel and external movie receiver now
use `authored_menu_play_animation_v4`; request.result receives actual label
acceptance, preserving focus_in-to-show fallback.

Multi.PushMenu438514 loads GOT996ba4, which points to the one-byte
`USE_NATIVE_DRM_GAME9f640e` in `.bss` (`SHT_NOBITS`). There is no NativeDRM
getter function at this site. The actual byte read controls the branch to
`ALicenseCheck_ValidateLicense89becc`, a thunk to89bdc0. GSInit and Level also
read this byte. `AuthoredMenuNativeDrmV4` owns the BSS initial zero byte and
exposes its read; no runtime writer or license-validation success is claimed.
The capture lists literal candidates separately from confirmed source reads.

## Verification and limits

All four changed/new production TUs pass strict ARM64 syntax checks (vendor
GameSWF warnings are excluded consistently with its existing target). Isolated
native fixture `authored-menu-platform-v4` passed on emulator5554; binary SHA256
`effeb0108935d01fb397162881c3d5b6f28b1d6194c26353f3f8a05c416f9396`.
Receipt is in `port/level-world/reports/android-native-owner-tests/`.
It verifies same-owner scalar/listener/touch effects and required failure
prefixes using declared callbacks. It does not prove a live movie, portrait
alignment, GPU rendering, original DRM runtime writes or full settings flow.
Root owns the application binding and live original-menu validation.
