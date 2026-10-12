# vox-audio notes

Status after wave 4. The ledger has 339 data rows (reconstructed 167, partial
108, skipped 64). Wave 4 added 201 of them. The queue has 1374 functions, of
which 339 are ledgered and 1035 remain.

Builds: `tools/decomp_build.sh host vox-audio` and `tools/decomp_build.sh windows
vox-audio` both compile with no warnings. All 15 tests pass on both.

Tests: access_controller, data_obj, decoder_cursor_base, decoder_raw,
emitter_obj, groups, handlable, handle, ima_adpcm, priority_bank, state_mutex,
stream_base, stream, string_compare, vox_thread.

## Class coverage

Every queued method is ledgered for these classes:

- Fully reconstructed: AdpcmState (4), HandlableContainer (11), StreamMemoryBuffer (8),
  StreamMemoryBufferCursor (9).
- All ledgered, with some rows partial: DecoderCursorInterface (10 reconstructed,
  1 partial), StreamCFileCursor (8 + 1 partial), StreamCFile (7 + 1 partial),
  DataObj (7 + 8 partial).
- Mutex: 7 reconstructed, 3 skipped (decompiler thunk, and a deleted copy constructor).

Partly done:

- VoxEngine: 91 of 111. 85 are partial forwarders (see below). 6 are empty bodies
  reconstructed as they are.
- EmitterObj: 11 of 60. DecoderNative 3 of 14, DecoderMSWav 3 of 9, FileLimited
  3 of 10, PriorityBankManager 10 of 13, SequentialGroup 10 of 15, SegmentGroup
  6 of 10, RandomGroup 2 of 16, VoxThread 2 of 9, VoxNativeSubDecoderIMAADPCM 1 of 21.

Not started: VoxEngineInternal (123 methods, declared only), see the remaining
list below.

## Layouts inferred (32-bit offsets from the pseudocode, kept as comments)

- `vox::StreamCursorInterface` vtable: +0x08 Init, +0x0c Shutdown, +0x10 Seek,
  +0x18 Tell, +0x1c Read, +0x20 ReadRef, +0x28 AllowBufferReference. Slots
  +0x14 and +0x24 are pure virtuals with no symbol. The header calls them
  IsSeekable and EndOfStream respectively. The earlier notes had them the other
  way round. Neither assignment is confirmed by a listing.
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

## Decisions

- stb_vorbis is ledgered `skipped`, not `upstream`. The binary's copy is a
  Gameloft-modified derivative: stream I/O goes through vox StreamCursorInterface
  shims (fgetc/fread/ftell/fseek), the state struct is 0x5a8 bytes, and there is a
  C++ build path. The README defines `upstream` as unmodified, so these rows do
  not qualify. The upstream stb_vorbis v1.22 was fetched to the scratchpad for
  comparison only and nothing from it is in the repo. This departs from the brief,
  which asked for `upstream`. Rows note "see NOTES.md".
- VoxEngine forwarders are `partial`. Each one calls `VoxEngineInternal`, which
  is declared but not defined. A test that links vox_engine.o would fail, so no
  VoxEngine test exists yet. Functions that take a null engine return a default
  (documented per function in voxengine.hpp).
- DecoderRaw allocation: `VoxAlloc(sizeof(DecoderRawCursor))`. The original
  allocates 0x20 bytes on the 32-bit layout, but the 64-bit object is about 48
  bytes. The old size caused a heap overflow that aborted decoder_raw_test.
- StreamCursorInterface::ReadRef default calls Read(ref, size). The ARM at
  0x8886f8 loads *ref and calls vtable +0x1c with (this, ref, size).
- DecoderCursorInterface::Reset (0x86fb5c) calls the virtual Seek(0) and drops
  the result, as the original does.
- EmitterObj reproduces the ARM accessor quirks: SetLoop writes +0x8d but GetLoop
  reads +0x8c, and SetState writes +0x94 but GetState reads +0x90. Both are kept
  as the binary has them, and each accessor's header comment points here.
  IsReady returns (ready_flag_ ^ 1) != 0.
- Mutex is a std::mutex wrapper (portable to MinGW). The copy constructor is
  deleted. Lock/Unlock keep the original lock-guard pattern.
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

## Remaining work (1035 unledgered)

- VoxEngineInternal: 123 methods. The class has no bodies yet, so every VoxEngine
  forwarder stays partial until it does.
- Emitter and handle types: EmitterObj (49 left), EmitterHandle (8), DataHandle (8).
- Driver and sound classes: VoxSoundManager (52), DriverCallbackSourceInterface (40),
  VoxSoundPackXML (39), DriverAndroid (25), DriverCallbackInterface (12),
  DriverInterface (11), FileSystemInterface (11).
- Playlists and archives: NativePlaylist (19), NativePlaylistsManager (18), CZipReader (10).
- Decoders: VoxNativeSubDecoder (27), VoxNativeSubDecoderPCM (18),
  VoxNativeSubDecoderIMAADPCM (20), DecoderNativeCursor (16), DecoderMSWavCursor
  (10), VoxMSWavSubDecoderIMAADPCM (9), VoxMSWavSubDecoderPCM (8), DecoderRawCursor
  (9), DecoderRaw (8), DecoderStbVorbisCursor (8), DecoderNative (11 left), DecoderMSWav (6 left).
- Musepack: DecoderMPC8 (8) and DecoderMPC8Cursor (10), about 46 mpc_* helpers, and
  7 C-named helpers (streaminfo_read_header_sv7/sv8, streaminfo_gain,
  streaminfo_encoder_info, huff_init_lut, huff_fill_lut, can_fill_lut). Not started.
- stb_vorbis-derived: 16 queued entries not yet ledgered. Apply the same skipped
  decision and note it.
- Unnamed FUN_ entries: 237 remain. These are decompiler-named helpers without
  a symbol, mostly in the 0x88xxxx-0x89xxxx range, and need their owners identified first.
- glitch:: entries: 5 queued (for example VoxSoundManager::ConvertVisual3DToSound3D).
  Their types live in the glitch category, so they may belong there.
- Groups and random: RandomGroup (14 left), SequentialGroup (5 left), SegmentGroup (4 left).
- FileLimited (7 left), PriorityBankManager (3 left), VoxThread (7 left).
