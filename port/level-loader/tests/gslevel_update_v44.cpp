#include "../gslevel_update_v44.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <array>
using namespace dh2::loader;
void require(bool ok,const char* why){if(!ok)throw std::runtime_error(why);}
struct Level {std::uint32_t phase{};std::uint8_t byte145{};unsigned id{};};
struct Fixture {
 std::shared_ptr<Level> level=std::make_shared<Level>(),other=std::make_shared<Level>();
 std::shared_ptr<void> owner=std::make_shared<int>(1);GSLevelFieldsV2<Level> fields;
 std::uint8_t paused{},flag=126;std::uint32_t word=0x13579bdf;bool enabled{},mutation{};std::uint32_t update{};std::vector<std::array<std::uint32_t,4>> trace;
 GSApplicationBorrowV44 app(){return {owner,&paused,&word,&flag};}
 GSLevelUpdateServicesV44<Level> services(){GSLevelUpdateServicesV44<Level> s;s.actual_debug_owner=owner;
 s.borrow_level=[](const auto& level,auto& out,std::string&){out={level,reinterpret_cast<std::uintptr_t>(level.get()),&level->phase,&level->byte145};return true;}; // Explicit source-field fixture, not actual C1 provider.
 s.level_load=[this](const auto& l,std::string&){trace.push_back({4,l->id,0,0});l->phase=0;return true;};
 s.level_update=[this](const auto& l,bool flag,std::string&){trace.push_back({5,l->id,std::uint32_t(flag),0});l->phase=update;if(mutation)fields.level34=other;return true;};
 s.debug_load=[this](std::string&){trace.push_back({1,word,flag,0});return true;};
 s.debug_get_switch=[this](const char* name,bool& out,std::string&){require(std::string(name)=="Lua_DumpCalls","source debug key");trace.push_back({2,std::uint32_t(enabled),0,0});out=enabled;return true;};
 s.debug_set_switch=[this](const char* name,bool value,std::string&){require(std::string(name)=="Lua_DumpCalls","source debug set key");trace.push_back({3,std::uint32_t(value),0,0});return true;};
 s.menu_instance=[this](auto& out,std::string&){trace.push_back({6,0,0,0});out={owner,reinterpret_cast<std::uintptr_t>(owner.get())};return true;};
 s.menu_update=[this](const auto&,bool value,std::string&){trace.push_back({7,std::uint32_t(value),0,0});return true;};return s;
 }
};
int main(int argc,char** argv){if(argc!=2)return 2;try{std::ifstream gold(argv[1]);unsigned count{};gold>>count;require(bool(gold),"gold missing");unsigned checked=0;
 for(unsigned i=0;i<count;++i){Fixture f;std::array<std::uint32_t,9> in;std::array<std::uint32_t,7> expected;unsigned events;
  for(auto& v:in)gold>>v;for(auto& v:expected)gold>>v;gold>>events;std::vector<std::array<std::uint32_t,4>> trace(events);for(auto& event:trace)for(auto& v:event)gold>>v;require(bool(gold),"short gold");
  f.fields.loading38=in[0];f.paused=in[1];f.level->id=1;f.other->id=2;f.level->byte145=f.other->byte145=in[3];f.level->phase=in[4];f.other->phase=in[8];f.enabled=in[5];f.update=in[6];f.mutation=in[7];f.fields.active3c=127;if(in[2])f.fields.level34=f.level;
  GSLevelUpdateV44<Level> runtime(f.owner,f.fields,f.app(),f.services());const auto result=runtime.tick();require(result==(f.paused?GSLevelUpdateResultV44::application_gate:GSLevelUpdateResultV44::advanced),"native source result");
  const std::array<std::uint32_t,7> actual{f.fields.loading38,f.level->phase,f.other->phase,f.word,f.flag,f.fields.active3c,f.fields.level34?f.fields.level34->id:0};
  require(actual==expected&&f.trace==trace,"original GS branches/field reread/global/menu trace differ");++checked;
 }
 unsigned safeguards=0;
 {Fixture f;f.fields.loading38=2;GSLevelUpdateV44<Level> run(f.owner,f.fields,f.app(),f.services());require(run.tick()==GSLevelUpdateResultV44::failed&&f.fields.loading38==2,"unsafe null source Level.Load must remain required");++safeguards;}
 {Fixture f;f.fields.loading38=3;f.fields.level34=f.level;auto s=f.services();s.level_update={};GSLevelUpdateV44<Level> run(f.owner,f.fields,f.app(),s);require(run.tick()==GSLevelUpdateResultV44::failed&&f.fields.loading38==3&&f.trace.empty(),"missing Level.Update lost prefix");++safeguards;}
 {Fixture f;f.fields.loading38=4;f.fields.level34=f.level;auto s=f.services();s.debug_load={};GSLevelUpdateV44<Level> run(f.owner,f.fields,f.app(),s);require(run.tick()==GSLevelUpdateResultV44::failed&&f.word==0&&f.flag==0&&f.fields.loading38==4,"Debug failure rolled back source global prefix");const auto before=f.trace;require(run.tick()==GSLevelUpdateResultV44::failed&&f.trace==before,"failed GS.Update replayed");safeguards+=2;}
 {Fixture f;f.fields.loading38=3;f.fields.level34=f.level;auto s=f.services();s.borrow_level=[&](const auto&,auto& out,std::string&){out={f.other,reinterpret_cast<std::uintptr_t>(f.other.get()),&f.other->phase,&f.other->byte145};return true;};GSLevelUpdateV44<Level> run(f.owner,f.fields,f.app(),s);require(run.tick()==GSLevelUpdateResultV44::failed&&f.fields.loading38==3,"foreign read borrow substituted source level");++safeguards;}
 {Fixture f;f.fields.loading38=3;f.fields.level34=f.level;auto s=f.services();GSLevelUpdateV44<Level>* nested{};s.level_update=[&](const auto&,bool,std::string&){require(nested->tick()==GSLevelUpdateResultV44::failed,"reentry accepted");return true;};GSLevelUpdateV44<Level> run(f.owner,f.fields,f.app(),s);nested=&run;require(run.tick()==GSLevelUpdateResultV44::failed&&f.fields.loading38==3,"reentry failure overwritten by source tail");++safeguards;}
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<checked<<",\"native_safeguards\":"<<safeguards<<",\"current_level_global_created\":false,\"whole_level_init_verified\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
