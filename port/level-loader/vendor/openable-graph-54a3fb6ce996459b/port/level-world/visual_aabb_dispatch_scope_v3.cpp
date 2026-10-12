#include "visual_aabb_dispatch_scope_v3.hpp"
namespace dh2::world {
namespace {thread_local const VisualAabbDispatchV3* active;}
VisualAabbDispatchScopeV3::VisualAabbDispatchScopeV3(VisualAabbDispatchV3 b):previous_(active),binding_(b){active=&binding_;}
VisualAabbDispatchScopeV3::~VisualAabbDispatchScopeV3(){active=previous_;}
bool visual_aabb_dispatch_v3(CanonicalGameObjectBaseOwnerV1& base,const float* bounds,bool flat,bool& handled,std::string& e){
 handled=active&&active->receiver==&base;if(!handled)return true;
 if(!active->apply){e="Required selected Visual ApplyMeshBox virtual9c";return false;}
 return active->apply(active->context,bounds,flat,e);
}
bool item_relative_aabb_v3(CanonicalGameObjectBaseOwnerV1& b,bool flat,std::string&){
 auto* bounds=b.relative_aabb144();if(!flat){for(unsigned i:{0u,1u,3u,4u})bounds[i]=bounds[i]*1.5f;}
 b.update_absolute_aabb();return true;
}
}
