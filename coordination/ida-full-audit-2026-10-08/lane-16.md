# Lane 16 — libDungeonHunter2.so records 27992–29857

Read-only working-tree audit at the supplied baseline marker HEAD 75a7c2fe3261403e841ffbeb46734e9e4e84e75c. This worker wrote only lane-16.md and lane-16.csv. No game source edit, build, test, emulator, external message or gameplay run was performed. All integrated runtime acceptance fields are **not_run**.

## Coverage and disposition

[lane-16.csv](lane-16.csv) contains **1,866 unique records**, exactly **27992–29857 inclusive**: zero missing or duplicate function IDs in this partition. All 1,866 have successful IDA pseudocode. There are **0 failed decompilations and 0 external-import records** in the range. Every body was read and its normalized pseudocode SHA-256 checked against the inventory; there were zero hash mismatches.

The assembly scan covered 84,540 address-bearing listing rows. It distinguished 48 empty original bodies, 104 subsequent exact code-hash duplicate records, ten one-instruction branch thunks and 14 compiler clone-name records. A compiler clone suffix alone did not establish body equality. Identical bodies have separate CSV records and identify their representative/code hash.

| Comparison status | Rows |
|---|---:|
| matched | 27 |
| partial | 1,003 |
| disconnected | 474 |
| unclear | 248 |
| clone | 104 |
| import/thunk | 10 |
| missing | 0 |
| failed-body | 0 |
| out-of-scope | 0 |

| Subsystem | Assigned IDs | Records |
|---|---|---:|
| GLX user, socket, RSS/slim, packet/session | 27992–28391 | 400 |
| Lua C API, libraries, compiler, VM, GC | 28392–28862 | 471 |
| Time helper and Vox audio | 28863–29857 | 995 |

The CSV records the individual body path/hash, branches/loops/literals/returns, exported reference examples, current hashed source location or scoped reason no counterpart was established. All 471 Lua records resolve to an exact source definition, including luaI_openlib aliasing and parenthesized API names. Vox locations often identify a **partial subsystem counterpart**, rather than an identically named implementation.

This is full partition accounting with bounded semantic review. It does **not** establish complete parity for every nontrivial body: generic APIs, indirect edges and generalized ownership/error/ABI behavior explicitly remain partial or unclear. The matched rows are restricted to the stated reviewed effects and valid-input domain. Static evidence is not integrated gameplay acceptance.

Reference accounting includes 4,282 non-self incoming code-reference sites, 7,715 outgoing function-target reference sites, and 67 incoming data/address-taken sites. Fall-through type21 was excluded. CSV examples are bounded; the original xrefs.jsonl remains the full evidence. Zero exported callers is not evidence of a dead function: vtables, global pointers and indirect calls are incompletely represented.

## Findings

### F16-01 — Lua uses a different random generator and changes shared-stream effects

**High confidence; partial.** IDA #28642 math_randomseed at 0x854058 calls srand48 at instruction 0x854064. #28646 math_random at 0x854180 calls lrand48 at 0x854188, applies modulo 0x7fffffff, converts to float and multiplies by float constant bits 0x30000000. Its zero-, one- and two-argument interval branches then return float32 values.

The active [lmathlib.c](../../port/script-runtime/lua/lmathlib.c) definition at line181 uses rand()/RAND_MAX at line184, while randomseed at line209 calls srand at line210. [script-runtime/CMakeLists.txt](../../port/script-runtime/CMakeLists.txt):4–10 compiles lua/lmathlib.c. Scoped searches of active port C/C++/headers found no rand/srand macro or function override that routes these calls to rand48. The VM opens math in [script_runtime.c](../../port/script-runtime/script_runtime.c):103–108; runtime_return_v3 includes return_v1, which includes that runtime.

Original Vox event #29831 calls lrand48 at 0x88bbc8 and 0x88bc18. The current [renderer_campaign_audio_services_v68.inc](../../port/android-native/app/src/main/cpp/renderer_campaign_audio_services_v68.inc):18–23 deliberately uses the process lrand48 stream; other native source paths also use it. Original Lua random/randomseed advance or seed that same stream, whereas reconstructed Lua uses another generator. Sequence and interleaving parity therefore differ. Authored-script occurrence frequency and integrated gameplay consequences were not established.

### F16-02 — Registered online callbacks have no current dispatcher implementation

**High confidence for endpoint disconnection.** Cross-lane original bodies were read:

- #6890 0x43a0e0 NativeGLXPlayerLoggedIn reads OnlineGameState+38 into the AS result.
- #7040 0x43fdbc NativeConnectToServer validates a room index and sets OnlineGameState+48 and +20=103.
- #7048 0x440d3c NativeGLXPlayerSignin gates on the live online state, copies credentials, creates CSignInCredentials and calls CSignIn through its virtual entry.

The original handlers lead into earlier online-manager/sign-in ranges. This lane includes GLX user requests/responses and socket/session/packet machinery: #28033 OnUpdateSuccess dispatches response IDs and forwards to GLXPlayerWebComponent; #28223 Connection::receiveDataLen reads a one-byte frame size; #28272 ConnectionLobby reads a two-byte big-endian frame size and rejects lengths greater than 4096. Those are real bodies, not imports.

Current [native_process_menu_actions_v119.inc](../../port/android-native/app/src/main/cpp/native_process_menu_actions_v119.inc):21,130,132 registers the names. [native_process_startup_v119.inc](../../port/android-native/app/src/main/cpp/native_process_startup_v119.inc):226 attaches them to movies. The actual dispatcher in [front_ui_session_v87.cpp](../../port/android-native/app/src/main/cpp/front_ui_session_v87.cpp):817 has no branches for these names, and falls through to **Unknown owned menu native action** at line1012. [swf_actionscript_connection.cpp](../../port/engine-ui/swf_actionscript_connection.cpp):34–43 propagates provider failure into the retained AS graph.

NativeOnlineSanityCheck is implemented at front_ui_session_v87.cpp:841, but clears a process selector byte; it does not implement authentication or transport. Scoped source searches covered active port C/C++/headers and the concrete AS registry/provider/dispatcher. GLX rows marked disconnected describe this endpoint path; they do not assert that every helper is required by offline gameplay. Remote service availability and complete original virtual online closure remain unresolved.

RSS/slim/Unicode records remain unclear. [xml_document_v1.cpp](../../port/level-loader/xml_document_v1.cpp):9 uses TiXmlDocument for level-resource consumers. That existing parser does not establish a replacement for the original RSS feed/item ownership or encoding behavior.

### F16-03 — Original event lookup ignores case; source lookup does not

**High confidence; partial.** IDA #29788 at 0x88a3f4 VoxSoundPackXML::GetEventUid loops event rows and calls LC_API_STRCASECMP_0. Current [audio_catalog_v34.cpp](../../port/engine-audio/audio_catalog_v34.cpp):61 uses a normal std::map<string,int>::find, so differently cased labels do not match.

The current application initialization in [audio_application_manager_v42.cpp](../../port/engine-audio/integration-v42/audio_application_manager_v42.cpp):129 requests exact-case preload_sfx, which is unaffected. Numeric-event gameplay consumers bypass this label lookup. Mixed-case authored-call reachability is not established, so this is a generalized parity difference rather than a proved selected-asset failure.

### F16-04 — Soundpack parser accepts a different and narrower domain

**High confidence; partial.** The complete #29854 0x88d344 VoxSoundPackXML::LoadXML body was read through row allocation, TiXml traversal, optional fields and custom-parameter ownership.

Original event params are tokenized on space/semicolon: first token sets history, clamped to source-list length; an optional second token sets probability capped at 100. Current [audio_catalog_v34.cpp](../../port/engine-audio/audio_catalog_v34.cpp):46 parses params as one strict integer and takes probability from a separate attribute. Original low-priority bank spelling is **steal lowest priority**, while source line44 recognizes **steal low. prio.** Original optional ref/max distance, rolloff, gain/pitch modulation, custom parameters and automatic extension fields also exceed the retained row shape in [audio_catalog_v34.hpp](../../port/engine-audio/audio_catalog_v34.hpp):8.

The supplied [sounds.xml](../../port/level-world/reference/audio-source-v34/sounds.xml) contains 578 sound rows: 460 pcm, 38 adpcm, 80 vxn ; 12 groups, 7 banks and 63 events. Its params values are single integers 1/2/24. Bank policies are do nothing, steal oldest, or steal low. prio. or old. same prio. There are no ogg/mpc-format rows or optional custom/modulation/refdistance/basegain rows. These domain differences do not prove this selected pack fails.

[audio_gameplay_runtime_v42.cpp](../../port/engine-audio/audio_gameplay_runtime_v42.cpp):6–13 actually reads data/sounds/sounds.xml, applies this parser, validates binding UID targets and configures banks. Asset path and UID relationships were traced statically. No parser execution was performed.

### F16-05 — Native music cursor, playlist and transition behavior remains partial

**High confidence for explicit source limits.** Original #29386 DecoderNativeCursor::ParseFile, #29383 SetInteractiveMusicState, #29546–29632 playlist/group machinery and #29634–29713 native decoders carry generalized group selection, snapshots, cue/transition interpretation, current/old/dying segments, emulated seek and decoder buffers.

[audio_sample_v34.cpp](../../port/engine-audio/audio_sample_v34.cpp):50–66 rejects authored explicit cues and additional segment cue continuation. [audio_mixer_v34.cpp](../../port/engine-audio/audio_mixer_v34.cpp):13–25 requires a constrained sequential-group, unit-weight, once/infinite-repeat domain. Its native_state branch from line89 emits control_required for interrupted transitions and unsupported rule/cue/asymmetric fade fields. No general continuation consumer resolving those conditions was established.

A concrete state limitation occurs in #29640 0x8844e8 MixSegmentInBuffer. Original SegmentState life==3 can recompute a negative slope to finish within a short buffer and forces the segment to stop. [audio_native_envelope_v34.cpp](../../port/engine-audio/audio_native_envelope_v34.cpp):7 preserves ordinary Q30 shifts, PCM accumulation and delay directions, but [audio_native_envelope_v34.hpp](../../port/engine-audio/audio_native_envelope_v34.hpp):6 has only a stopped flag and cannot represent life==3. Current native transitions construct two symmetric envelopes. Ordinary fade arithmetic is bounded support, not whole native-music parity. Exhaustive selected-cache rule/state reachability and runtime acceptance remain unresolved.

### F16-06 — Generic codecs, asynchronous loading and handle ownership need further mapping

**Medium confidence for generalized source parity.** #28975 0x862d10 VoxEngine::Initialize registers raw, MSWav, Vorbis, MPC8 and native decoder factories plus memory/file streams, and constructs source/emitter workers. Inner MPC/Vorbis records are successful bodies, not external imports.

Current [engine-audio/CMakeLists.txt](../../port/engine-audio/CMakeLists.txt):3 builds reconstructed sample/catalog/mixer sources. [audio_sample_v34.cpp](../../port/engine-audio/audio_sample_v34.cpp):18–23 supports PCM16/IMA mono/stereo and RIFF/VoxN containers; [audio_catalog_v34.cpp](../../port/engine-audio/audio_catalog_v34.cpp):39 accepts pcm/adpcm/vxn labels. No selected Vorbis/MPC registration or decode route was established. The supplied catalog lacks those formats, so no selected-asset regression is claimed. Generic raw buffer-reference interfaces, codec widths, cursor seek and allocation behavior are partial.

Original #29214 LoadDataSourceAsync creates pending DataObj state3 and queues source updates. #29118 GetEmitterObject checks cached object/generation before active or staged container lookup. DataHandle/EmitterHandle copies and destruction manage refcounts. Current [audio_bank_v34.cpp](../../port/engine-audio/audio_bank_v34.cpp):8 loads shared immutable samples synchronously; [audio_gameplay_runtime_v42.cpp](../../port/engine-audio/audio_gameplay_runtime_v42.cpp):48 consumes voice token receipts. Missing-file UID slots retain invalid/no-sample/no-retry identity, matching that original prefix, but general async loading, generation invalidation, reference deletion, failure rollback and internal callback closure are not established.

### F16-07 — Lua debug source exists but is omitted from the current target

**High confidence for build disconnection.** Original #28543–28564 includes luaopen_debug, db_* functions, hookf and support bodies. Exact definitions exist in [ldblib.c](../../port/script-runtime/lua/ldblib.c), but [script-runtime/CMakeLists.txt](../../port/script-runtime/CMakeLists.txt):4 excludes ldblib from LUA_CORE. [script_runtime.c](../../port/script-runtime/script_runtime.c):124 opens only base/math/table/string through its source-library interface.

These are disconnected source bodies, rather than absent implementations. Original debug-library registration and selected gameplay necessity remain unverified.

## Bounded positive comparisons and original no-ops

Lua C API/table/numeric/header leaves were compared with exact active definitions. lua_pushnumber #28413 stores one 32-bit payload and tag 3, confirmed in assembly 0x84b458–0x84b474. Current [luaconf.h](../../port/script-runtime/lua/luaconf.h):505 uses float. #28782 luaU_header emits version 0x51, format0, LE1, int4,size_t4,instruction4,number4,integral0; [lundump.c](../../port/script-runtime/lua/lundump.c):215 and [ldump.c](../../port/script-runtime/lua/ldump.c):62 retain 32-bit serialized fields. luaH_getnum preserves the original unsigned array-range test while avoiding modern signed-overflow UB. Weak-mode bounded memchr preserves original strchr first-NUL k/v behavior for valid allocated TString. Whole VM/compiler/GC ABI/error/allocator proof remains partial.

#29831 GetEventSoundUid was compared deeply with [audio_catalog_v34.cpp](../../port/engine-audio/audio_catalog_v34.cpp):63: probability precedes selection; random events use a second draw, swap/pop the chosen entry, append history and reinsert oldest history on limit/exhaustion; playlists cycle; probability miss returns -1 without selection/history change. Valid authored event behavior is matched. Invalid-input, allocation/capacity, timing and runtime acceptance are excluded. ResetEvent remains partial: the original reinserts historical entries into the remaining vector, whereas source reset_event restores source ordering. No active source caller of reset_event was found.

Original empty bodies include #28008 processUploadAvatar, #28093 CAndroidSocket::Init, RSS/channel/item write methods, #28867–28869 Vox public Update/UpdateSources/UpdateEmitters, #28872–28873 public notification registration, #28982 internal SetRoutingVolume, and #29857 DriverSourceInterface::SetDSPParameter. They are not missing behavior. Empty public notification methods do not prove all internal callback machinery unnecessary. Current source_static_bus_routing_v100 at audio_gameplay_runtime_v42.cpp:142 preserves a no-op inherited driver method 0x88ed4c from lane17; it does not invent DSP file IO.

Exact branch thunks: 28713,28875,28973,28974,29537,29699,29700,29701,29763,29764. All remain individual records, and each names its target/underlying comparison status.

## Cross-lane edges and limitations

- Original online AS handlers #6890/#7040/#7048 lead to earlier OnlineGameState/CSignIn/CMatching ranges. GLX response #28033 forwards to WebComponent #27810–#27814.
- Lua C API callers include sfc::script::lua::Value::_pushOnStack #761. Core resume/luaD_call invoke #28802 luaV_execute. The active versioned runtime include and CMake chain were followed.
- VoxSoundManager::PlayEvent #2641 calls #29831; constructors #2653/#2655 call #29854. This gameplay/audio boundary is represented by current runtime/campaign providers.
- Cross-range allocator/synchronization edges include VoxFree #403, VoxAlloc #408/#417, Mutex::Lock/Unlock #29970/#29969, AccessController #29973/#29974 and FileSystemInterface #30017. Assembly resolved the decompiler's uninitialized receiver impression in #29225: the destructor keeps its receiver in R5; Clean uses the static shared mix buffer.
- DriverSourceInterface #29857 continues into lane17 inherited driver methods. Selected-subclass/vtable behavior cannot be inferred solely from an empty generic method.

Unresolved work: exact live source mapping for every GLX/RSS/Unicode/container helper; all indirect/data-only callers; online callback asset reachability and remote services; original debug-library necessity; complete nontrivial Lua VM/compiler/GC ABI/error/allocator parity; authored rand48 frequencies; generalized asynchronous Vox ownership/generation/callbacks; optional soundpack fields and case-varied labels; codec/container families and raw cursor domains; native random/cue/extra/dying/rewind/state branches; exhaustive selected-cache rule reachability; and integrated gameplay/output acceptance.

Those limits remain explicit in the CSV. This lane establishes exact record accounting and supported findings, not complete semantic closure or the overall audit completion gate.

## Source hash ledger

Hashes captured when source contents were loaded and rechecked before report emission. Changed during this window: none.

| Source file | SHA-256 |
|---|---|
| port/android-native/app/src/main/cpp/CMakeLists.txt | ee593e57786a13b37a90364667da0507d1ca4e92d56adc0dccac02b05217da45 |
| port/android-native/app/src/main/cpp/front_ui_session_v87.cpp | f88ceb1ed519625d059b20fefdffabf78ff65282d1b707760d8f0adb42b8b3ae |
| port/android-native/app/src/main/cpp/native_process_menu_actions_v119.inc | 3328910b1c343059b7300a2092bd0463153353ad8abb3c6b966fbb56aa90b538 |
| port/android-native/app/src/main/cpp/native_process_startup_v119.inc | 256c0b9ec77a720ad60d507309727428337aeb44d72fd1fba6b689cd9598cffa |
| port/android-native/app/src/main/cpp/renderer_campaign_audio_services_v68.inc | 0e48f0cbe55988af38417824352a0a62c2dc2d33f54851af32f4aad654d995b2 |
| port/engine-audio/audio_bank_v34.cpp | 439d459f014976e461db3fd2607f6fbbc73cd46e7f2ff32c614c9dae3a87ce8a |
| port/engine-audio/audio_catalog_v34.cpp | 23e0f1e249f012ff104a393359c46f050e4b1837aff18e4aafd20729f853b8d5 |
| port/engine-audio/audio_catalog_v34.hpp | 7654e07625e1d15e7286e48a0dad353942e9541aa54edf47570c5e049cf28f64 |
| port/engine-audio/audio_gameplay_runtime_v42.cpp | f8f61a86a76bf733d8236bf592bd28966a1192a734f77c63edd297f2d8bdcc76 |
| port/engine-audio/audio_mixer_v34.cpp | 0cc122c254f918338419509d80814905b7b59226955c4f01baed7080a9fc952f |
| port/engine-audio/audio_mixer_v34.hpp | ea622a92c2177d60a6717baac955b829d0492fa1faa814e7268101ad38278cf6 |
| port/engine-audio/audio_native_envelope_v34.cpp | 9c62269d40d01521f91e4c74111d38d6dbba913f671d7fc4216c1fc18e87e31a |
| port/engine-audio/audio_native_envelope_v34.hpp | bd4330a3ca10099b941e2ac65b81783eac94e63fb4d4c32c337c37688e61914e |
| port/engine-audio/audio_sample_v34.cpp | 7a31d0493ae300a23be3b269badc20c47d617d88ddd364f43b622a8d4636fd32 |
| port/engine-audio/audio_spatial_v34.cpp | 9bf555424502a015694af03c217d156fa2404e8d8a9b39f2d30f2c75e42982f9 |
| port/engine-audio/CMakeLists.txt | 9ea0b97a309f94aa22df243fb66f5159384e31955a6b6c401dc7fd4953cd4b13 |
| port/engine-audio/integration-v42/audio_application_manager_v42.cpp | ea8a49468fff92373579569eeaaf8df6de766a601f759af052ca00bc44c1f043 |
| port/engine-audio/vox_source_fields_v38.cpp | e3de5b5519051ea830980ee9032fcda307b3caaefcedfa8e1519be06bdb62670 |
| port/engine-ui/swf_actionscript_connection.cpp | c3e873f984d9033fd9950a8963738110b6cd07e0cc6db34cbc6c39aeea70c9ee |
| port/engine-ui/swf_movie.cpp | 16e9c8d492c4226dd93172a9530dd6b2ed129b52b3cd8e1f5fb5c80566ab627c |
| port/level-loader/xml_document_v1.cpp | 1f66fee7d930ebd8f0e20585bb65bae869ddc9dd537d45b2b255b68c22169361 |
| port/level-world/reference/audio-source-v34/sounds.xml | 4bc1de654d65470d49e48cee354d5d5cdd45006c18d65420ddc07acf5b400a6e |
| port/level-world/vox_audio_bridge_v34.cpp | 226f7d6f400590235ee7a8e5891c64683270d5e79b5ee1526a51124e14954534 |
| port/script-runtime/CMakeLists.txt | 842752cc55310bc10ba5a49fe49aeda864ef78b42ae397cf38dd94eacb2c88cd |
| port/script-runtime/lua/lapi.c | 6aa0d9fdd88b13fe46568ad6722ecf44164fb84476cfdf6480cf05b5704935f8 |
| port/script-runtime/lua/lauxlib.c | b564650bf4aca2757b9966311d32c8d5fea8803f6f33d72a856448d3b5d6d992 |
| port/script-runtime/lua/lbaselib.c | 703b8c0de8142d486bc6d6082cf6427583189319a4aed3a02dd8c1653e0fd42b |
| port/script-runtime/lua/lcode.c | f8026741a3e6ea3520c751ef2e7f6c0b88e0e36b4dab93a238d2a5ef0dab18a5 |
| port/script-runtime/lua/ldblib.c | 961946ac6f3d0ffbf9d4c8e845bcb57a407c6b98e26ca881144933d0c53051fa |
| port/script-runtime/lua/ldebug.c | afa5840195befaec55177273c8168748026b38df3243767fc740cc7ced0c4c8f |
| port/script-runtime/lua/ldo.c | 1839bd9227430c5344b348f57e5cd680f56e854932abde31f402282d95882326 |
| port/script-runtime/lua/ldump.c | 661d499262e9808732b2bf67d2d0cedafcaf0b4eefb54c922959af450fd2b87b |
| port/script-runtime/lua/lfunc.c | 525681d79098958530e5c937f7cbe94a4dafa679746ffdb16258f82b7ece29ce |
| port/script-runtime/lua/lgc.c | c095b53992c084bf80a9a2f0f0b885eda474646806c8a8c9ab55c4fb5643ad32 |
| port/script-runtime/lua/llex.c | aecb5f00908832222e62fb1b970825278dc460a65a080d79a3693f82438e52c8 |
| port/script-runtime/lua/lmathlib.c | e2933a56332d567cdd69f0fe514d638a8f82da6878754ff259e77cf2157263c3 |
| port/script-runtime/lua/lmem.c | e63c50cb1c75356bd4123c9ca05f77bd0cd99e05c099cf6d1bf472086761e9c5 |
| port/script-runtime/lua/lobject.c | d704915c79c721556cc5850d741ed3bef1611221356f0e3a2f0ee5cfd5fd3632 |
| port/script-runtime/lua/lparser.c | 61c85a7c4f5387d36b91d53a60df3802d7639928b1d76675ede6fb0868ad2510 |
| port/script-runtime/lua/lstate.c | c36d6b9354aaa30b5323b4984f3172783f69e0deb3186c2ef6f16a8281c59c29 |
| port/script-runtime/lua/lstring.c | aa7e934c435d870b9d937ac2fb03dbfa15d367012d46e7e995a2ad8d440f2f6d |
| port/script-runtime/lua/lstrlib.c | a978166ca36525de2708ac931834d575c8df057719bc2da39dbe3befe810ec1d |
| port/script-runtime/lua/ltable.c | b5d3a42b7068c0c822845563c9974fa00b41a190fd4352910f5e29f0eef94dfb |
| port/script-runtime/lua/ltablib.c | e934dc4b2fc117a501c717302a00147f7b44eea978ef87a58a3e40e9bfe45d20 |
| port/script-runtime/lua/ltm.c | 9759ae5d1aa3b11e1c7faa6c0f65e05c7126417f7f8f3f989d82fc170b07f891 |
| port/script-runtime/lua/luaconf.h | b3ec2e1f0d42e099157cc0adc8a4ef5c62afa09f5fefad9c95de3ef3be89cf23 |
| port/script-runtime/lua/lundump.c | 611fdad9c97d2b81c918d08bc610c830dc6b8f1167b718cb32b5d1eb76149f21 |
| port/script-runtime/lua/lvm.c | f9f0979df389c571193c4b5d20ae80180d9857ca5a9873c601c42055583cf19d |
| port/script-runtime/lua/lzio.c | 892bc24d898c19cbedceba419c68f910b99321c0b462be2c2c00b9d763563173 |
| port/script-runtime/script_runtime.c | 571f4009dfac62b4251d9e4555f6b3cf343166a29d8f65ab92d5bc9d55391b9b |
| port/script-runtime/script_runtime_return_v1.c | ec7454cbab2ac29b4dc89e6bb759bb92dc7735f7341597322bfcdeb6416bb75f |
| port/script-runtime/script_runtime_return_v3.c | 67787fa2f1e98cfdb41daa8288631c299b84171c65f9fe4ac6aa432cb40e1dfe |
