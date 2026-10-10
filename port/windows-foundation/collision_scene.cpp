#include "collision_scene.hpp"
#include "level_manifest.hpp"
#include "content_paths.hpp"
#include "../scene-materials/scene.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <map>
#include <stdexcept>

namespace dh::foundation {
namespace {
Vec3 sub(Vec3 a,Vec3 b){return {a.x-b.x,a.y-b.y,a.z-b.z};}
Vec3 cross(Vec3 a,Vec3 b){return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
float dot(Vec3 a,Vec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
bool finite(Vec3 p){return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z);}
Vec3 normal(const CollisionTriangle& t){auto n=cross(sub(t.b,t.a),sub(t.c,t.a));auto length=std::sqrt(dot(n,n));if(length>0){n.x/=length;n.y/=length;n.z/=length;}return n;}
Vec3 point(const Mat4& m,const float* p){return {m[0]*p[0]+m[4]*p[1]+m[8]*p[2]+m[12],m[1]*p[0]+m[5]*p[1]+m[9]*p[2]+m[13],m[2]*p[0]+m[6]*p[1]+m[10]*p[2]+m[14]};}
}

bool CollisionScene::floor(float x,float y,float referenceZ,FloorHit& hit,float maxStepUp,float maxDrop)const{
    if(!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(referenceZ)||!std::isfinite(maxStepUp)||!std::isfinite(maxDrop)||maxStepUp<0||maxDrop<0)return false;
    bool found=false;float closest=std::numeric_limits<float>::infinity();FloorHit selected;
    for(std::size_t i=0;i<triangles.size();++i){const auto& t=triangles[i];if(!t.floor)continue;
        const float bx=t.b.x-t.a.x,by=t.b.y-t.a.y,cx=t.c.x-t.a.x,cy=t.c.y-t.a.y;
        const double denominator=double(bx)*cy-double(by)*cx;
        if(std::abs(denominator)<1e-9)continue;
        const double dx=x-t.a.x,dy=y-t.a.y;
        const double u=(dx*cy-dy*cx)/denominator,v=(bx*dy-by*dx)/denominator;
        if(u < -1e-6||v < -1e-6||u+v>1.000001)continue;
        const float z=static_cast<float>(t.a.z+u*(t.b.z-t.a.z)+v*(t.c.z-t.a.z));
        if(z>referenceZ+maxStepUp||z<referenceZ-maxDrop)continue;
        const float distance=std::abs(z-referenceZ);
        if(distance>=closest)continue;
        auto n=normal(t);if(n.z<0){n.x=-n.x;n.y=-n.y;n.z=-n.z;}
        selected={{x,y,z},n,i};closest=distance;found=true;
    }
    if(found)hit=selected;return found;
}

bool CollisionScene::raycast(Vec3 from,Vec3 to,FloorHit& hit)const{
    if(!finite(from)||!finite(to))return false;
    const auto direction=sub(to,from);if(dot(direction,direction)<=1e-12f)return false;
    bool found=false;double closest=2;FloorHit selected;
    for(std::size_t i=0;i<triangles.size();++i){const auto& t=triangles[i];const auto e1=sub(t.b,t.a),e2=sub(t.c,t.a),p=cross(direction,e2);
        const double determinant=dot(e1,p);if(std::abs(determinant)<1e-9)continue;
        const auto distance=sub(from,t.a);const double u=dot(distance,p)/determinant;if(u<0||u>1)continue;
        const auto q=cross(distance,e1);const double v=dot(direction,q)/determinant;if(v<0||u+v>1)continue;
        const double along=dot(e2,q)/determinant;if(along<0||along>1||along>=closest)continue;
        auto n=normal(t);if(dot(n,direction)>0){n.x=-n.x;n.y=-n.y;n.z=-n.z;}
        selected={{from.x+float(along)*direction.x,from.y+float(along)*direction.y,from.z+float(along)*direction.z},n,i};closest=along;found=true;
    }
    if(found)hit=selected;return found;
}

bool decode_collision_module(const std::vector<std::uint8_t>& bytes,const std::string& authoredNode,const Mat4& placement,CollisionScene& output,std::string& error){
    try{
        for(auto v:placement)if(!std::isfinite(v))throw std::runtime_error("Nonfinite collision placement");
        dh2::resources::BresView view{};if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)throw std::runtime_error("Collision BRES rejected");
        // Physics/navigation helpers can be hidden from the visual scene.
        // Membership is determined by their exported role, not draw visibility.
        dh2::scene::Scene scene;dh2::scene::AuthoredVisibilityV76 visibility;
        if(!dh2::scene::load_authored_v76(view,scene,visibility,error))return false;
        std::int32_t selected=-1;
        if(!authoredNode.empty()){
            for(std::size_t n=0;n<scene.graph.size();++n){const auto& node=scene.graph[n];if(node.id==authoredNode||node.name==authoredNode||node.sid==authoredNode){if(selected>=0)throw std::runtime_error("Ambiguous collision module selector");selected=static_cast<std::int32_t>(n);}}
            if(selected<0)throw std::runtime_error("Collision module selector missing: "+authoredNode);
        }
        std::vector<Mat4> worlds(scene.graph.size());
        for(std::size_t n=0;n<scene.graph.size();++n){const auto& node=scene.graph[n];
            if(selected<0){worlds[n]=dh2::scene::multiply(placement,node.world);continue;}
            if(n==static_cast<std::size_t>(selected)){worlds[n]=placement;continue;}
            auto p=node.parent;while(p>=0&&p!=selected)p=scene.graph.at(p).parent;if(p!=selected)continue;
            Mat4 local{};dh2_node_matrix(local.data(),node.translation,node.quaternion,node.scale);worlds[n]=dh2::scene::multiply(worlds.at(node.parent),local);
        }
        CollisionScene result;
        for(const auto& instance:scene.instances){const auto& node=scene.graph.at(instance.node_index);
            if(selected>=0){auto p=static_cast<std::int32_t>(instance.node_index);while(p>=0&&p!=selected)p=scene.graph.at(p).parent;if(p!=selected)continue;}
            const auto& name=node.name.empty()?node.id:node.name;
            const bool isFloor=name.find("_floor")!=std::string::npos;
            const bool isCollider=name.find("_colbox_")!=std::string::npos;
            if(!isFloor&&!isCollider)continue;
            if(instance.controller>=0)throw std::runtime_error("Skinned collision helper is unsupported");
            dh2::assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&view,instance.geometry)!=dh2::assets::Error::ok)throw std::runtime_error("Collision helper geometry rejected");
            if(isFloor)++result.floorInstances;else ++result.collisionInstances;
            for(std::uint32_t p=0;p<mesh.primitives;++p){dh2::assets::Primitive primitive{};
                if(dh2_mesh_primitive(&mesh,p,&primitive)!=dh2::assets::Error::ok||primitive.collada_type||primitive.index_count%3)throw std::runtime_error("Collision helper is not triangle geometry");
                if(result.triangles.size()+primitive.index_count/3>1000000)throw std::runtime_error("Collision triangle budget exceeded");
                dh2::assets::Attribute positions{};if(dh2_mesh_attribute(&mesh,primitive.attributes[0],&positions)!=dh2::assets::Error::ok||positions.components<3)throw std::runtime_error("Collision helper position stream missing");
                for(std::uint32_t k=0;k<primitive.index_count;k+=3){CollisionTriangle triangle;triangle.floor=isFloor;Vec3* points[]{&triangle.a,&triangle.b,&triangle.c};
                    for(unsigned v=0;v<3;++v){std::uint32_t index=0;float values[4]{};
                        if(!dh2_index_read(&primitive,k+v,&index)||index>=mesh.vertices||!dh2_attribute_read(&positions,index,values))throw std::runtime_error("Collision triangle decode failed");
                        *points[v]=point(worlds.at(instance.node_index),values);if(!finite(*points[v]))throw std::runtime_error("Nonfinite collision triangle");}
                    const auto n=cross(sub(triangle.b,triangle.a),sub(triangle.c,triangle.a));if(dot(n,n)>1e-12f)result.triangles.push_back(triangle);
                }
            }
        }
        output=std::move(result);error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}

bool load_collision_level(AssetCatalog& assets,const std::filesystem::path& manifestRelative,CollisionScene& output,std::string& error){
    try{
        std::vector<ModulePlacement> modules;if(!decode_level_manifest(assets.read(manifestRelative),modules,error))return false;
        CollisionScene result;std::map<std::filesystem::path,std::vector<std::uint8_t>> cache;
        for(const auto& module:modules){if(!module.visible||!module.activateCondition.empty()){result.notices.push_back("Collision module skipped pending visibility/campaign state: "+module.name);continue;}
            const auto path=resolve_content_path(assets,module.assetPath,manifestRelative);auto entry=cache.find(path);if(entry==cache.end())entry=cache.emplace(path,read_content(assets,module.assetPath,manifestRelative)).first;
            CollisionScene part;if(!decode_collision_module(entry->second,module.authoredNode,module.placement,part,error)){error=module.name+": "+error;return false;}
            if(result.triangles.size()+part.triangles.size()>1000000)throw std::runtime_error("Level collision triangle budget exceeded");
            result.triangles.insert(result.triangles.end(),part.triangles.begin(),part.triangles.end());result.floorInstances+=part.floorInstances;result.collisionInstances+=part.collisionInstances;
        }
        if(result.floorInstances==0){error="Level has no original floor helper geometry";return false;}
        result.notices.push_back("Static module helpers only: dynamic actors, gameplay obstacles and floor-type movement rules require their own owners.");
        output=std::move(result);error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
}
