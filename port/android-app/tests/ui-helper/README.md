# Local Android UI snapshot helper

This small instrumentation APK reads the active accessibility tree directly,
without waiting for an animated interface to become idle. It is used only by
the emulator test; it is not included in the source renderer or game APKs.
The Android SDK and JDK are its only build dependencies.

Build the source app first (which creates its local debug signing key), then:

```powershell
python port/android-app/tests/ui-helper/build.py --sdk PATH_TO_ANDROID_SDK
```

Pass the resulting `build/dh2-ui-helper.apk` to the animation runtime script
using `--ui-helper`. The script requires an emulator, installs this helper,
then invokes `local.dh2.uitest/.Snapshot` for each screen read. Snapshots are
base64-encoded XML in the instrumentation result. The helper waits at most
five seconds for an active root and bounds traversal to 80 levels/10,000 nodes.
The test retries transient screen transitions and checks fresh results.

The original `uiautomator dump` path remains available without `--ui-helper`
for older static preview builds. The live playback time display requires this
helper: the old dumper's idle wait fails while text changes continuously.

The helper was built and used on Android 17 API 37 with both 4 KiB and 16 KiB
pages. Its exact hash and source hashes are in [build-validation.json](build-validation.json).
The source app's [timeline runtime report](../../timeline-runtime-validation.json)
records the helper identity used in each run. Physical-device automation and
other Android versions remain unverified.
