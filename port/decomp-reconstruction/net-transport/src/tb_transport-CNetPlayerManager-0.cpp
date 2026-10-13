// Batch e1-net-transport-CNetPlayerManager-0: the first 20 CNetPlayerManager functions of
// libDungeonHunter2.so. Class: include/net-transport/tb/c_net_player_manager.hpp.
// The NetStruct calls (net_struct_ and the player virtuals) are real (decisions-1); the one shim left is
// COnline in include/net-transport/tb/net_player_manager_deps.hpp.

#include "net-transport/tb/c_net_player_manager.hpp"

#include <cstdint>

#include "net-transport/deps/connection_deps.hpp"  // CMatching (stand-in)

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
        if (player != nullptr && first_key == player->conn_index_.GetValue() &&  // +0x1a0
            second_key == player->unk_1c8.GetValue()) {                          // +0x1c8
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
            if (players_[static_cast<std::size_t>(i)]->state_.GetValue() == 0) {  // +0x1f0
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
// Restores the default factory: CNetPlayerInfo::Create and CNetPlayerInfo::Delete.
void CNetPlayerManager::UnregisterPlayerInfoFunctions() {
    CreateNetPlayerInfo = CNetPlayerInfo::Create;
    DeleteNetPlayerInfo = CNetPlayerInfo::Delete;
}

// 0x810338 CNetPlayerManager::AreAllPlayersAcknowledged()
// True when there are no players or every player reports AreChangesAcknowledged (vtable +0x2c). A null
// slot ends the scan with false.
bool CNetPlayerManager::AreAllPlayersAcknowledged() const {
    if (players_.empty()) {
        return true;
    }
    for (CNetPlayerInfo* player : players_) {
        if (player == nullptr || !player->AreChangesAcknowledged()) {  // vtable +0x2c
            return false;
        }
    }
    return true;
}

// 0x8103f4 CNetPlayerManager::ResendManagerInfo()
// NetStruct::Resend() (0x81347c) on the manager's replicated info. The tail call returns Resend's leftover
// R0, which nothing reads, so the port returns void.
void CNetPlayerManager::ResendManagerInfo() {
    net_struct_.Resend();
}

// 0x8103fc CNetPlayerManager::ProcessLostPacket(int,int)
// NetStruct::ProcessLostPacket on the manager's info, then vtable +0x44 (ProcessLostPacket) of every
// non-null player in both lists.
void CNetPlayerManager::ProcessLostPacket(int a2, int a3) {
    net_struct_.NetStruct::ProcessLostPacket(a2, a3);
    for (std::size_t i = 0; i < players_.size(); ++i) {
        if (players_[i] != nullptr) {
            players_[i]->ProcessLostPacket(a2, a3);  // vtable +0x44
        }
    }
    for (std::size_t i = 0; i < secondary_players_.size(); ++i) {
        if (secondary_players_[i] != nullptr) {
            secondary_players_[i]->ProcessLostPacket(a2, a3);  // vtable +0x44
        }
    }
}

// 0x8104c0 CNetPlayerManager::sProcessLostPacket(int,int)
// Forwards to the singleton (thunk).
void CNetPlayerManager::sProcessLostPacket(int a2, int a3) {
    s_playerManager->ProcessLostPacket(a2, a3);
}

// 0x8104ec CNetPlayerManager::ProcessAcknowledgedPacket(int,int)
// NetStruct::ProcessAcknowledgedPacket on the manager's info, then vtable +0x40 (ProcessAcknowledgedPacket)
// of every non-null player in both lists.
void CNetPlayerManager::ProcessAcknowledgedPacket(int a2, int a3) {
    net_struct_.NetStruct::ProcessAcknowledgedPacket(a2, a3);
    for (std::size_t i = 0; i < players_.size(); ++i) {
        if (players_[i] != nullptr) {
            players_[i]->ProcessAcknowledgedPacket(a2, a3);  // vtable +0x40
        }
    }
    for (std::size_t i = 0; i < secondary_players_.size(); ++i) {
        if (secondary_players_[i] != nullptr) {
            secondary_players_[i]->ProcessAcknowledgedPacket(a2, a3);  // vtable +0x40
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
// Partial: COnline::IsServer (GetOnline(), net-session-online) is the np_deps stand-in. The NetStruct
// calls are real: the manager info is enabled for the server and serialised with ConditionalSerialize
// (0x814cd4), each player with its vtable +0x0c (ConditionalSerialize) and the secondary players with +0x28
// (HasDataToSend) and +0x08 (Serialize). The state branches follow the Hex-Rays pseudocode exactly. The
// loops re-read the vectors as the original does.
int CNetPlayerManager::WritePacketData(int a2, int a3, NetBitStream& stream) {
    // COnline::IsServer(GetOnline()) feeds NetStruct::SetEnabled.
    net_struct_.SetEnabled(np_deps::GetOnline()->IsServer());
    const bool is_server = CMatching::Get()->IsServer();
    int result = net_struct_.ConditionalSerialize(is_server, stream, a2, a3) != 0 ? 1 : 0;

    for (std::size_t i = 0; i < players_.size(); ++i) {
        CNetPlayerInfo* player = players_[i];
        // The state is read without a null check, as in the original.
        const unsigned int state = player->state_.GetValue();  // +0x1f0
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
        // Vtable +0x0c of the player (ConditionalSerialize), with the send flag and the stream.
        const int wrote = players_[i]->ConditionalSerialize(send_flag, stream, a2, a3);
        if (wrote != 0) {
            result = 1;
        }
    }

    const bool has_secondary = !secondary_players_.empty();
    for (std::size_t i = 0; i < secondary_players_.size(); ++i) {
        if (secondary_players_[i] != nullptr &&
            secondary_players_[i]->HasDataToSend(a2)) {  // vtable +0x28
            stream.WriteByte(1, 1);
            stream.WriteByte(static_cast<std::uint8_t>(secondary_players_[i]->unk_1c8.GetValue()), 8);  // +0x1c8
            secondary_players_[i]->Serialize(stream, a2, a3);  // vtable +0x08
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
