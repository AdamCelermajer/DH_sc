#include "../original_actor_physical.hpp"
#include "../original_actor_properties.hpp"
#include "../playable_actor_world.hpp"
#include "../modular_defaults.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace dh::foundation;
void check(bool x,const std::string&e){if(!x)throw std::runtime_error(e);}
struct Fixture {std::array<float,3> position{1090.75f,-212.202f,258},destination{};std::uintptr_t attached=0,visual=0;};
int main(int argc,char**argv){try{
 check(argc==2,"Actual original shared assets required");AssetCatalog assets(argv[1]);std::string error;const std::string tables="original-cache/data/pydata/";
 auto db=std::make_shared<OriginalPropertyDatabase>();check(load_original_property_tables(assets,tables,*db,error),error);auto ai=std::make_shared<dh2::data::AiTables>();check(load_original_ai_tables(assets,tables,*ai,error),error);dh2::data::PropertyRules rules;check(dh2::data::load_property_rules(db->characters,rules,error),error);
 auto world=std::make_shared<dh2::physical::NativeWorld>();const float limits[]{-1000,-1000,1000,1000};world->load(limits);
 unsigned actor_count=0;
 for(const std::string row:{"KnightPlayerBase","Swamp_LizadMan_Type1"}){
  auto state=std::make_shared<OriginalActorProperties>();check(resolve_original_actor_properties(db->characters,db->classes,row,{256,true},*state,error),error);auto view=dh2::data::property_view(rules,state->sheets);auto actor=std::make_shared<Fixture>();ActorBoundsConfig config;
  config.model_path=row=="KnightPlayerBase"?"models/prince_modular.bdae":"original-cache/data/3d/characters/lizardman/lizardman.bdae";
  if(row=="KnightPlayerBase"){std::vector<ModularDefaultCategory>categories;check(decode_modular_defaults(assets.read(config.model_path),categories,error),error);for(const auto&category:categories)for(const auto&id:category.available_controller_ids)if(id.find("_default_warrior-mesh-skin")!=std::string::npos)config.components.push_back({id,category.node_id});}
  OriginalActorBounds bounds;check(bounds.load(assets,config,error),error);ActorBoundsPlacement placement;placement.position={actor->position[0],actor->position[1],actor->position[2]};placement.base_scale={view.base[12],view.base[13],view.base[14]};placement.collision_scale=view.resolved[16];OriginalActorBoundsResult produced;check(bounds.calculate(placement,nullptr,produced,error),error);
  OriginalActorPhysicalBindings b;b.actor_lease=actor;b.world_lease=world;b.data_lease=db;b.identity=100+actor_count;b.position160=actor->position.data();b.destination1a8=actor->destination.data();b.attached2e0=&actor->attached;b.visual2d8=&actor->visual;b.world=world.get();b.properties=&view;b.ai=ai.get();std::vector<std::string> order;
  b.static84=[](std::uint8_t&out,std::string&){out=0;return true;}; // Explicit constructor/property fixture.
  b.is_player=[](std::int32_t type,bool&out,std::string&e){if(type==0){e="Archetype branch not supplied in this fixture";return false;}out=type==1;return true;};
  b.debug_switch=[&](const char*key,bool&out,std::string&){order.push_back(key);out=false;return true;};
  b.update_pf=[&](auto,const auto&native,const auto&,std::string&){check(native.body!=nullptr,"PF fixture reached before native creation");order.push_back("PF_FIXTURE");return true;};
  b.filter=[](void*,const auto&,const auto&,bool&,std::string&e){e="No contact filter fixture bound";return false;};b.contact=[](auto,void*,unsigned,std::string&e){e="No contact gameplay fixture bound";return false;};
  auto physical=std::make_shared<OriginalActorPhysical>(b);check(physical->bind_bounds(produced,error),error);check(physical->initialize(error),error);check(order==std::vector<std::string>({"MP_NoCollisions","MP_NoPhysics","PF_FIXTURE"}),"Original physical debug/PF order changed");check(physical->native().body&&physical->phase()==4,"Genuine source-defined native body absent");
  const auto&definition=physical->definition();const float expected=std::max(produced.absolute_box[3]-produced.absolute_box[0],produced.absolute_box[4]-produced.absolute_box[1])*.01f*.5f;check(std::abs(physical->native().radius-expected)<.00001f,"Body radius differs from exact original bounds source");std::cout<<row<<" AItype="<<ai->rows.at(view.resolved[1]).type<<" group="<<definition.shape.group_index<<" category="<<definition.shape.category_bits<<" mask="<<definition.shape.mask_bits<<" radius="<<physical->native().radius<<"\n";
  check(definition.shape.group_index==(row=="KnightPlayerBase"?-1:2),"Actual original player/lizard filter group differs");check(definition.shape.category_bits==(row=="KnightPlayerBase"?4u:16u),"Actual original player/lizard category differs");OriginalTriggerActorBorrow live;check(physical->actor_borrow(live,error),error);check(live.position160==actor->position.data()&&*live.physical2dc==reinterpret_cast<std::uintptr_t>(physical.get()),"Source body/position authority not shared");OriginalTriggerPhysicalBorrow body;check(physical->physical_borrow(*live.physical2dc,body,error)&&body.native==&physical->native(),error);
  check(physical->set_position({250,375,500},true,error),error);check(actor->position==std::array<float,3>{250,375,500}&&actor->destination==actor->position,"Source owner current/destination projection differs");dh2::physical::NativeBodyObservation observed{};check(dh2_native_body_observe(&observed,&physical->native())==0&&observed.position[0]==2.5f&&observed.position[1]==3.75f,"Actual native XY position not source-scaled");for(unsigned k=0;k<6;++k)check(live.absolute12c[k]==produced.relative_box[k]+actor->position[k%3],"Live source AABB translation differs");
  check(physical->release(error),error);check(!*live.physical2dc&&!physical->native().body,"Actual destruction did not detach source body");++actor_count;
 }
 std::cout<<"PASS originalRows="<<actor_count<<" genuineBox2D=true sourceBoundsRadius=true sourcePosition=true PF_DEBUG_FIXTURES=true noGameplayContactClaim=true\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}


