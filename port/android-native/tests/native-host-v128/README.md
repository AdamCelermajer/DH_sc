# Actual native application on the desktop

This harness builds the same `dh2_native` sources and engine libraries as the
Android application, then calls the shipping JNI initialize/resize/draw/input
functions through a real desktop JVM. It uses a real software GLES2 context
and the shipping cache reader/asset bytes. There is no Android OS, QEMU, fake
JNIEnv, fabricated World/Level, or success-only menu/gameplay provider.

The desktop asset adapter supplies filesystem reads and positional descriptors
over the real asset directory/cache ZIP. The only shipping-code observation
hook is enabled by `DH2_NATIVE_HOST_BUILD`: it reports retained startup/menu
phase counters and active menu names without advancing or replacing them.
Android builds default that option to OFF.

From PowerShell at the repository root:

```powershell
& port/android-native/tools/run_native_host_v128.ps1
```

Prerequisites in Ubuntu/WSL: CMake, Ninja, GCC/G++, zlib development files,
`libegl1-mesa-dev`, `libgles2-mesa-dev`, and `openjdk-21-jdk-headless`.
The runner uses the installed Android SDK's VM-neutral JNI/AAudio declarations;
it copies those two API headers into its ignored build directory. It does not
copy or modify the original game archive. Output defaults to
`.local-inputs/native-host-v128`.

Compilation is serial. Each process has a 3.5 GiB virtual-address-space limit;
the Java heap is limited to256 MiB and Mesa uses two software-rendering threads.
The build and run have finite timeouts. The PowerShell wrapper rejects overlap
with an active Android emulator. These are per-process limits; they do not
claim that the WSL VM's total commitment is exactly the Java or compiler limit.

The first scenario drives real GSInit, all reached MenuManager.Init phases,
and30 ordinary frames of the actual main-menu state. It records phase changes,
native logs, a PNG screenshot, timing and peak resident memory. Any required
native callback failure makes the run fail. The supported intro skip event
is selected explicitly; the media request and native cache callback still run.
The desktop reports no mobile music-management API and does not publish an
Android audio-focus/device-output grant. Intro codec playback and audible
AAudio output are outside this scenario's coverage. A startup PASS would not
by itself establish character creation, Play, gameplay, save/reload, original
ARM32 parity, Android lifecycle or final phone-rendering acceptance.

Current compilation/runtime receipts, rather than this harness description,
determine which stages actually passed. `run.log` contains the genuine reached
failure if startup stops; `failure.png` captures the frame when possible.
