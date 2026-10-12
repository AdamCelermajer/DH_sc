# vox-audio notes

Status after wave 7. The ledger has 699 data rows: 452 reconstructed, 182 partial,
65 skipped. Wave 7 added 68 rows (from 631 at the start of the wave). The queue has 1374
functions: 699 ledgered, 675 remain. Of the 675, 237 are queue entries that are not
vox-audio code (see Wave 7), so the vox-audio pool is 438 (386 named bodies and 52
Musepack entries).

Builds: `tools/decomp_build.sh host vox-audio` (30 tests, all pass) and
`tools/decomp_build.sh windows vox-audio` (MinGW cross build, tests under Wine; 30 of 30
pass, no compile errors or warnings). Each category is one static library (port/decomp-reconstruction/CMakeLists.txt),
so a source that calls a declared-but-undefined function only fails in a test that links
it. No test links the VoxEngineInternal or VoxEngine sources yet.

Tests (30): access_controller, data_obj, decoder_cursor_base, decoder_mpc8,
decoder_mswav_cursor, decoder_raw, decoder_stb_vorbis, driver, driver_android,
driver_callback, emitter_obj, file_stdio, gain_fade, group_ctor, groups, handlable,
handle, ima_adpcm, mswav_sub_decoder_pcm, native_playlist, native_sub_decoder,
priority_bank, random48, sound_pack_xml, state_mutex, stream_base, stream,
string_compare, vox_thread, voxcallback_manager.

Wave 6 added: file_stdio (vox:: stdio helpers, a new header and source), native_playlist
and native_playlist_calls (PlaylistElement, NativePlaylist, NativePlaylistsManager
partial), driver_callback (DriverCallbackSourceInterface state machine and ring),
driver_android (DriverAndroid mode dispatch and VoxSet* exports), native_sub_decoder_state
(derived state ctors and dtors, partial), mswav_sub_decoder_imaadpcm (dtor), the
SegmentGroup and SequentialGroup constructors (groups.cpp, with GroupInfos), and the
DecoderRaw factory (decoder_raw.cpp). Also corrected: GetPitch and GetGain use 2^-14,
not 2^-15 (the ARM constant is 0x38800000).

## Class coverage

Counts are ledger rows by status (reconstructed / partial / skipped) and the
functions still queued. Queued totals come from queue.csv.

Complete (every queued method ledgered):

- AdpcmState (4), HandlableContainer (11, all reconstructed), DriverInterface (11),
  DriverSourceInterface (6), DecoderRaw (8) and DecoderRawCursor (9),
  StreamMemoryBuffer (8) and StreamMemoryBufferCursor (9), PriorityBank (1 skipped).
- DataHandle (8) and EmitterHandle (8): all rows are partial. The refcount helpers
  they call (Increase/DecreaseDataObjectRefCount, Increase/DecreaseEmitterObjectRefCount)
  are not defined, so the bodies stop at those calls.
- DataObj (15: 7 reconstructed, 8 partial), DecoderCursorInterface (11: 10 + 1 partial),
  StreamCFile (8: 7 + 1 partial), StreamCFileCursor (9: 8 + 1 partial), Mutex (10:
  7 reconstructed, 3 skipped as the decompiler thunk and the deleted copy constructor).

In progress:

- VoxEngineInternal: 79 of 123 ledgered (78 reconstructed, 1 partial), 44 remain.
  Remaining: the debug output, GetDataObject (0x8686bc), DetachEmitterObject
  (0x86a5d0), the update loops, and SetInteractiveMusicStateChange.
- VoxEngine facade: 98 of 111 ledgered, all but 6 partial. Each forwarder calls a
  VoxEngineInternal target (see Decisions). 13 remain.
- EmitterObj: 38 of 60 ledgered (36 reconstructed, 2 partial), 22 remain. Play, Pause,
  Stop, Resume, SetGain, SetPitch, Update and the 3D debug helpers depend on a fade
  state machine that is not yet traced.
- DecoderMSWavCursor 7 of 10, DecoderMPC8 6 of 8, DecoderMSWav 3 of 9, DecoderNative
  3 of 14, VoxNativeSubDecoder 3 of 27, VoxNativeSubDecoderPCM 7 of 18,
  VoxNativeSubDecoderIMAADPCM 1 of 21, VoxMSWavSubDecoderPCM 2 of 8.
- Groups: SequentialGroup 11 of 15 (GetGroupElementPosition partial), SegmentGroup 6
  of 10, RandomGroup 2 of 16 (its position cycle needs lrand48, see Open questions).
- PriorityBankManager 10 of 13, FileLimited 4 of 10, VoxThread 2 of 9.

Not started: VoxSoundManager (52), VoxSoundPackXML (39), DriverAndroid (7) and
DriverAndroidSource (6), CZipReader (10) and SZipFileEntry (4), DecoderNativeCursor
(16), DecoderMPC8Cursor (10), DecoderStbVorbisCursor (8), DriverCallbackInterface
(12), FileSystemInterface (11), FileSystemStdio (5), plus the Musepack helpers.
NativePlaylist (15) and NativePlaylistsManager (10) are blocked, see Remaining work.

## Layouts inferred (32-bit offsets from the pseudocode, kept as comments)

Base layouts from waves 1 to 4:

- `vox::StreamCursorInterface` vtable: +0x08 Init, +0x0c Shutdown, +0x10 Seek,
  +0x18 Tell, +0x1c Read, +0x20 ReadRef, +0x28 AllowBufferReference. Slots
  +0x14 and +0x24 are pure virtuals with no symbol. The header calls them
  IsSeekable and EndOfStream respectively. Neither assignment is confirmed by a listing.
- `vox::StreamInterface` vtable: +0x10 Size.
- `vox::StreamMemoryBuffer`: +0x04 size, +0x08 data pointer, +0x0c owns byte.
  Params block `StreamMemoryBufferParams`: +0x00 data, +0x04 size, +0x08
  explicit-owns flag, +0x09 owns byte.
- `vox::StreamMemoryBufferCursor` (12 bytes, VoxAlloc(0xc)): +0x04 owner,
  +0x08 position.
- `vox::StreamCFile`: +0x04 size, +0x18 path string, +0x20 file system.
- `vox::StreamCFileCursor` (0x801c bytes): +0x04 owner, +0x08 file handle,
  +0x0c position (-1 = unknown), +0x10 32 KiB read-ahead window, +0x8010 window
  fill, +0x8014 window index, +0x8018 window base offset.
- File handle (`vox::FileInterface`, stand-in): +0x08 Read(dst, size, count),
  +0x0c Seek, +0x10 Tell. File system (`FileSystemInterface`): +0x08 OpenFile,
  +0x0c CloseFile. The port exposes the handle as `GetFilePtr()` at +0x04.
- `vox::DecoderCursorInterface` (base members, inherited by DecoderRawCursor):
  +0x14 decoder_ (DecoderInterface*), +0x18 stream_ (StreamCursorInterface*),
  +0x1c loop_ (bool). Vtable: +0x14 DecodeRef, +0x1c Reset, +0x20
  GetRewindLimit, +0x24 Rewind, +0x28 Seek, +0x2c SetLoop, +0x30
  GetStreamCursor, +0x34 AllowBufferReference.
- `vox::EmitterObj` (see Decisions for the accessor quirks): +0x18 mutex_,
  +0x1c ready_flag_, +0x28 group_, +0x44 gain_modifier_, +0x8c loop_read_,
  +0x8d loop_, +0x90 state_, +0x94 state_set_, +0x99 reset_flag_, +0x134
  user_data_.
- `vox::VoxEngineInternal` vtable (52 bytes, 13 slots): +0x00/+0x04 destructor,
  +0x08 Initialize, +0x0c Suspend, +0x10 Resume, +0x14 IsSuspended, +0x18
  UpdateSources, +0x1c UpdateEmitters, +0x20 KillEmitter(EmitterObj*), +0x24
  SetGroupGain, +0x28 SetMasterGain. The last two slots are not named.
- `vox::VoxThread`: Sleep is usleep(ms * 1000) in the original; GetCurThreadId
  is pthread_self(). The port uses std::this_thread for both.
- `AdpcmState`: uint16 at +0x00 (initialised to 0xffff), byte at +0x02 (0).
  Meaning not established. Decoder fields in VoxNativeSubDecoderIMAADPCM are
  at +0x1a0 onward, four-byte stride.

Wave 5 additions:

- `vox::VoxEngineInternal` members: data_objects_ (HandlableContainer) +0x08,
  data_pending_ +0x28, emitters_ +0x78, emitter_pending_ +0x98, emitter_access_ +0xc4,
  emitter_pending_access_ +0xd0, priority_bank_ +0xdc, master_gain_ +0xe4,
  group_slots_[32] +0xf8 (20-byte stride, `VoxGroupSlot`: float gain at +0x00, four
  words after), data_objects_access_ +0x54, data_access_ +0x60, driver_ +0x54c,
  mutex_ +0x3f4. Listener block: position +0x3f8 (x y z), velocity +0x404,
  orientation +0x410 (front xyz, up xyz). General 3D parameters +0x428 (param 0 float),
  +0x42c (param 1 float), +0x430 (param 2 int). Dirty bytes: general +0x434..+0x436,
  listener +0x437 (position), +0x438 (velocity), +0x439 (orientation). Stream factory
  table: 31 pointers at +0x444, count at +0x4c4. Decoder factory table: 31 pointers
  at +0x4c8, count at +0x548. suspend_count_ +0x594.
- `vox::DriverInterface` vtable: +0x10 Suspend, +0x14 Resume (no symbol; declared pure
  in the port and named after the VoxEngineInternal callers), +0x18 RampOut, +0x1c
  RampIn, +0x20 Update(float), +0x28 SetDSPParameter, +0x2c SetStaticBusRouting, +0x30
  SetDynamicBusRouting, +0x34 SetSFXPresetActive, +0x38 SetOutputMode, +0x3c
  GetOutputMode.
- `vox::DriverSourceInterface` vtable: +0x40 SetDSPParameter, +0x44 Update, +0x48
  AllowBufferReference, +0x4c FreeDisposableData.
- `vox::EmitterObj` (wave 5 slots): bank_ +0x30, in_bank_ +0x34, flag_6c_ +0x6c,
  callback_pending_ +0x98, dsp_buffer_ +0xec, dsp_buffer_size_ +0xf0, owned_buffer_
  +0xf4, dsp_source_ +0x110 (DriverSourceInterface*), need_to_die_ +0x11c, auto_kill_
  +0x11d, state_callback_ +0x138, state_callback_user_ +0x13c. 3D block: position
  +0x9c (parameter 8), direction +0xa8 (parameter 10), velocity +0xb4 (parameter 9),
  param0 +0xc0, float parameters 1..7 at +0xc4..+0xdc, dirty bytes +0xe0..+0xea.
- `vox::DataObj`: priority_bank_id_ +0x20, uid_ +0x24 (the other slots are above).
- `vox::Vox3DGeneralParameters` (12 bytes) is the copy shape of the +0x428 block.
- `DecoderMPC8`: first (int32) at +0x00 of the params block, param_ at +0x04
  (initialised to -1).
- `VoxMSWavSubDecoder` (interface): Decode +0x08, Seek +0x0c, HasData +0x10.
  `DecoderMSWavCursor`: sub_decoder_ +0x24, loop_ +0x28 (the cursor sets it).
- `VoxNativeSubDecoder`: has_data_ +0x16c. `VoxNativeSubDecoderPCM`: bytes_per_sample_
  (int16) at +0x10 of the PCM object.
- `mpc_reader_t`: callbacks[5] at +0x00..+0x10 (not used by the wrappers), `data`
  (StreamCursorInterface*) at +0x14.

Wave 9 native sub-decoder slots (w9-native-subdecoder). Offsets are from the vptr, which is
the address point of the vtable symbol plus 8. The decompiled indirect calls use the same
numbers (`*(this) + N`). Names come from the PCM and IMA ADPCM overrides in the vtable
symbols (vtables-001.json) and from the call sites; the base class vtable has zero words
(pure) in every slot not listed as a method.

- `vox::VoxNativeSubDecoder` (14 slots): +0x00 D1, +0x04 D0, +0x08 HasData (defined),
  +0x0c Seek(unsigned), +0x10 Reset (defined), +0x14 DecodeCurrentSegmentWithOffset(void*, int),
  +0x18 DecodeSegment(void*, int, SegmentState*), +0x1c GetBytePositionFromSampleOffset(unsigned)
  (returns unsigned), +0x20 GetDecodingBuffer(), +0x24 ReleaseDecodingBuffer(int),
  +0x28 Seek(int, SegmentState*), +0x2c SetDecodingBufferToSegmentPosition(SegmentState*),
  +0x30 EmulateDecodeCurrentSegmentWithOffset(int), +0x34 EmulateDecodeSegment(int, SegmentState*).
  The first call of the decode path is Decode through +0x18 with the segment, the mix of
  one segment through +0x14, and the emulated mix through +0x30 and +0x34.
- `vox::VoxNativeSubDecoderIMAADPCM` adds +0x38 EmulateSetDecodingBufferToSegmentPosition(SegmentState*),
  which the IMA-only vtable (68 bytes) holds after the shared 14 slots.
- Segment record: 0x44 bytes, copied as 0x41 bytes (the record without padding). The three
  records sit at +0x98 (A), +0xdc (B) and +0x120 (C) in VoxNativeSubDecoder, and at +0x6c, +0xb0
  and +0xf4 in NativeSubDecoderState. Word n of a record is +4n: 0 chunk, 1 play mode, 2 byte
  offset, 3 sample position, 4 loop start, 5 end frame, 6 entry count, 7 entries left, 8 restart
  at end, 9 state, 10 remaining frames, 11 (field_2c), 12 ramp frames, 13 ramp step, 14 gain,
  15 decoding slot, byte 16 reset flag.
- Object words used by the segment code: +0x2c loop lists (vector<vector<int>>*), +0x30
  playlists, +0x94 active segments, +0x164 next event, +0x168 pending seek, +0x16c has data,
  +0x0a channels, +0x0c track word, +0x10 bytes per sample, +0x12 bits. The mixing buffer is
  s_pMixingBuffer (32-bit sums) with s_nbSamplesMixingBuffer frames.
- Constructors are blocked: the base C1 and C2 take the stream, chunks, states, audio segments,
  loop lists, rules, parameters, a map, an int, a string compare and the playlists. The ARM
  order (stream in r1, unused r2, states in r3) does not match the mangled order, so the
  NativeChunks position is taken from the readers only. Needs the caller to settle it.

## Decisions

- stb_vorbis is ledgered `skipped`, not `upstream`. The binary's copy is a
  Gameloft-modified derivative: stream I/O goes through vox StreamCursorInterface
  shims (fgetc/fread/ftell/fseek), the state struct is 0x5a8 bytes, and there is a
  C++ build path. The README defines `upstream` as unmodified, so these rows do
  not qualify. The upstream stb_vorbis v1.22 was fetched to the scratchpad for
  comparison only and nothing from it is in the repo. This departs from the brief,
  which asked for `upstream`. Rows note "see NOTES.md". All 61 stb_vorbis-named
  queue entries are now ledgered this way (`skipped`).
- VoxEngine forwarders are `partial`. Each one calls `VoxEngineInternal`, which
  is declared but not defined. Functions that take a null engine return a default
  (documented per function in voxengine.hpp), for example the no-engine DataHandle
  is DataHandle(-1, nullptr, nullptr, 0, 0).
- VoxEngine facade vs instance. The port's VoxEngine forwarders are static. The
  original UpdateEmittersThreaded, UpdateThreaded, the constructor and the destructor
  take `this` (an object with a double timestamp at +0x10). Two thunks, 0x862d08
  (UpdateThreaded) and 0x862d0c (UpdateEmittersThreaded), are 4-byte `b` jumps to
  0x862cf0 and 0x862c4c. They are not ported, because their targets are instance
  methods the port does not have yet. The brief asks thunks to forward to the same
  target, so these two are left open rather than copied.
- AAPCS for 64-bit handle arguments: a 64-bit argument goes in an even register pair.
  The handle constructor's arguments (0, 0xffffffff, 0xffffffff, 0, 0, 0, 0) therefore
  mean padding in r1, id = -1 in r2:r3, a null engine and handlable, and stamps of 0.
- EmitterObj::IsAlive (0x8657f4) locks the mutex twice in the original (own lock,
  then ShouldDie). The port calls the unlocked body, because a second lock on a
  non-recursive mutex would deadlock. Marked partial.
- EmitterObj state callbacks. NeedToSendStateChangedCallback hands out a callback
  once (clears +0x98) and maps the state word: 0 or 3 -> 4, 1 -> 1, 2 -> 2, else 0.
  GetStatus reports state 1 as 2 when flag_6c_ is set, else 0x11 or 0x21 by the
  sub-state (+0x94). State 2 gives 2, states 0 and 3 give 4, others 0.
- VoxEngineInternal::Play/Stop (EmitterObj*) handle the priority bank membership:
  a bank entry that is already set is removed first; a rejected AddEmitter stops the
  emitter. Without a bank manager nothing starts.
- Driver defaults with no driver: GetOutputMode returns -1, SetOutputMode returns 0,
  and UpdateDSP, SetRoutingVolume and _SetDefault3DParameters are empty.
- Registry: RegisterStreamType and RegisterDecoderType return the slot index, or -1
  when all 31 slots are full. UpdateDebugServer is empty.
- mpc_seek returns 1 when the cursor seeks to the absolute position, 0 otherwise.
  mpc_canseek returns 1. mpc_getsize returns 0 (see Open questions).
- DecoderMSWavCursor's destructor deletes the sub-decoder it owns (virtual destructor,
  then VoxFree). The test places a FakeSub in malloc storage with placement new so
  that the cursor can free it safely.
- EmitterObj reproduces the ARM accessor quirks: SetLoop writes +0x8d but GetLoop
  reads +0x8c, and SetState writes +0x94 but GetState reads +0x90. Both are kept
  as the binary has them, and each accessor's header comment points here.
  IsReady returns (ready_flag_ ^ 1) != 0.
- DecoderRaw allocation: `VoxAlloc(sizeof(DecoderRawCursor))`. The original
  allocates 0x20 bytes on the 32-bit layout, but the 64-bit object is about 48
  bytes. The old size caused a heap overflow that aborted decoder_raw_test.
- StreamCursorInterface::ReadRef default calls Read(ref, size). The ARM at
  0x8886f8 loads *ref and calls vtable +0x1c with (this, ref, size).
- DecoderCursorInterface::Reset (0x86fb5c) calls the virtual Seek(0) and drops
  the result, as the original does.
- Mutex is a std::mutex wrapper (portable to MinGW). The copy constructor is
  deleted. Lock/Unlock keep the original lock-guard pattern.
- Mutex-wrapped accessors (Get3DGeneralParameteri and the like) have garbled
  pseudocode. Their bodies were read from the ARM listing (asm.py).
- StringCompare uses LessByBytes (memcmp over the shorter length, tie goes to
  the shorter string). c8stringcomp is ASCII case-insensitive, returning ca < cb.
- The test for stream_base uses placement new, an explicit destructor via
  `Destruct`, then `::operator delete`. Calling the destructor and then `delete`
  is undefined behaviour and segfaulted at -O2.
- IMA ADPCM (`ima_adpcm.hpp`): the step and index tables are the standard IMA
  values. The native binary is not in the container (recovered/native/ has only
  the debug, symbols and assembly directories), so the rodata tables are not
  confirmed. The algorithm and frame count are ported and tested, but the stream
  read is not. DecodeBlock is therefore partial.
- Cursor and stream factories (StreamMemoryBufferFactory, StreamCFileFactory)
  return raw VoxAlloc blocks. Their constructor calls are not visible in the
  pseudocode.
- StreamMemoryBuffer copy branch sets size to 0 after the copy, as the pseudocode
  shows. Not yet checked against the ARM.
- Stdio shims: fgetc returns the byte (0 at EOF), fseek discards the seek result.
  VoxAlloc out-of-memory reports through vox::SetOutOfMemoryHook with no stats dump.
- Decoder MSWav and Native classes are abstract. GetType returns 1 and 4. Their
  CreateNewCursor is not ported. DestroyCursor is a virtual destructor followed
  by VoxFree.
- SetUserData overloads take the first word of the record (EmitterObj) and the
  whole record (DataObj). The mangled EmitterObj symbol takes EmitterHandleUserData&,
  which does not match the port's `const uintptr_t&`. Left as is (see Open questions).

- Wave 6: VoxNativeSubDecoderPCM Reset, SetState and GetState are empty bodies (4-byte
  ARM, bare return), ledgered reconstructed. GetNbAvailableSamples returns 0 for a zero
  buffer_bytes_, where the original divides without a guard.

## Wave 7 (Hex-Rays as the primary source)

Source: `tools/decomp_extract.py --category vox-audio` defaults to Hex-Rays
(recovered/native/hexrays-all, commit 9476235b). ARM listings were used to confirm stores,
float helper calls and declared sizes. A Hex-Rays `int` return on a method the port
declares void is the Mutex::Unlock tail call: the original returns the unlock result and
the port drops it. Those bodies are marked partial (SetGroupGain, SetMasterGain, SetGain,
SetPitch, Resume).

Queue byte counts are not reliable. The ARM declared_size is the real size
(VoxMSWavSubDecoder::GoToNextDataChunk is 140 bytes, not 4).

Queue defect: all 237 FUN_ entries in queue.csv have elf_address exactly 0x10000 below the
address in their names (FUN_0089ece4 is listed at 88ece4). Resolved by name they are
libstdc++ (197, std::exception and locale), LCAndroidSocket (17), LCXPlayerSocket (17) and a
few runtime helpers. None is vox-audio code, so none is ledgered. They need re-queueing in
the owning category.

Layouts (offsets from the pseudocode):
- VoxGainFade (gain_fade.hpp): current +0x00, target +0x04, elapsed +0x08, duration +0x0c,
  flag +0x10; 20 bytes. Used for the master fade (VoxEngineInternal +0xe0), the group fades
  (from +0xf4, 20-byte stride) and three EmitterObj records.
- EmitterObj: raw gain +0x40 (requested_gain_); gain fade +0x48..+0x58 (target +0x4c);
  gain_ +0x3c is what GetGain returns (SetGain does not write it); pitch raw +0x74 (pitch_);
  pitch fade +0x78..+0x88 (target +0x7c, which also takes the raw pitch); resume fade
  +0x5c..+0x6c (target 1.0). The flag at +0x6c is what GetStatus tests in state 1, so
  flag_6c_ is now resume_fade_.flag. Interactive state: flag +0x144, name +0x146
  (port: std::string).
- VoxEngineInternal: master fade +0xe0 (target +0xe4); group fades +0xf4 + 20*i (target
  +0xf8 + 20*i); emitter access +0xc4; data objects access +0x54; data access +0x60;
  emitter pending access +0xd0.
- VoxMSWavSubDecoder base: stream +0x04, chunk +0x08, node +0x0c, channels +0x10, sample rate
  +0x14, bits +0x18, total frames +0x1c, node position +0x20, frame position +0x24, loop +0x28.
- DataNode {offset +0x00, size +0x04, next +0x08}; WaveChunk {channels +0x16, sample_rate
  +0x18, block_align +0x20, bits_per_sample +0x22, current_data_size +0x28, data_list +0x38}.
  Stand-ins until DecoderMSWavCursor::ParseFile is ported.
- VoxCallback: vtable +0x00 dtor, +0x08 Invoke. Ring node VoxCallbackNode {next +0x00,
  prev +0x04, callback +0x08}; the sentinel head is the manager.

Decisions:
- Fade durations. SetMasterGain and SetGroupGain store time + 0.001 (constant 0x3a83126f).
  EmitterObj SetGain, SetPitch and Resume store the time as given. RestartFadeWithDuration
  (gain_fade.hpp) is the exact form; RestartFade adds 0.001.
- Clamping. Master and group gains clamp to [0, 1]. EmitterObj::SetGain stores the raw value
  (a test keeps 3.0).
- Float helpers, from use and constants: 0x30e70c __aeabi_fcmplt (a < b), 0x30e2f8
  __aeabi_fcmpgt (a > b), 0x30ed6c __aeabi_fmul, 0x30ec94 __aeabi_fdiv, 0x30e3ac
  __aeabi_fsub, 0x30eba4 __aeabi_fadd (0x3a83126f is 0.001f), 0x30e904 __aeabi_idivmod,
  0x30e2a4 __aeabi_idiv. Division by zero returns 0 (the ARM default handler), as DivOrZero
  does in mswav_sub_decoder_pcm.cpp.
- lrand48 is ported (random48.cpp): X = (0x5DEECE66D * X + 0xB) mod 2^48, output X >> 17,
  srand48 sets (seed << 16) | 0x330E. Seeded sequences match host glibc (seed 1: 89400484,
  976015093). The unseeded state follows POSIX (0x1234ABCD330E). glibc starts from zero, so
  the unseeded values are not checked against the host.
- GetFreeEmitterObjectId and GetFreeDataObjectId return int64 (Hex-Rays shows the
  GetFreeHandleId type).
- Deleting destructors (D0) are compiler-generated. They are ledgered as reconstructed when the
  D1 they wrap is ported (VoxNativeSubDecoderPCM D0 0x8884f8).
- Musepack (mpc_*) is not ledgered; see the policy question below.

Why the remaining bodies are not reconstructed in this wave:
- Classes not in the port: VoxSoundManager (game glue at 0x36xxxx), CZipReader and
  SZipFileEntry, DecoderNativeCursor, VoxNativeSubDecoderIMAADPCM, DriverCallbackInterface,
  GroupXMLDef and EventXMLDef.
- Members the port lacks: NativePlaylist Reset, PeekAtNextElement, SetToPreviousState and
  GetState; NativePlaylistsManager AddPlaylistElement, AddGroup and TransposePlaylistParameters
  (they call element virtuals that are not ported); VoxSoundPackXML ResetEvent(int) and
  GetEventSoundUid(int, int&) (the int overloads); the VoxEngine singleton (GetVoxEngine,
  DestroyVoxEngine); the VoxThread object (ctor, Stop, Update, dtors); the
  DriverCallbackSourceInterface ctor, dtor and SetDriverCallbackPeriod (the static is not
  ported); EmitterHandleUserData (forward-declared only, so the emitter SetUserData overloads
  cannot take it).
- Unnamed DSP virtuals: EmitterObj::GetPlayCursor (source vtable +0x1c) and PrintDebug
  (+0x50), and SetPlayCursor (1382 bytes). The driver source vtable slots are not modelled.
- Large state machines: EmitterObj Play, Pause, Stop and Update; VoxEngineInternal
  UpdateEmitters, CreateEmitter, LoadDataSource, GetDataObject, DetachEmitterObject;
  PriorityBankManager::Update (2395 bytes) and AddEmitter (4839 bytes).
- IMA ADPCM family (ctor 0x8755b0, Decode 0x875cc8, Seek 0x875c34, HasData 0x875418): large,
  and the decoder class is not ported.

Policy question: the 52 mpc_* queue entries are Musepack (upstream libmpcdec) code. They are
not ledgered as skipped or upstream. Decide whether vox-audio records them as upstream rows.

Windows build (MinGW cross build, tests under Wine), final code: no compile errors or
warnings; `100% tests passed, 0 tests failed out of 30`.

## Open questions

- StreamCFile::Init and StreamCFileCursor::Init test the owner's +0x1c value and
  pass +0x1c to OpenFile. The port reads +0x1c as "path set" and opens the path.
  Confirm against the disassembly.
- StreamCFileCursor::Read and StreamMemoryBufferCursor::Read: the pseudocode stops
  after memcpy (the decompiler treats memcpy as non-returning). Position advance
  and return count are reconstructed. The original return is 0 on every visible path.
- StreamCFileCursor::Seek: the file Seek arguments are lost. The port seeks with
  whence 0 to the target.
- Several memcpy and idiv calls are marked non-returning, so tails of some
  functions are missing (ADPCM DecodeBlock, stream Read/ReadRef tails).
- Stream vtable slots +0x14 and +0x24 (see Layouts).
- DataObj: the duration helpers at 0x30e2e0, 0x30e964 and 0x30ec94 are not
  reconstructed. The DebugChunk_dataSource layout is also unknown, which blocks
  GetDebugInfo.
- DataObj virtual slots +0x08 and +0x0c are called by the data refcount helpers
  (Increase/DecreaseDataObjectRefCount). Those helpers are not in the port, so the
  DataHandle copy and destructor rows stay partial.
- GetDataObject (0x8686bc) and DetachEmitterObject (0x86a5d0) are not reconstructed.
  The data forwarders and KillEmitter(EmitterHandle&) call them. Both are declared only.
- GetFreeEmitterObjectId and GetFreeDataObjectId: resolved in wave 7 (int64, from
  Hex-Rays). Both are reconstructed.
- RandomGroup::GetActiveElementIndex uses lrand48. lrand48 is portable now (random48.cpp,
  wave 7). RandomGroup::GetActiveElementIndex itself is not ported.
- FileInterface and FileLimited read and seek go through unidentified global pointers
  (DAT_00a43eb4, DAT_00a43eb8) and FileSystemInterface::m_IOFunc. The owner of those
  globals is not identified.
- EmitterObj +0x6c is the flag of the resume fade record (+0x5c, wave 7). The owner of the
  buffer at +0xf4 is not identified.
- EmitterObj::UpdateDSP frees dsp_buffer_ without clearing it when +0xf0 is set. The
  original does the same, so a second call frees it again. Kept as is.
- SequentialGroup::GetGroupElementPosition returns -1 for an empty element vector; the
  original reads through a null vector. Partial.
- mpc_getsize needs the owning stream's size through the cursor's owner pointer.
  StreamCursorInterface has no owner slot in the port, so it returns 0 (partial).
- EmitterObj::SetUserData signature mismatch (see Decisions).
- Android JNI setters (VoxSetJavaVM, VoxSetDataThreshold, VoxSetAndroidAPILevel) are
  queued but not started. They belong with DriverAndroid.

- Float helpers (wave 6). The bl targets 0x30ed6c, 0x30e2f8, 0x30e70c, 0x30e9ac, 0x30e4cc,
  0x30e964, 0x30ec94, 0x30eba4, 0x30e2a4, 0x30e904 are PLT stubs, so symbols.csv has no
  names for them. 0x30ed6c is __aeabi_fmul: the 16384.0f argument (0x46800000) in
  SetDriverCallbackPeriod and SetGain fixes the meaning. 0x30e2a4 is __aeabi_idiv (Ghidra
  names it in GetNbAvailableSamples); 0x30e904 is taken as the remainder helper (inferred). 0x30e4cc
  (float to int) and 0x30e964 (int to float) are inferred from the Q14 scale. The two
  compare helpers (0x30e2f8, 0x30e70c) are not confirmed as eq or le. Wave 7 confirmed
  both from use (see Wave 7, Decisions). SetGain and SetPitch are partial only for the
  unlock result.
- GetPitch and GetGain: wave 5 used 2^-15. The ARM constant 0x38800000 is 2^-14, so wave
  6 corrected both (driver_callback.cpp and their ledger rows).
- DriverBuffer +0x10 is named field_10_ (the count GetNbAvailableSamples subtracts);
  +0x08 and +0x0c stay unidentified.


## Remaining work (675 unledgered of 1374 queued)

By class, as the queue names them (recounted at the end of wave 7):

- FUN_ entries: 237. Their queue addresses are 0x10000 low, and their real bodies are
  libstdc++ and socket code (see Wave 7). They are not vox-audio code. Re-queue them in
  the owning category; do not ledger them here.
- VoxSoundManager 52 (game glue: JNI, iPod playlist, glitch-side calls). Not started.
  The glitch:: entries belong in that category.
- VoxEngineInternal 44 (debug output, GetDataObject, DetachEmitterObject, update loops,
  SetInteractiveMusicStateChange) and VoxEngine 13 (instance methods, see Decisions).
- VoxSoundPackXML 39, DriverAndroid 7, DriverAndroidSource 6. Not started.
- EmitterObj 22: the fade state machine (Play, Pause, Stop, Resume, SetGain, SetPitch,
  Update, 3D debug helpers).
- DriverCallbackSourceInterface 22: GetWorkData, FillBufferStereo16, FillBufferMono16(NoInter),
  FillBuffer, UploadData, SetPitch, SetGain (partial, done), GetDopplerPitch, GetDistanceGain,
  GetDirectionalGain, GetStereoPanning, Set3DParameter(s), the constructors and dtors.
  Most float bodies call helpers that are not named (see Open questions).
- DriverCallbackInterface 12: its base class is not in the port yet, so its constructors
  and dtors wait for that header.
- Native sub-decoders: VoxNativeSubDecoderIMAADPCM 20, VoxNativeSubDecoder 18,
  VoxNativeSubDecoderPCM 8 (Seek-like and state methods still missing), VoxMSWavSubDecoderIMAADPCM 6.
  The native segment code (SegmentState, TransitionRule) is not ported.
- DecoderNativeCursor 16 and DecoderNative 11: the native cursor class is not in the port;
  DecoderNative has no CreateNewCursor, so it stays abstract.
- NativePlaylist 15 and NativePlaylistsManager 10: blocked. The original NativePlaylist
  is larger than the port layout (a vector of element pointers at +0x24..+0x2c, and
  AddGroup allocates 0x5c-byte group objects). Reset, SetState, SetToPreviousState and
  the manager copies call element virtuals (slots 0x10, 0x14, 0x18, 0x1c, 0x24) that
  are not ported.
- RandomGroup 14: needs lrand48 (see Open questions) and the list sentinel layout.
- SegmentGroup 2 and SequentialGroup 2: the copy constructors (0x88199c, 0x881a08,
  0x8839fc, 0x883ab8) need the GroupInfos-based copy, not yet traced.
- FileSystemInterface 11, FileSystemStdio 5, FileInterface 4, FileLimited 6: Read and Seek
  go through unidentified global pointers (see Open questions).
- CZipReader 10 and SZipFileEntry 4: the SAllocator string layout (+0x10, +0x18, +0x30)
  is not reproduced, and there is no zip reader in the port.
- DecoderMPC8Cursor 10, DecoderMPC8 2, DecoderStbVorbisCursor 8, DecoderStbVorbis 5:
  the cursors are not in the port. stb_vorbis is upstream (see Decisions).
- VoxUtils 6, VoxThread 7, vox 7 (the decoder factories and stdio shims),
  VoxCallbackManager 4, PriorityBankManager 3, EventXMLDef 3, GroupXMLDef 3, DataNode 1,
  StreamInterface 1.
- Musepack (not started): the mpc_* helpers (mpc_bits_*, mpc_demux_*, mpc_decoder_*,
  mpc_synthese_filter_float_internal, mpc_random_int, mpc_streaminfo_get_length*,
  mpc_get_encoder_string, mpc_set_replay_level) plus streaminfo_gain,
  streaminfo_encoder_info, streaminfo_read_header_sv7/sv8, huff_init_lut,
  huff_fill_lut and can_fill_lut.

## Wave 8 (parallel slices, Windows verified)

Fourteen workers each took one class slice (see ledger.parts/ for the per-slice ledgers, now
merged into ledger.csv). The Musepack rows were already upstream on the integrator branch, so
the duplicate rows from the musepack slice were dropped. The 237 FUN_ rows were re-queued by the
integrator (280fd865). Three FUN_ rows (0x8a0364, 0x8a2330, 0x8a7e90) are still unassigned.

Build: `tools/decomp_build.sh windows vox-audio` (MinGW-w64 under Wine) passes 45 of 45 tests.
MinGW and Wine were installed with apt on the integrator container.

Blocked groups, each needing a structural decision rather than another row:
- Driver source and decoder vtable slots (EmitterObj ctors, LoadAsync, Update, Update3D, the
  play-cursor getters): the port does not name these slots yet.
- PlaylistElement table at +0x30: the original is a PlaylistElement* vector, the port an int32
  table. Blocks AddPlaylistElement, the NativePlaylist dtor and copy ctor.
- Game glue for VoxSoundManager: 49 of 52 rows depend on JAVA_SOUNDS, the Arrays tables, menus and
  the native sound bridge.
- Native decoder cursor Seek, Decode and Rewind: need the sub-decoder Seek and Decode slots.
- VoxMSWav/MPC8 demux calls: libmusepack demux functions are declared but not in the port.

## Wave 9 (w9-mpc: DecoderMPC8 cursor and streaminfo rows)

Hex-Rays and the asm listing (decomp_asm.py) were checked against the port. Changes:
- Reader wrappers: the cursor constructor stores mpc_read, mpc_seek, mpc_tell, mpc_getsize and
  mpc_canseek at reader +0x00..+0x10 (0x87018c). mpc_reader_t now has typed slots (read, seek,
  tell, get_size, can_seek, data at +0x14).
- Frame block for mpc_demux_decode: samples at +0x00, the PCM buffer at +0x08. The Decode loop
  stores the PCM buffer there before each call (asm at 0x86ff24).
- Decode loop restart: the if-path seeks to the pre-update position (+0x20 read before the
  update). The else-path argument is clobbered on the ARM side, so it is not recoverable (partial).
- No new named vtable slots in this slice: the DecoderMPC8 virtuals are the DecoderInterface and
  DecoderCursorInterface slots already in the port. Slot offsets were not re-measured here.
- New test decoder_mpc8_cursor_demux_test drives the cursor through a fake demuxer.
- streaminfo_gain and streaminfo_encoder_info stay skipped (Musepack rule, name not confirmed).

## Wave 9: native decoder slice (w9-native-decoder)

Layouts (vptr-relative slot offsets, the same convention as the wave 1 to 4 entries above):
- `vox::DecoderNativeCursor` vtable: +0x00/+0x04 D1/D0, +0x08 Init, +0x0c Shutdown, +0x10
  Decode, +0x14 DecodeRef, +0x18 HasData, +0x1c Reset, +0x20 GetRewindLimit, +0x24 Rewind,
  +0x28 Seek, +0x2c SetLoop, +0x30 GetStreamCursor, +0x34 AllowBufferReference.
- `vox::DecoderNative` vtable: +0x00/+0x04 D1/D0, +0x08 Init, +0x0c Shutdown, +0x10
  CreateNewCursor, +0x14 DestroyCursor, +0x18 GetType, +0x1c GetParam.
- Sub-decoder slots the cursor calls (vptr-relative, VoxNativeSubDecoder and its PCM and IMA
  classes): +0x08 HasData, +0x0c Seek(unsigned), +0x10 Reset. The cursor reaches them through
  the concrete class picked by the format tag (decoder +0x24, 17 selects IMA ADPCM, else PCM),
  which is the class the original vptr would select. Decode, SetState(int) and
  EmulateDecode are direct (non-virtual) calls on VoxNativeSubDecoder.
- `vox::DecoderNative` format tag: halfword at +0x24, the first word of the 'fmt ' chunk body.
  Added as `format_tag_`. The original cursor reaches the header block through its own +0x20
  pointer (decoder +0x04); the port reads `decoder_` directly.
- Frame counters of the cursor (names as in the port): frames_before_ +0x58, frames_current_
  +0x5c, frames_capacity_ +0x60 (the cursor's frame limit, from 600 * channels * the second track
  word / 500 in the constructor), frames_mark_ +0x64, decode_calls_ +0x4c.

Decisions:
- Seek, Reset, Rewind and Decode are reconstructed with the sub-decoder class chosen by the
  format tag, not by a new virtual in VoxNativeSubDecoder (that header belongs to the
  native sub-decoder slice). If that slice adds the virtuals, these calls can switch to them.
- A rewind or a decode that uses the IMA state copies depends on IMA SetState and GetState,
  which are partial in their own slice. The cursor bodies are complete.
- The cursor and decoder destructors are reconstructed. The playlist manager destructor is
  partial in the playlists slice, so its body is what is still open.
- CreateTransitionRulesContainer and CreateSegmentsInfoContainers match the pseudocode line for
  line and are reconstructed. The 36-byte and 24-byte records are still stand-ins, as before.

Still partial (this slice):
- ParseFile: the chunk loop (Afmt at 0x746D6641, Segm at 0x6D676553, Cues at 0x73657543, then
  Grpe, Rule, Plst, Stat and end) is about 14 KB of pseudocode. Only the VoxN header is checked.
- The cursor C1 and C2: after parsing, the original creates the sub-decoder (PCM object 0x170
  bytes with states of 0x144 bytes, or IMA object 0x1c0 bytes with states of 0x150 bytes) and
  the playlist manager copy, then sets the track parameters and the frame capacity.
- SetImplicitSegmentCues: the decompiled structure pushes one value twice on the size-1 path
  (LABEL_7 then LABEL_3). Left empty until the assembly settles it.
- CreatePlaylistsContainer: the original returns NativePlaylistsManager::Init's result, which
  the port does not return.

## Wave 9: files-xml slice (SLICE w9-files-xml)

Rows: 23 (ledger.parts/w9-files-xml.csv). Reconstructed 12, partial 7, skipped 4 (decided from
the Hex-Rays body and the ARM listing via decomp_asm.py). Host build passes 47 of 47 tests
(new: sound_pack_event_pick_test, zip_path_strip_test). No MinGW or Wine run in this slice.

Layouts confirmed by the listing:
- `vox::StreamInterface` base: `Size()` returns the word at +0x04 (LDR R0,[R0,#4]). Still
  partial; the port's stream classes keep their own size_.
- `vox::FileInterface`: handle at +0x04. Seek calls slot (handle, offset, origin); Tell
  calls slot (handle). Both return -1 for a null handle or slot. The slots are
  off_A33EB4 and off_A33EB8 read through m_IOFunc.
- `vox::FileSystemInterface::CloseFile`: vtable slot +0x18 of the file is tested; a nonzero
  result calls off_A33EC0; then vtable slot 0, VoxFree, return 0.
- `vox::VoxSoundPackXML` event record (44 bytes): used list +0x08 (sentinel at +0x08),
  picked uids pool vector +0x10..+0x18, mode short +0x1c (0 random without repeat, 1 in
  order), max used short +0x1e, percent-no-sound short +0x20, mode 1 index short +0x22.
  ResetEvent stores the event record count in the +0x22 index (STRH R3, count).
- `VoxSoundPackXML` +0x48: string-keyed map (StringComp order) of group masks; the port
  member is group_masks_.
- `CZipReader::files_`: the map orders keys with StringComp (the port now uses it too).
  The entry data offset (+0x48 in the port's SZipFileEntry) and the method and uncompressed
  size in the local header are consistent with the listing's node offsets (+0x10 node
  header of 16 bytes plus the 24-byte key string).
- `CZipReader::deletePathFromFilename`: the string ends at +0x10 and starts at +0x14; the
  loop walks backwards from the end.

Still open in this slice:
- EventXMLDef (88a090, 88b104, 88b1fc) and SoundXMLDef (889e98) are loader records that
  are not in the port. The original copies the +0x28 pointer block shallowly and frees it
  in the destructor, so two copies would double-free. Not reproduced.
- LoadXML (88d344) and the VoxSoundPackXML constructors (88ec4c, 88ecc4) are partial: LoadXML
  is a stub, so a pack built from a file is empty. The TinyXML loader belongs with the
  TinyXML category.
- VoxSoundPackXML destructors (88c2cc, 88c36c) use the implicit destructor.
- FileSystemInterface::CloseFile needs the vtable slot +0x18 (not in the port).
- The owner of the FileInterface slots (off_A33EB4..off_A33EC0 through m_IOFunc) is still
  not identified.

## Wave 9 (w9-emitter-slots): driver and decoder vtable slots

Source of truth: `recovered/native/symbols/libDungeonHunter2.so/vtables-001.json`. Its
entries are symbol-relative, so the offsets below are vptr-relative (symbol offset minus 8).
Pure slots have no entry in the base vtable, so their names come from the concrete
overrides and from the emitter call sites.

- `vox::DriverSourceInterface` (emitter `dsp_source_` at +0x110): +0x08 Play, +0x0c Stop,
  +0x10 Pause, +0x14 Reset, +0x18 GetState, +0x1c GetByteOffset, +0x20 SetByteOffset,
  +0x24 NeedData, +0x28 UploadData, +0x2c SetGain, +0x30 SetPitch, +0x34 GetGain, +0x38
  GetPitch, +0x3c Set3DParameter, +0x40 SetDSPParameter, +0x44 Update, +0x48
  AllowBufferReference, +0x4c FreeDisposableData, +0x50 PrintDebug, +0x54 GetBufferCount,
  +0x58 Init, +0x5c Cleanup. The header declares them with empty defaults (the base slots are
  pure in the binary, and driver_test derives a plain source from the base).
- `vox::DriverInterface`: +0x08 Init(void*) and +0x0c Shutdown (named by DriverAndroid only,
  not declared), +0x10 Suspend, +0x14 Resume, +0x18 RampOut, +0x1c RampIn, +0x20 Update,
  +0x24 Set3DParameter, +0x28 SetDSPParameter, +0x2c SetStaticBusRouting, +0x30
  SetDynamicBusRouting, +0x34 SetSFXPresetActive, +0x38 SetOutputMode, +0x3c GetOutputMode,
  +0x40 PrintDebug, +0x44 CreateDriverSource and +0x48 DestroyDriverSource (DriverAndroid).
- `vox::DecoderCursorInterface` (the cursor at emitter +0x114): +0x08 Init, +0x0c Shutdown,
  +0x10 Decode (pure), +0x14 DecodeRef, +0x18 HasData (pure), +0x1c Reset, +0x20
  GetRewindLimit, +0x24 Rewind, +0x28 Seek (pure), +0x2c SetLoop, +0x30 GetStreamCursor,
  +0x34 AllowBufferReference. HasData moved to +0x18 in the header (it was last).
- `vox::DecoderInterface` (the decoder at DataObj +0x3c): +0x08 Init, +0x0c Shutdown, +0x10
  CreateNewCursor, +0x14 DestroyCursor, +0x18 GetType (1 MSWav, 3 MPC8, 4 Native), +0x1c
  GetParam. The header order was GetType, GetParam, CreateNewCursor, DestroyCursor; the
  slots are now commented and reordered to match.
- Call-site arguments in the Hex-Rays output are not reliable: the decompiler prints a
  leftover register as an argument (for example Reset is called with a float). Use the
  vtable names above, not the printed argument lists.

## Wave 9: engine-internal-a (VoxEngine facade, Suspend to SetAutoKillAfterDone)

Queue: 67 facade rows from Suspend/Resume through SetAutoKillAfterDone (the brief said 68; the
queue file holds 67 data rows). Every row is a tail call (B) to a VoxEngineInternal method, or a
null-guarded forward whose no-engine result is 0, -1 or 1 (IsEngineSuspended, IsDone, GetOutputMode).
The arguments pass through unchanged, including the stack arguments of SetRoutingVolume.

Vtable slots used by the facade (VoxEngineInternal, named in voxengine_internal.hpp):
+0x0c Suspend, +0x10 Resume, +0x14 IsSuspended, +0x18 UpdateSources, +0x24 SetGroupGain,
+0x28 SetMasterGain. The facade reads m_internal and calls through the vtable for these six.
All other targets are direct calls.

Decisions:
- SetOutputMode and GetAllEmitters now return the target's int (the original returns it); the
  facade declarations in voxengine.hpp changed from void to int, returning 0 without an engine.
- Partial (5 rows): SetMasterGain and SetGroupGain (the target returns the Mutex unlock result,
  which the void facade drops, as in wave 7); UpdateSourcesThreaded (target UpdateSources is
  skipped); GetDebugInfo (target skipped, DebugChunk layout unknown); SetInteractiveMusicState
  (target skipped).
- No test links the facade yet. voxengine.o references the three skipped targets and EmitterObj
  GetPlayCursor, SetPlayCursor, PrintDebug and Update, so any test that pulls it fails to link.
  Add a null-default test once those land.
## Wave 9 playlists (layouts and decisions)

Layouts (32-bit offsets, from the Hex-Rays bodies and the ARM listings):

- `vox::NativePlaylist`: +0x00 valid, +0x04 field_04 (the reset flag the walk tests),
  +0x08 total repeats, +0x0c current group, +0x10 current element, +0x14 remaining repeats,
  +0x18 previous group, +0x1c previous element, +0x20 saved repeats, +0x24..+0x2c the
  SegmentGroup* vector, +0x30..+0x38 the PlaylistElement* vector (changed in this wave from the
  int32 table).
- `vox::PlaylistElement` (0x14 bytes): +0x00 word 3 of the element info, +0x04 unset (0),
  +0x08 word 4, +0x0c word 5, +0x10 word 6 (the default is 1). The element accessors return the
  element pointer, as the original returns the pointer word.
- `vox::RandomGroupElement` (the record RandomGroup::AddElement stores): +0x00 value (element
  info word 1), +0x04 weight (element info word 7).
- `vox::NativePlaylistsManager`: +0x00 valid, +0x04 field_04, +0x08 count, +0x0c NativePlaylist*
  array.
- `vox::SegmentGroup` vtable slots (vptr-relative, the entry the ARM code calls with `ldr pc,
  [vptr, #n]`). Slot 0x00 D1, 0x04 D0, 0x08 GetGroupElementPosition(), 0x0c
  PeekAtNextGroupElement(GroupPeekMode), 0x10 Reset(int reset_mode), 0x14 SegmentGroup::SetState
  (SegmentGroup&), 0x18 SetToPreviousState(), 0x1c GetState(record), 0x20 SetState(group&),
  0x24 SetState(record). The derived classes fill 0x08 to 0x24 with their own functions
  (`vtables-001.json`, symbol offset + 8). Slot 0x10 has a return-type conflict in the port
  (SequentialGroup::Reset is void, RandomGroup::Reset is int), and slots 0x1c and 0x24 take
  different record types per kind, so the playlist code calls the group functions by the
  group's select mode instead of through SegmentGroup.

Decisions:

- The element table is a `std::vector<PlaylistElement*>`. Each element is a VoxAlloc block owned
  by its playlist (freed by the destructor, copied by the copy constructor).
- Accessors that return an element return `PlaylistElement*` (GetCurrentElement, the two
  GetPlaylistElement overloads, PeekAtNextElement, and the manager's GetPlaylistElement and
  PeekAtNextPlaylistElement). The original returns the element pointer in an int-typed function.
- Group dispatch: AddGroup builds a RandomGroup for a nonzero select mode and a SequentialGroup
  otherwise, so the select mode gives the dynamic type. The walk and state code cast on that
  basis; the calls reach the same functions as the vtable slots.
- Reset calls on each group pass 1 as the reset mode. The Hex-Rays body in the walk hides the
  argument; the ARM listing sets r1 to 1 before each call (0x88202c and 0x882124).
- A RandomGroup element record is copied to a heap RandomGroupElement owned by the group, as the
  original's AddElement copies its two-word argument.
- The NativeSubDecoderState constructor copies the manager argument (manager copy constructor).
  Its scalar defaults stay unreproduced (partial).

Build: `tools/decomp_build.sh host vox-audio` passes 46 of 46 (the 45 tests already on the branch
plus native_playlist_elements_test from this slice). MinGW and Wine
are not installed in this container, so the Windows build was not run.

Still open in this area: VoxNativeSubDecoder and the PCM and IMA ADPCM sub-decoder constructors
(skipped) need NativeChunks, States, AudioSegments, TransitionRules and the TransitionParams
vectors; those types are not ported.
