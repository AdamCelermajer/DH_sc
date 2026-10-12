// Batch e1-net-transport-CNetPlayerInfo-0: CNetPlayerInfo methods from libDungeonHunter2.so.
//
// Not written here (see the ledger): SetState, IsHost, IsInRoom, the constructors, operator=
// and the destructors. Their bodies need the NetStruct base, the net player manager and the
// NetStructMember blocks, none of which are in the tree yet.
//
// The NetStructMember::SetChanged calls (dirty flags for the replicated fields) and the
// CEventQueueBase event sends are not ported; each place is marked where it was.

#include "net-transport/tb/cnet_player_info.hpp"

#include "net-transport/deps/connection_deps.hpp"

// 0x36d208 CNetPlayerInfo::IsActivating()
bool CNetPlayerInfo::IsActivating() const {
    if (member_id_ < 0 || conn_index_ < 0 || unk_1c8 < 0) {
        return false;
    }
    return state_ == 1 || state_ == 2;
}

// 0x80f124 CNetPlayerInfo::IsActive()
bool CNetPlayerInfo::IsActive() const {
    return member_id_ >= 0 && conn_index_ >= 0 && unk_1c8 >= 0 && state_ == 3;
}

// 0x80f164 CNetPlayerInfo::IsRemote()
bool CNetPlayerInfo::IsRemote() const {
    return !IsLocal();  // vtable +0x50 call
}

// 0x80f1a0 CNetPlayerInfo::IsLocalTo(int)
bool CNetPlayerInfo::IsLocalTo(int member_index) const {
    CMatching* matching = CMatching::Get();
    if (matching->ServerMemberId() != member_index) {  // vtable +0x74 (provisional name)
        return conn_index_ == member_index;
    }
    if (conn_index_ >= 0) {
        return conn_index_ == member_index;
    }
    return true;
}

// 0x80f1ec CNetPlayerInfo::IsLocal()
bool CNetPlayerInfo::IsLocal() const {
    const bool is_server = CMatching::Get()->IsServer();
    if (!is_server || conn_index_ >= 0) {
        return conn_index_ == CMatching::Get()->LocalMemberId();  // vtable +0x6c (provisional name)
    }
    return true;
}

// 0x80f260 CNetPlayerInfo::Add()   [partial]
// The field write is done. The original then calls NetStructMember::SetChanged on the state
// member at +0x1d0 (not ported), and returns that result.
void CNetPlayerInfo::Add() {
    if (state_ != 1) {
        state_ = 1;
        // NetStructMember::SetChanged(state member) not ported.
    }
}

// 0x80f27c CNetPlayerInfo::Reset()   [partial]
// Virtual slot +0x4c. The field writes are done in the original order. The SetChanged call after
// each changed field is not ported.
void CNetPlayerInfo::Reset() {
    if (conn_index_ != -1) {
        conn_index_ = -1;
        // SetChanged(conn_index_ member) not ported.
    }
    if (unk_1c8 != -1) {
        unk_1c8 = -1;
        // SetChanged(unk_1c8 member) not ported.
    }
    if (state_ != 0) {
        state_ = 0;
        // SetChanged(state member) not ported.
    }
}

// 0x80f2e0 CNetPlayerInfo::Remove()   [partial]
// Server: clears the state and both indices. Client: sets the removed state (6).
// The SetChanged calls are not ported; the original's return value is not kept.
void CNetPlayerInfo::Remove() {
    if (CMatching::Get()->IsServer()) {
        if (state_ != 0) {
            state_ = 0;
            // SetChanged(state member) not ported.
        }
        if (conn_index_ != -1) {
            conn_index_ = -1;
            // SetChanged(conn_index_ member) not ported.
        }
        if (unk_1c8 != -1) {
            unk_1c8 = -1;
            // SetChanged(unk_1c8 member) not ported.
        }
    } else if (state_ != 6) {
        state_ = 6;
        // SetChanged(state member) not ported.
    }
}

// 0x80f374 CNetPlayerInfo::Disconnect()   [partial]
// Server: state 4. Client: state 5. The SetChanged calls are not ported; the original's return
// value is not kept.
void CNetPlayerInfo::Disconnect() {
    if (CMatching::Get()->IsServer()) {
        if (state_ != 4) {
            state_ = 4;
            // SetChanged(state member) not ported.
        }
    } else if (state_ != 5) {
        state_ = 5;
        // SetChanged(state member) not ported.
    }
}

// 0x80f3d0 CNetPlayerInfo::Update()   [partial]
// Virtual slot +0x48. The state machine follows the pseudocode. Not ported:
//   - CEventQueueBase::AddEvent calls on the net player manager queue (events 9437185,
//     9437184, 3145730, 3145731) and the NetStruct::SetEnabled(this, 1) calls that go with them;
//   - the NetStruct::SetEnabled(this, 0) at the end, which needs the NetStruct base byte at +0x124
//     and the NetStruct virtual at slot +0x2c (AreChangesAcknowledged);
//   - CNetPlayerManager::ClearPlayerListCaches(conn_index_), called at the end of the state step.
// The NetStructMemberType<unsigned int>::SetValue calls on the state member are written as plain
// assignments to state_.
void CNetPlayerInfo::Update() {
    if (applied_state_ == state_) {
        return;  // the original jumps to LABEL_19 (the tail is not ported)
    }

    CMatching* matching = CMatching::Get();
    if (matching->IsServer() && state_ == 5) {
        Disconnect();
    }

    matching = CMatching::Get();
    if (matching->IsServer() && state_ == 6) {
        Remove();
    }

    int s = state_;
    if (s == 1) {
        if (IsLocal()) {  // vtable +0x50
            state_ = 2;   // SetValue(state member, 2)
        }
        s = state_;
    }
    if (s == 2) {
        // AddEvent(net player manager queue, 9437185, member_id_) and SetEnabled(this, 1) not ported.
        state_ = 3;  // SetValue(state member, 3)
        s = state_;
    }
    if (s == 3) {
        // AddEvent(net player manager queue, 9437184, member_id_) and SetEnabled(this, 1) not ported.
        s = state_;
    }
    if (s == 4) {
        // AddEvent(net player manager queue, 3145730, member_id_) not ported.
        s = state_;
    }
    if (s == 0) {
        if (applied_state_ != 4) {
            // AddEvent(net player manager queue, 3145730, member_id_) not ported.
        }
        // AddEvent(net player manager queue, 3145731, member_id_) not ported.
        Reset();  // vtable +0x4c
        s = state_;
    }

    applied_state_ = s;
    // CNetPlayerManager::ClearPlayerListCaches(mgr, conn_index_) not ported.
}
