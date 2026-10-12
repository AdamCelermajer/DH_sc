#include "ai.hpp"
#include <algorithm>
#include <cstring>
#include <set>
#include <stdexcept>

namespace dh2::data {
namespace {
struct Reader {
 Bytes data;std::size_t at=0;
 explicit Reader(Bytes b):data(b){if(!b.data||b.size<4||b.size>1024*1024)throw std::runtime_error("AI data size outside limit");}
 std::uint32_t word(){if(at>data.size||data.size-at<4)throw std::runtime_error("Truncated AI word");const auto* p=data.data+at;at+=4;return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
 std::int32_t integer(){auto bits=word();std::int32_t value;std::memcpy(&value,&bits,4);return value;}
 float number(){auto bits=word();float value;std::memcpy(&value,&bits,4);return value;}
 std::uint32_t byte(){if(at>=data.size)throw std::runtime_error("Truncated AI byte");return data.data[at++];}
 std::string string(){auto n=word();if(n>4096||at>data.size||n>data.size-at)throw std::runtime_error("AI string outside limit");std::string s(reinterpret_cast<const char*>(data.data+at),n);at+=n;if(std::any_of(s.begin(),s.end(),[](unsigned char c){return c<32||c>126;}))throw std::runtime_error("AI identifier is not ASCII");return s;}
 std::vector<std::string> strings(){auto n=word();if(n>4096)throw std::runtime_error("AI name count outside limit");std::vector<std::string> out;std::set<std::string> seen;for(unsigned i=0;i<n;++i){auto s=string();if(s.empty()||!seen.insert(s).second)throw std::runtime_error("AI name empty or duplicated");out.push_back(std::move(s));}return out;}
 void end(){if(at!=data.size)throw std::runtime_error("Unexpected AI data suffix");}
};
}
bool load_ai(Bytes data,Bytes names,Bytes schema,Bytes factions,Bytes faction_names,Bytes faction_schema,AiTables& out,std::string& error){
 out={};error.clear();try{
  Reader r(data),n(names),s(schema),f(factions),fn(faction_names),fs(faction_schema);AiTables next;
  next.names=n.strings();n.end();next.faction_names=fn.strings();fn.end();
  const std::vector<std::string> fields={"AttackDelay","CombatBeat","CombatMusic","DelayedLoad","Flags","InteractRadius","LeashDistance","MeleeRadius","OnAggroSFX","Script","SelfFX","Trophy","Type","ViewRadius","ViewRadiusNoAggro"};
  if(s.strings()!=fields)throw std::runtime_error("AI schema differs");
  s.end();
  if(fs.strings()!=std::vector<std::string>{"Id","Value"}||fs.strings()!=std::vector<std::string>{"factions"})throw std::runtime_error("AI faction schema differs");
  fs.end();
  auto count=r.word();if(count!=next.names.size()||count<=8)throw std::runtime_error("AI table dimensions differ");next.rows.reserve(count);
  for(unsigned i=0;i<count;++i){AiProps p{};p.attack_delay=r.integer();p.combat_beat=r.integer();p.combat_music=r.integer();p.delayed_load=r.byte();p.flags=r.word();p.interact_radius=r.number();p.leash_distance=r.number();p.melee_radius=r.number();p.on_aggro_sfx=r.integer();p.script=r.string();p.self_fx=r.integer();p.trophy=r.integer();p.type=r.integer();p.view_radius=r.number();p.view_radius_no_aggro=r.number();next.rows.push_back(std::move(p));}r.end();
  count=f.word();if(count!=next.faction_names.size()||count<=10)throw std::runtime_error("Faction table dimensions differ");next.factions.resize(count);
  for(auto& row:next.factions){auto size=f.word();if(size>4096)throw std::runtime_error("Faction row outside limit");for(unsigned j=0;j<size;++j)row.push_back({f.integer(),f.integer()});}f.end();
  out=std::move(next);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
const AiProps* ai_props(const AiTables& t,std::int32_t id){if(id<0||std::size_t(id)>=t.rows.size())id=8;return std::size_t(id)<t.rows.size()?&t.rows[id]:nullptr;}
bool ai_enemy(const AiTables& t,std::int32_t owner,std::int32_t target,bool op,bool tp){if(owner<0||std::size_t(owner)>=t.factions.size())owner=10;if(target<0||std::size_t(target)>=t.factions.size())target=10;if(std::size_t(owner)>=t.factions.size())return false;const auto& row=t.factions[owner];return dh2_ai_enemy(row.data(),row.size(),target,op,tp)!=0;}
}
extern "C" unsigned dh2_ai_enemy(const dh2::data::AiFactionEntry* row,unsigned count,int target,unsigned op,unsigned tp){
 if(count>4096||(!row&&count)||op>1||tp>1||(op&&tp))return 0;
 for(unsigned i=0;i<count;++i)if(row[i].id==target)return row[i].value<0;
 return 0;
}
extern "C" unsigned dh2_ai_range(dh2::data::AiRangeResult* out,const dh2::data::AiRangeRequest* r){
 if(!out||!r)return 1;
 // Explicit rounding prevents ARM64 multiply-add contraction from changing
 // the ARM32 soft-float order. Negative/NaN radii retain original semantics.
 volatile float dx=r->owner[0]-r->target[0],dy=r->owner[1]-r->target[1],dz=r->owner[2]-r->target[2];
 volatile float xx=dx*dx,yy=dy*dy,zz=dz*dz,xy=xx+yy,distance=xy+zz;
 volatile float reach=r->owner_melee+r->target_melee,reach_sq=reach*reach,view_sq=r->view*r->view;
 dh2::data::AiRangeResult result{};float d=distance;std::memcpy(&result.distance_bits,&d,4);result.melee=reach_sq>distance;result.sight=view_sq>distance;*out=result;return 0;
}
extern "C" unsigned dh2_ai_target_update(dh2::data::AiTargetResult* out,const dh2::data::AiTargetRequest* r){
 using namespace dh2::data;if(!out||!r||r->facts&~1023u||r->previous_alive>1||r->previous_sight>1)return 1;
 AiTargetResult next{};next.target_present=next.last_target_present=bool(r->facts&ai_target_present);next.alive=r->previous_alive;next.sight=r->previous_sight;
 auto event=[&](unsigned id,bool null=false){if(null)next.null_arguments|=1u<<next.event_count;next.events[next.event_count++]=id;};
 if(r->owner_state==0||r->owner_state==17||!next.target_present){*out=next;return 0;}
 if(!(r->facts&ai_targetable)){next.target_present=next.last_target_present=0;event(12,true);*out=next;return 0;}
 const unsigned alive=bool(r->facts&ai_target_alive);
 if(next.alive!=alive){event(alive?11:10);if(!alive&&(r->facts&ai_callback_clears_dead))next.target_present=next.last_target_present=0;}next.alive=alive;
 if(next.target_present){const unsigned sight=bool(r->facts&ai_target_sight);if(next.sight!=sight){event(sight?13:12);if(!sight&&(r->facts&ai_callback_clears_sight))next.target_present=next.last_target_present=0;}next.sight=sight;
  if(next.target_present&&sight)event(r->facts&ai_owner_can_range?(r->facts&ai_target_close_range?16:r->facts&ai_target_ranged_range?15:14):(r->facts&ai_target_melee_range?17:14));
 }*out=next;return 0;
}
