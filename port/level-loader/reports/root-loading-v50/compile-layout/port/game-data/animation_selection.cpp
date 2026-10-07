#include "animation_tables.hpp"
#include <stdexcept>
extern "C" std::uint32_t dh2_animation_random(std::uint32_t* seed,std::uint32_t* calls,std::uint32_t count){
 std::uint32_t result=0;
 if(count){*seed=(*seed*59051u+177149u)%14348907u;result=*seed%count;}
 ++*calls;return result;
}
namespace dh2::data {
bool choose_animation_start(const AnimationTables& table,std::int32_t id,AnimationRandom& random,AnimationStart& out,std::string& error,bool random_enabled){
 out={};error.clear();try{
  AnimationStart next;auto rng=random;
  for(unsigned depth=0;depth<3;++depth){
   if(id<0||std::size_t(id)>=table.sequences.size())throw std::runtime_error("Animation start reference outside table");
   const auto& sequence=table.sequences[id];if(sequence.steps.empty())throw std::runtime_error("Animation start has no steps");
   unsigned index=sequence.type==2&&random_enabled?dh2_animation_random(&rng.seed,&rng.calls,sequence.steps.size()):0;
   next.layers.emplace_back(id,index);const auto& step=sequence.steps[index];
   if(step.redir==1){id=step.anim;continue;}
   if(step.redir!=0||step.anim<0)throw std::runtime_error("Animation start has no clip");
   next.step=step;out=std::move(next);random=rng;return true;
  }throw std::runtime_error("Animation redirect exceeds original three layers");
 }catch(const std::exception& e){error=e.what();return false;}
}
}
