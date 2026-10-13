// Batch e2-net-transport-CNetPlayerManager-0e46d6-1: the id-list lookups and the update entry of
// CNetPlayerManager in libDungeonHunter2.so (second half of the class's batches).
// Class: include/net-transport/tb/c_net_player_manager.hpp.
// Status per function is in ledger.d/e2-net-transport-CNetPlayerManager-0e46d6-1.csv.
//
// Not defined here (skipped, see the ledger): the deleting and complete destructors, both copies of
// the constructor, Initialize(int) and Update(int).

#include "net-transport/tb/c_net_player_manager.hpp"

#include <cstddef>
#include <vector>

#include "net-session-online/deps/matching.hpp"  // CMatching (one definition, net-session-online)
#include "net-transport/tb/cnet_player_info.hpp"

// 0x812d50 CNetPlayerManager::GetPlayerIdListByMemberId(int)
// Per-key cache in player_list_caches_ (+0x1c4). The entry is rebuilt only when it is empty and
// id_list_dirty_ (+0x1b4) is set. The rebuild clears the flag, then sets it again for every active
// player, as the original does. An active player whose conn_index_ (+0x1a0) equals the key adds its
// member_id_ (+0x178) to the entry.
std::vector<int>& CNetPlayerManager::GetPlayerIdListByMemberId(int a2) {
    if (player_list_caches_[a2].empty() && id_list_dirty_) {
        id_list_dirty_ = false;
        for (std::size_t i = 0; i < players_.size(); ++i) {
            CNetPlayerInfo* player = players_[i];
            if (player != nullptr && player->IsActive()) {
                id_list_dirty_ = true;
                if (a2 == player->conn_index_.GetValue()) {
                    player_list_caches_[a2].push_back(player->member_id_.GetValue());
                }
            }
        }
    }
    return player_list_caches_[a2];
}

// 0x812e74 CNetPlayerManager::GetMemberPlayerCount(int)
int CNetPlayerManager::GetMemberPlayerCount(int a2) {
    return static_cast<int>(GetPlayerIdListByMemberId(a2).size());
}

// 0x812e90 CNetPlayerManager::MemberHasActivePlayers(int)
bool CNetPlayerManager::MemberHasActivePlayers(int a2) {
    return GetMemberPlayerCount(a2) > 0;
}

// 0x812ea8 CNetPlayerManager::GetLocalPlayerIdList()
// Local member id from CMatching (vtable +0x6c, LocalMemberId), then the lookup above. CMatching is
// the CMatching object (net-session-online/deps/matching.hpp).
std::vector<int>& CNetPlayerManager::GetLocalPlayerIdList() {
    return GetPlayerIdListByMemberId(CMatching::Get()->LocalMemberId());
}

// 0x812ed0 CNetPlayerManager::GetLocalPlayerCount()
int CNetPlayerManager::GetLocalPlayerCount() {
    return static_cast<int>(GetLocalPlayerIdList().size());
}
