#include "gameplay_camera_factory_v16.hpp"
#include <cstring>
namespace dh2::camera {
namespace {float raw_float(std::uint32_t bits){float out;std::memcpy(&out,&bits,4);return out;}}
const std::array<CameraFactoryTypeV16,16>& source_camera_factory_catalog_v16()noexcept{
 static const std::array<CameraFactoryTypeV16,16> entries{{{0x65627563,"cube"},{0x72687073,"sphere"},{0x74786574,"text"},{0x72726574,"terrain"},{0x5f796b73,"skyBox"},{0x77646873,"shadowVolume"},{0x6873656d,"mesh"},{0x7468676c,"light"},{0x79746d65,"empty"},{0x796d6d64,"dummyTransformation"},{0x5f6d6163,"camera"},{0x6c6c6962,"billBoard"},{0x68736d61,"animatedMesh"},{0x6c637470,"particleSystem"},{0x4d6d6163,"cameraMaya"},{0x466d6163,"cameraFPS"}}};return entries;
}
struct CameraProceduralNodeV16::ParentLease {
 std::shared_ptr<CameraProceduralNodeV16> node;
 explicit ParentLease(std::shared_ptr<CameraProceduralNodeV16> value):node(std::move(value)){}
 ~ParentLease(){std::string error;if(!node->source_drop_leaf(error))node->lifetime_error_=std::move(error);}
};
CameraProceduralNodeV16::CameraProceduralNodeV16(std::shared_ptr<world::GameObjectSceneRootRegistryV1> manager,CameraFactoryBackendV16 backend):manager_(manager),backend_(std::move(backend)){
 // Exact DefaultFactory cam_ branch6ba660: id=-1, position=(0,0,0),
 // target=(0,0,100), input=false. Camera C1 refs1/culling0/empty children.
 scene::Node node;node.parent=-1;graph_.graph.push_back(std::move(node));graph_.nodes=1;
 view_.target={0,0,100};view_.up={0,1,0};view_.fov=raw_float(0x3fa0d97c);view_.aspect=raw_float(0x3faaaaab);view_.near_plane=1;view_.far_plane=3000;
}
bool CameraProceduralNodeV16::grab(std::string& e){if(!alive_||!lifetime_error_.empty()){e="Required live procedural camera grab: "+lifetime_error_;return false;}++references_;return true;}
bool CameraProceduralNodeV16::source_drop_leaf(std::string& e){if(!alive_||!references_){e="Invalid source procedural-camera reference drop";return false;}--references_;if(!references_){if(parentec_){e="Camera parent reference was not retained";lifetime_error_=e;return false;}alive_=false;animators_.clear();graph_=scene::Scene{};backend_={};}return true;}
bool CameraProceduralNodeV16::notify_visibility(bool parent,std::string& e){if(!alive_){e="Released actual camera visibility receiver";return false;}parent121_=parent;if(local120_&&parent)flags11c_|=1u;else flags11c_&=~1u;return true;}
bool CameraProceduralNodeV16::remove_animators(std::string& e){if(!alive_){e="Released actual camera animator-list receiver";return false;}animators_.clear();return true;}
bool CameraProceduralNodeV16::changed_manager(std::uintptr_t manager,std::string& e){
 manager110_=manager;if(!manager){view_.aspect=raw_float(0x3faaaaab);return true;}
 if(!backend_.configured_backend||!backend_.viewport){e="Required actual configured camera driver viewport";return false;}std::int32_t width,height;if(!backend_.viewport(width,height,e))return false;view_.aspect=static_cast<float>(width)/static_cast<float>(height);return true;
}
bool CameraProceduralNodeV16::add_to_root(std::string& e){
 if(!alive_||parent_attempted_){e="Required fresh camera root attachment";return false;}auto manager=manager_.lock();if(!manager){e="Released same source SceneManager";return false;}
 if(!grab(e))return false;auto lease=std::make_shared<ParentLease>(shared_from_this());world::GameObjectSceneRootBorrowV1 b;b.owner=lease;b.identity=identity();b.flags11c=&flags11c_;b.parentec=&parentec_;std::weak_ptr<CameraProceduralNodeV16> weak=shared_from_this();
 b.notify_visibility=[weak](bool visible,std::string& error){auto node=weak.lock();if(!node){error="Released source camera visibility";return false;}return node->notify_visibility(visible,error);};
 b.remove_animators=[weak](std::string& error){auto node=weak.lock();if(!node){error="Released source camera animator list";return false;}return node->remove_animators(error);};
 b.scene_manager_changed=[weak](std::uintptr_t id,std::string& error){auto node=weak.lock();if(!node){error="Released source camera scene-manager receiver";return false;}return node->changed_manager(id,error);};
 parent_attempted_=true;return manager->add_child(std::move(b),e);
}
bool CameraProceduralNodeV16::remove_from_root(std::string& e){auto manager=manager_.lock();if(!manager){e="Required live same camera root manager";return false;}if(!parentec_)return remove_animators(e);return manager->release_visual_root(identity(),e);}
bool CameraProceduralNodeV16::set_data(float fov,float aspect,float near,float far,std::string& e){if(!alive_){e="Released camera SetData";return false;}view_.fov=fov;view_.aspect=aspect;view_.near_plane=near;view_.far_plane=far;view_.up={0,0,1};return true;}
bool CameraProceduralNodeV16::set_planes(float near,float far,std::string& e){if(!alive_){e="Released camera near/far setters";return false;}view_.near_plane=near;view_.far_plane=far;return true;}
bool CameraProceduralNodeV16::set_position(const PointV2& p,std::string& e){if(!alive_){e="Released camera SetPosition";return false;}std::copy(p.begin(),p.end(),graph_.graph[0].translation);flags11c_|=0x40u;return scene::update_world(graph_,e);}
bool CameraProceduralNodeV16::set_target(const PointV2& p,std::string& e){if(!alive_){e="Released camera SetTarget";return false;}view_.target=p;return true;}
bool CameraProceduralNodeV16::set_up(const PointV2& p,std::string& e){if(!alive_){e="Released camera SetUpVector";return false;}view_.up=p;return true;}
bool CameraProceduralNodeV16::set_rotation(const std::array<float,4>& p,std::string& e){if(!alive_){e="Released camera SetRotation";return false;}std::copy(p.begin(),p.end(),graph_.graph[0].quaternion);flags11c_|=0x40u;return scene::update_world(graph_,e);}
bool CameraProceduralNodeV16::view(CameraViewV11& out,std::string& e){if(!alive_||!lifetime_error_.empty()){e="Released camera view: "+lifetime_error_;return false;}if(!scene::update_world(graph_,e))return false;for(unsigned i=0;i<3;++i)view_.eye[i]=graph_.graph[0].world[12+i];dh2_camera_look_at_v8(&view_.view,view_.eye.data(),view_.target.data(),view_.up.data());const float settings[4]{view_.fov,view_.aspect,view_.near_plane,view_.far_plane};dh2_camera_perspective_v8(&view_.projection,settings);out=view_;return true;}
world::GameObjectSceneCameraBorrowV13 CameraProceduralNodeV16::camera_borrow(){world::GameObjectSceneCameraBorrowV13 out;auto self=shared_from_this();out.owner=self;out.identity=identity();out.grab=[self](std::string& e){return self->grab(e);};out.drop=[self](std::string& e){return self->drop(e);};return out;}
bool CameraDefaultFactoryV16::create(std::uint32_t type,std::uintptr_t parent,std::shared_ptr<CameraProceduralNodeV16>& out,std::string& e){
 out.reset();bool known=false;for(const auto& record:source_camera_factory_catalog_v16())if(record.type==type){known=true;break;}if(!known)return true;
 if(type!=0x5f6d6163){e="Required actual selected non-cam_ scene factory constructor";return false;}
 if(parent){e="Required actual requested non-NULL factory parent AddChild";return false;}auto manager=manager14_.lock();if(!manager){e="Required same retained default SceneNodeFactory manager14";return false;}
 out=std::make_shared<CameraProceduralNodeV16>(manager,backend_);
 // cam_ creation makes this actual scene camera active before returning.
 // CameraBase process slot is deliberately not modified by the factory.
 return manager->set_active_camera_v13(out->camera_borrow(),e);
}
}
