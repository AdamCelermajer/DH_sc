#define main previous_identity_fixture_main
#include "player_save_identity_authority_v29.cpp"
#undef main
#include "../player_add_after_save_v29.hpp"
#include "../character_script_objects.hpp"
using namespace dh2::player;
struct AddFixture {
 MatchingLocalSelectionOwnerV4 matching;PlayerNetworkLocalOwnerV4 network{matching};
 PlayerManagerOwnerV1 manager{{this,manager_service}};std::shared_ptr<void> lease=std::make_shared<int>(1);
 std::map<PlayerInfoFieldsV1*,std::unique_ptr<PlayerInfoSkillBuffersV26>> arrays;
 PlayerInfoFieldsV1* info{};std::unique_ptr<PlayerAddAfterSaveV29> continuation;
 std::shared_ptr<character::ScriptCharacterObject> object;actor::RuntimeState runtime{};target_providers::Handle16 handle{};
 std::uint32_t type{};std::int32_t room=-1,controller{},internal{};const char* class_name="Character";
 std::string name="PlayerCharacter_0",archetype="Character",error;std::unique_ptr<world::CanonicalPlayerFacetV3> facet;
 std::int32_t class380=-1;std::uint8_t visible4e5{};unsigned initialized{},slots_updated{},camera{},controller_attached{},light_attached{};
 bool fail_init{},fail_controller{};std::vector<PlayerAddOperationV5> trace;
 static bool manager_service(void* raw,const PlayerManagerRequestV1& q,PlayerManagerResponseV1& out,std::string& e){
  auto& f=*static_cast<AddFixture*>(raw);
  if(q.operation==PlayerManagerOperationV1::construct_player_info){*q.player=PlayerInfoFieldsV1{};if(!f.network.construct(*q.player,f.lease,e))return false;auto a=std::make_unique<PlayerInfoSkillBuffersV26>();if(!a->construct(*q.player,f.network,e))return false;f.arrays.emplace(q.player,std::move(a));return true;}
  if(q.operation==PlayerManagerOperationV1::online_enabled){out.value=0;return true;}
  if(q.operation==PlayerManagerOperationV1::character_initialization)return f.continuation->execute(*q.player,q.character_count6c4,q.id,e);
  e="Required other manager operation";return false;
 }
 explicit AddFixture(std::shared_ptr<data::PropertyState> p){
  check(manager.initialize(error)&&manager.add_player(0,0,0,true,error)&&manager.get_by_internal(0,false,info,error),error);
  object=std::make_shared<character::ScriptCharacterObject>(0x100000001ULL,name,std::move(p),std::make_shared<data::CombatActorState>(),std::array<float,3>{});handle.cached=object->identity;
  world::CanonicalPlayerFieldsV3 fields;fields.object=object;fields.runtime=&runtime;fields.handle=&handle;fields.type_f4=&type;fields.room64=&room;fields.class_name20=&class_name;fields.name=&name;fields.archetype=&archetype;fields.world_lease=lease;facet=std::make_unique<world::CanonicalPlayerFacetV3>(fields);
  info->character660=object->identity; // Declared development after-source660 entry.
 }
 PlayerAddServicesV5 services(PlayerSaveIdentityAuthorityV29& save){return {lease,
  [this,&save](const auto& q,auto& r,std::string& e){trace.push_back(q.operation);using O=PlayerAddOperationV5;
   switch(q.operation){
   case O::online:r.value=0;return true;
   case O::init_all:{++initialized;std::int32_t metadata{};if(!save.safe_properties_id(metadata,e))return false;if(fail_init){e="Required declared whole InitAll endpoint";return false;}return true;}
   case O::is_active:r.value=1;return true; // Declared offline endpoint fixture.
   case O::init_camera:++camera;return true;
   case O::set_idle:return true;
   case O::set_visible:check(q.argument==1,"Actual CNet local player visibility");return true;
   case O::current_level:r.identity=0;return true; // No fake Level/QuickSave success.
   case O::attach_controller:++controller_attached;if(fail_controller){e="Required declared controller endpoint";return false;}return true;
   case O::attach_light:++light_attached;return true;
   default:e="Unprovided source endpoint "+std::to_string(std::uint32_t(q.operation));return false;
   }
  },
  [this](auto& record,auto& out,std::string& e){if(&record!=info){e="Wrong network record";return false;}out={&class380,&visible4e5};return true;},
  [this](auto id,auto& out,std::string& e){if(id!=object->identity){e="Wrong Character";return false;}out={id,lease,facet.get(),&controller,&internal};return true;}};}
};
int main(int argc,char** argv){try{
 check(argc==2,"Expected actual cache data directory");std::string error,path=argv[1];auto a=read(path+"/character_properties_pyarray.bin"),b=read(path+"/character_properties_pyarraynames.bin"),c=read(path+"/character_properties_pystructnames.bin");data::CharacterTable table;check(data::load_characters(view(a),view(b),view(c),table,error),error);data::PropertyRules rules;check(data::load_property_rules(table,rules,error),error);
 a=read(path+"/character_classes_pyarray.bin");b=read(path+"/character_classes_pyarraynames.bin");c=read(path+"/character_classes_pystructnames.bin");data::ClassTables classes;check(data::load_classes(view(a),view(b),view(c),classes,error),error);
 a=read(path+"/ai_pyarray.bin");b=read(path+"/ai_pyarraynames.bin");c=read(path+"/ai_pystructnames.bin");auto fa=read(path+"/ai_factions_pyarray.bin"),fb=read(path+"/ai_factions_pyarraynames.bin"),fc=read(path+"/ai_factions_pystructnames.bin");data::AiTables ai;check(data::load_ai(view(a),view(b),view(c),view(fa),view(fb),view(fc),ai,error),error);
 a=read(path+"/skills_pyarray.bin");b=read(path+"/skills_pyarraynames.bin");c=read(path+"/skills_pystructnames.bin");data::SkillTables skills;check(skills.load(view(a),view(b),view(c),error),error);auto skillrows=skills.borrow();
 auto at=std::find(table.names.begin(),table.names.end(),"KnightPlayerBase");check(at!=table.names.end(),"Actual authored player fallback");const auto knight=int(at-table.names.begin());
 for(unsigned scenario=0;scenario<3;++scenario){
  auto properties=std::make_shared<data::PropertyState>();data::reset_properties(rules,*properties,&table.rows[std::size_t(knight)]);check(data::recalc_properties_with_class(classes,rules,*properties,error),error);
  AddFixture f(properties);Source source{ai,*properties};data::LootRandom8V2 random{};std::int16_t metadata=-1;auto actual_save=std::make_shared<data::PlayerSavegameV1>();actual_save->set_character(f.object->identity);
  const auto list=properties->resolved[28];check(list>=0&&std::size_t(list)<skillrows.lists().size()&&actual_save->initialize_skills(skillrows.lists()[list],error),error);
  PlayerSaveIdentityAuthorityV29 authority(actual_save,f.object->identity,metadata,table,random,{&source,Source::player,nullptr,nullptr},{},{});
  data::SavedSkillUpdateServicesV1 update{&f,[](void* raw,auto id,std::string& e){auto& f=*static_cast<AddFixture*>(raw);if(id!=f.object->identity){e="Wrong skill update Character";return false;}++f.slots_updated;return true;}};
  f.continuation=std::make_unique<PlayerAddAfterSaveV29>(f.manager,authority,f.services(authority),f.network,*f.arrays.at(f.info),update);
  f.fail_init=scenario==0;f.fail_controller=scenario==2;
  check(*f.manager.character_count_field()==0,"Source count before continuation");const bool ok=f.manager.add_character(0,error);
  check(f.initialized==1&&metadata==knight&&actual_save->class_id()==knight,"Actual source metadata/Save phase");
  check(std::find(f.trace.begin(),f.trace.end(),PlayerAddOperationV5::initialize_save)==f.trace.end(),"Constructed Save allocation replayed");
  check(f.info->character_base_id13c8==&metadata&&&authority.receiver()==actual_save.get(),"Different Save/metadata authority published");
  if(scenario==0){check(!ok&&*f.manager.character_count_field()==0&&f.slots_updated==0,"Failed InitAll published initialized player");}
  else{check(*f.manager.character_count_field()==1&&f.slots_updated==3&&f.camera==1&&f.controller_attached==1,"Source slots/camera/count/controller ordering");
   for(int slot=0;slot<3;++slot)check(actual_save->skill_in_slot(slot)==-1,"Source constructor arrays were replaced by selected demo skill");
   check(ok==(scenario==1)&&f.light_attached==(scenario==1?1u:0u),"Source controller failure tail differs");}
  const auto count=*f.manager.character_count_field();check(!f.manager.add_character(0,error)&&*f.manager.character_count_field()==count,"Failed/completed source prefix replayed");
 }
 std::cout<<"PASS "<<checks<<" checks; actual cache/Save/PM/Net/skill buffers; no Save C1 replay; actual count store ordering; InitAll/camera/controller/light endpoints explicitly fixtures; no live startup claim\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
