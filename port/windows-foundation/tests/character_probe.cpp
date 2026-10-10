#include "../original_character.hpp"
#include "../asset_catalog.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
std::vector<Vec3> snapshot(const CharacterVisual& v){std::vector<Vec3> result;for(const auto&m:v.meshes())for(const auto&p:m.vertices){check(std::isfinite(p.position.x)&&std::isfinite(p.position.y)&&std::isfinite(p.position.z),"nonfinite position");check(std::isfinite(p.normal.x)&&std::isfinite(p.normal.y)&&std::isfinite(p.normal.z),"nonfinite normal");result.push_back(p.position);}return result;}
bool changed(const std::vector<Vec3>& a,const std::vector<Vec3>& b){check(a.size()==b.size(),"vertex domain changed");for(size_t i=0;i<a.size();++i)if(std::abs(a[i].x-b[i].x)+std::abs(a[i].y-b[i].y)+std::abs(a[i].z-b[i].z)>0.0001f)return true;return false;}
int main(int argc,char**argv){try{
check(argc==2,"expected asset root");AssetCatalog assets(argv[1]);CharacterVisual v;std::string e;
CharacterVisualConfig c;c.model_path="models/prince_modular.bdae";c.template_clip_path="animations/prince_template_anim.bdae";c.animation_paths={"animations/prince_idle_shield.bdae","animations/prince_walk_1hand.bdae","animations/prince_1hand_combo_01.bdae"};c.skin_id_contains="_default_warrior-mesh-skin";c.expected_controller_count=4;
check(v.load(assets,c,e),e);size_t vertices=0,indices=0;for(const auto&m:v.meshes()){vertices+=m.vertices.size();indices+=m.indices.size();}check(vertices>0&&indices>0,"empty geometry");
for(auto pose:{CharacterPose::idle,CharacterPose::walk,CharacterPose::attack}){v.select(pose);check(v.update(0,e),e);auto begin=snapshot(v);check(v.update(0.15,e),e);check(changed(begin,snapshot(v)),"animation did not change vertices");check(v.update(std::numeric_limits<double>::max(),e),e);snapshot(v);}
v.select(static_cast<CharacterPose>(99));check(v.update(0,e),e);check(!v.update(-1,e),"negative elapsed accepted");check(!v.update(std::numeric_limits<double>::infinity(),e),"infinite elapsed accepted");
auto old=snapshot(v);auto bad=c;bad.model_path="models/missing.bdae";check(!v.load(assets,bad,e),"missing reload accepted");check(!changed(old,snapshot(v)),"failed reload changed character");
CharacterVisual moved=std::move(v);check(!v.loaded()&&v.meshes().empty()&&v.texture_uris().empty(),"moved-from unsafe");check(moved.update(0.01,e),e);
c.clips={{"custom-combat",c.animation_paths[2]},{"custom-idle",c.animation_paths[0]},{"custom-walk",c.animation_paths[1]}};check(moved.load(assets,c,e),e);
check(moved.select("custom-combat",false,e),e);check(moved.update(0,e),e);auto attackStart=snapshot(moved);check(moved.update(0.15,e),e);check(changed(attackStart,snapshot(moved)),"named clip did not animate");check(moved.update(10,e),e);auto ended=snapshot(moved);check(moved.update(0.2,e),e);check(!changed(ended,snapshot(moved)),"one-shot endpoint unstable");check(moved.update(std::numeric_limits<double>::max(),e),e);check(!changed(ended,snapshot(moved)),"one-shot huge interval unsafe");
check(!moved.select("missing-clip",true,e),"unknown clip accepted");check(!changed(ended,snapshot(moved)),"unknown select changed pose");
check(moved.select("custom-walk",true,e),e);check(moved.update(0,e),e);auto walkStart=snapshot(moved);check(moved.update(0.8,e),e);check(!changed(walkStart,snapshot(moved)),"loop did not wrap at original 800ms duration");
auto duplicate=c;duplicate.clips.push_back(c.clips[0]);check(!moved.load(assets,duplicate,e),"duplicate name accepted");check(!changed(walkStart,snapshot(moved)),"duplicate reload changed pose");
std::cout<<"{\"status\":\"pass\",\"meshes\":"<<moved.meshes().size()<<",\"vertices\":"<<vertices<<",\"indices\":"<<indices<<",\"animated_clips\":3,\"large_elapsed\":true,\"failed_reload_preserved\":true,\"moved_from_safe\":true}\n";
}catch(const std::exception&ex){std::cerr<<ex.what()<<'\n';return 1;}}
