#include "../player_level_quicksave_v29.hpp"
#include <iostream>
#include <stdexcept>
#include <array>
using namespace dh2::player;
static unsigned checks;
static void check(bool b,const std::string& e){++checks;if(!b)throw std::runtime_error(e);}
struct Fixture {
 PlayerManagerOwnerV1 manager{{this,service}};std::string error;PlayerInfoFieldsV1* player{};
 bool online{},hosting{true},fail_save{};unsigned manager_queries{},borrows{},online_queries{},host_queries{},save_calls{};std::uint8_t suppress{};
 std::shared_ptr<void> lease=std::make_shared<int>(1),saved=std::make_shared<int>(2),alternate=std::make_shared<int>(3),primary=saved;
 std::uint32_t phase{},dead{};std::uint8_t flag=7,alternate_flag=9;std::array<float,3> position{{113,227,331}},checkpoint{};
 bool replace_save{};
 static bool service(void* raw,const PlayerManagerRequestV1& q,PlayerManagerResponseV1& out,std::string& e){auto& f=*static_cast<Fixture*>(raw);
  if(q.operation==PlayerManagerOperationV1::construct_player_info){*q.player=PlayerInfoFieldsV1{};return true;}
  if(q.operation==PlayerManagerOperationV1::online_enabled){++f.manager_queries;out.value=0;return true;} // Declared offline manager provider, separate QuickSave online cases.
  e="Required unrelated manager operation";return false;
 }
 Fixture(){check(manager.initialize(error)&&manager.add_player(0,0,0,true,error)&&manager.get_by_internal(0,false,player,error),error);player->character660=0x100000001ULL;}
 PlayerQuickSaveServicesV29 services(){PlayerQuickSaveServicesV29 s;s.owner=lease;s.players=&manager;
  s.character=[this](auto id,auto& out,std::string& e){++borrows;if(id!=player->character660){e="Wrong real Character";return false;}out={lease,id,&dead,position.data(),checkpoint.data()};return true;};
  s.online_byte5=[this](auto& out,auto&){++online_queries;out=online;return true;};
  s.local_hosting=[this](auto& out,auto&){++host_queries;out=hosting;return true;};s.player_manager719=&suppress;
  s.save_flag39=[this](const auto& actual,auto*& out,std::string& e){if(actual==primary)out=&flag;else if(actual==alternate)out=&alternate_flag;else{e="Wrong source Save_ec receiver";return false;}return true;};
  s.save_ec=[this](const auto& actual,std::string& e){++save_calls;check(actual==saved,"Wrong saved receiver");if(fail_save){e="Actual fixture save failure";return false;}if(replace_save)saved=alternate;return true;};return s;
 }
};
int main(){try{
 Fixture f;PlayerLevelQuickSaveV29 quick(f.services());PlayerQuickSaveLevelV29 level{f.lease,0x100000002ULL,&f.saved,&f.phase};
 auto before_queries=f.manager_queries;check(quick.execute(level,false,f.error),f.error);check(f.manager_queries>before_queries&&!f.borrows&&!f.save_calls,"C1 phase0 must query local player then return");check(*f.manager.character_count_field()==0,"QuickSave fabricated initialized count");
 f.phase=38;f.dead=1;check(quick.execute(level,true,f.error)&&f.borrows==1&&!f.online_queries,f.error);
 f.dead=0;f.online=true;f.hosting=false;check(quick.execute(level,true,f.error)&&!f.save_calls,f.error);
 f.hosting=true;f.suppress=1;check(quick.execute(level,true,f.error)&&!f.save_calls,f.error);
 f.suppress=0;f.online=false;check(quick.execute(level,true,f.error),f.error);check(f.save_calls==1&&f.checkpoint==f.position&&f.flag==7&&quick.receipt().saved,"Actual checkpoint/temporary flag/save/restore sequence differs");
 f.fail_save=true;check(!quick.execute(level,true,f.error)&&f.flag==0&&quick.receipt().phase==PlayerQuickSavePhaseV29::save,"Save failure erased reached prefix");
 // Source restores captured old flag to a freshly reread actual Save_ec.
 Fixture g;g.phase=38;g.replace_save=true;PlayerLevelQuickSaveV29 replace(g.services());PlayerQuickSaveLevelV29 other{g.lease,0x100000003ULL,&g.saved,&g.phase};
 check(replace.execute(other,true,g.error),g.error);check(g.saved==g.alternate&&g.alternate_flag==7,"QuickSave restored flag to a stale receiver");
 std::cout<<"PASS "<<checks<<" checks; source local query before C1 guard; actual count remains0; positive save callbacks declared fixtures; checkpoint/online/gates/prefix/rebound-flag behavior\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
