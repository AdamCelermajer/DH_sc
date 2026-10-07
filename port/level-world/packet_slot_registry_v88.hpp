#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <string>
namespace dh2::network {
//Source process BSS a33540 (8 slots, source stride92) and bitmap a33820.
//Only the observed used byte and four callback fields are projected here.
//Packet payload storage14..5b/CPacketManager queue and transport are separate;
//this owner neither delivers packets nor fabricates an online connection.
struct PacketSlotBorrowV88 {
 std::uint8_t used{};
 std::array<std::uintptr_t,4> callback_identities{}; //actual native callbacks
};
class PacketSlotRegistryV88 final {
 std::array<PacketSlotBorrowV88,8> slots_{};
 std::uint8_t bitmap_{};
public:
 //Whole815258. Already-used slot retains all fields; source return is0.
 bool register_slot(std::uint32_t index,const std::array<std::uintptr_t,4>& callbacks,std::string& e){
  if(index>=slots_.size()){e="Packet slot outside original eight process slots";return false;}
  auto& slot=slots_[index];if(!slot.used){slot.used=1;slot.callback_identities=callbacks;bitmap_=std::uint8_t(bitmap_|(1u<<index));}
  e.clear();return true;
 }
 //Whole8152c0, unconditional writes even on an unused slot. Source writes
 //10 before4/8/c, then clears only the corresponding bitmap bit.
 bool unregister_slot(std::uint32_t index,std::string& e){
  if(index>=slots_.size()){e="Packet slot outside original eight process slots";return false;}
  auto& slot=slots_[index];slot.used=0;slot.callback_identities[3]=0;
  slot.callback_identities[0]=0;slot.callback_identities[1]=0;slot.callback_identities[2]=0;
  bitmap_=std::uint8_t(bitmap_&~(1u<<index));e.clear();return true;
 }
 const PacketSlotBorrowV88& slot(std::size_t index)const{return slots_.at(index);}
 std::uint8_t bitmap()const noexcept{return bitmap_;}
};
//Same once-created process storage for every campaign, not a new Level copy.
inline std::shared_ptr<PacketSlotRegistryV88> packet_slot_registry_process_v88(){
 static const auto owner=std::make_shared<PacketSlotRegistryV88>();return owner;
}
}
