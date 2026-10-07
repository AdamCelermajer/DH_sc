#include "character_kill_production_v23.hpp"
namespace dh2::character {
CharacterKillProductionV23::CharacterKillProductionV23(skills::CharacterWorldRuntimeV1& w,
 player::PlayerManagerOwnerV1& p,KillWorld16& globals,KillLevelProviderV23 level,
 CharacterKillProductionServicesV23 services):level_(std::move(level)),services_(std::move(services)),
 world_(w,p,globals,{services_.providers,[this](auto& a,const auto& q,auto& out,auto& e){return route(a,q,out,e);}}){}
bool CharacterKillProductionV23::add(CharacterKillLiveBorrowV21 fields,
 std::shared_ptr<CharacterKillContributorEventV23> contributor,std::string& e){
 if(!services_.providers||!services_.scope||!services_.refresh_contributor||!services_.died||
    !services_.remaining||!contributor||contributors_.count(fields.identity)){
  e="Required actual Kill production providers/contributor owner";return false;
 }
 const auto identity=fields.identity;
 if(!world_.add(std::move(fields),e))return false;
 contributors_.emplace(identity,std::move(contributor));return true;
}
bool CharacterKillProductionV23::route(KillActor56& actor,const KillRequest56& q,
 KillResponse16& out,std::string& e){
 bool handled{};if(!level_.route(q,out,handled,e))return false;if(handled)return true;
 if(q.service==kill_raise_event){
  const auto* scope=services_.scope();
  if(scope&&!dh2_script_callback_scope_valid(scope)){e="Expired actual Kill callback scope";return false;}
  if(q.argument==4){
   const auto found=contributors_.find(q.subject);
   if(found==contributors_.end()){e="Required same contributor CharAI owner";return false;}
   if(!services_.refresh_contributor(q.subject,e))return false;
   return found->second->raise(q.target,scope,e);
  }
  if(q.argument==2)return services_.died(q.subject,q.target,scope,e);
  e="Required source Kill event receiver "+std::to_string(q.argument);return false;
 }
 return services_.remaining(actor,q,out,e);
}
int CharacterKillProductionV23::command(std::uintptr_t controller,std::uintptr_t character,
 std::uintptr_t attacker,std::uint32_t force){
 level_.begin();struct End {CharacterKillLevelEventsV23& level;~End(){level.end();}} end{level_};
 return world_.command(controller,character,attacker,force);
}
}
