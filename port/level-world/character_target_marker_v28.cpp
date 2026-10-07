#include "character_target_marker_v28.hpp"
#include <algorithm>
#include "canonical_point3d_globals_v1.hpp"
namespace dh2::character {
CharacterTargetMarkerV28::CharacterTargetMarkerV28(fx::CharacterMeshFxOwnerV4& fx,data::EffectsTables::Borrow tables,std::uintptr_t player,TargetMarkerServicesV28 services)
 :effects_(fx),tables_(std::move(tables)),services_(services),player_(player){}
CharacterTargetMarkerV28::~CharacterTargetMarkerV28(){std::string ignored;release(ignored);}
bool CharacterTargetMarkerV28::initialize(std::string& error){
 error.clear();if(attempted_||!player_||!tables_){error="Required fresh SAME-player target indicator owner";return false;}attempted_=true;
 // The shipping names table lookup is ordered and case sensitive.
 const auto& names=tables_.set_names();
 const auto found=std::find(names.begin(),names.end(),"target_circle_00_chest");
 const std::int32_t base=found==names.end()?-1:std::int32_t(found-names.begin());
 for(std::int32_t i=0;i<9;++i){
  if(!effects_.grab_marker_v28(base+i,0,circles1494_[i],error))return false;
  if(circles1494_[i]){
   const auto& origin=world::canonical_vec3_origin_v1();
   // Complete source3b52f0..534c suffix, also necessary for warm receivers
   // whose preceding owner changed rotation or timeline looping.
   if(!effects_.marker_rotation_v70(circles1494_[i],origin.data(),error)||
      !effects_.marker_sync_v83(circles1494_[i],false,error)||
      !effects_.marker_visible_v28(circles1494_[i],false,error)||
      !effects_.marker_loop_v76(circles1494_[i],true,error))return false;
  }
 }
 initialized_=true;return true;
}
bool CharacterTargetMarkerV28::update(std::uintptr_t target,std::uintptr_t ooi,std::string& error){
 error.clear();if(!initialized_){error="Required initialized SAME-player target indicator";return false;}
 if(!target||target==player_)target=ooi;
 if(!target||target==player_){
  if(selected1498_>=0&&circles1494_[std::size_t(selected1498_)]){
   if(!effects_.marker_visible_v28(circles1494_[std::size_t(selected1498_)],false,error))return false;
   selected1498_=-1;
  }
  return true;
 }
 std::int32_t type=-1;
 if(!services_.interaction_type||!services_.interaction_type(services_.context,target,player_,type,error)){
  if(error.empty())error="Required target's actual GetInteractionType virtual";return false;
 }
 if(type<0)return true;
 // Shipping allocation has nine receivers. Refuse unsupported indices rather
 // than reproduce its unchecked memory access (e.g. a future revive producer).
 if(type>=9){error="Required target indicator interaction family "+std::to_string(type);return false;}
 if(type==1&&(!services_.item_tooltip||!services_.item_tooltip(services_.context,target,player_,error))){
  if(error.empty())error="Required SAME Item SetToolTip receiver";return false;
 }
 const auto marker=circles1494_[std::size_t(type)];if(!marker)return true;
 if(!effects_.marker_anchor_v28(marker,target,true,error))return false;
 if(type==selected1498_)return true;
 if(selected1498_>=0&&circles1494_[std::size_t(selected1498_)]&&!effects_.marker_visible_v28(circles1494_[std::size_t(selected1498_)],false,error))return false;
 if(!effects_.marker_visible_v28(marker,true,error))return false;
 selected1498_=type;return true;
}
bool CharacterTargetMarkerV28::release(std::string& error){
 error.clear();initialized_=false;
 for(auto& marker:circles1494_)if(marker&&!effects_.drop(marker,error))return false;
 selected1498_=-1;return true;
}
}
