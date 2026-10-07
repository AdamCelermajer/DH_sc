#include "world_item_object_owner_v1.hpp"
namespace dh2::character {
namespace {struct Guard {bool& b;explicit Guard(bool& v):b(v){b=true;}~Guard(){b=false;}};}
RetainedWorldItemObjectV1::RetainedWorldItemObjectV1(std::uintptr_t id,std::shared_ptr<void> pin,data::LootTablesV2::Borrow tables,data::LootAudioVisualV8::Borrow audiovisual,WorldItemServicesV1 services)
 :base_(id,3,std::move(pin),runtime_),inventory_(std::move(tables)),audiovisual_(std::move(audiovisual)),services_(services){
 // ItemObjectC1 3ec394/3ec3a0 override the SAME GameObject fields.
 base_.lifecycle().updating85=1;base_.lifecycle().zoning2ee=0;
}
bool RetainedWorldItemObjectV1::call(WorldItemOperationV1 op,std::string& error,std::uintptr_t character,const float* position,std::int32_t integer,bool flag,data::ItemInstanceV1* item,std::int32_t* result){
 std::int32_t value{};WorldItemRequestV1 r{op,base_.identity(),character,&inventory_,item,position,integer,flag};
 if(!services_.invoke||!services_.invoke(services_.context,r,value,error)){
  failed_=true;if(error.empty())error="Required ItemObject source service "+std::to_string(static_cast<unsigned>(op));return false;
 }if(result)*result=value;return true;
}
LootItemObjectBorrowV8 RetainedWorldItemObjectV1::pool_borrow()noexcept{return {base_.identity(),3,&fields_.category3ac,&base_.lifecycle().updating85};}
bool RetainedWorldItemObjectV1::source_set_position_v57(const float* position,bool set_destination,std::string& error){
 return call(WorldItemOperationV1::set_position,error,0,position,0,set_destination);
}
void RetainedWorldItemObjectV1::bind_interaction(LootInteractServicesV8 s){interact_=std::make_unique<CharacterLootInteractV8>(base_.identity(),fields_,inventory_,s);}
bool RetainedWorldItemObjectV1::interact(std::uintptr_t user,std::string& error){
 if(running_||failed_||!interact_){error="Required retained ItemObject interaction owner";return false;}
 Guard guard(running_);if(!interact_->interact(user,error)){failed_=true;return false;}return true;
}
bool RetainedWorldItemObjectV1::interact_from_update_v11(std::uintptr_t user,std::string& error){
 if(!running_||!source_update_interact_v11_||failed_||!interact_){error="Required bounded SAME Item GameObject.Update interaction scope";return false;}
 // Original38cc78..84 invokes the Item Interact virtual synchronously from
 // inside Update. Retain its existing inner Interact guard; bypass only the
 // enclosing Item update guard during this exact source callback delivery.
 if(!interact_->interact(user,error)){failed_=true;return false;}return true;
}
bool RetainedWorldItemObjectV1::init_once(std::int32_t category,std::string& error){
 if(running_||failed_){error="ItemObject initialization cannot retry a failed prefix";return false;}Guard guard(running_);
 fields_.category3ac=static_cast<std::int16_t>(category);fields_.speed3b0=6.f;
 auto* bdae=base_.string(0x290);auto* mesh=base_.string(0x2a8);
 if(!bdae||!mesh){failed_=true;error="Required GameObject visual CString fields290/2a8";return false;}
 *bdae="data/3D/GameObjects/itemdrops.bdae";
 if(!audiovisual_){failed_=true;error="Required actual loot audiovisual table";return false;}
 *mesh=category<0||static_cast<std::size_t>(category)>=audiovisual_.rows().size()?"dummy_itemdrop_bag":audiovisual_.rows()[category].visual;
 if(!call(WorldItemOperationV1::game_init_post,error))return false;
 bool visual{};if(!services_.visual_present||!services_.visual_present(services_.context,base_.identity(),visual,error)){failed_=true;if(error.empty())error="Required same ItemObject visual2d8";return false;}
 return !visual||call(WorldItemOperationV1::apply_mesh_box,error);
}
bool RetainedWorldItemObjectV1::add_destination(void* context,std::unique_ptr<data::ItemInstanceV1>& item,bool force,bool convert,std::int32_t& result,std::string& error){
 auto& self=*static_cast<RetainedWorldItemObjectV1*>(context);
 if(!force||convert){error="ItemObject InitAgain source transfer policy changed";return false;}
 if(!self.inventory_.store(item,self.services_.inventory_debug,self.services_.context,self.services_.full_notifications,error))return false;
 result=1;return true;
}
bool RetainedWorldItemObjectV1::init_again(data::LootTemporaryInventoryV8& source,std::uint32_t index,std::uintptr_t owner,std::string& error){
 if(running_||failed_){error="ItemObject reuse cannot retry a failed prefix";return false;}Guard guard(running_);
 std::int32_t transferred{};if(!source.transfer(index,true,false,this,add_destination,transferred,error)){failed_=true;return false;}
 auto* item=inventory_.peek();if(!item){failed_=true;error="Source InitAgain GetItem(0) is NULL after transfer";return false;}
 const auto* metadata=inventory_.metadata(*item);if(!metadata){failed_=true;error="Required actual ItemObject item metadata";return false;}
 const auto category=metadata->record.words[21];
 if(category!=-1){if(!audiovisual_||category<0||static_cast<std::size_t>(category)>=audiovisual_.rows().size()){failed_=true;error="Source InitAgain audiovisual row outside actual table";return false;}
  fields_.audio_drop3b4=static_cast<std::int16_t>(audiovisual_.rows()[category].audio_drop);fields_.audio_pickup3b6=static_cast<std::int16_t>(audiovisual_.rows()[category].audio_pickup);}
 bool visual{};if(!services_.visual_present||!services_.visual_present(services_.context,base_.identity(),visual,error)){failed_=true;if(error.empty())error="Required same ItemObject visual2d8";return false;}
 if(visual&&!call(WorldItemOperationV1::visual_item_material,error,0,nullptr,0,false,item))return false;
 if(owner)fields_.owner3bc=owner;
 const float* position{};if(!services_.sound_position1a8||!services_.sound_position1a8(services_.context,base_.identity(),position,error)||!position){failed_=true;if(error.empty())error="Required actual ItemObject sound position1a8";return false;}
 if(!call(WorldItemOperationV1::drop_sound,error,0,position,fields_.audio_drop3b4,true))return false;
 if(!call(WorldItemOperationV1::create_decor_physical,error)||!call(WorldItemOperationV1::set_physical,error,0,nullptr,0,false))return false;
 // ShowGlow3ebca4 is literally bx lr. No invented glow or successful FX.
 return true;
}
bool RetainedWorldItemObjectV1::pool_operation(const LootItemRequestV8& r,std::string& error){
 if(!r.object||r.object->identity!=base_.identity()||r.object->type_index!=3){error="Pool request must borrow the same canonical ItemObject";return false;}
 switch(r.operation){
 case LootItemOperationV8::init_once:return init_once(static_cast<std::int32_t>(r.index),error);
 case LootItemOperationV8::init_again:if(!r.inventory){error="Required source pool transfer inventory";return false;}return init_again(*r.inventory,r.index,r.character,error);
 case LootItemOperationV8::enable:return call(WorldItemOperationV1::enable,error,0,nullptr,0,r.flag);
 case LootItemOperationV8::remove_all:return call(WorldItemOperationV1::remove_all,error,0,nullptr,0,r.flag);
 case LootItemOperationV8::physical:return call(WorldItemOperationV1::set_physical,error,0,nullptr,0,r.flag);
 case LootItemOperationV8::position:return call(WorldItemOperationV1::set_position,error,0,r.vector,0,r.flag);
 case LootItemOperationV8::destination:return call(WorldItemOperationV1::set_destination,error,0,r.vector);
 }error="Unknown source ItemObject pool operation";return false;
}
bool RetainedWorldItemObjectV1::is_interactive(bool& out,std::string& error){
 auto* disabled=base_.byte(0x81);auto* visible=base_.byte(0x80);
 if(!disabled||!visible){error="Required actual ItemObject visibility/disabled fields";return false;}
 out=!*disabled&&*visible&&inventory_.peek()!=nullptr;return true;
}
bool RetainedWorldItemObjectV1::update(std::uint32_t dt,std::uintptr_t ooi,std::string& error){
 if(running_||failed_){error="ItemObject update cannot retry failed prefix";return false;}Guard guard(running_);std::int32_t at{};
 if(!call(WorldItemOperationV1::is_at_destination,error,0,nullptr,0,false,nullptr,&at))return false;
 if(at&&!call(WorldItemOperationV1::stop,error))return false;
 {Guard update_scope(source_update_interact_v11_);
  if(!call(WorldItemOperationV1::game_update,error,0,nullptr,static_cast<std::int32_t>(dt)))return false;}
 if(fields_.tooltip3c8){std::int32_t visible{};if(!call(WorldItemOperationV1::tooltip_visible,error,0,nullptr,0,false,nullptr,&visible))return false;
  if(visible){if(!call(WorldItemOperationV1::tooltip_position,error))return false;
   if(fields_.tooltip_character3c4&&ooi!=base_.identity()&&!call(WorldItemOperationV1::hide_tooltip,error))return false;}
  if(!call(WorldItemOperationV1::tooltip_update,error))return false;}
 if(fields_.lock3b8>0)fields_.lock3b8=static_cast<std::int16_t>(static_cast<std::uint16_t>(fields_.lock3b8)-dt);
 return true;
}
bool RetainedWorldItemObjectV1::collision_end_v2(std::uintptr_t character,std::uintptr_t ooi,std::string& error){
 if(!character||ooi==base_.identity())return true;
 if(running_){error="Unsupported destructive ItemObject collision-end reentry";return false;}Guard guard(running_);
 // HideTooltip3ebcf8 returns immediately when source pointer3c8 is NULL.
 // Positive tooltip requires whole original visibility/Show(false,100).
 if(fields_.tooltip3c8&&!call(WorldItemOperationV1::hide_tooltip,error))return false;
 fields_.tooltip_character3c4=0;return true;
}
bool RetainedWorldItemObjectV1::collision(std::uintptr_t character,std::uintptr_t ooi,std::string& error){
 if(!character||!inventory_.peek())return true;
 if(running_||failed_){error="ItemObject collision cannot retry failed prefix";return false;}Guard guard(running_);std::int32_t type{};
 if(!call(WorldItemOperationV1::collision_interact_type,error,character,nullptr,0,false,nullptr,&type))return false;
 if(type!=-1){if(ooi!=base_.identity())return true;std::int32_t local{};
  if(!call(WorldItemOperationV1::is_local_player,error,character,nullptr,0,false,nullptr,&local))return false;
  if(local){fields_.tooltip_character3c4=character;return call(WorldItemOperationV1::show_tooltip,error,character);}return true;}
 std::int32_t moving{};if(!call(WorldItemOperationV1::is_moving,error,character,nullptr,0,false,nullptr,&moving))return false;
 if(moving){auto* p=base_.pointer(0x2e4);if(!p){failed_=true;error="Required source ItemObject collision target2e4";return false;}*p=character;}return true;
}
}
