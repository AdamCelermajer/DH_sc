#pragma once
#include "data.hpp"

namespace dh2::data {
// Scalar projection of original Structs::Item's 164-byte ARM32 record.
// The original vtable and string pointers at words0/2/20 are zero; lengths,
// signed words and IEEE float bits retain their source positions. Word7 holds
// the serialized Stackable byte with deterministic zero upper padding.
struct ItemRecord164 {std::int32_t words[41];};
struct ItemTextSpan16 {const std::uint8_t* data;std::uint32_t size,reserved;};
static_assert(sizeof(ItemRecord164)==164&&sizeof(ItemTextSpan16)==16);
struct Item {ItemRecord164 record{};std::string icon_name,name;};
struct ItemTable {
 std::vector<std::string> identifiers,fields;
 std::vector<Item> rows;
 std::size_t data_begin=0,data_consumed=0,names_begin=0,names_consumed=0,fields_consumed=0;
};
// Reads the first four original loot_table data/name sections and first two
// schema blocks; trailing tables/subclass schemas remain uninterpreted.
// Commits only after success, retaining the old output on malformed input.
bool load_items(Bytes records,Bytes names,Bytes fields,ItemTable&,std::string&);
const Item* item(const ItemTable&,std::int32_t id)noexcept;
std::int32_t item_id(const ItemTable&,const std::string& identifier)noexcept;
std::int32_t item_type(const Item&)noexcept;
float item_equip_value(const Item&,std::uint32_t class_index)noexcept;
}
extern "C" {
// Atomic allocation-free record decoder. text[0]/text[1] borrow IconName/Name
// byte spans from input. Return0 success/1 malformed native boundary. Output
// objects must be aligned and disjoint from each other and the input span.
int dh2_item_decode_record(dh2::data::ItemRecord164* output,
                          dh2::data::ItemTextSpan16* text,
                          std::uint32_t* consumed,const std::uint8_t* bytes,
                          std::uint32_t size);
}
