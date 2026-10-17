# Historical Android 17 Test 5 x86_64 wrapper loader diagnosis

The official API 37 x86_64 emulator can start the published Test 5 wrapper and load its ARM64 `libzbridge.so` through `libndk_translation.so`. Its package record has `primaryCpuAbi=arm64-v8a`, while the system advertises `x86_64,arm64-v8a`. This does not make the Java process an ARM64 process: the emulator runs its x86_64 runtime with a native bridge for supported ARM64 libraries.

The game plugin uses a separate `PluginClassLoader`, a `DexClassLoader` subclass. For a translated plugin its constructor passes `null` as the native library search path. The API 37 log then shows `nativeloader` creating an isolated namespace with `library_path=` empty. When plugin code requests `libnativeinterface.so`, `findLibrary` returns an ARM64 proxy under the wrapper's private files directory, but that namespace uses the x86_64 loader and rejects it:

```text
.../proxy/libnativeinterface.so is for EM_AARCH64 (183) instead of EM_X86_64 (62)
```

This is consistent with Android's [native loader namespace code](https://android.googlesource.com/platform/art/+/d808f69a57/libnativeloader/native_loader_namespace.cpp): without a parent native namespace, it calls `NativeBridgeIsPathSupported(search_paths)` only when the search path is nonempty; otherwise it creates a native namespace. The [native loader](https://android.googlesource.com/platform/art/+/master/libnativeloader/native_loader.cpp) creates such an isolated namespace for a custom class loader on first native load. The published wrapper's `libzbridge.so` loads through its main class loader's bridged namespace, so its success does not prove that the plugin's namespace is bridged.

The source of this branch is visible in `PluginClassLoader.java` in the local ZettaBridge research checkout. This was the blocker for the published **Test 5** package, not the current experimental wrapper. Later Test 7 work supplied a supported ARM64 native-library search path and bundled ARM32 sysroot aliases; the API 37.0 x86_64 emulator then loaded the proxy and started the guest engine. Subsequent Test 9 and Test 10 packages reached bounded gameplay, as recorded in [the Android 17 results](ANDROID-17-EMULATOR-RESULTS.md). On an ARM64 Android host, the plugin loader naturally chooses a native ARM64 namespace, so this specific x86_64 mismatch does not explain production ARM64 failures. None of these emulator checks establishes broad device compatibility.

Evidence: `work/emulator-test/api37-4k-wrapper-logcat.txt` in the local task workspace (the raw log is not committed), plus the AVD package and system ABI properties captured on 2026-10-02. No Fold7 testing was used for this diagnosis.
