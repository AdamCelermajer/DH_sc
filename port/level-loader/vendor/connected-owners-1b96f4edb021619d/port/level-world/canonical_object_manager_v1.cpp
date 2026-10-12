#include "canonical_object_manager_v1.hpp"
#include <cstring>
#include <limits>
namespace dh2::world {
bool CanonicalObjectManagerV1::required(bool available,const char* name,std::string& e){if(!available){e=std::string("required source ObjectManager provider: ")+name;return false;}return true;}
const CanonicalObjectBorrowV1* CanonicalObjectManagerV1::object(std::int32_t key)const noexcept{auto i=entries_.find(key);return i==entries_.end()||!i->second.actor.identity?nullptr:&i->second.actor;}
bool CanonicalObjectManagerV1::by_name(const char* name,std::int32_t room,bool create,const char* threat,target_providers::Handle16& out,std::string& e){
 out={0,UINT32_MAX,0};if(!name){e="source GetObjectByName null name is outside safe native domain";return false;}
 if(!std::strncmp(name,"Player",6)||!std::strncmp(name,"PlayerCharacter_",16)||!std::strncmp(name,"LocalPlayer",11))room=-1;
 if(!std::strcmp(name,"HighestThreatPlayer"))return required(services_.highest_threat!=nullptr,"HighestThreatPlayer",e)&&services_.highest_threat(services_.context,threat,room,create,out,e);
 for(auto& [key,entry]:entries_){auto& a=entry.actor;if(!a.identity)continue;
  if(room!=-1){if(!required(a.room64!=nullptr,"same receiver room64",e))return false;if(room!=*a.room64){std::uint8_t global{};if(a.read_across_rooms87){if(!a.read_across_rooms87(a.context,global,e))return false;}else {if(!required(a.across_rooms87!=nullptr,"same receiver byte87",e))return false;global=*a.across_rooms87;}if(!global)continue;}}
  bool match=entry.name==name;
  if(!match&&(!std::strcmp(name,"Player")||!std::strcmp(name,"LocalPlayer"))){
   std::uintptr_t player{},character{};
   if(!required(services_.local_player!=nullptr,"GetLocalPlayer(0,true)",e)||!services_.local_player(services_.context,player,e))return false;
   if(!required(a.shared_handle!=nullptr,"same receiver shared Handle",e))return false;
   a.shared_handle->frame=frame_;
   if(!required(a.as_character!=nullptr,"AsChar",e)||!a.as_character(a.context,character,e))return false;
   match=player==character;
  }
  if(match){out.key=key;return true;}
 }
 if(!create)return required(services_.missing_name_debug!=nullptr,"missing-name Debug load/query",e)&&services_.missing_name_debug(services_.context,e);
 const auto bits=next_key_++;std::int32_t key{};static_assert(sizeof key==sizeof bits);std::memcpy(&key,&bits,sizeof key);
 // Original counter mutation precedes allocation/insertion. Prefix survives.
 auto [i,inserted]=entries_.try_emplace(key);if(!inserted){e="source key wrapped onto an existing map node";return false;}i->second.name=name;out.key=key;return true;
}
bool CanonicalObjectManagerV1::get_handle(std::int32_t key,target_providers::Handle16& out,std::string& e){auto* a=object(key);if(!a||!a->shared_handle){e="source GetHandle requires canonical receiver";return false;}a->shared_handle->frame=frame_;out=*a->shared_handle;return true;}
bool CanonicalObjectManagerV1::add(CanonicalObjectBorrowV1 actor,const char* name,const char* archetype,std::int32_t room,bool network,target_providers::Handle16& out,std::string& e){
 if(!actor.identity||!actor.lease||!actor.shared_handle||!actor.type_f4||!actor.room64||!name||!archetype){e="source Add requires actual constructed receiver fields/name/archetype";return false;}
 if(!by_name(name,room,true,nullptr,out,e))return false;
 out.frame=frame_;if(auto* prior=object(out.key)){out.cached=prior->identity;return required(services_.destroy_duplicate!=nullptr,"duplicate virtual deleting destructor",e)&&services_.destroy_duplicate(services_.context,actor,e);}
 auto& entry=entries_[out.key];entry.actor=std::move(actor);auto& a=entry.actor;++count_;out.cached=a.identity;*a.shared_handle=out;
 if(!required(a.set_name!=nullptr,"SetName",e)||!a.set_name(a.context,name,e))return false;entry.name=name;
 if(!required(a.set_archetype!=nullptr,"CString archetype48",e)||!a.set_archetype(a.context,archetype,e))return false;*a.room64=room;
 std::uintptr_t character{};a.shared_handle->frame=frame_;
 if(!required(a.as_character!=nullptr,"GetHandle->AsChar",e)||!a.as_character(a.context,character,e))return false;
 if(character)characters_.push_back(character);
 a.shared_handle->frame=frame_;
 if(*a.type_f4==5&&!std::strcmp(archetype,"Module"))modules_.push_back(a.identity);
 if(network&&(!required(services_.assign_network_id!=nullptr,"AssignObjectNetworkId",e)||!services_.assign_network_id(services_.context,a,e)))return false;
 if(services_.published&&!services_.published(services_.context,out.key,a,character,e))return false;
 return true;
}
}
