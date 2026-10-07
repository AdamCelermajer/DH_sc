#include "skin_pose_cache_v32.hpp"
#include <cmath>
#include <cstring>
#include <atomic>
namespace dh2::skinning {
namespace {
std::atomic<std::uint64_t> next_pose_revision{1};
template<class T> void size_storage(std::vector<T>& v,std::size_t n,SkinPoseCountersV32& c){
 const auto old=v.capacity();v.resize(n);if(v.capacity()!=old)++c.storage_growths;
}
}
void SkinPoseCacheV32::reset()noexcept{skin_=nullptr;rest_=nullptr;valid_=changed_=false;++revision_;}
void SkinPoseCacheV32::bind(const Skin& skin,const std::vector<std::array<float,3>>& rest){
 if(skin_==&skin&&rest_==&rest)return;
 skin_=&skin;rest_=&rest;valid_=changed_=false;++revision_;
}
bool SkinPoseCacheV32::sample(const scene::Scene& scene,std::string& error){
 error.clear();++counters_.calls;changed_=false;
 if(!skin_||!rest_){error="V32 skin immutable binding absent";return false;}
 const auto& skin=*skin_;const auto& rest=*rest_;
 if(skin.nodes.empty()||skin.nodes.size()!=skin.inverse_bind.size()){
  error="Skin joint table differs";return false;
 }
 if(rest.size()!=skin.influences.size()||!skin.influence_count||skin.influence_count>4){
  error="Skin input dimensions differ";return false;
 }
 bool changed=!valid_||worlds_.size()!=skin.nodes.size();
 for(std::size_t i=0;i<skin.nodes.size();++i){
  ++counters_.joint_checks;
  if(skin.nodes[i]>=scene.graph.size()){error="Skin joint out of range";return false;}
  if(!changed&&std::memcmp(scene.graph[skin.nodes[i]].world.data(),worlds_[i].data(),sizeof(Matrix)))changed=true;
 }
 if(!changed){++counters_.hits;return true;}
 size_storage(candidate_worlds_,skin.nodes.size(),counters_);
 for(std::size_t i=0;i<skin.nodes.size();++i)candidate_worlds_[i]=scene.graph[skin.nodes[i]].world;
 size_storage(palette_,skin.nodes.size()*16,counters_);
 for(std::size_t i=0;i<skin.nodes.size();++i){
  auto* matrix=palette_.data()+i*16;
  dh2_skin_palette_matrix(matrix,candidate_worlds_[i].data(),skin.inverse_bind[i].data(),skin.bind_shape.data());
  for(unsigned k=0;k<16;++k)if(!std::isfinite(matrix[k])){error="Skin matrix overflow";return false;}
 }
 size_storage(candidate_positions_,rest.size(),counters_);
 for(std::size_t i=0;i<rest.size();++i){
  const auto& influence=skin.influences[i];
  for(unsigned j=0;j<skin.influence_count;++j)if(influence.joints[j]>=skin.nodes.size()){
   error="Skin influence out of range";return false;
  }
  dh2_skin_point(candidate_positions_[i].data(),palette_.data(),influence.joints.data(),influence.weights.data(),skin.influence_count,rest[i].data());
  for(float x:candidate_positions_[i])if(!std::isfinite(x)){error="Skin position overflow";return false;}
 }
 worlds_.swap(candidate_worlds_);positions_.swap(candidate_positions_);
 valid_=changed_=true;revision_=next_pose_revision.fetch_add(1,std::memory_order_relaxed);
 ++counters_.deformations;counters_.vertices+=rest.size();return true;
}
}
