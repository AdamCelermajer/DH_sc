#include "../collision_scene.hpp"
#include <iostream>
#include <limits>
#include <cmath>
int main(int argc,char**argv){try{
    if(argc!=3&&argc!=6)return 2;dh::foundation::AssetCatalog assets(argv[1]);dh::foundation::CollisionScene scene;std::string e;
    if(!dh::foundation::load_collision_level(assets,argv[2],scene,e)){std::cerr<<e;return 1;}
    std::cout<<"triangles="<<scene.triangles.size()<<'\n';
    float minX=1e30f,minY=1e30f,minZ=1e30f,maxX=-1e30f,maxY=-1e30f,maxZ=-1e30f;
    for(const auto& t:scene.triangles)for(auto v:{t.a,t.b,t.c}){minX=std::min(minX,v.x);minY=std::min(minY,v.y);minZ=std::min(minZ,v.z);maxX=std::max(maxX,v.x);maxY=std::max(maxY,v.y);maxZ=std::max(maxZ,v.z);}
    std::cout<<"bounds="<<minX<<','<<minY<<','<<minZ<<"->"<<maxX<<','<<maxY<<','<<maxZ<<'\n';
    if(argc==6){const float x=std::stof(argv[3]),y=std::stof(argv[4]),z=std::stof(argv[5]);dh::foundation::FloorHit hit;const bool ok=scene.floor(x,y,z,hit,100000,100000);std::cout<<"query="<<x<<','<<y<<','<<z<<" floor="<<ok;if(ok)std::cout<<" z="<<hit.point.z<<" tri="<<hit.triangle;std::cout<<'\n';return 0;}
    for(float x:{1030.f,1090.75f,1150.f,793.9f})for(float y:{-272.2f,-212.202f,-152.2f,-617.658f}){
        dh::foundation::FloorHit hit;bool ok=scene.floor(x,y,258,hit,100000,100000);
        std::cout<<x<<','<<y<<" floor="<<ok;if(ok)std::cout<<" z="<<hit.point.z<<" normal="<<hit.normal.x<<','<<hit.normal.y<<','<<hit.normal.z<<" tri="<<hit.triangle;std::cout<<'\n';
    }
}catch(const std::exception&e){std::cerr<<e.what();return 1;}}
