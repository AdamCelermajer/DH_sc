#pragma once

#include "../../../game-data/fresh_inventory_owned_v4.hpp"
#include "../../../level-world/application_services_owner_v5.hpp"
#include "../actor_frame/source_current_level_backend_v1.hpp"

namespace dh::foundation::inventory {
using dh2::data::FreshInventoryOwnedV4;
using dh2::data::OwnedInventoryOperationV4;
using dh2::data::OwnedInventoryRequestV4;
using dh2::data::OwnedInventoryResponseV4;

// FreshInventoryOwnedV4 retains the historical operation label
// `current_player`, but the original AddLoot call at 0x4043cc is
// Application::GetCurrentLevel (0x31f594). The second call at 0x4043dc reads
// that current Level's source +0x118 difficulty field. This adapter borrows
// the already-published root Level backend and Application PlayerManager; it
// owns no GS global, Level, Application, or PlayerManager projection.
class SourceFixedLootCurrentLevelProviderV1 final {
    std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application_;
    std::shared_ptr<dh::foundation::actor_frame::SourceCurrentLevelBackendV1> level_backend_;
    std::shared_ptr<dh2::player::ApplicationPlayerManagerBootstrapV59> player_manager_;
    dh::foundation::actor_frame::SourceCurrentLevelBorrowV1 pending_level_;
    bool pending_level_query_{};
    bool busy_{};

    bool validate(std::string&) const;
    bool query_level(std::uint32_t source_caller, OwnedInventoryResponseV4&, std::string&);
    bool query_count(OwnedInventoryResponseV4&, std::string&) const;

public:
    SourceFixedLootCurrentLevelProviderV1(
        std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>,
        std::shared_ptr<dh::foundation::actor_frame::SourceCurrentLevelBackendV1>,
        std::string& error);
    SourceFixedLootCurrentLevelProviderV1(const SourceFixedLootCurrentLevelProviderV1&) = delete;
    SourceFixedLootCurrentLevelProviderV1& operator=(const SourceFixedLootCurrentLevelProviderV1&) = delete;

    // Route only FreshInventoryOwnedV4's actual AddLoot application/PM
    // queries from the host's existing OwnedInventoryServicesV4 dispatcher.
    // Other operations remain owned by their existing native callbacks.
    bool query(const OwnedInventoryRequestV4&, OwnedInventoryResponseV4&, std::string&);
    static bool query_callback(void*, FreshInventoryOwnedV4&,
        const OwnedInventoryRequestV4&, OwnedInventoryResponseV4&, std::string&);

    bool ready() const;
};

} // namespace dh::foundation::inventory
