# vox-audio notes

Status after wave 6. The ledger has 631 data rows: 395 reconstructed, 171 partial,
65 skipped. Wave 6 added 92 rows (from 539 at the end of wave 5). The queue has 1374
functions: 631 ledgered, 743 remain.

Builds: `tools/decomp_build.sh host vox-audio` (23 tests, all pass) and
`tools/decomp_build.sh windows vox-audio` (MinGW cross build, tests under Wine, 23 of 23
pass). Each category is one static library (port/decomp-reconstruction/CMakeLists.txt),
so a source that calls a declared-but-undefined function only fails in a test that links
it. No test links the VoxEngineInternal or VoxEngine sources yet.

Tests (23): access_controller, data_obj, decoder_cursor_base, decoder_mswav_cursor,
decoder_raw, driver, driver_android, driver_callback, emitter_obj, file_stdio,
group_ctor, groups, handlable, handle, ima_adpcm, native_playlist, native_sub_decoder,
priority_bank, state_mutex, stream_base, stream, string_compare, vox_thread.

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
- GetFreeEmitterObjectId and GetFreeDataObjectId: the pseudocode types the return as
  undefined4, while GetFreeHandleId returns int64. Not written.
- RandomGroup::GetActiveElementIndex uses lrand48 (POSIX, absent on MinGW). Not written
  until a portable replacement is chosen.
- FileInterface and FileLimited read and seek go through unidentified global pointers
  (DAT_00a43eb4, DAT_00a43eb8) and FileSystemInterface::m_IOFunc. The owner of those
  globals is not identified.
- EmitterObj +0x6c flag meaning and the owner of the buffer at +0xf4 are not identified.
- EmitterObj::UpdateDSP frees dsp_buffer_ without clearing it when +0xf0 is set. The
  original does the same, so a second call frees it again. Kept as is.
- SequentialGroup::GetGroupElementPosition returns -1 for an empty element vector; the
  original reads through a null vector. Partial.
- mpc_getsize needs the owning stream's size through the cursor's owner pointer.
  StreamCursorInterface has no owner slot in the port, so it returns 0 (partial).
- EmitterObj::SetUserData signature mismatch (see Decisions).
- Android JNI setters (VoxSetJavaVM, VoxSetDataThreshold, VoxSetAndroidAPILevel) are
  queued but not started. They belong with DriverAndroid.

## Remaining work (743 unledgered of 1374 queued)

By class, as the queue names them (remaining bodies, recounted at the end of wave 6):

- FUN_ entries (no symbol, decompiler-named): 237. Mostly in the 0x88xxxx-0x89xxxx
  range. Owners need identifying before these can be named.
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
