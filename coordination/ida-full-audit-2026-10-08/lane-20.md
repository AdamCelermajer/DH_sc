# Lane 20 — Android DEX and Samsung native interface

Read-only working-tree audit dated 2026-10-08. Baseline marker: `75a7c2fe3261403e841ffbeb46734e9e4e84e75c`; the pre-existing dirty tree is the source under review. Only this report and `lane-20.csv` were written. No source edits, builds, tests, emulator, installs, or external messages were performed. Coordination messages to the parent audit worker were used.

**Inventory coverage is complete for this lane; source semantic closure is not complete.** The CSV contains exactly 2,360 unique assigned rows: all `libnativeinterface.so` inventory indices 0–49 and all `classes.dex` inventory indices 0–2309, with no gaps or duplicates. The unresolved rows are explicit and must remain on the overall audit's unresolved list. These are inventory indices, not the distinct DEX `method_id` numbers printed in full-listing comments.

| Input | Records | Exported pseudocode | Assembly disposition |
|---|---:|---|---|
| libnativeinterface.so | 50 | 32 success, 18 failed | Every record read; 18 failures are external import records; one successful decompilation has a wrong instruction mode |
| classes.dex | 2,310 | All 2,310 unsupported by native Hex-Rays | Every exported body traversed: 62,698 instruction lines, 15,047 invoke instructions, 5,566 conditional/goto/switch transfers, 147 opcode kinds |

| Comparison status | Native interface | DEX | Total |
|---|---:|---:|---:|
| matched | 0 | 2 | 2 |
| partial | 2 | 78 | 80 |
| disconnected | 0 | 19 | 19 |
| missing | 0 | 5 | 5 |
| unclear | 12 | 2,007 | 2,019 |
| import/thunk | 35 | 0 | 35 |
| clone | 0 | 199 | 199 |
| failed-body | 1 | 0 | 1 |
| **Total** | **50** | **2,310** | **2,360** |

Every row has `integrated_runtime_acceptance=not_run`. Existing `port/engine-ui/reports/android-music-support-v1-dex-proof.json` reports 78 original DEX interpreter cases; its stated scope explicitly excludes a native differential claim. It is supporting historical evidence for one original method, not acceptance of the current Java/JNI application.

The mechanical body pass records each method's byte hash, exact assembly-file line, opcode counts, control-flow count, field references, full resolved string references, unique callees, and incoming start-address xrefs. This pass makes every record reviewable; it does not establish every branch's semantic equivalence. Deep comparisons concentrate on the live Activity/JNI, settings, resource-prefix, input/renderer, front audio/movie, catalog/browser, and DRM paths below. Unresolved billing, installer, Bluetooth, account and restoration bodies retain `unclear` rather than receiving a blanket parity or scope waiver.

## Findings and traced behavior

### F20-01 — Reached platform locale differs for Chinese, Russian and Korean

**Disposition: partial; high confidence in the static difference; runtime not run.** Original `GLMediaPlayer.detectPhoneLang`, DEX #119 at `0x35a0c`, returns `eng=0`, `fra=2`, `deu=1`, `ita=4`, `spa=3`, `jpn=5`, `zho` with ISO3 country `CHN=7`, and otherwise 0. French and German values match the current reached callback. Original Korean locale alone reaches the default; the native Korean-build branch is separate.

The actual original Settings edge is `SavegameManager::__loadLanguageAndOrientation` (DH2 #7763, `0x46c7b8`) → `nativeDetectPhoneLang` (DH2 #12394, `0x531ab0`) → `GLMediaPlayer.detectPhoneLang` (#119). It is **not** `DungeonHunter2.Get_PhoneLanguage` (#25, `0x32d6c`), whose raw French/German codes differ. Those methods must not be conflated.

Current `MainActivity.java:421` returns `ru=7`, `ko=6`, and returns 8 for Chinese. `android_intro_movie_v119.cpp:30` calls this exact live Activity method with JNI `CallIntMethod`. `native_process_startup_v119.inc:251` supplies that callback to the retained Settings owner. `owned_hud_settings_v1.cpp:88` applies `{0,2,1,7,3,4,5,6}` directly, with no intervening remap. Thus an ordinary Chinese mainland locale selects the default 0 instead of the original 6 result; Russian selects 6 instead of the original default; Korean selects 5 outside the original Korean-build condition. Saved language is scanned but this language-only branch applies the platform result afterward.

Source hashes are recorded below and in the CSV. The exact original bodies are in `libraries/classes.dex/assembly-functions.asm` at the named addresses and `libraries/libDungeonHunter2.so/pseudocode/0046/0046c7b8.c`, `pseudocode/0053/00531ab0.c`.

### F20-02 — Catalog and browser producers lack platform consumers; GLive owner explicitly unavailable

**Disposition: disconnected for More Games/browser, missing reached GLive delivery; high static confidence; runtime not run.** DEX #31 `OpenIGP` (`0x3308c`) → #48 `launchIGP` (`0x334e4`) constructs an Intent for `IGPActivity` and starts it with a language extra. DEX #50 `openBrowser` (`0x33574`) constructs `ACTION_VIEW`, parses the supplied URL, and starts an Activity. DEX #30 `OpenGLive` → #47 `launchGLLive` creates the original account Activity.

Current `front_ui_session_v87.cpp:236` enqueues browser URLs; line 248 enqueues catalog language requests. The event-provider binding at line 255 connects these producers to authored native events. `consume_menu_browser` at line 1650 and `consume_menu_catalog` at line 1654 have declarations and definitions but no callers in the active `port` source search. `MainActivity.java:169` drains audio/effect requests and front-world launch requests; `NativeBridge.java` exposes those methods but no browser/catalog consumer. Current `AndroidManifest.xml:15` declares only `.MainActivity`.

`OriginalCatalogActivity.java:28` and `OriginalCatalogUrl.java:25` contain actual WebView/navigation and URL construction code, including the original redirect host, `from=D2SS`, version `1.0.2`, encrypted UDID, device/firmware/country/height fields. This implementation exists but has no queue-to-Activity launch edge and no manifest declaration. It must not be called a missing source implementation or counted as integrated just because its class exists. The original IGP methods #455–476 also contain serial/device fallback, activity state and error branches that have not all been established in the replacement.

For GLive, the current reached `menu_online_request(live=true)` explicitly returns failure with `Required Gameloft Live account Activity owner unavailable`. The absence of delivery is therefore supported by an explicit failing provider, not only by a class-name search.

The empty original `NativeLaunchIGP` AS callback is correctly represented as empty at `front_ui_session_v87.cpp:898`; the actual platform action belongs to the native menu event on release. That empty callback does not excuse the missing consumer after the event queues its action. The same producer/consumer break applies to URLs queued by browser-native events.

### F20-03 — Active intro playback omits original localized timed captions

**Disposition: missing caption behavior, partial general movie delivery; high static confidence; runtime not run.** DEX #513 `MyVideoView._clinit_` (`0x475a0`) creates the localized caption table. #525 `onCreate` (`0x4846c`) creates the caption TextView from the layout, clears it, and sets its size. #521 starts playback and posts a Handler message. #634 `bc.handleMessage` (`0x4da78`) calls #516 `MyVideoView.access$000` (`0x479ec`) and, while it returns 1, schedules the next message after `0x64` = 100 ms. #516 compares actual `VideoView.getCurrentPosition()` to caption time windows and calls `TextView.setText` from the language-indexed table. #524 `MyVideoView.e` (`0x4801c`) has the identical code hash `80cf7db42676c260151e0f04228d22d7abe37f30ffb9ad1dd2646215731f2e35` and is recorded as a clone of #516, whose caption behavior is missing.

The live Java owner is constructed by `MainActivity.java:215` and bound to JNI at line 216. Current native `android_intro_movie_v119.cpp:18` chooses `intro_jp.mp4`, `intro_kr.mp4` or `intro.mp4` and calls the bound `MainActivity.startSourceIntroV119`. That Java method at line 415 discards its language argument and calls `IntroMovieV119.show(generation, filename)`. `IntroMovieV119.java:32` creates only a SurfaceView and Skip button for the movie, then owns actual prepared/completion/error/skip events. It has no caption TextView, localized caption table, position-window selector or 100 ms caption callback. The separate `CinematicActivity.java:32` TextView is an error label, and that Activity has no current manifest or launch edge.

Real movie completion and generation ownership are present in source; they are not inferred from elapsed time. This supports `partial` for general movie delivery while keeping the specific caption gap `missing`. Retained movie-position behavior, skip-button timing, device-specific pause/resume and original Activity restoration remain unverified.

### F20-04 — Conditional positive campaign license validation is unimplemented; Samsung closure unresolved

**Disposition: explicit partial/failing source adapter; high static confidence when the branch is true; runtime not run.** Original DEX #563 `Zirconia_DRM.onCreate` (`0x4bc84`) constructs the license owner and listener. #1784 `Zirconia.checkLicense` schedules the CheckerRunnable. #1768 runs local/phase-two checks and may retrieve a license. #1766 calls nativeinterface #26 `checkLicenseFile` (`0xe2c`) → #25 `CheckLicenseFile` (`0xd20`); #1767 calls nativeinterface #18 `checkLicenseFile2` (`0x920`), which is literally constant true. Network-response DEX #1753 invokes nativeinterface #21 `storeLicenseKey` (`0x9d0`). DEX #1757 loads `nativeinterface`.

Current `NativeBridge.java:5` loads `dh2_native`, `CMakeLists.txt:49` builds the source reconstruction, and the current manifest has no Samsung license Activity root. The individual Samsung method/source counterparts remain `unclear`; licensing absence is not established merely from old JNI names disappearing.

There is stronger evidence for the reached source campaign path: `source_campaign_runtime_v61.cpp:799` checks the actual `USE_NATIVE_DRM_GAME` condition and calls `validate_native_campaign_license_v93(world,1,error)` when true. `native_menu_resources_v93.inc:284` borrows the native menu manager, sets `Required actual ALicenseCheck_ValidateLicense(1) positive source branch`, and always returns false. A positive DRM path cannot complete through that adapter. This is conditional; the finding does not assert that every present gameplay path enables DRM.

The native library's other helpers were read: 20-byte digest formatting, truncated passphrase/hash-derived table slicing, two 20-byte license-file writes, 20-byte read/compare, JNI UTF/array lease release, and SHA1 initialization/buffering/compression/padding/result. Exact current source counterparts are unestablished. Old pseudocode's JNI release arity and the word-serializer instruction mode are unreliable; those contracts need assembly/ABI resolution before implementation.

## Proven local matches and bounded partial comparisons

DEX #45 `DungeonHunter2.isSupportMM` (`0x33380`) initializes v3 to -1; every original manufacturer/model branch returns that same register. Current `NativeBridge.java:16` returns -1. `native_app.cpp:249` resolves that method on the current retained bridge class, and `MusicPlatformV1::query` calls it. This is a local semantic match with an actual source connection, not general media-player parity.

DEX #120 `GLMediaPlayer.getSDFolder` (`0x35ad0`) is one string constant and return. Its referenced string `aSdcardAndroidD_0` resolves at DEX address `0x18ce1` to `/sdcard/Android/data/com.gameloft.android.GAND.GloftD2SS/files/`. Original DH2 #12395 `nativeGetSdFolderPath` (`0x531af8`) calls it, obtains UTF bytes, allocates 1024 bytes, copies the string and releases the UTF lease. Current `process_resource_prefix_source_v95.hpp:14` reproduces that value and independent process byte ownership; `native_app.cpp:222` initializes it on the live source Application. This local match does not establish all GLResLoader stream methods. Some old source comments cite another address or DEX method number; the supplied inventory/address pair above is authoritative for this report.

The current manufacturer/model predicates preserve case-sensitive `SHARP`, `HTC`, and `SHW-M130L` checks used by menu selection (`front_ui_session_v87.cpp:1795`). DEX #26–27 also return other numeric codes. Their other consumers are not established, so they remain `partial`.

DEX #242–245 catalog cryptography was compared to `OriginalCatalogUrl.java:14`: space padding to the next 16-byte boundary only when the remainder is nonzero, the exact delta table at original `arraydata_38C88` in `full-listing.asm:62847`, and the 24 encoded key bytes match the local reconstruction. AES/ECB/PKCS5Padding intent is present, but the key's algorithm label, encoding, null/default input and exception behavior differ. The catalog owner is also disconnected. These rows are not marked fully matched.

Original surface callbacks #449–452 reach native environment/config/init, resize and app update through DH2 exports. Current MainActivity renderer → NativeBridge → `native_app.cpp` is connected. Full EGL config selection, original key-buffer draining, IGM render suppression, context-loss globals and every ordering branch remain unverified. DEX #446 sends integer coordinates, original primary-down pointer id 0, a long pointer id for other paths and original phase codes. DH2 #12437 replaces the Java time with `gettimeofday`, sets the native argument flag and honors `Touch_Hack_int`. Current Java queues pointer events to the GL owner and sends float coordinates through `authoredHudTouch` / `dispatch_source_app_touch_v121`. These paths have meaningful implementation but are `partial` without ABI, coordinate/time/order and state comparison closure.

GLMediaPlayer #114–151 was reviewed for sound-slot arrays, SoundPool vectors, generalized media indexing, ringer/master-mute, track resetting and loop/stop/release behavior. Current `FrontAudio` and native audio ownership implement the reached menu subset. Original generic slot APIs and Musicplayer/MediaPlayList device-library contracts are not proven equivalent. GLResLoader #152–168 reads raw/drawable/asset/file paths through a static stream owner. Current APK archive/cache and generation-owned staged intro copy are partial replacements, not proof of all original read/length/skip/close semantics.

## Import, clone and failed-body dispositions

Native interface #0 is the PLT resolver, #1–16 are GOT-dispatch import thunks, and #32–49 are libc/unwind external import records. All 35 are `import/thunk`. The 18 special-segment failures are therefore completely accounted for. Their shared all-zero placeholder hash `df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119` does not make different imported symbols semantic clones.

Native interface #17 `start` (`0x8e4`) is exported as a successful pseudocode body but its ARM listing interprets Thumb pairs as nonsensical conditional ARM instructions. The byte sequence starts with halfwords `4a0d b082 9101 447a 6812`, and later loads/stores individual bytes with two endian-dependent order branches. Calls from #19 serialize five SHA1 words to successive four-byte output positions. The manual Thumb interpretation indicates a four-byte word serializer, not a trustworthy game entrypoint; exact decoding/literal-endian/ABI confirmation remains unresolved. This row is `failed-body` despite exporter status `success`, with the distinction recorded in its evidence.

DEX has 2,111 distinct `code_sha256` values and 199 additional exact-byte clone rows. Every clone points to its canonical inventory index. Major groups include 71 identical Object constructors, 38 empty returns, and 28 null-return stubs. Canonical source semantics may remain `unclear` or `missing`; clone identity is not source acceptance or owner-identity proof. Exact #516/#524 caption-body duplication is retained explicitly. The 2,310 DEX decompiler failures do not imply absent bodies: assembly is present for all and each has a comparison disposition.

## Unresolved source families and acceptance limits

The following cannot be established from this pass and are not silently waived:

- The 2,007 `unclear` DEX rows and 12 `unclear` native interface rows require individual source/branch/owner comparison beyond the recorded body characterization. The 199 clone rows inherit the canonical row's unresolved semantic limits. Only the two bounded DEX methods above have full local matches.
- Bluetooth #67–113, remote billing/IAB #5–20/#533–551/#647–983, GLive #371–427 and its obfuscated listeners, KDDI/AccountManager/Binder methods #1438–1732, and Samsung Plasma #1794–2256 contain real protocol/UI/state machines. No current exact acceptance or equivalent platform owner was established. Their native/Java framework imports are retained as call evidence. They are not all declared out of scope or missing from a name search.
- Installer #984–1437 contains download/validation/version/CRC/MD5/storage/network/preferences branches. Current bundled-cache mounting is not semantic proof of the original downloader/repair/install UI. Regional/configuration reachability and ownership need a separate comparison.
- Supplied APK additions include the `android.support.v4.app.app` preference/promotional routine and `com.savegame.SavesRestoringPortable` #2257–2309. DEX #563 calls #2260, which calls #2275 `SmartDataRestoreForYou`, then #2300 `unZipIt`. Decoded helper-built constants reveal `data.save`, `extobb.save`, `extdata.save`, `savegame/notfirst`, and AES/CBC/PKCS5Padding restoration to internal/OBB/external-data paths. This is real supplied baseline behavior, not ordinary current settings-file I/O. Whether these added archive-restoration behaviors belong to the intended source acceptance baseline is unresolved; they remain covered and `unclear`.
- Three called native signatures have no exact named export match among the supplied native inventories: `GLBluetooth.nativeInit`, `InAppBilling.nativeSendData`, and `InAppBilling.nativeInit`. Dynamic registration, load failures and dormant/mismatched ABI paths cannot be resolved from a named-export miss. Each has an explicit unresolved edge below and in its CSV row.
- Outgoing invoke targets are statically named targets, not proof of every virtual/interface dispatch receiver. Incoming CSV xrefs are exported start-address references; absent direct xrefs do not prove unreachable methods. DEX table payloads outside body ranges were consulted for the catalog key, and string references were resolved; remaining switch/fill-array/exception-table and indirect-dispatch contracts need semantic validation.
- No integrated runtime receipt for the current hashes was established. This report is not gameplay, Activity-recreation, catalog network, movie subtitle, billing, installer or DRM runtime acceptance.

## Coverage and cross-boundary tables

The generated tables and source-hash appendix follow. All IDA references use `.local-inputs/ida-apk-export-2026-10-07/libraries/`. Cross-library records cited here are context edges only and are not duplicate coverage rows in `lane-20.csv`.

| DEX body family | Inventory indices | Methods | Instructions | Invokes | Control transfers |
|---|---|---:|---:|---:|---:|
| bootstrap/billing | 0–20 | 21 | 360 | 69 | 38 |
| Activity | 21–66 | 46 | 864 | 202 | 134 |
| Bluetooth | 67–113 | 47 | 1516 | 414 | 147 |
| media | 114–151 | 38 | 1035 | 156 | 182 |
| resource | 152–168 | 17 | 634 | 165 | 85 |
| utilities | 169–370 | 202 | 3938 | 910 | 474 |
| GLive | 371–427 | 57 | 8799 | 1617 | 440 |
| EGL/renderer | 428–452 | 25 | 681 | 98 | 45 |
| catalog | 453–476 | 24 | 719 | 164 | 51 |
| playlist/movie | 477–532 | 56 | 1599 | 272 | 150 |
| DRM/listeners | 533–646 | 114 | 4831 | 770 | 241 |
| billing/IAB | 647–983 | 337 | 6013 | 1552 | 692 |
| installer | 984–1437 | 454 | 14272 | 3453 | 1653 |
| KDDI/accounts | 1438–1732 | 295 | 5933 | 1303 | 556 |
| Zirconia | 1733–1793 | 61 | 784 | 207 | 61 |
| Samsung Plasma | 1794–2256 | 463 | 7593 | 2257 | 594 |
| save restoration | 2257–2309 | 53 | 3127 | 1438 | 23 |

The Dalvik call scan finds **40 imported native invoke sites across 31 distinct signatures**. Twenty-eight signatures have an exact named JNI export candidate in the supplied native function inventories; three remain unresolved. Name correspondence is not independent ABI/runtime proof. Current-source dispositions below are limited to the caller rows and the detailed paths above.

| Imported native signature | DEX callers | Supplied JNI function record | Current disposition |
|---|---|---|---|
| `DungeonHunter2.nativeSetPhone` | #55 | libDungeonHunter2.so #12417 `0x532e78` | partial; not_run |
| `DungeonHunter2.nativegetState` | #58 | libDungeonHunter2.so #12441 `0x533558` | partial; not_run |
| `DungeonHunter2.nativeCanInterrupt` | #59 | libDungeonHunter2.so #12418 `0x532ea8` | partial; not_run |
| `DungeonHunter2.nativeGetGameMusicVolume` | #61 | libDungeonHunter2.so #12440 `0x533554` | partial; not_run |
| `DungeonHunter2.nativeAccelerometer` | #62 | libDungeonHunter2.so #12419 `0x532ec8` | unclear; not_run |
| `DungeonHunter2.nativeSetOrientation` | #62 | libDungeonHunter2.so #12439 `0x533500` | unclear; not_run |
| `DungeonHunter2.nativeonTrackballEvent` | #65 | libDungeonHunter2.so #12435 `0x533430` | unclear; not_run |
| `GLBluetooth.nativeInit` | #76 | **Unresolved**: registration/load/ABI not established | unclear; not_run |
| `GLMediaPlayer.nativeInit` | #122 | libDungeonHunter2.so #12397 `0x531ca4` | partial; not_run |
| `GLMediaPlayer.nativeGetTotalSounds` | #122 | libDungeonHunter2.so #12364 `0x531214` | partial; not_run |
| `GLMediaPlayer.nativeGetTotalSoundsOfSameInstance` | #122 | libDungeonHunter2.so #12365 `0x53121c` | partial; not_run |
| `GLResLoader.nativeInit` | #168 | libDungeonHunter2.so #12402 `0x5324c0` | partial; not_run |
| `GameGLSurfaceView.nativeOnTouch` | #446 | libDungeonHunter2.so #12437 `0x533440` | partial; not_run |
| `DungeonHunter2.nativePause` | #447 | libDungeonHunter2.so #12433 `0x533424` | partial; not_run |
| `DungeonHunter2.nativeResume` | #447 | libDungeonHunter2.so #12438 `0x5334dc` | partial; not_run |
| `GameRenderer.nativeGameRenderer` | #449 | libDungeonHunter2.so #12355 `0x531150` | partial; not_run |
| `GameRenderer.nativeConfig` | #449 | libDungeonHunter2.so #12356 `0x531154` | partial; not_run |
| `DungeonHunter2.nativeKeyDown` | #450 | libDungeonHunter2.so #12436 `0x533438` | partial; not_run |
| `DungeonHunter2.nativeKeyUp` | #450 | libDungeonHunter2.so #12434 `0x533428` | partial; not_run |
| `GameRenderer.nativeRender` | #450 | libDungeonHunter2.so #12362 `0x5311a0` | partial; not_run |
| `GameRenderer.nativeOnSurfaceChanged` | #451 | libDungeonHunter2.so #12360 `0x531190` | partial; not_run |
| `GameRenderer.nativeGetJNIEnv` | #452 | libDungeonHunter2.so #12358 `0x53115c` | partial; not_run |
| `DungeonHunter2.nativeInit` | #452 | libDungeonHunter2.so #12403 `0x5325c4` | partial; not_run |
| `GameRenderer.nativeInit` | #452 | libDungeonHunter2.so #12363 `0x5311c8` | partial; not_run |
| `Musicplayer.nativeInitplayer` | #507 | libDungeonHunter2.so #12445 `0x533658` | unclear; not_run |
| `Musicplayer.nativeDisplayMusicTitle` | #508, #621 | libDungeonHunter2.so #12455 `0x533b2c` | unclear; not_run |
| `InAppBilling.nativeSendData` | #813 | **Unresolved**: registration/load/ABI not established | unclear; not_run |
| `InAppBilling.nativeInit` | #823 | **Unresolved**: registration/load/ABI not established | unclear; not_run |
| `NativeInterface.storeLicenseKey` | #1753 | libnativeinterface.so #21 `0x9d0` | unclear; not_run |
| `NativeInterface.checkLicenseFile` | #1766 | libnativeinterface.so #26 `0xe2c` | unclear; not_run |
| `NativeInterface.checkLicenseFile2` | #1767 | libnativeinterface.so #18 `0x920` | unclear; not_run |

Reverse callbacks deeply traced in this lane include original DH2 #12394 nativeDetectPhoneLang → DEX #119, DH2 #12395 nativeGetSdFolderPath → DEX #120, source JNI music query → current NativeBridge.isSupportMM, and source native movie request → live MainActivity.startSourceIntroV119 → IntroMovieV119 → NativeBridge.introMovieEventV119 → retained native generation state. The native-menu event → catalog/browser queue → Activity gap is explicitly disconnected. Other reverse JNI resource/media/phone/network callbacks require the neighboring native audit and are not claimed closed by this table.

## Source hashes and source-change check

All cited current source files are pinned below. Fourteen had an earlier hash capture during detailed review. Thirteen retained that hash; **front_ui_session_v87.cpp changed during review** from `f88ceb1ed519625d059b20fefdffabf78ff65282d1b707760d8f0adb42b8b3ae` to `3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638`. The changed file's catalog/browser producers, consumer definitions, explicit unavailable GLive owner, and device predicates were re-read, and the relevant scoped call searches were repeated. Those findings remain supported; other modified regions were not re-audited. All final CSV-cited hashes were checked again after this change, and affected rows carry `source_changed_during_review=true`. Files first captured later are a final read snapshot, not a promise of continuous filesystem monitoring.

| Current source file | SHA256 |
|---|---|
| `port/android-native/app/src/main/AndroidManifest.xml` | `8eac3950e5f0b39ef7ff943b324ed4a84808bc0f2a0b6fc968e5cfce352415ab` |
| `port/android-native/app/src/main/cpp/CMakeLists.txt` | `ee593e57786a13b37a90364667da0507d1ca4e92d56adc0dccac02b05217da45` |
| `port/android-native/app/src/main/cpp/android_intro_movie_v119.cpp` | `8f6e6a5dcf772b96fe874c6c2af0e2c77313c1aa71632b45dcafdbe686e42c0c` |
| `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp` | `3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638` |
| `port/android-native/app/src/main/cpp/native_app.cpp` | `a24f348fafea17e488bf5b3cdf97f832651818f45e78077a59b3a6f5407ddef3` |
| `port/android-native/app/src/main/cpp/native_menu_resources_v93.inc` | `c4ed613435541f69ef9d668c926e4713ae3e62dcd757888a90529d200581a57e` |
| `port/android-native/app/src/main/cpp/native_process_startup_v119.inc` | `256c0b9ec77a720ad60d507309727428337aeb44d72fd1fba6b689cd9598cffa` |
| `port/android-native/app/src/main/cpp/original_cache_assets_v1.cpp` | `6a4eeef2116e4da7555eca168f663b272a8ac4cf53aeabae15f033717b385bdb` |
| `port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp` | `6b20e56262c4051cc5f35b61b6c7a7fd6b7fc9b75765f7825666326a90a5a38e` |
| `port/android-native/app/src/main/java/com/example/dh2/CinematicActivity.java` | `60964f1101c3da30afdea48dfe79f3359db70dc2702c5cb924e5dcbd48d1b508` |
| `port/android-native/app/src/main/java/com/example/dh2/FrontAudio.java` | `7df11999b6184d8838514704f349be9d8ad03db174fd85d0a2a76084c435b30f` |
| `port/android-native/app/src/main/java/com/example/dh2/IntroMovieV119.java` | `70431283d42b882d4958e32e86d16ecb63045d659f82fb7293090c08ee2117d0` |
| `port/android-native/app/src/main/java/com/example/dh2/MainActivity.java` | `c544e0550f100b68d55a5661cde8c1fee67105f17b95dabc6eb7276968d45001` |
| `port/android-native/app/src/main/java/com/example/dh2/NativeBridge.java` | `02e95bd3476dfb64cab894089caf47c4180fc61dc18a3953d3dfcb69fbdb0109` |
| `port/android-native/app/src/main/java/com/example/dh2/OriginalCatalogActivity.java` | `1a146c9b3fb06bc955ece482628bde211e079307a7f5d3d7595c9859a0a5bbda` |
| `port/android-native/app/src/main/java/com/example/dh2/OriginalCatalogUrl.java` | `7c44a3bd6b8daafb0cd1be3e21334729bfa7e7267a47b44a6bad577c131f541f` |
| `port/android-native/app/src/main/java/com/example/dh2/VsyncSurfaceViewV44.java` | `ddedd27c1f4ad9a70c67c084431dd5c92884d9cf997c1832d1b962a23ad49990` |
| `port/engine-ui/owned_hud_settings_v1.cpp` | `fec0cf75356b8c45883bfbb829af2ca7e17c6478437bdcac7edb7a53adb16eef` |
| `port/level-world/process_resource_prefix_source_v95.hpp` | `09cc3dc509af1a42b78f633b6f9d00186ddaf18a8bf09b14373972f71fe593ec` |

Exact inventory coverage and source parity limits are separate: the 2,360-row count is closed; the 2,019 unclear rows, 80 partial rows, 19 disconnected rows, 5 missing rows, one unreliable native body and inherited clone limits keep lane semantic/runtime acceptance open.
