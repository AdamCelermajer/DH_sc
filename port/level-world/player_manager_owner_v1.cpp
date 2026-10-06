#include "player_manager_owner_v1.hpp"
#include <climits>
namespace dh2::player {
bool PlayerManagerOwnerV1::call(PlayerManagerRequestV1 q,PlayerManagerResponseV1& out,std::string& e){
 if(!services_.invoke){e="Required actual PlayerManager source receiver "+std::to_string(std::uint32_t(q.operation));return false;}return services_.invoke(services_.context,q,out,e);
}
bool PlayerManagerOwnerV1::network(bool& enabled,std::string& e){
 enabled=false;PlayerManagerResponseV1 r;
 for(auto op:{PlayerManagerOperationV1::online_enabled,PlayerManagerOperationV1::network_enabled,PlayerManagerOperationV1::session_ready,PlayerManagerOperationV1::session_active}){
  if(!call({op},r,e))return false;if(!r.value)return true;
 }enabled=true;return true;
}
bool PlayerManagerOwnerV1::initialize(std::string& e){
 e.clear();if(running_||initialized_){e="Unsupported PlayerManager constructor reentry";return false;}
 running_=true;struct G{bool& b;~G(){b=false;}}g{running_};PlayerManagerResponseV1 r;
 if(!call({PlayerManagerOperationV1::construct_player_info,0,0,0,false,&dummy_},r,e))return false;
 initialized_=true;return true;
}
bool PlayerManagerOwnerV1::numbers(std::string& e){
 bool net;if(!network(net,e))return false;
 if(net){e="Required whole online UpdatePlayerNumbers vector and network-property producer";return false;}
 std::int32_t friendly=0,local=0,remote=0;
 for(auto& row:players_){auto& p=*row.second;p.friendly678=friendly++;p.local_remote67c=p.local66c?local++:remote++;}return true;
}
bool PlayerManagerOwnerV1::add_player(std::int32_t id,std::int32_t controller,std::int32_t index,bool local,std::string& e){
 e.clear();if(!initialized_||running_){e="Required initialized non-reentrant PlayerManager";return false;}
 running_=true;struct G{bool& b;~G(){b=false;}}g{running_};bool net;if(!network(net,e))return false;
 PlayerManagerResponseV1 r;
 if(net)return call({PlayerManagerOperationV1::network_add_player,id,controller,index,local},r,e);
 if(players_.find(id)!=players_.end())return true;
 auto value=std::make_unique<PlayerInfoFieldsV1>();if(!call({PlayerManagerOperationV1::construct_player_info,id,0,0,false,value.get()},r,e))return false;
 value->local66c=std::uint8_t(local);value->controller668=index;value->internal670=id;value->controller_local674=controller;
 players_.emplace(id,std::move(value));return numbers(e);
}
bool PlayerManagerOwnerV1::add_character(std::int32_t id,std::string& e){
 e.clear();if(!initialized_||running_){e="Required initialized non-reentrant PlayerManager";return false;}
 running_=true;struct G{bool& b;~G(){b=false;}}g{running_};PlayerInfoFieldsV1* p;if(!get_by_internal(id,false,p,e))return false;
 // Source slot is an embedded network property; whole original AddCharacter
 // owns its read/gates/Spawn/Save/AI/skill/enable/controller/light order.
 PlayerManagerResponseV1 r;
 return call({PlayerManagerOperationV1::character_initialization,id,0,0,false,p,&character_count6c4_},r,e);
}
bool PlayerManagerOwnerV1::remove_player(std::int32_t id,std::string& e){
 e.clear();if(!initialized_||running_){e="Required initialized non-reentrant PlayerManager";return false;}
 running_=true;struct G{bool& b;~G(){b=false;}}g{running_};bool net;if(!network(net,e))return false;PlayerManagerResponseV1 r;
 if(net)return call({PlayerManagerOperationV1::network_remove_player,id},r,e);
 auto it=players_.find(id);if(it==players_.end())return true;
 if(it->second->character660){if(!call({PlayerManagerOperationV1::remove_character,id,0,0,false,it->second.get(),&character_count6c4_},r,e))return false;if(it->second->character660){e="RemoveCharacter did not clear same PlayerInfo character660";return false;}}
 players_.erase(it);return numbers(e);
}
bool PlayerManagerOwnerV1::get_by_internal(std::int32_t id,bool flag,PlayerInfoFieldsV1*& out,std::string& e){
 e.clear();if(!initialized_){e="Required source PlayerManager constructor";return false;}
 if(id==-1){out=&dummy_;return true;}bool net;if(!network(net,e))return false;
 if(net){PlayerManagerResponseV1 r;if(!call({PlayerManagerOperationV1::network_get_info,id,0,0,flag},r,e))return false;if(!r.player){e="Required actual network PlayerInfo receiver";return false;}out=r.player;return true;}
 auto found=players_.find(id);out=found==players_.end()?&dummy_:found->second.get();return true;
}
bool PlayerManagerOwnerV1::mapped(std::int32_t index,bool flag,unsigned kind,PlayerInfoFieldsV1*& out,std::string& e){
 e.clear();if(!initialized_){e="Required source PlayerManager constructor";return false;}bool net;if(!network(net,e))return false;
 if(net){e="Required original online friendly/local/remote vector producer";return false;}
 std::int32_t id=-1;std::uint32_t remaining=std::uint32_t(index);
 if(remaining<players_.size())for(auto& row:players_){auto& p=*row.second;
  if(kind==1&&!p.local66c)continue;if(kind==2&&p.local66c)continue;
  // Offline source checks Character660 before adding node18: +678 in node.
  if(flag&&!p.character660)continue;if(!remaining){id=p.internal670;break;}--remaining;
 }return get_by_internal(id,flag,out,e);
}
bool PlayerManagerOwnerV1::get_player(std::int32_t n,bool f,PlayerInfoFieldsV1*& p,std::string& e){return mapped(n,f,0,p,e);}
bool PlayerManagerOwnerV1::get_local_player(std::int32_t n,bool f,PlayerInfoFieldsV1*& p,std::string& e){return mapped(n,f,1,p,e);}
bool PlayerManagerOwnerV1::get_remote_player(std::int32_t n,bool f,PlayerInfoFieldsV1*& p,std::string& e){return mapped(n,f,2,p,e);}
bool PlayerManagerOwnerV1::get_by_character(std::uintptr_t character,bool f,PlayerInfoFieldsV1*& p,std::string& e){
 bool net;if(!network(net,e))return false;if(net){e="Required original online GetPlayerByCharacter vector producer";return false;}
 for(auto& row:players_)if(row.second->character660==character){p=row.second.get();return true;}return get_by_internal(-1,f,p,e);
}
bool PlayerManagerOwnerV1::num_players(std::int32_t& out,std::string& e){bool net;if(!network(net,e))return false;if(net){e="Required original online friendly vector producer";return false;}out=std::int32_t(players_.size());return true;}
bool PlayerManagerOwnerV1::num_local_players(bool flag,std::int32_t& out,std::string& e){bool net;if(!network(net,e))return false;if(net){e="Required original online local vector producer";return false;}out=0;for(auto& row:players_)if(row.second->local66c&&(!flag||row.second->character660))++out;return true;}
bool PlayerManagerOwnerV1::loot_player(void* p,std::int32_t index,bool f,LootPlayerBorrowV8& out,std::string& e){PlayerInfoFieldsV1* record;if(!static_cast<PlayerManagerOwnerV1*>(p)->get_player(index,f,record,e))return false;out={reinterpret_cast<std::uintptr_t>(record),&record->character660,record->character_base_id13c8};return true;}
LootPlayerManagerServicesV8 PlayerManagerOwnerV1::loot_services()noexcept{return {this,loot_player};}
bool PlayerManagerOwnerV1::class_count(std::int32_t base,std::int32_t& out,std::string& e){return player_class_count_v8(&character_count6c4_,base,loot_services(),out,e);}
}
