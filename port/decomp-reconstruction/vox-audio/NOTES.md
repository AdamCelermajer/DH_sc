# vox-audio notes

Session scope: stream cursors and buffers, stdio-style shims, allocation entry
points, the mutex wrapper and AdpcmState. 64 ledger rows (42 reconstructed,
17 partial, 5 skipped). Queue total is 1374 functions; the rest is unstarted.

## Layouts inferred (32-bit offsets from the pseudocode, kept as comments)

- `vox::StreamCursorInterface` vtable: +0x08 Init, +0x0c Shutdown, +0x10 Seek,
  +0x14 EndOfStream (inferred), +0x18 Tell, +0x1c Read, +0x20 ReadRef,
  +0x24 IsSeekable (inferred), +0x28 AllowBufferReference. The stdio shims
  confirm +0x10, +0x18, +0x1c; the rest follow from tools/decomp_vtable.py output.
- `vox::StreamInterface` vtable: +0x10 Size.
- `vox::StreamMemoryBuffer`: +0x04 size, +0x08 data pointer, +0x0c owns byte.
  Its params block (`StreamMemoryBufferParams`): +0x00 data, +0x04 size,
  +0x08 explicit-owns flag, +0x09 owns byte.
- `vox::StreamMemoryBufferCursor` (12 bytes, VoxAlloc(0xc)): +0x04 owner,
  +0x08 position.
- `vox::StreamCFile`: +0x04 size, +0x18 path string, +0x20 file system.
- `vox::StreamCFileCursor` (0x801c bytes): +0x04 owner, +0x08 file handle,
  +0x0c position (-1 = unknown), +0x10 32 KiB read-ahead window,
  +0x8010 window fill, +0x8014 window index, +0x8018 window base offset.
- File handle (`vox::FileInterface`, stand-in): +0x08 Read(dst, size, count),
  +0x0c Seek, +0x10 Tell. File system (`FileSystemInterface`): +0x08 OpenFile,
  +0x0c CloseFile.
- `AdpcmState`: uint16 at +0x00 (initialized to 0xffff), byte at +0x02 (0).
  Meaning not established; decoder fields in VoxNativeSubDecoderIMAADPCM are
  at +0x1a0 onward, four-byte stride.

## Decisions

- Owns flag in StreamMemoryBuffer: owns defaults to true; when the explicit
  flag is clear, owns is the +0x09 byte (inverted in an early draft, caught by
  the test).
- Copy branch of the StreamMemoryBuffer constructor sets size to 0 after the
  copy (pseudocode shows this store). Kept as written. Worth checking against
  the ARM disassembly before relying on it.
- Stdio shims: fgetc returns the byte value even when Read returns nothing
  (0 at EOF), fseek discards the seek result, as the originals do.
- VoxAlloc out-of-memory path: reports through vox::SetOutOfMemoryHook; no
  statistics dump and no gAssertLevel behaviour.
- Mutex is a std::mutex wrapper rather than pthread (Windows MinGW portability).
  The copy constructor is deleted.
- Cursor and stream factories (StreamMemoryBufferFactory, StreamCFileFactory)
  return raw VoxAlloc blocks. Their constructor calls are not visible in the
  pseudocode.

## Open questions

- StreamCFile::Init and StreamCFileCursor::Init test the owner's +0x1c value
  and pass +0x1c to OpenFile. Since the path is a string at +0x18, +0x1c is read
  as "path set", and the open uses the path. Confirm against the disassembly.
- StreamCFileCursor::Read and StreamMemoryBufferCursor::Read: the pseudocode
  stops after memcpy (decompiler treats memcpy as non-returning). Position
  advance and return count are reconstructed. Original return value is 0 in
  every visible path.
- StreamCFileCursor::Seek: the file Seek arguments are lost; the port seeks with
  whence 0 to the target.
- The decompiler marks several memcpy/idiv calls as non-returning, so tails of
  some functions are missing (ADPCM DecodeBlock, stream Read/ReadRef tails).

## Not started

- ADPCM decoding (VoxNativeSubDecoderIMAADPCM::DecodeBlock and the state
  transitions): the pseudocode is 1008 bytes of per-channel pointer arithmetic
  with truncated tails. Needs the disassembly before a faithful port. The
  ADPCM decode test is therefore not written.
- stb_vorbis functions (get_stb_vorbis_struct_size and the stb_vorbis_*
  accessors). Identify the upstream version before ledgering them as upstream.
- vox::VoxEngine, DriverAndroid, DecoderNative/Playlist, SoundPack XML,
  Handle/DataObj, RandomGroup/SequentialGroup, PriorityBankManager, CZipReader
  and the rest of the 1300+ queued vox:: functions.
