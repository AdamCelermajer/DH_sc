#include "../retained_character_actor_v1.hpp"
#include "../retained_character_visual_connection_v5.hpp"
#include "../character_visual_asset_owner_v6.hpp"
#include "../character_same_scene_animator_v6.hpp"
#include "../character_scene.hpp"
#include "../character_live_body_projection_v6.hpp"
#include "../physical_world.hpp"
#include "../retained_character_position_owner_v7.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>
using namespace dh2;
static unsigned checks;
static void check(bool b,const std::string& e){++checks;if(!b)throw std::runtime_error(e);}
static std::vector<std::uint8_t> read(std::string path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("cache "+path);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{
 check(argc==2,"actual cache");std::string directory=argv[1],error;std::vector<std::vector<std::uint8_t>> raw;raw.reserve(32);
 auto bytes=[&](const std::string& name){raw.push_back(read(directory+"/"+name));auto& b=raw.back();return data::Bytes{b.data(),b.size()};};
 character::GameDesignInputs256 input{};character::GameDesignTableInput48* slots[]{&input.characters,&input.classes,&input.ai,&input.factions,&input.levels};const char* names[]{"character_properties","character_classes","ai","ai_factions","levels"};
 for(unsigned i=0;i<5;++i)*slots[i]={bytes(std::string(names[i])+"_pyarray.bin"),bytes(std::string(names[i])+"_pyarraynames.bin"),bytes(std::string(names[i])+"_pystructnames.bin")};
 character::CharacterGameDesign design;check(design.initialize(input,error),error);auto tables=design.borrow();
 data::Dictionary models,clips;check(data::load_dictionary(bytes("character_models_dictionary_pyarraynames.bin"),bytes("character_models_dictionary_pyarray.bin"),models,error),error);check(data::load_dictionary(bytes("animations_dictionary_pyarraynames.bin"),bytes("animations_dictionary_pyarray.bin"),clips,error),error);
 data::AnimationTables animation;check(data::load_animation_tables(bytes("animations_pyarray.bin"),bytes("animations_pyarraynames.bin"),bytes("animations_pystructnames.bin"),clips,animation,error),error);
 auto world=std::make_shared<int>(1);auto manager=std::make_shared<world::GameObjectSceneRootRegistryV1>();auto sets=std::make_shared<character::CharacterAnimationSetCacheV6>();unsigned families=0,frames=0;physical::NativeWorld physical_world;const float physical_bounds[4]{-1000,-1000,1000,1000};physical_world.load(physical_bounds);
 for(const char* name:{"WanderingPriest","Crypt_Skeleton","CryptSlime","Crypt_Ghost","WanderingPriest"}){
  auto row=std::find(tables.characters()->names.begin(),tables.characters()->names.end(),name);check(row!=tables.characters()->names.end(),"actual family row");auto index=std::size_t(row-tables.characters()->names.begin());
  auto properties=std::make_shared<data::PropertyState>();data::reset_properties(*tables.rules(),*properties,&tables.characters()->rows[index]);check(data::recalc_properties_with_class(*tables.classes(),*tables.rules(),*properties,error),error);auto life=std::make_shared<data::CombatActorState>();
  auto actor=std::make_shared<character::RetainedCharacterActorV1>(0x6000+index,world,"Character",character::RetainedCharacterConstructionV7::fresh_canonical);world::CanonicalPropertyMapV1 schema({nullptr,&world::canonical_vec3_origin_v1(),nullptr});auto property=actor->properties();check(schema.init_properties(property,error)&&schema.load_defaults(property,error),error);
  auto object=std::make_shared<character::ScriptCharacterObject>(0x6000+index,name,properties,life,std::array<float,3>{});check(actor->construct_fields(object,{}),actor->error());
  const auto model=properties->resolved[3];check(model>=0&&std::size_t(model)<models.values.size(),"actual model ID");check(property.fields.write_string(property.fields.context,0x290,models.values[model],error),error);
  float scale[3];check(dh2_character_visual_scale(scale,properties->base.data()+12)==0,"source scale");check(property.fields.write_vector3(property.fields.context,0x120,{scale[0],scale[1],scale[2]},error),error);
  unsigned pf_calls=0;world::GameObjectVisualFieldBorrowV5 fields;check(actor->visual_fields_v5(actor,[&](auto&){++pf_calls;return true;},fields,error),error); // explicit external PF fixture
  world::RetainedGameObjectVisualServicesV1 vs;vs.owner=world;vs.parent_is_animated=[](bool& b,auto&){b=true;return true;};vs.update_pf=[&](auto&){++pf_calls;return true;};
  auto source_read=[&](const std::string& uri,std::vector<std::uint8_t>& b,std::string& e){std::string p=uri;std::transform(p.begin(),p.end(),p.begin(),[](unsigned char c){return char(std::tolower(c));});try{b=read(directory+"/"+p);return true;}catch(const std::exception& x){e=x.what();return false;}};
  vs.read_asset=[&](const auto& path,auto& payload,bool& found,auto& e){found=source_read(path,payload,e);return found;};
  auto connection=std::make_shared<world::RetainedCharacterVisualConnectionV5>(fields,actor->source_visual(),vs,manager);
  std::string* model_name=nullptr;std::string* xref=nullptr;check(actor->visual_strings_v6(model_name,xref,error),error);
  world::CharacterVisualAssetOwnerV6 assets(object->identity,actor->source_visual(),*model_name,*xref,connection->services(connection));check(assets.load_visual(error),error);auto visual=connection->attached();check(visual&&visual->ready()&&visual->root_game_object()==object->identity,"same root parent authority");check(manager->roots().size()==1,"actual same manager root registration");
  const std::array<float,3> root_scale{visual->binding().root.scale[0],visual->binding().root.scale[1],visual->binding().root.scale[2]};const std::array<float,4> root_quaternion{visual->binding().root.quaternion[0],visual->binding().root.quaternion[1],visual->binding().root.quaternion[2],visual->binding().root.quaternion[3]};
  character::RetainedCharacterPositionBackendsV7 position_services;position_services.world=world;position_services.visual_sync_position=[&](auto identity,auto& e){check(identity==actor->source_visual(),"same position-only visual");return visual->sync_position_v7(e);};character::RetainedCharacterPositionOwnerV7 position_owner(*actor,position_services);const float offset[3]{11,23,37};check(position_owner.set_position(offset,true,error),error.c_str());
  for(unsigned i=0;i<3;++i)check(visual->binding().root.position[i]==offset[i]&&visual->binding().root.scale[i]==root_scale[i],"source position-only root update preserves scale");for(unsigned i=0;i<4;++i)check(visual->binding().root.quaternion[i]==root_quaternion[i],"source position-only root preserves rotation");check(actor->runtime.object.motion.floor==~0u,"pre-InitPost position does not synthesize floor");
  physical::CharacterOwnerBoundsInput expected_input{};std::copy_n(visual->mesh_box().data(),6,expected_input.mesh_box);std::copy_n(object->position.data(),3,expected_input.position);expected_input.collision_scale=properties->resolved[16];expected_input.already_scaled=visual->marker().found;physical::CharacterOwnerBounds expected_bounds{};check(dh2_character_owner_bounds(&expected_bounds,&expected_input)==0,"source marker bounds kernel");for(unsigned i=0;i<6;++i)check(actor->runtime.subobjects.local_bounds[i]==expected_bounds.relative_box[i],"actual Visual+28 marker forwarded to SAME Character bounds");
  auto property_view=data::property_view(*tables.rules(),*properties);physical::NativeBody native{};physical::NpcBodyRequest body_request;body_request.properties=&property_view;body_request.ai=tables.ai();body_request.owner=reinterpret_cast<void*>(object->identity);body_request.new_physical=&native;body_request.static_owner=*actor->source_bool_field(0x84);
  physical::NpcBodyProjection projection;check(character::character_live_body_projection_v6(*actor,*visual,body_request,projection,error),error);check(projection.is_player==0&&projection.body.enabled&&projection.body.pinned,"genuine non-player body branch");
  if(std::string(name)=="WanderingPriest")check(projection.character_type==2&&projection.body.shape.group_index==-2&&projection.body.shape.category_bits==8&&projection.body.shape.mask_bits==0xd3b,"original Priest type2 physical filter");
  physical::WorldObject physical_object{};native={physical_world.create_character(projection.body,&physical_object),projection.body.radius,projection.body.pinned};check(native.body!=nullptr,"genuine NativeWorld body allocation");physical::NativeBodyObservation observation;check(dh2_native_body_observe(&observation,&native)==0&&observation.mass==0&&observation.pinned,"source POCharacter pinned native body");physical_world.destroy(native.body);
  character::CharacterSameSceneAnimatorServicesV6 services;services.world=world;services.same_set_manager=sets;services.animation_dictionary=&clips;services.read=source_read;
  services.registration.invoke=[](const auto&,auto&){return true;}; // declared Debug/FX preload fixture; no full InitPost claim
  services.attach=[visual](auto remove,auto& e){return visual->attach_character_animator_v6(std::move(remove),e);};services.pose_changed=[visual](auto& e){return visual->character_pose_changed_v6(e);};
  character::CharacterSameSceneAnimatorV6 animator(visual,services);const int anim_table=properties->resolved[2];check(animator.initialize(animation,anim_table,3,{},error),std::string(name)+": "+error);
  check(&animator.binding().root==&visual->binding().root,"SAME retained Root storage");check(visual->root_animator_present(),"root actual animator attached");
  const auto idle=animation.characters.at(anim_table).fields[9].at(0);character::State* state=&actor->machine->state();auto flags=state->flags;data::AnimationRandom random{123456789,0};check(!animator.start(animation,idle,random,1.f,error)&&error=="Required SAME Character event and authored-step FX services","missing actual event/FX cannot silently start");
  animator.playback().observer={nullptr,[](void*,actor::BlendedPlayback&,const actor::BlendedPlaybackEvent&){}};animator.playback().selection_fx_v2={nullptr,[](void*,actor::BlendedPlayback&,const data::AnimationStep&,std::string&){return true;}}; // declared event/FX fixture, not gameplay success
  check(animator.start(animation,idle,random,1.f,error),error);
  auto* scene_identity=&visual->scene();for(unsigned ms=0;ms<1200;ms+=33){check(animator.scene_phase(ms,error)&&animator.animator_phase(animation,random,1.f,error),error);check(&visual->scene()==scene_identity&&actor->object->properties==properties&&state->flags==flags,"same scene/property/FSM authorities");for(auto& skin:visual->skinned_meshes())check(skin.positions.size()==skin.source_positions.size(),"same retained skin resampled");++frames;}
  check(pf_calls>0&&manager->roots().size()==1,"bounds/PF fixture plus sole root");check(assets.set_visual(std::uintptr_t{},error),error);check(!animator.ready()&&manager->roots().empty()&&actor->source_visual()==0,"source root teardown removes actual animator before receiver release");++families;
 }
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_families\":"<<families<<",\"same_scene_frames\":"<<frames<<",\"external_PF_Debug_FX_fixtures\":true}"<<std::endl;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
