#pragma once
#include "source_item_descriptors.hpp"
#include <map>

namespace dh::foundation::inventory {
// Reconstructs the native saved fields through the recovered LoadInventory
// AddPower/text routines. The single resource pin must contain these exact
// ItemTable, Power definitions, presentation and ItemText owners.
// Construct a fresh owner on resource/character replacement (Continue).
class SourceMetadataOwnerV1 {
public:
    using RowBorrow = std::function<bool(const std::string&,InventoryItem&,std::string&)>;
    SourceMetadataOwnerV1(const dh2::data::ItemTable&,
      dh2::data::ItemPowerTablesV5::Borrow,
      std::shared_ptr<dh2::data::ItemPresentationOwnerV5>,
      std::shared_ptr<const void> resource_pin,
      dh2::ui::ItemTextOwnerV5&, RowBorrow);
    ~SourceMetadataOwnerV1();
    SourceMetadataOwnerV1(const SourceMetadataOwnerV1&)=delete;
    SourceMetadataOwnerV1& operator=(const SourceMetadataOwnerV1&)=delete;
    bool resolve(const InventoryItem&,SourceInstanceLease&,std::string&);
    bool properties(const InventoryItem&,std::vector<dh2::data::GearPowerProperty12V5>&,std::string&);
    bool rank(const InventoryItem&,std::int32_t&,std::uint32_t&,std::string&);
    // Invalidates all pre-restore leases before a save/character owner swap.
    void invalidate() noexcept;
private:
    struct State;
    std::shared_ptr<State> state_;
};
}
