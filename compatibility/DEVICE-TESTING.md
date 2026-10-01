> Test 2 update: the Fold7 report confirms 4096-byte pages and native library/JNI startup. A MediaStore query crash was identified and repaired; the repaired APK awaits a device retest. See [TEST2](work/fold7-build/TEST2.md). The initial test 1 evidence below is retained historically.

# Fold7 device acceptance procedure

This is the remaining gate for the user-requested working app. No row below has a phone pass result yet.

Record phone model, One UI version, Android build/security patch, CPU ABIs, host page size, APK SHA-256, cache SHA-256/size, available storage, and whether the test uses the cover or inner screen. The setup app's diagnostic report supplies several of these.

| Order | Action | Expected result | Evidence to retain |
| --- | --- | --- | --- |
| 1 | Install APK without removing the original game | Separate Fold7 Test app appears and opens | Install error or setup screenshot |
| 2 | Wait for preparation and open diagnostics | Libraries prepare; page size is 4096 | Full diagnostic report |
| 3 | Import complete cache ZIP from Downloads | Import completes; destination is shown | Cache hash, file layout, import message |
| 4 | Tap Launch game | Game activity reaches its menu | Video/screenshot and logs on failure |
| 5 | Start a new game and enter a level | Correct textures, UI, lighting and character rendering | Screenshot of any missing/incorrect graphics |
| 6 | Exercise movement, combat, inventory and menus | Touch coordinates and UI actions are correct | Reproduction for dropped/misaligned input |
| 7 | Play with sound; change volume and background/resume | Music/effects work and recover | Audio symptom and lifecycle sequence |
| 8 | Save, exit, force-stop app, reopen and load | Progress/preferences persist | Before/after saved state |
| 9 | Switch between cover and inner screens, fold/unfold and rotate where allowed | Activity remains usable; surface and touch recover | Sequence and screen recording |
| 10 | Lock/unlock phone; switch apps; return after memory pressure | No crash or unrecoverable black screen | Diagnostic report and lifecycle details |
| 11 | Run at least 30 minutes and transition through multiple areas | Stable gameplay with no sustained stalls or memory failures | Duration, battery/thermal observations, last action |
| 12 | Test offline/online startup and intended legitimate service flows | Behavior is understood; obsolete-service failures are separated from local crashes | Endpoint/error information, without credentials |
| 13 | Exercise multiplayer only after single-player passes | Permissions and connection behavior are documented | Permission prompts and connection results |

If the app reports a non-4096 page size, record it and stop the gameplay acceptance run. The existing build cannot resolve that issue through cache placement.

If it crashes, reopen **View / share diagnostic report**. Include the first failing step, what appeared onscreen, and whether it happened before or after cache import. With ADB available, collect a time-bounded logcat around reproduction; review it for unrelated personal information before sharing publicly.

## Result template

```text
APK SHA-256:
Phone/model:
Android / One UI / build:
Page size:
Cache SHA-256 / bytes / layout:
Screen (cover/inner):
First failing step:
Expected:
Observed:
Repeatable (attempts/failures):
Diagnostic report:
Supporting screenshots/logs:
```

Completion means the required device cases pass with recorded evidence, plus fixes and reruns for any actual failures. The existing host tests cannot establish this result.
