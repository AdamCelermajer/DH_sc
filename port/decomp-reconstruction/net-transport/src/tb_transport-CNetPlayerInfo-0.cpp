// Batch e1-net-transport-CNetPlayerInfo-0: CNetPlayerInfo methods from libDungeonHunter2.so.
// Completed by the integrator decisions (decisions-1): CNetPlayerInfo derives from NetStruct and declares
// its NetStructMember blocks (include/net-transport/tb/cnet_player_info.hpp), so the stores below are the
// blocks' SetValue (compare, store, NetStructMember::SetChanged) and SetEnabled is the real NetStruct one.
//
// Not written here (see the ledger): the copy constructors and operator= (they copy the NetStruct base's
// member pointer array with memcpy, so the copy would point at the source's blocks), and the CEventQueueBase
// event sends in Update.

#include "net-transport/tb/cnet_player_info.hpp"

#include "net-transport/deps/connection_deps.hpp"
#include "net-transport/tb/c_net_player_manager.hpp"

// 0x36d208 CNetPlayerInfo::IsActivating()
bool CNetPlayerInfo::IsActivating() const {
    if (member_id_.GetValue() < 0 || conn_index_.GetValue() < 0 || unk_1c8.GetValue() < 0) {
        return false;
    }
    return state_.GetValue() == 1 || state_.GetValue() == 2;
}

// 0x80f124 CNetPlayerInfo::IsActive()
bool CNetPlayerInfo::IsActive() const {
    return member_id_.GetValue() >= 0 && conn_index_.GetValue() >= 0 && unk_1c8.GetValue() >= 0 &&
           state_.GetValue() == 3;
}

// 0x80f164 CNetPlayerInfo::IsRemote()
bool CNetPlayerInfo::IsRemote() const {
    return !IsLocal();  // vtable +0x50 call
}

// 0x80f180 CNetPlayerInfo::IsInRoom()
// CMatching vtable +0x68 (IsInRoom(int)) with the connection index. CMatching is the net-transport
// stand-in, so the result is the stand-in's.
bool CNetPlayerInfo::IsInRoom() const {
    return CMatching::Get()->IsInRoom(conn_index_.GetValue());
}

// 0x80f1a0 CNetPlayerInfo::IsLocalTo(int)
bool CNetPlayerInfo::IsLocalTo(int member_index) const {
    CMatching* matching = CMatching::Get();
    if (matching->ServerMemberId() != member_index) {  // vtable +0x74
        return conn_index_.GetValue() == member_index;
    }
    if (conn_index_.GetValue() >= 0) {
        return conn_index_.GetValue() == member_index;
    }
    return true;
}

// 0x80f1ec CNetPlayerInfo::IsLocal()
bool CNetPlayerInfo::IsLocal() const {
    const bool is_server = CMatching::Get()->IsServer();
    if (!is_server || conn_index_.GetValue() >= 0) {
        return conn_index_.GetValue() == CMatching::Get()->LocalMemberId();  // vtable +0x6c
    }
    return true;
}

// 0x80f23c CNetPlayerInfo::IsHost()
// The player's member id against the host member id the manager keeps (manager +0x170, the value of the
// NetStructPlayerManager block at +0x150). GetNetPlayerMgr() is CNetPlayerManager::GetInstance().
bool CNetPlayerInfo::IsHost() const {
    return member_id_.GetValue() == CNetPlayerManager::GetInstance()->net_struct_.host_member_id_.GetValue();
}

// 0x80f260 CNetPlayerInfo::Add()
// state_ = 1 (compare, store and SetChanged are the block's SetValue). The original returns SetChanged's
// leftover R0; void here.
void CNetPlayerInfo::Add() {
    state_.SetValue(1);
}

// 0x80f27c CNetPlayerInfo::Reset()
// Virtual slot +0x4c. connection index -1, then the +0x1a8 block -1, then the state 0, each only when it
// differs (SetValue). member_id_ is left alone.
void CNetPlayerInfo::Reset() {
    conn_index_.SetValue(-1);
    unk_1c8.SetValue(-1);
    state_.SetValue(0);
}

// 0x80f2e0 CNetPlayerInfo::Remove()
// Server: clears the state and both indices. Client: sets the removed state (6). The original returns
// the last SetChanged's leftover R0; void here.
void CNetPlayerInfo::Remove() {
    if (CMatching::Get()->IsServer()) {
        state_.SetValue(0);
        conn_index_.SetValue(-1);
        unk_1c8.SetValue(-1);
    } else {
        state_.SetValue(6);
    }
}

// 0x80f374 CNetPlayerInfo::Disconnect()
// Server: state 4. Client: state 5. The original returns SetChanged's leftover R0; void here.
void CNetPlayerInfo::Disconnect() {
    if (CMatching::Get()->IsServer()) {
        state_.SetValue(4);
    } else {
        state_.SetValue(5);
    }
}

// 0x36f40c CNetPlayerInfo::SetState(char)
// The Hex-Rays text builds a temporary NetStructUInt<8u> on the stack (the compare against its uninitialised
// value word only decides whether the temporary is marked changed, which nothing reads) and passes its value to
// the +0x220 block's SetValue (vtable +0x1c). The effect is unk_240.SetValue(state), with the char widened
// to int as the store does.
void CNetPlayerInfo::SetState(char state) {
    unk_240.SetValue(static_cast<std::uint32_t>(static_cast<int>(state)));
}

// 0x80f3d0 CNetPlayerInfo::Update()   [partial]
// Virtual slot +0x48. The state machine follows the pseudocode, with the real SetEnabled and the real
// cache clear on the manager. Not ported: the CEventQueueBase::AddEvent calls on the manager's two event
// queues (+0x04: events 9437185 and 9437184 with the member id; +0x180: events 3145730 and 3145731 with
// the member id), because CNetPlayerManager does not declare those queue subobjects.
void CNetPlayerInfo::Update() {
    if (applied_state_ != static_cast<int>(state_.GetValue())) {
        CMatching* matching = CMatching::Get();
        if (matching->IsServer() && state_.GetValue() == 5) {
            Disconnect();
        }

        matching = CMatching::Get();
        if (matching->IsServer() && state_.GetValue() == 6) {
            Remove();
        }

        std::uint32_t s = state_.GetValue();
        if (s == 1) {
            if (IsLocal()) {  // vtable +0x50
                state_.SetValue(2);
            }
            s = state_.GetValue();
        }
        if (s == 2) {
            // AddEvent(manager queue +0x04, 9437185, member_id_) not ported.
            SetEnabled(true);  // NetStruct::SetEnabled (0x81366c)
            state_.SetValue(3);
            s = state_.GetValue();
        }
        if (s == 3) {
            // AddEvent(manager queue +0x04, 9437184, member_id_) not ported.
            SetEnabled(true);
            s = state_.GetValue();
        }
        if (s == 4) {
            // AddEvent(manager queue +0x180, 3145730, member_id_) not ported.
            s = state_.GetValue();
        }
        if (s == 0) {
            if (applied_state_ != 4) {
                // AddEvent(manager queue +0x180, 3145730, member_id_) not ported.
            }
            // AddEvent(manager queue +0x180, 3145731, member_id_) not ported.
            Reset();  // vtable +0x4c
            s = state_.GetValue();
        }

        applied_state_ = static_cast<int>(s);
        CNetPlayerManager::GetInstance()->ClearPlayerListCaches(conn_index_.GetValue());
    }

    if (enabled_ && state_.GetValue() == 0) {  // LABEL_19: +0x124, +0x1f0
        if (AreChangesAcknowledged()) {        // vtable +0x2c
            SetEnabled(false);
        }
    }
}
