#include "character_skills.hpp"
namespace {
using namespace dh2::character::skills;
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool slots(const Slots16& v){return !v.reserved&&v.count<=65536&&(!v.count||aligned(v.items));}
bool valid(State40* s,const Services16* cb){return aligned(s)&&s->owner&&slots(s->skills)&&slots(s->spells)&&aligned(cb)&&cb->invoke;}
bool send(State40* s,const Services16* cb,const Request48& r,Response32& out){out={};return !cb->invoke(cb->context,s,&r,&out)&&slots(s->skills)&&slots(s->spells);}
bool send(State40* s,const Services16* cb,const Request48& r){Response32 out{};return send(s,cb,r,out);}
bool active(State40* s,const Services16* cb,std::uintptr_t& id,std::uintptr_t owner=0,unsigned kind=0){Response32 out{};if(!send(s,cb,{active_script,kind,0,0,0,nullptr,nullptr,owner?owner:s->owner},out))return false;id=out.object;return true;}
int invoke_instance(State40* s,const Services16* cb,const Instance32* item,bool cleanup){
 if(!aligned(item)||!item->owner||!item->script||item->reserved)return -1;std::uintptr_t id=0;if(!active(s,cb,id,item->owner,1))return -2;if(!id)return 1;Response32 result{};
 if(!send(s,cb,{set_skill,0,0,0,id,"SetSkill",item,0},result))return -2;
 if(!result.source_error){if(result.count&&!send(s,cb,{erase_results,0,0,result.count,0,nullptr,nullptr,result.object}))return -2;if(!active(s,cb,id,item->owner,1))return -2;if(!id)return -2; // source has no second null guard
  if(!send(s,cb,{cleanup?call_cleanup:call_update,0,0,0,id,cleanup?"OnSkillCleanUp":"OnSkillUpdate",nullptr,result.object}))return -2;}
 if(!send(s,cb,{release_results,0,0,0,0,nullptr,nullptr,result.object}))return -2;return 1;
}
int iterate(State40* s,const Services16* cb,unsigned kind,bool cleanup){auto count=(kind?s->spells:s->skills).count;for(unsigned i=0;i<count;++i){auto& v=kind?s->spells:s->skills;if(i>=v.count)return -2; // prevent undefined original read after destructive shrink
  const auto* item=v.items[i];if(item){auto r=invoke_instance(s,cb,item,cleanup);if(r<0)return r;}}return 1;}
}
extern "C" int dh2_character_skills_update(State40* s,const Services16* cb){if(!valid(s,cb))return -1;Response32 out{};if(!send(s,cb,{state_predicate,0,6,0,0,nullptr,nullptr,0},out))return -2;if(out.count)return 1;if(!send(s,cb,{state_predicate,0,7,0,0,nullptr,nullptr,0},out))return -2;if(out.count)return 1;auto r=iterate(s,cb,0,false);return r<0?r:iterate(s,cb,1,false);}
extern "C" int dh2_character_skills_cleanup(State40* s,unsigned kind,const Services16* cb){if(!valid(s,cb)||kind>1)return -1;return iterate(s,cb,kind,true);}
extern "C" int dh2_character_skills_configure(State40* s,const Services16* cb){
 if(!valid(s,cb))return -1;std::uintptr_t id=0;if(!active(s,cb,id)||!id)return -2;
 if(!send(s,cb,{debug_load,0,0,0,0,nullptr,nullptr,0})||!send(s,cb,{debug_switch,0,0,0,0,"Lua_LoadMemUsage",nullptr,0}))return -2;
 if(!active(s,cb,id)||!id)return -2;Response32 saved{};if(!send(s,cb,{capture_path,0,0,0,id,nullptr,nullptr,0},saved)||!saved.object)return -2;
 if(!active(s,cb,id)||!id||!send(s,cb,{assign_path,0,0,20,id,"data/scripts/skills/",nullptr,0}))return -2;
 for(unsigned kind=0;kind<2;++kind){if((kind?s->spells:s->skills).count)continue;Response32 list{};if(!send(s,cb,{get_list,kind,0,0,0,nullptr,nullptr,0},list)||!aligned(list.list)||list.list->reserved||list.list->count>65536)return -2;const auto* captured=list.list;
  if(!send(s,cb,{reserve_slots,kind,0,captured->count,0,nullptr,nullptr,0})||!send(s,cb,{arguments_begin,kind,0,0,0,"",nullptr,0}))return -2;
  for(unsigned index=0;index<captured->count;++index){if(captured->count>65536||captured->reserved)return -2;Response32 row{};if(!send(s,cb,{get_row,kind,index,0,0,nullptr,nullptr,0},row)||!aligned(row.row)||row.row->reserved||row.row->bytes>1048576||!row.row->script)return -2;
   const auto* r=row.row;const bool has_script=r->bytes!=0;std::uintptr_t instance=0;
   if(has_script){if(!active(s,cb,id)||!id||!send(s,cb,{load_file,kind,index,0,id,"_commons",nullptr,0}))return -2;Instance32 arg{s->owner,r->script,kind?0xffffffffu:index,-1,kind?-1.0f:static_cast<float>(static_cast<std::int32_t>(index)),0};
    if(!active(s,cb,id)||!id||!send(s,cb,{declare_skill,kind,index,0,id,"DeclareSkill",&arg,0}))return -2;
    Response32 loaded{};if(!active(s,cb,id)||!id||!send(s,cb,{load_file,kind,index,0,id,r->script,nullptr,0},loaded))return -2;
    if(loaded.count){arg.owner=s->owner;arg.script=r->script;Response32 made{};if(!send(s,cb,{construct_instance,kind,index,0,0,r->script,&arg,0},made)||!made.object)return -2;instance=made.object;}
   }
   if(!send(s,cb,{append_slot,kind,index,0,0,nullptr,nullptr,instance}))return -2;
   if(has_script){if(!active(s,cb,id)||!id||!send(s,cb,{reset_declaration,kind,index,0,id,"DeclareSkill",nullptr,0}))return -2;}
  }
  if(!send(s,cb,{arguments_end,kind,0,0,0,nullptr,nullptr,0}))return -2;
 }
 if(!active(s,cb,id)||!id||!send(s,cb,{assign_path,0,0,0,id,nullptr,nullptr,saved.object}))return -2;
 if(!send(s,cb,{debug_load,0,0,0,0,nullptr,nullptr,0})||!send(s,cb,{debug_switch,0,0,0,0,"Lua_LoadMemUsage",nullptr,0}))return -2;
 if(!active(s,cb,id)||!id||!send(s,cb,{init_vcb,0,0,0,id,nullptr,nullptr,0})||!send(s,cb,{release_path,0,0,0,0,nullptr,nullptr,saved.object}))return -2;return 1;
}
