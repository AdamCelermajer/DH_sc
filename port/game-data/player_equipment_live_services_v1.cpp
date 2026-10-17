#include "player_equipment_live_services_v1.hpp"
#include <stdexcept>

namespace dh2::data {
namespace {
bool aligned(const void* p,std::size_t n){auto a=reinterpret_cast<std::uintptr_t>(p);return p&&a%4==0&&a<=UINTPTR_MAX-n;}
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);return b&&x<y+bn&&y<x+an;}
std::int32_t asr8(std::int32_t v){return v>=0?v/256:-std::int32_t((std::uint64_t(-std::int64_t(v))+255)/256);}
struct Callback {unsigned& depth;explicit Callback(unsigned& d):depth(d){++depth;}~Callback(){--depth;}};
struct Running {bool& running;~Running(){running=false;}};
template<class F> bool deliver(F f,std::string& e){try{if(f())return true;if(e.empty())e="Required equipment provider rejected";return false;}catch(const std::exception& x){e=x.what();return false;}catch(...){e="Required equipment provider threw";return false;}}
}
int equipment_requirements_v1(std::int32_t* out,const EquipmentRequirements32V1* f,const ItemRecord164* row) noexcept {
 if(!aligned(out,sizeof(*out))||!aligned(f,sizeof(*f))||f->present>1||(f->present&&!aligned(row,sizeof(*row)))||overlap(out,sizeof(*out),f,sizeof(*f))||(f->present&&overlap(out,sizeof(*out),row,sizeof(*row))))return -1;
 std::int32_t result=1;if(!(f->online&&f->remote)&&f->present)for(unsigned i=0;i<5;++i)if(row->words[29+i]>asr8(f->cached[i])){result=0;break;}*out=result;return 0;
}
PlayerEquipmentLiveServicesV1::PlayerEquipmentLiveServicesV1(FreshInventoryOwnedV4& i,PropertyView& p,const ClassRow* c,std::uint32_t n,ItemPowerTablesV5::Borrow powers,EquipmentLiveHooksV1& h):inventory_(&i),properties_(&p),hooks_(&h),gear_(i,p,c,n,std::move(powers)){}
bool PlayerEquipmentLiveServicesV1::graph(std::string& e)const {
 const auto* s=inventory_->properties();
 if(!s||dh2_property_validate(properties_)||properties_->base!=s->base.data()||properties_->saved!=s->saved.data()||properties_->gear!=s->gear.data()||properties_->resolved!=s->resolved.data()){e="Equipment view must project the authoritative inventory Character including live buffs";return false;}
 return true;
}
bool PlayerEquipmentLiveServicesV1::begin(std::string& e){
 e.clear();if(running_||callback_depth_){e="Unsupported destructive equipment callback reentry";return false;}
 running_=true;
 if(!binding(e)){running_=false;return false;}
 return true;
}
bool PlayerEquipmentLiveServicesV1::binding(std::string& e){
 if(!graph(e))return false;
 if(!hooks_->validate_binding||!hooks_->visual||!hooks_->required.observe_storage||!hooks_->world.invoke){e="Required equipment renderer/lifecycle/world binding unavailable";return false;}
 Callback callback(callback_depth_);
 return deliver([&]{return hooks_->validate_binding(hooks_->context,*inventory_,*properties_,e);},e);
}
bool PlayerEquipmentLiveServicesV1::query(EquipmentWorldQueryV1 q,std::uintptr_t& id,std::int32_t& value,std::string& e){
 id=0;value=0;if(!hooks_->world.invoke){e="Required actual equipment world query unavailable";return false;}
 Callback callback(callback_depth_);return deliver([&]{return hooks_->world.invoke(hooks_->world.context,q,inventory_->character(),id,value,e);},e);
}
bool PlayerEquipmentLiveServicesV1::skin(std::string& e){
 // The live field and service descriptor are re-read each refresh. V5 also
 // re-reads the field before every original visual call.
 return gear_.update_skin(hooks_->visual,hooks_->skin,e);
}
bool PlayerEquipmentLiveServicesV1::effect(void* p,FreshInventoryOwnedV4& i,const OwnedInventoryRequestV4& q,OwnedInventoryResponseV4& out,std::string& e){
 auto& s=*static_cast<PlayerEquipmentLiveServicesV1*>(p);if(s.inventory_!=&i){e="Equipment authoritative inventory identity mismatch";return false;}
 Callback callback(s.callback_depth_);if(!s.binding(e))return false;
 using O=OwnedInventoryOperationV4;
 switch(q.operation){
 case O::update_gear_properties:return s.gear_.update_properties(e);
 case O::skin:return s.skin(e);
 case O::validate_hp_mp:return s.gear_.validate_hp_mp(e);
 case O::current_player:return s.query(q.argument?EquipmentWorldQueryV1::current_difficulty:EquipmentWorldQueryV1::current_player,out.identity,out.value,e);
 case O::player_count:return s.query(EquipmentWorldQueryV1::player_count,out.identity,out.value,e);
 default:
  if(!s.hooks_->required.invoke){e="Required equipment continuation unavailable at source "+std::to_string(q.source_caller);return false;}
  return deliver([&]{return s.hooks_->required.invoke(s.hooks_->required.context,i,q,out,e);},e);
 }
}
void PlayerEquipmentLiveServicesV1::observe(void* p,FreshInventoryOwnedV4& i,const OwnedInventoryRequestV4& q){
 auto& s=*static_cast<PlayerEquipmentLiveServicesV1*>(p);
 if(s.inventory_!=&i)throw EquipmentLifecycleFailureV1("Equipment authoritative inventory identity mismatch at storage observation");
 Callback callback(s.callback_depth_);
 std::string error;
 if(!s.binding(error))throw EquipmentLifecycleFailureV1(error);
 // The genuine caller hook runs synchronously while the actual Item is still
 // alive. A failure cannot fall through to V4's reset/erase continuation.
 try{s.hooks_->required.observe_storage(s.hooks_->required.context,i,q);}
 catch(const EquipmentLifecycleFailureV1&){throw;}
 catch(const std::exception& x){throw EquipmentLifecycleFailureV1(x.what());}
 catch(...){throw EquipmentLifecycleFailureV1("Required equipment lifetime retirement provider threw");}
}
OwnedInventoryServicesV4 PlayerEquipmentLiveServicesV1::services() noexcept{return {this,effect,observe};}
bool PlayerEquipmentLiveServicesV1::meets_requirements(const ItemInstanceV1* instance,bool& result,std::string& e){
 e.clear();if(!graph(e))return false;
 std::uintptr_t id;std::int32_t value;
 if(!query(EquipmentWorldQueryV1::online,id,value,e))return false;
 EquipmentRequirements32V1 facts{};facts.online=std::uint32_t(value);
 if(value){if(!query(EquipmentWorldQueryV1::remotely_updated,id,value,e))return false;facts.remote=std::uint32_t(value);if(value){result=true;return true;}}
 if(!instance){result=true;return true;}
 auto* row=item(inventory_->table(),instance->id);if(!row){e="Equipment requirement metadata absent";return false;}
 constexpr unsigned fields[5]{19,149,150,151,152};facts.present=1;
 for(unsigned j=0;j<5;++j)facts.cached[j]=properties_->resolved[fields[j]];
 std::int32_t accepted;if(equipment_requirements_v1(&accepted,&facts,&row->record)){e="Malformed source requirement projection";return false;}
 result=accepted!=0;return true;
}
bool PlayerEquipmentLiveServicesV1::prune(std::string& e,unsigned depth){
 if(depth>18){e="Requirement pruning exceeds bounded source owner budget";return false;}
 bool changed=false;for(unsigned slot=0;slot<9;++slot){auto set=slot==1||slot==2?inventory_->current_equipment():0;auto* cell=inventory_->equipment()[set][slot];bool accepted;
  if(!meets_requirements(cell?cell->item.get():nullptr,accepted,e))return false;
  if(!accepted){if(!inventory_->unequip_from_slot(slot,-1,services(),e))return false;changed=true;}
 }
 if(changed)return gear_.update_properties(e)&&prune(e,depth+1)&&skin(e)&&gear_.validate_hp_mp(e);
 return true;
}
bool PlayerEquipmentLiveServicesV1::refresh_impl(bool requirements,std::string& e){return gear_.update_properties(e)&&(!requirements||prune(e))&&skin(e)&&gear_.validate_hp_mp(e);}
bool PlayerEquipmentLiveServicesV1::refresh(bool requirements,std::string& e){if(!begin(e))return false;Running guard{running_};return deliver([&]{return refresh_impl(requirements,e);},e);}
bool PlayerEquipmentLiveServicesV1::check_item_requirements(std::string& e){if(!begin(e))return false;Running guard{running_};return deliver([&]{return prune(e);},e);}
bool PlayerEquipmentLiveServicesV1::skin_only(std::string& e){if(!begin(e))return false;Running guard{running_};return deliver([&]{return skin(e);},e);}
bool PlayerEquipmentLiveServicesV1::equip(std::uint32_t slot,std::uint32_t index,std::string& e){if(!begin(e))return false;Running guard{running_};return deliver([&]{return inventory_->equip_to_slot(slot,index,false,services(),e)&&refresh_impl(true,e);},e);}
bool PlayerEquipmentLiveServicesV1::unequip(std::uint32_t slot,std::string& e){if(!begin(e))return false;Running guard{running_};return deliver([&]{return inventory_->unequip_from_slot(slot,-1,services(),e)&&refresh_impl(true,e);},e);}
bool PlayerEquipmentLiveServicesV1::swap(std::string& e){if(!begin(e))return false;Running guard{running_};inventory_->swap_equipment();return deliver([&]{return refresh_impl(true,e);},e);}
bool PlayerEquipmentLiveServicesV1::auto_equip(std::uint32_t index,std::int32_t& result,std::string& e){if(!begin(e))return false;Running guard{running_};return deliver([&]{return inventory_->character_auto_equip(index,result,services(),e);},e);}
}
