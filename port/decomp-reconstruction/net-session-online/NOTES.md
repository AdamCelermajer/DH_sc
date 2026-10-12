# net-session-online notes

Status: 1268 functions queued. Ledger now has 152 rows (CEvent, CMessage, CEventQueueBase,
the CEventQueue<T,N> instances, and the fixed-size CMsg* types).

## Layouts (original 32-bit offsets, kept as comments)

CEvent (`include/net-session-online/c_event.hpp`), vtable `_ZTV6CEvent`:
- +0x00 vptr, +0x04 type, +0x08 payload pointer, +0x0c payload size, +0x10 online clock at Init.
- Init copies the payload only when data != 0, size != 0 and size is non-negative as a signed int.

CMessage (`include/net-session-online/c_message.hpp`), vtable `_ZTV8CMessage`:
- Header block, 16 bytes at +0x04: type (low byte on the wire), destMask (+0x08), sender (+0x0c), reserved (+0x10).
- +0x2c sendMode: 0 routes via the server, 1 is remote, >1 is never local.
- +0x30 reliable, +0x31 local, +0x33 includeLocalMember, +0x34 timestamp, +0x38 trackSent,
  +0x40 ackMask, +0x48 sentMask, +0x4c lifetime.
- The original also has a std::string at +0x14 (STL private layout). It is not modelled, so D1/D2 are partial.
- Virtual slots: +0x00 GetDataPtr, +0x04 unnamed (body not recovered), +0x08 GetDataSize,
  +0x0c ResetData, +0x10 WriteData, +0x14 ReadData, +0x20 Duplicate.

## Inferred CMatching interface (`deps/matching.hpp`)

Slot names are inferred from call sites; the vtable dump shows them as null/pure.
- +0x84 MemberBit(memberId) gives the bit for a member in a mask.
- +0x6c LocalMemberId(), +0x74 ServerMemberId().
- IsServer() and GetRoomMask() are non-virtual in the original. They are declared virtual here for testing.

## Decisions and deviations

- Deviation: CEvent copy-assignment nulls the freed payload pointer. The original left it dangling.
- Deviation: CMessage::CreateMessage returns nullptr for an out-of-range or unregistered type. The original calls through the slot.
- TerminateMessageRegistration stores 0 into the terminated flag, as decompiled. The name suggests 1. Kept as decompiled, and this is worth checking against the real binary.
- Unserialize reads the type id as a signed char, as the original does, so type ids of 128 and above map to negative slots.
- Several small getters are declared inline in the header. Their addresses are in the ledger notes.

## Not done / next

- CMessage::SetDefaultProperties (0x81a33c): mostly std::string internals. Not reconstructed.
- CEventQueueBase and the CEventQueue<...> templates: most ledger candidates decompile to a bare tail call into CNetMutex::Lock. This looks like identical-code folding or mislabeled thunks, not real bodies. Only HasEvents (0x7fde10) has a real body. Needs a check against the binary before it is ledgered.
- Still queued: CMsgDropLoot and CMsgQuestsSync, CMessaging, CSignIn*, GameCenter, MultiplayerCallback, lib-online-glx (GLXPlayer*, COnline*, Json::), item, channel, tFriendInfo/tRoomInfo.
- Stand-ins in deps/ are minimal. NetBitStream is byte-aligned only. The real implementation belongs to net-transport.
