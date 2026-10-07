#include "canonical_object_update_v102.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::world {
bool borrow_object_update_storage_v102(const std::shared_ptr<CanonicalObjectManagerV1>& manager,ObjectUpdateStorageV102& out,std::string& e){
 if(!manager){e="Required SAME retained ObjectManager C1";return false;}
 out={manager,&manager->source_active2c_v102(),&manager->source_pending34_v102(),&manager->source_deletion3c_v102(),&manager->source_conditions44_v102(),&manager->source_characters70_v102(),&manager->source_updates58_v102(),&manager->source_updates5c_v102(),&manager->source_onlinefc_v102(),&manager->source_onlinefd_v102()};return true;
}
bool process_next_start_v102(CanonicalObjectManagerV1& manager,const std::function<bool(std::uintptr_t,std::string&)>& entered,std::string& e){
 auto& list=manager.source_next_start90_v102();if(list.empty())return true;
 const auto identity=list.front();if(identity&&(!entered||!entered(identity,e))){if(e.empty())e="Required source GameObject.ZoneEntered";return false;}
 //Source reloads the front after ZoneEntered, preserving callback mutations.
 if(list.empty()){e="Source ZoneEntered removed the retained start90 front";return false;}list.pop_front();return true;
}
bool update_remote_objects_v102(CanonicalObjectManagerV1& manager,float dt,const ObjectUpdateServicesV102& s,std::string& e){
 for(auto i=manager.source_network100_v102().begin();i!=manager.source_network100_v102().end();++i){ObjectUpdateActorV102 actor;bool remote{};
  if(!s.actor||!s.actor(*i,actor,e)||actor.object.identity!=*i||!actor.object.lease||!s.remotely_updated||!s.remotely_updated(actor,remote,e))return false;
  if(!remote)continue;auto word=actor.word?actor.word(0x114):nullptr;auto network=actor.integer?actor.integer(0x110):nullptr;
  if(!word||!network){e="Required source remote114/110 fields";return false;}float elapsed;std::memcpy(&elapsed,word,4);
  //3402c0 resolves __aeabi_fcmpge at GOT994d08, threshold42480000.
  if(elapsed>=50.f){*word=0;*network=-1;}else{volatile float next=dt+elapsed;const float stored=next;std::memcpy(word,&stored,4);}
 }return true;
}
bool canonical_object_update_v102(CanonicalObjectManagerV1& manager,float dt,const ObjectUpdateServicesV102& s,std::string& e){
 auto required=[&](const char* text){if(e.empty())e=std::string("Required source ObjectManager.Update ")+text;return false;};
 if(!s.owner)return required("provider lifetime");
 ObjectUpdateStorageV102 f;if(!s.storage||!s.storage(f,e)||!f.owner||!f.active2c||!f.pending34||!f.deletion3c||!f.conditions44||!f.characters70||!f.updates58||!f.updates5c||!f.online_fc||!f.online_fd)return required("same C1 containers/cells");
 bool online{};if(!s.online||!s.online(online,e))return required("GetOnline.byte5");if(online)*f.online_fc=*f.online_fd=1;
 std::shared_ptr<void> level;const std::uint8_t* word144{};const std::uint8_t* word198{};
 if(!s.level||!s.level(level,word144,word198,e))return required("GetCurrentLevel");
 if(level){if(!word144)return required("Level144");if(*word144){if(!word198)return required("Level198");if(!*word198){if(!s.online||!s.online(online,e))return required("GetOnline early gate");if(!online)return true;}}}
 if(!s.rooms||!s.rooms(e))return required("UpdateRooms");
 if(!update_remote_objects_v102(manager,dt,s,e))return required("DoRemoteUpdateUpdate");
 if(!process_next_start_v102(manager,s.zone_entered,e))return required("ProcessNextGameObjectToStartUpdate");
 f.active2c->splice(f.active2c->end(),*f.pending34);
 auto borrow=[&](std::uintptr_t id,ObjectUpdateActorV102& a){return s.actor&&s.actor(id,a,e)&&a.object.identity==id&&a.object.lease&&a.byte&&a.pointer;};
 auto byte=[&](ObjectUpdateActorV102& a,unsigned offset)->std::uint8_t*{return a.byte(offset);};
 for(auto i=f.deletion3c->begin();i!=f.deletion3c->end();){ObjectUpdateActorV102 a;if(!borrow(*i,a))return required("deletion receiver");auto flag29=byte(a,0x29),delay82=byte(a,0x82);if(!flag29||!delay82)return required("deletion29/82");
  if(!*flag29&&!*delay82){bool deferred{};if(!s.deferred||!s.deferred(a,deferred,e))return required("IsOnlineDeferred");if(!deferred){bool character{};if(!s.is_character||!s.is_character(a,character,e))return required("virtual24 deletion");bool fake=false;
    if(character){if(!s.character_virtual28||!s.character_virtual28(a,fake,e))return required("Character.virtual28");}
    if(!s.remove||!s.remove(a,fake,e))return required(fake?"FakeRemove":"Remove");i=f.deletion3c->erase(i);continue;
   }}
  // Deferred callback may update82. Reload it at the original branch.
  delay82=byte(a,0x82);if(!delay82)return required("reloaded deletion82");if(*delay82)--*delay82;++i;
 }
 for(auto i=f.conditions44->begin();i!=f.conditions44->end();++i){ObjectUpdateActorV102 a;if(!borrow(*i,a))return required("condition receiver");auto ac=byte(a,0xac);auto pa8=a.pointer(0xa8);if(!ac||!pa8)return required("enable condition fields");bool test=!*ac&&*pa8;
  if(!test){auto d0=byte(a,0xd0);auto pcc=a.pointer(0xcc);if(!d0||!pcc)return required("disable condition fields");test=!*d0&&*pcc;}
  if(test){if(!s.test_enable||!s.test_enable(a,true,e))return required("TestEnableCondition");if(!s.test_disable||!s.test_disable(a,true,e))return required("TestDisableCondition");}
 }
 *f.updates58=*f.updates5c=0;
 for(auto i=f.active2c->begin();i!=f.active2c->end();){const auto id=*i;if(!id){++i;continue;}ObjectUpdateActorV102 a;if(!borrow(id,a))return required("active receiver");bool character{};
  if(!s.is_character||!s.is_character(a,character,e))return required("virtual24 active");if(character&&(!s.update_ai||!s.update_ai(a,e)))return required("UpdateAIPointers");
  auto enabled85=byte(a,0x85),always8a=byte(a,0x8a);if(!enabled85||!always8a)return required("source85/8a");bool update=*enabled85&&*always8a;
  if(!update){if(!s.online||!s.online(online,e))return required("GetOnline actor gate");if(online){if(!s.remotely_updated||!s.remotely_updated(a,update,e))return required("virtual54");}
   if(!update){auto byte86=byte(a,0x86);if(!byte86)return required("source86");*byte86=0;if(!s.is_character||!s.is_character(a,character,e))return required("virtual24 unload");if(character&&(!s.unload_script||!s.unload_script(a,e)))return required("UnLoadScriptProcess");++i;continue;}}
  auto disabled81=byte(a,0x81);if(!disabled81)return required("source81");if(*disabled81){if(!manager.source_mark_for_deletion_v89(id,e))return false;i=f.active2c->erase(i);continue;}
  auto current88=byte(a,0x88);if(!current88)return required("source88");*current88=0;
  if(!s.update||!s.update(a,e))return required("virtual2c actor frame");std::uintptr_t exists{};
  if(!s.resolve_handle_false||!s.resolve_handle_false(a,exists,e))return required("post-update GetHandle/GetObject(false)");
  if(exists){current88=byte(a,0x88);auto old89=byte(a,0x89);if(!current88||!old89)return required("post-update88/89");if(*current88!=*old89){*old89=*current88;std::uintptr_t converted{};
    if(!s.resolve_character||!s.resolve_character(a,converted,e))return required("post-update Character handle");
    if(*current88)f.characters70->push_back(converted);else f.characters70->remove(converted);
   }}++i;
 }
 return s.reset_debug_switches&&s.reset_debug_switches(e)?true:required("Debug.SetSwitch tail");
}
}
