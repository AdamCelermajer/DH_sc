# Owned original HUD settings v1

This batch implements the complete private settings map/read ownership needed by
`NativeLoadSettings`, with explicit mandatory world inventory, audio and device
boundaries. It does not implement the distinct campaign/profile chunk save
owner, save writes, or accept unavailable game effects as successful callbacks.
The existing startup wrapper and historical d1 receipts remain unchanged.

## Original evidence and source order

`source-capture/original-functions.json` binds 26 actual ELF routines to original
SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Actual original manager execution supplies 254 full-owner gold cases. The source
manager uses its real STL map, constructor, descriptor pointers and getters;
storage/file-copy, an explicitly empty ObjectManager graph, TextManager switch
and the platform enum are services. A separate 162-case original/O2 graph proof
executes nonempty setLanguage traversal and the original IsPlayer, IsMerchant and
IsGameObject wrappers, with type and inventory/item bodies supplied as services.

* `SavegameManager` constructor `46cd4c`: file null, flags false, hint -1 and
  orientation false. Its tutorial bytes are not source-initialized here; native
  storage is defined zero until actual `_initSettings` sets all 14 to one.
* `_initSettings` `46e47c`: clear prior option nodes; false mode copies actual
  serialized descriptor defaults into a private map in resource order; true mode
  creates no default option entries. Duplicate/C-string-prefix keys overwrite.
  All 14 tutorials become one. Existing loaded/new flags are preserved.
* `loadSettings` `46e584`: delete previous file, initialize settings, create raw
  `Savegame("dh2_settings.savegame", true)`, assign file, then set language from
  hint before parsing. `_cacheFile` `315ad0` flag +38==1 retains the copied raw
  stream and returns before campaign index/header parsing. FileSystem open/copy/
  close implementation remains a boundary; native stdio performs actual reads.
* False mode with a present stream: `__loadOptions` `46d8f4`, 14 tutorial bytes
  `46c778`, getLanguage `46d514`; language -1 selects zero/new flag, otherwise
  rereads getter and sets it. Loaded becomes one. Missing file does not set loaded
  or fabricate a save. Reload does not erase existing loaded/new flags.
* True mode `46c7b8` scans Language and AutoOrientation until both found, then
  **always replaces the hint**: KOREAN_BUILD `9f640b` gives 5, else JAPANESE_BUILD
  `9f640c` gives 4, else `nativeDetectPhoneLang` `531ab0` enum 0..7 maps to
  `[0,2,1,7,3,4,5,6]`; unsigned out-of-range gives zero. These are not SHARP/HTC
  flags. No loaded flag or second setLanguage delivery occurs in this branch.
* `getOption` `46d474` miss is -1. Application saved-option miss is zero.
  `setOption` `46d0d0` ignores misses. `getLanguage` falls back to hint if Language
  is absent; present -1 uses actual `Application.GetDeviceLanguage` `31f75c`,
  whose recovered implementation returns -1.
* `setLanguage` `46d104` first writes an existing Language option. It walks the
  Character list: virtual IsPlayer (+28), otherwise IsMerchant, then actual
  inventory localization for either. It reloads ObjectManager before tree walk:
  nonnull object virtual IsGameObject (+20), type3 item localization, **reload
  type**, and type14 clears +819. Finally TextManager switches pack(lang,true).
  Character.IsPlayer `3a49f0` and IsMerchant `3a30c4` depend on genuine GetType;
  type0 IsPlayer also checks the source name prefix. Do not replace these with
  guessed booleans. Inventory wrapper `3b36e4` tails `3fdfa0`; item wrapper
  `3ebca8` gets its item then calls `3fc1b4`. Those bodies must be supplied.
* `UpdateSavedValues` `43a8f0` reads AutoOrientation then calls ResetOrientation
  `31f748`; the latter is genuinely empty. No fabricated display effect.
* `NativeLoadSettings` `43b2d8` does not write the ActionScript result. The separate
  multiplayer wrapper writes actual set_bool; source reference cleanup stays with
  the real AS provider. Audio SetInitialVolume remains required when sound exists.

## Cache and deterministic format

Canonical ZIP SHA256
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
Members under `com.gameloft.android.GAND.GloftD2SS/files/data/pydata/`:

| File | SHA256 |
|---|---|
| design_pyarray.bin | fd70c8be93cd3d5b7947309e22f00a7c5230a4a7541ec6f9ebfdac67335e6296 |
| design_pyarraynames.bin | d9d5ec686e24d2ee0dcf9f7b627b6679ee522b2df311c6861b7c1bb39ab96c7b |
| design_pystructnames.bin | d955c29a9ddeb091bc5382eff44002a93e6390228bbe0974769648cc58044dc6 |

GameOption is the third table: records byte240/count16; names byte56. Each source
record reads seven signed words Default/Label/Max/Min/Step/Type/ValueStr. Native
32-byte projection adds a zero normalization word for the source vtable identity.
Actual defaults include AutoOrientation1, DPad1, ForwardCam1, Language-1,
VolumeFX100, VolumeMusic100; the ten other options are zero. All 16 actual names
and all fields are in `original-cache.json` and are compared to original readers.
The owned loader structurally skips Design/Difficulty prefixes and validates the
reached GameOption schema; it does not claim full first-table/schema parity.

Settings save bytes are little-endian u32 record count, then u32 key length/exact
key bytes/signed32 value for each, followed by 14 bytes. Unknown keys are ignored;
duplicates last-write; embedded NUL keys use the C-string prefix. Original
`readString` `317734` reads at most127 bytes for capacity128 and returns false
when declared length >=128. It does **not** skip the remainder. Options stop,
then tutorials read at that cursor, potentially inside the long key tail. The
native parser reproduces this behavior on supported complete streams. Truncated
inputs and unsafe bounds get explicit native rejection rather than reproducing
original uninitialized/short-read behavior. Native caps: 16MiB file, 65536 rows,
and 65536 graph steps; these are portability guards, not original game limits.

## Stable integration API and lifetimes

Add four production cpp files to the existing UI target:
`game_option_table_v1.cpp`, `owned_hud_settings_v1.cpp`,
`settings_native_files_v1.cpp`, `settings_language_scene_v1.cpp`.
They use existing `localization.cpp` and `hud_startup_callbacks.cpp`; no external
dependency is introduced. Existing production CMake/facade/app were not edited.

1. Load caller-supplied canonical design streams into GameOptionTableV1. Retain
   its immutable Borrow in OwnedHudSettingsV1. Reload rejects while borrowed;
   failed load is atomic; names/rows and strings remain alive for the owner.
2. Retain SettingsNativeFilesV1(actual private application directory). `services()`
   opens only `dh2_settings.savegame` read-only; genuine ENOENT is missing; other
   errors reject. Found files return a live full-width FILE lease; bytes copy and
   lease closes before parsing/language services. No files are created by runtime.
3. Retain real Localization and SettingsLanguageServicesV1. Its refresh_scene
   must project the actual ObjectManager graph and invoke the graph kernel with
   real required leaf providers. `SettingsLanguageScene24V1` borrows live intrusive
   list/tree nodes and object type/valid-byte pointers. All must survive synchronous
   callbacks. Character `next`, object tree start and post-item type are reread.
   The graph service cannot be replaced with `return true` for unavailable world
   inventory. platform_language is required only for language-only non-forced mode.
4. SettingsStartupBindingV1 owns no contexts. Retain owner/files/language/device,
   actual application/savegame identities and downstream audio/AS result provider.
   Pass `settings_startup_v1_service` to the frozen startup wrapper. Mismatched
   identities, missing inventory/audio/platform/AS providers fail visibly. Native
   host fixtures are explicitly labeled; stable Application/savegame ownership
   during the synchronous owned load is required. Full manager replacement during
   load is outside this owner facade contract. Providers must not throw across a
   noexcept C ABI and must preserve borrowed receiver/node lifetime.

`settings_language_scene_v1` supplies traversal, not inventory ownership. The
fresh empty graph in full-manager gold is a fixture; nonempty graph proofs verify
ordering/live mutation but still use required inventory services. Campaign saves,
world inventory, sound effects, device observation, and language asset packaging
are separate integration obligations; unavailable operations are rejected.

## Proof and reproducible isolated audits

`owned-hud-settings-v1-arm64-differential.json`: 784 exact original/O2 cases
(272 record,512 parser), with 254 original complete-manager gold cases. Native
map ownership is host-tested, not falsely labeled ARM64 STL instruction parity.
`settings-language-scene-v1-arm64-differential.json`:162 original/O2 graph cases.
Both execute the same optimized complete additive ARM64 DSO, including 64-bit
callback context identities. Separate complete-batch Android x86_64 build passes.

`owned-hud-settings-v1-host-audit.json`:1038 owner/parser/record comparisons,
259 additional checks,12 atomic guards,4 failure prefixes,1 synchronous mutation;
plus162 graph comparisons/3 failure-live-mutation checks/3 guards. Real stdio
missing/present files and real Localization cache owner execute; inventory graph
and platform values are labeled fixtures. ASan/UBSan/LSan findings zero.

Host targets can be named `owned_settings_v1_audit` and
`settings_language_scene_v1_audit`. Link the existing UI DSO (or isolated sources)
with sanitizer flags. Arguments for the first are:
`reference/owned-hud-settings-v1/fixtures.bin`, actual design directory,
actual common_text directory, and an empty private test directory. The test
refuses to overwrite an existing source save and cleans its generated fixture.
The second takes `reference/owned-hud-settings-v1/language-scene-fixtures.bin`.
See `build-commands.json` for exact isolated commands and binary/source receipts.
No packaged or live Android proof is claimed by this batch.
