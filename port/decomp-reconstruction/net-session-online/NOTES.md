# net-session-online notes

Status: 1268 functions queued. Ledger has 722 rows (554 reconstructed, 99 partial, 69 skipped).
Wave 7 added 124 rows (598 at its start). 546 queued functions remain (see "Wave 7" below).

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

## Wave 6: sign-in, observers, GameCenter, CXPlayerManager, COnline (this session)

Ledger: 598 rows in total, 202 added this session (185 reconstructed, 14 partial, 3 skipped).
Queue: 1268 rows, 670 still unledgered. Every ledger address is in queue.csv (checked numerically).

### Layouts (original 32-bit offsets, kept as comments)

- CSignIn (`c_sign_in.hpp`): +0x00 vptr, +0x04 credentials (name, password, context), +0x10 active
  flag, +0x11 logged-in flag, +0x14 CEventQueueBase. Virtual slots: +0x08 Initialize, +0x0c and
  +0x10 SignIn, +0x14 SignOut, +0x18 CancelLogin, +0x1c Update, +0x20 Terminate, +0x24 Destroy.
  Singleton s_SignIn; provider switch s_SignInProvider (0 = CSignInDummy, 1 = CSignInGLLive).
- CSignInGLLiveObserver (`c_sign_in_gl_live_observer.hpp`): +0x04 server-configured flag (set by
  code 1), +0x05 logged-in flag (set by 0xf, cleared by 0x11), +0x06 server address (0x80 bytes),
  +0x88 last error code. Error codes: 1 no user name, 0x31 wrong user name or password,
  0x43 user name does not exist, 0x61 authentication failed, 0x6e nickname used, 0x71 invalid
  nickname, -2 no connection, 0x29 request timeout.
- CXPlayerManager (`cxplayer_manager.hpp`): +0x14 state (0x10 ready, 0x11 error or logged out),
  +0x7c last error (name inferred; 3 after a network error). +0x08, +0x0c and +0x10 are owned
  objects deleted through their virtual dtor (types not identified, not modelled).
- GameCenter (`game_center.hpp`): +0x07 features enabled, +0x24 player-ID list (the STL list
  sentinel, now std::list<std::string>), +0x2c player count, +0x30 host UID, +0x34 local user UID,
  +0x4c player alias (std::string), +0x70 achievement list tail (SaveGCAchievement, queued).
- GameCenterInfo (`game_center_info.hpp`): +0x10 list of strings. Other members not identified.
- COnline (`c_online.hpp`): +0x05 online-game flag, +0x30 time-synced flag. Non-virtual.
- COnlineImpl (`c_online_impl.hpp`): +0x04 initialized flag, +0x08 event queue, +0x40 last-check timeval.
- CMatchingLocal (`c_matching_local.hpp`): +0x3638 local member id, +0x4b6d room closed, +0x4b8d room hidden.
- CMatchingGLLive (`c_matching_gl_live.hpp`): +0x0d room hidden, +0x0e room closed, +0x3640 local
  member id, +0x3644 server member id, +0x6700 friends, +0x6be0 friend list loaded. The byte at
  +0x67ed is the alpha-server switch read by the sign-in observer (reached through the CMatching seam).
- CMatchingGLLiveLobbyObserver: +0x04 logged-in, +0x05 connected, +0x06 game started, +0x14 state,
  +0x48 last result. The friend observer has a flag at +0x04. The message observer has only a vptr.
- GLXPlayerLogin: +0x40 keep-alive enabled, +0x44 time of last keep-alive, APNS strings and ports
  at +0x48..+0x60, product arrays at +0x64..+0x70. Keep-alive interval 90000 ms.
- GLXPlayerMessage: message arrays at +0x3c..+0x5c, count +0x60, current id +0x64, body +0x68,
  boxCounts_[6][3] at +0x6c..+0xb3 (group mapping in getMsgCount), sent count +0xb4.
- GLXPlayerLeaderboard: entry count +0x3c, name and score arrays +0x40..+0x50, current player
  fields +0x58..+0x64. A missing score is 0xfff5d3d6.
- GLXPlayerSocket and LCXPlayerSocket: +0x04 state (7 = error), +0x08 handle, +0x820 socket type,
  +0x824 accept address (owned string), +0x828 accept port, +0x850 send percentage (GLX only).
- GLXPlayerHttp and LCXPlayerHttp: +0x04 socket pointer, +0x08 request buffer (0x400 bytes),
  +0x408 response text, +0x40c response length, +0x410 server address (GLX), +0x41c post body,
  +0x420 post body length, +0x424 send-by-post flag.
- GLXPlayerPing: +0x24 probe state, 1 when finished.
- OnlineGameState: +0x3c start-game timeval, +0x44 room-creation timeval (`time_of_day.hpp` stand-in).

### Decisions and deviations

- CSignIn::Get (partial): provider 0 creates a CSignInDummy. Provider 1 would create CSignInGLLive,
  which is not ported, so Get returns null then. s_SignInProvider is a static that defaults to 0 and
  nothing in this port sets it.
- The observer is a standalone class. Its vtable (first slot OnNetworkError) is not modelled, and the
  request handlers are direct calls. Events are posted through CSignIn::Get() each time, and a null
  singleton is skipped. The original would dereference it.
- OnRequestSuccess (partial): the code-1 branch omits GLXPlayerSereverConfig::GetXPlayerURL (queued),
  because its result is discarded. Code 0x6b copies the address with strncpy and keeps the NUL inside
  the 0x80-byte buffer. The original does not check the length. This port clamps it to 0x7f.
- Code 0x6b with the alpha flag set copies "alpha01.gameloft.com:7500" (26 bytes with the NUL). The
  pseudocode shows the strncpy after the branch, but the ARM listing (0x824c2c-0x824c78) returns after
  the memcpy. The ARM is authoritative, and the strncpy is not run on that path.
- The CMatching stand-in (`deps/matching.hpp`) gains two non-pure seams: UseAlphaServer() (default
  false) and SetGCState(int) (default no-op). CMatchingGLLive::SetGCState (0x81f178) is not ported.
- GameCenter list and alias: a null user UID or null alias is taken as an empty string. The original
  would construct std::string from a null pointer. GetGCPlayerAlias returns the std::string by value,
  which matches the sret in the pseudocode.
- GameCenterInfo: only the destructor is reconstructed. Its list at +0x10 is std::list<std::string>.
- CXPlayerManager::OnNetworkError prints the same line as the original, with puts.
- CMessage::CompareMsgSendPriority (0x80a6f8, 24 bytes) reads a uint at +0x38. The CMessage header
  names +0x38 as trackSent_ (a bool, described as int in the original). The two readings conflict,
  so the comparator is not ported yet. Resolve the field name before adding it.
- Earlier-wave decisions still apply: XP_API_STRNEW uses new[] and pairs with XP_API_FREE; the
  GLXPlayerSocket accept-address leak is fixed; the box and bounds checks are in the GLX message and
  leaderboard getters; IsHost treats a null UID as not the host; the observers are standalone;
  COnline is non-virtual; time uses the std::chrono stand-in in deps/time_of_day.hpp.
- Several bodies were garbled in the pseudocode (tail calls to memcpy, strncpy or __aeabi helpers
  shown as noreturn). The ARM listing was used for those, and the ARM is authoritative.

### What remains

Queue rows left, by owner and class (670 in total):
- lib-online-glx CMatchingGLLive 81, CMatchingLocal 55, CMatching 40, CMatchingBluetooth 17,
  tMatchingPeer 7. The matching layer is large and is still mostly queued.
- CMessaging 40, CMessage 4 (the ctor at 0x80a540/0x80a61c and CompareMsgSendPriority; see above),
  CMsgRaisedEvent 2, CMsgAttackResult 1 (CMsgAttackResult::Create at 0x3af330).
- Struct copies tFriendInfo 3, tRoomInfo 2, tGLRoomInfo 1, tMemberInfo 1, channel 6 and item 6 (slim
  XML objects), _LiveFeed 1 (its destructor at 0x83ab64).
- CSignInGLLive 14 (Initialize, SignIn, CancelLogin, SignOut, LoginLobby, SignIn(credentials), ctors,
  dtors, Terminate, Destroy). It needs CMatchingGLLive::GetGLXPlayerLogin and GLXPlayerLogin::SendLogout.
  CSignIn 4 (ctors 0x81a58c/0x81a61c and dtors 0x81a7b4/0x81a8b4; the CSignIn ctor sets an event queue
  expiry of 100 that the port does not model).
- GameCenter 15 (ctor 0x32bc34, dtors, GCStartMultiplayerGame family, GetJoinPlayerList,
  GCAddToJoinPlayerList, CoinTossForHost, SetPlayerIDStatus, SaveGCAchievement, ReportGCScore and
  ReportGCAchievement, GCStartGameTimeoutExpired, DumpPlayerIDStatusList). MultiplayerCallback 22.
- GLX and LCX services: GLXPlayerSereverConfig 27, GLXPlayerLogin 21, GLXPlayerMessage 21,
  GLXPlayerHttp 17, GLXPlayerLeaderboard 15, LCXPlayerHttp 15, GLXPlayerUserFriend 13,
  GLXPlayerSocket 13, LCXPlayerSocket 13, GLXPlayerPing 11, GLXPlayerMPLobby 5, GLXPlayerMPBase 4,
  GLXPlayerUser 3.
- CXPlayerManager 13 (Init is garbled: its tail call goes to an allocator. Login, Logout, IsLoggedIn,
  OnRequestSuccess, OnRequestFailure, OnRequestTimeout, UploadMyScore, ctors and dtors). The owned
  objects at +0x08, +0x0c and +0x10 need their types.
- COnline 17 (GetPing, SendPackets, ReceivePackets, SendSyncTime, Initialize, Terminate, Destroy,
  ReportDisconnect, ctors). These need CConnectionManager, which is not in deps/.
- COnlineImpl 10, OnlineGameState 9 (Update, Destroy, ctors, dtors, timeout helpers), and
  OnlineSingleton<OnlineGameState> 3. OnlineGameState::Update needs MultiplayerStateMachine and COnline.
- ALicenseCheck 21 and ALicenseCheck_GetUserAgent 1.
- XP_API quick wins still queued: XP_API_ITOA (82a66c), XP_API_TOLOWER (82ad54), XP_API_IP_INT2STR,
  XP_API_IP_STR2INT, XP_API_FTOA, XP_API_GET_RANDOM, XP_API_GET_TIME, XP_API_PARSE_DATA,
  XP_API_STRCHRFIND, the XP_API_FILE_* family, and the XP_DEBUG_*_LOG functions.
- No Json:: functions are queued for this category, so no upstream rows are needed.

## Wave 7: Hex-Rays source, sockets and HTTP, ping, CXPlayerManager (this session)

Source: IDA Hex-Rays is now the default for `tools/decomp_extract.py` (export in
`recovered/native/hexrays-all/`, commit 9476235b). Ghidra is only a fallback (`--source ghidra`).
Where the decompile garbled a tail call, the ARM listing (`recovered/native/assembly/...`) was
checked; the notes name each case.

### Rows added (124 this wave)

- GLXPlayerSocket (13): Start, Cancel, IsInProgress, both destructors, both constructor aliases,
  RemoveHttpHeader, CalculateTotalLength (reconstructed); ParseHttpHeader, ParseChunkedContent
  (partial); GLXPlayerSocketFactory::GetSocket (partial). D0 is skipped.
- GLXPlayerHttp (12): GetResponseLen, Start, UpdateRequest, IsInProgress, IsErrorOccurred, Cancel,
  SetResponseData (reconstructed); destructors D1 and D2 and both constructor aliases (partial);
  D0 skipped.
- GLXPlayerPing (9): Init, Prepare, both destructors, SendEchoRequest (reconstructed);
  WaitForEchoReply and both constructor aliases (partial); D0 skipped.
- GLXPlayerLeaderboard addScoreEntry (1, reconstructed). Deleting destructors of the leaderboard
  and OnlineGameState are skipped (2).
- Earlier in the wave (87): CXPlayerManager, GLXPlayerSereverConfig, the GLXPlayerLogin, Message,
  Leaderboard and UserFriend senders, and the XP_API wave-7 set.

### Layouts (checked against the ARM ctor and the Hex-Rays accessors)

GLXPlayerSocket (`glx_player_socket.hpp`): +0x04 state (0 idle, 1 started, 6 finished, 7 error,
8 cancelled), +0x08 handle (-1), +0x10 port, +0x14 host (owned copy), +0x18 observer,
+0x1c receive buffer (0x800 bytes, zeroed by Start), +0x81c receive length, +0x820 socket type,
+0x824 accept address (owned), +0x828 accept port, +0x82c received content (std::string),
+0x850 send percentage. Seven more words (+0x84c..+0x868) are zeroed by the ctor and not decoded.
The earlier header had no ctor arguments; the ctor was not stored.

GLXPlayerHttp (`glx_player_http.hpp`): +0x04 socket (owned), +0x08 request buffer (0x400),
+0x408 response (owned), +0x40c response length, +0x410 server address (owned copy of ctor
argument 1), +0x414 and +0x418 owned copies of ctor arguments 2 and 3, +0x41c post body (owned),
+0x420 post length, +0x424 send-by-post flag. The ctor creates the socket on port 0x50 with the
HTTP object as observer, so GLXPlayerHttp derives from GLXPlayerSocketObserver.

GLXPlayerPing (`glx_player_ping.hpp`): +0x08 flag (1), +0x0c host, +0x10 address (null),
+0x14 port, +0x18 socket, +0x1c result (-1), +0x24 state (1, then 2 after Prepare), +0x28 and
+0x2c timeouts (0, 5000 us; operator new in the original, held by value here), +0x30 echo id (123).

GLXPlayerSocket virtual hooks added: Start (+0x0c), Run (+0x10), Cancel (+0x14), IsInProgress
(+0x18), IsErrorOccurred (+0x1c), OnCancelled (+0x3c), SendTo (+0x58), Connect (+0x74). The base
defaults are no-ops; SendTo returns -1 and Connect returns 0, so a port without a transport
reports failure.

### Decisions and deviations

- Owned buffers. GLXPlayerHttp and GLXPlayerSocket own their strings and buffers through
  unique_ptr. The test seams now copy the post body and the response.
- Cancel clears the response and post pointers after freeing them. The original leaves them dangling.
- D1 and D2 of GLXPlayerHttp free the response and post with different allocators (operator delete in
  D1, free in D2). The port cannot reproduce that mix, so both are partial.
- std::string::substr past the end throws in the original. The port returns an empty string. This
  matters only for the chunked decode.
- ParseChunkedContent is kept as decompiled. The size line is cut at the absolute CRLF position and
  the next offset is CRLF+2+CRLF. Multi-chunk bodies may not decode the way the server meant.
- ParseHttpHeader with no CRLF CRLF terminator returns npos. The original copies npos bytes there,
  which is undefined.
- XP_API_GET_TIME is the glx_stand_ins.hpp stub (0), so the ping timestamp is 0.
- The ping checksum is a stand-in for the BSD in_cksum, which is not in the queue. A test checks that
  a packet with its checksum sums to all ones. Fields are in host byte order, as the original writes them.
- The socket factory builds the base class. The original builds CAndroidSocket (0xcf4 bytes). The
  ctors, and the factory, are partial for this reason.
- WaitForEchoReply does not call select(). It reports no data (0).

### Layout mismatches found in older headers (not fixed here)

- GLXPlayerLeaderboard: +0x50 is an array of per-entry int* arrays (score data), but the header models
  it as a flat vector. getLeaderboardEntry (0x8321cc) and clearLeaderboard (0x832a48) need the
  per-entry model. The leaderboard ctors (0x833244, 0x8332dc) store the request client at the
  GLXPlayerWebComponent requestClient_ (void*), which is not typed as GLXPlayerHttp yet.
- GLXPlayerMessage: +0x50 is a char* array (the des-user name lists), but the header says receiver
  ids. processMsgCount (0x834ffc) is clean and is the next candidate; the other parsers depend on
  the corrected layout.
- GameCenter: the full layout needs +0x74 (the list sentinel) and GCSingleton<GameCenter>. The
  header models only the simplified state, so the ctor and dtors are not started.
- OnlineGameState: the ctor (0x49ffe0, 0x4a0040) sets members from +0x04 to +0x138 that the header
  does not model.
- LCXPlayerSocket and LCXPlayerHttp mirror the GLX classes, but their receive buffer and state
  members are not in the header yet.

### What remains (546 queued functions; 76 short of the 200-row target for this wave)

Blocked on classes or stand-ins the port does not have:
- COnline and COnlineImpl (about 27): need CConnectionManager (GetConnectionMgr, RegisterPacketType,
  GetServerPing) and CPacketManager (SendPackets). Those belong in deps/ as stand-ins.
- CMatching, CMatchingLocal, CMatchingGLLive and the observers (about 200): need the CMatching base
  with tMatchingPeer and the CNetworkId and CRoomAttributes types.
- CMessaging (about 40): needs the CMessage queues with std::list and std::set, NetBitStream and
  CNetMutex.
- GameCenter (about 15) and CSignInGLLive (about 14): need the full GameCenter layout and GCSingleton.
- ALicenseCheck (21): FUN_ names, not yet identified.
- MultiplayerCallback (22): large message handlers.
- channel and item copy constructors: need slim::XmlNode and the item fields.

Not blocked, but large or layout-dependent: GLXPlayerLeaderboard and GLXPlayerMessage parsers
(see the mismatches above), GLXPlayerSereverConfig SaveServerConfig and LoadConfig (std::map node
walks), GLXPlayerMPBase and GLXPlayerMPLobby senders, LCXPlayerSocket and LCXPlayerHttp (receive
buffer and state members), and the LC_API_FREE and LC_API_MALLOC thunks.

## Wave 4 decisions (feature/decomp-net, 2026-10-12)

- Ledgers merged from the 10 glx slices and 4 nso slices. Blocked rows are not in the ledger.
- GLXPlayerMessage ctor (0x8368e0 / 0x836980) was missing from src. Added as partial: LoadConfig, current id -1 and
  clearMsgCount are reproduced. The 0x428-byte GLXPlayerHttp at +0x20 is not allocated (no typed slot).
- glx_player_user_responses_test expectations were aligned with the ledger: processUserInformation("ignored") gives an
  empty name (not null), and processUserGameList("ignored") gives one record. Later replaced by the integrator's version.
- Host 61/61 and MinGW/Wine 91/91 pass for net-transport and net-session-online together.

- Decision (2026-10-12): LCXPlayerSocket::ParseChunkedContent (0x8a0af0) stays faithful to the binary. The
  Hex-Rays body and IDA listing use the absolute CR index as the next-data offset, so a second chunk reads from the
  wrong place. The data-end constant (v58 = end + 4 in the pseudocode, +2 in the listing) is unresolved. Left partial
  until the wire format is checked against a real capture. Do not "fix" it to standard chunked semantics without that.
