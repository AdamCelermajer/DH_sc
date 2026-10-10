#include "level_get_spawn_point_v1.hpp"
#include <canonical_point3d_globals_v1.hpp>
#include <cstdio>
#include <cstdlib>
#include <map>
using namespace dh2;
unsigned checks{},authored{};
void check(bool value,const char* message){++checks;if(!value){std::fprintf(stderr,"FAIL %u: %s\n",checks,message);std::exit(1);}}
struct Record {
 actor::RuntimeState runtime{};
 std::shared_ptr<world::CanonicalSpawnPointV15> spawn;
 std::shared_ptr<world::CanonicalDummyOwnerV14> dummy;
};
struct Fixture {
 std::shared_ptr<void> world=std::make_shared<int>(1);
 std::shared_ptr<world::CanonicalObjectManagerV1> objects=std::make_shared<world::CanonicalObjectManagerV1>(world::CanonicalObjectManagerServicesV1{});
 std::map<std::uintptr_t,std::shared_ptr<Record>> records;
 std::int32_t entrypoint{};unsigned loans{};bool live=true,foreign=false,unresolved=false;
 std::uintptr_t unresolved_identity{};
 std::string error;
 std::function<bool(const world::CanonicalObjectBorrowV1*,target_providers::Handle16&,std::string&)> handle;
 std::function<bool(const world::CanonicalObjectBorrowV1&,std::shared_ptr<world::CanonicalSpawnPointV15>&,std::string&)> loan;
 Fixture(){
  handle=[this](const auto* actor,auto& out,auto& e){
   if(unresolved||actor->identity==unresolved_identity){out={0,0,0};return true;}
   return objects->get_handle(actor->shared_handle->key,out,e);
  };
  // Same production adaptation: the existing typed loan accepts actor identity
  // and returns an alias of the already retained record, never another owner.
  loan=[this](const auto& actor,auto& out,auto& e){++loans;auto at=records.find(actor.identity);
   if(at==records.end()||!at->second->spawn){e="Unavailable actual typed loan";return false;}
   auto record=at->second;out=foreign?record->spawn:std::shared_ptr<world::CanonicalSpawnPointV15>(record,record->spawn.get());return true;
  };
 }
 std::shared_ptr<Record> add_spawn(std::map<std::string,std::string> attrs){
  auto record=std::make_shared<Record>();record->spawn=std::make_shared<world::CanonicalSpawnPointV15>(world,record->runtime,world::GameObjectInitializationServicesV1{},world::SpawnPointServicesV15{});
  auto& owner=*record->spawn;owner.base().class_name20()="SpawnPoint";
  auto actor=owner.properties();world::CanonicalPropertyMapV1 properties({nullptr,&world::canonical_vec3_origin_v1(),[](void*,std::string&){return true;}});
  check(properties.init_properties(actor,error)&&properties.load_defaults(actor,error),"actual SpawnPoint declaration/default loading");
  world::CanonicalSourceObjectRequestV1 request;request.source_lease=record;request.source_context=&attrs;
  request.attribute=[](void* raw,std::uint32_t,const char* key)->const char*{const auto& attrs=*static_cast<std::map<std::string,std::string>*>(raw);auto at=attrs.find(key);return at==attrs.end()?nullptr:at->second.c_str();};
  check(properties.load_overrides(actor,request,error),"actual authored SpawnPoint overrides");
  target_providers::Handle16 published;
  check(objects->add(owner.canonical(record),attrs["name"].c_str(),"SpawnPoint",0,false,published,error),"actual type13 map publication");
  records.emplace(owner.base().identity(),record);return record;
 }
 std::shared_ptr<Record> add_spawn(const char* name,std::int32_t entry,std::uint8_t visible=1){
  auto record=add_spawn({{"name",name},{"gametype","SpawnPoint"},{"entrypointID",std::to_string(entry)},{"position","1,2,3"}});
  check(record->spawn->write_bool(0x8a,visible,error),"same actual visible8a store");return record;
 }
 void add_dummy(){
  auto record=std::make_shared<Record>();record->dummy=std::make_shared<world::CanonicalDummyOwnerV14>(world,record->runtime,world::GameObjectInitializationServicesV1{},world::DummyContinuationServicesV14{});
  target_providers::Handle16 published;check(objects->add(record->dummy->canonical(record),"dummy","Dummy",0,false,published,error),"actual non-type13 map publication");records.emplace(record->dummy->base().identity(),record);
 }
 void add_null(){target_providers::Handle16 missing{999,UINT32_MAX,0};const world::CanonicalObjectBorrowV1* resolved{};check(objects->resolve_handle_v4(missing,false,resolved,{},error)&&!resolved,"source operator[] null map node");}
 bool select(std::shared_ptr<world::CanonicalSpawnPointV15>& out){return loader::source_get_spawn_point_v1(objects,entrypoint,handle,loan,[this](std::string& e){if(!live)e="Retired source phase";return live;},out,error);}
};
void run(std::map<std::string,std::string>& attrs){
 if(attrs["gametype"]!="SpawnPoint")return;
 Fixture f;auto record=f.add_spawn(attrs);f.entrypoint=record->spawn->entrypoint();std::shared_ptr<world::CanonicalSpawnPointV15> out;
 check(f.select(out)&&out.get()==record->spawn.get()&&f.loans==1,"authored type13 traverses with existing typed loan");
 check(!out.owner_before(record)&&!record.owner_before(out),"selected authored point retains SAME record control block");
 check(out->base().vector3(0x160)==record->runtime.subobjects.position,"selected point160 is actual authored mutable storage");++authored;
}
#ifdef DH2_STAGE34_COMPOSITION_EXCERPTS
// The runner extracts the CURRENT production callbacks into a temporary include.
// Only World scope is a fixture; the selected typed actors/manager remain real.
void production_forwarding(){
 struct Bag {struct Checkpoint {std::function<bool(const world::CanonicalObjectBorrowV1&,std::shared_ptr<world::CanonicalSpawnPointV15>&,std::string&)> spawn_point;} checkpoint;};
 struct SourceWorldBorrowV61 {std::shared_ptr<Bag> stage34_native_v80;};
 struct Composition {std::function<bool(std::uintptr_t,std::shared_ptr<world::CanonicalSpawnPointV15>&,std::string&)> borrow_spawn_point_v80;};
 Fixture f;auto record=f.add_spawn("production-default",0);
 auto source_world=std::make_shared<SourceWorldBorrowV61>();source_world->stage34_native_v80=std::make_shared<Bag>();
 Bag native34;auto scope_world=[&](auto& out,std::string& e){if(!f.live){e="Retired actual production scope";return false;}out=source_world;return true;};
 Composition noncharacters;noncharacters.borrow_spawn_point_v80=[&](auto id,auto& out,auto& e){auto at=f.records.find(id);if(at==f.records.end())return false;auto* actor=f.objects->object(at->second->spawn->base().shared_handle().key);return actor&&f.loan(*actor,out,e);};
 auto& e=f.error;
#include "production_stage34_spawn.inc"
}
#endif
int main(){
#ifdef DH2_STAGE34_COMPOSITION_EXCERPTS
 production_forwarding();
#endif
#include "../../level-world/tests/canonical_swamp_family_declarations_v15.inc"
 check(authored>0,"real authored SpawnPoint declarations exercised");
 {Fixture f;std::shared_ptr<world::CanonicalSpawnPointV15> out;check(f.select(out)&&!out&&f.loans==0,"empty/reserved-null map returns NULL without loan");}
 for(const auto mode:{0,1,2}){Fixture f;auto point=f.add_spawn("single",mode==2?7:0,mode==1?0:1);std::shared_ptr<world::CanonicalSpawnPointV15> out;
  check(f.select(out)&&out.get()==point->spawn.get(),"matching/invisible/wrong-entrypoint source R0 retained");}
 {Fixture f;f.add_dummy();std::shared_ptr<world::CanonicalSpawnPointV15> out;check(f.select(out)&&!out&&f.loans==0,"non-SpawnPoint returns NULL without typed loan");}
 {Fixture f;auto point=f.add_spawn("rejected",7);f.add_null();std::shared_ptr<world::CanonicalSpawnPointV15> out;check(f.select(out)&&out.get()==point->spawn.get(),"null successor retains residual rejected SpawnPoint");}
 {Fixture f;f.add_spawn("rejected",7);f.add_dummy();std::shared_ptr<world::CanonicalSpawnPointV15> out;check(f.select(out)&&!out,"non-type13 successor clears residual SpawnPoint");}
 {Fixture f;f.add_spawn("rejected",7);auto point=f.add_spawn("matching",0);f.add_dummy();std::shared_ptr<world::CanonicalSpawnPointV15> out;check(f.select(out)&&out.get()==point->spawn.get()&&f.loans==2,"first accepted point stops before later non-type13");}
 {Fixture f;f.add_spawn("first",7);auto last=f.add_spawn("last",8);std::shared_ptr<world::CanonicalSpawnPointV15> out;check(f.select(out)&&out.get()==last->spawn.get(),"last rejected type13 becomes source accumulator");}
 {Fixture f;f.add_spawn("unresolved",0);f.unresolved=true;std::shared_ptr<world::CanonicalSpawnPointV15> out;check(f.select(out)&&!out&&f.loans==0,"unresolved handle clears source accumulator");}
 {Fixture f;f.add_spawn("rejected",7);auto last=f.add_spawn("unresolved",8);f.unresolved_identity=last->spawn->base().identity();std::shared_ptr<world::CanonicalSpawnPointV15> out;check(f.select(out)&&!out&&f.loans==1,"unresolved successor clears residual rejected point");}
 {Fixture f;auto point=f.add_spawn("fresh-entrypoint",7);auto actual=f.loan;f.loan=[&](const auto& actor,auto& out,auto& e){f.entrypoint=7;return actual(actor,out,e);};f.add_dummy();std::shared_ptr<world::CanonicalSpawnPointV15> out;check(f.select(out)&&out.get()==point->spawn.get(),"Level110 is reread after typed loan callbacks");}
 {Fixture f;f.add_spawn("missing",0);f.loan={};std::shared_ptr<world::CanonicalSpawnPointV15> out;check(!f.select(out),"required typed loan absence fails");}
 {Fixture f;f.add_spawn("foreign",0);f.foreign=true;std::shared_ptr<world::CanonicalSpawnPointV15> out;check(!f.select(out)&&f.error=="Required SAME mapped SpawnPoint receiver/lease","same address with foreign control block rejected");}
 {Fixture f;f.add_spawn("retired",0);f.live=false;std::shared_ptr<world::CanonicalSpawnPointV15> out;check(!f.select(out)&&f.loans==0,"phase retirement rejects before typed loan");}
 std::printf("Stage34 GetSpawnPoint PASS: %u authored declarations, %u checks\n",authored,checks);
}
