# Diagnostics layout update

Date: 2026-10-03

`MainActivity` now keeps the GLES preview in a dedicated pane and puts the complete diagnostics control list inside a vertical `ScrollView`. The preview and controls each retain a weighted share of the available content area, so short landscape windows no longer push the preview or lower import buttons off-screen. Window padding is now expressed in dp.

`ResponsiveLayoutPolicy` switches to side-by-side preview and controls for landscape content at least 720 dp wide. Narrow landscape and portrait content stay stacked, with 45% of the content height reserved for the preview and 55% for the scrollable controls. The panes re-evaluate their arrangement when the window bounds change, including resize and rotation. This only changes the diagnostics activity's view layout; it does not change renderer, scripts, or gameplay/native-engine behavior.

## Host verification

Run from the repository root:

```powershell
C:/Python313/python.exe port/android-activity-audit/tests/run_responsive_layout_test.py
```

The test checks the scroll container and resize listener wiring in `MainActivity`, and compiles/runs the platform-independent Java layout policy across portrait, wide landscape, the 720 dp breakpoint, narrow landscape, density scaling, and unmeasured dimensions. It passed on the host JVM. It does not compile the Android activity or prove actual touch/GL behavior.

## Required Android retest

After the next APK build, use an API 37 emulator (not Fold7) to open Diagnostics from the encounter. In portrait, landscape, and a narrow resized landscape window, verify that the preview remains visible, scroll to and activate the final import control, and confirm resizing/rotation preserves the activity and GL surface. Record the tested APK SHA-256 with screenshots or view-hierarchy evidence. Do not treat the existing INFECTED VILLAGE runtime evidence as validation of this screen; the root agent owns emulator `emulator-5558` for its current candidate test.
