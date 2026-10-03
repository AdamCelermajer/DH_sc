#include "animation.hpp"
#include "skinning.hpp"
#include "animation_tables.hpp"
#include "vitals.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
using namespace dh2::data;
std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
Bytes bytes(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
int main(int argc,char** argv){try{
 if(argc!=3&&argc!=4)return 2;const bool death=argc==4&&std::string(argv[3])=="--death";const std::string root=argv[1];std::string error;
 auto a=read(root+"/data/character_properties_pyarray.bin"),b=read(root+"/data/character_properties_pyarraynames.bin"),c=read(root+"/data/character_properties_pystructnames.bin");CharacterTable characters;PropertyRules rules;check(load_characters(bytes(a),bytes(b),bytes(c),characters,error)&&load_property_rules(characters,rules,error),error);
 auto ca=read(root+"/data/character_classes_pyarray.bin"),cb=read(root+"/data/character_classes_pyarraynames.bin"),cc=read(root+"/data/character_classes_pystructnames.bin");ClassTables classes;check(load_classes(bytes(ca),bytes(cb),bytes(cc),classes,error),error);
 auto knight=std::find(characters.names.begin(),characters.names.end(),"KnightPlayerBase");check(knight!=characters.names.end(),"Knight preset absent");const unsigned row=knight-characters.names.begin();PropertyState owner;reset_properties(rules,owner,&characters.rows[row]);SpawnVitals initialized;check(recalc_properties_with_class(classes,rules,owner,error)&&initialize_spawn_vitals(rules,owner,initialized,error),error);
 auto reference=read(argv[2]);check(reference.size()==448*3584,"Original reference dimensions");unsigned at=row*3584;for(const auto* sheet:{&owner.base,&owner.saved,&owner.gear,&owner.resolved}){check(std::memcmp(sheet->data(),reference.data()+at,896)==0,"Player sheet differs from original");at+=896;}
 a=read(root+"/data/animations_pyarray.bin");b=read(root+"/data/animations_pyarraynames.bin");c=read(root+"/data/animations_pystructnames.bin");auto d=read(root+"/data/animations_dictionary_pyarraynames.bin"),e=read(root+"/data/animations_dictionary_pyarray.bin");Dictionary dictionary;AnimationTables tables;check(load_dictionary(bytes(d),bytes(e),dictionary,error)&&load_animation_tables(bytes(a),bytes(b),bytes(c),dictionary,tables,error),error);
 const auto* attack=animation_state(tables,owner.resolved[2],death?"Died":"AttackStatic");check(attack&&attack==&tables.sequences[death?259:243],"Original Knight stationary attack differs");std::set<int> ids;std::function<void(int,unsigned)> collect=[&](int id,unsigned depth){check(depth<3,"Redirect depth");for(const auto& step:tables.sequences.at(id).steps)if(step.redir)collect(step.anim,depth+1);else ids.insert(step.anim);};collect(attack-tables.sequences.data(),0);check(ids.size()==(death?1u:9u),"Original player clip count");
 auto model=read(root+"/models/prince_modular.bdae");dh2::resources::BresView view{};check(dh2_bres_open(&view,model.data(),model.size())==dh2::resources::BresError::ok,"Prince BRES rejected");dh2::scene::Scene rest;check(dh2::scene::load(view,rest,error),error);
 std::vector<dh2::skinning::Skin> skins;std::vector<std::vector<std::array<float,3>>> positions;
 for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::controller);++i){dh2::skinning::Skin skin;check(dh2::skinning::load(view,i,rest,skin,error),error);if(skin.id.find("_default_warrior-mesh-skin")==std::string::npos)continue;dh2::assets::Mesh mesh{};check(dh2_mesh_open(&mesh,&view,skin.geometry)==dh2::assets::Error::ok,"Prince mesh rejected");dh2::assets::Primitive primitive{};dh2_mesh_primitive(&mesh,0,&primitive);dh2::assets::Attribute attribute{};check(dh2_mesh_attribute(&mesh,primitive.attributes[0],&attribute)==dh2::assets::Error::ok,"Position missing");positions.emplace_back(mesh.vertices);for(unsigned v=0;v<mesh.vertices;++v){float value[4]{};check(dh2_attribute_read(&attribute,v,value),"Position rejected");std::copy(value,value+3,positions.back()[v].begin());}skins.push_back(std::move(skin));}
 check(skins.size()==4,"Warrior preview skin count");unsigned total=0,events=0;std::cout<<"{\"player_row\":"<<row<<",\"original_owner_words_compared\":896,\"hp_raw\":"<<owner.resolved[36]<<",\"mp_raw\":"<<owner.resolved[41]<<",\"animation_table\":"<<owner.resolved[2]<<",\"clips\":[";bool first=true;
 for(int id:ids){auto path=dictionary.values.at(id);auto name=path.substr(path.find_last_of("/\\")+1);auto raw=read(root+"/animations/"+name);dh2::animation::Player player;check(player.load(raw.data(),raw.size(),rest,error),error);check(player.unbound==0&&player.skipped==0,"Unbound or unsupported Prince track");auto scene=rest;unsigned poses=0;
  for(int ms=player.start;ms<=player.end;++ms){check(player.sample(scene,ms,error),error);for(unsigned i=0;i<skins.size();++i){std::vector<dh2::skinning::Matrix> palette;std::vector<std::array<float,3>> output;check(dh2::skinning::palette(skins[i],scene,palette,error)&&dh2::skinning::positions(skins[i],palette,positions[i],output,error),error);for(const auto& position:output)for(float v:position)check(std::isfinite(v),"Nonfinite Prince skin");}++poses;}
  const int event=dh2_events_time(&player.events.view(),"attack_mainhand");events+=event>=0;total+=poses;if(!first)std::cout<<',';first=false;std::cout<<"{\"id\":"<<id<<",\"name\":\""<<name<<"\",\"start\":"<<player.start<<",\"end\":"<<player.end<<",\"tracks\":"<<player.track_count()<<",\"attack_mainhand_ms\":"<<event<<",\"poses\":"<<poses<<'}';
 }
 check(events==(death?0u:3u),"Authored player melee event count");std::cout<<"],\"warrior_skins\":4,\"every_millisecond_poses\":"<<total<<",\"melee_event_tracks\":"<<events<<",\"finite_skinned_vertices\":true}\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 3;}}
