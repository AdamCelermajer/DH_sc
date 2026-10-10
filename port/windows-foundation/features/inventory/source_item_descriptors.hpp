#pragma once
#include "inventory_feature.hpp"
#include "../../../game-data/item_presentation_v5.hpp"
#include "../../../engine-ui/item_text_owner_v5.hpp"
#include <memory>
#include <functional>
namespace dh::foundation::inventory {
struct SourceDescriptors {std::string name,stats,requirements;};
// For existing foundation's BARE items only: derives source creation value,
// then invokes the recovered original name/stats/requirements producers.
// Temporary ItemInstance is a text projection, never an inventory/save owner.
// Shared source StringManager/StrID/varargs/class cache remain required services.
bool source_bare_item_descriptors(const InventoryItem&,const dh2::data::ItemTable&,
                                 const dh2::data::ItemTextServicesV5&,
                                 SourceDescriptors&,std::string& error);
// For a genuine powered item, callers MUST borrow its actual ItemInstance and
// ItemPresentationOwnerV5; do not collapse it to a foundation bare projection.
bool source_owned_item_descriptor(const dh2::data::ItemInstanceV1&,
                                 const dh2::data::ItemPresentationOwnerV5*,
                                 const std::string& source_field,
                                 std::string&,std::string& error);
struct SourceInstanceLease {
    // Pins the genuine same inventory owner, not an independently copied item.
    std::shared_ptr<const void> owner;
    const dh2::data::ItemInstanceV1* item=nullptr;
    const dh2::data::ItemPresentationOwnerV5* powers=nullptr;
    const dh2::ui::ItemTextOwnerV5* text_owner=nullptr;
    // Actual index in FreshInventoryOwnedV4::items() when this lease resolves.
    std::uint32_t source_index{};
    bool descriptors_current=false;
    std::function<bool(const dh2::data::ItemInstanceV1*)> owns;
};
struct SourceOwnedDescriptors {
    SourceDescriptors base;
    std::vector<std::string> powers;
    std::int32_t quantity{},value{};bool identified{};
};
// Resolver must map foundation instance ID to its genuine canonical native
// source object lease. Missing mapping fails, never falls back to bare data.
class SourceInstanceDescriptorProvider {
    const dh2::data::ItemTable& table_;
    std::function<bool(const std::string&,SourceInstanceLease&,std::string&)> resolve_;
public:
    SourceInstanceDescriptorProvider(const dh2::data::ItemTable& table,
      std::function<bool(const std::string&,SourceInstanceLease&,std::string&)> resolve):table_(table),resolve_(std::move(resolve)){}
    bool present(const InventoryItem&,SourceOwnedDescriptors&,std::string& error)const;
    bool name(const InventoryItem&,std::string&,std::string& error)const;
    bool field(const InventoryItem&,const std::string& source_path,std::string&,std::string& error)const;
};
}
