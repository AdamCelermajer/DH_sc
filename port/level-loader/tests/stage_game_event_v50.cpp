#include "stage_loader_v50_game_events.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
using namespace dh2::loader;
using Bytes=std::vector<std::uint8_t>;
Bytes file(const std::string& p){std::ifstream s(p,std::ios::binary);assert(s);return Bytes(std::istreambuf_iterator<char>(s),{});}
// ONLY stage6 body fixture: these source-phase6 stores stand in for a completed
// actual C1 borrow. No claim that stages0..5 or actual PlayerManager.Update ran.
struct LevelFixture {std::uint32_t state130{6};std::uintptr_t field194{};std::shared_ptr<GameEventManagerV50> retained;};
GameEventLevelFieldsV50 borrow(const std::shared_ptr<LevelFixture>& p){return {p,reinterpret_cast<std::uintptr_t>(p.get()),&p->state130,&p->field194,&p->retained};}
int main(int argc,char** argv){assert(argc==2);auto rows=file(std::string(argv[1])+"/v2eventmanager_pyarray.bin"),names=file(std::string(argv[1])+"/v2eventmanager_pyarraynames.bin");GameEventTablesV50 tables;std::string error;assert(tables.initialize(rows.data(),rows.size(),names.data(),names.size(),error));
 unsigned cases=0;auto owner=std::make_shared<int>(1);std::uint8_t player6c9=0;std::vector<int> order;
 auto services=[&](){Stage6ServicesV50 s;s.debug.actual_owner=owner;s.debug.get_instance_and_switch=[&](const char* key,bool&,std::string&){assert(std::string(key)=="isTracingLevel_Loading");order.push_back(1);return true;};s.player={owner,1,&player6c9};s.player_update=[&](const auto& actual,std::string&){assert(actual.byte6c9==&player6c9&&player6c9==1);order.push_back(2);return true;};s.actual_events=tables.borrow();return s;};
 auto level=std::make_shared<LevelFixture>();std::weak_ptr<GameEventManagerV50> retained;
 {auto run=stage6_game_events_v50(borrow(level),services());assert(run(error)==LifecycleStepV36::pending);assert(level->field194==reinterpret_cast<std::uintptr_t>(level->retained.get())&&level->retained->events().size()==66&&level->state130==6);retained=level->retained;LifecycleStepV36 result=LifecycleStepV36::pending;for(int n=0;n<2000&&result==LifecycleStepV36::pending;++n)result=run(error);assert(result==LifecycleStepV36::complete&&level->retained->diagnostics().published==66&&order==std::vector<int>({1,2})&&level->state130==6);assert(run(error)==LifecycleStepV36::failed);++cases;}
 assert(!retained.expired());level.reset();assert(retained.expired());
 // Real required bodies fail at the reached prefix; no success stubs.
 order.clear();player6c9=0;level=std::make_shared<LevelFixture>();{auto s=services();s.player_update={};auto run=stage6_game_events_v50(borrow(level),s);assert(run(error)==LifecycleStepV36::failed&&player6c9==1&&!level->retained&&!level->field194&&order==std::vector<int>({1}));assert(run(error)==LifecycleStepV36::failed&&order.size()==1);++cases;}
 order.clear();player6c9=0;level=std::make_shared<LevelFixture>();{auto s=services();s.debug={};auto run=stage6_game_events_v50(borrow(level),s);assert(run(error)==LifecycleStepV36::failed&&!player6c9&&!level->retained&&order.empty());++cases;}
 order.clear();level=std::make_shared<LevelFixture>();{auto s=services();s.actual_events={};auto run=stage6_game_events_v50(borrow(level),s);assert(run(error)==LifecycleStepV36::failed&&level->retained&&level->field194==reinterpret_cast<std::uintptr_t>(level->retained.get())&&level->retained->events().empty()&&level->state130==6);assert(run(error)==LifecycleStepV36::failed&&order==std::vector<int>({1,2}));++cases;}
 order.clear();level=std::make_shared<LevelFixture>();{auto s=services();s.player_update=[&](const auto&,std::string& e){e="Actual Update missing";return false;};auto run=stage6_game_events_v50(borrow(level),s);assert(run(error)==LifecycleStepV36::failed&&error=="Actual Update missing"&&!level->retained&&!level->field194);++cases;}
 order.clear();level=std::make_shared<LevelFixture>();{auto s=services();std::function<LifecycleStepV36(std::string&)> run;s.player_update=[&](const auto&,std::string&){assert(run(error)==LifecycleStepV36::failed);return true;};run=stage6_game_events_v50(borrow(level),s);assert(run(error)==LifecycleStepV36::failed&&!level->retained&&!level->field194);++cases;}
 level=std::make_shared<LevelFixture>();{auto s=services();auto run=stage6_game_events_v50(borrow(level),s);assert(run(error)==LifecycleStepV36::pending);level->field194=1;assert(run(error)==LifecycleStepV36::failed&&level->retained->diagnostics().published==0);++cases;}
 level=std::make_shared<LevelFixture>();level->state130=0;{auto run=stage6_game_events_v50(borrow(level),services());assert(run(error)==LifecycleStepV36::failed&&!level->retained&&!level->field194);++cases;}
 // Callback may release the Level's lease; receiver remains pinned until
 // return and the changed alias must fail, including under ASAN.
 level=std::make_shared<LevelFixture>();{auto s=services();s.modern_allocation_policy.owner=owner;s.modern_allocation_policy.allow=[&](auto,auto,std::string&){level->retained.reset();level->field194=0;return true;};auto run=stage6_game_events_v50(borrow(level),s);assert(run(error)==LifecycleStepV36::failed&&!level->retained&&!level->field194);++cases;}
 std::cout<<"{\"validation\":\"PASS\",\"stage6_body_cases\":"<<cases<<",\"event194_lease_persists_after_stage_body\":true,\"source_trace_player_publish_load_order\":true,\"same_completed_actual_C1_verified\":false,\"PlayerManagerUpdate_body_is_fixture\":true,\"whole_level_ready\":false}\n";
}
