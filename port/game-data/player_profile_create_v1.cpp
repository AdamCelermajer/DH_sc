#include "player_profile_create_v1.hpp"
#include "data.hpp"
#include <cstring>
#include <map>

namespace dh2::data {namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
 const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
 return an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y||(an&&bn&&x<y+bn&&y<x+an);
}
bool metadata(const std::string& tag){for(const char* s:{"PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP"})if(tag==s)return true;return false;}
bool word(const PlayerMetadataWriteServicesV1& s,std::uint32_t value,std::string& error){
 const std::uint8_t bytes[]{std::uint8_t(value),std::uint8_t(value>>8),std::uint8_t(value>>16),std::uint8_t(value>>24)};
 return s.write(s.context,{bytes,4},error);
}
bool text(const PlayerMetadataWriteServicesV1& s,const std::string& value,std::string& error){
 if(value.size()>=std::size_t(INT32_MAX)){error="source metadata string outside bounded span";return false;}
 const auto count=std::uint32_t(value.size()+1);
 if(!word(s,count,error))return false;
 if(value.size()+1<count){error="source string became shorter during stream delivery";return false;}
 return s.write(s.context,{reinterpret_cast<const std::uint8_t*>(value.c_str()),count},error);
}
void append_word(std::vector<std::uint8_t>& out,std::uint32_t n){for(unsigned i=0;i<4;++i)out.push_back(std::uint8_t(n>>(8*i)));}
void patch_word(std::vector<std::uint8_t>& out,std::size_t pos,std::uint32_t n){for(unsigned i=0;i<4;++i)out[pos+i]=std::uint8_t(n>>(8*i));}
}
bool write_player_metadata_section_v1(const char* tag,const PlayerSavegameV1& save,
 const CharacterTable& characters,const std::int32_t* difficulty,
 const PlayerMetadataWriteServicesV1& stream,std::string& error){
 if(!tag||!stream.write||overlap(&error,sizeof(error),&save,sizeof(save))||overlap(&error,sizeof(error),&stream,sizeof(stream))){return false;}
 error.clear();
 try{
  if(!std::strcmp(tag,"PNAM"))return text(stream,save.name(),error);
  if(!std::strcmp(tag,"PLVL"))return word(stream,std::uint32_t(save.level()),error);
  if(!std::strcmp(tag,"PCLS")){
   const auto id=save.class_id();if(id<0||std::uint32_t(id)>characters.names.size())return true;
   if(std::uint32_t(id)==characters.names.size()){error="source PCLS id==size unsafe table read";return false;}
   const std::string captured=characters.names[std::size_t(id)];return text(stream,captured,error);
  }
  if(!std::strcmp(tag,"PDFL")){
   if(!difficulty){error="source CurrentDifficulty global required";return false;}
   return word(stream,std::uint32_t(*difficulty),error)&&word(stream,std::uint32_t(save.unlocked_difficulty()),error);
  }
  if(!std::strcmp(tag,"LNAM")){
   if(!word(stream,save.save_date(),error))return false;
   for(std::size_t i=0;i<3;++i){const auto& fields=save.level_name_fields();
    if(!word(stream,std::uint32_t(fields.word50[i]),error)||!word(stream,std::uint32_t(fields.word5c[i]),error)||!word(stream,std::uint32_t(fields.quest_wordfc[i]),error))return false;
   }return true;
  }
  if(!std::strcmp(tag,"LEPT")){for(std::size_t i=0;i<3;++i)if(!word(stream,std::uint32_t(save.level_entry_points()[i]),error))return false;return true;}
  if(!std::strcmp(tag,"LUSP")){for(std::size_t i=0;i<3;++i){const auto byte=save.use_spawn_points()[i];if(!stream.write(stream.context,{&byte,1},error))return false;}return true;}
  error="non-metadata writer provider required";return false;
 }catch(...){if(error.empty())error="metadata stream delivery threw";return false;}
}
bool serialize_player_metadata_profile_v1(const PlayerProfileIndexV1::Borrow& prior,
 const std::vector<std::string>& writers,const PlayerSavegameV1& save,
 const CharacterTable& characters,const std::int32_t* difficulty,
 std::vector<std::uint8_t>& output,std::string& error){
 if(!prior||overlap(&output,sizeof(output),&save,sizeof(save))||overlap(&error,sizeof(error),&save,sizeof(save))||overlap(&error,sizeof(error),&output,sizeof(output))){return false;}
 std::map<std::string,bool> tags;
 for(const auto& row:prior.source_sections()){
  if(std::memchr(row.tag,0,4)){error="source saveAll embedded-NUL tag span unsupported";return false;}
  tags[std::string(reinterpret_cast<const char*>(row.tag),4)]=false;
 }
 for(const auto& tag:writers){if(!metadata(tag)){error="registered non-metadata writer required";return false;}tags[tag]=true;}
 output.clear();error.clear();
 try{
  append_word(output,UINT32_MAX);
  PlayerMetadataWriteServicesV1 sink{&output,[](void* raw,Bytes bytes,std::string&){auto& v=*static_cast<std::vector<std::uint8_t>*>(raw);v.insert(v.end(),bytes.data,bytes.data+bytes.size);return true;}};
  for(const auto& pair:tags){
   const auto offset=output.size();append_word(output,0);output.insert(output.end(),pair.first.begin(),pair.first.end());const auto start=output.size();
   if(pair.second){if(!write_player_metadata_section_v1(pair.first.c_str(),save,characters,difficulty,sink,error))return false;}
   else{const auto bytes=prior.payload(pair.first.c_str());if(bytes.size)output.insert(output.end(),bytes.data,bytes.data+bytes.size);}
   if(output.size()-start>UINT32_MAX){error="profile section exceeds bounded source size";return false;}patch_word(output,offset,std::uint32_t(output.size()-start));
  }
  if(tags.size()>UINT32_MAX){error="profile tag count exceeds source word";return false;}
  patch_word(output,0,std::uint32_t(tags.size()));return true;
 }catch(...){if(error.empty())error="profile stream construction threw";return false;}
}
PlayerProfileCreateRuntimeV1::PlayerProfileCreateRuntimeV1(PlayerProfileCreateBindingsV1 b):bindings_(b){}
PlayerProfileCreateStatusV1 PlayerProfileCreateRuntimeV1::create(const std::string& name,const std::string& class_name,PlayerProfileCreateResultV1* out,std::string& error){
 const auto& b=bindings_;using Status=PlayerProfileCreateStatusV1;using Stage=PlayerProfileCreateStageV1;
 const auto separate=[&](const void* p,std::size_t size){
  if(overlap(p,size,this,sizeof(*this))||overlap(p,size,&name,sizeof(name))||overlap(p,size,&class_name,sizeof(class_name))||
     overlap(p,size,name.data(),name.size()+1)||overlap(p,size,class_name.data(),class_name.size()+1))return false;
  if((b.save&&overlap(p,size,b.save,sizeof(*b.save)))||(b.profile&&overlap(p,size,b.profile,sizeof(*b.profile)))||
     (b.loader&&overlap(p,size,b.loader,sizeof(*b.loader)))||(b.current_difficulty&&overlap(p,size,b.current_difficulty,4)))return false;
  if(b.characters){if(overlap(p,size,b.characters,sizeof(*b.characters))||overlap(p,size,b.characters->names.data(),b.characters->names.size()*sizeof(std::string)))return false;
   for(const auto& v:b.characters->names)if(overlap(p,size,v.data(),v.size()+1))return false;
  }return true;
 };
 if(!out||!b.save||!b.profile||!b.loader||!b.characters||!b.current_difficulty||&b.loader->save()!=b.save||&b.loader->profile()!=b.profile||
    !separate(out,sizeof(*out))||!separate(&error,sizeof(error))||overlap(out,sizeof(*out),&error,sizeof(error)))return Status::invalid_argument;
 if(active_)return Status::busy;
 if(entered_constructor_){error="source creation requires another fresh temporary Save";return Status::failed;}
 active_=true;struct Guard{bool& active;~Guard(){active=false;}}guard{active_};*out={};error.clear();
 const auto fail=[&](const char* text){if(error.empty())error=text;return Status::failed;};
 const auto& s=b.services;
 try{
  out->stage=Stage::class_lookup;
  for(std::size_t i=0;i<b.characters->names.size();++i){++out->name_comparisons;if(!std::strcmp(class_name.c_str(),b.characters->names[i].c_str())){out->character_class=std::int32_t(i);break;}}
  if(out->character_class!=263&&out->character_class!=290&&out->character_class!=325)return Status::rejected;
  out->stage=Stage::next_slot;if(!s.next_free_slot||!s.next_free_slot(s.context,&out->slot,error))return fail("source next-free-slot provider required");
  if(out->slot<0)return fail("bounded nonnegative new-profile slot required");
  out->stage=Stage::indexed_constructor;entered_constructor_=true;
  if(b.profile->identity||b.profile->owner||b.profile->campaign||!b.save->initialize_new_profile_metadata(out->slot,error))return fail("fresh indexed Save/profile required");
  out->stage=Stage::load_metadata;if(!b.loader->load(1,error))return fail("indexed constructor SG_Load(1) failed");
  out->stage=Stage::name;b.save->set_player_name(std::string(name.c_str()));
  out->stage=Stage::level;b.save->set_player_level(1);
  out->stage=Stage::class_store;b.save->set_class(out->character_class);
  out->stage=Stage::difficulty;if(b.save->unlocked_difficulty()<0)b.save->set_unlocked_difficulty(0);*b.current_difficulty=0;
  std::uint32_t value=0;out->stage=Stage::seeds;if(!s.seed_time||!s.seed_time(s.context,&value,error))return fail("source seed-time provider required");b.save->generate_profile_seeds(value);
  out->stage=Stage::date;if(!s.save_date_time||!s.save_date_time(s.context,&value,error))return fail("source save-date clock required");b.save->set_save_date(value);
  b.save->set_saved_properties_byte_194(1);out->stage=Stage::locations;
  if(!s.difficulty_count||!s.difficulty_count(s.context,&value,error)||!b.save->set_new_profile_locations(value,error))return fail("source difficulty table required");
  if(b.profile->identity&&!b.save->source_save_blocked()){
   bool online=false,hosting=false,flag=false;out->stage=Stage::save_mode;
   const auto query=[&](bool& result){++out->online_queries;return s.online&&s.online(s.context,&result,error);};
   if(!query(online))return fail("fresh source online provider required");
   if(online){if(!s.local_hosting||!s.local_hosting(s.context,&hosting,error))return fail("source local-hosting provider required");if(hosting&&(!s.hosting_quest_flag||!s.hosting_quest_flag(s.context,&flag,error)))return fail("source hosting quest flag required");}
   b.save->set_source_save_mode(online&&hosting&&!flag?2:1);
   if(!query(online))return fail("second fresh source online provider required");
   out->stage=Stage::save_all;
   if(!online){if(!s.save_all||!s.save_all(s.context,*b.save,*b.profile,error))return fail("source saveAll/persistence provider required");}
   else{
    if(!s.local_hosting||!s.local_hosting(s.context,&hosting,error))return fail("second local-hosting provider required");
    if(hosting){if(!s.hosting_quest_flag||!s.hosting_quest_flag(s.context,&flag,error))return fail("second hosting quest flag required");}
    if(hosting&&!flag){if(!s.save_all||!s.save_all(s.context,*b.save,*b.profile,error))return fail("source saveAll/persistence provider required");}
    else if(!s.online_save||!s.online_save(s.context,*b.save,hosting,flag,error))return fail("source online-save continuation required");
   }
   out->stage=Stage::volatile_tail;if(!query(online))return fail("third fresh source online provider required");
   if(online&&(!s.volatile_save||!s.volatile_save(s.context,*b.save,error)))return fail("source volatile-save continuation required");
  }
  out->stage=Stage::result_publication;out->source_return_published=true;out->stage=Stage::complete;return Status::complete;
 }catch(...){return fail("source profile creation provider threw");}
}
}
