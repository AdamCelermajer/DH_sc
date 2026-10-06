#pragma once
#include "item_inventory_v1.hpp"
#include "item_power_tables_v5.hpp"
#include <map>
namespace dh2::data {
enum class ItemTextOperationV5:std::uint32_t {constant=0x4c4bdc,integer_string=0x508edc,parse_varargs=0x508ef4,parse_ex=0x509aec,class_name=0x3fb090};
struct ItemTextArgumentV5 {float number{};std::int32_t integer{};const char* text{};};
struct ItemTextRequestV5 {ItemTextOperationV5 operation;std::int32_t value{};const char* group{};const char* key{};const char* input{};const ItemTextArgumentV5* arguments{};std::uint32_t count{};};
struct ItemTextResponseV5 {std::int32_t value{};std::string text;};
// parse_varargs is the distinct source 508ef4 service, not parseEx. It appends
// to the provided output string. Class-name service takes the actual row ID.
// Constants/localization/formatting and live record lookup remain required.
struct ItemTextServicesV5 {void* context{};const Item*(*metadata)(void*,const ItemInstanceV1&,std::string&){};bool(*invoke)(void*,ItemInstanceV1&,const ItemTextRequestV5&,ItemTextResponseV5&,std::string& output,std::string& error){};};
bool item_update_name_v5(ItemInstanceV1&,const ItemTextServicesV5&,std::string&);
bool item_update_stats_v5(ItemInstanceV1&,const ItemTextServicesV5&,std::string&);
bool item_update_requirements_v5(ItemInstanceV1&,const ItemTextServicesV5&,std::string&);
struct ItemPowerInstanceV5 {std::int32_t id{},sorting_order{};std::string description;};
// Additional original full Power state is tied to the actual owned V4 item
// identity. No inventory/equipment mirror. Caller must call forget BEFORE
// destroying an item, including failed creations/splits. IDs and records are
// reordered together. Source swap preserves positional SortingOrder words. Unbound formatting rejects after the genuine append prefix.
// Destructive/reentrant AddPower is explicitly unsupported on this owner.
class ItemPresentationOwnerV5 {
 ItemPowerTablesV5::Borrow tables_;std::map<ItemInstanceV1*,std::vector<ItemPowerInstanceV5>> powers_;bool running_{};
public:
 explicit ItemPresentationOwnerV5(ItemPowerTablesV5::Borrow b):tables_(std::move(b)){}
 bool add_power(ItemInstanceV1&,std::int32_t id,std::int32_t mode,const ItemTextServicesV5&,std::string&);
 const std::vector<ItemPowerInstanceV5>* powers(const ItemInstanceV1&)const noexcept;
 bool forget(ItemInstanceV1&,std::string&) noexcept;
};
}

