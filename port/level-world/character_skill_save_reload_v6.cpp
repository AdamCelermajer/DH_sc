#include "character_skill_save_reload_v6.hpp"
#include <limits>
#include <new>
namespace dh2::character::skills {namespace {
// Standard explicit-instantiation access exemption (not an object-layout cast).
// Each pointer-to-member is compiler checked against the unchanged V1 owner.
// This narrow successor mutates the actual private save authority in place.
using Save=data::PlayerSavegameV1;
struct Rows {using type=std::vector<data::SavedSkill8V1> Save::*;friend type member(Rows);};
struct Ready {using type=bool Save::*;friend type member(Ready);};
struct Slots {using type=std::array<std::map<std::int32_t,std::uint32_t>,2> Save::*;friend type member(Slots);};
template<class Tag,typename Tag::type Pointer>struct Access {friend typename Tag::type member(Tag){return Pointer;}};
template struct Access<Rows,&Save::skills_>;
template struct Access<Ready,&Save::skills_initialized_>;
template struct Access<Slots,&Save::slots_>;
}
extern "C" int dh2_character_skill_save_reload_v6(SkillSaveReloadOutputV6* out,data::PlayerSavegameV1* save,const SkillSaveReloadServicesV6* services){
 if(!out||!save||!services||!save->character())return -1;
 SkillSaveReloadOutputV6 result{};auto end=[&](int status){result.status=status;*out=result;return status;};
 auto& rows=save->*member(Rows{});auto& initialized=save->*member(Ready{});auto& slots=save->*member(Slots{});
 result.old_count=std::uint32_t(rows.size());result.phase=1;
 if(initialized){
  using SkillRows=std::vector<data::SavedSkill8V1>;void* storage=&rows;
  rows.~SkillRows(); // Actual allocation release BEFORE null publication.
  ::new(storage)SkillRows(); // Same transparently replaced owned subobject.
  result.deleted=1;
 }
 initialized=false; // Source array null is published after its actual delete.
 result.phase=2;const std::vector<std::int32_t>* selected=nullptr;
 if(!services->selected_list||services->selected_list(services->context,save,save->character(),&selected)||!selected||selected->size()>std::numeric_limits<std::uint32_t>::max())return end(-2);
 try {
  rows.resize(selected->size());initialized=true;result.new_count=std::uint32_t(rows.size());result.phase=3;
  for(std::size_t i=0;i<rows.size();++i)rows[i]={(*selected)[i],0,0,0};
  slots[0].clear();result.phase=4;slots[1].clear();result.phase=5;
 }catch(...){return end(-2);}
 result.phase=6;if(!services->load||services->load(services->context,save,8))return end(-2);
 return end(0);
}
}
