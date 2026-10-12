# Owned effects cache reconstruction

The native loader and preload backing preserve the five exact original cache
streams. `original-reader-projection.json` records execution of the original
table and row readers from the verified game library, with explicit raw-stream
and allocator services. The independent EFX1 projection contains every loaded
field, name, dictionary entry and consumption boundary.

The cache contains 276 sets with 276 steps, three character rows, seven
footstep rows and 284 dictionary entries. Record boundaries are 12,700,
12,755 and 12,953 bytes; dictionary paths consume 16,770 bytes. The footstep
floor field is a string: the original row's length word initially resembled
a scalar, and the complete reader projection corrected that interpretation
before the native loader was validated.

`EffectsTables` owns an immutable snapshot. Its borrowed views retain all rows
and strings after the input buffers and loader are destroyed. Failed reloads
preserve the previous snapshot, and a reload while borrowed is rejected.
Native allocation limits, schema checks and dictionary uniqueness are explicit
port contracts; they are not claimed as original-reader guards.

`PreloadBacking` pins the same snapshot and prepares stable File/Redir arrays
for the separately proven registration kernel. The central test replays the
complete original projection, 650 malformed-input rejections and 200 real
FX77 registration calls through the genuine shared debug module/file service.
All 1,712 checks pass with ASan/UBSan and leak detection. The later InitFX
helper separately proves source character-row registration order and invalid
Grab handling.

The Android world context owns these tables, queue storage and the real debug
module adapter. Five hash-checked streams are included in both projects, and
the emulator checks identical snapshot/module identities across rotation.
This is ownership preparation. FX instance factories, pools, scene attachment,
particle rendering and complete original startup/frame execution remain
required; the live context does not invoke the startup helper in a fabricated
order.

Evidence:

- `original-reader-projection.json` and `original-reader-projection.bin`
- `../../../level-world/reports/character-effects-tables-main-linked-host-audit.json`
- `../../../level-world/reports/character-init-fx-main-linked-host-audit.json`
- `../../../android-native/reports/effects-tables-stage.json`
- `../../../android-native/reports/native-effects-tables-source-build-inspection.json`
