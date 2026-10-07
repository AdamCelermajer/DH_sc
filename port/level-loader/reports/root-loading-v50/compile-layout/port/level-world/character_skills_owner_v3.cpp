#include "character_skills_owner_v3.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
namespace dh2::character::skills {
struct CharacterSkillOwnerV3::Impl {
 struct OwnedInstance {std::string script;Instance32 value{};};
 std::uintptr_t identity;data::PropertyView* properties;data::SkillTables::Borrow skill_tables;data::FaeryTables::Borrow faery_tables;Services16 required;
 State40 state{};std::vector<const Instance32*> slots[2];std::vector<std::unique_ptr<OwnedInstance>> instances;
 std::vector<List16> lists[2];std::vector<Row16> rows[2];std::vector<std::unique_ptr<std::string>> paths;
 std::vector<std::pair<std::string,float>> temporary_arguments;std::string error;
 Impl(std::uintptr_t owner,data::PropertyView* view,data::SkillTables::Borrow s,data::FaeryTables::Borrow f,const Services16& cb):identity(owner),properties(view),skill_tables(std::move(s)),faery_tables(std::move(f)),required(cb){
  if(!owner||!properties||dh2_property_validate(properties)||!skill_tables||!faery_tables||!required.invoke)throw std::invalid_argument("Invalid CharacterSkillOwnerV3 bindings");if(skill_tables.lists().size()<4||faery_tables.lists().empty())throw std::invalid_argument("Missing source skill/faery fallback rows");state.owner=owner;
  for(const auto& l:skill_tables.lists())lists[0].push_back({l.data(),static_cast<unsigned>(l.size()),0});for(const auto& l:faery_tables.lists())lists[1].push_back({l.data(),static_cast<unsigned>(l.size()),0});for(const auto& r:skill_tables.skills())rows[0].push_back({r.script.c_str(),static_cast<unsigned>(r.script.size()),0});for(const auto& r:faery_tables.faeries())rows[1].push_back({r.script.c_str(),static_cast<unsigned>(r.script.size()),0});sync();
 }
 void sync(){state.skills={slots[0].data(),static_cast<unsigned>(slots[0].size()),0};state.spells={slots[1].data(),static_cast<unsigned>(slots[1].size()),0};}
 unsigned selected(unsigned kind){const auto id=properties->resolved[kind?29:28];return id>=0&&static_cast<std::size_t>(id)<lists[kind].size()?static_cast<unsigned>(id):kind?0:3;}
 int external(const Request48& r,Response32& out){auto code=required.invoke(required.context,&state,&r,&out);sync();if(code){error="Required skill service failed at operation "+std::to_string(r.operation);return -1;}return 0;}
 int invoke(const Request48& r,Response32& out){
  try{if(r.kind>1)return -1;
   switch(r.operation){
    case get_list:out.list=&lists[r.kind].at(selected(r.kind));break;
    case get_row:{const auto& l=lists[r.kind].at(selected(r.kind));if(r.index>=l.count)throw std::runtime_error("Source row index exceeds live selected list");auto id=l.ids[r.index];if(id<0||std::size_t(id)>=rows[r.kind].size())throw std::runtime_error("Source table reference outside backing");out.row=&rows[r.kind][id];break;}
    case reserve_slots:slots[r.kind].reserve(r.count);sync();break;
    case arguments_begin:temporary_arguments.emplace_back("",-1.0f);break;
    case arguments_end:if(temporary_arguments.empty())throw std::runtime_error("Skill Arguments lifetime mismatch");temporary_arguments.pop_back();break;
    case construct_instance:{if(!r.instance||r.instance->reserved||!r.name)throw std::runtime_error("Malformed constructed skill");auto instance=std::make_unique<OwnedInstance>();instance->script=r.name;instance->value=*r.instance;instance->value.owner=state.owner;instance->value.script=instance->script.c_str();out.object=reinterpret_cast<std::uintptr_t>(&instance->value);instances.push_back(std::move(instance));break;}
    case append_slot:{const auto* p=reinterpret_cast<const Instance32*>(r.payload);if(p&&std::none_of(instances.begin(),instances.end(),[p](const auto& v){return &v->value==p;}))throw std::runtime_error("Unowned skill slot identity");slots[r.kind].push_back(p);sync();break;}
    case capture_path:{Response32 borrowed{};if(external(r,borrowed))return -1;if(!borrowed.row||!borrowed.row->script||borrowed.row->reserved||borrowed.row->bytes>1048576)throw std::runtime_error("Invalid source path span");auto snapshot=std::make_unique<std::string>(borrowed.row->script,borrowed.row->bytes);out.object=reinterpret_cast<std::uintptr_t>(snapshot.get());paths.push_back(std::move(snapshot));break;}
    case assign_path:{auto actual=r;if(r.payload){auto it=std::find_if(paths.begin(),paths.end(),[&r](const auto& p){return reinterpret_cast<std::uintptr_t>(p.get())==r.payload;});if(it==paths.end())throw std::runtime_error("Unknown captured path token");actual.name=(*it)->data();actual.count=static_cast<unsigned>((*it)->size());actual.payload=0;}if(!actual.name)throw std::runtime_error("Missing source path bytes");return external(actual,out);}
    case release_path:{auto it=std::find_if(paths.begin(),paths.end(),[&r](const auto& p){return reinterpret_cast<std::uintptr_t>(p.get())==r.payload;});if(it==paths.end())throw std::runtime_error("Unknown captured path teardown");paths.erase(it);break;}
    case declare_skill:if(temporary_arguments.empty()||!r.instance)throw std::runtime_error("Missing temporary skill Arguments");temporary_arguments.back()={r.instance->script,r.instance->argument_index};return external(r,out);
    default:return external(r,out);
   }return 0;
  }catch(const std::exception& e){error=e.what();sync();return -1;}
 }
 static int service(void* p,State40* s,const Request48* r,Response32* out){auto& t=*static_cast<Impl*>(p);if(s!=&t.state||!r||!out)return -1;return t.invoke(*r,*out);}
 int run(unsigned kind){error.clear();Services16 cb{this,service};auto code=kind==0?dh2_character_skills_configure(&state,&cb):kind==1?dh2_character_skills_update(&state,&cb):dh2_character_skills_cleanup(&state,kind-2,&cb);if(code<0&&error.empty())error="Source skill kernel could not continue";return code;}
};
CharacterSkillOwnerV3::CharacterSkillOwnerV3(std::uintptr_t id,data::PropertyView* v,data::SkillTables::Borrow s,data::FaeryTables::Borrow f,const Services16& cb):impl_(std::make_unique<Impl>(id,v,std::move(s),std::move(f),cb)){}
CharacterSkillOwnerV3::~CharacterSkillOwnerV3()=default;
int CharacterSkillOwnerV3::configure(){return impl_->run(0);}int CharacterSkillOwnerV3::update(){return impl_->run(1);}int CharacterSkillOwnerV3::cleanup_skills(){return impl_->run(2);}int CharacterSkillOwnerV3::cleanup_spells(){return impl_->run(3);}
int CharacterSkillOwnerV3::set_cooldown(std::uint32_t kind,std::uint32_t index,std::int32_t timer){
 auto& t=*impl_;if(kind>1||index>=t.slots[kind].size())return -1;
 const auto* slot=t.slots[kind][index];if(!slot)return 0;
 for(auto& instance:t.instances)if(&instance->value==slot){instance->value.word18=timer;return 1;}return -1;
}
const data::SkillRecord* CharacterSkillOwnerV3::skill(std::uint32_t index)const noexcept{
 try{auto& t=*impl_;const auto& list=t.lists[0].at(t.selected(0));if(index>=list.count)return nullptr;const auto id=list.ids[index];return id>=0&&std::size_t(id)<t.skill_tables.skills().size()?&t.skill_tables.skills()[id]:nullptr;}catch(...){return nullptr;}
}
int CharacterSkillOwnerV3::list_count(std::uint32_t kind,std::uint32_t* count)const noexcept{if(kind>1||!count)return -1;try{auto& t=*impl_;*count=t.lists[kind].at(t.selected(kind)).count;return 0;}catch(...){return -1;}}
const State40& CharacterSkillOwnerV3::state()const noexcept{return impl_->state;}const std::string& CharacterSkillOwnerV3::error()const noexcept{return impl_->error;}
}
