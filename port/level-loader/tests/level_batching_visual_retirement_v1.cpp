#include "../level_batching_source_v96.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh2::loader;
namespace {
unsigned checks{};
void check(bool value,const char* message){++checks;if(!value)throw std::runtime_error(message);}
struct Node {std::string name;bool visible{true};std::vector<std::shared_ptr<Node>> children;};
struct Object {
 std::string type,name;std::uint8_t deleted{},linked{};std::uintptr_t visual{1};
 std::shared_ptr<Node> root;
};
struct Mesh {unsigned references{};std::vector<std::uintptr_t> segments;};
struct Fixture {
 std::shared_ptr<int> provider=std::make_shared<int>(0);
 std::vector<std::shared_ptr<Object>> objects;
 std::shared_ptr<Mesh> mesh=std::make_shared<Mesh>();
 std::shared_ptr<Node> batch=std::make_shared<Node>(Node{"compiled-root",true,{}});
 std::vector<std::string> operations,retired;
 std::uintptr_t rendered{};std::uint32_t word138{};bool fail_retirement{};
 std::string error;
 Fixture(const std::vector<std::string>& types,bool nobatch=false,bool absent=false){
  for(const auto& type:types){auto object=std::make_shared<Object>();object->type=type;object->name=type+"_fixture";
   object->visual=absent?0:reinterpret_cast<std::uintptr_t>(object.get());
   object->root=std::make_shared<Node>(Node{object->name,true,{}});
   if(nobatch)object->root->children.push_back(std::make_shared<Node>(Node{"part_nobatch",true,{}}));
   objects.push_back(std::move(object));
  }
 }
 BatchNodeBorrowV96 node(const std::shared_ptr<Node>& actual){
  BatchNodeBorrowV96 result;result.owner=actual;result.identity=reinterpret_cast<std::uintptr_t>(actual.get());result.name24=&actual->name;
  result.child_count=[actual](std::size_t& count,std::string&){count=actual->children.size();return true;};
  result.child=[this,actual](std::size_t index,BatchNodeBorrowV96& out,std::string&){out=node(actual->children.at(index));return true;};
  result.set_visible48=[this,actual](bool visible,std::string&){actual->visible=visible;operations.push_back(actual->name+(visible?":show":":hide"));return true;};
  return result;
 }
 BatchObjectBorrowV96 object(const std::shared_ptr<Object>& actual){
  return {actual,reinterpret_cast<std::uintptr_t>(actual.get()),&actual->deleted,&actual->name,&actual->type,&actual->linked,&actual->visual};
 }
 BatchNativeServicesV96 services(bool retirement_provider=true){
  BatchNativeServicesV96 result;result.provider=provider;result.validate_current=[](std::string&){return true;};
  result.trace=[](const char*,std::string&){return true;};result.is_faerie=[](std::uintptr_t,bool& out,std::string&){out=false;return true;};
  result.object=[this](std::uintptr_t id,BatchObjectBorrowV96& out,std::string&){for(const auto& actual:objects)if(reinterpret_cast<std::uintptr_t>(actual.get())==id){out=object(actual);return true;}return false;};
  result.visual_root=[this](const BatchObjectBorrowV96& actual,BatchNodeBorrowV96& out,std::string&){for(const auto& owner:objects)if(reinterpret_cast<std::uintptr_t>(owner.get())==actual.identity){out=node(owner->root);return true;}return false;};
  result.construct_mesh=[this](std::uint32_t attributes,BatchMeshBorrowV96& out,std::string&){
   check(attributes==0x30,"Original mesh attribute mask");out.owner=mesh;out.identity=reinterpret_cast<std::uintptr_t>(mesh.get());
   out.grab=[this](std::string&){++mesh->references;return true;};out.drop=[this](std::string&){check(mesh->references!=0,"Real fixture mesh reference");--mesh->references;return true;};return true;
  };
  result.construct_root=[this](std::int32_t id,const BatchMeshBorrowV96& actual,BatchRootBorrowV96& out,std::string&){
   check(id==-1&&actual.owner==mesh,"Original root ID and SAME supplied mesh");++mesh->references;
   out.owner=batch;out.identity=reinterpret_cast<std::uintptr_t>(batch.get());out.mesh130=actual.identity;out.node=node(batch);out.source_word138=&word138;
   out.set_automatic_culling=[](std::uint32_t value,std::string&){check(value==0,"Original culling argument");return true;};
   out.drop=[this](std::string&){check(mesh->references==1,"Compiler root releases final fixture mesh reference");--mesh->references;operations.push_back("root-drop");return true;};return true;
  };
  result.scene_add_child5c=[this](const BatchNodeBorrowV96& actual,std::string&){operations.push_back("attach:"+*actual.name24);return true;};
  result.scene_compile50=[this](const auto& roots,BatchRootBorrowV96& actual,bool option,const BatchLinkedCallbackV96& callback,std::nullptr_t,const std::array<float,3>& point,std::string& e){
   check(!option&&point==std::array<float,3>{0,0,0}&&actual.owner==batch,"Original compile arguments and root");operations.push_back("compile");
   for(const auto& root:roots){rendered=root.identity;mesh->segments.push_back(0);if(!callback(actual.mesh130,mesh->segments.size()-1,e))return false;}
   return true;
  };
  result.current_rendered_node9c=[this](std::uintptr_t& out,std::string&){out=rendered;return true;};
  result.segment=[this](std::uintptr_t id,std::uintptr_t index,BatchSegmentBorrowV96& out,std::string&){check(id==reinterpret_cast<std::uintptr_t>(mesh.get()),"SAME linked segment mesh");out={mesh,&mesh->segments.at(index)};return true;};
  result.flush_mesh_buffers=[this](std::uintptr_t id,bool first,bool second,bool third,std::string&){check(id==reinterpret_cast<std::uintptr_t>(mesh.get())&&first&&!second&&!third,"Original flush arguments");operations.push_back("flush");return true;};
  if(retirement_provider)result.set_visual_null=[this](const BatchObjectBorrowV96& actual,std::string& e){
   operations.push_back("retire:"+*actual.archetype48);retired.push_back(*actual.archetype48);
   for(const auto& owner:objects)if(reinterpret_cast<std::uintptr_t>(owner.get())==actual.identity)owner->visual=0;
   if(fail_retirement){e="injected original visual release failure";return false;}return true;
  };
  return result;
 }
 void enroll(BatchNodeCompilerSourceV96& compiler,const BatchNativeServicesV96& services){
  for(const auto& actual:objects){bool eligible{};check(original_batch_eligible_v96(object(actual),services,eligible,error)&&eligible,"Source-selected family must reach compiler");check(compiler.append_object(object(actual),error),"Append selected SAME object");}
  check(compiler.build_map(error),"Build original node-to-object map");
 }
};
const std::vector<std::string> families{"Character","Door","TimerTrap","TriggerTrap","AnimatedDecor","Decor","Module","OpenableContainer","DestructibleContainer"};
// Original ARM 0x50df94..0x50dfbc compares only Decor and Module. Both BEQ
// branches enter SetVisualObject(NULL) at 0x50df54..0x50df5c; all other
// archetypes proceed to the next object without that call.
bool original_retires(const std::string& type){return type=="Decor"||type=="Module";}
void success_case(bool nobatch,bool absent){
 Fixture fixture(families,nobatch,absent);auto services=fixture.services();BatchNodeCompilerSourceV96 compiler(services);fixture.enroll(compiler,services);
 check(compiler.compile(false,fixture.error),"Compile selected family call set");
 check(compiler.compiled0()==!absent,"Empty visual list never compiles");
 std::vector<std::string> expected;
 for(const auto& actual:fixture.objects){const bool retire=!nobatch&&!absent&&original_retires(actual->type);if(retire)expected.push_back(actual->type);
  check(actual->visual==(absent||retire?0:reinterpret_cast<std::uintptr_t>(actual.get())),"Original family visual survives or is retired exactly");
  check(actual->linked==(!absent?1:0),"Linked GameObject flag belongs to SAME selected object");
  for(const auto& child:actual->root->children)check(child->visible,"nobatch visibility restored before retirement selection");
 }
 check(fixture.retired==expected,"Only original Decor and Module retirement callbacks, in source order");
 if(!expected.empty()){auto flush=std::find(fixture.operations.begin(),fixture.operations.end(),"flush");auto retire=std::find(fixture.operations.begin(),fixture.operations.end(),"retire:Decor");check(flush<retire,"Retire only after original compile/flush");}
 check(compiler.source_objects10().empty()&&compiler.source_node_map1c().empty(),"Successful source Trim");
 check(compiler.finish_scene_attachment(fixture.error)&&fixture.batch->visible,"Original compiled root attach/visible suffix");
 check(fixture.mesh->references==1&&fixture.word138==2,"Root retains SAME fixture mesh after temporary drop");
 check(compiler.destroy_source(fixture.error)&&fixture.mesh->references==0,"Explicit compiler D1 releases retained fixture resource");
}
}
int main(){try{
 success_case(false,false);success_case(true,false);success_case(false,true);
 // Preserved families do not require an unreachable visual-release provider.
 {Fixture fixture({"Character","Door","TimerTrap","TriggerTrap","AnimatedDecor","OpenableContainer","DestructibleContainer"});auto services=fixture.services(false);BatchNodeCompilerSourceV96 compiler(services);fixture.enroll(compiler,services);
  check(compiler.compile(false,fixture.error)&&fixture.retired.empty(),"Preserved source families do not dispatch missing retirement provider");check(compiler.destroy_source(fixture.error),"Preserved-family fixture D1");}
 // A genuine reached Decor release failure stops before Module and cannot
 // replay the reached callback or discard its compiler/resource receipt.
 {Fixture fixture(families);fixture.fail_retirement=true;auto services=fixture.services();BatchNodeCompilerSourceV96 compiler(services);fixture.enroll(compiler,services);
  check(!compiler.compile(false,fixture.error)&&fixture.retired==std::vector<std::string>{"Decor"},"Only reached original Decor retirement before failure");
  const auto operations=fixture.operations;check(!compiler.compile(false,fixture.error)&&operations==fixture.operations,"Failed retirement callback never replays");
  check(!compiler.source_objects10().empty()&&fixture.mesh->references==1,"Failed source prefix retains selection and resource ownership");check(compiler.destroy_source(fixture.error),"Failed retirement still drains original compiler D1");}
 std::cout<<"PASS "<<checks<<" Stage24 original-family visual/linked/order/lifetime checks\n";
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
