# Development encounter persistence on Android 17

This checkpoint adds cold-process persistence to the authored development
encounter. It does not implement the original campaign save format or save
semantics.

## What is persisted

The native `DH2S` version 1 payload records a content-generation fingerprint,
encounter definition, stable actor spawn IDs, transforms and cooldowns, full
owned actor property sheets and death state, counted-quest progress, and random
generator state. Restore and Reset construct and validate a candidate session;
the live session is replaced only after every field and actor has passed checks.
Sentry order does not define identity.

`EncounterSaveStore` wraps native payloads in a checksummed `DHF1` envelope and
uses two same-directory atomic replacement slots with one process-wide writer.
It tries the newest valid candidate first, retains rejected/corrupt files, and
does not overwrite an incompatible save until the player selects Reset. Failed
writes leave the other slot intact and report an error in the HUD. Lifecycle
checkpoints occur as combat changes and when the activity pauses or stops.

The native payload is an authored encounter schema. There is no migration from
original `.savegame` files, no inventory/reward/campaign state, and no
compatibility promise across future authored encounter schema revisions. The
save store's Java host tests exercise its file semantics; the exact Android APK
tests below exercise the installed runtime and app-private storage.

## Verification

APK: **1,234,908 bytes**, SHA-256
`aa557d05aa9d6bdacb63f61f879289402de418f00c43a52d0a2b38c0a3504094`.

- Android 17 API 37.0 x86_64, 4 KiB pages: installed build hash matched, held
  Attack to reach **2/2 COMPLETE**, force-stopped the process, then relaunched
  and observed **Progress restored** with **2/2 COMPLETE**.
- Android 17 API 37.2 x86_64, 16 KiB pages: same exact build hash, victory and
  cold-process restoration passed.
- Native persistence runner with original encounter inputs: 3,096 corrupted
  saves and 3,096 truncations rejected; exact mid-combat restore, reordered
  named-spawn records, transactional rollback, 120 deterministic continuation
  steps, victory, Reset and defeat restoration passed.
- Java save-store tests: 128 corruption checks, 128 truncation checks, atomic
  two-slot writes, corrupt-newest fallback, rejected-save retention, stale
  owner/reset queue protection and write-failure rollback passed.
- Both Android ABI libraries were compiled from source and checked for 16 KiB
  ELF segment alignment. API 37.2 Java Activity compilation and strict native
  compile checks passed.

The 4 KiB and 16 KiB emulator screenshots and native reports are generated
locally under `work/emulator-test/` and the ignored
`port/android-app/build/persistence-tests/` directory. Build command:

```powershell
python port/android-app/build.py --sdk <Android-SDK> --ndk <NDK> --cache <unpacked-cache-files>
```

The release APK is available [in Google Drive](https://drive.google.com/file/d/1ZGjquZoWE-mXx8iB4OtpaqxlhlgyPzPP/view?usp=drivesdk).

## What this does not prove

The encounter remains the authored `void_maze` cross-room test with two
development sentries. The SWAMP MLX/MGP/MVP importer is a separate source
component under [`port/world-data`](../world-data/README.md): its host and
ARM64 checks validate recovered placements, but it is not connected to this
APK's renderer or gameplay. The original world lifecycle, object activation,
AI, command and dialog execution, actual rewards, inventory/progression, audio,
all-level navigation and original campaign saves remain unfinished. Rights and
asset redistribution limits are documented in [`RIGHTS.md`](../../RIGHTS.md).
