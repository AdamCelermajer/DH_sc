#define main existing_mesh_fixture_main
#include "character_mesh_fx_owner_v1.cpp"
#undef main
int main(int argc,char**argv){try{
 require(argc==4,"table/assets/prince arguments");std::string error;
 const std::string table=argv[1];auto a=read(table+"/effects_pyarray.bin"),b=read(table+"/effects_pyarraynames.bin"),c=read(table+"/effects_pystructnames.bin"),d=read(table+"/effects_dictionary_pyarraynames.bin"),f=read(table+"/effects_dictionary_pyarray.bin");
 data::EffectsTables tables;require(tables.load(bytes(a),bytes(b),bytes(c),bytes(d),bytes(f),error),error);auto borrow=tables.borrow();
 auto raw=read(argv[3]);resources::BresView view{};require(dh2_bres_open(&view,raw.data(),raw.size())==resources::BresError::ok,"Actual Prince BRES");scene::Scene live;require(scene::load(view,live,error),error);
 Providers providers{argv[2],&live};fx::CharacterMeshFxOwnerV1 manager(borrow,live,{&providers,Providers::asset},{&providers,Providers::service});require(manager.precache_libraries(error),error);
 std::vector<skinning::VisualDrawPartV6> retained;
 const float position[3]{17,29,41},rotation[3]{0.1f,0.2f,0.3f};unsigned draws=0;
 for(int set:{79,80}){
  std::uintptr_t id=0;require(manager.play_set(set,position,rotation,0,&id,error),error);require(id!=0,"Actual positive blood set creates source mesh instance");
  auto snapshots=manager.views();require(!snapshots.empty()&&snapshots.back().state.fixed_rotation&&snapshots.back().state.anchor==0,"Actual source fixed target rotation/no anchor");
  require(manager.scene_frame(0,error)&&manager.manager_frame(0,error),error);
  std::vector<skinning::VisualDrawPartV6> parts;require(manager.draw_parts(parts,error),error);require(!parts.empty(),"Positive source blood resource yields mesh draw");
  for(const auto& part:parts){require(part.geometry&&part.material_table&&part.materials&&part.retention&&!part.positions.empty(),"Actual retained draw resource");require(std::all_of(part.world.begin(),part.world.end(),[](float x){return std::isfinite(x);}),"Finite source world matrix");++draws;}
  retained.insert(retained.end(),parts.begin(),parts.end());
  const auto end=manager.views().back().end_ms;
  require(manager.scene_frame(end+1,error)&&manager.manager_frame(end+1,error),error);
 }
 require(draws>=2&&manager.cold_creations()==2,"Both actual blood resources used");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_blood_sets\":[79,80],\"actual_mesh_draws\":"<<draws<<",\"cold_creations\":"<<manager.cold_creations()<<",\"actual_debug\":true,\"gpu_submission\":false}"<<std::endl;return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<std::endl;return 1;}}
