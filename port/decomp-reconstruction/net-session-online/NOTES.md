# net-session-online notes

Status: 1268 functions queued. Ledger has 396 rows (CEvent, CMessage, CEventQueueBase,
the CEventQueue<T,N> instances, the fixed-size CMsg* types, and the GLX player,
MultiplayerStateMachine and IEvent classes from wave 3).

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
- Still queued: CMessaging, CSignIn*, GameCenter, MultiplayerCallback, lib-online-glx (GLXPlayer*, COnline*, Json::), item, channel, tFriendInfo/tRoomInfo.
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

## GLXPlayerMPBase, observer and GLXPlayerUserFriend getters (session 2)

- `include/net-session-online/glx_player_mp_base.hpp`: GLXPlayerMPBaseObserver with the 14 callbacks in vtable order (slot 0 is OnMPFinishGameSuccess). GLXPlayerMPBase has observer at +0x04, error message at +0x4c, aux object at +0x68.
- The lobby refusal path (`RefuseRequest`) calls observer slot 0. That is OnMPFinishGameSuccess in the vtable, which looks wrong for a refusal. Verify against the binary.
- `include/net-session-online/glx_player_user_friend.hpp`: per-entry arrays at +0x40..+0x70 (see the comment). Getters use an inclusive bound (index <= count), GetName uses strict (<). The original reads at index == count, which is past the end. The port returns the default.
- GLXPlayerUserFriend ctors/dtors, clearFriendsList (816 bytes), processFriendsList and the send* methods are not reconstructed yet.
- GLXPlayerMPBase::mpSendEstablishConnection (0x84ad78), mpSendDisconnect (0x84ae28) and the setErrMsg() overload (0x84abd8) are not done.

## Wave 3: GLX player, state machine, IEvent, CMsg types

152 rows added in this wave (113 reconstructed, 32 partial, 7 skipped). Host and Windows
builds both pass: 16 tests, all passing.

### Layouts (original 32-bit offsets, kept as comments)

GLXPlayerComponent (`glx_player_component.hpp`): +0x00 vptr, +0x04 observer (GLXPlayerObserver*),
+0x08 GGI (unsigned long), +0x0c user id (owned string).

GLXPlayerObserver (`glx_player_observer.hpp`): the vtable has seven slots, and only slot 0 is named
in the dump. Four slots that the web component calls are declared: slot 0 OnDownloadError,
slot 1 OnRequestTimeout, slot 2 OnResponseText, slot 3 OnResponseError. Slots 1-3 are inferred
from call sites. The virtual destructor is an addition and is not in the slot count.

GLXPlayerWebComponent (`glx_player_web_component.hpp`): base GLXPlayerComponent.
- +0x10, +0x14, +0x18 owned strings (role not identified); +0x1c service URL (set by LoadConfig).
- +0x20 request client (type not identified; allocation partial).
- +0x24 response cursor (points into the response text); +0x28 response function id;
  +0x2c enabled flag; +0x34 request start time; +0x38 request id, -1 when idle.
- Virtual slots after the dtors: +0x08 Update, +0x0c SendByGet, +0x10 SendByPost, +0x14 Cancel,
  +0x18 OnUpdateParse, +0x1c OnUpdateSuccess, +0x20 OnUpdateFailure, +0x24 LoadConfig.

GLXPlayerUser (`glx_player_user.hpp`): extends GLXPlayerWebComponent (base at +0x00..+0x3b).
- Owned strings at +0x40..+0x124 (userName_ +0x44, password_ +0x48, email_ +0x4c, country_ +0x54,
  userData_ +0xc0, avatarKey_ +0xa8, chatWarning_ +0x118, and others; the header lists them).
- Vectors: gamePid_ +0x70, gameName_ +0x74, gameTrophy_ +0x78 (int32), avatarKeys_ +0x108,
  avatarLastUpdates_ +0x10c, avatarStatus_ +0x110; avatarCount_ +0x114.
- Scalars: email flag +0x50, sex +0x60, online time +0x68, total trophy +0x6c, game count +0x7c,
  field80_ +0x80 (-1), currentState_ +0x84 (-1), reputation (double) +0x90, bad/good reputation
  +0x98/+0x9c, trophy +0xa0, best score +0xa4, trophy-icon mode +0xd4 (-1).
- Not modelled: vector<channel> at +0xe8 and +0xf8, _LiveFeed array at +0x12c, request client at +0x20.

MultiplayerStateMachine (`multiplayer_state_machine.hpp`): +0x00 state (-1 when none),
+0x04 entry count, +0x08 head entry, +0x0c tail link (the Entry** slot the next entry is stored in).
Entry: +0x00 skip flag, +0x04 state id, +0x08 callback, +0x0c next, +0x10 link (the slot that holds
this entry, so removal can fix the predecessor). It is an intrusive singly linked list.

IEvent (`i_event.hpp`): +0x04 event id. IEventReceiver: virtual destructor only.

CMsgDropLoot (`c_msg_drop_loot.hpp`): header bytes +0x50..+0x67 (fixed_[0x18]); block length
int32 at +0x64; block owned at +0x68. GetDataSize = block length + 24.

CMsgQuestsSync (`c_msg_quests_sync.hpp`): header bytes +0x50..+0x57 (fixed_[8]); block length
int32 at +0x50; second header word at +0x54; block owned at +0x58. GetDataSize = block length + 8.

### Decisions and deviations

- Observer slot numbers are inferred from call sites. Each name in glx_player_observer.hpp says so.
- Pseudocode ends in a tail call that Ghidra could not follow for several bodies (process*,
  setPassword, sendSetUserStoreData, sendUploadUserAvatar, sendSetUserMind, IsNextResponseIntToken,
  OnUpdateSuccess, CMsgDropLoot::WriteData, the GLXPlayerUser ctor). Those bodies were written from
  the ARM listing in recovered/native/assembly/libDungeonHunter2.so/, which is authoritative.
- Text setters differ on empty input. setPassword uses OwnedDup (null stays null, "" is kept).
  The chat warning, promo and RSS attachment and trophy-icon setters use OwnedNonEmpty (null or
  "" gives null).
- Request builders write "f|kind|i|GGI|u|UID|..." into a 0x1000-byte buffer. A missing user id
  is sent as "" in the u| slot. A rejected argument calls observer slot 3 with (kind, -100) and sends nothing.
- Getters are bounded by the stored list size. The avatar getters also check avatarCount_. Out of
  range they return nullptr, -1 (trophy) or 0 (status). The original's getUserGamePID returns the
  stored pointer with no count check, so this is a deviation.
- MultiplayerStateMachine::runStateMachine calls each callback with nullptr. The ARM passes the skip
  byte there, which is 0 at that point. The state is re-read for each entry, so a callback that
  changes it affects the entries after it.
- Test seams (*ForTest, BlockLengthForTest, SendModeForTest, IncludeLocalForTest) are not in the original.
- The CMsgDropLoot and CMsgQuestsSync ctors set sendMode_ and includeLocalMember_ (CMsgQuestsSync also
  sets reliable_). The bool argument is not stored. The CMessage(const char*, bool) base ctor is not run.
- Fifteen earlier ledger rows (GLXPlayerMPBase, GLXPlayerMPBaseObserver, GLXPlayerUserFriend)
  had mangled-name lengths that did not match the queue. They were corrected to the queue names.

### What remains

- GLXPlayerUser response parsers (partial): processLiveFeeds (the 452-byte list goes into the
  _LiveFeed array at +0x12c, not modelled), processGetPromo_RSS and processGetADV_RSS (XML document
  reader slim::XmlDocument), and the parse bodies of processUserAvatar, processUserBestScore,
  processUserTrophy, processUserReputation, processUserGameIcon, processUserData,
  processUserAvatarList, processUserInformation, processUserState and processUserGameList.
  These need the token-parse helpers 0x82b634, 0x82abd4 and 0x82ace0 identified.
- processDownloadAvatar and processUploadAvatar: the copy of the avatar data is not reconstructed.
- sendSetUserMind (length check only), sendUploadUserAvatar (null case only).
- sendAwardTrophy (0x83e918) and sendUpdateUserInfo (0x83eb14): not reconstructed.
- GLXPlayerUser ctor and dtor: the 0x428-byte request client at +0x20 and the channel vectors at
  +0xe8/+0xf8 are not allocated or destroyed. clearUserState does not free the object at +0x88.
- GLXPlayerWebComponent transport: SendByGet, SendByPost, Update, Cancel, OnUpdateParse, LoadConfig
  and the dtors are stubs (partial). Their request client type is not identified.
- CMessage(const char*, bool) base constructor is not recovered. The std::string at +0x14 in CMessage is
  not modelled either.
