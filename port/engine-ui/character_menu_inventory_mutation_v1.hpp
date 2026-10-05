#pragma once
#include "../game-data/fresh_inventory_owned_v4.hpp"
#include <functional>
namespace dh2::data {
// Borrows the actual mutable V4 authority. Never owns a second inventory.
class CharacterMenuInventoryMutationV1 {
 FreshInventoryOwnedV4& inventory_;
public:
 explicit CharacterMenuInventoryMutationV1(FreshInventoryOwnedV4& inventory):inventory_(inventory){}
 FreshInventoryOwnedV4& inventory()const noexcept{return inventory_;}
 static bool make_drop_inventory(FreshInventoryOwnedV4& source,std::unique_ptr<FreshInventoryOwnedV4>&,std::string&);
 bool remove(std::uint32_t,const OwnedInventoryServicesV4&,std::string&);
 bool add_quantity(ItemInstanceV1&,std::int32_t,std::string&);
 bool callback(const std::function<bool(std::string&)>&,std::string&);
 bool transfer(std::uint32_t,FreshInventoryOwnedV4&,std::int32_t,bool,bool,std::int32_t&,
               const OwnedInventoryServicesV4&,const OwnedInventoryServicesV4&,std::string&);
};
}
