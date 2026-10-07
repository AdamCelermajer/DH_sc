#include "canonical_object_manager_v1.hpp"
#include <cstring>
#include <limits>
#include <algorithm>
#include <exception>
namespace dh2::world {
bool CanonicalObjectManagerV1::borrow_native_storage_v108(const std::shared_ptr<CanonicalObjectManagerV1>& actual,ObjectManagerNativeStorageV1& out,std::string& e){
 if(!actual||actual.get()!=this){e="ObjectManager storage borrower addressed another source receiver";return false;}
 ObjectManagerNativeStorageV1 next;next.owner=actual;next.manager_identity=reinterpret_cast<std::uintptr_t>(this);
 next.orphan4=&orphan4_v108_;next.list2c=&active2c_v102_;next.characters70=&tracked_characters70_v102_;
 next.no_room88=&no_room88_v89_;next.objects90=&next_start90_v102_;next.network100=&network100_v102_;next.removed120=&removed120_v108_;
 next.word54=&word54_v108_;next.word58=&updates58_v102_;next.word138=&word138_v108_;next.word13c=&word13c_v108_;next.word140=&word140_v108_;next.byte160=&byte160_v108_;
 const auto weak=std::weak_ptr<CanonicalObjectManagerV1>(actual);
 next.clear=[weak](ObjectManagerContainerV1 id,std::string& e){auto m=weak.lock();if(!m){e="Actual ObjectManager container owner retired";return false;}
  switch(id){
   case ObjectManagerContainerV1::list24:m->rooms24_v104_.clear();break;
   case ObjectManagerContainerV1::list3c:m->marked_for_deletion3c_v89_.clear();break;
   case ObjectManagerContainerV1::list44:m->conditions44_v102_.clear();break;
   case ObjectManagerContainerV1::list80:m->room_objects80_v105_.clear();break;
   case ObjectManagerContainerV1::tree98:m->tree98_v108_.clear();break;
   case ObjectManagerContainerV1::treeb0:m->treeb0_v108_.clear();break;
   case ObjectManagerContainerV1::treec8:m->treec8_v108_.clear();break;
   case ObjectManagerContainerV1::treee0:m->treee0_v108_.clear();break;
   case ObjectManagerContainerV1::tree108:m->deferred108_v107_.clear();break;
   case ObjectManagerContainerV1::list128:m->list128_v108_.clear();break;
   case ObjectManagerContainerV1::list130:m->list130_v108_.clear();break;
   case ObjectManagerContainerV1::tree148:m->tree148_v108_.clear();break;
   case ObjectManagerContainerV1::tree164:m->tree164_v108_.clear();break;
   case ObjectManagerContainerV1::tree17c:m->tree17c_v108_.clear();break;
   case ObjectManagerContainerV1::tree194:m->tree194_v108_.clear();break;
   default:e="Unknown original ObjectManager container offset";return false;
  }
  e.clear();return true;
 };
 next.register_no_room88=[weak](std::string& e){auto m=weak.lock();if(!m){e="Actual no-room list registration owner retired";return false;}
  return m->source_add_room_objects_v105(&m->no_room88_v89_,e);
 };
 out=std::move(next);e.clear();return true;
}
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
 if(lifecycle_busy_v1_||lifecycle_failed_v1_){e="ObjectManager publication is quiesced during/after failed native destruction";return false;}
 if(!actor.identity||!actor.lease||!actor.shared_handle){e="Required published source Spawn pending receiver";return false;}
 const auto* actual=object(actor.shared_handle->key);
 if(!actual||actual->identity!=actor.identity||actual->lease!=actor.lease){e="Spawn pending receiver differs from canonical manager publication";return false;}
 source_c1_fresh_v50_=false;source_flush_complete_v121_=false;pending_.push_back(actor.identity);return true;
}
bool CanonicalObjectManagerV1::source_rename_v114(std::uintptr_t id,const char* name,const std::function<bool(std::string&)>& actual,std::string& e){
 if(!name||!actual){e="Require actual SetName source receiver";return false;}
 for(auto& row:entries_)if(row.second.actor.identity==id){if(!actual(e))return false;row.second.name=name;return true;}
 e="SetName receiver absent from same source map";return false;
}
bool CanonicalObjectManagerV1::by_name(const char* name,std::int32_t room,bool create,const char* threat,target_providers::Handle16& out,std::string& e){
 if(lifecycle_failed_v1_){e=lifecycle_failure_v1_;return false;}
 if(create&&lifecycle_busy_v1_){e="ObjectManager allocating name lookup is quiesced during/after failed destruction";return false;}
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
 source_c1_fresh_v50_=false;source_flush_complete_v121_=false;const auto bits=next_key_++;std::int32_t key{};static_assert(sizeof key==sizeof bits);std::memcpy(&key,&bits,sizeof key);
 // Original counter mutation precedes allocation/insertion. Prefix survives.
 auto [i,inserted]=entries_.try_emplace(key);if(!inserted){e="source key wrapped onto an existing map node";return false;}i->second.name=name;out.key=key;return true;
}
bool CanonicalObjectManagerV1::get_handle(std::int32_t key,target_providers::Handle16& out,std::string& e){if(lifecycle_failed_v1_){e=lifecycle_failure_v1_;return false;}auto* a=object(key);if(!a||!a->shared_handle){e="source GetHandle requires canonical receiver";return false;}a->shared_handle->frame=frame_;out=*a->shared_handle;return true;}
bool CanonicalObjectManagerV1::resolve_handle_v4(target_providers::Handle16& handle,bool asserted,
 const CanonicalObjectBorrowV1*& out,const std::function<bool(std::string&)>& assertion,std::string& e){
 if(lifecycle_failed_v1_&&!lifecycle_busy_v1_){e=lifecycle_failure_v1_;out=nullptr;return false;}
 out=nullptr;std::uintptr_t result{};
 if(handle.key){
  if(!handle.cached||handle.frame!=frame_){
   // Original std::map::operator[] inserts a default node on a missing key.
   // It does not advance next_key4c or source_count50.
   auto [it,inserted]=entries_.try_emplace(handle.key);if(inserted){source_c1_fresh_v50_=false;source_flush_complete_v121_=false;}
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
 if(lifecycle_busy_v1_||lifecycle_failed_v1_){e="ObjectManager Add is quiesced during/after failed native destruction";return false;}
 if(!actor.identity||!actor.lease||!actor.shared_handle||!actor.type_f4||!actor.room64||!name||!archetype){e="source Add requires actual constructed receiver fields/name/archetype";return false;}
 if(!by_name(name,room,true,nullptr,out,e))return false;
 out.frame=frame_;if(auto* prior=object(out.key)){out.cached=prior->identity;return required(services_.destroy_duplicate!=nullptr,"duplicate virtual deleting destructor",e)&&services_.destroy_duplicate(services_.context,actor,e);}
 source_c1_fresh_v50_=false;source_flush_complete_v121_=false;auto& entry=entries_[out.key];entry.actor=std::move(actor);auto& a=entry.actor;++count_;out.cached=a.identity;*a.shared_handle=out;
 // Map publication precedes SetName/AsChar. Keep partial Add prefixes visible
 // to the same localization scene without issuing another receiver method.
 if(!language_registry_v109_.source_added({out.key,a.identity,a.lease,a.type_f4,a.type14_localization_valid819,0},e))return false;
 if(!required(a.set_name!=nullptr,"SetName",e)||!a.set_name(a.context,name,e))return false;entry.name=name;
 if(!required(a.set_archetype!=nullptr,"CString archetype48",e)||!a.set_archetype(a.context,archetype,e))return false;*a.room64=room;
 std::uintptr_t character{};a.shared_handle->frame=frame_;
 if(!required(a.as_character!=nullptr,"GetHandle->AsChar",e)||!a.as_character(a.context,character,e))return false;
 if(character){characters_.push_back(character);if(!language_registry_v109_.source_character_added(out.key,character,e))return false;}
 a.shared_handle->frame=frame_;
 if(*a.type_f4==5&&!std::strcmp(archetype,"Module"))modules_.push_back(a.identity);
 if(network&&(!required(services_.assign_network_id!=nullptr,"AssignObjectNetworkId",e)||!services_.assign_network_id(services_.context,a,e)))return false;
 if(services_.published&&!services_.published(services_.context,out.key,a,character,e))return false;
 return true;
}
bool CanonicalObjectManagerV1::lifecycle_fail_v1(std::string& e){lifecycle_failed_v1_=true;if(lifecycle_failure_v1_.empty())lifecycle_failure_v1_=e.empty()?"Required actual ObjectManager native lifecycle leaf; prefix retained":e;e=lifecycle_failure_v1_;return false;}
bool CanonicalObjectManagerV1::lifecycle_admit_v1(const CanonicalObjectLifecycleV1& s,ObjectManagerNativeStorageV1& f,std::string& e){
 if(lifecycle_failed_v1_||lifecycle_busy_v1_){e=lifecycle_failure_v1_.empty()?"ObjectManager native lifecycle cannot replay/reenter":lifecycle_failure_v1_;return false;}
 if(!s.owner||!s.require_quiescent||!s.native_storage||!s.require_quiescent(*this,e)||!s.native_storage(*this,f,e)||!f.owner||f.manager_identity!=reinterpret_cast<std::uintptr_t>(this)){if(e.empty())e="Required SAME ObjectManager storage and actual delivery quiescence";return false;}
 return true;
}
bool CanonicalObjectManagerV1::remove_source_v1(target_providers::Handle16 handle,const CanonicalObjectLifecycleV1& s,std::string& e){
 ObjectManagerNativeStorageV1 f;if(!lifecycle_admit_v1(s,f,e))return false;
 lifecycle_busy_v1_=true;source_c1_fresh_v50_=false;source_flush_complete_v121_=false;struct Guard{bool& b;~Guard(){b=false;}}guard{lifecycle_busy_v1_};
 auto fail=[&]{return lifecycle_fail_v1(e);};
 auto receiver=[&](std::uintptr_t id,CanonicalObjectBorrowV1& out){if(!id){e="Original lifecycle reaches NULL receiver outside safe native domain";return false;}for(const auto& entry:entries_)if(entry.second.actor.identity==id&&entry.second.actor.lease){out=entry.second.actor;return true;}if(s.native_receiver&&s.native_receiver(id,out,e)&&out.identity==id&&out.lease)return true;if(e.empty())e="Required existing actual lifecycle receiver";return false;};
 auto resolve=[&](CanonicalObjectBorrowV1& out){out={};const CanonicalObjectBorrowV1* p{};if(!resolve_handle_v4(handle,false,p,{},e))return false;if(p)out=*p;return true;};
 auto character=[&](std::uintptr_t& id){id=0;CanonicalObjectBorrowV1 a;if(!resolve(a))return false;if(!a.identity)return true;if(!a.as_character){e="Required original Handle Character conversion";return false;}return a.as_character(a.context,id,e);};
 auto remove_all=[](auto& list,std::uintptr_t id){list.erase(std::remove(list.begin(),list.end(),id),list.end());};
 auto remove_first=[](auto& list,std::uintptr_t id){auto i=std::find(list.begin(),list.end(),id);if(i!=list.end())list.erase(i);};
 try{
  CanonicalObjectBorrowV1 first;if(!resolve(first))return fail();bool game=false;ObjectManagerGameObjectFieldsV1 g;
  if(first.identity&&(!s.game_object||!s.game_object(first,game,g,e)))return fail();
  if(game){
   if(!g.owner||g.owner.owner_before(first.lease)||first.lease.owner_before(g.owner)||g.identity!=first.identity||!g.room_zone2f4||!g.no_room2f8||!g.orphan2fc){e="Required SAME GameObject2f4/2f8/2fc cells";return fail();}
   if(*g.room_zone2f4&&(!s.room_remove||!s.room_remove(g,*g.room_zone2f4,e)))return fail();
   if(!f.no_room88||!f.objects90){e="Required actual no-room88/object90 lists";return fail();}
   auto i=std::find(f.no_room88->begin(),f.no_room88->end(),g.identity);if(i!=f.no_room88->end()){f.no_room88->erase(i);*g.no_room2f8=0;}
   remove_first(*f.objects90,g.identity);
  }
  CanonicalObjectBorrowV1 a;if(!resolve(a))return fail();if(!f.list2c){e="Required actual ObjectBase list2c";return fail();}remove_all(*f.list2c,a.identity);
  std::uintptr_t ch{};if(!character(ch))return fail();if(!f.characters70){e="Required actual Character list70";return fail();}remove_all(*f.characters70,ch);
  if(!character(ch))return fail();if(ch){remove_all(characters_,ch);language_registry_v109_.source_character_removed(ch);CanonicalObjectBorrowV1 actual;if(!receiver(ch,actual)||!s.ai_remove_from_group||!s.ai_remove_from_group(actual,e))return fail();}
  if(!resolve(a))return fail();if(a.identity){if(!a.type_f4){e="Required SAME type_f4 for source Remove";return fail();}if(*a.type_f4==5)remove_all(modules_,a.identity);}
  bool online{};if(!s.online_byte5||!s.online_byte5(online,e))return fail();
  if(online){
   if(!f.network100||!f.removed120){e="Required actual network100/removed120 lists";return fail();}
   // Original operator[] may insert a NULL node, without advancing next4c.
   auto i=f.network100->begin();
   for(;i!=f.network100->end();++i){auto& mapped=entries_[handle.key].actor;if(*i==mapped.identity)break;}
   if(i!=f.network100->end()){
    CanonicalObjectBorrowV1 network;if(!receiver(*i,network))return fail();std::uintptr_t nc{};
    if(!network.shared_handle){e="Required SAME network GetHandle";return fail();}network.shared_handle->frame=frame_;auto nh=*network.shared_handle;const CanonicalObjectBorrowV1* n{};
    if(!resolve_handle_v4(nh,false,n,{},e))return fail();if(n&&(!n->as_character||!n->as_character(n->context,nc,e)))return fail();
    bool virtual28=false;if(nc&&(!s.character_virtual28||!s.character_virtual28(network,virtual28,e)))return fail();
    if(!nc||!virtual28){std::uint16_t id{};if(!s.network_id108||!s.network_id108(network,id,e))return fail();f.removed120->push_back(id);}
    f.network100->erase(i);
   }
  }
  --count_; // Native wraps; NULL GameObject is only rejected at its reached read.
  if(!game){e="Original Remove reaches NULL GameObject2fc read after count50 decrement; unsupported safe native domain";return fail();}
  auto& mapped=entries_[handle.key].actor;
  if(*g.orphan2fc){if(mapped.identity){if(!f.orphan4||!s.orphan_admit||!s.orphan_admit(mapped,e))return fail();f.orphan4->push_back(mapped.identity);}}
  else if(mapped.identity){auto pin=mapped;if(!s.class_d0||!s.class_d0(pin,e))return fail();}
  if(language_registry_v109_.contains(handle.key)&&!language_registry_v109_.source_removed(handle.key,e))return fail();
  entries_.erase(handle.key);++frame_;
  if(!s.retire_after_unpublication||!s.retire_after_unpublication(*this,e))return fail();e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return fail();}catch(...){e="ObjectManager Remove native leaf threw; prefix retained";return fail();}
}
bool CanonicalObjectManagerV1::flush_source_v1(const CanonicalObjectLifecycleV1& s,std::string& e){
 ObjectManagerNativeStorageV1 f;if(!lifecycle_admit_v1(s,f,e))return false;
 lifecycle_busy_v1_=true;source_c1_fresh_v50_=false;source_flush_complete_v121_=false;struct Guard{bool& b;~Guard(){b=false;}}guard{lifecycle_busy_v1_};
 auto fail=[&]{return lifecycle_fail_v1(e);};
 auto receiver=[&](std::uintptr_t id,CanonicalObjectBorrowV1& out){if(!id){e="Original lifecycle reaches NULL receiver outside safe native domain";return false;}for(const auto& entry:entries_)if(entry.second.actor.identity==id&&entry.second.actor.lease){out=entry.second.actor;return true;}if(s.native_receiver&&s.native_receiver(id,out,e)&&out.identity==id&&out.lease)return true;if(e.empty())e="Required actual Character/orphan receiver";return false;};
 auto clear=[&](ObjectManagerContainerV1 id){if(id==ObjectManagerContainerV1::tree108){deferred108_v107_.clear();return true;}if(!f.clear){e="Required reached native manager container clear";return false;}return f.clear(id,e);};
 try{
  for(auto id:characters_){CanonicalObjectBorrowV1 a;if(!receiver(id,a)||!s.flush_target_list||!s.flush_target_list(a,e))return fail();if(!s.object_delete||!s.object_delete(a,e))return fail();}
  for(auto id:characters_){CanonicalObjectBorrowV1 a;if(!receiver(id,a)||!s.ai_update_pointers||!s.ai_update_pointers(a,e))return fail();}
  for(auto i=entries_.begin();i!=entries_.end();++i)if(i->second.actor.identity){auto pin=i->second.actor;if(!s.class_d0||!s.class_d0(pin,e))return fail();if(i->second.actor.identity!=pin.identity||i->second.actor.lease.owner_before(pin.lease)||pin.lease.owner_before(i->second.actor.lease)){e="SAME native map receiver changed during class D0";return fail();}if(language_registry_v109_.contains(i->first)&&!language_registry_v109_.source_removed(i->first,e))return fail();i->second.actor={};}
  if(!f.objects90){e="Required actual object90 list";return fail();}f.objects90->clear();entries_.clear();characters_.clear();modules_.clear();language_registry_v109_.source_flush();
  if(!f.network100||!f.removed120){e="Required native network100/removed120 lists";return fail();}f.network100->clear();f.removed120->clear();
  if(!clear(ObjectManagerContainerV1::tree108)||!clear(ObjectManagerContainerV1::tree164)||!clear(ObjectManagerContainerV1::tree17c)||!clear(ObjectManagerContainerV1::tree194)||!clear(ObjectManagerContainerV1::list128)||!clear(ObjectManagerContainerV1::list130)||!clear(ObjectManagerContainerV1::list80))return fail();
  if(!f.no_room88||!f.register_no_room88){e="Required SAME no-room88 and real AddRoomObjects registration";return fail();}f.no_room88->clear();if(!f.register_no_room88(e))return fail();
  if(!f.word138||!f.word13c||!f.word140){e="Required actual manager138/13c/140 cells";return fail();}*f.word138=0;*f.word13c=0;*f.word140=0;
  if(!clear(ObjectManagerContainerV1::tree148))return fail();
  if(!f.byte160){e="Required actual manager160 byte";return fail();}*f.byte160=0;entries_.try_emplace(0);next_key_=1;
  if(!f.word54||!f.word58){e="Required actual manager54/58 cells";return fail();}*f.word58=0;count_=0;*f.word54=0;
  if(!clear(ObjectManagerContainerV1::list3c)||!f.list2c){if(e.empty())e="Required actual manager2c storage";return fail();}f.list2c->clear();
  if(!clear(ObjectManagerContainerV1::list44))return fail();pending_.clear();if(!f.characters70){e="Required actual Character70 storage";return fail();}f.characters70->clear();if(!clear(ObjectManagerContainerV1::list24))return fail();init_phase7c_=0;
  // FlushAllOrphanRenderObjects3454dc: genuine D0, NULL cell, then list clear.
  if(!f.orphan4){e="Required actual orphan4 list/receiver journals";return fail();}
  for(auto& id:*f.orphan4)if(id){CanonicalObjectBorrowV1 a;if(!receiver(id,a)||!s.class_d0||!s.class_d0(a,e))return fail();id=0;}f.orphan4->clear();
  if(!clear(ObjectManagerContainerV1::tree98)||!clear(ObjectManagerContainerV1::treeb0)||!clear(ObjectManagerContainerV1::treec8)||!clear(ObjectManagerContainerV1::treee0))return fail();
  if(!s.retire_after_unpublication||!s.retire_after_unpublication(*this,e))return fail();
  source_flush_complete_v121_=true;++source_flush_generation_v121_;e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return fail();}catch(...){e="ObjectManager Flush native leaf threw; prefix retained";return fail();}
}
}

namespace dh2::world {
bool CanonicalObjectManagerV1::source_update_rooms_v104(
 const std::function<bool(std::uintptr_t,std::string&)>& update,std::string& e){
 visible_rooms_f8_v104_=0;
 for(auto i=rooms24_v104_.begin();i!=rooms24_v104_.end();++i){
  if(!update||!update(*i,e)){if(e.empty())e="Required actual RoomZone virtual2c";return false;}
 }
 e.clear();return true;
}
bool CanonicalObjectManagerV1::source_mark_for_deletion_v89(std::uintptr_t identity,std::string& e){
 for(const auto same:marked_for_deletion3c_v89_)if(same==identity){e.clear();return true;}
 try{marked_for_deletion3c_v89_.push_back(identity);}catch(const std::exception& failure){e=failure.what();return false;}
 e.clear();return true; //3432f8 neither modifies82/81 nor runs D0.
}


bool CanonicalObjectManagerV1::source_add_no_room_object_v89(const CanonicalObjectBorrowV1& actor,std::string& e){
 if(!actor.identity||!actor.lease||!actor.source_no_room2f8_v89){e="Required actual GameObject2f8 AddNoRoomObject fields";return false;}
 // Whole344184: original existing byte skip; source88 append allocation
 // precedes its native2f8=1 store. Do not reset this flag when list is cleared.
 if(*actor.source_no_room2f8_v89){e.clear();return true;}
 try{no_room88_v89_.push_back(actor.identity);}catch(const std::exception& failure){e=failure.what();return false;}
 *actor.source_no_room2f8_v89=1;e.clear();return true;
}
bool CanonicalObjectManagerV1::source_handle_no_room_objects_v89(
 const std::function<bool(const CanonicalObjectBorrowV1&,bool&,std::string&)>& is_game_object,std::string& e){
 // Whole345954: clear SAME88 list FIRST, without touching per-object2f8.
 no_room88_v89_.clear();
 for(auto entry=entries_.begin();entry!=entries_.end();){
  const auto key=entry->first;auto actor=entry->second.actor;
  if(actor.identity){
   if(!actor.lease||!is_game_object){e="Required SAME receiver virtual20 IsGameObject";return false;}
   bool game_object{};if(!is_game_object(actor,game_object,e))return false;
   if(game_object){
    if(!actor.source_room2f4_v89){e="Required actual GameObject2f4 field after virtual20";return false;}
    if(!*actor.source_room2f4_v89){
     bool present=false;for(const auto identity:no_room88_v89_)if(identity==actor.identity){present=true;break;}
     if(!present&&!source_add_no_room_object_v89(actor,e))return false;
    }
   }
  }
  // Source signed map successor. The pure source virtual20 receiver is lent
  // from retained transport; no class/type guessing or alternate registry.
  entry=entries_.upper_bound(key);
 }
 e.clear();return true;
}

bool CanonicalObjectManagerV1::source_remove_no_room_object_v108(const CanonicalObjectBorrowV1& actor,std::string& e){
 if(!actor.identity||!actor.lease){e="Required actual RemoveNoRoomObject receiver";return false;}
 for(auto at=no_room88_v89_.begin();at!=no_room88_v89_.end();++at)if(*at==actor.identity){
  if(!actor.source_no_room2f8_v89){e="Required SAME removed GameObject2f8 cell";return false;}
  no_room88_v89_.erase(at);*actor.source_no_room2f8_v89=0;break;
 }
 e.clear();return true;
}

}
