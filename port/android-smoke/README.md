# Android asset-reader smoke check

This source-only command-line harness links the reconstructed texture parser and PVRTC decoder with the checked BRES, image, effect and material readers. It creates tiny synthetic BTEX/PVRTC 2bpp and 4bpp textures and a BRES image in memory, checks decoded red RGBA pixels, a local effect link, sampler/image indices in both effect groups, and malformed-input rejection. No original game bytes are embedded or committed.

Build with a host C++17 compiler and Android NDK r29:

```text
python port/android-smoke/build.py --ndk PATH_TO_NDK_R29
```

The command runs the synthetic fixture on the host and writes `build-validation.json`. It also builds an Android x86_64 PIE executable under the ignored `build/` directory and checks its ELF machine, Android dynamic linker and all `PT_LOAD` segments for 16 KiB alignment. NDK r29's API 35 target is used for the executable, which runs on the API 37 Android 17 AVD. The build report links runtime evidence only when the current executable SHA-256 matches the runtime report.

Optional private real-cache fixtures can be checked on the host without putting them in Git:

```text
python port/android-smoke/build.py --ndk PATH_TO_NDK_R29 \
  --texture PATH_TO_CACHE/files/data/3d/textures/atlas_modular_character_01.tga \
  --bres PATH_TO_CACHE/files/data/3d/animateddecors/candle_flame.bdae
```

Run on an Android 17/API 37 x86_64 emulator with 16 KiB pages:

```text
python port/android-smoke/run_android.py --adb PATH_TO_ADB --serial EMULATOR_SERIAL \
  --texture PATH_TO_CACHE/files/data/3d/textures/atlas_modular_character_01.tga \
  --bres PATH_TO_CACHE/files/data/3d/animateddecors/candle_flame.bdae
```

The runner verifies OS release, API, ABI and page size, pushes the executable and optional private fixtures only to `/data/local/tmp`, verifies their SHA-256 on-device, runs synthetic and optional real-asset checks, and records [`runtime-validation.json`](runtime-validation.json). The recorded API 37 run on a 16 KiB page x86_64 emulator returned exit code zero. It decoded the 1024×1024 owner-supplied texture and parsed the 16,856-byte candle BRES with 3 images, 2 effects and 4 materials. It did not access or change the installed game package. A passing command proves these new reader functions execute on that Android image; it does not test the original game engine, rendering, input, or gameplay. The generated executable and private asset bytes stay outside Git.
