# Lane 17 â€” libDungeonHunter2.so function records 29858â€“31724

Read-only working-tree audit. Coordinator baseline HEAD: 75a7c2fe3261403e841ffbeb46734e9e4e84e75c. This lane writes only lane-17.md and lane-17.csv.

Coverage: **1,867 distinct records**, exactly the inclusive assigned range; 1,510 successful decompilations, 354 failed external-import-slot decompilations and 3 failed executable STLport bodies. Every assigned assembly section was read. All successful pseudocode bodies were read/parsed (1185680 on-disk bytes) and match export SHA256 after LF normalization; Windows CRLF bytes differ intentionally.

Every row retains body/category/control excerpts, code/body fingerprints, bounded xref display with total counts, source location/hash, comparison status and confidence. The cross_lane_edges column lists every exported direct or function-pointer reference between assigned and other indexed functions, with its disposition; bounded display applies only to the general xref summary. The full xrefs.jsonl preserves remaining intra-lane/data edges. All **1,867 runtime statuses are not_run**; no build/test/emulator/audio/service was run. Component receipts do not count as integrated runtime acceptance. Manifest accounting is complete for this lane; unresolved behavior prevents a full parity claim. There are 2,996 exported cross-range direct/function-pointer reference records across 616 assigned rows; every crossing record is listed in the CSV.

## Comparison counts

- clone: 150
- failed-body: 3
- import/thunk: 384
- matched: 3
- missing: 1
- out-of-scope: 1015
- partial: 145
- unclear: 166

Clone requires identical machine-code hash AND normalized body; underlying_comparison_status preserves each named row mapping. Equal no-op bytes do not prove owners/entrypoints/features equivalent. Imports are not executable clones.

## A0/A1 â€” Driver defaults and the active Android output path

IDA 29858â€“29914 begins inside Vox DriverInterface defaults, then covers DriverAndroid, JNI forwarding, construction/destruction and driver source creation. RampIn/RampOut, inherited DSP/routing/SFX hooks, PrintDebug and OSL hooks are original literal no-op bodies. Output mode defaults return0/return1; these are successful bodies, not decompiler failures. The seven JNIEnv records forward through SDK function-table slots, including varargs V forms. They are SDK ABI implementation substrate, separately listed.

VoxSetJavaVM stores one process VM, VoxSetAndroidAPILevel stores one integer, and VoxSetDataThreshold clamps a double ratio into [0,1]; VoxSetJavaVMC stores the VM then sets threshold1. Current AAudio does not reproduce these AudioTrack pacing globals.

The original chain is VoxEngineInternal.Initialize -> CreateDriver 29901/0x88fac4 -> DriverAndroid C1 29900/0x88fa80 -> Init 29896/0x88f9a0 -> _InitAT 29895/0x88f660. The direct call into CreateDriver occurs at 0x869368. _InitAT acquires android/media/AudioTrack methods including constructor (IIIIII)V, write([BII)I, play/pause/stop/release. It creates a pthread with UpdateThreadedAT 29891/0x88f328 as callback (pointer reference0x88f7a4). The sample-rate pseudocode's apparent elf_hash_bucket pointer is a constant artifact; ARM instructions use 44100 (0xAC44). The worker attaches to JavaVM, creates a local frame and byte array, writes PCM16 stereo, waits while suspended, then stops/releases/detaches. _ShutdownAT clears its running flag and joins. Suspend/resume maintain queued duration/start time under the same driver mutex. CreateDriverSource checks driver readiness and adds an allocated source to a retained linked list; destroy unlinks and invokes its own virtual destructor.

The current active replacement is AudioNativeSessionV42.initialize/control_thread/shutdown -> its RealControl -> AudioControlV40 -> AndroidAudioOutputV40.open/apply/data/close. Android CMake explicitly includes V42 session and V40 control/output at lines116â€“122. An older V34 output remains compiled, but the V42 session's concrete RealControl selects V40. V40 opens float stereo at the actual AAudio device rate, gates audibility by source epoch, publishes timestamp snapshots, and preserves the complete owner on an unproved close barrier before join/drain. AssetProvider publication retains Java AssetManager/global-ref and the same live World provider through callback ownership. This is connected current implementation with different transport, timing, ABI and source ownership contracts, so driver records are partial.

Reached inherited routing no-ops have a concrete source match: AudioGameplayRuntimeV42.source_static_bus_routing_v100 (line142) and source_dynamic_bus_routing_v94 (line197) succeed without reading routing files or fabricating a DSP graph. They add live-owner/request validation, so parity is limited to valid reached requests. CSV rows29861/29862 retain that named-row match in underlying_comparison_status even though their original bytes belong to the no-op clone group.

Sources: port/engine-audio/integration-v40/focus/audio_output_v40.cpp; audio_control_v40.cpp; integration-v42/audio_native_session_v42.cpp; integration-v42/application/native_audio_application_v42.cpp; audio_gameplay_runtime_v42.cpp; Android CMake. The registry below gives their exact current hashes.

## A2/A3 â€” Callback buffers, state and spatial behavior

IDA 29915â€“29967 owns callback-period filtering, 24-byte borrowed-buffer ring descriptors, byte/pitch/gain cursors, allocation, upload/disposable-data handling, locked states and mono/stereo interpolation. DriverCallbackInterface._FillBuffer 29927/0x890548 sets static listener/general fields, allocates an int32 stereo mix buffer, dispatches every source through virtual slot+96, and saturates to [-32768,32767]. DoCallbackAT 29890 locks this mixer while Java's critical byte array is borrowed; JNI write/pacing happen after release.

Current AudioSampleBankV34.play_uid retains a sample and queues commands to AudioMixerV34.apply/render; callback receipts retire pinned sample ownership. V40 AAudio.data reaches that same mixer. Thus a concrete playback consumer exists. It uses float interpolation/clipping and command/receipt voice state rather than original Q14 integer accumulation, borrowed ring slots and byte/ramp state. Exact exhaustion, ramp, seek, resampling, rounding and disposal semantics remain partial.

**L17-02 â€” Original SetPitch clamping and smoothing are not preserved at the replacement leaf.** IDA 29937/0x890be8 assembly compares pitch with float2, replaces greater values by Q14 32768, replaces <=0 by Q14 1, writes target+0x34, and computes callback smoothing delta+0x3c while playing. Current AudioMixerV34.apply accepts positive pitch through8; its gains command directly assigns command pitch. The original cap/ramp is not represented at this leaf. The reached fresh-emitter producer in renderer_campaign_audio_services_v68.inc supplies pitch1. Disposition partial; arbitrary discrepant SetPitch inputs being exercised by current assets was not established. Mixer arithmetic also clips floats to[-1,1] rather than original PCM16's asymmetric integer endpoints; exact output comparison remains unresolved.

Three complete spatial leaves are matched in their documented finite input domain:

- 29960/0x891b80 GetStereoPanning: relative/global delta, normalized front-cross-up basis, equal-power square roots and Q14 quantization.
- 29961/0x891ed0 GetDopplerPitch: disabled/relative branches, listener/source dot products, denominator guard, unity fallback and clamps16/47513.
- 29962/0x892114 GetDistanceGain: six inverse/linear/exponential models, clamped variants, nonpositive-denominator guards and Q14 conversion.

Current audio_spatial_v34.cpp preserves scalar operation order with volatile intermediates. Actual audio_source_command_v40/renderer campaign services reach vox_source_spatial_command_v38 -> audio_spatial_v34 -> mixer. Source additionally rejects nonfinite and out-of-integer-range results. NaN/infinity/overflow parity is outside the claimed domain; leaf matches do not validate whole playback delivery.

**L17-03 â€” Finite directional-cone behavior is unsupported by the current spatial representation.** GetDirectionalGain 29963/0x892460 has cone>=360/zero-direction unity branches plus actual angle/outer-gain interpolation. Both original mono fill routines call it (instructions0x89280c and0x892c28). Assembly reads inner/outer cone+0xa0/+0xa4 and outer gain+0xa8. Current AudioSpatialSourceV34 has position/velocity/relative/distance/Doppler fields; AudioSpatialResultV34 has pan/distance/Doppler only. There is no direction/cone field or directional result, and vox_source_spatial_command_v38 multiplies distance*pan only. The original fresh driver defaults have cone360, so that unity branch remains represented. No consumed finite-cone asset or reached cone-changing setter was established. Disposition partial, not a claim every sound is wrong.

Original Reset/Pause/Stop/Play/GetState/NeedData/UploadData bodies are individually retained in the CSV. UploadData stores borrowed pointer/size into the next empty ring descriptor under source lock. Modern retained shared sample bytes and receipt retirement are a partial ownership replacement, not proof of the literal buffer-reference/reclamation contract.

Sources: port/engine-audio/audio_mixer_v34.cpp; audio_bank_v34.cpp; audio_spatial_v34.cpp/.hpp; vox_source_fields_v38.cpp; audio_source_command_v40.cpp; renderer_campaign_audio_services_v68.inc.

## A4 â€” Synchronization and periodic worker ownership

IDA 29968â€“29990 covers exact pthread branch thunks, read/write-access counters, mutex initialization/destruction and VoxThread. AccessController.GetReadAccess 29974/0x893548 is reached throughout original VoxEngine emitter/data queries, updates, gain/pitch/play/stop/release operations outside this lane. GetWriteAccess coordinates actual writers. Original VoxEngine.Initialize constructs two VoxThread owners (calls0x862df0 and0x862e24), preserving callback context and platform priority setup. Its worker performs timed updates and microsecond sleep.

Current V42 uses producer identity, mutex/atomic epoch guards, a dedicated control worker and explicit close/join/drain owner retention. This establishes a connected synchronization consumer with partial original access-counter, priority, time-unit and reentrant-callback equivalence. Single-B wrappers keep import/thunk with their named target, not missing.

Source: port/engine-audio/integration-v42/audio_native_session_v42.cpp.

## A5/A6 â€” File callbacks, limited windows and ZIP policies

IDA 29991â€“30058 covers FileInterface/FileLimited, FileSystemInterface/Stdio, CZipReader/SZipFileEntry and their STL support. Stdio constructors register six open/close/read/write/seek/tell function pointers. OpenFile 30021/0x894330 builds the directory prefix, follows archive-first or file-first order under byte+4, and constructs a limited file over a stored archive extent. Failed allocation closes the actual opened file. FileLimited maintains origin/extent/relative cursor for Read/Seek/Tell; its Write intentionally returns0. SetArchive 30020/0x894290 destroys the previous archive before trying the replacement and clears its archive pointer/flag on failure.

Current AudioSampleBankV34.load builds exact data/sounds/<catalog filename>. The real Application AssetProvider.read/read_optional forwards into its retained OriginalCacheAssetsV1 instance; that mounts dh2-original-cache.zip from a retained APK descriptor and calls ZipAssetPackV1.read. CacheIStreamV65 retains the same archive authority, validates sequential-read extent, checks receiver/control-block identity and releases it on close. These concrete readers are connected. Mutable process-global Vox callbacks, writable direct stdio, directory stacking, relative seek windows, fallback order and destroy-old-first failure policy are not recreated by that read-only archive transport. Disposition partial, not inferred absence from renamed classes.

**L17-04 â€” Archive recognition and name policies differ.** CZipReader.scanLocalHeader 30056/0x895c64 iterates local header signatures/extra/data descriptors/payload offsets. Constructor/getFileInfo 30053/0x89544c optionally lowercase ASCII and/or strip directories. getFileInfo rejects a nonzero compression method and returns stored byte offset/length. Current ZipAssetPackV1.mount requires and validates a central directory/end directory, canonicalizes full URI names with unconditional lowercase, rejects duplicate logical paths and checks CRC; reads support stored/deflate. It does not expose original optional basename/case policies or destroy-old-first replacement rollback. A local-header-only archive can satisfy the original reader yet fail current mount; full-path/case-sensitive policies also differ. Disposition partial. Actual archive selection/incidence needs the data/resource lane before asserting a gameplay failure.

SZip entry copy/assignment/destructor allocation bodies are distinct from compiler string/list/tree implementations. Replacing the container does not itself prove the Vox policies equivalent.

Sources: port/engine-audio/audio_bank_v34.cpp; integration-v42/application/native_audio_application_v42.cpp; port/android-native/app/src/main/cpp/original_cache_assets_v1.cpp; source_campaign_file_io_v65.hpp; port/asset-payloads/zip_asset_pack_v1.cpp.

## A7/A8 â€” WAV continuation and Musepack decoding

**L17-05 â€” Multiple WAVE data chunks are explicitly unsupported.** GoToNextDataChunk 30063/0x896068 follows WaveChunk.next, seeks the actual stream to chunk+8, changes remaining extent and resets decoder cursor. Original PCM Decode/Seek and IMA constructors outside this lane call it. Current audio_sample_open_v34 explicitly throws "Required multiple source WAVE data chunks" on a second data chunk, and its RIFF cursor owns one data segment. Single-chunk PCM16/IMA decoding is implemented. Disposition partial; actual multi-data-chunk sound incidence was not established.

30059â€“30062 are AdpcmState constructor/destructor records; constructor initializes predictor-1/index0 and destructor is no-op. Current IMA decoding initializes predictor/index from each actual block header, with different owner/storage contracts. No missing codec is inferred from these constructor differences.

30064â€“30086 are concrete Musepack CRC, bit/Golomb/log/enum/Huffman tables, stream info, SV7/SV8 decode, synthesis and RNG bodies, plus a distinct mpc_decoder_exit thunk. mpc_demux_init call0x880810 -> mpc_decoder_init 30078/0x8970b8 and mpc_demux_decode calls0x87fb44/0x87fc78 -> decode_frame 30080/0x898104 prove a real original codec family crossing this lane. Current parser recognizes RIFF WAVE/VoxN, and its format function admits PCM16/IMA only; other family/codec inputs fail explicitly. Scoped source-family searches plus parser/consumer review did not establish a Musepack decoder. An actual consumed Musepack asset and possible renamed/provider equivalent were not established. Therefore these records remain unclear, not missing solely from token-search failure.

Source: port/engine-audio/audio_sample_v34.cpp, including format, audio_sample_open_v34, audio_ima_block_v34 and AudioSampleCursorV34.frame.

## L1/L2/L3 â€” Conditional licensing and native HTTP/socket closure

**L17-01 â€” The positive native-license endpoint is missing.** ALicenseCheck_ValidateLicense 30112/0x89becc is an exact B thunk to ValidateServer 30111/0x89bdc0. Original caller instructions are:

- GSInit.Update:0x3854dc.
- Level._LoadProcess:0x3f6fa8.
- MultiMenuManager.PushMenu:0x43870c.

ValidateServer gathers IMEI/Java license/time-random code, formats server requests, reaches sendRequestByGet, checks persistent RMS and invokes Java Quit/exit branches. Init keeps a global Java class ref and license/config/quit method IDs. LoadConfig invokes ConfigFile.Decode; RMS helpers own original file/XOR/checksum data. sendRequestByGet 30110/0x89bc38 -> initXPlayer -> LCXPlayerHttp -> LCAndroidSocket is the actual protocol owner chain, with blob response decoding and validation.

Current source_campaign_runtime_v61.cpp:795â€“799 reads the native-DRM projection at phase38 and only calls validate_native_campaign_license_v93(...,1) if true, before fade-menu push/progress tail. native_menu_resources_v93.inc:284â€“287 borrows the actual menu owner then unconditionally returns false with "Required actual ALicenseCheck_ValidateLicense(...) positive source branch". This is a concrete absent positive behavior at a reached source endpoint, not a search-only conclusion.

Current AuthoredMenuNativeDrmV4 owns a zero-initialized byte, exposes only source_global() and documents no recovered runtime writer. native_process_startup_v119.inc:279â€“280 explicitly excludes online/CX/GC/DRM operations in its offline reconstruction. **This report does not assert current ordinary loads necessarily reach/fail native_drm=true.** Missing is scoped to the conditional positive behavior; current false-branch preservation, possible future writers and supported-product policy are separate unresolved facts.

ValidateNative, Config Encode/Seed, LC debug and some string/number utility entries are original literal no-op/constant/identity bodies. They are retained as such, not reported as lost behavior. Licensing JNI/config/RMS helpers remain unclear mapping, not blanket absence. LC utilities are individually listed, including UTF/blob/parser/file/time/log helpers and exact libc branch aliases.

HTTP/socket records include GET/POST/video/range request headers, NUL-terminated copied response ownership, cancel/destruction, port80 factory, nonblocking connect state, zero-timeout select, send/recv, DNS worker/cache and join, UDP/broadcast/accept/local-address operations. LCXPlayerSocket.Run 30256/0x8a1c1c dispatches virtual DNS/create/connect/select/send/recv and observer methods through its actual retained derived socket. It handles timeout/Content-Length/chunked states. ParseHttpHeader30252, RemoveHttpHeader30253, ParseChunkedContent30254 and CalculateTotalLength30255 own actual parser branches. Lack of direct xrefs into Run is not proof of dead code; the LCXPlayerHttp.UpdateRequest virtual slot calls it.

After exact names, semantic socket/HTTP operations, current CMake/startup and the explicit license seam were reviewed, no equivalent native protocol owner was established in current source. Those rows remain unclear because a supported active online consumer or renamed/provider equivalent was not established. Current More Games WebView is a different feature and is not counted as native LC protocol parity.

Sources: port/android-native/app/src/main/cpp/native_menu_resources_v93.inc; source_campaign_runtime_v61.cpp; native_process_startup_v119.inc; port/engine-ui/authored_menu_native_drm_v4.hpp; menu_stack_v1.cpp.

## R/J/V/F/I â€” Runtime providers, veneers, imports and failures

The large tail contains STLport exceptions, locale/ctype/codecvt/numeric/time/money/messages facets, containers/allocation, streams/filebuf and ARM EABI support. These inspected bodies are compiler/runtime implementation substrate; current CMake uses C++17 and its platform compiler/standard-library runtime. Original STLport ABI/object layout and internationalization edge behavior are not source parity claims. The out-of-scope disposition applies to that substrate, not consuming game behavior. The appended final record31724/0x8a4950 (_Locale_impl._S_uninitialize) is included despite nonmonotonic address and is original Thumb BX LR.

Executable failed bodies are distinct:

- 30698/0x8b3674: vector node-pointer allocate/copy. Assembly allocates then branches through noncontiguous continuation0x8b386a ->0x8b3d6e. Full copy/exception parity remains unproved.
- 30876/0x8b651c: BL _Pthread_alloc_impl.deallocate then POP; forwarding intent recovered in assembly, failed-body export disposition retained.
- 31324/0x8bd9c8: loads _Messages receiver and stack arguments, then BL _Messages.do_get; forwarding recovered, failed-body export disposition retained.

31365â€“31369 are five ARM/Thumb interworking veneers (literal LDR, ADD PC, BX R12) to named STL throw/allocation/deallocation targets, each import/thunk.

31370â€“31723 are 354 external import slots, at0xa3642câ€“0xa369cc. Each exactly matches imports.json .dynsym metadata; each assembly section is IMPORT storage. Their failed decompilations are expected because there is no executable body. APIs include libc/POSIX/pthread/math/EABI, GLES, C++ allocation/exception and Android logging. Platform-linked import implementations are not missing game bodies. Exact names and inbound PLT/data references remain individually listed.

Source provider evidence: port/android-native/app/src/main/cpp/CMakeLists.txt:126 (C++17) and its platform link declarations; individual source ABI/provider implementation is intentionally not asserted.

## Cross-lane paths and unresolved work

1. Join VoxEngineInternal.Initialize -> CreateDriver/DriverAndroid and VoxEngine.Initialize -> VoxThread to higher-level emitter/data ownership in lane16.
2. Join mono/stereo fill dispatch and cone/pitch setters to actual asset/current callers before treating finite spatial leaf matches as playback acceptance.
3. Determine consumed WAVE multi-data-chunk assets for the original PCM/IMA -> GoToNextDataChunk path.
4. Determine consumed Musepack assets and any provider equivalent before upgrading unclear support to an active missing path.
5. Join GSInit/Level/MultiMenuManager -> ALicenseCheck_ValidateLicense, Java licensing methods and DRM writer/initial state with primary/callback lanes. Initial BSS0 is not authority for an unobserved later writer.
6. Join original directory/archive/limited-file virtual consumers with lane22's resource/path/failure/lifecycle closure.

## Scoped searches and limitations

Searches covered current port C/C++/headers/include fragments and Java sources, excluding reports/tests, reference/source-snapshot/staged/prospective/compile-layout and vendor copies from source-implementation claims. They included exact families and semantic alternatives: AAudio/AudioTrack, render/mixer/pitch/cone, ZIP/archive/local/central headers, file/read/seek ownership, licensing/native_drm, LC socket/HTTP and Musepack. Actual consumers/providers and CMake selections were read after those searches. Old receipts/documentation were leads only.

Negative findings are grounded in actual source: L17-01 has an explicit failing endpoint; L17-03 has an incapable current representation/command consumer; L17-05 has an explicit parser rejection. Direct calls/jumps and pointer refs came from the complete native export; body/assembly established virtual and JNI table calls. Missing direct edges are not proof missing/dead code.

This pass does not prove every transitive game caller, imported API behavior, STL exceptional/locale edge case, device/audio timing, external server availability or positive DRM state. Input domains for NaN/overflow, ring/byte-seek parity, ZIP replacement and name policy, finite-cone/pitch values, Musepack and multiple-WAVE-data-chunk incidence remain unresolved. Helpers classified by runtime family are not claimed line-for-line equivalents of modern compiler output. No integrated acceptance for any assigned record was established; all runtime fields remain not_run.

## Exact repeated-body groups

Each CSV record remains separately listed. This table lists every member of each repeated-body group. Distinct relocated bodies are not collapsed.

| Code SHA256 | Normalized body SHA256 | Representative | Every assigned member |
|---|---|---:|---|
| 379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f | a8f04102732e8eeaca3d921dc2f3ae0bfd8bf9aecfd9b06b658a915c18f3bf0a | 29858 | 29858, 29859, 29860, 29861, 29862, 29863, 29873, 29874, 29875, 29876, 29877, 29878, 29879, 29880, 29909, 29910, 29912, 29913, 29914, 29991, 29993, 30061, 30062, 30087, 30123, 30124, 30135, 30198 |
| 6dcd5e75586fe6683156c0d559d4827b73bd48d501fa5781a1c6099aacd7c877 | 44a1d2edf9e4dbfb6cf5697f921aa5104f0a5af5a53a8bed6c6ff5e8b76a95ae | 29864 | 29864, 29911, 29994, 30117, 30118 |
| 007f34a6c3441b0240da53f253e513b959105da2d8803255e1856aa48f48db47 | 4c071c3330125cde9ce10b97f2627e3d5deea2bd316f9f4cd761a92b5e426d4d | 29865 | 29865, 29866 |
| 233df71df779d3345a7ab19d42b1cbe9ea7a93fc7a3d39ca6eb67d424cd03382 | 5bca468bcb70ebd52616cbe4e21800f7961258143dfffc0b0a00190d805abc18 | 29975 | 29975, 29976 |
| e09098c225d2d1bca5a1210105e0597c69a1bad62bdd562d849cb5ba507cc8e0 | 9bde4b4d0a800a39cbb2a79567a98bcbf88cbefaed63c1d4ab0f3c8f84b311fa | 29999 | 29999, 30000 |
| 74893ee8ef2d006689d049ed648ecc120989798a2195e20df04a7f4372a1b7dc | 74660d08d4a5ff2a6e55ff893ffd76ba33f8cc948937c2b8855a4a672bd13bd4 | 30023 | 30023, 30024 |
| 8e0e8978effd1180b6e4966b2a077a20110f8a0f6cf7a44f3bd93e543c7c168c | 94d9e7377284868f80b88f42ab03f86ec4a9e3aa693246210aa311b93751ac97 | 30059 | 30059, 30060 |
| 224ac606e349352411ee4e84aa9fd6a43d892d2f8ae09c4f53030b3130621516 | 6efff98a7021b7d4cca4f72359c764028b899b0b975ffd65c9d52d69e8b58964 | 30065 | 30065, 30070 |
| 379bec29dccd0a93c94826144d7ef6e42fab64ef195a3b8313a16926f66f388f | 395c5d5ffc62b448dba382eb65a576e323f78ed622b7efc97c3a1e28fa1386a6 | 30113 | 30113, 30114, 30115, 30116 |
| ce6d1c4cedc677a022c5dc23ab42952d2b177fe6c54c35b4095ddc8410a6e4b8 | 432855ae7f9687dc1b87601c93fb66d9f1d7923801e2c679518f6e8da8275980 | 30257 | 30257, 30258 |
| c7dfbb7d02759eacb64dbc916c1bb6f21eabaff1c1032ea5c9176abf7fd28df8 | 395c5d5ffc62b448dba382eb65a576e323f78ed622b7efc97c3a1e28fa1386a6 | 30259 | 30259, 30260, 30999 |
| 2788c4ce76c3d3c6b5e29a35ed5b9a53ae8d0f127dc963dac4878041dcc80089 | 3cdd3ef38e2852ef64a1f1445a7d05e3c15642e0709df765526bb6b5ea89acc4 | 30334 | 30334, 30335 |
| c7dfbb7d02759eacb64dbc916c1bb6f21eabaff1c1032ea5c9176abf7fd28df8 | a8f04102732e8eeaca3d921dc2f3ae0bfd8bf9aecfd9b06b658a915c18f3bf0a | 30364 | 30364, 30396, 30887, 30909, 30910, 30911, 30912, 30913, 30914, 30915, 30985, 30995, 31235, 31269, 31271, 31724 |
| 10c465ab1337de333f6a01cc466d001197d3c4e36ff61c5362fa66689961a188 | a2e6cc0dc09b78a2181f155746598d643e25b295575a5909f2fbf23d7c1bfd21 | 30409 | 30409, 30410, 30411, 30412 |
| 13a1bf93cfbb609d191111a28c30b34754c544546071fc0c1709f9c38d3842e5 | 320f98662a4b77560523a94c79da3b7c302b2c2e95ab64aee8f7abf1828bc6d9 | 30509 | 30509, 30512 |
| a7ddd513d149ea16fdd4db3f82267f83087aeaddd06b5dde5468adb704205fc4 | 44a1d2edf9e4dbfb6cf5697f921aa5104f0a5af5a53a8bed6c6ff5e8b76a95ae | 30725 | 30725, 30953, 30954, 31002, 31003, 31121, 31126, 31129, 31134, 31341 |
| 8015340a996ef65969348ad51755c489844b7fbe65d6bc3ee1ad0f63c9b0dffe | 007fc243ddf188135fcfb3c2efc983bfbc1d77a21ca0ebefa2bac2019c874d88 | 30896 | 30896, 30897, 30898, 30899, 30900, 30901, 30902, 30903, 30904, 30905, 30906, 30907, 30908 |
| 9ecd8697a2e654d12385c77a7405e47c66fecce1c63da5116341b27142019551 | 2727b9f2bd3eeb99cb2f405a10d7a17d2c0200cabd0380dfdbf8d6cfde377edf | 30916 | 30916, 30917, 30918, 30919, 30920, 30921 |
| a7ddd513d149ea16fdd4db3f82267f83087aeaddd06b5dde5468adb704205fc4 | 292a9a99e65963d02a45a500d719547299cb945537f099089a46720b7b887ffd | 30922 | 30922, 30923, 30924, 30925, 30926, 30927 |
| 64d23481cb6a254816fb9c4c9b22031f78ed0a6ece5a6b0f6849a3ee7f77a2bb | 4c071c3330125cde9ce10b97f2627e3d5deea2bd316f9f4cd761a92b5e426d4d | 30931 | 30931, 30932, 30933, 31084, 31085, 31086, 31093, 31094, 31096 |
| abb282b50860639ee18170c25f6bcc37173b294c5a2eb1d1abb75d8453c3bf90 | ade070c7206cf193f415e436d834edba12ce75ff97f4d5c3a8772e2f2538d4b3 | 30937 | 30937, 30942, 30948, 30963, 31217, 31219 |
| 5f081e292e7ce3c1008dd0e497f1d2b02f1d6cb4b96a52eb390fdead5fc41220 | 39a346ed53582bed04128c3049fcc6fe4905372d856fd110aac6c6d2db04e3f6 | 30938 | 30938, 30943, 30949, 30964, 31218, 31220 |
| 9123c49152798024279aa6044230931a28ef737e7a309d356bc999f4b23c6321 | de27b90ca2c2531cfc300d397fd2d028fa270f9b54ccb81e47837e230efa1801 | 30939 | 30939, 30946, 30947, 30950, 30951, 30952, 30974, 30975 |
| 3ee1b4c634aeccc7035f5f381963556f532f4e9191b7e13333c052247b886cb2 | cfc4fba64976a26ac98edf2e503b1ab46f6bee39070caa583e7b50cd219e83f5 | 30955 | 30955, 30956, 30957, 30958, 30959, 30960 |
| 29511eff40504805d0cae1e5570872c8462cdda094d19f6ab9274efb53e4c40e | 65f4cdb179ffb877520f2ce9302ddf06172abb1a0f745a82b28580eca5b2e9fd | 30961 | 30961, 30962, 30965, 30966 |
| 1e470513657e1cc3455bee4d416eff5058527f7e354ba13a41c801d3ce036922 | 0ec4d9eef935654385a4b69cfc401f86f8e3a75584a1b2a81712cf715c00db34 | 30971 | 30971, 30972 |
| 81fa77e1d0f2a6e9afab154b29a59cdf7c5ae7c424d5cd6cf5e49b441001f0e5 | 2d5222f480932bbe2d801bd32c5500b2dc5d4aedb990e4cb8593f33bff5578bd | 30984 | 30984, 31004, 31005, 31006, 31268, 31270, 31342 |
| d9295d349e5e181a2d0e586b5a60fe2880d6d6835c83ddf99f373d1ed751e288 | d3b1ab61a2eb571a26088b425b9ed0f80c87585b540741100cc6ef658e253505 | 31083 | 31083, 31095 |
| e1ff6ec50d8010f734ff4e51d3ef32b1d22753ac15dd43eed0a89299cb946e58 | b6dcadf812062b81d1d8ce067bb4648c3770d7055533e09a4643e0ad8fa6b2de | 31088 | 31088, 31089 |
| 1d05676ca52f33ba98585fe640558cd9beaebf13fafa4511c3a583576035c1cb | bad6012153cafec6d5e8cfb9af1e0e73f4a916623b4e8312664970cc205a109e | 31117 | 31117, 31118, 31122, 31123, 31127, 31128, 31132, 31133 |
| a64fce40790a3d7d5dfb0ed4ef475f25be09bd4317db9b5e0c1cc9dd24f3601a | 3918175ac5a0ff6bebac69d34fa6d90f2830c00f75d1d68fc5d49a6a8aca38a9 | 31119 | 31119, 31124, 31130, 31135 |
| ec9c814015d509041d72ca80169f13752889c3faa3879fdc9bce43d4a962aa34 | acfbe6854ba2e522cecb6a37ffce3b14c64ae943929e68dc7c2c76c6a2e87720 | 31120 | 31120, 31125, 31131, 31136 |
| d7daa101183c4f8debe67c2f6e73ad107645be5fd5b26f0a6e570ded2c33795a | 9de2b7e3160b64cfb3c82536af17d3944fa5a69a6a189b8f7c2075f8fab1e6a1 | 31325 | 31325, 31326 |

## Current source hash registry

Citation capture hashes were rechecked at writing. Changed files: none among the 30 cited files.

| Source file | SHA256 |
|---|---|
| port/android-native/app/src/main/cpp/CMakeLists.txt | ee593e57786a13b37a90364667da0507d1ca4e92d56adc0dccac02b05217da45 |
| port/engine-audio/CMakeLists.txt | 9ea0b97a309f94aa22df243fb66f5159384e31955a6b6c401dc7fd4953cd4b13 |
| port/engine-audio/integration-v40/focus/audio_output_v40.cpp | 6e6c4b594b705871bb80d889833f4db97b1fcebd9cf5aba393a6acb94084606a |
| port/engine-audio/integration-v40/focus/audio_control_v40.cpp | 523446f9cf9f0c81acbb1206c7dc2b4fb26f33c41c52f2002711b7bec524967a |
| port/engine-audio/integration-v42/audio_native_session_v42.cpp | 4fa51239292536591836546465082fb2067ca352b43c4c08e0cde304decb1c4c |
| port/engine-audio/audio_mixer_v34.cpp | 0cc122c254f918338419509d80814905b7b59226955c4f01baed7080a9fc952f |
| port/engine-audio/audio_mixer_v34.hpp | ea622a92c2177d60a6717baac955b829d0492fa1faa814e7268101ad38278cf6 |
| port/engine-audio/audio_spatial_v34.cpp | 9bf555424502a015694af03c217d156fa2404e8d8a9b39f2d30f2c75e42982f9 |
| port/engine-audio/audio_spatial_v34.hpp | 0260968fcc40ea69fc3fe8a914c53c535e8918f519e98f0f56755146d8cd28a2 |
| port/engine-audio/vox_source_fields_v38.cpp | e3de5b5519051ea830980ee9032fcda307b3caaefcedfa8e1519be06bdb62670 |
| port/engine-audio/audio_source_command_v40.cpp | be2dc7d524f9040142699585e924fc99fd445dedd0b330d7944b864ae863f496 |
| port/engine-audio/audio_sample_v34.cpp | 7a31d0493ae300a23be3b269badc20c47d617d88ddd364f43b622a8d4636fd32 |
| port/engine-audio/audio_bank_v34.cpp | 439d459f014976e461db3fd2607f6fbbc73cd46e7f2ff32c614c9dae3a87ce8a |
| port/android-native/app/src/main/cpp/original_cache_assets_v1.cpp | 6a4eeef2116e4da7555eca168f663b272a8ac4cf53aeabae15f033717b385bdb |
| port/asset-payloads/zip_asset_pack_v1.cpp | 38322384ea3114f1ea3aad0ee0211bb6f583dfaf4a76ae642645fe208a9b17df |
| port/android-native/app/src/main/cpp/source_campaign_file_io_v65.hpp | 7be253e16e3b89e4c6fc7b31fe5f66d87033c0a6f4289f187d34ac1cb9d17b7d |
| port/android-native/app/src/main/cpp/native_menu_resources_v93.inc | c4ed613435541f69ef9d668c926e4713ae3e62dcd757888a90529d200581a57e |
| port/android-native/app/src/main/cpp/source_campaign_runtime_v61.cpp | 6b20e56262c4051cc5f35b61b6c7a7fd6b7fc9b75765f7825666326a90a5a38e |
| port/engine-ui/authored_menu_native_drm_v4.hpp | 82e5ba173f60b37ea1621be6f4103b172850937c7da8decd4936cc31eddeda33 |
| port/engine-ui/menu_stack_v1.cpp | 8c68bb6f8ebcb0c5bc33bc1373e656d9d14165521fbf922337e3766d6c7c8f2d |
| port/android-native/app/src/main/cpp/native_process_menu_actions_v119.inc | 3328910b1c343059b7300a2092bd0463153353ad8abb3c6b966fbb56aa90b538 |
| port/android-native/app/src/main/cpp/native_process_startup_v119.inc | 256c0b9ec77a720ad60d507309727428337aeb44d72fd1fba6b689cd9598cffa |
| port/android-native/app/src/main/cpp/native_application_audio_v45.inc | d2591784b7e491a63f09311c776785b893a14f80af1ada86b6d3adb4bb9af019 |
| port/engine-audio/integration-v40/runtime/default-source-fields/default_source_fields_v40.hpp | 93a9a2e164f5ab18d4931b6fbb87abf5504ac699fabe50e9d89943658cdc1884 |
| port/engine-audio/audio_gameplay_runtime_v42.cpp | f8f61a86a76bf733d8236bf592bd28966a1192a734f77c63edd297f2d8bdcc76 |
| port/android-native/app/src/main/cpp/renderer_campaign_audio_services_v68.inc | 0e48f0cbe55988af38417824352a0a62c2dc2d33f54851af32f4aad654d995b2 |
| port/engine-audio/audio_level_routing_stop_v94.hpp | 4b37aa298cf54c02e3ae14c39f59f015f51950b6b9192bed3f89716165cc6611 |
| port/engine-audio/integration-v42/audio_application_manager_v42.cpp | ea8a49468fff92373579569eeaaf8df6de766a601f759af052ca00bc44c1f043 |
| port/engine-audio/integration-v42/application/native_audio_application_v42.cpp | 2ba71f13d21f6e082b705f95226ab59a0178337798f84737521fe66f5b6caa88 |
| port/level-world/vox_audio_bridge_v34.cpp | 226f7d6f400590235ee7a8e5891c64683270d5e79b5ee1526a51124e14954534 |

## Input fingerprints

- .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/functions.jsonl: 3922efb894cf09cee9690708bc3976eaf6bc1372f7a584808ad9583ef3d8ff40
- .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/imports.json: 17b8845ba2950ba8038540186415ef7d231bf3f95137c265fbb60f95dff7f5d0
- .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/assembly-functions.asm: 2545683b66dbb3f8400f64774593b439ac72f4d9647f3f8506b3b028dbb9eb77
- .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/xrefs.jsonl: a4052ed15489b398edee7660a63e8286a35ffd642856a3c3ce82becf2efc8008
- .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/decompile-failures.jsonl: 9d9647538f5cd6a1237d8645390df7baf3e241b9c26ce1b568fda31b52997dcd
