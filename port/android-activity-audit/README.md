# Android activity and latest-platform audit

Audit date: 2026-10-03  
Scope: Android manifest, activity routing, view layouts, touch handling, and the checked-in API 37 runtime evidence. This audit is static; the API 37 emulator is reserved for the parent task's exact-candidate rebuild test. No Fold7 or physical device was used.

## Current navigation

- The launcher is `GameplayActivity`, a development encounter, not a title/menu screen (`AndroidManifest.xml:6-10`).
- Its Diagnostics button opens `MainActivity`; its SWAMP and INFECTED VILLAGE buttons open their corresponding preview activities (`GameplayActivity.java:201-207`).
- Both preview activities have a visible Return to encounter button which calls `finish()` (`SwampPreviewActivity.java:228-232`, `InfectedVillagePreviewActivity.java:235-239`). Android Back also follows the default Activity finish behavior because none of these Activities overrides Back handling. The saved API 37 cycle evidence verifies the infected preview's explicit return button three times, not the system Back gesture.
- There is no main-menu route from `MainActivity` back to the encounter; system Back exits `MainActivity` to its previous Activity, if one exists. The launcher itself is the encounter.

## Latest Android API 37 findings

The build targets API 37, has min API 26, and now records Build Tools 37.0.0 (`port/android-app/build-validation.json`). The source manifest has no `android:appCategory` on `<application>` and requests `sensorLandscape` for Gameplay, SWAMP, and INFECTED VILLAGE (`AndroidManifest.xml:4-13`). Android 17 targeting API 37 ignores orientation and resizability restrictions on `sw600dp+` displays unless the app is classified as a game; the manifest currently does not classify it as one. Large-window portrait, rotation, and multi-window behavior therefore need responsive layouts rather than relying on these orientation declarations. The Android 17 emulator evidence in the existing runtime JSON is a phone-sized 16 KiB x86_64 profile and does not cover this large-window behavior.

Targets at API 35+ run edge-to-edge on Android 15+. Gameplay applies legacy system-window insets to the root, while MainActivity applies the same deprecated inset getters to a root `LinearLayout`; the two static preview activities do not apply insets to their controls. All preview/gameplay screens use deprecated `SYSTEM_UI_FLAG_*` immersive flags. This means gesture navigation, transient system bars, display cutouts, and caption bars on resizable desktop windows have not been verified. The fixed 16dp return-button margin and SWAMP movement pad can approach system gesture regions.

### Diagnostics screen: source fix made, runtime verification pending

`MainActivity` previously placed roughly sixteen fixed-height controls/status rows above the weighted GL surface without a scroll container. The source now keeps the GL preview and a vertically scrollable controls pane in separate weighted panes. Landscape content at least 720 dp wide uses side-by-side panes; narrower landscape and portrait use stacked panes. The layout recomputes after window size changes, and root padding is density-scaled. This source-only change does not alter gameplay or native rendering. Host policy/source checks pass; API 37 runtime verification of Diagnostics across portrait, landscape, and narrow resize is still pending. See [RESPONSIVE-DIAGNOSTICS-FIX.md](RESPONSIVE-DIAGNOSTICS-FIX.md) for the implementation and exact retest.

### Preview layout and touch notes

- SWAMP positions fixed 410dp and 430dp panels side-by-side at the top, and places fixed controls at the bottom (`SwampPreviewActivity.java:195-232`). Those panels overlap or spill beyond narrow/portrait windows. Its movement pad tracks a pointer and clears movement on cancel/up; the source intro trace has a scroll panel.
- INFECTED VILLAGE has a full-screen GL surface, a top overlay, drag orbit/pitch, pinch zoom, and a return button (`InfectedVillagePreviewActivity.java:196-239`). The existing runtime report explicitly marks pinch as unverified. The handler switches between two-pointer pinch and one-pointer orbit without resetting its one-pointer anchor when a pinch pointer is lifted; verify that transition to catch a possible orbit jump.
- Gameplay keeps its movement pad and attack control anchored to lower corners (`GameplayActivity.java:231-258`) and routes into both previews. Its top action row and overlay are fixed-position and have no responsive rules for arbitrary window sizes.

## Runtime evidence and limits

`port/android-app/infected-village-preview-runtime-validation.json` records three successful API 37, x86_64, 16 KiB emulator cycles for the APK SHA-256 `1a57732eb509b6c67a358617b2301351d907e26fc7edd061a553398e38685deb`: open, render, and return to encounter. The saved view hierarchy contains a rendered surface, readable overlay, and enabled Return to encounter button. Drag orbit and pitch passed. The report says no matching app/renderer error lines were captured. It also explicitly excludes 4 KiB runtime, pinch, native render parity, and gameplay. This evidence belongs only to that hash; re-run the same gate if the current Build Tools 37 APK hash differs.

This audit did not interact with the emulator because it was reserved for the parent task's rebuild retest. MainActivity, SWAMP, hardware/system Back, transient system bars, rotations, and resize/multi-window cases have no equivalent checked-in API 37 runtime evidence.

## Suggested API 37 runtime test sequence

1. On API 37 x86_64, open the launcher encounter, open Diagnostics, verify the GL preview remains visible while scrolling to and activating the last import control, then return with system Back. Repeat in portrait, landscape, and narrow landscape resize/rotation. Bind screenshots or view-hierarchy evidence to the tested APK SHA-256.
2. Open SWAMP; run and scroll the intro trace, move with the pad, cancel a touch, use the visible return button, and repeat using system Back.
3. Open INFECTED VILLAGE; verify drag, pinch, pinch-to-drag transition, system Back, and the visible return button.
4. Rotate and resize an API 37 large-screen/resizable emulator (not a Fold7), then confirm all controls remain reachable and GL surface aspect changes without losing the activity or crashing.
5. Capture package-scoped logcat for each transition and bind the evidence to the tested APK SHA-256.

## Commands used

```powershell
git status --short
Get-Content port/android-app/AndroidManifest.xml
Get-Content port/android-app/src/local/dh2/sourceviewer/MainActivity.java
Get-Content port/android-app/src/local/dh2/sourceviewer/GameplayActivity.java
Get-Content port/android-app/src/local/dh2/sourceviewer/SwampPreviewActivity.java
Get-Content port/android-app/src/local/dh2/sourceviewer/InfectedVillagePreviewActivity.java
Get-Content port/android-app/infected-village-preview-runtime-validation.json
Get-Content port/android-app/build-validation.json
rg -n "FATAL EXCEPTION|Fatal signal|ANR in|Force finishing|DH2InfectedVillage|AndroidRuntime|libEGL|OpenGLRenderer" port/android-app/build/infected-village-candidate-16k-logcat.txt
```

Platform references: [Android 17 orientation and resizability behavior](https://developer.android.com/about/versions/17/changes/ff-restrictions-ignored), [Android edge-to-edge guidance](https://developer.android.com/develop/ui/views/layout/edge-to-edge).
