#include "character_kill_level_events_v23.hpp"
#include <cstring>
namespace dh2::character {
namespace {
struct QuestEnvelope {const KillQuest48* payload;};
bool type(void* raw,std::int32_t& out,std::string& e){auto* envelope=static_cast<QuestEnvelope*>(raw);if(!envelope||!envelope->payload){e="Required actual stack QE event";return false;}std::memcpy(&out,&envelope->payload->type,4);return true;} // whole IEvent31d8dc field4
}
const KillQuest48* kill_quest_payload_v23(const events::EventBorrowV12& event)noexcept{
 if(event.get_type!=type||!event.context)return nullptr;auto* envelope=static_cast<QuestEnvelope*>(event.context);
 return envelope->payload&&event.identity==reinterpret_cast<std::uintptr_t>(envelope->payload)?envelope->payload:nullptr;
}
bool CharacterKillLevelEventsV23::route(const KillRequest56& q,KillResponse16& out,bool& handled,std::string& e){
 handled=q.service==kill_current_level||q.service==kill_raise_async;if(!handled)return true;
 auto provider=provider_.lifetime.lock();if(!provider||!provider_.current||!depth_){e="Required scoped SAME GSLevel provider lifetime";return false;}
 if(q.service==kill_current_level){loader::CanonicalCurrentLevelBorrowV1 current;if(!provider_.current(current,e))return false;
  if(!current){out.pointer=0;return true;} // Actual empty GS slot, source unsafe-null branch remains in Kill.
  const auto* constructor=current.level()->constructor_owner_v3();if(!constructor||constructor->phase()!=loader::LevelConstructorPhaseV3::complete){e="Required whole SAME Level C1 before Kill publication";return false;}
  if(!current.kill_level()||current.kill_level()->identity!=current.identity()){e="Required SAME Level gate150 projection";return false;}
  out.pointer=reinterpret_cast<std::uintptr_t>(current.kill_level());levels_.push_back(std::move(current));return true;
 }
 if(!q.event||q.event->kind<1||q.event->kind>4||q.event->reserved||q.event->reserved2||q.event->flag0||q.event->flag1||q.event->network_id!=-1||q.subject!=q.event->level){e="Required exact source stack Kill QE payload";return false;}
 const auto captured=q.event->level;std::shared_ptr<loader::CanonicalLevelContextV1> level;
 for(auto i=levels_.rbegin();i!=levels_.rend();++i)if(i->identity()==captured){level=i->level();break;}
 if(!level){e="Required SAME captured Level through immediate QE delivery";return false;}
 auto* manager=level->constructor_fields_v3().events.get();if(!manager||manager->identity()!=level->identity()){e="Required actual Level embedded EventManager receiver";return false;}
 QuestEnvelope envelope{q.event};events::EventBorrowV12 event{reinterpret_cast<std::uintptr_t>(q.event),&envelope,type,level};
 return manager->raise_async(event,e); // literal339090 -> synchronous338ebc
}
}
