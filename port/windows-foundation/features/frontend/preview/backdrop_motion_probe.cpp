#include "../../../asset_catalog.hpp"
#include "../../../content_paths.hpp"
#include "../../../original_scene.hpp"
#include "../../../camera.hpp"
#include "class_preview_scene.hpp"
#include "../../../../engine-animation/animation.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>
int main(int argc,char**argv){if(argc!=2)return 2;try{
    dh::foundation::AssetCatalog assets(argv[1]);std::string error;
    auto bytes=dh::foundation::read_content(assets,"models/class_selection.bdae");dh2::resources::BresView view{};
    if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)throw std::runtime_error("BRES");
    dh2::scene::Scene scene;dh2::animation::Player animation;
    if(!dh2::scene::load(view,scene,error)||!animation.load(bytes.data(),bytes.size(),scene,error))throw std::runtime_error(error);
    const auto rest=scene.instances;
    dh::foundation::OriginalScene decoded;
    if(!dh::foundation::decode_original_scene(bytes,decoded,error))throw std::runtime_error(error);
    std::cout<<"SCENE geometry instances "<<scene.instances.size()<<" decoded "<<decoded.instanceCount<<" ranges "<<decoded.mesh.ranges.size()<<" triangles "<<decoded.triangleCount<<" ignored "<<scene.ignored_instances<<" tracks "<<animation.track_count()<<" skipped "<<animation.skipped<<'\n';
    for(int time:{0,650,1299,1333,1983,2633,2666,3316,4000,4033,4683,5333,6000,6333,6666}){
        if(!animation.sample(scene,time,error))throw std::runtime_error(error);
        for(std::size_t i=0;i<scene.instances.size();++i){const auto& current=scene.instances[i];float delta=0;
            for(unsigned k=0;k<16;++k)delta=std::max(delta,std::abs(current.world[k]-rest[i].world[k]));
            std::cout<<"INSTANCE ms "<<time<<" name "<<current.node<<" node "<<current.node_index<<" geometry "<<current.geometry<<" worldDelta "<<delta<<" position "<<current.world[12]<<' '<<current.world[13]<<' '<<current.world[14]<<'\n';
        }
    }
    for(const auto&note:decoded.notices)std::cout<<"DECODER_NOTICE "<<note<<'\n';
    dh::foundation::frontend::ClassPreviewScene preview;
    if(!preview.load(assets,error)||!preview.sample(1,1600,error)||!preview.sample(1,0,error))throw std::runtime_error(error);
    const auto& camera=preview.camera();dh::foundation::CameraPose pose;
    pose.position={camera.eye.x,camera.eye.y,camera.eye.z};pose.target={camera.target.x,camera.target.y,camera.target.z};pose.up={camera.up.x,camera.up.y,camera.up.z};
    const auto matrix=dh2::scene::multiply(dh::foundation::cameraProjectionMatrix(camera.verticalFovDegrees,camera.aspectRatio,camera.nearPlane,camera.farPlane),dh::foundation::cameraViewMatrix(pose));
    auto projected=[&](const dh::foundation::Vec3&p){std::array<float,4> out{};
        for(unsigned k=0;k<4;++k)out[k]=matrix[k]*p.x+matrix[4+k]*p.y+matrix[8+k]*p.z+matrix[12+k];return out;};
    auto clipped=[&](const auto&a,const auto&b,const auto&c){std::vector<std::array<float,4>> polygon{a,b,c};
        for(unsigned plane=0;plane<6&&!polygon.empty();++plane){
            const unsigned axis=plane/2;const float sign=plane%2?-1.f:1.f;
            auto distance=[&](const auto&p){return p[3]+sign*p[axis];};
            std::vector<std::array<float,4>> next;auto previous=polygon.back();float previousDistance=distance(previous);
            for(const auto&current:polygon){const float currentDistance=distance(current);
                if((previousDistance>=0)!=(currentDistance>=0)){const float t=previousDistance/(previousDistance-currentDistance);std::array<float,4> intersection;
                    for(unsigned k=0;k<4;++k)intersection[k]=previous[k]+t*(current[k]-previous[k]);next.push_back(intersection);}
                if(currentDistance>=0)next.push_back(current);previous=current;previousDistance=currentDistance;
            }
            polygon=std::move(next);
        }
        for(auto&p:polygon)for(unsigned k=0;k<3;++k)p[k]/=p[3];return polygon;
    };
    auto edge=[](const auto&a,const auto&b,float x,float y){return (b[0]-a[0])*(y-a[1])-(b[1]-a[1])*(x-a[0]);};
    for(auto pixel:{std::array<int,2>{8,8},std::array<int,2>{32,8},std::array<int,2>{8,32},std::array<int,2>{48,32},std::array<int,2>{120,8}}){
        const float x=2*(pixel[0]+.5f)/960-1,y=2*(pixel[1]+.5f)/540-1;unsigned total=0,front=0;
        for(unsigned r=0;r<decoded.mesh.ranges.size();++r){const auto&range=decoded.mesh.ranges[r];unsigned hits=0;
            for(std::size_t i=range.firstIndex;i<range.firstIndex+range.indexCount;i+=3){
                const auto a=projected(decoded.mesh.vertices[decoded.mesh.indices[i]].position),b=projected(decoded.mesh.vertices[decoded.mesh.indices[i+1]].position),c=projected(decoded.mesh.vertices[decoded.mesh.indices[i+2]].position);
                const auto polygon=clipped(a,b,c);
                for(unsigned fan=1;fan+1<polygon.size();++fan){const auto&a=polygon[0];const auto&b=polygon[fan];const auto&c=polygon[fan+1];
                    const auto e1=edge(a,b,x,y),e2=edge(b,c,x,y),e3=edge(c,a,x,y);
                    if((e1>=0&&e2>=0&&e3>=0)||(e1<=0&&e2<=0&&e3<=0)){++hits;++total;if(edge(a,b,c[0],c[1])>0)++front;}
                }
            }
            std::cout<<"COVERAGE pixel "<<pixel[0]<<' '<<pixel[1]<<" range "<<r<<" material "<<decoded.materials[r].id<<" triangles "<<hits<<'\n';
        }
        std::cout<<"COVERAGE_TOTAL pixel "<<pixel[0]<<' '<<pixel[1]<<" all "<<total<<" CCW "<<front<<'\n';
    }
    return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
