#include "playable_actor_bodies.hpp"
#include "original_actor_properties.hpp"
#include "playable_actor_world.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool ok,const std::string& why){if(!ok)throw std::runtime_error(why);}
struct Fields {ActorState actor;std::array<float,3> destination{};std::uintptr_t attached=0,visual=0;};
int main(int argc,char**argv){try{
 check(argc==2,"Original shared assets root required");AssetCatalog assets(argv[1]);std::string error;
 auto database=std::make_shared<OriginalPropertyDatabase>();
 check(load_original_property_tables(assets,"original-cache/data/pydata/",*database,error),error);
 auto ai=std::make_shared<dh2::data::AiTables>();
 check(load_original_ai_tables(assets,"original-cache/data/pydata/",*ai,error),error);
 auto world=std::make_shared<dh2::physical::NativeWorld>();const float bounds[]{-1000,-1000,1000,1000};world->load(bounds);
 PlayableActorBodies pool;std::array<std::shared_ptr<Fields>,2> fields;
 std::array<std::shared_ptr<OriginalCombatProperties>,2> properties;
 bool collision_filter_probe=false,delivery_mutation_rejected=false;
 const std::array<std::string,2> rows{"KnightPlayerBase","Swamp_LizadMan_Type1"};
 for(unsigned i=0;i<rows.size();++i){
  fields[i]=std::make_shared<Fields>();auto& actor=fields[i]->actor;actor.id=100+i;
  actor.transform.position={100.f+static_cast<float>(i)*100.f,100.f,0};fields[i]->destination=actor.transform.position;
  actor.health=73.f+i;actor.target_id=900+i;actor.action=CharacterAction::attacking;
  OriginalActorProperties resolved;check(resolve_original_actor_properties(database->characters,database->classes,rows[i],{256,true},resolved,error),error);
  properties[i]=std::make_shared<OriginalCombatProperties>();properties[i]->sheets=std::move(resolved.sheets);
  OriginalActorBodyPlanInput input;input.properties=&properties[i]->sheets;input.ai=ai.get();input.owner_identity=actor.id;
  input.position={actor.transform.position[0],actor.transform.position[1],actor.transform.position[2]};input.source_name=rows[i];
  input.visual.model_path=i==0?"models/prince_modular.bdae":"original-cache/data/3d/characters/lizardman/lizardman.bdae";
  input.visual.use_authored_modular_defaults=i==0;OriginalActorBodyPlan plan;
  check(make_original_actor_body_plan(assets,input,plan,error),error);
  OriginalActorPhysicalBindings binding;binding.actor_lease=fields[i];binding.world_lease=world;binding.data_lease=properties[i];
  binding.world=world.get();binding.ai=ai.get();binding.destination1a8=fields[i]->destination.data();
  binding.attached2e0=&fields[i]->attached;binding.visual2d8=&fields[i]->visual;
  binding.static84=[](auto& out,auto&){out=0;return true;};
  binding.is_player=[row=rows[i]](auto type,auto& out,auto&){out=original_actor_source_is_player(type,row);return true;};
  binding.debug_switch=[](const char*,bool& out,std::string&){out=false;return true;};
  binding.filter=[&](void*,const auto&,const auto&,bool& allowed,auto& e){
   if(collision_filter_probe){allowed=false;delivery_mutation_rejected=
    !pool.set_source_physical_filter(100,[&](ActorId id)->ActorState*{
      return id==fields[0]->actor.id?&fields[0]->actor:nullptr;},0,0x51c,3,false,e);return true;}
   allowed=true;e.clear();return true;
  };
  binding.contact=[](auto,void*,unsigned,auto& e){e="Reached gameplay contact delivery is outside this source-filter test";return false;};
  check(pool.bind(actor,*properties[i],plan,std::move(binding),error),error);
 }
 auto lookup=[&](ActorId id)->ActorState*{for(const auto& item:fields)if(item->actor.id==id)return &item->actor;return nullptr;};
 for(unsigned i=0;i<fields.size();++i){
  const ActorId id=fields[i]->actor.id;auto* native=pool.physical(id)->native().body;check(native,"Genuine same-world NativeWorld body absent");
  auto* shape=native->GetShapeList();check(shape,"Source primary shape absent");const auto saved=shape->GetFilterData();
  const auto health=fields[i]->actor.health;const auto target=fields[i]->actor.target_id;const auto action=fields[i]->actor.action;
  const auto position=fields[i]->actor.transform.position;const auto destination=fields[i]->destination;
  const auto mass=native->GetMass();const auto pinned=pool.physical(id)->native().pinned;
  dh2::physical::NativeBodyObservation before{};check(!dh2_native_body_observe(&before,&pool.physical(id)->native()),"Native body observation failed before filter operation");
  auto set=[&](bool secondary){return pool.set_source_physical_filter(id,lookup,0,0x51c,3,secondary,error);};
  check(set(false),error);auto dead=shape->GetFilterData();
  check(dead.groupIndex==0&&dead.categoryBits==0x51c&&dead.maskBits==3,"Source setFilter primary words/order differ");
  check(fields[i]->actor.health==health&&fields[i]->actor.target_id==target&&fields[i]->actor.action==action,"Source filter changed HP/target/action");
  check(set(false),error);check(shape->GetFilterData().categoryBits==0x51c,"Repeated source setFilter was not applied");
  check(pool.reset_source_physical_filter(id,lookup,error),error);auto restored=shape->GetFilterData();
  check(restored.groupIndex==saved.groupIndex&&restored.categoryBits==saved.categoryBits&&restored.maskBits==saved.maskBits,"Source resetFilter did not restore exact saved primary data");
  check(pool.reset_source_physical_filter(id,lookup,error),error);
  check(set(true),error);check(shape->GetFilterData().categoryBits==0x51c,"applySecondary=true did not apply primary source filter");
  check(pool.reset_source_physical_filter(id,lookup,error),error);
  check(pool.set_physical_filter_enabled(id,false,error),error);auto zero=shape->GetFilterData();
  check(zero.groupIndex==0&&zero.categoryBits==0&&zero.maskBits==0,"Existing disableFilter no longer zeroes the shape");
  check(set(false),error);check(pool.set_physical_filter_enabled(id,true,error),error);
  check(shape->GetFilterData().categoryBits==0x51c&&shape->GetFilterData().maskBits==3,
        "setFilter did not clear the saved disabled byte before enableFilter");
  check(pool.reset_source_physical_filter(id,lookup,error),error);
  check(pool.set_physical_filter_enabled(id,false,error),error);check(pool.set_physical_filter_enabled(id,true,error),error);
  restored=shape->GetFilterData();check(restored.groupIndex==saved.groupIndex&&restored.categoryBits==saved.categoryBits&&restored.maskBits==saved.maskBits,
        "Existing disableFilter/enableFilter interaction lost the original saved filter");
  dh2::physical::NativeBodyObservation after{};check(!dh2_native_body_observe(&after,&pool.physical(id)->native()),"Native body observation failed after filter operation");
  check(fields[i]->actor.transform.position==position&&fields[i]->destination==destination&&
        before.position[0]==after.position[0]&&before.position[1]==after.position[1]&&
        native->GetMass()==mass&&pool.physical(id)->native().pinned==pinned,
        "Source filter operation changed position/destination/mass/pin");
 }
 ActorState wrong;wrong.id=100;check(!pool.set_source_physical_filter(100,[&](ActorId){return &wrong;},0,0x51c,3,false,error),"Stale current-owner lookup was accepted");
 check(!pool.reset_source_physical_filter(999,lookup,error),"Unknown reset owner was accepted");
 collision_filter_probe=true;check(!world->ShouldCollide(pool.physical(100)->native().body->GetShapeList(),
  pool.physical(101)->native().body->GetShapeList()),"Explicit source collision filter did not reject the fixture pair");
 collision_filter_probe=false;check(delivery_mutation_rejected,"Filter mutation was accepted during NativeWorld filter delivery");
 check(pool.remove_physical(100,error),error);check(!pool.set_source_physical_filter(100,lookup,0,0x51c,3,false,error),"Absent physical receiver accepted source SetFilter");
 check(!pool.reset_source_physical_filter(100,lookup,error),"Absent physical receiver accepted source resetFilter");
 std::string cleanup;check(pool.clear(cleanup),cleanup);
 std::cout<<"PASS isolated same-world Knight/Lizard ActorState bodies; repeat, disable/enable interaction, stale/missing owners, callback-delivery and absent-body rejection; filter ops preserve HP/target/action/transform/destination/mass/pin; secondary shape absent in Character owner\n";
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
