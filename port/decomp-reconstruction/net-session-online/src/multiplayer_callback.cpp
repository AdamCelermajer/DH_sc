#include "net-session-online/multiplayer_callback.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "core-platform/memory.hpp"            // gAssertLevel
#include "dh2/Application.hpp"                // ApplicationGetInstance
#include "dh2/Character.hpp"                  // Character::IsDead, Kill, CreateNPC, F_ApplyResult
#include "dh2/GSLevel.hpp"                    // GSLevel::s_level (the +0x130 load step)
#include "dh2/ItemInstance.hpp"               // ItemInstance::ItemInstance, AddPower
#include "dh2/ItemInventory.hpp"              // ItemInventory, CalcLootItemValue
#include "dh2/ItemObject.hpp"                 // ItemObject::DropInventory, DropLootTable
#include "dh2/Level.hpp"                      // Level::m_loadStep, m_isLoaded, m_levelListIndex
#include "dh2/MenuManager.hpp"                // MenuManager::GetInstance, GetMenuByName
#include "dh2/MenuFX.hpp"                     // MenuFX (the RenderFX behind State::m_menu)
#include "dh2/ObjectManager.hpp"              // ObjectManager::unk_100, GetObjectHandleByNetworkId
#include "dh2/Point3D.hpp"                    // Point3D<float>, PlayerManager::InitialSetup
#include "dh2/PlayerInfo.hpp"                 // PlayerInfo::unk_660 (the local Character)
#include "dh2/PlayerManager.hpp"
#include "dh2/RoomZone.hpp"                   // RoomZone::AddInitialObject (0x396a90)
#include "dh2/ScriptManager.hpp"              // ScriptManager::s_inst
#include "dh2/global/ObjectManagerInstance.hpp"  // dword_99F764
#include "dh2/global/net_singletons.hpp"      // GetOnline
#include "net-session-online/c_event_queue_base.hpp"  // CEventQueueBase (internal_events_)
#include "net-session-online/c_matching_base.hpp"
#include "net-session-online/c_messaging.hpp"
#include "net-session-online/c_msg_attack_result.hpp"  // CMsgAttackResult (the typed AttackResult payload)
#include "net-session-online/c_msg_fixed.hpp"          // CMsgSpawnObject
#include "net-session-online/c_msg_quests_sync.hpp"
#include "net-session-online/c_msg_raised_event.hpp"
#include "net-session-online/c_online.hpp"
#include "net-session-online/online_game_state.hpp"    // OnlineGameState flags, OnlineSingleton
#include "net-session-online/tb/online_singleton.hpp"  // OnlineSingleton<OnlineGameState>::GetInstance
#include "net-transport/tb/c_net_player_manager.hpp"  // CNetPlayerManager (host member id, players)

// 0x49a0c4 _ZN19MultiplayerCallback9idleStateEPv
std::int32_t MultiplayerCallback::idleState(void* /*context*/) { return 100; }

// 0x49a0cc _ZN19MultiplayerCallback11ingameStateEPv
std::int32_t MultiplayerCallback::ingameState(void* /*context*/) { return 300; }

// Redo pass (2026-10-13, batch r1n). Bodies are written in place. Every message handler ends by
// setting the consumed flag, the byte at CMessage +0x3c (CMessage::SetFinished; net structure pass: the field and its
// setter are declared now). Handlers that depend on the unrecovered
// OnlineGameState flags, on ObjectManager lookups (declared, but no definition in the tree), on
// RenderFX::InvokeASCallback (no definition), on CMatching::SearchRoom or JoinRoom (no definition),
// or on private CNetPlayerManager fields (the host member id at +0x170) stay stubs that keep the
// original return code where the pseudocode returns one unconditionally, and 0 otherwise.

// 0x49a0d4 _ZN19MultiplayerCallback32HandleAskResendAttributesMessageEPv
// On the server, or on a client whose local member id is the member of the host player (the player at the host
// member id, manager +0x170; its CNetPlayerInfo +0x1a0 value), the original asks the matching object's room attributes
// (vtable +0xa4) to resend (their vtable +0x18, CRoomAttributes::Resend). The message is then marked consumed.
// Partial: CRoomAttributes does not declare Resend (the NetStruct wire format is not ported), so the resend itself is
// not made; the condition and the consumed flag are.
std::int32_t MultiplayerCallback::HandleAskResendAttributesMessage(void* /*message*/) {
    CMessage* message = CMessaging::GetInstance()->GetMessageFromQueue("CMsgAskResendAttributes");
    bool resend = CMatching::Get()->IsServer();
    if (!resend) {
        const std::int32_t local = CMatching::Get()->LocalMemberId();  // vtable +0x6c
        CNetPlayerManager* manager = GetNetPlayerMgr();
        const CNetPlayerInfo* host = manager->GetPlayer(manager->net_struct_.host_member_id_.GetValue(), false);
        resend = local == host->matching_member_id_.GetValue();  // +0x1a0
    }
    if (resend) {
        // Not made: CMatching::Get()->GetRoomAttributes()->Resend() (CRoomAttributes vtable +0x18).
    }
    message->SetFinished();  // +0x3c
    return 0;
}

// 0x49a17c _ZN19MultiplayerCallback24HandleGlobalDeathMessageEPv
// The local player's Character (PlayerInfo::unk_660) is killed when alive, or its death timer is reset when it is
// dead and PlayerManager::m_globalDeathState is 0; then the message is marked consumed (+0x3c).
std::int32_t MultiplayerCallback::HandleGlobalDeathMessage(void* /*message*/) {
    CMessage* message = CMessaging::GetInstance()->GetMessageFromQueue("CMsgGlobalDeath");
    PlayerManager* players = ApplicationGetInstance().m_playerManager;  // dword_99F76C = s_inst +0x40
    PlayerInfo* localPlayer = players->GetLocalPlayer(0, true);
    Character* character = localPlayer->unk_660;
    if (character != nullptr) {
        if (!character->IsDead()) {
            character->Kill(nullptr, true);
        } else if (players->m_globalDeathState == 0) {
            players->ResetCharacterDeathTimer();
        }
    }
    message->SetFinished();  // +0x3c
    return 0;
}

// 0x49a6e4 _ZN19MultiplayerCallback24HandleRaisedEventMessageEPv
// The message is raised only while the current Level is loaded (m_loadStep 38 and m_isLoaded set), and then marked
// consumed (+0x3c); the early return leaves it queued. CMsgRaisedEvent::RaiseEventFromNetwork is itself partial
// (its guard only).
std::int32_t MultiplayerCallback::HandleRaisedEventMessage(void* /*message*/) {
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level == nullptr || level->m_loadStep != 38 || !level->m_isLoaded) {
        return 0;
    }
    auto* message = static_cast<CMsgRaisedEvent*>(CMessaging::GetInstance()->GetMessageFromQueue("CMsgRaisedEvent"));
    CMsgRaisedEvent::RaiseEventFromNetwork(message);
    message->SetFinished();  // +0x3c
    return 0;
}

// 0x49a75c _ZN19MultiplayerCallback17LoadGLLiveFriendsEv
// Partial: the friend list calls are all declared now (GLFriendsListLoaded, GetGLFriendsCount, GetGLFriendsName,
// GetGLFriendState, AddFriendInGameList with GLFriendInfo), but CMatchingGLLive::GetGLFriendsNumber is declared to
// return std::int32_t, while the pseudocode strdups its result as a string. It follows GLXPlayerUserFriend::GetUserNumber,
// which the port declares as uint32_t. Until that owner type is settled the number cannot be copied. The original
// returns the loaded flag, or after loading the object pointer the setter leaves in r0; the stub returns 0.
std::int32_t MultiplayerCallback::LoadGLLiveFriends() { return 0; }

// 0x49a808 _ZN19MultiplayerCallback16HandlePlayerLeftEPv
// While the current Level is loaded (m_loadStep > 35), the internal player-left event (3145731 = 0x300003) is read
// from CNetPlayerManager::internal_events_ (+0x180). The room-zone list at ObjectManager::unk_100 (+0x100, a
// std::list<ObjectBase*> whose sentinel is the +0x100 member) is walked to count its entries (asm 0x49a880..0x49a88c),
// then the loop asks GetObjectHandleByNetworkId for index 0..count-1 and, for an entry that converts to a Character
// (ObjectHandle::operator Character*, 0x33ff54) that is not null, answers IsPlayer() (vtable +0x28) and whose
// m_netInternalId (+0x1f8c, LDR R3,[R9,R10] with R10 = 0x1F8C) equals the event's member id, calls
// PlayerManager::RemoveCharacter. The loop ends after the entry whose index reaches the count, or immediately when a
// removal lands on the last index. With at most three players left (GetPlayerCount() <= 3) the matching object's
// virtual slot +0x44 is invoked with 1, and the event is consumed.
namespace {

// The original is `LDR PC,[R3,#0x44]` on the CMatching object (asm 0x49a94c). That slot has no name in the tree:
// c_matching_base.hpp's vtable sketch places +0x44 at OpenRoom, which is a room-lobby call and does not read as a
// "few players left" notification, so no name is adopted. The slot offset is reproduced instead.
void CallMatchingSlot44(CMatching* matching, int argument) {
    void (*slot44)(CMatching*, int) = nullptr;
    std::memcpy(&slot44, reinterpret_cast<const std::uint8_t*>(*reinterpret_cast<void* const*>(matching)) + 0x44,
                sizeof(slot44));
    if (slot44 != nullptr) {
        slot44(matching, argument);
    }
}

}  // namespace

std::int32_t MultiplayerCallback::HandlePlayerLeft(void* /*message*/) {
    Level* level = GSLevel::s_level;
    if (level != nullptr && level->m_loadStep > 35) {
        std::int32_t memberId = -1;
        CNetPlayerManager* manager = GetNetPlayerMgr();
        manager->internal_events_.GetEventData(0x300003, &memberId, 4);  // 3145731, +0x180
        ObjectManager* objects = dword_99F764;
        const std::list<ObjectBase*>& zones = objects->unk_100;  // +0x100
        if (!zones.empty()) {
            std::uint32_t count = 0;
            for (auto it = zones.begin(); it != zones.end(); ++it) {
                ++count;
            }
            for (std::uint32_t index = 0; index != count; ++index) {
                ObjectHandle handle = objects->GetObjectHandleByNetworkId(static_cast<int>(index));
                Character* character = static_cast<Character*>(handle);
                if (character == nullptr || !character->IsPlayer() || character->m_netInternalId != memberId) {
                    continue;
                }
                ApplicationGetInstance().m_playerManager->RemoveCharacter(character);
                if (index + 1 == count) {
                    break;
                }
            }
        }
        if (manager->GetPlayerCount() <= 3) {
            CallMatchingSlot44(CMatching::Get(), 1);
        }
        manager->internal_events_.ConsumeEvent(0x300003);
    }
    return 0;
}

// 0x49a960 _ZN19MultiplayerCallback22HandleMenuReadyMessageEPv
// Partial: the body is gated on OnlineGameState's state being 200, and the state has no accessor (the
// MultiplayerStateMachine state is test-only). The as_value pair the pseudocode builds is never passed to a
// call, so it has no effect. The stub returns 0 (and does not mark the message consumed).
std::int32_t MultiplayerCallback::HandleMenuReadyMessage(void* /*message*/) { return 0; }

// 0x49a9fc _ZN19MultiplayerCallback19HandleIsHostMessageEPv
// The sender (CMessage header +0x0c) names the new host: its player (GetPlayerByMemberId(sender, 0, false)) gives the
// player id (+0x178), which becomes the manager's host member id (+0x170, with NetStructMember::SetChanged on +0x150
// when it differs); then the message is marked consumed (+0x3c).
std::int32_t MultiplayerCallback::HandleIsHostMessage(void* /*message*/) {
    CMessage* message = CMessaging::GetInstance()->GetMessageFromQueue("CMsgIsHost");
    const std::int32_t playerId =
        GetNetPlayerMgr()->GetPlayerByMemberId(message->Sender(), 0, false)->member_id_.GetValue();  // +0x178
    GetNetPlayerMgr()->net_struct_.host_member_id_.SetValue(playerId);  // compares, stores and SetChanged
    message->SetFinished();  // +0x3c
    return 0;
}

// 0x49aa60 _ZN19MultiplayerCallback24HandleSpawnObjectMessageEPv
// Partial: ObjectManager::GetObjectHandleByNetworkId and AssignObjectNetworkId, AddNoRoomObject are declared
// (game/include) but have no definition in the tree, and RoomZone::AddInitialObject has none either. The
// payload fields of CMsgSpawnObject are not named. The stub returns 0 (and does not mark the message consumed).
std::int32_t MultiplayerCallback::HandleSpawnObjectMessage(void* /*message*/) { return 0; }

// 0x49ac18 _ZN19MultiplayerCallback17HandleLootMessageEPv
// Partial: the drop target lookup needs ObjectManager::GetObjectHandleByNetworkId, which has no definition in the
// tree. The payload fields of CMsgDropLoot are only partly named (block length and data). The stub returns 0 (and does not mark the message
// consumed).
std::int32_t MultiplayerCallback::HandleLootMessage(void* /*message*/) { return 0; }

// 0x49ade0 _ZN19MultiplayerCallback19HandleAttackMessageEPv
// Partial: the attacker and target lookups need ObjectManager::GetObjectHandleByNetworkId, which has no definition in
// the tree. CMsgAttackResult's payload fields exist in c_msg_attack_result.hpp, which conflicts with the second
// CMsgAttackResult in c_msg_fixed.hpp, so it is not included here yet. The stub returns 0 (and does not mark the
// message consumed).
std::int32_t MultiplayerCallback::HandleAttackMessage(void* /*message*/) { return 0; }

// 0x49af74 _ZN19MultiplayerCallback23HandleQuestsSyncMessageEPv
// On a server the handler asserts (ASSERT trace at MultiplayerCallbacks.cpp:562, recovered from the pseudocode). At gAssertLevel 2 the
// original stores to address 0 to fault; the port aborts instead. Off the server the block goes to
// PlayerManager::HandleQuestSyncMsg.
std::int32_t MultiplayerCallback::HandleQuestsSyncMessage(void* /*message*/) {
    auto* message = static_cast<CMsgQuestsSync*>(CMessaging::GetInstance()->GetMessageFromQueue("CMsgQuestsSync"));
    COnline* online = GetOnline();
    if (online->IsServer()) {
        if (gAssertLevel == 2) {
            std::abort();  // the original faults on a store to address 0
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "false",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Multiplayer\\MultiplayerCallbacks.cpp", 562);
        }
    } else {
        PlayerManager* players = ApplicationGetInstance().m_playerManager;  // dword_99F76C = s_inst +0x40
        players->HandleQuestSyncMsg(message->BlockData(), message->BlockSizeValue());
    }
    message->SetFinished();  // +0x3c
    return 0;
}

// 0x49b054 _ZN19MultiplayerCallback20HandleInitialMessageEPv
// CMsgInitialSetup has no named fields yet, so the three payload words (original +0x50, +0x54, +0x58) are read through GetDataPtr
// as raw 32-bit words and their bits become the Point3D<float> position.
std::int32_t MultiplayerCallback::HandleInitialMessage(void* /*message*/) {
    CMessage* message = CMessaging::GetInstance()->GetMessageFromQueue("CMsgInitialSetup");
    std::uint32_t words[3] = {0, 0, 0};
    if (const void* payload = message->GetDataPtr(); payload != nullptr) {
        std::memcpy(words, payload, sizeof words);
    }
    PlayerManager* players = ApplicationGetInstance().m_playerManager;  // dword_99F76C = s_inst +0x40
    if (!players->IsLocalPlayerHosting()) {
        Point3D<float> position{};
        std::memcpy(&position, words, sizeof position);
        players->InitialSetup(position);
    }
    message->SetFinished();  // +0x3c
    return 0;
}

// 0x49b0d8 _ZN19MultiplayerCallback22HandleStartGameMessageEPv
// Partial: the body reads OnlineGameState fields that the class does not declare (+0x24, +0x34, +0x4c), the
// CMsgStartGame payload (no named fields), and the Arrays::LevelList table (level name at element +0x20), which is not in the tree.
// Application::LoadLevel, Level::SG_SaveAllPlayer, Character::ReloadSkills and PlayerSavegame::SG_GetNextFreeSlot
// are declared and defined. The stub returns 0 (and does not mark the message consumed).
std::int32_t MultiplayerCallback::HandleStartGameMessage(void* /*message*/) { return 0; }

// 0x49b3c8 _ZN19MultiplayerCallback15roomLobbyUpdateEPv
// Partial: RenderFX::InvokeASCallback has no definition in the tree. The gate also reads the OnlineGameState byte
// at +0x28 and COnline's online-game flag (+0x05), which has no getter. The attribute lookup
// (CRoomAttributes::GetAttributeInt, attribute 7) and the menu lookup (MenuManager::GetMenuByName) are declared.
// Returns 100 as the original does.
std::int32_t MultiplayerCallback::roomLobbyUpdate(void* /*context*/) { return 100; }

// 0x49b47c _ZN19MultiplayerCallback11lobbyUpdateEPv
// Partial: RenderFX::InvokeASCallback has no definition in the tree, and COnline's online-game flag (+0x05) has no
// getter. The player list needs PlayerManager::GetLocalPlayer (declared) and the menu lookup (declared). The
// OnlineGameState byte at +0x28 is not declared. Returns 200 as the original does.
std::int32_t MultiplayerCallback::lobbyUpdate(void* /*context*/) { return 200; }

// 0x49b580 _ZN19MultiplayerCallback19searchRoomPushNotifEPv
// Partial, and the note it carried was WRONG about three of its four blockers. Re-read against the 0x49b580
// disassembly (36 instructions):
//  - CRoomSearchFilter IS defined: the class is dh2/CRoomSearchFilter.hpp with bodies in
//    game-world/src/g1-game-world-CRoomSearchFilter-434a1f-0.cpp (ctor 0x8191fc, SetSearchAttributeInt
//    0x819cf8 at line 164).
//  - The OnlineGameState bytes at +0x24 and +0x25 ARE declared: online_game_state.hpp flag24_ / flag25_.
//  - Application::GetVersionNumber IS declared and defined.
//  - CMatching::SearchRoom (0x7feb40) has NO definition in the tree, and this time that is the one real
//    blocker: its listing tail calls the search object's vtable +0x08 (CMatchingLocal::SearchRoomInternal
//    0x807bec / CMatchingGLLive::SearchRoomInternal 0x81fadc), and the port's CMatching has no declaration at
//    that slot. See consequence 4 of the KNOWN DEFECT block in c_matching_base.hpp. Reconstructing
//    CMatching::SearchRoom would add a virtual slot to CMatching, which is exactly the integrator decision
//    that block reserves.
// The rest of the body is now fully recovered, so the next worker needs only the SearchRoom slot:
//   if (!OnlineGameState::flag24_) return 100;                     // asm 0x49b588..0x49b5a8
//   flag25_ = false;                                               // +0x25, asm 0x49b5b8
//   CRoomSearchFilter filter;                                      // ctor 0x8191fc, asm 0x49b5c0
//   filter.SetSearchAttributeInt(ROOM_INT_ATTRIBUTE_1, kOpEqual, 0xFEEDABBA);        // asm 0x49b5c4..0x49b5d8
//   filter.SetSearchAttributeInt(ROOM_INT_ATTRIBUTE_2, kOpEqual, Application::GetVersionNumber());  // 0x49b5e4
//   CMatching::Get()->SearchRoom(filter-copy, true, 0);            // asm 0x49b600..0x49b624
//   if (mode_ (+0x34) != 3) return 100;                            // asm 0x49b630..0x49b648
//   LoadGLLiveFriends(); return 100;                               // asm 0x49b650
// Returns 100 in every reachable case here, as the stub does.
std::int32_t MultiplayerCallback::searchRoomPushNotif(void* /*context*/) { return 100; }

// 0x49b660 _ZN19MultiplayerCallback10searchRoomEPv
// Partial, and the note it carried was WRONG about three of its four blockers; re-read against the 0x49b660
// disassembly (86 instructions):
//  - CRoomSearchFilter IS defined (see the row above).
//  - The OnlineGameState bytes at +0x24 and +0x25 and the int at +0x34 ARE declared: flag24_, flag25_, mode_.
//  - RenderFX::GetRoot (0x7a7cac), gameswf::root::get_root_movie and RenderFX::InvokeASCallback (0x7abe0c)
//    are all declared and defined.
//  - CMatching::SearchRoom (0x7feb40) has NO definition, which is the one real blocker and the same
//    integrator decision as the row above (consequence 4 of the KNOWN DEFECT block in c_matching_base.hpp).
// Recovered body, for the next worker: flag24_ must be set (else 100); flag25_ = false; PlayerSavegame built
// with GetLocalPlayer(0, false)->unk_664 (+0x664, -1 falls back to dword_99F778+8); difficulty =
// PlayerSavegame::m_difficultyLevel; the level id is SG_GetQuestSG()[4 * difficulty + 0x44] (asm
// 0x49b70c..0x49b718); MenuManager::GetRoot()->GetRoot()->get_root_movie() feeds
// RenderFX::InvokeASCallback("GetCurrentDifficulty") whose result the listing DISCARDS (asm 0x49b750 leaves it
// in r0 and r0 is overwritten); when mode_ == 2 only attribute 4 is set (to dword_99F778+0x0C), otherwise
// attributes 1 (0xFEEDABBA), 2 (GetVersionNumber) and then 4; attribute 5 is always set to the level id, copy
// the filter and call CMatching::SearchRoom(copy, true, 0); then if mode_ == 3, LoadGLLiveFriends(). Returns 100.
std::int32_t MultiplayerCallback::searchRoom(void* /*context*/) { return 100; }

// 0x49b990 _ZN19MultiplayerCallback20HandleMenuBackButtonEPv
// Partial: the body is a dispatch over the menu name stored at OnlineGameState +0x30 (LC_API_STRCMP_0 chain over
// menu_* names). The OnlineGameState fields it reads (+0x24, +0x26..+0x28, +0x30, +0x34, +0x38, +0x40, +0x4c, +0x4d)
// are not declared, and the branches use CMatching::SetMatchingProvider (defined), GameCenter (defined) and
// Application::GoToMainMenu (defined), but the menu-name strings and state flags are unrecovered. Returns 0 as the original does.
std::int32_t MultiplayerCallback::HandleMenuBackButton(void* /*message*/) { return 0; }

// 0x49c5e4 _ZN19MultiplayerCallback15HandleScriptCmdEPv
// Partial: case 3 (the dialog message queue
// at dword_99FB8C and dword_99FB9C, DialogMsg::s_SkipFuncName and MenuMessageManager<DialogMsg,1>::Invoke) is not
// reconstructed and does nothing here. The other cases are complete. The command and its two arguments are the
// three payload words (original +0x50, +0x54, +0x58), read through GetDataPtr since CMsgScriptCmd has no named fields.
std::int32_t MultiplayerCallback::HandleScriptCmd(void* /*message*/) {
    CMessage* message = CMessaging::GetInstance()->GetMessageFromQueue("CMsgScriptCmd");
    std::uint32_t words[3] = {0, 0, 0};
    if (const void* payload = message->GetDataPtr(); payload != nullptr) {
        std::memcpy(words, payload, sizeof words);
    }
    const auto command = static_cast<std::int32_t>(words[0]);
    const auto arg1 = static_cast<std::int32_t>(words[1]);
    const auto arg2 = static_cast<std::int32_t>(words[2]);
    if (message->IsFrom(CMatching::Get()->LocalMemberId())) {
        message->SetFinished();  // +0x3c (the original's LABEL_9)
        return 0;                // the local member sent it: nothing to do
    }
    switch (command) {
        case 0:
            ScriptManager::s_inst.StartScript(arg1, arg2, true);
            break;
        case 1:
            ScriptManager::s_inst.SkipScript(arg1, true);
            break;
        case 2:
            ScriptManager::s_inst.StopScript(arg1, true);
            break;
        case 3:
            break;  // dialog queue: not reconstructed (see above)
        case 4:
            if (!GetOnline()->IsServer() && ApplicationGetInstance().m_playerManager->unk_71a) {
                ScriptManager::s_inst.StartScript(arg1, arg2, true);
                ScriptManager::s_inst.SkipScript(arg1, true);
            }
            break;
        default:
            break;
    }
    message->SetFinished();  // +0x3c
    return 0;
}

// 0x49c7b4 _ZN19MultiplayerCallback8joinRoomEPv
// Partial, and the note it carried was WRONG: CMatching::JoinRoom HAS a definition in the tree
// (net-session-online/src/tb_session-online-CMatching-0.cpp:153, row 0x7ff120, itself partial on the
// SetEnabled(element, 0) loop), the tRoomInfo vector at OnlineGameState +0x50 IS modelled (roomInfos_, a
// std::vector<tRoomInfo> member, with tRoomInfo 0x3c8 bytes as the binary has it), and the online-state gate at +0x24 IS
// declared (flag24_). Re-read against the 0x49c7b4 disassembly (42 instructions):
//   menuName30_ = state->menuName30_;                                                   // +0x30, asm 0x49c7dc
//   if (state->flag24_ == 0) return 0x67;                                               // asm 0x49c7c0..0x49c7d4
//   if (CMatching::Get()->UnknownGate()) { CMatching::Get()->LeaveRoom(); }             // asm 0x49c7ec, 0x49c860
//   std::vector<tRoomInfo> rooms = state->roomInfos_;                                   // operator=, asm 0x49c828
//   if (mode_ (+0x34) == 2) { CMatching::Get()->GetRoomList(&rooms); }                  // asm 0x49c8a0, slot +0x90
//   const std::uint64_t roomId = *reinterpret_cast<const std::uint64_t*>(                 // asm 0x49c834..0x49c83c
//       reinterpret_cast<const std::uint8_t*>(rooms.data()) + 0x3C8 * menuName30_);
//   CMatching::Get()->JoinRoom(roomId);                                                 // asm 0x49c840
//   return 0xC8;
// The two remaining blockers are NOT missing definitions:
//  1. the mode-2 branch. Vtable +0x90 is CMatchingGLLive::GetRoomList (0x82051c, returns
//     std::vector<tRoomInfo> by value), which is a queued `skipped` row (see c_matching_gl_live.hpp:212), and
//     the port's CMatching has no declaration at +0x90 -- the same integrator decision as consequence 4 of the
//     KNOWN DEFECT block in c_matching_base.hpp. Without that branch's body the room id source differs from
//     the binary's whenever mode_ == 2.
//  2. the room id is read as the int64 at the head of the 0x3C8-byte element at index menuName30_; the port's
//     tRoomInfo has roomId at +0x00, so this arithmetic is reproducible, but the vector it indexes cannot be
//     filled on the mode-2 path (1). The mode != 2 path itself is ready.
// Returns 200, which is the value the binary leaves in r0 on the path that reaches JoinRoom.
std::int32_t MultiplayerCallback::joinRoom(void* /*context*/) { return 200; }

// 0x49c8c0 _ZN19MultiplayerCallback10createRoomEPv
// Partial (redo 2026-10-13). Declared now: PlayerSavegame (ctor, SG_Load, SG_GetCurrentPrimaryQuest,
// static m_difficultyLevel), DebugSwitches, Application::GetVersionNumber, the engine CRoomAttributes
// setters, CMatching::Get. Still missing, so the body is not written:
//  - OnlineGameState flags: the byte at +0x24 (the gate; the original returns 101 when it is clear), the
//    byte at +0x25 (set to 1 after the join call), the int at +0x34 (2 and 3 pick the attribute branch and
//    the GL Live path) and the byte at +0x4d (set to 1 on the GL Live path). OnlineGameState declares none
//    of them. Only the room-creation timeval at +0x44 (roomCreation_) is declared.
//  - CMatching::CreateJoinRoom(int, CRoomAttributes&) is not declared. CMatching::Get() takes no argument.
//  - Garbled reads in the pseudocode: RealTime and v24 are read from &v35[4*difficulty + 28] (a
//    stack-relative address past the local string), and v22 from SG_GetQuestSG()[4*difficulty + 68].
//    The assembly is needed for the element types and offsets.
//  - dword_99F778 (+8 player id fallback, +12 difficulty) and the PlayerManager instance dword_99F76C have
//    no recovered names. The GetLocalPlayer(...)[1].unknown_000 access has no declared type.
//  - The GetCurrentDifficulty callback result (the stack slot read as v29) has no declared return value.
//  - The timeval write to OnlineGameState+0x44 needs gettimeofday and the OnlineGameState accessor above.
// Returns 100 in every case. The original returns 101 when the online-state flag is clear.
std::int32_t MultiplayerCallback::createRoom(void* /*context*/) { return 100; }

// 0x49cd94 _ZN19MultiplayerCallback18HandlePlayerJoinedEPv
// Partial (redo 2026-10-13). Declared now: the current Level's m_levelListIndex (+0x3c), m_difficulty
// (+0x118), m_loadStep (+0x130, 38 = loaded) and m_seed (+0xdc); PlayerManager::IsLocalPlayerHosting and
// GetHostingPlayer; CMessage::CreateMessage(const char*, bool); CMsgStartGame (a CMessage). Still missing:
//  - The joined-player event. GetNetPlayerMgr()+0x180 (NetPlayerMgr+384) holds a CEventQueueBase, read with
//    event type 0x300000 and a 4-byte payload. CNetPlayerManager declares no member there. The gap 0x180 to
//    0x193, before player_slot_count_ at +0x194, fits a 0x14-byte CEventQueueBase. This is a layout request
//    for net-transport. Without it the player pointer is unknown, so nothing after the event read is written.
//  - The player record: +0x1a0 matches CNetPlayerInfo::conn_index_ (value), but the name bounds at +0x2e0
//    and +0x2e4 lie past the 0x288-byte object, and name_ is at +0x248. Verify against the assembly.
//  - CMsgStartGame: the payload words at +0x50, +0x54, +0x58, +0x59, +0x5c, +0x60, +0x64 and +0x68 are written
//    through raw offsets. payload_ is a private byte array, so the field names are not recovered.
//  - OnlineStatusMsg is not declared. The deque at 0x99fb34 (push_back, _M_subtract) and the statics
//    OnlineStatusMsg::s_StartFuncName, s_NodeName and s_NodeCachedChar are not recovered.
//  - StringManager::formatForMulti: the pseudocode passes a formatted argument block (sret, string, bool),
//    and the declared signature is (std::string, bool). StringManager::parse and
//    PyDataConstants::getConstant are declared.
//  - The pseudocode's CMessaging::Get(level) takes a level, and the declared CMessaging::GetInstance takes
//    none. The server-side tail appends a node to a list at dword_99F764+296 whose owner is not identified.
// Returns 0 in every case, as the original does.
std::int32_t MultiplayerCallback::HandlePlayerJoined(void* /*message*/) { return 0; }
