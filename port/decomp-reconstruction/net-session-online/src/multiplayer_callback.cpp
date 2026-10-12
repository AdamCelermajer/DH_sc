#include "net-session-online/multiplayer_callback.hpp"

// 0x49a0c4 _ZN19MultiplayerCallback9idleStateEPv
std::int32_t MultiplayerCallback::idleState(void* /*context*/) { return 100; }

// 0x49a0cc _ZN19MultiplayerCallback11ingameStateEPv
std::int32_t MultiplayerCallback::ingameState(void* /*context*/) { return 300; }

// Partial group. Every body below is a stub: the Hex-Rays pseudocode reads the message
// queue through CMessaging (its GetMessageFromQueue is not in this port), and the work
// goes through game types that are also missing: PlayerManager, ObjectManager, Character,
// Application, GSLevel, MenuManager, RenderFX, CNetPlayerManager and OnlineGameState
// (fields +0x24 flag and +0x34 state, not modelled). Each stub keeps the original return
// code where the pseudocode returns one unconditionally, and 0 otherwise.
// The message-consumed flag (the byte at message +0x3c, set to 1 in the original) is not
// written here because the CMessage layout past +0x34 is not recovered.

// 0x49a0d4 _ZN19MultiplayerCallback32HandleAskResendAttributesMessageEPv
// Partial: missing CMessaging, CMatching::IsServer, CNetPlayerManager, GetNetPlayerMgr.
std::int32_t MultiplayerCallback::HandleAskResendAttributesMessage(void* /*message*/) { return 0; }

// 0x49a17c _ZN19MultiplayerCallback24HandleGlobalDeathMessageEPv
// Partial: missing CMessaging, PlayerManager, Character (Kill, death timer).
std::int32_t MultiplayerCallback::HandleGlobalDeathMessage(void* /*message*/) { return 0; }

// 0x49a6e4 _ZN19MultiplayerCallback24HandleRaisedEventMessageEPv
// Partial: missing Application/GSLevel level check, CMsgRaisedEvent::RaiseEventFromNetwork.
std::int32_t MultiplayerCallback::HandleRaisedEventMessage(void* /*message*/) { return 0; }

// 0x49a75c _ZN19MultiplayerCallback17LoadGLLiveFriendsEv
// Partial: missing CMatchingGLLive (GLFriendsListLoaded, GetGLFriendsCount/Name/Number/State,
// AddFriendInGameList). Original returns the loaded flag; stub returns 0.
std::int32_t MultiplayerCallback::LoadGLLiveFriends() { return 0; }

// 0x49a808 _ZN19MultiplayerCallback16HandlePlayerLeftEPv
// Partial: missing GSLevel, CEventQueueBase event data (id 3145731), ObjectManager,
// ObjectHandle, PlayerManager::RemoveCharacter, CNetPlayerManager::GetPlayerCount.
std::int32_t MultiplayerCallback::HandlePlayerLeft(void* /*message*/) { return 0; }

// 0x49a960 _ZN19MultiplayerCallback22HandleMenuReadyMessageEPv
// Partial: missing CMsgMenuReady field layout (+0x50, +0x54), gameswf::as_value.
std::int32_t MultiplayerCallback::HandleMenuReadyMessage(void* /*message*/) { return 0; }

// 0x49a9fc _ZN19MultiplayerCallback19HandleIsHostMessageEPv
// Partial: missing CNetPlayerManager::GetPlayerByMemberId, NetStructMember::SetChanged.
std::int32_t MultiplayerCallback::HandleIsHostMessage(void* /*message*/) { return 0; }

// 0x49aa60 _ZN19MultiplayerCallback24HandleSpawnObjectMessageEPv
// Partial: missing ObjectManager, ObjectHandle, ObjectBase, Character::CreateNPC,
// GameObject, RoomZone.
std::int32_t MultiplayerCallback::HandleSpawnObjectMessage(void* /*message*/) { return 0; }

// 0x49ac18 _ZN19MultiplayerCallback17HandleLootMessageEPv
// Partial: missing ItemInstance, ItemInventory, ItemObject, ObjectManager.
std::int32_t MultiplayerCallback::HandleLootMessage(void* /*message*/) { return 0; }

// 0x49ade0 _ZN19MultiplayerCallback19HandleAttackMessageEPv
// Partial: missing CNetPlayerManager, PlayerManager, Character::F_ApplyResult, ObjectManager.
std::int32_t MultiplayerCallback::HandleAttackMessage(void* /*message*/) { return 0; }

// 0x49af74 _ZN19MultiplayerCallback23HandleQuestsSyncMessageEPv
// Partial: missing PlayerManager::HandleQuestSyncMsg and GetOnline(); the ASSERT trace
// string at MultiplayerCallbacks.cpp:562 is not recovered.
std::int32_t MultiplayerCallback::HandleQuestsSyncMessage(void* /*message*/) { return 0; }

// 0x49b054 _ZN19MultiplayerCallback20HandleInitialMessageEPv
// Partial: missing PlayerManager::IsLocalPlayerHosting and PlayerManager::InitialSetup.
std::int32_t MultiplayerCallback::HandleInitialMessage(void* /*message*/) { return 0; }

// 0x49b0d8 _ZN19MultiplayerCallback22HandleStartGameMessageEPv
// Partial: missing Level, PlayerManager, Character::ReloadSkills, Level::SG_SaveAllPlayer,
// Application::LoadLevel, Arrays::LevelList table, PlayerSavegame::SG_GetNextFreeSlot.
std::int32_t MultiplayerCallback::HandleStartGameMessage(void* /*message*/) { return 0; }

// 0x49b3c8 _ZN19MultiplayerCallback15roomLobbyUpdateEPv
// Partial: missing MenuManager, RenderFX::InvokeASCallback, gameswf::character, CRoomAttributes
// int lookup (type exists, but the menu side does not). Returns 100 as the original does.
std::int32_t MultiplayerCallback::roomLobbyUpdate(void* /*context*/) { return 100; }

// 0x49b47c _ZN19MultiplayerCallback11lobbyUpdateEPv
// Partial: missing MenuManager, RenderFX::InvokeASCallback, PlayerManager::GetLocalPlayer.
// Returns 200 as the original does.
std::int32_t MultiplayerCallback::lobbyUpdate(void* /*context*/) { return 200; }

// 0x49b580 _ZN19MultiplayerCallback19searchRoomPushNotifEPv
// Partial: missing Application::GetVersionNumber, OnlineGameState flags (+0x24, +0x25),
// CMatching::SearchRoom call chain. Returns 102 or 100 as the original does; the search
// itself is not issued.
std::int32_t MultiplayerCallback::searchRoomPushNotif(void* /*context*/) { return 100; }

// 0x49b660 _ZN19MultiplayerCallback10searchRoomEPv
// Partial: missing PlayerSavegame, MenuManager::GetRoot, RenderFX, gameswf::root.
// Returns 102 or 100 as the original does; the search itself is not issued.
std::int32_t MultiplayerCallback::searchRoom(void* /*context*/) { return 100; }

// 0x49b990 _ZN19MultiplayerCallback20HandleMenuBackButtonEPv
// Partial: missing GameCenter, Application::GoToMainMenu, MenuManager, and the menu-name
// dispatch (LC_API_STRCMP_0 chain over menu_* names) is not reconstructed.
std::int32_t MultiplayerCallback::HandleMenuBackButton(void* /*message*/) { return 0; }

// 0x49c5e4 _ZN19MultiplayerCallback15HandleScriptCmdEPv
// Partial: missing ScriptManager (StartScript/SkipScript/StopScript), DialogMsg queue,
// MenuMessageManager<DialogMsg,1>, PlayerManager.
std::int32_t MultiplayerCallback::HandleScriptCmd(void* /*message*/) { return 0; }

// 0x49c7b4 _ZN19MultiplayerCallback8joinRoomEPv
// Partial: missing CMatching::JoinRoom and tRoomInfo (type named in NOTES as tRoomInfo, not in port).
// Returns 200, or 103 when the online-state flag is clear, as the original does.
std::int32_t MultiplayerCallback::joinRoom(void* /*context*/) { return 200; }

// 0x49c8c0 _ZN19MultiplayerCallback10createRoomEPv
// Partial: missing PlayerSavegame, DebugSwitches, glitch::os::Timer,
// CRoomAttributes setters, CMatching::CreateJoinRoom, gettimeofday use.
// Returns 100, or 101 when the online-state flag is clear, as the original does.
std::int32_t MultiplayerCallback::createRoom(void* /*context*/) { return 100; }

// 0x49cd94 _ZN19MultiplayerCallback18HandlePlayerJoinedEPv
// Partial: missing StringManager, PyDataConstants, DebugCachedCharacter, MenuFX/RenderFX,
// OnlineStatusMsg deque, CNetPlayerManager::GetPlayer, CMessage::CreateMessage for CMsgStartGame.
std::int32_t MultiplayerCallback::HandlePlayerJoined(void* /*message*/) { return 0; }
