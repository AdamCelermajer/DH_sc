// Compile the shipping Stage32 adapter and verbatim shipping SpotTarget leaf.
// Fixtures supply only the retained receiver/callback boundaries; PFWorld,
// policy storage, selector/octree/collision and ValidatePosition are real code.
#include <floors.hpp>
#include <lifecycle_v36.hpp>
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
static unsigned checks;
static void check(bool value,const char* why){++checks;if(!value)throw std::runtime_error(why);}
namespace dh2::loader {
struct EarlyLoadingDebugV46{};
inline bool early_loading_trace_v46(const EarlyLoadingDebugV46&,std::string&){return true;}
struct CanonicalLevelContextV1 {
 struct Fields{std::uint32_t field130{32};std::uintptr_t field128{1};} fields;
 std::uintptr_t config{1};
 const Fields& constructor_fields_v3()const{return fields;}
 struct Config{const std::uintptr_t* config38;};
 Config config_fields()const{return{&config};}
};
struct SourceLoadingInputsV43{LifecycleServicesV36 external;};
}
namespace model_renderer {
struct TestApplication{};
struct TestManager {std::vector<std::uintptr_t> characters_;const auto& characters()const{return characters_;}};
struct TestScene {std::array<float,4> ambient{};void source_set_ambient_v67(const std::array<float,4>& value){ambient=value;}};
struct TestCanonical {TestManager manager;std::shared_ptr<TestManager> manager_lease;std::shared_ptr<TestScene> scene_roots_v20=std::make_shared<TestScene>();};
struct TestConfig {std::array<float,3> rgb{.1f,.2f,.3f};const auto* vector(unsigned offset)const{return offset==0x1cc?&rgb:nullptr;}};
struct TestModules {TestConfig config;unsigned reads{};const TestConfig* level_config(std::uintptr_t id){++reads;return id==1?&config:nullptr;}};
struct TestFactory {std::shared_ptr<TestModules> modules=std::make_shared<TestModules>();};
struct TestPreparation {std::shared_ptr<dh2::loader::CanonicalLevelContextV1> level_;TestManager* manager_;const auto& level()const{return level_;}TestManager& manager()const{return *manager_;}};
struct TestRooms {std::shared_ptr<dh2::floors::World> floors;const auto& world()const{return floors;}};
struct RendererSourceCandidateV55 {std::shared_ptr<TestFactory> factory;std::shared_ptr<TestPreparation> preparation;std::shared_ptr<TestCanonical> canonical;std::shared_ptr<TestRooms> rooms;};
struct TestGameplay {
 std::shared_ptr<void> provider;std::shared_ptr<TestApplication> actual_application;std::shared_ptr<TestManager> actual_object_manager;
 std::function<bool(float,std::string&)> update_objects;std::function<bool(std::string&)> physical_update,camera_update;
};
struct SourceWorldBorrowV61 {std::shared_ptr<void> owner;std::shared_ptr<TestApplication> application;std::shared_ptr<TestCanonical> canonical_world;std::shared_ptr<dh2::floors::World> source_pf_floors_v115;std::shared_ptr<TestGameplay> gameplay_services_v66;};
struct TestMachine {struct FSM{std::uintptr_t character{};} fsm;const auto& native_fsm()const{return fsm;}int initialize_level_preset_v95(int){return 1;}const std::string& error()const{static std::string e;return e;}};
struct TestActor {struct Object{std::uintptr_t identity{};};std::shared_ptr<Object> object;std::shared_ptr<TestMachine> machine;const std::string* source_string(unsigned)const{static const std::string s;return &s;}};
struct TestCharacter {std::shared_ptr<TestActor> actor;};
struct SourceCampaignCharacterBorrowV62 {std::shared_ptr<TestCharacter> character;};
static bool borrow_source_campaign_character_v62(const std::shared_ptr<void>&,std::uintptr_t,SourceCampaignCharacterBorrowV62&,std::string&){return false;}
static std::function<bool(const std::shared_ptr<void>&,std::string&)> ai_callback;
static bool source_campaign_ai_inc_queue_v105(const std::shared_ptr<void>& world,std::string& e){return ai_callback&&ai_callback(world,e);}
#include "../app/src/main/cpp/renderer_source_stage18_stage32_v95.inc"
struct TestSpotFields {std::uintptr_t spot_fx14cc{9};bool spot_enabled14c8{true};std::int32_t spot_set14ca{1};float spot14b0[3]{10,10,250},previous_spot14bc[3]{3,4,5};};
struct TestSpotActor {TestSpotFields fields;auto* source_frame_fields_v106(){return &fields;}};
struct TestFx {unsigned rotations{},syncs{};bool grab_marker_v28(std::int32_t,std::uintptr_t,std::uintptr_t& marker,std::string&){marker=9;return true;}bool drop(std::uintptr_t& marker,std::string&){marker=0;return true;}bool marker_rotation_v70(std::uintptr_t,const float*,std::string&){++rotations;return true;}bool marker_sync_v83(std::uintptr_t,bool,std::string&){++syncs;return true;}};
struct Record {std::shared_ptr<TestSpotActor> actor=std::make_shared<TestSpotActor>();std::shared_ptr<TestFx> target_fx_manager_v70=std::make_shared<TestFx>();std::shared_ptr<void> target_fx_pin_v70=std::make_shared<int>(1);};
struct SourceCampaignCandidateBorrowV55 {std::shared_ptr<dh2::floors::World> floors;};
// Generated from the current production file by the runner, without rewriting
// its body or substituting a navigation/policy function.
#include "stage32-production-spot.inc"
}
struct Geometry {
 std::shared_ptr<dh2::floors::World> world=std::make_shared<dh2::floors::World>();
 dh2::collision::Triangle triangle{{{0,0,0},{100,0,0},{0,100,0}}},selected{};
 std::array<dh2::octree::Node,8> nodes{};std::array<unsigned,64> indices{},scratch{};unsigned selected_id{};
 dh2::octree::Tree tree{};dh2::selector::Workspace workspace{};dh2::selector::Floor selector{};unsigned floor_index{};
 Geometry(){
  tree={nodes.data(),indices.data(),scratch.data(),&triangle,1,0,0,8,64,64,1,0,0};
  check(dh2_octree_build(&tree,&triangle,1,1)==0,"actual octree build");
  workspace={&selected_id,&selected,1,0};dh2::octree::Box bounds{{-1,-1,-500},{101,101,500}};
  selector={{&tree,nullptr,0,0},bounds,&workspace};
  world->collision_floors.push_back({&selector,{0,1}});world->collision_rooms.push_back({bounds,&floor_index,1,0});
  world->collision_world={world->collision_rooms.data(),world->collision_floors.data(),1,1,bounds};
 }
 void validate(bool override_expected){
  auto& policy=world->source_motion_policy_v95;check(policy.ignore_height_delta==unsigned(override_expected),"actual PFWorld94 store");
  dh2::navigation::MotionObject actor{0,0,~0u,~0u,{9,9,50},{}};float point[]{10,10,250};dh2::navigation::PositionResult result{};
  check(dh2_nav_validate_position(&result,&world->collision_world,&actor,point,&policy)==0,"actual ValidatePosition delivery");
  check(result.kind==(override_expected?2u:3u),"ValidatePosition reads SAME PFWorld94 and height gap");
  check(point[2]==(override_expected?0.f:50.f),"actual override acceptance versus source clamp");
 }
};
struct StageFixture {
 Geometry geometry;std::shared_ptr<model_renderer::SourceWorldBorrowV61> world=std::make_shared<model_renderer::SourceWorldBorrowV61>();
 std::shared_ptr<model_renderer::RendererSourceCandidateV55> candidate=std::make_shared<model_renderer::RendererSourceCandidateV55>();
 std::shared_ptr<dh2::loader::CanonicalLevelContextV1> level=std::make_shared<dh2::loader::CanonicalLevelContextV1>();
 dh2::loader::SourceLoadingInputsV43 loading;std::vector<std::string> order;int failure{};
 explicit StageFixture(int fail=0):failure(fail){
  using namespace model_renderer;world->owner=std::make_shared<int>(1);world->application=std::make_shared<TestApplication>();world->canonical_world=std::make_shared<TestCanonical>();
  world->canonical_world->manager_lease=std::shared_ptr<TestManager>(world->canonical_world,&world->canonical_world->manager);
  world->source_pf_floors_v115=geometry.world;world->gameplay_services_v66=std::make_shared<TestGameplay>();
  auto& services=*world->gameplay_services_v66;services.provider=std::make_shared<int>(2);services.actual_application=world->application;services.actual_object_manager=world->canonical_world->manager_lease;
  candidate->canonical=world->canonical_world;candidate->factory=std::make_shared<TestFactory>();candidate->preparation=std::make_shared<TestPreparation>(TestPreparation{level,&candidate->canonical->manager});candidate->rooms=std::make_shared<TestRooms>(TestRooms{geometry.world});
  services.update_objects=[this](float dt,std::string& e){check(dt==1.f,"source literal ObjectManager.Update dt");geometry.validate(true);order.push_back("objects");if(failure==1){e="objects failed";return false;}return true;};
  ai_callback=[this](const auto& owner,std::string& e){check(owner==world->owner,"SAME World AI receiver");geometry.validate(true);order.push_back("ai");if(failure==2){e="AI failed";return false;}return true;};
  services.physical_update=[this](std::string& e){geometry.validate(true);order.push_back("physical");if(failure==3){e="physical failed";return false;}return true;};
  services.camera_update=[this](std::string& e){geometry.validate(false);order.push_back("camera");if(failure==4){e="camera failed";return false;}return true;};
  std::string e;check(bind_source_stage18_stage32_v95(world,candidate,level,{},loading,e),"actual Stage32 composer binding");
 }
};
int main(){try{
 using Step=dh2::loader::LifecycleStepV36;
 for(int fail=0;fail<=4;++fail){StageFixture f(fail);std::string e;f.geometry.validate(false);
  const auto result=f.loading.external.stage_body[32](e);check(result==(fail?Step::failed:Step::complete),"actual warm-start result");
  check(f.level->fields.field130==32,"body preserves dispatcher-owned state130");check(f.candidate->factory->modules->reads==3,"three original config reads");
  check(f.world->canonical_world->scene_roots_v20->ambient[3]==1.f,"source ambient alpha");
  const std::vector<std::string> expected{"objects","ai","physical","camera"};check(f.order==std::vector<std::string>(expected.begin(),expected.begin()+(fail?fail:4)),"source ordered callback prefix");
  f.geometry.validate(fail>0&&fail<4);const auto calls=f.order.size();const auto again=f.loading.external.stage_body[32](e);
  check(again==(fail?Step::failed:Step::complete)&&f.order.size()==calls,"failed prefix latched and completed body not replayed");
 }
 {StageFixture f;std::string e;auto foreign=std::make_shared<dh2::floors::World>();f.world->source_pf_floors_v115=foreign;
  check(f.loading.external.stage_body[32](e)==Step::failed&&f.order.empty(),"foreign PFWorld receiver rejected before override");
  check(!foreign->source_motion_policy_v95.ignore_height_delta&&!f.geometry.world->source_motion_policy_v95.ignore_height_delta,"foreign failure mutates neither policy");}
 for(int failure:{0,1}){StageFixture f(failure);std::uint32_t progress=84,counter=9,current=7;unsigned publications{};
  f.loading.external.publish_progress=[&](std::int32_t state,std::int32_t value,std::string&){++publications;check(state==33&&value==86,"real dispatcher state/progress after complete body");return true;};
  dh2::loader::LifecycleV36 dispatcher({&progress,&f.level->fields.field130,&counter,&current},f.level,{},f.loading.external);
  auto result=dispatcher.tick();
  if(failure)check(result==dh2::loader::LifecycleStatusV36::failed&&f.level->fields.field130==32&&progress==84&&!publications,"failed native prefix cannot advance/publish progress");
  else check(result==dh2::loader::LifecycleStatusV36::loading&&f.level->fields.field130==33&&progress==86&&counter==7&&publications==1,"real source progress/counter tail");
 }
 {Geometry g;model_renderer::Record record;model_renderer::SourceCampaignCandidateBorrowV55 scope{g.world};std::string e;
  check(model_renderer::spot(record,scope,e),"shipping positive null-actor SpotTarget");check(record.actor->fields.spot14b0[2]==0.f&&record.actor->fields.previous_spot14bc[2]==0.f,"actual floor hit writes source spot Z");
  check(record.target_fx_manager_v70->rotations==1&&record.target_fx_manager_v70->syncs==1,"positive authored marker continuation");
  g.world->source_motion_policy_v95.maximum_height_delta=-1.f;check(!model_renderer::spot(record,scope,e),"spot consumes actual receiver policy rather than dummy");
  g.world->source_motion_policy_v95.maximum_height_delta=100.f;record.actor->fields.spot14b0[0]=200.f;check(model_renderer::spot(record,scope,e)&&record.actor->fields.spot14b0[0]==10.f,"floor miss restores previous source point");
 }
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"shipping_stage32_and_spot\":true,\"native_floor_queries\":true}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
