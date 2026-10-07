#include "character_kill_live_v21.hpp"
#include <cstring>
#include <limits>
#include <cmath>
namespace dh2::character {
bool source_kill_aggro_entry_v21(const data::AggroTable& table,std::int32_t index,
 std::uintptr_t& character,std::uint32_t& threat,std::string& e){
 if(table.count>table.capacity||(table.count&&!table.entries)){e="Required actual source outgoing Aggro map";return false;}
 std::map<float,std::pair<std::uintptr_t,std::uint32_t>,std::less<float>> converse;
 for(unsigned i=0;i<table.count;++i){const auto& entry=table.entries[i];
  if(entry.reserved||(i&&table.entries[i-1].character>=entry.character)){e="Required source key-ordered Aggro entries";return false;}
  float value;std::memcpy(&value,&entry.threat_bits,4);
  if(std::isnan(value)){e="Required original unordered NaN converse-map tree domain";return false;}
  converse.emplace(value,std::make_pair(std::uintptr_t(entry.character),entry.threat_bits));
 }
 character=0; // Original missing entry leaves its float output untouched.
 if(index<0)return true;std::uint32_t cursor=0;
 for(const auto& entry:converse){if(cursor++==std::uint32_t(index)){character=entry.second.first;threat=entry.second.second;break;}}
 return true;
}
bool CharacterKillLiveWorldV21::add(CharacterKillLiveBorrowV21 b,std::string& e){
 e.clear();skills::WorldTargetActorBorrowV1 target{};
 if(!b.identity||actors_.count(b.identity)||!b.receiver||!b.properties||!b.life||b.life->dead>1||
    !b.fields||!b.fields->produced||!b.oid64||!b.property13c8||!b.tracked14a4||!b.outgoing||
    !b.controller||b.controllable_character!=b.identity||!b.shared_handle||b.shared_handle->cached!=b.identity||
    world_.actor(b.identity,&target)||!target.character||target.character->resolved!=b.properties->resolved||target.life!=b.life){
  e="Required SAME produced Character Kill/controller/metadata/property/World graph";return false;
 }
 auto entry=std::make_unique<Entry>();entry->borrow=std::move(b);auto& f=entry->borrow;
 entry->projection={f.identity,f.properties,f.fields->killer144c,nullptr,*f.tracked14a4,*f.oid64,
   *f.property13c8,f.fields->template13ca,std::uint8_t(f.life->dead),f.fields->suppress_quest14e4,{}};
 entry->published_killer=entry->projection.killer;entry->published_tracked=entry->projection.tracked_target;
 entry->owner=std::make_unique<CombatCtrlKillOwnerV1>(entry->projection,*f.life,globals_,KillServices16{this,service});
 actors_.emplace(f.identity,std::move(entry));return true;
}
bool CharacterKillLiveWorldV21::synchronize(){
 for(auto& pair:actors_){auto& entry=*pair.second;auto& b=entry.borrow;auto& p=entry.projection;
  if(!b.fields->produced||b.life->dead>1){error_="Required valid live Kill scalar authorities";return false;}
  if(p.killer!=entry.published_killer)b.fields->killer144c=p.killer;else p.killer=b.fields->killer144c;
  if(p.tracked_target!=entry.published_tracked)*b.tracked14a4=p.tracked_target;else p.tracked_target=*b.tracked14a4;
  entry.published_killer=p.killer;entry.published_tracked=p.tracked_target;
  p.oid=*b.oid64;p.property_id=*b.property13c8;p.template_id=b.fields->template13ca;p.suppress_quest=b.fields->suppress_quest14e4;
  const auto master=b.fields->master14d4;auto found=actors_.find(master);
  if(master&&found==actors_.end()){error_="Required actual registered master14d4 Character";return false;}
  p.owner=master?&found->second->projection:nullptr;
 }
 return true;
}
int CharacterKillLiveWorldV21::query(std::uint32_t op,std::uintptr_t id,std::int32_t& out){
 auto services=world_.targets().query_services();std::uintptr_t value{};
 const target_providers::Request24 request{op,0,id,0};
 if(!services.invoke||services.invoke(services.context,&request,&value)){error_="Required source live Kill virtual query";return -1;}
 out=std::int32_t(value);return 0;
}
int CharacterKillLiveWorldV21::service(void* raw,KillActor56* actor,const KillRequest56* q,KillResponse16* out){
 auto& self=*static_cast<CharacterKillLiveWorldV21*>(raw);
 if(!actor||!q||!out||!self.synchronize())return -1;
 const auto status=self.invoke(*actor,*q,*out);
 if(!self.synchronize())return -1;return status;
}
int CharacterKillLiveWorldV21::invoke(KillActor56& actor,const KillRequest56& q,KillResponse16& out){
 auto found=actors_.find(actor.identity);if(found==actors_.end()||&found->second->projection!=&actor){error_="Required exact registered Kill projection";return -1;}
 switch(q.service){
 case kill_is_dead:{auto entry=actors_.find(q.subject);if(entry==actors_.end()){error_="Required actual Kill IsDead Character";return -1;}out.word=std::int32_t(entry->second->borrow.life->dead);return 0;}
 case kill_is_player:return query(target_providers::virtual_player,q.subject,out.word);
 case kill_is_character:return query(target_providers::virtual_character,q.subject,out.word);
 case kill_is_local_player:{player::PlayerInfoFieldsV1* record{};
  if(!players_.get_by_character(q.subject,false,record,error_)||!record)return -1;
  // CNet IsLocal has a genuine separate network identity owner; do not use
  // local66c as its virtual return. Route it through actual source backend.
  break;
 }
 case kill_get_local_player:{player::PlayerInfoFieldsV1* record{};
  if(!players_.get_local_player(q.index,q.argument!=0,record,error_)||!record)return -1;
  out.pointer=record->character660;return 0;
 }
 case kill_aggro_count:{const auto& table=*found->second->borrow.outgoing;
  if(table.count>table.capacity||(table.count&&!table.entries)||table.count>std::uint32_t(std::numeric_limits<std::int32_t>::max())){error_="Required source AggroCount8c";return -1;}
  out.word=std::int32_t(table.count);return 0;
 }
 case kill_aggro_entry:{std::uintptr_t id{};std::uint32_t unused=0;
  if(!source_kill_aggro_entry_v21(*found->second->borrow.outgoing,q.index,id,unused,error_))return -1;
  auto entry=actors_.find(id);if(id&&entry==actors_.end()){error_="Required same registered Kill contributor Character";return -1;}
  out.pointer=id?reinterpret_cast<std::uintptr_t>(&entry->second->projection):0;return 0;
 }
 case kill_handle_character:{target_providers::Handle16 local{},*shared{};target_providers::Registry24* registry{};
  if(world_.get_handle(q.subject,&local)||world_.handle_borrow(q.subject,&shared,&registry)){error_="Required actual canonical killer Handle";return -1;}
  auto services=world_.targets().query_services();if(target_providers::dh2_target_handle_character(&out.pointer,&local,shared,registry,&services)){error_="Required source killer Handle->Character cast";return -1;}return 0;
 }
 default:break;
 }
 if(!backends_.provider_lease||!backends_.invoke||!backends_.invoke(actor,q,out,error_)){
  if(error_.empty())error_="Required whole live Kill service "+std::to_string(q.service);return -1;
 }return 0;
}
int CharacterKillLiveWorldV21::command(std::uintptr_t controller,std::uintptr_t identity,std::uintptr_t attacker,std::uint32_t force){
 error_.clear();auto found=actors_.find(identity);
 if(found==actors_.end()||found->second->borrow.controller!=controller||found->second->borrow.controllable_character!=identity){error_="Required source Cmd_Kill controllable+4/virtual58 receiver";return -1;}
 auto& entry=*found->second;
 if(entry.failed){error_="Failed live Kill prefix cannot replay";return -2;}
 if(!synchronize())return -1;
 entry.projection.dead=std::uint8_t(entry.borrow.life->dead);
 const bool nested=entry.depth!=0;++entry.depth;struct Depth{std::uint32_t& value;~Depth(){--value;}}depth{entry.depth};
 entry.attempted=true;std::string coordinator;KillResult24 nested_result{};
 const int status=entry.owner->ctrl_kill(nested?nested_result:entry.result,attacker,force,coordinator);
 const auto backend=error_;if(!synchronize()){if(!nested){entry.failed=true;entry.complete=false;}return -2;}
 if(status!=1){if(!nested){entry.failed=true;entry.complete=false;}error_=coordinator;if(!backend.empty())error_+="; "+backend;return status;}
 if(!nested)entry.complete=true;return 1;
}
const KillResult24* CharacterKillLiveWorldV21::result(std::uintptr_t id)const noexcept{const auto found=actors_.find(id);return found==actors_.end()?nullptr:&found->second->result;}
bool CharacterKillLiveWorldV21::complete(std::uintptr_t id)const noexcept{const auto found=actors_.find(id);return found!=actors_.end()&&found->second->complete;}
}
