#include "canonical_object_manager_v1.hpp"
#include <cstring>
#include <limits>
namespace dh2::world {
bool CanonicalObjectManagerV1::required(bool available,const char* name,std::string& e){if(!available){e=std::string("required source ObjectManager provider: ")+name;return false;}return true;}
const CanonicalObjectBorrowV1* CanonicalObjectManagerV1::object(std::int32_t key)const noexcept{auto i=entries_.find(key);return i==entries_.end()||!i->second.actor.identity?nullptr:&i->second.actor;}
bool CanonicalObjectManagerV1::published_name_conflict_v4(const char* name,std::int32_t room,bool& out,std::string& e)const{
 out=false;if(!name){e="Canonical adoption preflight requires actual name";return false;}
 for(const auto& entry:entries_){const auto& value=entry.second;const auto& actor=value.actor;
  if(!actor.identity||value.name!=name)continue;
  if(room==-1){out=true;return true;}
  if(!actor.room64){e="Required same receiver room64 in adoption preflight";return false;}
  if(*actor.room64==room){out=true;return true;}
  std::uint8_t global{};
  if(actor.read_across_rooms87){if(!actor.read_across_rooms87(actor.context,global,e))return false;}
  else if(actor.across_rooms87)global=*actor.across_rooms87;
  else {e="Required actual across-rooms87 in adoption preflight";return false;}
  if(global){out=true;return true;}
 }
 return true;
}
bool CanonicalObjectManagerV1::append_pending(const CanonicalObjectBorrowV1& actor,std::string& e){
 if(!actor.identity||!actor.lease||!actor.shared_handle){e="Required published source Spawn pending receiver";return false;}
 const auto* actual=object(actor.shared_handle->key);
 if(!actual||actual->identity!=actor.identity||actual->lease!=actor.lease){e="Spawn pending receiver differs from canonical manager publication";return false;}
 source_c1_fresh_v50_=false;pending_.push_back(actor.identity);return true;
}
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
 source_c1_fresh_v50_=false;const auto bits=next_key_++;std::int32_t key{};static_assert(sizeof key==sizeof bits);std::memcpy(&key,&bits,sizeof key);
 // Original counter mutation precedes allocation/insertion. Prefix survives.
 auto [i,inserted]=entries_.try_emplace(key);if(!inserted){e="source key wrapped onto an existing map node";return false;}i->second.name=name;out.key=key;return true;
}
bool CanonicalObjectManagerV1::get_handle(std::int32_t key,target_providers::Handle16& out,std::string& e){auto* a=object(key);if(!a||!a->shared_handle){e="source GetHandle requires canonical receiver";return false;}a->shared_handle->frame=frame_;out=*a->shared_handle;return true;}
bool CanonicalObjectManagerV1::resolve_handle_v4(target_providers::Handle16& handle,bool asserted,
 const CanonicalObjectBorrowV1*& out,const std::function<bool(std::string&)>& assertion,std::string& e){
 out=nullptr;std::uintptr_t result{};
 if(handle.key){
  if(!handle.cached||handle.frame!=frame_){
   // Original std::map::operator[] inserts a default node on a missing key.
   // It does not advance next_key4c or source_count50.
   auto [it,inserted]=entries_.try_emplace(handle.key);if(inserted)source_c1_fresh_v50_=false;
   handle.cached=it->second.actor.identity;handle.frame=frame_;
  }
  result=handle.cached;
 }
 if(asserted&&!result){
  if(!assertion){e="Required actual ObjectHandle NULL assertion mode/provider";return false;}
  if(!assertion(e))return false;
 }
 if(!result)return true;
 // Source cached-address fast path is allowed even when its key has changed.
 // Borrow only an actual retained receiver; a foreign cached address is not
 // turned into an invented object or dereferenced as ARM memory.
 for(const auto& entry:entries_)if(entry.second.actor.identity==result){out=&entry.second.actor;return true;}
 e="Required retained receiver for source cached ObjectHandle address";return false;
}
bool CanonicalObjectManagerV1::add(CanonicalObjectBorrowV1 actor,const char* name,const char* archetype,std::int32_t room,bool network,target_providers::Handle16& out,std::string& e){
 if(!actor.identity||!actor.lease||!actor.shared_handle||!actor.type_f4||!actor.room64||!name||!archetype){e="source Add requires actual constructed receiver fields/name/archetype";return false;}
 if(!by_name(name,room,true,nullptr,out,e))return false;
 out.frame=frame_;if(auto* prior=object(out.key)){out.cached=prior->identity;return required(services_.destroy_duplicate!=nullptr,"duplicate virtual deleting destructor",e)&&services_.destroy_duplicate(services_.context,actor,e);}
 source_c1_fresh_v50_=false;auto& entry=entries_[out.key];entry.actor=std::move(actor);auto& a=entry.actor;++count_;out.cached=a.identity;*a.shared_handle=out;
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
