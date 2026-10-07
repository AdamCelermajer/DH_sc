#include "authored_camera_basis_v22.hpp"
namespace dh2::camera {
bool borrow_authored_camera_basis_v22(std::shared_ptr<GameplayCameraSceneV3> scene,std::uint32_t index,
 std::function<bool(float&,std::string&)> fov,const PointV2* zero,const PointV2* up,CameraBasisServicesV21&out,std::string&e){
 if(!scene||index>=scene->cameras().size()||!fov||!zero||!up){e="Required actual authored camera instance/FOV/source globals";return false;}
 const auto& c=scene->cameras()[index];if(c.node>=scene->graph().graph.size()){e="Invalid actual authored camera parent";return false;}
 CameraBasisServicesV21 s;s.owner=scene;s.camera_present=true;s.source_vec3_zero=zero;s.source_vec3_up=up;s.camera_fov=std::move(fov);
 s.absolute_camera_matrix=[scene,index](MatrixV8&m,std::string&error){if(index>=scene->cameras().size()){error="Released same authored camera instance";return false;}const auto& child=scene->cameras()[index];if(child.node>=scene->graph().graph.size()){error="Released actual camera parent transform";return false;}const auto& parent=scene->graph().graph[child.node].world;for(unsigned i=0;i<16;++i)m.values[i]=parent[i];PointV2 eye,target;if(!scene->eye_and_target(index,eye.data(),target.data(),error))return false;for(unsigned i=0;i<3;++i)m.values[12+i]=eye[i];m.identity_flag=0;return true;};
 s.absolute_camera_position=[scene,index](PointV2&p,std::string&error){PointV2 target;return scene->eye_and_target(index,p.data(),target.data(),error);};
 out=std::move(s);return true;
}
}
