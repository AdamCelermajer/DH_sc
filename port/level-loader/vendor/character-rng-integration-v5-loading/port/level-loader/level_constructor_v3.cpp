#include "level_constructor_v3.hpp"
#include <algorithm>
#include <cctype>
#include <cstring>
namespace dh2::loader {
bool LevelConstructorV3::fail(const char* reason){
 if(error_.empty())error_=reason;
 failed_at_=phase_;phase_=LevelConstructorPhaseV3::failed;return false;
}
bool LevelConstructorV3::construct(LevelConstructorArgumentsV3 a){
 if(busy_||attempted_){error_="Level constructor already attempted or reentered";return false;}
 attempted_=true;busy_=true;
 // This once-only constructor capsule needs service leases only during the
 // call. Completed/failed source fields own their actual receivers separately;
 // retaining Application.currentLevel here would introduce a Level cycle.
 struct Guard{
  bool& busy;LevelConstructorBorrowV3& borrow;LevelConstructorServicesV3& services;
  ~Guard(){busy=false;borrow.owner.reset();services={};}
 } guard{busy_,borrow_,services_};error_.clear();
 auto& b=borrow_;auto& s=services_;
 if(!b.owner||!b.identity||!b.fields||!a.name||!b.config38||!b.music11c||!b.safezone120||!b.ambient124||!b.gate150||!b.module_offset160||!b.module_id18c)
  return fail("Required SAME Level constructor fields and selected CString");
 auto& f=*b.fields;
 phase_=LevelConstructorPhaseV3::event_manager;
 if(f.events)return fail("Level EventManager base already constructed");
 f.events=std::make_unique<events::EventManagerOwnerV12>(b.identity);
 f.phase30=0;*b.config38=0;f.row3c=-1;f.difficulty40=-1;
 phase_=LevelConstructorPhaseV3::script_constructor;
 if(!s.construct_script||!s.construct_script(b.identity,false,f.script44,error_))return fail("Required actual embedded LuaScript C1/BindFunction(false)");
 if(!f.script44)return fail("LuaScript constructor did not retain its actual receiver");
 f.party_dc=a.party;f.word_e0=a.word_e0;f.byte_f1=a.byte_f1;f.byte_f2=a.byte_f2;f.phase_e4=1;
 f.save_ec.reset();f.byte_f0=0;f.byte_f3=0;f.byte_f4=0;f.byte_f5=0;
 f.name_f8=a.name;f.level110=a.level;f.seed114=a.seed;
 f.field130=0;f.field1a4=0;f.field134=0;b.module_offset160[0]=0;f.field138=0;
 b.module_offset160[1]=0;b.module_offset160[2]=0;f.field19c=0;f.field1a0=0;f.mode118=a.mode;
 *b.music11c=-1;*b.safezone120=-1;*b.ambient124=-1;
 f.field128=0;f.field12c=0;f.field13c=0;f.field140=0;f.byte144=0;f.byte145=0;f.field148=-1;
 f.field14c=0;*b.gate150=0;f.field154=0;f.field158=0;f.field15c=0;f.field194=0;f.byte198=0;f.byte1a8=0;
 phase_=LevelConstructorPhaseV3::script_path;
 if(!s.application||!s.debug_level_load_count)return fail("Required application debug Level-load counter");
 ++*s.debug_level_load_count;
 if(!s.script_assign_path||!s.script_assign_path(f.script44,"data/scripts/",13,error_))return fail("Required SAME LuaScript path assignment");
 phase_=LevelConstructorPhaseV3::first_script;
 if(!s.script_load||!s.script_load(f.script44,"level/combat_formulas",error_))return fail("Required real Level combat_formulas LuaScript::Load");
 phase_=LevelConstructorPhaseV3::second_script;
 if(!s.script_load(f.script44,"level/death_scripts",error_))return fail("Required real Level death_scripts LuaScript::Load");
 phase_=LevelConstructorPhaseV3::module_reset;
 *b.module_id18c=-1;f.byte16c=0;
 if(!s.module_id_global)return fail("Required SAME Module::s_moduleId global");
 *s.module_id_global=0;f.byte_e8=0;
 phase_=LevelConstructorPhaseV3::level_lookup;
 if(!s.levels)return fail("Required actual application Arrays::LevelList");
 for(std::size_t i=0;i<s.levels->levels.size();++i){
  const auto& row=s.levels->levels[i];
  // Original strcpy to1024 then ToLowerCase(-1). Preserve its native safe
  // domain explicitly; do not overflow, truncate or alias a different row.
  if(row.file.size()>1023||row.file.find('\0')!=std::string::npos)return fail("Level filename outside original CString1024 domain");
  std::string lower=row.file;
  std::transform(lower.begin(),lower.end(),lower.begin(),[](unsigned char ch){return char(std::tolower(ch));});
  if(f.name_f8.find(lower)==std::string::npos)continue;
  f.byte_e8=std::uint8_t(row.scalar.words[5]);f.row3c=std::int32_t(i);
  std::memcpy(&f.difficulty40,&row.scalar.words[4],4);break;
 }
 phase_=LevelConstructorPhaseV3::online;
 std::uint8_t online{};
 if(!s.online_byte5||!s.online_byte5(online,error_))return fail("Required actual GetOnline byte5");
 if(online){
  bool hosting{};
  if(!s.local_player_hosting||!s.local_player_hosting(hosting,error_))return fail("Required actual PlayerManager::IsLocalPlayerHosting");
  bool read719=hosting;
  if(!hosting){
   std::int32_t mode{};
   if(!s.online_state34||!s.online_state34(mode,error_))return fail("Required actual OnlineGameState34");
   if(std::uint32_t(mode)-3u<=1u){
    bool matching{};
    if(!s.matching_is_host||!s.matching_is_host(matching,error_))return fail("Required actual CMatchingGLLive::IsHost");
    read719=matching;
   }
  }
  if(read719){
   std::uint8_t value{};
   if(!s.player_manager_byte719||!s.player_manager_byte719(value,error_))return fail("Required actual PlayerManager byte719");
   if(value)f.byte_f1=0;
  }else f.byte_f1=0;
 }
 if(f.row3c!=-1){
  if(a.requested_difficulty!=-1){
   f.byte_f5=1;
   if((std::uint32_t(a.requested_difficulty)|std::uint32_t(f.difficulty40))==0||
      (a.requested_difficulty!=f.difficulty40&&f.difficulty40!=0))f.byte_f3=1;
  }
  phase_=LevelConstructorPhaseV3::save_allocation;
  std::shared_ptr<void> allocation;
  if(!s.allocate_save||!s.allocate_save(allocation,error_))return fail("Required actual LevelSavegame allocation");
  if(!allocation)return fail("LevelSavegame allocation did not retain its actual storage");
  phase_=LevelConstructorPhaseV3::save_constructor;
  const LevelSaveConstructionV3 request{b.identity,f.seed114,f.difficulty40,f.row3c,f.mode118,false};
  if(!s.construct_save||!s.construct_save(allocation,request,error_))return fail("Required SAME LevelSavegame C1");
  f.save_ec=std::move(allocation);
 }
 phase_=LevelConstructorPhaseV3::complete;error_.clear();return true;
}
}
