#define main prior_level_bindings_main
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"
#include "../../level-loader/tests/level_constructor_bindings_v4.cpp"
#pragma clang diagnostic pop
#undef main
#include "../character_kill_level_events_v23.hpp"
using namespace dh2::character;
struct QuestObserverV23 {
 std::shared_ptr<CanonicalLevelContextV1>* global{};CharacterKillLevelEventsV23* routing{};
 const KillQuest48* expected{};std::vector<KillQuest48> seen;bool clear_global{},fail{};
 static bool event(void* p,const events::EventBorrowV12& event,events::EventManagerOwnerV12&,std::int32_t& result,std::string& error){
  auto& self=*static_cast<QuestObserverV23*>(p);auto* actual=kill_quest_payload_v23(event);check(actual&&actual==self.expected&&event.identity==reinterpret_cast<std::uintptr_t>(actual));self.seen.push_back(*actual);result=0;
  if(self.clear_global){self.global->reset();self.routing->begin();KillRequest56 q{};q.service=kill_current_level;KillResponse16 out{};bool handled{};check(self.routing->route(q,out,handled,error)&&handled&&out.pointer==0);self.routing->end();self.clear_global=false;}
  if(self.fail){error="actual quest receiver failure";return false;}return true;
 }
};
int main(int argc,char** argv){try{check(argc==4);Inputs inputs(argv[1]);character::CharacterGameDesign design;std::string error;check(design.initialize(inputs.input,error));auto actual=std::make_shared<CharacterGameDesign::Borrow>(design.borrow());
 auto files=std::make_shared<ApplicationFilesFixture>();files->script_dir=argv[2];files->save_dir=argv[3];auto cache=std::make_shared<scripts::LuaScriptCacheOwnerV13>(scripts::LuaScriptCacheServicesV13{files,files.get(),ApplicationFilesFixture::script});std::uint32_t debug{},module{};
 LevelConstructorApplicationV4 app;app.owner=actual;app.debug_level_load_count=&debug;app.module_id_global=&module;app.levels=actual->levels();app.lua_cache=cache;app.lua.owner=actual;app.lua.design=*actual->design();app.private_vm_limit=16u*1024u*1024u;app.saves.files.context=files.get();app.saves.files.read_file=ApplicationFilesFixture::save;app.saves.files.storage_lease=files;app.online_byte5=[](auto& value,auto&){value=0;return true;}; // Explicit offline source-state fixture.
 std::string selected;for(auto& row:actual->levels()->levels){std::string lower=row.file;for(auto& c:lower)c=char(std::tolower(static_cast<unsigned char>(c)));if(lower.find("swamp")!=std::string::npos){selected="worlds/"+lower;break;}}check(!selected.empty());
 std::shared_ptr<CanonicalLevelContextV1> level;LevelSourceRequestV1 source;source.identity=selected;source.definition="isolated source selection";source.seed=7;check(CanonicalLevelContextV1::create(source,actual,level,error));auto bindings=LevelConstructorBindingsV4::create(app,error);check(bool(bindings));check(level->construct_source_v3({selected.c_str(),0,7,1,0,1,0,-1,0},bindings->services(),error));check(level->constructor_fields_v3().events&&level->kill_level()->loot_gate150==0&&bindings->script()->loaded_files().size()==2&&bindings->save()->owner().ready());
 std::shared_ptr<CanonicalLevelContextV1> global=level;auto global_owner=std::make_shared<int>(1);CanonicalGSLevelGlobalSlotV1 slot{global_owner,&global};CharacterKillLevelEventsV23 routing({global_owner,[&](auto& out,auto& e){return borrow_current_canonical_level_v1(slot,out,e);}});KillRequest56 current{};current.service=kill_current_level;KillResponse16 out{};bool handled{};check(!routing.route(current,out,handled,error));routing.begin();check(routing.route(current,out,handled,error)&&out.pointer==reinterpret_cast<std::uintptr_t>(level->kill_level()));
 auto observer=std::make_shared<QuestObserverV23>();observer->global=&global;observer->routing=&routing;observer->clear_global=true;const auto identity=level->identity();auto* manager=level->constructor_fields_v3().events.get();std::weak_ptr<CanonicalLevelContextV1> weak=level;
 const char* keys[]{"KillXEnemies","ClearEnemies","KillEnemyTemplate","ClearEnemyTemplate"};const auto* constants=actual->design();
 for(unsigned i=0;i<4;++i){std::int32_t type{};check(constants->lookup(constants->context,0,"v2QuestObjectiveType",keys[i],&type)==0);bool inserted{};check(manager->attach(type,{reinterpret_cast<std::uintptr_t>(observer.get()),observer.get(),QuestObserverV23::event,observer},0,inserted,error)&&inserted);}
 level.reset();
 for(unsigned i=0;i<4;++i){std::int32_t type{};check(constants->lookup(constants->context,0,"v2QuestObjectiveType",keys[i],&type)==0);KillQuest48 event{i+1,std::uint32_t(type),0x100000001ull,44,-1,i<2?263:17,0,0,0,identity,0};observer->expected=&event;KillRequest56 q{};q.service=kill_raise_async;q.subject=identity;q.event=&event;check(routing.route(q,out,handled,error)&&handled);check(observer->seen.size()==i+1&&observer->seen.back().type==event.type&&observer->seen.back().subject_id==event.subject_id);check(manager->pending_count()==0&&!weak.expired());}
 check(!global&&observer->seen.size()==4);routing.end();check(weak.expired());check(!routing.route(current,out,handled,error));
 // Genuine empty global returns NULL only inside a scope; source Kill keeps
 // its reached unsafe null-Level failure rather than receiving a fake Level.
 routing.begin();check(routing.route(current,out,handled,error)&&out.pointer==0);routing.end();
 std::cout<<"PASS same actual Level C1/51rows/real Lua-cache/native Save + gate150 + four real constants/typed synchronous QE listeners + nested current-level change/old-level pin/no queue checks="<<checks<<"; quest handler is declared observer, not progress acceptance\n";
}catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
