#include "../level_update_loading_v46.hpp"
#include "../stage_loader_v46_physical.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::loader;
// Explicit source-differential field fixture; production uses completed
// CanonicalLevelContext C1, covered by the separate real-C1 test.
struct Context:std::enable_shared_from_this<Context> {
 struct Fields {std::uint32_t phase30{},field130{},field134{9},field138{11},field13c{13};std::uint8_t byte144{127};} fields;
 struct Constructor {enum class Phase{incomplete,complete};Phase state{Phase::complete};Phase phase()const{return state;}} constructor;
 struct LoadingFieldsV26 {std::shared_ptr<void> level_owner;std::uintptr_t identity;std::uint32_t *progress30,*state130;};
 struct Borrow {std::shared_ptr<void> owner;std::uintptr_t identity;Fields* fields;};
 const Constructor* constructor_owner_v3()const{return &constructor;}
 std::uintptr_t identity()const{return reinterpret_cast<std::uintptr_t>(this);}
 Borrow constructor_borrow_v3(){return {shared_from_this(),identity(),&fields};}
 bool loading_fields_v26(LoadingFieldsV26& out,std::string&){out={shared_from_this(),identity(),&fields.phase30,&fields.field130};return true;}
};
static void check(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
using Bridge=LevelLoadingUpdateV46<Context>;
static std::unique_ptr<Bridge> bridge(std::shared_ptr<Context> c,std::function<bool(std::string&)> body){std::unique_ptr<Bridge> out;std::string e;check(Bridge::create(c,{c,c->identity(),std::move(body)},out,e),e);return out;}
struct App {std::uint32_t bigI{0x12345678},bigV{0xabcdef00};std::uint8_t byteb4{126};};
static Stage0ApplicationBorrowV46 app_borrow(std::shared_ptr<App> app){return {app,app,reinterpret_cast<std::uintptr_t>(app.get()),&app->bigI,&app->bigV,&app->byteb4};}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;std::ifstream gold(argv[1]);check(bool(gold),"original loading gold unavailable");std::size_t n{},verified=0;gold>>n;
 for(std::size_t i=0;i<n;++i){std::uint32_t phase,force,updated,expected_state,gameplay;gold>>phase>>force>>updated>>expected_state>>gameplay;
  auto c=std::make_shared<Context>();c->fields.field130=phase;unsigned calls=0;
  auto b=bridge(c,[&](std::string&){++calls;c->fields.field130=updated;return true;});
  auto result=b->update(c,bool(force));check(original_level_update_loading_branch_v46(phase,bool(force))==!gameplay,"original branch classifier differs");
  if(gameplay)check(result==LevelLoadingUpdateResultV46::main_gameplay_required&&calls==0&&c->fields.field130==expected_state,"gameplay gate executed loading provider");
  else if(phase>37)check(result==LevelLoadingUpdateResultV46::failed&&calls==0&&c->fields.field130==phase,"native invalid-domain guard failed");
  else check(result==LevelLoadingUpdateResultV46::advanced&&calls==1&&c->fields.field130==expected_state,"original loading envelope differs");
  ++verified;
 }
 auto c=std::make_shared<Context>();std::string e;unsigned guards=0;
 {auto old=bridge(c,{});auto* saved=old.get();auto foreign=std::make_shared<Context>();check(!Bridge::create(c,{foreign,foreign->identity(),{}},old,e)&&old.get()==saved,"foreign body replaced output");++guards;}
 {auto old=bridge(c,{});auto* saved=old.get();c->constructor.state=Context::Constructor::Phase::incomplete;check(!Bridge::create(c,{c,c->identity(),{}},old,e)&&old.get()==saved,"incompleteC1 replaced output");c->constructor.state=Context::Constructor::Phase::complete;++guards;}
 {auto b=bridge(c,{});check(b->update(c,false)==LevelLoadingUpdateResultV46::failed&&b->required_service()=="Level._LoadProcess"&&c->fields.field130==0,"missing load invented Init");++guards;}
 {auto b=bridge(c,[](std::string&){return true;});check(b->update(std::make_shared<Context>(),false)==LevelLoadingUpdateResultV46::failed&&b->load_process_calls()==0,"foreign receiver ran source");++guards;}
 {Bridge* ptr{};unsigned calls=0;auto b=bridge(c,[&](std::string&){++calls;c->fields.field134=7;check(ptr->update(c,false)==LevelLoadingUpdateResultV46::failed,"nested update accepted");return true;});ptr=b.get();check(b->update(c,false)==LevelLoadingUpdateResultV46::failed&&calls==1&&c->fields.field134==7&&b->update(c,false)==LevelLoadingUpdateResultV46::failed&&calls==1,"nested failure lost originalprefix");++guards;}
 {auto b=bridge(c,[&](std::string&)->bool{c->fields.field138=3;throw std::runtime_error("source failed after prefix");});check(b->update(c,false)==LevelLoadingUpdateResultV46::failed&&c->fields.field138==3,"exception lost prefix");++guards;}
 unsigned prefixes=0;
 for(unsigned failure=0;failure<4;++failure){
  auto actual=std::make_shared<Context>();auto app=std::make_shared<App>();Stage0LevelBorrowV46 borrowed;check(borrow_stage0_level_v46(actual,borrowed,e),e);std::vector<unsigned> events;auto sound=std::make_shared<int>(0);Stage0ServicesV46 services;
  if(failure!=1)services.clean_glitch=[&](const auto& view,std::string& error){events.push_back(1);check(*view.g_bigI==0&&*view.g_bigV==0,"globals notresetbeforeCleanGlitch");if(failure==2){error="device refused";return false;}return true;};
  services.sound_manager_owner=sound;services.sound_manager_identity=reinterpret_cast<std::uintptr_t>(sound.get());
  services.stop_all_sounds=[&](std::uintptr_t receiver,int fade,std::string& error){events.push_back(2);check(receiver==reinterpret_cast<std::uintptr_t>(sound.get())&&fade==500,"original audio receiver/fade changed");if(failure==3){error="Vox refused";return false;}return true;};
  auto body=stage0_body_v46(std::move(borrowed),app_borrow(app),std::move(services));auto result=body(e);check(app->bigI==0&&app->bigV==0&&actual->fields.field130==0,"stage0 original globalprefix/increment differs");
  if(!failure){check(result==LifecycleStepV36::complete&&app->byteb4==1&&actual->fields.field134==0&&actual->fields.field138==0&&actual->fields.field13c==0&&actual->fields.byte144==0&&events==std::vector<unsigned>({1,2}),"stage0 source reset order differs");}
  else check(result==LifecycleStepV36::failed&&app->byteb4==126&&actual->fields.field134==9&&actual->fields.field138==11&&actual->fields.field13c==13&&actual->fields.byte144==127,"failedstage0 changed unreached suffix");
  auto count=events.size();check(body(e)==LifecycleStepV36::failed&&events.size()==count,"stage0 replayed partial side effects");++prefixes;
 }
 {auto actual=std::make_shared<Context>();auto app=std::make_shared<App>();Stage0LevelBorrowV46 borrowed;check(borrow_stage0_level_v46(actual,borrowed,e),e);std::function<LifecycleStepV36(std::string&)> body;Stage0ServicesV46 s;s.clean_glitch=[&](const auto&,std::string&){std::string inner;check(body(inner)==LifecycleStepV36::failed,"stage0 nested call accepted");return true;};body=stage0_body_v46(borrowed,app_borrow(app),s);check(body(e)==LifecycleStepV36::failed&&actual->fields.field134==9&&app->byteb4==126&&e.find("reentered")!=std::string::npos,"stage0 nested failure overwritten");++guards;}
 unsigned physics=0;
 {auto actual=std::make_shared<Context>();actual->fields.field130=5;LifecycleBorrowV36 borrow;check(borrow_lifecycle_fields_v36(actual,borrow,e),e);auto world=std::make_shared<dh2::physical::NativeWorld>();auto debug=std::make_shared<int>(0);unsigned debugcalls=0;
  auto body=stage5_physical_body_v46(borrow,world,{debug,[&](const char* key,bool& ignored,std::string&){check(std::string(key)=="isTracingLevel_Loading"&&!world->backend(),"stage5 query order/key changed");ignored=true;++debugcalls;return true;}});
  check(body(e)==LifecycleStepV36::complete&&actual->fields.field130==5&&world->backend()&&world->backend()->GetBodyCount()==1&&debugcalls==1,"actual stage5 world load failed");auto backend=world->backend();
  for(float x:{-1999.0f,1999.0f,-2001.0f,2001.0f}){b2BodyDef d;auto* b=world->create(&d);check(b!=nullptr,"actual physical body absent");b2CircleDef circle;circle.radius=0.1f;b->CreateShape(&circle);b->SetXForm(b2Vec2(x,0),0);b->SetXForm(b2Vec2(x,0),0);check(b->IsFrozen()==(x<-2000||x>2000),"actual source2000broadphasebounds differ");world->destroy(b);++physics;}
  check(body(e)==LifecycleStepV36::failed&&world->backend()==backend&&debugcalls==1,"stage5 reset replayedbackend");++guards;
 }
 {auto actual=std::make_shared<Context>();actual->fields.field130=5;LifecycleBorrowV36 borrow;check(borrow_lifecycle_fields_v36(actual,borrow,e),e);auto world=std::make_shared<dh2::physical::NativeWorld>();auto body=stage5_physical_body_v46(borrow,world,{});check(body(e)==LifecycleStepV36::dependency_missing&&!world->backend()&&actual->fields.field130==5,"missingstage5debugcreatedworld");++guards;}
 {auto actual=std::make_shared<Context>();actual->fields.field130=5;LifecycleBorrowV36 borrow;check(borrow_lifecycle_fields_v36(actual,borrow,e),e);auto world=std::make_shared<dh2::physical::NativeWorld>();std::function<LifecycleStepV36(std::string&)> body;auto pin=std::make_shared<int>(0);body=stage5_physical_body_v46(borrow,world,{pin,[&](const char*,bool&,std::string&){std::string inner;check(body(inner)==LifecycleStepV36::failed,"stage5 reentered");return true;}});check(body(e)==LifecycleStepV36::failed&&!world->backend(),"stage5 reentry stillcreatedbackend");++guards;}
 std::cout<<"{\"validation\":\"PASS\",\"original_loading_branch_cases\":"<<verified<<",\"stage0_order_failure_prefix_cases\":"<<prefixes<<",\"real_physics_boundary_cases\":"<<physics<<",\"native_guards\":"<<guards<<",\"field_receiver_fixture\":true,\"actual_physics_body\":true,\"engine_callbacks_are_explicit_fixtures\":true,\"whole_level_init_verified\":false}\n";
 return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
