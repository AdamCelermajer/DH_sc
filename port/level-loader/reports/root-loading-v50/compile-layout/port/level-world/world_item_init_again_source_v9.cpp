#include "world_item_object_owner_v1.hpp"
#include "loot_inventory_source_v9.hpp"
namespace dh2::character {
bool RetainedWorldItemObjectV1::init_again_source_v9(LootInventorySourceV9& source,
 std::uint32_t index,std::uintptr_t owner,std::string& e){
 if(running_||failed_){e="ItemObject reuse cannot retry a failed prefix";return false;}
 running_=true;struct Guard{bool& value;~Guard(){value=false;}}guard{running_};
 std::int32_t transferred{};
 if(!source.owner||!source.transfer||!source.transfer(index,true,false,this,add_destination,transferred,e)){failed_=true;return false;}
 auto* item=inventory_.peek();if(!item){failed_=true;e="Source InitAgain GetItem(0) NULL after transfer";return false;}
 const auto* metadata=inventory_.metadata(*item);if(!metadata){failed_=true;e="Required actual ItemObject item metadata";return false;}
 const auto category=metadata->record.words[21];
 if(category!=-1){if(!audiovisual_||category<0||std::size_t(category)>=audiovisual_.rows().size()){failed_=true;e="Source InitAgain audiovisual row outside table";return false;}
  fields_.audio_drop3b4=static_cast<std::int16_t>(audiovisual_.rows()[category].audio_drop);fields_.audio_pickup3b6=static_cast<std::int16_t>(audiovisual_.rows()[category].audio_pickup);}
 bool visual{};if(!services_.visual_present||!services_.visual_present(services_.context,base_.identity(),visual,e)){failed_=true;if(e.empty())e="Required same ItemObject visual2d8";return false;}
 if(visual&&!call(WorldItemOperationV1::visual_item_material,e,0,nullptr,0,false,item))return false;
 if(owner)fields_.owner3bc=owner;
 const float* position{};if(!services_.sound_position1a8||!services_.sound_position1a8(services_.context,base_.identity(),position,e)||!position){failed_=true;if(e.empty())e="Required same ItemObject sound position1a8";return false;}
 if(!call(WorldItemOperationV1::drop_sound,e,0,position,fields_.audio_drop3b4,true))return false;
 return call(WorldItemOperationV1::create_decor_physical,e)&&call(WorldItemOperationV1::set_physical,e,0,nullptr,0,false);
}
}
