# net-transport notes

Low-level multiplayer networking. Queue: 794 functions (`queue.csv`). Ledger: 365 rows
(259 reconstructed, 78 partial, 28 skipped); 429 queued functions are still unledgered.
Wave 3 added 177 rows (139 reconstructed, 25 partial, 13 skipped).

## Classes completed

Earlier waves:

- `Buffer` (6 rows): block-chunked byte store, `include/net-transport/buffer.hpp`.
- `CMutex`, `CSemaphore`, `CReadWriteLock`, `CNetMutex`, `Thread` (sync shells):
  `include/net-transport/sync.hpp`.
- `NetBitStream` (28 rows): MSB-first bit stream, `include/net-transport/net_bit_stream.hpp`.
- `NetBitStreamArray` (7 rows).
- `NetStructUInt<1|5|8|32>`, `NetStructBool`, `NetStructRangedFloat<..>`,
  `NetStructNetworkId`: `include/net-transport/net_struct.hpp`.
- `CNetworkId` (7 rows): `include/net-transport/net_network_id.hpp`.
- `CNetworkEmulation` (23 rows) and `CNetLog` (4 rows).
- `CAndroidSocket` (36 rows): see its own section below.

Wave 3 (all rows of the class are in the ledger):

- `StreamBuffer` (21 rows), `BufferedStream` (1 row): `stream_buffer.hpp`, `buffered_stream.hpp`.
- `DataPacket` (8 rows): virtual base with no-op addBlob/getBlob.
- `CTransport` (6 rows): abstract base, `transport.hpp`.
- `CSocketAbstract` (3 rows), `CSocket` (24 rows): `socket_abstract.hpp`, `csocket.hpp`.
- `CTcp` (20 rows): `ctcp.hpp`.
- `CBluetoothSocketAbstract` (9 rows), `CBluetoothSocket` (21 rows): `bluetooth_socket.hpp`.
- `updateJob_thread` (13 rows): `update_job_thread.hpp`.
- `NetStructInterpolation<float>` destructors (2 of 3 queued rows), see Remaining.

Partly done in wave 3 (the rest is listed under Remaining):

- `CUdp` (15 of 16): everything except `CUdp::Receive`.
- `CBluetoothReliable` and `CBluetoothUnreliable` (15 of 30 each): the leaf virtuals
  and accessors are in `bluetooth_transport.hpp`. The constructors, destructors,
  Create, InitializeTransport, Open, Listen, Connect, AcceptConnections,
  GetPeerNetworkId and GetLocalNetworkId are not done.
- `NetStructInterpolatedRangedFloat<Bits,Min,Max>` (4 of 8): destructors done, see Remaining.

## Inferred layouts

NetBitStream (32-bit offsets): +0x04 buffer, +0x08 size (bytes), +0x0C read bit pos,
+0x10 write bit pos, +0x14/+0x18 saved read/write pos (SetRevertPoint),
+0x1C flags (bit0 read overflow, bit1 write overflow). Bits are MSB first.
`ReadByte` checks available data against the write position, so a receiver must
fill the stream with WriteStream/WriteByteArray before reading. SetBuffer alone
does not make data readable.

Buffer: +0x00 block size (default 0x800), +0x04..0x0C block vector (with a trailing
nullptr sentinel), +0x10 size, +0x14 "sized" flag. Accessors `block_size()` and
`FirstBlock()` were added for StreamBuffer.

CNetworkId: +0x00 address0 and +0x08 address1, each 8 bytes = {u16 lo, pad, u32 hi}
(a 48-bit address), +0x10 and +0x14 u32 fields, +0x18 presence flags
(bit0 = address1, bit1 = address0, bit2 = field2, bit3 = field3). Transport N
corresponds to presence bit N-1. Accessors SetAddress0/SetAddress1/GetAddress0/GetAddress1
were added for the transports.

Thread: vtable +0x08 Init, +0x0C Step (the loop body), +0x10 Cleanup;
+0x04 worker handle, +0x08 stop flag.

CNetworkEmulation: sets at +0x1C with stride 0x24, two sets, 9 fields each
(delay, jitter, loss rate, loss param, duplicate rate, reorder rate, two int
reorder params, one float reorder param). Peak timer at +0x08..+0x18.

StreamBuffer (implements IStreamBase): +0x08 read position and +0x10 write position
(64-bit in the port: low word at +0x08 and +0x10, high word at +0x0c and +0x14), embedded
Buffer at +0x18 (block size +0x18, block vector +0x1c, size +0x28, sized flag +0x2c).
The 64-bit position model is a port decision; the 32-bit offsets stay as comments.

BufferedStream: +0x00 data (new[] of the source size), +0x04 size.

DataPacket: vtable +0x4c addBlob and +0x50 getBlob (no-op virtuals in the port);
GetMessageType and GetMMOMessageType return -1.

CTransport (abstract): vtable +0x08 Connect, +0x14 Disconnect, +0x30 StartBroadcast,
+0x34 StopBroadcast.

CSocketAbstract: +0x04 handle (int, -1 when closed), +0x08 valid flag (set by Assign),
+0x0a connected flag (copied from IsConnected() by Assign). Accessors Handle(), IsValid()
and IsConnectedFlag() are not in the original; the tests use them.

CSocket (on CSocketAbstract): +0x09 can-open flag (1 until a socket is opened; OpenUDP and
OpenTCP check it), +0x0b listening flag (set by Listen), +0x0c peer address (in_addr,
set by Accept), +0x10 peer port (host order, set by Accept).

CUdp: +0x00 vptr (CTransport base), +0x04 create flags (the argument of CreateByPort),
+0x08 CSocket (its valid byte is the +0x10 byte that CUdp tests). Static
`CUdp::s_initialized_` (`_ZN4CUdp13s_initializedE`).

CTcp: +0x00 vptr, +0x04 create flags, +0x08 CSocket (valid byte at +0x10, listening
byte at +0x13). Static `CTcp::s_initialized_`.

CBluetoothSocketAbstract: std::string device_name_ at +0x04; the virtuals return
constants, as the original stubs do.

CBluetoothSocket: all original stubs, reproduced as constant returns.

CBluetoothReliable (GetType 4) and CBluetoothUnreliable (GetType 3): +0x08 CBluetoothSocket,
+0x2c flags (bit0 = listening), +0x68 initialized flag (set by Initialize, cleared by
Disconnect), static `s_initialized_`. IsListening = initialized_ && socket_.IsServer() && (flags_ & 1).

updateJob_thread: +0x00 vptr, +0x04 std::thread (pthread_t in the original), +0x08 flag_
(set to true by the constructor), +0x0c argument_. Virtuals: Prepare +0x08,
Kernel +0x0c, Finish +0x10.

NetStructInterpolation<T> and NetStructInterpolatedRangedFloat<Bits,Min,Max>: header
templates with empty virtual destructors. The cpp holds the explicit instantiations.
The classes are abstract, so they cannot be constructed in tests. The interpolated
range-float constructor and PostLoad are not reconstructed yet.

## Socket layer (wave 3)

`include/net-transport/socket_platform.hpp` (namespace `net_platform`): NativeSocket,
kErrWouldBlock, kErrIsConnected, kErrInProgress, CloseSocket, LastError,
SetNonBlocking (fcntl on POSIX, ioctlsocket on Windows). The socket headers use
std::uint32_t where the original uses in_addr_t (not defined under Winsock), and htons/ntohs
for byte order.

Socket options (CSocket::SetSocketOptions, bit mask): bit1 SO_REUSEADDR, bit0 SO_BROADCAST,
bit2 TCP_NODELAY. OpenTCP and OpenUDP do not check the result of SetSocketOptions. The
pseudocode shows no check, so the port does not add one (partial).

Accept sets O_NONBLOCK on the listening handle and records the peer. The accepted handle is
returned; the original does not store it.

GetLocalAddress uses SIOCGIFCONF on POSIX. Winsock has no SIOCGIFCONF, so it uses
gethostbyname instead (partial). The probe socket is closed on every path.

## CAndroidSocket (36 rows, session 2)

Files: `include/net-transport/android_socket.hpp`, `src/android_socket.cpp`,
`tests/android_socket_test.cpp` (loopback UDP send/poll/recv). One source file serves
both platforms: POSIX headers, or winsock2 under `_WIN32` (Startup/Clearup call
WSAStartup/WSACleanup there).

Layout (32-bit offsets from the original): +0x04 status_ (7 = error), +0x08 fd_ (-1 when
closed), +0x0c host_ (hostent*, set by ConnectByName), +0x10 port_ (host byte order),
+0x820 socket_kind_ (1 TCP, 2 UDP/ICMP), +0x85c conn_state_ (0 idle, 1 in progress,
2 done, 3 failed), +0x860 worker_state_ (1 = lookup busy, disables the host cache),
+0x864 connect_start_ms_, +0x868 lookup_start_ms_, +0x86c ready_bits_[32] (the fd_set).
Clock is steady_clock truncated to int ms, like XP_API_GET_TIME. Timeouts: connect 30001 ms,
lookup 9999 ms.

Host cache: static array of 4 entries (l_host/l_count). Entries are deep copies made
from the system hostent. The ARM listing (`CAndroidSocket-fbded90aa208-001.asm`, at
0x841ca8) confirms the cache-hit path and the "return last entry when full" behaviour.

Not done here: the GLXPlayerSocket base (owned by net-session-online), its virtual
slots (the original calls them through the vtable; here they are direct calls), the
worker thread join in the destructor, and the object construction inside Accept().
Winsock SOCKET values are stored in an int, which assumes small handle values.

Bugs reproduced and flagged: Bind discards inet_addr() so it always binds INADDR_ANY;
Bind's port-retry loop has no exit if no port works; ConnectByName ignores its port
argument and uses port_.

## Decisions and original quirks (reproduced, flagged for review)

- Debug ASSERT and XP_DEBUG_OUT logging is dropped (gAssertLevel, debug prints).
  Functions that lose that output are marked `partial`. Wave 3 examples: the sized-flag
  ASSERT in the StreamBuffer(IStreamBase) constructors, the static printed counter in
  CSocket::Receive, and the error logs in CSocket.
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
  worker, which the original does not do. updateJob_thread does the same: its
  destructor joins a running worker, and Start/Start2 fail while a worker runs.
- CSocket::SendTo returns the sendto result. The decompile shows no return value (partial).
- CTcp::IsConnectedTo returns GetPeerNetworkId().IsEqual(peer). The decompile does not
  show the return value, so this is inferred. CTcp::CanDisconnectFrom assumes its tail call
  passes the result through. CTcp::Send assumes IsConnectedTo(peer) because the decompiled
  check is a virtual call with no arguments (all three partial).
- CTcp::ReceivePackets reads up to 21 packets of 4096 bytes. CUdp::InitializeTransport
  calls AddTransport and CTcp calls AcceptCallback and AddTransport on the CTransportManager
  stand-in (deps/transport_manager.hpp). Those calls are no-ops for now.
- BufferedStream::FillBuffer stops on a zero-byte read instead of spinning (partial).
- No Bluetooth on Windows: CBluetooth* OS calls are constant-return stubs, as the
  original stubs are. The port keeps the logic and the constant results.
- Deleting destructors (D0) are compiler-generated and are marked `skipped`.

## Test coverage

tests/ (14 executables, all run by ctest): stream_buffer_test, transport_test,
csocket_test, udp_tcp_test, bluetooth_job_test, bluetooth_transport_test, buffer_test,
sync_test, net_bit_stream_test, net_bit_stream_array_test, net_struct_test, net_log_test,
net_network_emulation_test, android_socket_test. An interpolation test was tried and
removed, because the templates are abstract.

## Known Ghidra artifacts (left for ARM listing review)

- Noreturn CNetMutex::Lock tails truncate some pseudocode bodies.
- Some symbol ranges are misaligned: CConnectionManager (KeepAliveConnections,
  sReceiverCallback, Update, TerminateInternal). Not reconstructed from the pseudocode
  until checked against the ARM listing.

## Remaining (429 functions unledgered)

- Connection and transport managers: CConnectionManager (56), CConnection (26),
  Connection (35), ConnectionLobby (48), DataPacketLobby (50), DefaultDataPacket (42),
  CPacketManager (33), CTransportManager (35), CTransportManagerImpl (9),
  CTransportFactory (3). CConnectionManager and CConnection need CMatching and
  CEventQueueBase stand-ins in deps/. CTransportManager is a no-op stand-in for now.
- LCAndroidSocket (36): same socket layer as CAndroidSocket. Not started.
- NetStructArray<CMatching::MemberInfoNetStruct, 32> (6) and
  NetStructArray<CMatchingLocal::ServerBackupNetStruct, 32> (6): vtable-driven
  (member size 0x36 words and 0x2B words).
- NetStructInterpolatedRangedFloat (4): <18,-200000,200000> and <16,-50000,50000>
  constructors (240 bytes each) and PostLoad (16 bytes each).
- NetStructInterpolation<float>::AddValue (1).
- CUdp::Receive (1, 340 bytes).
- CBluetoothReliable and CBluetoothUnreliable (15 each): constructors, destructors,
  Create, InitializeTransport, Open, Listen, Connect, AcceptConnections, GetPeerNetworkId,
  GetLocalNetworkId.
- CBluetoothNetRoomInfo (3): constructor and destructors.
- CDataStats<int> (5): GetAverage, GetSum, AddData, destructors.
- CTransportManagerImpl (9): GetHostname, GetLocalhostname, receiver thread start/stop.
