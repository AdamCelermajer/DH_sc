# Source scripts inside the Android 17 character preview

The newer [property integration checkpoint](PROPERTY-INTEGRATION.md) adds owned
property data imports and typed script methods. The package and reports below
retain their earlier identities.

The source APK is **730,019 bytes**, SHA-256
`ffcb32a34ed5de300ca05420bfb67637a89e64264b581a855f918a1e79d8b211`.
It contains source-built ARM64 and x86_64 renderer/Lua libraries, with 16 KiB
ELF/ZIP alignment, target SDK 37 and minimum SDK 26. It contains no original
engine library or ARM32 translator. This is a character/scene preview with
working shared scripts; full source-built gameplay remains unfinished.

## Use

Install the APK on Android 17. The initial script status should read
**Scripts ready: 3 shared files. Game objects are not connected yet.**
Import the warrior model, texture and animations described in [the renderer
guide](README.md). **Import script source** opens the standard document picker
and executes a plaintext Lua file in the activity's current runtime.

The three unchanged packaged sources total 60,830 bytes:

| Asset | Bytes | SHA-256 |
| --- | ---: | --- |
| `ai-commons.lua` | 13,535 | `20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c` |
| `skills-commons.lua` | 11,291 | `f3abf69124728bab3d344b56e8f3cb67127fbea4a8d1a7ab1d7401f2a2132e9d` |
| `combat-formulas.lua` | 36,004 | `f83639a12c5c1910f9a23fff494f4013330943ad4ef8f4d91c33fe1eb975565f` |

The builder checks these hashes against the recovered source manifest. The
activity owns one 8 MiB runtime, with a 1 MiB source limit and one million
instruction budget per import. Closing the activity destroys its runtime.
Rejected execution clears the stack and instruction hook; earlier global writes
can remain, so imports are not transactions. Error messages are ASCII sanitized
before crossing JNI. This runtime is not a security sandbox.

## Exact-package checks

The installed APK hash matched on Android 17 x86_64 with both 4 KiB and 16 KiB
pages. Each run checked the three asset hashes and seven imports in the same
process: field/shared-helper assertions, infinite-loop rejection, non-string
error, invalid text error bytes, Lua bytecode rejection, memory exhaustion, and
successful recovery. No current-run fatal error occurred. See
[4 KiB evidence](scripts-4k-runtime-validation.json) and
[16 KiB evidence](scripts-16k-runtime-validation.json).

The same APK passed textured warrior import, two animation imports, midpoint
seek, paused 0/50/100% mixing, advancing Play at 100%, and stable Pause on both
page sizes. All six mix screenshots were visually inspected. See
[character regression evidence](scripts-animation-runtime-validation.json).
ARM64 is built and alignment checked; ARM64 hardware execution is unverified.
No Fold7 was tested.

## Remaining work

The shared scripts define helpers and combat formulas, but native game objects,
typed character properties, real combat callbacks, AI, navigation, level
progression and the game loop are not connected. Array/constant cache loading
is not exposed by this APK. Its diagnostic animation timing and pose mixing
retain the scope in the renderer guide. Original game metadata/assets retain
the repository's [rights statement](../../RIGHTS.md).

## Reproduce

```sh
python port/android-app/build.py --help
python port/android-app/tests/script_runtime.py --help
python port/android-app/tests/animation_runtime.py --help
```

Build with the Android SDK/NDK and JDK paths accepted by the builder. The tests
require a running Android 17 emulator, adb, and the owner's cache fixtures.
