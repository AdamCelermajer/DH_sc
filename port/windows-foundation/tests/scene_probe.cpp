#include "original_scene.hpp"
#include "assembled_level.hpp"
#include <fstream>
#include <iostream>

int main(int argc,char** argv) {
    if(argc<2) {std::cerr<<"Usage: scene_probe <original.bdae> [module-node] | --level <asset-root> <manifest-relative>\n";return 2;}
    dh::foundation::OriginalScene scene;
    std::string error;
    if(std::string(argv[1])=="--level") {
        if(argc!=4) return 2;
        dh::foundation::AssetCatalog assets(argv[2]);
        if(!dh::foundation::load_level(assets,argv[3],scene,error)) {std::cerr<<error<<'\n';return 1;}
    } else if(argc>2) {
        std::ifstream file(argv[1],std::ios::binary|std::ios::ate);
        if(!file) {std::cerr<<"Cannot open probe asset\n";return 2;}
        const auto size=file.tellg();
        if(size<=0) return 2;
        std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));file.seekg(0);
        if(!file.read(reinterpret_cast<char*>(bytes.data()),size)) return 2;
        const dh::foundation::Mat4 unit{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        if(!dh::foundation::decode_original_scene_module(bytes,argv[2],unit,scene,error)) {
            std::cerr<<error<<'\n';return 1;
        }
    } else if(!dh::foundation::load_original_scene(argv[1],scene,error)) {
        std::cerr<<error<<'\n';return 1;
    }
    std::cout<<"nodes="<<scene.nodeCount<<" instances="<<scene.instanceCount
             <<" vertices="<<scene.mesh.vertices.size()<<" triangles="<<scene.triangleCount
             <<" ranges="<<scene.mesh.ranges.size()<<"\nminimum="
             <<scene.minimum.x<<','<<scene.minimum.y<<','<<scene.minimum.z
             <<"\nmaximum="<<scene.maximum.x<<','<<scene.maximum.y<<','<<scene.maximum.z<<'\n';
    for(const auto& notice:scene.notices) std::cout<<notice<<'\n';
}
