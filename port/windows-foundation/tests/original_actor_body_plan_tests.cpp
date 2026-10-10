#include "../original_actor_body_plan.hpp"
#include "../original_actor_properties.hpp"
#include "../playable_actor_world.hpp"
#include "../modular_defaults.hpp"
#include "../../level-world/character_body_config.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace dh::foundation;
void check(bool x,const std::string&e){if(!x)throw std::runtime_error(e);}
int main(int argc,char**argv){try{
 check(argc==2,"Actual original shared assets required");AssetCatalog assets(argv[1]);std::string error;OriginalPropertyDatabase db;dh2::data::AiTables ai;const std::string root="original-cache/data/pydata/";check(load_original_property_tables(assets,root,db,error)&&load_original_ai_tables(assets,root,ai,error),error);
 for(const std::string row:{"KnightPlayerBase","Swamp_LizadMan_Type1"}){
  OriginalActorProperties properties;check(resolve_original_actor_properties(db.characters,db.classes,row,{256,true},properties,error),error);OriginalActorBodyPlanInput input;input.properties=&properties.sheets;input.ai=&ai;input.owner_identity=100;input.source_name="source-npc";input.position={-3725.75f,252.509f,250};input.rotation_degrees={0,0,89.6209f};
  input.visual.model_path=row=="KnightPlayerBase"?"models/prince_modular.bdae":"original-cache/data/3d/characters/lizardman/lizardman.bdae";
  if(row=="KnightPlayerBase"){std::vector<ModularDefaultCategory>categories;check(decode_modular_defaults(assets.read(input.visual.model_path),categories,error),error);for(const auto&category:categories)for(const auto&id:category.available_controller_ids)if(id.find("_default_warrior-mesh-skin")!=std::string::npos)input.visual.controller_ids.push_back(id);input.visual.expected_controller_count=4;}
  OriginalActorBodyPlan plan;check(make_original_actor_body_plan(assets,input,plan,error),error);check(plan.physical_enabled&&plan.circular&&plan.radius_game_units>0,"Actual source circular body plan absent");check(plan.is_player==(row=="KnightPlayerBase"),"Actual source IsPlayer branch differs");
  OriginalActorBounds source;check(source.load(assets,plan.components,error),error);ActorBoundsPlacement placement;placement.position=input.position;placement.rotation_degrees=input.rotation_degrees;placement.base_scale={properties.sheets.base[12],properties.sheets.base[13],properties.sheets.base[14]};placement.collision_scale=properties.sheets.resolved[16];OriginalActorBoundsResult expected;check(source.calculate(placement,nullptr,expected,error),error);check(plan.bounds.relative_box==expected.relative_box&&plan.bounds.absolute_box==expected.absolute_box,"Source rotated rest box differs");
  dh2::physical::CharacterBodyInput body{};body.owner=reinterpret_cast<void*>(100);body.new_physical=&body;body.character_type=plan.character_type;body.is_player=plan.is_player;body.absolute_bounds[0]=expected.absolute_box[0];body.absolute_bounds[1]=expected.absolute_box[1];body.absolute_bounds[2]=expected.absolute_box[3];body.absolute_bounds[3]=expected.absolute_box[4];dh2::physical::CharacterBodyConfig definition{};check(dh2_character_body_config(&definition,&body)==0&&plan.radius_game_units==definition.radius*100.f,"Exact original bodydefinition radius differs");
  const auto saved=plan;input.visual.controller_ids.push_back("unavailable-original-controller");check(!make_original_actor_body_plan(assets,input,plan,error)&&plan.bounds.absolute_box==saved.bounds.absolute_box,"Unknown explicit ID silently dropped or failure changed plan");input.visual.controller_ids.pop_back();input.visual.expected_controller_count=99;check(!make_original_actor_body_plan(assets,input,plan,error),"Mismatched explicit controller count accepted");input.visual.expected_controller_count=row=="KnightPlayerBase"?4:0;input.static84=1;check(make_original_actor_body_plan(assets,input,plan,error)&&plan.physical_enabled&&!plan.circular&&plan.radius_game_units==0,"Special original polygon body pretended circular radius");
  std::cout<<row<<" sourceRestRadius="<<saved.radius_game_units<<" authoredRotation="<<input.rotation_degrees.z<<" selectedControllers="<<saved.selected_controller_count<<'\n';
 }
 check(original_actor_source_is_player(0,"PlayerCharacterOriginal")&&!original_actor_source_is_player(0,"xPlayerCharacter")&&!original_actor_source_is_player(0,"playerCharacter")&&original_actor_source_is_player(1,"")&&!original_actor_source_is_player(4,"PlayerCharacter"),"Original type0 exact name-prefix semantics differ");
 std::cout<<"PASS originalPlayerNPC=true originalRotation=true exactBodyRadius=true selectionFailClosed=true noReturnedPhysicalPointers=true restCacheOnly=true\n";
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
