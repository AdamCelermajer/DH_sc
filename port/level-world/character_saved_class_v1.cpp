#include "character_saved_class_v1.hpp"
#include <cstring>
#include <limits>

namespace dh2::character_saved_class_v1 {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(an&&bn&&x<y+bn&&y<x+an);
}
std::int16_t half(std::uint32_t word){const auto bits=std::uint16_t(word);std::int16_t value;std::memcpy(&value,&bits,2);return value;}
bool separate(const Bindings& b,const void* p,std::size_t n,const void* ignore=nullptr){
 const auto clear=[&](const void* q,std::size_t size){return !q||q==ignore||!overlap(p,n,q,size);};
 if(!clear(b.property_cache,2)||!clear(b.template_cache,2)||!clear(b.authored,sizeof(*b.authored))||
    !clear(b.templates,sizeof(*b.templates))||!clear(b.characters,sizeof(*b.characters))||
    !clear(b.random,sizeof(*b.random))||!clear(b.current_savegame,sizeof(*b.current_savegame))||
    !clear(b.current_loader,sizeof(*b.current_loader)))return false;
 if(b.current_savegame&&*b.current_savegame&&!clear(*b.current_savegame,sizeof(**b.current_savegame)))return false;
 if(b.current_loader&&*b.current_loader&&!clear(*b.current_loader,sizeof(**b.current_loader)))return false;
 if(b.authored)for(const auto* s:{&b.authored->object_name,&b.authored->editor_template_name,
   &b.authored->template_data_class,&b.authored->template_name,&b.authored->explicit_property_name})
   if(!clear(s->data(),s->size()+1))return false;
 if(b.characters){
  if(!clear(b.characters->names.data(),b.characters->names.size()*sizeof(std::string)))return false;
  for(const auto& s:b.characters->names)if(!clear(s.data(),s.size()+1))return false;
 }
 if(b.templates){
  if(!clear(b.templates->templates.data(),b.templates->templates.size()*sizeof(character::template_factory::TemplateRecord)))return false;
  for(const auto& row:b.templates->templates)
   if(!clear(row.name.data(),row.name.size()+1)||!clear(row.property_ids.data(),row.property_ids.size()*4))return false;
 }
 return true;
}
std::int32_t find(const std::vector<std::string>& names,const char* text,Result& out){
 for(std::size_t i=0;i<names.size();++i){++out.name_comparisons;if(!std::strcmp(text,names[i].c_str()))return half(std::uint32_t(i));}
 return -1;
}
}
Runtime::Runtime(Bindings b):bindings_(b){}
Status Runtime::resolve(Result* out,std::string& error){
 const auto& b=bindings_;
 if(!out||!b.character||!b.property_cache||reinterpret_cast<std::uintptr_t>(b.property_cache)%alignof(std::int16_t)||
    (b.template_cache&&overlap(b.property_cache,2,b.template_cache,2))||
    (b.template_cache&&reinterpret_cast<std::uintptr_t>(b.template_cache)%alignof(std::int16_t))||
    !separate(b,b.property_cache,2,b.property_cache)||(b.template_cache&&!separate(b,b.template_cache,2,b.template_cache))||
    overlap(out,sizeof(*out),this,sizeof(*this))||overlap(&error,sizeof(error),this,sizeof(*this))||
    overlap(out,sizeof(*out),&error,sizeof(error))||!separate(b,out,sizeof(*out))||!separate(b,&error,sizeof(error)))return Status::invalid_argument;
 if(busy_)return Status::busy;
 struct Guard{bool& busy;~Guard(){busy=false;}}guard{busy_};busy_=true;
 *out={};error.clear();
 const auto fail=[&](const char* text){if(error.empty())error=text;return Status::failed;};
 if(*b.property_cache!=-1){out->stage=Stage::cached;out->value=*b.property_cache;return Status::complete;}
 out->stage=Stage::is_player;std::uint32_t player=0;
 if(!b.services.is_player)return fail("genuine IsPlayer provider required");
 ++out->player_queries;
 try{if(b.services.is_player(b.services.context,b.character,&player,error))return fail("IsPlayer delivery failed");}
 catch(...){return fail("IsPlayer provider threw");}
 if(player){
  if(!b.current_savegame)return fail("live gameplay Save pointer slot required");
  auto* save=*b.current_savegame;
  if(save){
   out->stage=Stage::save_load;
   if(!b.current_loader||!*b.current_loader||&(*b.current_loader)->save()!=save||save->character()!=b.character)
    return fail("gameplay Save and its LoadOwner required");
   ++out->load_calls;
   if(!(*b.current_loader)->load(1,error))return fail("gameplay SG_Load(1) failed");
  }
  out->stage=Stage::save_class;save=*b.current_savegame;
  if(save&&save->character()!=b.character)return fail("fresh gameplay Save Character differs");
  const auto saved_class=save?save->class_id():-1;
  out->stage=Stage::publication;*b.property_cache=half(std::uint32_t(saved_class));++out->property_publications;
  if(*b.property_cache==-1){
   if(!b.characters)return fail("actual CharacterTable names required");
   *b.property_cache=half(std::uint32_t(find(b.characters->names,"KnightPlayerBase",*out)));++out->property_publications;
  }
  out->stage=Stage::class_writeback;save=*b.current_savegame;
  if(save){
   if(save->character()!=b.character)return fail("class writeback Save Character differs");
   save->set_class(*b.property_cache);++out->class_writebacks;
  }
 }else{
  if(!b.authored)return fail("actual authored Character strings required");
  if(!b.authored->template_name.empty()){
   out->stage=Stage::template_lookup;
   if(!b.template_cache||!b.templates)return fail("borrowed template cache and table required");
   if(b.templates->templates.size()>UINT32_MAX)return fail("unbounded template table");
   *b.template_cache=-1;
   for(std::size_t i=0;i<b.templates->templates.size();++i){
    ++out->name_comparisons;
    if(!std::strcmp(b.authored->template_name.c_str(),b.templates->templates[i].name.c_str())){*b.template_cache=half(std::uint32_t(i));break;}
   }
   out->template_id=*b.template_cache;
   if(*b.template_cache>=0){
    const auto& ids=b.templates->templates[std::size_t(*b.template_cache)].property_ids;
    if(ids.size()>std::size_t(INT32_MAX))return fail("unsafe signed template count");
    if(!ids.empty()){
     out->stage=Stage::random;
     if(!b.random)return fail("ordinary source RNG required");
     character::template_random::Selection selection;
     const auto result=character::template_random::select_uncached_slot(b.random,std::int32_t(ids.size()),&selection);
     if(result.status!=character::template_random::Status::selected)return fail("source template draw failed");
     ++out->random_draws;out->selected_slot=result.slot_index;
     *b.property_cache=half(std::uint32_t(ids[std::size_t(result.slot_index)]));++out->property_publications;
    }
   }
  }else if(!b.authored->explicit_property_name.empty()){
   if(!b.characters)return fail("actual CharacterTable names required");
   *b.property_cache=half(std::uint32_t(find(b.characters->names,b.authored->explicit_property_name.c_str(),*out)));++out->property_publications;
  }
 }
 out->stage=Stage::complete;out->value=*b.property_cache;return Status::complete;
}
}
