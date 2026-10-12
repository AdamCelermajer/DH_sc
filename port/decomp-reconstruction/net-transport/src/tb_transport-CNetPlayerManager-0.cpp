// Batch e1-net-transport-CNetPlayerManager-0: the first 20 CNetPlayerManager functions of
// libDungeonHunter2.so. Class: include/net-transport/tb/c_net_player_manager.hpp.
// Shims for the calls into other owners: include/net-transport/tb/net_player_manager_deps.hpp.

#include "net-transport/tb/c_net_player_manager.hpp"

#include <cstdint>

#include "net-transport/tb/net_player_manager_deps.hpp"

// Static state. Other batches of this class must not define these again.
// The initial value of the two factory hooks is not recovered (data section); they start null.
CNetPlayerManager* CNetPlayerManager::s_playerManager = nullptr;
CNetPlayerInfo* (*CNetPlayerManager::CreateNetPlayerInfo)() = nullptr;
void (*CNetPlayerManager::DeleteNetPlayerInfo)(CNetPlayerInfo*) = nullptr;

// 0x8100bc CNetPlayerManager::GetInstance()
CNetPlayerManager* CNetPlayerManager::GetInstance() {
    return s_playerManager;
}

// 0x8100e0 CNetPlayerManager::IsInitialized()
bool CNetPlayerManager::IsInitialized() {
    return s_playerManager != nullptr && s_playerManager->initialized_;  // +0x178
}

// 0x810108 CNetPlayerManager::Terminate()
// The original calls the object's vtable slot +0x04 (the virtual deleting destructor, presumed
// from the vtable layout). Partial: that slot is not confirmed against the class's vtable.
void CNetPlayerManager::Terminate() {
    if (s_playerManager != nullptr) {
        delete s_playerManager;
        s_playerManager = nullptr;
    }
}

// 0x81014c CNetPlayerManager::GetPlayerInternal(int)
CNetPlayerInfo* CNetPlayerManager::GetPlayerInternal(int index) const {
    if (index < 0) {
        return nullptr;
    }
    if (index >= player_slot_count_) {  // +0x194
        return nullptr;
    }
    if (static_cast<std::size_t>(index) >= players_.size()) {  // +0x198, +0x19c
        return nullptr;
    }
    return players_[static_cast<std::size_t>(index)];
}

// 0x810180 CNetPlayerManager::GetPlayerInternal(int,int)
// Partial: the first key is compared with +0x1a0 (conn_index_ in cnet_player_info.hpp), but
// GetLocalPlayer passes a member id here. Confirm the field name before merging.
CNetPlayerInfo* CNetPlayerManager::GetPlayerInternal(int first_key, int second_key) const {
    for (CNetPlayerInfo* player : players_) {
        if (player != nullptr && first_key == player->conn_index_ &&  // +0x1a0
            second_key == player->unk_1c8) {                          // +0x1c8
            return player;
        }
    }
    return nullptr;
}

// 0x8101e0 CNetPlayerManager::GetPlayer(int,bool)
// Partial: the fallback at +0x17c has no recovered name (fallback_player_ here).
CNetPlayerInfo* CNetPlayerManager::GetPlayer(int index, bool any_state) const {
    CNetPlayerInfo* player = GetPlayerInternal(index);
    if (player != nullptr && (any_state || player->IsActive())) {  // IsActive: vtable +0x5c
        return player;
    }
    return fallback_player_;  // +0x17c
}

// 0x810228 CNetPlayerManager::GetPlayerByMemberId(int,int,bool)
// Partial: same open names as GetPlayerInternal(int,int) and GetPlayer.
CNetPlayerInfo* CNetPlayerManager::GetPlayerByMemberId(int first_key, int second_key,
                                                       bool allow_inactive) const {
    CNetPlayerInfo* player = GetPlayerInternal(first_key, second_key);
    if (player != nullptr && (player->IsActive() || allow_inactive)) {  // IsActive is always called
        return player;
    }
    return fallback_player_;  // +0x17c
}

// 0x810270 CNetPlayerManager::GetFreePlayerId()
// Returns the first slot whose player has state 0 (the reset state), or -1.
int CNetPlayerManager::GetFreePlayerId() const {
    if (player_slot_count_ > 0) {  // +0x194
        for (int i = 0; i < player_slot_count_; ++i) {
            // As in the original, the state word is read without a null check.
            if (players_[static_cast<std::size_t>(i)]->state_ == 0) {  // +0x1f0
                return i;
            }
        }
    }
    return -1;
}

// 0x8102c0 CNetPlayerManager::RegisterPlayerInfoFunctions(CNetPlayerInfo* (*)(), void (*)(CNetPlayerInfo*))
// The Hex-Rays return value is the first argument passed through in r0. The source returns void.
void CNetPlayerManager::RegisterPlayerInfoFunctions(CNetPlayerInfo* (*create)(),
                                                    void (*destroy)(CNetPlayerInfo*)) {
    CreateNetPlayerInfo = create;
    DeleteNetPlayerInfo = destroy;
}

// 0x8102f0 CNetPlayerManager::UnregisterPlayerInfoFunctions()
// Partial: CNetPlayerInfo::Create and CNetPlayerInfo::Delete are not declared on the class yet;
// the shims in net_player_manager_deps.hpp stand in for them.
void CNetPlayerManager::UnregisterPlayerInfoFunctions() {
    CreateNetPlayerInfo = np_deps::CreatePlayerInfo;
    DeleteNetPlayerInfo = np_deps::DeletePlayerInfo;
}

// 0x810338 CNetPlayerManager::AreAllPlayersAcknowledged()
// Partial: the check is vtable +0x2c of the player, a NetStruct slot that is not declared yet.
bool CNetPlayerManager::AreAllPlayersAcknowledged() const {
    if (players_.empty()) {
        return true;
    }
    for (CNetPlayerInfo* player : players_) {
        if (player == nullptr || !np_deps::PlayerSlot2C(player)) {
            return false;
        }
    }
    return true;
}

// 0x8103f4 CNetPlayerManager::ResendManagerInfo()
// Partial: NetStruct::Resend (0x81347c) is a stand-in until NetStruct is merged.
int CNetPlayerManager::ResendManagerInfo() {
    return net_struct_.Resend();
}

// 0x8103fc CNetPlayerManager::ProcessLostPacket(int,int)
// Partial: NetStruct::ProcessLostPacket and vtable +0x44 of the player are not declared yet.
void CNetPlayerManager::ProcessLostPacket(int a2, int a3) {
    np_deps::NetStructProcessLostPacket(net_struct_, a2, a3);
    for (std::size_t i = 0; i < players_.size(); ++i) {
        if (players_[i] != nullptr) {
            np_deps::PlayerSlot44(players_[i], a2, a3);
        }
    }
    for (std::size_t i = 0; i < secondary_players_.size(); ++i) {
        if (secondary_players_[i] != nullptr) {
            np_deps::PlayerSlot44(secondary_players_[i], a2, a3);
        }
    }
}

// 0x8104c0 CNetPlayerManager::sProcessLostPacket(int,int)
// Forwards to the singleton (thunk).
void CNetPlayerManager::sProcessLostPacket(int a2, int a3) {
    s_playerManager->ProcessLostPacket(a2, a3);
}

// 0x8104ec CNetPlayerManager::ProcessAcknowledgedPacket(int,int)
// Partial: NetStruct::ProcessAcknowledgedPacket and vtable +0x40 of the player are not declared yet.
void CNetPlayerManager::ProcessAcknowledgedPacket(int a2, int a3) {
    np_deps::NetStructProcessAcknowledgedPacket(net_struct_, a2, a3);
    for (std::size_t i = 0; i < players_.size(); ++i) {
        if (players_[i] != nullptr) {
            np_deps::PlayerSlot40(players_[i], a2, a3);
        }
    }
    for (std::size_t i = 0; i < secondary_players_.size(); ++i) {
        if (secondary_players_[i] != nullptr) {
            np_deps::PlayerSlot40(secondary_players_[i], a2, a3);
        }
    }
}

// 0x8105b0 CNetPlayerManager::sProcessAcknowledgedPacket(int,int)
// Forwards to the singleton (thunk).
void CNetPlayerManager::sProcessAcknowledgedPacket(int a2, int a3) {
    s_playerManager->ProcessAcknowledgedPacket(a2, a3);
}

// 0x8105dc CNetPlayerManager::GetLocalPlayer(int)
// The member id comes from CMatching::LocalMemberId (vtable +0x6c, provisional name in
// connection_deps.hpp). The lookup itself is GetPlayerByMemberId, which is partial.
CNetPlayerInfo* CNetPlayerManager::GetLocalPlayer(int second_key) const {
    const int member_id = CMatching::Get()->LocalMemberId();
    return GetPlayerByMemberId(member_id, second_key, false);
}

// 0x810610 CNetPlayerManager::WritePacketData(int,int,NetBitStream &)
// Partial. Missing: the NetStruct::ConditionalSerialize body (a tail jump to SerializeInternal,
// not declared yet), the player slots +0x0c and +0x28 and +0x08 (NetStruct vtable, not declared
// yet), and the return of NetStruct::SetEnabled (unused in the original). The state branches
// follow the Hex-Rays pseudocode exactly. The loops re-read the vectors as the original does.
int CNetPlayerManager::WritePacketData(int a2, int a3, NetBitStream& stream) {
    // COnline::IsServer(GetOnline()) feeds NetStruct::SetEnabled.
    net_struct_.SetEnabled(np_deps::GetOnline()->IsServer());
    const bool is_server = CMatching::Get()->IsServer();
    int result = np_deps::NetStructConditionalSerialize(net_struct_, is_server, stream, a2, a3) != 0
                     ? 1
                     : 0;

    for (std::size_t i = 0; i < players_.size(); ++i) {
        CNetPlayerInfo* player = players_[i];
        // The state is read without a null check, as in the original.
        const unsigned int state = static_cast<unsigned int>(player->state_);  // +0x1f0
        bool send_flag = false;  // v22 in the pseudocode
        if (state <= 6) {
            const unsigned int bit = 1u << state;
            if ((bit & 0x6C) != 0) {  // states 2, 3, 5, 6
                if (player->IsLocal()) {  // vtable +0x50
                    send_flag = true;
                } else if (CMatching::Get()->IsServer() && !players_[i]->IsLocalTo(a2)) {  // +0x54
                    send_flag = true;
                }
            } else if ((bit & 0x11) != 0) {  // states 0 and 4
                send_flag = CMatching::Get()->IsServer();
            } else if ((bit & 0x2) != 0) {  // state 1
                if (CMatching::Get()->IsServer() && players_[i]->IsLocalTo(a2)) {
                    send_flag = true;
                }
            }
        }
        // Vtable +0x0c of the player, with the send flag and the stream.
        const int wrote = np_deps::PlayerSlot0C(players_[i], send_flag, &stream, a2, a3);
        if (wrote != 0) {
            result = 1;
        }
    }

    const bool has_secondary = !secondary_players_.empty();
    for (std::size_t i = 0; i < secondary_players_.size(); ++i) {
        if (secondary_players_[i] != nullptr &&
            np_deps::PlayerSlot28(secondary_players_[i], a2)) {  // vtable +0x28
            stream.WriteByte(1, 1);
            stream.WriteByte(static_cast<std::uint8_t>(secondary_players_[i]->unk_1c8), 8);  // +0x1c8
            np_deps::PlayerSlot08(secondary_players_[i], &stream, a2, a3);  // vtable +0x08
        }
    }
    if (has_secondary) {
        result = 1;
    }

    stream.WriteByte(0, 1);
    return result;
}

// 0x81086c CNetPlayerManager::sWritePacketData(int,int,NetBitStream &)
// Forwards to the singleton (thunk). The fourth Hex-Rays argument is unused.
int CNetPlayerManager::sWritePacketData(int a2, int a3, NetBitStream& stream) {
    return s_playerManager->WritePacketData(a2, a3, stream);
}

// 0x8108a8 CNetPlayerManager::RemovePlayer(int)
// Partial: the player's Remove is declared on CNetPlayerInfo, but the original's call form
// (member or static) is taken from the Hex-Rays call CNetPlayerInfo::Remove(Player).
int CNetPlayerManager::RemovePlayer(int index) {
    CNetPlayerInfo* player = GetPlayer(index, true);
    player->Remove();
    return 0;
}
