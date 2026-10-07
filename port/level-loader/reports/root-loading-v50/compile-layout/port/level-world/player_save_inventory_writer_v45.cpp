#include "player_save_inventory_writer_v45.hpp"
#include <cstring>
namespace dh2::level {
bool player_save_inventory_writer_v45(const data::FreshInventoryOwnedV4& inventory,
 const std::vector<std::string>& powers,SavegameStreamV2& out,std::string& e){
 if(!inventory.character()||inventory.items().size()>UINT32_MAX){e="Required actual Character ItemInventory GEAR domain";return false;}
 if(!out.write_u32(std::uint32_t(inventory.gold()),e)||!out.write_u32(std::uint32_t(inventory.current_equipment()),e)||
    !out.write_u32(std::uint32_t(inventory.items().size()),e))return false;
 const auto& table=inventory.table();
 for(const auto& slot:inventory.items()){
  if(!slot||!slot->item||slot->item->id<0||std::size_t(slot->item->id)>=table.identifiers.size()){
   e="Required actual GEAR item/name source lookup";return false;
  }
  const auto& item=*slot->item;
  if(!out.write_string(table.identifiers[std::size_t(item.id)],e)||
     !out.write_u32(std::uint32_t(std::int32_t(slot->slots[0])),e)||
     !out.write_u32(std::uint32_t(std::int32_t(slot->slots[1])),e)||
     !out.write_u32(std::uint32_t(item.signed_quantity()),e)||
     !out.write_u32(std::uint32_t(item.value),e)||
     !out.write({&item.identified,1},e))return false;
  if(item.powers.size()>UINT32_MAX){e="Required bounded GEAR source power vector";return false;}
  if(!out.write_u32(std::uint32_t(item.powers.size()),e))return false;
  for(const auto id:item.powers){if(id<0||std::size_t(id)>=powers.size()){e="Required actual GEAR power-name source lookup";return false;}if(!out.write_string(powers[std::size_t(id)],e))return false;}
 }
 return true;
}
bool player_save_properties_writer_v45(const data::PropertyView& view,
 const std::uint8_t* flag,SavegameStreamV2& out,std::string& e){
 if(!flag||dh2_property_validate(&view)){e="Required SAME saved property sheet and actual Save194 byte";return false;}
 if(!out.write_u32(224,e))return false;
 for(unsigned i=0;i<224;++i)if(!out.write_u32(std::uint32_t(view.saved[i]),e))return false;
 return out.write({flag,1},e);
}
bool player_save_properties_reader_v45(data::PlayerSavegameV1& save,data::PropertyView& view,
 data::Bytes bytes,std::size_t& used,std::string& e){
 used=0;if(!save.character()||dh2_property_validate(&view)||(!bytes.data&&bytes.size)||bytes.size>UINT32_MAX){e="Required actual Character/Save/property PROP input";return false;}
 SavegameStreamV2 input(bytes);std::uint32_t count{};if(!input.read_u32(count,e)){used=input.tell();return false;}
 if(count!=224){used=input.tell();return true;} // original count-mismatch early return
 for(unsigned i=0;i<224;++i){std::uint32_t value{};if(!input.read_u32(value,e)){used=input.tell();return false;}
  const auto type=view.types[i]==-1?16u:std::uint32_t(view.types[i]);
  if(type&0x20u){std::int32_t signed_value;std::memcpy(&signed_value,&value,4);view.saved[i]=signed_value;}
 }
 std::uint8_t raw{};if(!input.read(&raw,1,e)){used=input.tell();return false;}
 save.load_property_tail194_v45(raw);used=input.tell();return true;
}
}
