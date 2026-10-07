#pragma once
#include <cstdint>
namespace dh2::data {
// Borrowed projections of real owned item/slot identities. Slot bytes are the
// source slot index for equipment set0/set1, not a set number and slot number.
struct EquipmentItem16V3 {std::int32_t id;std::int16_t quantity;std::uint16_t reserved;std::uintptr_t identity;};
struct EquipmentSlot16V3 {EquipmentItem16V3* item;std::int8_t slots[2];std::uint8_t reserved[6];};
struct EquipmentRow12V3 {std::int32_t type,slotting;std::uint32_t stackable;};
struct EquipmentState72V3 {
 std::uintptr_t owner;EquipmentSlot16V3** items;std::uint32_t count;std::int32_t selected;
 EquipmentSlot16V3** equipment[2];std::uint32_t slots,flag1320,flag1324,reserved;
 const EquipmentRow12V3* table;std::uint32_t table_count,reserved2;
};
enum EquipmentOperationV3:std::uint32_t {split=0x3fc3e0,force_add=0x3ff5d4,
 has_like=0x3fe1cc,is_equipped=0x3fdaf0,add_quantity=0x3fa17c,delete_item=0x3fe7d8,
 update_gear_properties=0x3e08a8,skin=0x3a999c,validate_hp_mp=0x3bd140};
struct EquipmentRequest40V3 {std::uintptr_t owner;EquipmentItem16V3* item;std::uint32_t operation,caller;std::int32_t arguments[4];};
struct EquipmentResponse16V3 {EquipmentItem16V3* item;std::int32_t value;std::uint32_t index;};
struct EquipmentServices16V3 {void* context;int (*invoke)(void*,EquipmentState72V3*,const EquipmentRequest40V3*,EquipmentResponse16V3*);};
static_assert(sizeof(void*)==8&&sizeof(EquipmentItem16V3)==16&&sizeof(EquipmentSlot16V3)==16&&sizeof(EquipmentState72V3)==72&&sizeof(EquipmentRequest40V3)==40&&sizeof(EquipmentResponse16V3)==16&&sizeof(EquipmentServices16V3)==16);
}
extern "C" {
// 0 source delivered; -1 malformed; -2 missing/failed required provider;
// -3 unsupported original invalid-index/assertion continuation. Prefix kept.
// Providers may synchronously reenter these kernels, but all borrowed objects
// retained by the original caller must remain alive through its return.
int dh2_equipment_auto_v3(std::int32_t*,dh2::data::EquipmentState72V3*,std::uint32_t,const dh2::data::EquipmentServices16V3*) noexcept;
int dh2_equipment_character_auto_v3(std::int32_t*,dh2::data::EquipmentState72V3*,std::uint32_t,const dh2::data::EquipmentServices16V3*) noexcept;
int dh2_equipment_to_slot_v3(dh2::data::EquipmentState72V3*,std::uint32_t,std::uint32_t,std::uint32_t,const dh2::data::EquipmentServices16V3*) noexcept;
int dh2_equipment_from_slot_v3(dh2::data::EquipmentState72V3*,std::uint32_t,std::int32_t,const dh2::data::EquipmentServices16V3*) noexcept;
int dh2_equipment_has_two_hander_v3(std::int32_t*,const dh2::data::EquipmentState72V3*,std::uint32_t) noexcept;
int dh2_equipment_slot_taken_v3(std::int32_t*,const dh2::data::EquipmentState72V3*,std::uint32_t) noexcept;
}
