#include "original_actor_body_plan.hpp"
#include "modular_defaults.hpp"
#include "../engine-skinning/skinning.hpp"
#include "../level-world/character_body_config.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <stdexcept>
namespace dh::foundation {
bool original_actor_source_is_player(std::int32_t type,const std::string&name) noexcept{return type?type==1:name.compare(0,15,"PlayerCharacter")==0;}
bool make_original_actor_body_plan(const AssetCatalog&assets,const OriginalActorBodyPlanInput&input,OriginalActorBodyPlan&out,std::string&e){try{
 if(!input.properties||!input.ai||input.ai->rows.size()<=8||!input.owner_identity||input.visual.model_path.empty())throw std::runtime_error("Original body plan requires actual properties/AI/owner/model");
 const auto&v=input.visual;if(!v.skin_id_contains.empty()&&(v.use_authored_modular_defaults||!v.controller_ids.empty()))throw std::runtime_error("Conflicting original body controller selection policies");
 const auto bytes=assets.read(v.model_path);dh2::resources::BresView view{};if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)throw std::runtime_error("Original body plan BRES rejected");dh2::scene::Scene scene;if(!dh2::scene::load(view,scene,e))return false;
 std::vector<ModularDefaultCategory>categories;if(!decode_modular_defaults(bytes,categories,e))return false;
 if(!categories.empty()&&v.controller_ids.empty()&&v.skin_id_contains.empty()&&!v.use_authored_modular_defaults)throw std::runtime_error("Modular source body requires explicit selection or authored defaults");
 std::map<std::string,unsigned> ids;std::map<std::string,std::set<std::string>> groups;std::set<std::string> modular,defaults,visible;
 for(unsigned c=0;c<dh2_bres_library_count(&view,dh2::resources::Library::controller);++c){dh2::skinning::Skin skin;if(!dh2::skinning::load(view,c,scene,skin,e))return false;if(skin.id.empty()||!ids.emplace(skin.id,c).second)throw std::runtime_error("Duplicate/empty original body controller ID");}
 for(const auto&category:categories){if(!category.controller_id.empty())defaults.insert(category.controller_id);for(const auto&id:category.available_controller_ids){modular.insert(id);groups[id].insert(category.node_id);}}
 for(const auto&instance:scene.instances)if(instance.controller>=0){auto i=std::find_if(ids.begin(),ids.end(),[&](const auto&entry){return entry.second==unsigned(instance.controller);});if(i==ids.end())throw std::runtime_error("Visible source instance controller unavailable");visible.insert(i->first);if(!modular.count(i->first))groups[i->first].insert(instance.node);}
 std::set<std::string> selected;
 if(!v.controller_ids.empty()){for(const auto&id:v.controller_ids){if(id.empty()||!selected.insert(id).second)throw std::runtime_error("Duplicate/empty explicit body controller ID");if(!ids.count(id))throw std::runtime_error("Explicit body controller unavailable: "+id);}}
 else for(const auto&entry:ids){const auto&id=entry.first;if(!v.skin_id_contains.empty()){if(id.find(v.skin_id_contains)!=std::string::npos)selected.insert(id);}else if(v.use_authored_modular_defaults){if(defaults.count(id)||(!modular.count(id)&&visible.count(id)))selected.insert(id);}else selected.insert(id);}
 if((!ids.empty()&&selected.empty())||(!v.skin_id_contains.empty()&&selected.empty()))throw std::runtime_error("Original body controller selection is empty");
 if(v.expected_controller_count&&selected.size()!=v.expected_controller_count)throw std::runtime_error("Original body controller count differs from actual selection");
 OriginalActorBodyPlan plan;plan.components.model_path=v.model_path;plan.selected_controller_count=static_cast<unsigned>(selected.size());
 for(const auto&id:selected){if(groups[id].empty())throw std::runtime_error("Selected original body controller has no source group/instance: "+id);for(const auto&node:groups[id])plan.components.components.push_back({id,node});}
 OriginalActorBounds bounds;if(!bounds.load(assets,plan.components,e))return false;ActorBoundsPlacement placement;placement.position=input.position;placement.rotation_degrees=input.rotation_degrees;placement.base_scale={input.properties->base[12],input.properties->base[13],input.properties->base[14]};placement.collision_scale=input.properties->resolved[16];placement.previous_flat=input.previous_flat;if(!bounds.calculate(placement,nullptr,plan.bounds,e))return false;
 plan.ai_id=input.properties->resolved[1];if(plan.ai_id<0||static_cast<std::size_t>(plan.ai_id)>=input.ai->rows.size())plan.ai_id=8;plan.character_type=input.ai->rows[plan.ai_id].type;plan.is_player=original_actor_source_is_player(plan.character_type,input.source_name);
 // Temporary definition calculation only. No allocation or fake receiver is
 // returned; original pointer-bearing config remains strictly stack-local.
 dh2::physical::CharacterBodyInput request{};request.owner=reinterpret_cast<void*>(input.owner_identity);request.new_physical=&request;request.character_type=plan.character_type;request.is_player=plan.is_player;request.special_owner_byte=input.static84;
 const auto&box=plan.bounds.absolute_box;request.absolute_bounds[0]=box[0];request.absolute_bounds[1]=box[1];request.absolute_bounds[2]=box[3];request.absolute_bounds[3]=box[4];request.position[0]=input.position.x;request.position[1]=input.position.y;dh2::physical::CharacterBodyConfig definition{};if(dh2_character_body_config(&definition,&request))throw std::runtime_error("Original body definition rejected source plan");
 plan.physical_enabled=definition.enabled;plan.circular=definition.enabled&&definition.shape.kind==0;plan.radius_game_units=plan.circular?definition.radius*100.f:0;plan.group_index=definition.shape.group_index;plan.category_bits=definition.shape.category_bits;plan.mask_bits=definition.shape.mask_bits;
 out=std::move(plan);e.clear();return true;
 }catch(const std::exception&failure){e=failure.what();return false;}}
}
