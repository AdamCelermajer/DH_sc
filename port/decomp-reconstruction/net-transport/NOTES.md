# net-transport notes

Low-level multiplayer networking. Queue: 794 functions (`queue.csv`). This session
completed 152 ledger rows (110 reconstructed, 28 partial, 14 skipped thunks).

## Classes completed

- `Buffer` (6 rows): block-chunked byte store, `include/net-transport/buffer.hpp`.
- `CMutex`, `CSemaphore`, `CReadWriteLock`, `CNetMutex`, `Thread` (sync shells):
  `include/net-transport/sync.hpp`.
- `NetBitStream` (28 rows): MSB-first bit stream, `include/net-transport/net_bit_stream.hpp`.
- `NetBitStreamArray` (7 rows).
- `NetStructUInt<1|5|8|32>`, `NetStructBool`, `NetStructRangedFloat<..>`,
  `NetStructNetworkId`: `include/net-transport/net_struct.hpp`.
- `CNetworkId` (7 rows): `include/net-transport/net_network_id.hpp`.
- `CNetworkEmulation` (23 rows) and `CNetLog` (4 rows).

## Inferred layouts

NetBitStream (32-bit offsets): +0x04 buffer, +0x08 size (bytes), +0x0C read bit pos,
+0x10 write bit pos, +0x14/+0x18 saved read/write pos (SetRevertPoint),
+0x1C flags (bit0 read overflow, bit1 write overflow). Bits are MSB first.
`ReadByte` checks available data against the write position, so a receiver must
fill the stream with WriteStream/WriteByteArray before reading. SetBuffer alone
does not make data readable.

Buffer: +0x00 block size (default 0x800), +0x04..0x0C block vector (with a trailing
nullptr sentinel), +0x10 size, +0x14 "sized" flag. Matches the embedded Buffer at
+0x18 inside StreamBuffer (not yet reconstructed).

CNetworkId: +0x00 address0 and +0x08 address1, each 8 bytes = {u16 lo, pad, u32 hi}
(a 48-bit address), +0x10 and +0x14 u32 fields, +0x18 presence flags
(bit0 = address1, bit1 = address0, bit2 = field2, bit3 = field3). Transport N
corresponds to presence bit N-1.

Thread: vtable +0x08 Init, +0x0C Step (the loop body), +0x10 Cleanup;
+0x04 worker handle, +0x08 stop flag.

CNetworkEmulation: sets at +0x1C with stride 0x24, two sets, 9 fields each
(delay, jitter, loss rate, loss param, duplicate rate, reorder rate, two int
reorder params, one float reorder param). Peak timer at +0x08..+0x18.

## Decisions and original quirks (reproduced, flagged for review)

- Debug ASSERT and XP_DEBUG_OUT logging is dropped (gAssertLevel, debug prints).
  Functions that lose that output are marked `partial`.
- CSemaphore Wait/Release and CReadWriteLock methods are empty in the original. They
  are kept as no-ops (`partial`), so they are not real synchronisation. Real
  primitives need a decision before the game runs multi-threaded.
- NetBitStream copy constructor and operator= do not store size, so a copied
  stream reports size 0 (copy ctor). operator= keeps the old size after a
  reallocation. Both are reproduced and marked `partial`.
- NetBitStream::operator== returns the raw memcmp value in the original (inverted).
  The intended equality is implemented.
- WriteByte keeps the original `avail > 8` condition, so the last 8 bits of a
  stream cannot be written with WriteByte.
- Decompile dropped call arguments in a few places (WriteU32's per-byte width,
  the ShiftMemCopy arguments, SetNetConditionPreset's default branch). Those are
  inferred and marked `partial`.
- CNetworkId::IsEqual compares sums of present fields in two passes (as the
  original). Two ids compare equal when they share a present field and their sums
  match.
- CNetworkEmulation's time source (GetOnline() vtable slot 0) is replaced by
  steady_clock, and lrand48 is emulated in-file.
- std::thread replaces pthread_create/join; the destructor of Thread joins a running
  worker, which the original does not do.

## Remaining (642 functions queued, not started)

- Socket layer: CAndroidSocket (36), LCAndroidSocket (36), CSocket (24),
  CSocketAbstract (3), CTcp (20), CUdp (16), CTransport* , CTransportFactory.
  These need Winsock on `_WIN32` and POSIX sockets otherwise.
- Bluetooth: CBluetoothReliable/Unreliable/Socket/SocketAbstract (~100). Logic
  reconstruction only, with OS calls guarded and marked `partial`.
- Connection and transport managers: CConnectionManager (56), CConnection (26),
  Connection (35), ConnectionLobby (48), DataPacketLobby (50), DefaultDataPacket
  (42), DataPacket (8), CTransportManager (35), CPacketManager (33).
- StreamBuffer (21): depends on IStreamBase virtual slots (+0x08 size,
  +0x14 read, +0x18 read-into-buffer, +0x20 seek) and the 64-bit position model.
- NetStructArray<CMatching::MemberInfoNetStruct> and <CMatchingLocal::...>
  (12): vtable-driven (member size 0x36 words and 0x2B words).
- NetStructInterpolation / NetStructInterpolatedRangedFloat (8).
- CDataStats<int> (5).
- updateJob_thread (13), CTransportManagerImpl (9),
  CBluetoothNetRoomInfo (3).
