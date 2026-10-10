# Startup broad check and host execution

Follow-up: the V128 desktop application harness is now implemented and building
under `port/android-native/tests/native-host-v128`. It uses the same native app
sources and existing JNI entrypoints with a real JVM, real Mesa GLES2 context
and filesystem asset adaptation. Runtime receipts are still pending. Its README
records the deliberate intro skip and device-audio coverage limit. The earlier
missing-host-composition discussion below describes the V127 checkpoint.

The emulator-per-error loop is stopped. The new host batch runs the current
shipping C++ ZIP reader and process Arrays decoder directly in Ubuntu/WSL.
It uses the supplied cache ZIP in place, with no extraction or replacement
table parser. It does not launch Python, Android, a World, or a Level.

## Verified batch

Run from the repository root:

```powershell
& port/android-native/tools/run_startup_host_v127.ps1
```

The final current-source run returned exit 0, with zero reported failures:

| Check | Result |
| --- | --- |
| Canonical archive directory | 6,833 entries |
| Read/decompress/CRC every asset | 6,833 pass, 648,357,710 decoded bytes |
| All registered Arrays/name streams through exact EOF | 70 pass |
| Constant-file URI inventory | 27 pass; presence only, not constant decoding |
| Trophy schema and four primary menu movie URIs | 5 pass |
| Empty / incident-sized / byte-impossible top-level counts | 210 rejected |
| Nested-vector counts in four actual table streams | 8 rejected |
| Measured execution time | 7.59 seconds, excluding compilation |
| Peak runner resident memory | 51,236 KiB, approximately 50 MiB |

The archive sweep holds only one payload at a time. Every process in the
runner has a 2 GiB virtual-address-space limit; compilation is serial and
time-limited, and execution has a 180-second timeout. That limit is per
process, not a claim that the entire WSL VM occupies at most 2 GiB.

Receipts are `.local-inputs/startup-host-v127/results.jsonl`, `resources.txt`
and `build.log`. The runner explicitly reports `full_app_startup_verified=false`.

## Confirmed launch blocker and source repair

Last device observation remains the installed V126 x86_64 APK, SHA256
`e65cab43382adc220fdd555c6250916df8cefd78afc3bb27dc3a940333bb8733`.
It passed all corrected table streams and trophy loading, completed the intro,
displayed the splash, and failed at `Required SAME canonical InfoHUD operation 3`.

Operation 3 is `saved_option`. Original MenuManager.Init phase 4 initializes
InfoHUD/HUDControls before any World. Both legitimately read the process
Application's `HUDStyle`. The native adapter wrongly required campaign World
transport for that setting. `NativeMenuPostMovieV62::info` now borrows the
actual retained Application settings and reads the option before its World
fallback. Missing option keeps the source value 0; missing owner/key fails.

The repaired source compiled and linked successfully into the x86_64 native
library with Ninja `-j1`; `.local-inputs/act1-coordinator-v127-native.log`
is the receipt. No APK was repackaged or launched for this host-check turn.

The broader HUD audit checked the surrounding routes: operations 4/5/6 are
already handled by the same retained movie/core; null current Level is valid;
normal HUD gameplay updates remain behind the original in-game-view gate.
No temporary World or success-only provider is added.

## Wider startup dependency audit

Read-only audits cover GSInit stages 0–14, MenuManager.Init phases 0–26,
PostLoad/Hide/Show, pre-World HUD callbacks and V121/V122 preview services.
Their detailed evidence is in `coordination/luna-act1/lane-02-handoff.md`,
`lane-03-handoff.md`, and `lane-06-handoff.md`.

The V122 animation AddAnim/AddTemplate rejection is **not a confirmed blocker**.
The real same-Character animator handles those operations before delegating
other calls to that defensive boundary. An early suspicion was corrected by
following the complete call chain.

Remaining source questions requiring actual provider execution or additional
original evidence include:

- GSInit stage 14's original global/console branch versus the current
  `new_settings()` choice; equivalence is not proven.
- Selected-profile/first-local publication on preview/Play paths. Cold menu
  updates can legitimately borrow the constructor-created dummy PlayerInfo
  with null Character: source `PlayerManager::GetLocalPlayer` 0x36e478 forwards
  its selected internal ID to `GetPlayerByInternalID`, and the current retained
  manager preserves that dummy fallback. Original App.PostInit 0x32f7e8 creates
  the PM without adding a local player. This is not itself a cold-update blocker.
- Conditional preview Save/FX/AIS/script-host callbacks and their actual
  reached branches; unconditional error strings alone do not prove reachability.

## Scope of execution without the Android emulator

The current batch verifies cache integrity and real table decoding/safety.
Existing host targets also execute many UI, Lua, animation, physics, save,
and menu kernels. They are component checks. There is currently no complete
host application that composes the real GSInit, all native menu providers,
preview player, Show/Hide/Play and gameplay loop together.

The next useful host seam is to factor that retained process composition out
of `native_app.cpp` and use real filesystem, clocks, cache and CPU SWF services
on the PC, keeping the same source owners. Missing native callbacks must fail
and be reported by scenario; a fake World or a callback that always succeeds
would conceal the very integration errors this check needs to find.

Android/JNI lifecycle, graphics uploads/rendering, platform media and touch
still require a final combined device check. A connected Android phone can
replace the emulator for that check. No full app launch, menu acceptance,
Swamp gameplay, save/reload milestone or ARM64 package is claimed by this report.
