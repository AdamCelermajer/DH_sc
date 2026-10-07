#include "canonical_object_manager_v1.hpp"
#include <algorithm>
#include <exception>
namespace dh2::world {
bool CanonicalObjectManagerV1::fake_remove_source_v106(target_providers::Handle16 handle,
 const CanonicalObjectLifecycleV1& s,
 const std::function<bool(const CanonicalObjectBorrowV1&,std::uint8_t*&,std::string&)>& disabled,
 const std::function<bool(const CanonicalObjectBorrowV1&,std::string&)>& clean,std::string& e){
 ObjectManagerNativeStorageV1 f;if(!lifecycle_admit_v1(s,f,e))return false;
 lifecycle_busy_v1_=true;source_c1_fresh_v50_=false;
 struct Guard{bool& value;~Guard(){value=false;}} guard{lifecycle_busy_v1_};
 auto fail=[&]{return lifecycle_fail_v1(e);};
 auto resolve=[&](CanonicalObjectBorrowV1& out){out={};const CanonicalObjectBorrowV1* actual{};
  if(!resolve_handle_v4(handle,false,actual,{},e))return false;if(actual)out=*actual;return true;};
 auto character=[&](CanonicalObjectBorrowV1& out,std::uintptr_t& id){id=0;if(!resolve(out))return false;
  if(!out.identity)return true;if(!out.as_character){e="Required actual FakeRemove Character conversion";return false;}
  return out.as_character(out.context,id,e);};
 auto remove_all=[](auto& list,std::uintptr_t id){list.erase(std::remove(list.begin(),list.end(),id),list.end());};
 try{
  CanonicalObjectBorrowV1 actor;if(!resolve(actor))return fail();
  bool game{};ObjectManagerGameObjectFieldsV1 fields;
  if(!actor.identity||!s.game_object||!s.game_object(actor,game,fields,e)||!game||!fields.owner||
    fields.identity!=actor.identity||!fields.room_zone2f4||!fields.no_room2f8){if(e.empty())e="Source FakeRemove requires positive GameObject";return fail();}
  std::uint8_t* byte81{};if(!disabled||!disabled(actor,byte81,e)||!byte81){if(e.empty())e="Required SAME FakeRemove disabled81";return fail();}
  const auto room=*fields.room_zone2f4;*byte81=1;
  if(room&&(!s.room_remove||!s.room_remove(fields,room,e)))return fail();
  if(!f.no_room88||!f.objects90||!f.list2c||!f.characters70||!f.network100){e="Required actual FakeRemove lists88/90/2c/70/100";return fail();}
  auto no_room=std::find(f.no_room88->begin(),f.no_room88->end(),fields.identity);
  if(no_room!=f.no_room88->end()){f.no_room88->erase(no_room);*fields.no_room2f8=0;}
  auto first=std::find(f.objects90->begin(),f.objects90->end(),fields.identity);
  if(first!=f.objects90->end())f.objects90->erase(first);
  if(!resolve(actor))return fail();remove_all(*f.list2c,actor.identity);
  std::uintptr_t ch{};if(!character(actor,ch))return fail();remove_all(*f.characters70,ch);
  if(!character(actor,ch))return fail();
  if(ch){remove_all(characters_,ch);language_registry_v109_.source_character_removed(ch);
   if(!s.ai_remove_from_group||!s.ai_remove_from_group(actor,e))return fail();
   if(!clean||!clean(actor,e)){if(e.empty())e="Required whole Character.Clean3a66f8";return fail();}
  }
  if(!resolve(actor))return fail();
  if(actor.identity){if(!actor.type_f4){e="Required actual FakeRemove typef4";return fail();}if(*actor.type_f4==5)remove_all(modules_,actor.identity);}
  if(!resolve(actor))return fail();remove_all(*f.network100,actor.identity);
  // Exact3494d8 erases the old node;349448 then operator[] recreates the
  // NULL ObjectListItem. AddOrphanRenderObjectToDelete343188(NULL) is BXLR.
  // Neither count50 nor frame78 is changed; no classD0/retirement is run.
  if(language_registry_v109_.contains(handle.key)&&!language_registry_v109_.source_removed(handle.key,e))return fail();
  entries_.erase(handle.key);entries_.try_emplace(handle.key);
  e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return fail();}
 catch(...){e="ObjectManager FakeRemove source leaf threw; prefix retained";return fail();}
}
}
