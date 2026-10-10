#pragma once
#include "source_item_descriptors.hpp"
#include "../../character_state.hpp"
#include "../../../game-data/fresh_inventory_owned_v4.hpp"
#include <map>
#include <mutex>

namespace dh::foundation::inventory {

// Root supplies this on-demand loan from SourceCharacterOwnerFactory's
// borrow_completed_character. It must pin the same canonical record/services
// that own the inventory, Power presentation and ItemText cache.
struct SourceInventoryGraphLease {
    std::shared_ptr<const void> owner;
    std::shared_ptr<const void> services_owner;
    const dh2::data::FreshInventoryOwnedV4* inventory{};
    const dh2::data::ItemPresentationOwnerV5* powers{};
    const dh2::ui::ItemTextOwnerV5* text_owner{};
    bool descriptors_current=false;
};

using SourceInventoryGraphBorrow =
    std::function<bool(SourceInventoryGraphLease&,std::string&)>;

// Indexes existing stable UI/save IDs onto the actual native vector in source
// order. It owns no inventory/items/descriptors. Root refreshes this projection
// after source save restore and after any pickup/split/merge mutation.
class SourceInventoryInstanceResolver {
public:
    struct State;
private:
    std::shared_ptr<State> state_;
public:
    explicit SourceInventoryInstanceResolver(SourceInventoryGraphBorrow);
    bool projection_bound() const noexcept;
    // Call before a native restore or mutation. Existing leases become inert
    // until a successful projection bind publishes the post-mutation order.
    void invalidate_projection() noexcept;
    bool bind_projection(const CharacterState&,std::string& error);
    bool resolve(const std::string& stable_instance_id,SourceInstanceLease&,
                 std::string& error)const;
    std::function<bool(const std::string&,SourceInstanceLease&,std::string&)>
        resolver_callback()const;
};

} // namespace dh::foundation::inventory
