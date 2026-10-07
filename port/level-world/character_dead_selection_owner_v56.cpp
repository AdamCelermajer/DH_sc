#include "character_dead_selection_owner_v56.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::character {
struct CharacterDeadSelectionOwnerV56::Call {
 CharacterDeadSelectionOwnerV56& owner;DeadSelect32 select;
 bool source_selection_stores_published{};
};
CharacterDeadSelectionOwnerV56::CharacterDeadSelectionOwnerV56(CharacterDeadSelectionBorrowV56 b,CharacterDeadSelectionServicesV56 s):b_(b),s_(std::move(s)){
 if(!b_.machine||!b_.properties||dh2_property_validate(b_.properties)||!b_.animations||!b_.pending3f||!b_.secondary38||
    !b_.state_services.invoke||!s_.stance_mask||!s_.stance)throw std::invalid_argument("Required actual dead-selection FSM/properties/animation/field/services");
 rows_.reserve(b_.animations->characters.size());
 for(const auto& record:b_.animations->characters){
  for(unsigned i:{3u,4u,5u,6u})if(record.fields[i].size()!=1)throw std::invalid_argument("Required original scalar CharAnim death fields3..6");
  rows_.push_back({record.fields[3][0],record.fields[4][0],record.fields[5][0],record.fields[6][0]});
 }
}
int CharacterDeadSelectionOwnerV56::query(void* p,DeadSelect32* d,const DeadSelectRequest24* q,std::uint32_t* out){
 auto& c=*static_cast<Call*>(p);auto& t=c.owner;
 if(d!=&c.select||!q||!out||q->character!=t.b_.machine->native_fsm().character)return -1;
 std::int32_t value{};bool okay=false;
 switch(q->operation){
 case dead_animation_index:value=t.b_.properties->resolved[2];okay=true;break;
 case dead_stance_bits:okay=t.s_.stance_mask(value,t.error_);break;
 case dead_anim_stance:okay=t.s_.stance(value,t.error_);break;
 default:break;
 }
 // Pending source byte is reread after each synchronous selector callback.
 // The original retains its captured row pointer, which rows_ keeps stable.
 d->pending_alternate=*t.b_.pending3f;
 if(!okay){if(t.error_.empty())t.error_="Required original dead-selection query "+std::to_string(q->operation);return -1;}
 std::memcpy(out,&value,4);return 0;
}
int CharacterDeadSelectionOwnerV56::state(void* p,StateOwnerMachine40* m,const StateOwnerRequest48* q,StateOwnerResponse8* out){
 auto& c=*static_cast<Call*>(p);auto& t=c.owner;
 if(m!=&t.b_.machine->machine()||!q||!out)return -1;
 if(!c.source_selection_stores_published){
  *t.b_.pending3f=std::uint8_t(c.select.pending_alternate);
  *t.b_.secondary38=c.select.secondary_animation;
  c.source_selection_stores_published=true;
 }
 return t.b_.state_services.invoke(t.b_.state_services.context,m,q,out);
}
int CharacterDeadSelectionOwnerV56::set(bool mode,std::uintptr_t payload,bool force){
 error_.clear();
 if(!b_.machine||!b_.properties||dh2_property_validate(b_.properties)||rows_.size()!=b_.animations->characters.size()){
  error_="Required retained SAME source animation table/dead-selection properties";return -1;
 }
 Call call{*this,{&b_.machine->machine(),rows_.data(),std::uint32_t(rows_.size()),*b_.pending3f,*b_.secondary38,0},false};
 const DeadSelectServices16 selectors{&call,query};const StateOwnerServices16 state_services{&call,state};
 const int result=dh2_character_dead_select(&call.select,mode,payload,force,&selectors,&state_services);
 // State callbacks can reenter and produce new +3f/+38 values; publish the
 // original selection once before dispatch, never overwrite those later stores.
 if(!call.source_selection_stores_published&&result==1){*b_.pending3f=std::uint8_t(call.select.pending_alternate);*b_.secondary38=call.select.secondary_animation;}
 if(result<0&&error_.empty())error_="Required source SM_SetDeadState dispatch "+std::to_string(result);
 return result;
}
}
