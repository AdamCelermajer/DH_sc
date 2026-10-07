#include "world_item_visual_v2.hpp"
#include "base_index_animation_controller_v2.hpp"
#include "visual_aabb_dispatch_scope_v3.hpp"
#include <stdexcept>
namespace dh2::character {
namespace {world::CanonicalGameObjectBaseOwnerV1& base(const std::shared_ptr<RetainedWorldItemObjectV1>& p){if(!p)throw std::invalid_argument("Required same retained canonical Item");return p->base();}}
world::GameObjectInitializationServicesV1 WorldItemVisualV2::bind(world::GameObjectInitializationServicesV1 s){
 s.load_visual=[this](std::string& e){return assets_.load_visual(e);};
 s.visual_sync=[this](std::uintptr_t id,std::string& e){auto v=registry_.lookup(id);if(!v){e="Required same Item visual for Sync";return false;}return v->sync(e);};
 s.visual_root=[this](std::uintptr_t id,std::uintptr_t& out,std::string& e){auto v=registry_.lookup(id);if(!v){e="Required same Item visual root";return false;}out=v->root_identity();return true;};
 s.node_from_name=[this](std::uintptr_t root,const char* name,std::uintptr_t& out,std::string& e){auto v=registry_.lookup_root(root);if(!v){e="Required same Item root node lookup";return false;}return v->node_from_name(name,out,e);};
 s.visual_set_light_set=[this](std::uintptr_t id,std::int32_t light,std::string& e){auto v=registry_.lookup(id);if(!v){e="Required same Item visual light set";return false;}v->store_light_set(light);return true;};
 return s;
}
WorldItemVisualV2::WorldItemVisualV2(std::shared_ptr<RetainedWorldItemObjectV1> item,world::RetainedGameObjectVisualServicesV1 visual,world::GameObjectInitializationServicesV1 init)
 :item_(std::move(item)),registry_(base(item_),std::move(visual)),assets_(base(item_),registry_.services(item_)),initialization_(base(item_),bind(std::move(init))){}
bool WorldItemVisualV2::init_post(std::string& e){world::VisualAabbDispatchScopeV3 dispatch({&item_->base(),&item_->base(),[](void* p,const float*,bool flat,std::string& error){return world::item_relative_aabb_v3(*static_cast<world::CanonicalGameObjectBaseOwnerV1*>(p),flat,error);}});bool eligible=false;return initialization_.init_post(eligible,e);}
bool WorldItemVisualV2::init_final_v23(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
bool WorldItemVisualV2::present(bool& out,std::string& e)const{
 const auto* pointer=item_->base().pointer(0x2d8);if(!pointer){e="Required same Item visual2d8 storage";return false;}
 if(!*pointer){out=false;return true;}auto v=registry_.lookup(*pointer);if(!v){e="Item visual2d8 is outside its retained resource registry";return false;}out=true;return true;
}
bool WorldItemVisualV2::apply_mesh_box(std::string& e){auto v=visual();if(!v){e="Required attached Item visual ApplyMeshBox";return false;}return world::item_relative_aabb_v3(item_->base(),v->marker().found!=0,e);}
bool WorldItemVisualV2::init_again_clip(std::string& e){auto v=visual();if(!v){e="Required attached Item InitAgain controller";return false;}if(!v->root_animator_present())return true; // source GetAnim(0)==NULL -> PlayClip false, ignored by caller
 auto borrow=v->named_animation(v);bool accepted=false;return world::base_index_animation_play_v2(borrow,0,false,accepted,e);}
bool WorldItemVisualV2::init_again_source(const ItemColorLookupServicesV2& s,std::string& e){auto* item=item_->inventory().peek();if(!item){e="Required same transferred Item GetColor receiver";return false;}std::uint32_t discarded=0;if(!item_color_lookup_v2(*item,s,discarded,e))return false;return init_again_clip(e);}
bool WorldItemVisualV2::update(std::uint32_t time,std::string& e){auto v=visual();return !v||v->update(time,e);}
bool WorldItemVisualV2::update_v3(std::uint32_t time,const std::function<bool(std::string&)>& notify,std::string& e){auto v=visual();return !v||(v->commit_root_visibility_v3(notify,e)&&v->update(time,e));}
bool WorldItemVisualV2::release(std::string& e){if(!assets_.set_visual(std::uintptr_t{0},e))return false;return registry_.discard_unattached(e);}
}
