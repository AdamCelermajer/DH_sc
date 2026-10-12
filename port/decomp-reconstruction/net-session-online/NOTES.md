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

## CLobbyParameterAndQuery (session 2)

Layout (`include/net-session-online/c_lobby_parameter_and_query.hpp`), vtable `_ZTV23CLobbyParameterAndQuery`:
- +0x00 vptr, +0x04 parameter count, +0x14 array of owned value buffers, +0x18 inline int array of value lengths.
- PackParameter: count byte = number of non-empty parameters, then the non-empty values concatenated; returns nullptr when total length is 0.
- The original's ctor loads "oconf.bar" and parses it, then ends in a non-returning tail call. Only the not-found trace is kept (partial).
- AddQueryCondition and SetParameterValue: only the index guard is recovered (partial). Test seams Reserve and StoreParameter are not in the original.
- Deviation: the original frees values with scalar delete over new[] memory; the port uses delete[].

## Still not done

- GLXPlayerMPLobby::mpSend* (0x8376fc..0x83a744) need ConnectionLobby and GLXPlayerUserFriend stand-ins and the GLXPlayerMPLobby layout (+0x4, +0x48, +0x50, +0x60, +0x74, +0x80, +0x84). Not started.
- The rest of the queue (about 1100 functions) is unchanged.

## GLXPlayerMPLobby (session 2)

Layout is in `include/net-session-online/glx_player_mp_lobby.hpp`. Key offsets:
- +0x04 observer (GLXPlayerMPBase), +0x50 last result (-1 pending, 0x32 refused), +0x60 state.
- +0x74 ConnectionLobby (heap, 0x2058 bytes, time at +0x2050), +0x84 pending request kind, +0x80 pending flag, +0x81 master.
- Every mpSend* request checks state > 1 (0 and 1 are refused); a refusal sets 0x32 and calls observer vtable slot 0 (`OnInvalidState`).
- Request kinds at +0x84 are taken from the store instructions in each body; see the constants in the cpp.

Deviations and open questions:
- The closing `XP_DEBUG_OUT(&DAT_0091d6b0)` trace of each body is not reproduced (the string is not recovered).
- Argument types on the mpSend* wrappers follow the mangled symbol. Ghidra's own parameter types differ on some (for example the third argument of mpSendGetLobbyForFriend). Forwarding casts to the ConnectionLobby stand-ins follow the mangled names of the ConnectionLobby methods. Verify against the binary before relying on the exact widths.
- ConnectionLobby and GLXPlayerUserFriend are inline stand-ins in deps/ (no-op sends, friend count 0). They are not functional.
- The ConnectionLobby stand-in is not polymorphic (the original deletes it through a vtable slot).

Not done in this group: mpSendGetLobbyForFriend (friend-list overload, 0x837a88, 420 bytes), CreateLobby (0x8387a4), CreateLobbyWithGameCenter (0x83a744), Update (0x838a34), ProcessIncomingMessages (0x838c6c, 4492 bytes, skip), SendGetLobbyForFriend with GLXPlayerUserFriend, and the GLXPlayerMPLobby class members listed in the queue.
