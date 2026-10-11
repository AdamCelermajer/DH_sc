#include "original_scene.hpp"
#include "assembled_level.hpp"
#include "../scene-materials/scene.hpp"
#include "../asset-payloads/payloads.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

namespace {
std::vector<std::uint8_t> read_all(const char* path) {
    std::ifstream file(path,std::ios::binary|std::ios::ate);
    if(!file) return {};
    const auto size=file.tellg();
    if(size<=0) return {};
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));file.seekg(0);
    if(!file.read(reinterpret_cast<char*>(bytes.data()),size)) return {};
    return bytes;
}
// B109 diagnostic: authored node graph (rest TRS) and animation channels (target, type, key range) of a BRES scene.
int dump(const std::vector<std::uint8_t>& bytes,bool nodes,bool anims) {
    dh2::resources::BresView view{};
    if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok) {std::cerr<<"BRES rejected\n";return 1;}
    dh2::scene::Scene authored;std::string error;
    if(!dh2::scene::load(view,authored,error)) {std::cerr<<error<<'\n';return 1;}
    if(nodes) {
        for(std::size_t n=0;n<authored.graph.size();++n) {
            const auto& node=authored.graph[n];
            std::cout<<"node["<<n<<"] parent="<<node.parent<<" id="<<node.id<<" name="<<node.name<<" sid="<<node.sid
                     <<" t="<<node.translation[0]<<','<<node.translation[1]<<','<<node.translation[2]
                     <<" q="<<node.quaternion[0]<<','<<node.quaternion[1]<<','<<node.quaternion[2]<<','<<node.quaternion[3]
                     <<" s="<<node.scale[0]<<','<<node.scale[1]<<','<<node.scale[2]<<'\n';
        }
        for(unsigned k=0;k<static_cast<unsigned>(dh2::resources::Library::count);++k)
            std::cout<<"library "<<dh2::resources::libraries[k].name<<" count="<<dh2_bres_library_count(&view,static_cast<dh2::resources::Library>(k))<<'\n';
        for(const auto& instance:authored.instances) {
            std::cout<<"instance node="<<instance.node<<" node_index="<<instance.node_index<<" geometry="<<instance.geometry
                     <<" controller="<<instance.controller<<" materials="<<instance.materials.size()<<'\n';
            dh2::assets::Mesh mesh{};
            if(dh2_mesh_open(&mesh,&view,instance.geometry)!=dh2::assets::Error::ok) {std::cout<<"  mesh open failed\n";continue;}
            std::cout<<"  mesh id="<<(mesh.id?mesh.id:"")<<" name="<<(mesh.name?mesh.name:"")<<" vertices="<<mesh.vertices<<" stride="<<mesh.stride
                     <<" attributes="<<mesh.attributes<<" primitives="<<mesh.primitives<<" buffers="<<mesh.buffers
                     <<" bounds="<<mesh.minimum[0]<<','<<mesh.minimum[1]<<','<<mesh.minimum[2]<<" .. "<<mesh.maximum[0]<<','<<mesh.maximum[1]<<','<<mesh.maximum[2]<<'\n';
            for(std::uint32_t j=0;j<mesh.primitives;++j) {
                dh2::assets::Primitive primitive{};
                if(dh2_mesh_primitive(&mesh,j,&primitive)!=dh2::assets::Error::ok) continue;
                std::cout<<"  primitive["<<j<<"] material="<<(primitive.material?primitive.material:"")<<" collada_type="<<primitive.collada_type
                         <<" engine_type="<<primitive.engine_type<<" indices="<<primitive.index_count<<" slots:";
                for(int slot=0;slot<18;++slot) {
                    if(primitive.attributes[slot]<0) continue;
                    dh2::assets::Attribute attribute{};
                    if(dh2_mesh_attribute(&mesh,primitive.attributes[slot],&attribute)!=dh2::assets::Error::ok) continue;
                    std::cout<<' '<<slot<<"=(type "<<attribute.type<<" x"<<attribute.components<<" stride "<<attribute.stride<<')';
                }
                std::cout<<'\n';
                // Decoded position extent of this primitive (local space), to compare with the authored mesh bounds.
                dh2::assets::Attribute positions{};
                if(dh2_mesh_attribute(&mesh,primitive.attributes[0],&positions)==dh2::assets::Error::ok&&positions.components>=3) {
                    float lo[3]{1e30f,1e30f,1e30f},hi[3]{-1e30f,-1e30f,-1e30f};
                    for(std::uint32_t v=0;v<mesh.vertices;++v) {float p[4]{};if(!dh2_attribute_read(&positions,v,p)) break;for(int c=0;c<3;++c){lo[c]=std::min(lo[c],p[c]);hi[c]=std::max(hi[c],p[c]);}}
                    std::cout<<"  decoded position extent="<<lo[0]<<','<<lo[1]<<','<<lo[2]<<" .. "<<hi[0]<<','<<hi[1]<<','<<hi[2]<<'\n';
                }
                // Distinct vertex colours (attribute 2) of this mesh: baked tint vs packed indices.
                dh2::assets::Attribute colours{};
                if(std::getenv("SCENE_PROBE_COLORS")&&dh2_mesh_attribute(&mesh,primitive.attributes[2],&colours)==dh2::assets::Error::ok) {
                    std::vector<std::array<int,4>> seen;
                    for(std::uint32_t v=0;v<mesh.vertices&&seen.size()<24;++v) {float c[4]{};if(!dh2_attribute_read(&colours,v,c)) break;std::array<int,4> k{int(c[0]),int(c[1]),int(c[2]),int(c[3])};if(std::find(seen.begin(),seen.end(),k)==seen.end())seen.push_back(k);}
                    std::cout<<"  colours:";for(const auto& k:seen)std::cout<<' '<<k[0]<<','<<k[1]<<','<<k[2]<<','<<k[3];std::cout<<'\n';
                }
            }
        }
    }
    if(anims) {
        const auto count=dh2_bres_library_count(&view,dh2::resources::Library::animation);
        std::cout<<"animations="<<count<<" segments="<<dh2_animation_segments(&view)<<'\n';
        for(std::uint32_t i=0;i<count;++i) {
            dh2::assets::Animation animation{};
            if(dh2_animation_open(&animation,&view,std::int32_t(i),0)!=dh2::assets::Error::ok) {std::cout<<"anim["<<i<<"] open failed\n";continue;}
            const char* target=dh2_animation_target(&animation);
            std::cout<<"anim["<<i<<"] target="<<(target?target:"<none>")<<" segment="<<animation.segment_start<<".."<<animation.segment_end
                     <<" channels="<<dh2_animation_channels(&animation)<<" samplers="<<dh2_animation_samplers(&animation)<<'\n';
            for(std::uint32_t c=0;c<dh2_animation_channels(&animation);++c) {
                dh2::assets::Vector values{};
                const bool ok=dh2_animation_vector(&animation,std::int32_t(c),true,&values);
                std::cout<<"  channel["<<c<<"] type="<<dh2_animation_type(&animation,std::int32_t(c))
                         <<" interp="<<dh2_animation_interpolation(&animation,std::int32_t(c))
                         <<" keys="<<(ok?values.count:0)<<" comps="<<(ok?values.components:0)<<" vtype="<<(ok?values.type:0);
                if(ok&&values.count) {
                    float first[4]{},last[4]{};
                    dh2_vector_read(&values,0,first);dh2_vector_read(&values,values.count-1,last);
                    std::cout<<" t="<<dh2_animation_key_time(&animation,std::int32_t(c),0)<<".."<<dh2_animation_key_time(&animation,std::int32_t(c),std::int32_t(values.count-1))
                             <<" first="<<first[0]<<','<<first[1]<<','<<first[2]<<','<<first[3]
                             <<" last="<<last[0]<<','<<last[1]<<','<<last[2]<<','<<last[3];
                }
                std::cout<<'\n';
            }
        }
    }
    return 0;
}
}

// B109 diagnostic: top-down (XY) orthographic plan of the decoded triangles, painter-sorted by height, shaded by height
// (dark = low, bright = high) with opaque ranges in grey and transparent ranges tinted red. Crop in world units.
void write_plan(const dh::foundation::OriginalScene& scene,const char* path,float x0,float y0,float x1,float y1,int px) {
    const int w=px,h=std::max(1,int(px*(y1-y0)/(x1-x0)));
    std::vector<std::uint8_t> image(std::size_t(w)*h*3,16);
    struct Tri{std::uint32_t a,b,c;float z;bool transparent;};
    std::vector<Tri> tris;
    const auto& m=scene.mesh;
    auto rangeOf=[&](std::size_t index){for(const auto& r:m.ranges)if(index>=r.firstIndex&&index<r.firstIndex+r.indexCount)return &r;return (const dh::foundation::DrawRange*)nullptr;};
    for(std::size_t k=0;k+2<m.indices.size();k+=3){const auto* r=rangeOf(k);Tri t{m.indices[k],m.indices[k+1],m.indices[k+2],0,r&&r->material.transparent};t.z=(m.vertices[t.a].position.z+m.vertices[t.b].position.z+m.vertices[t.c].position.z)/3;tris.push_back(t);}
    std::sort(tris.begin(),tris.end(),[](const Tri& a,const Tri& b){return a.z<b.z;});
    const float zlo=scene.minimum.z,zhi=std::max(scene.maximum.z,zlo+1);
    auto px_x=[&](float x){return (x-x0)/(x1-x0)*w;};auto px_y=[&](float y){return (y1-y)/(y1-y0)*h;};
    for(const auto& t:tris){
        const auto& A=m.vertices[t.a].position;const auto& B=m.vertices[t.b].position;const auto& C=m.vertices[t.c].position;
        float ax=px_x(A.x),ay=px_y(A.y),bx=px_x(B.x),by=px_y(B.y),cx=px_x(C.x),cy=px_y(C.y);
        const int minx=std::max(0,int(std::floor(std::min({ax,bx,cx})))),maxx=std::min(w-1,int(std::ceil(std::max({ax,bx,cx}))));
        const int miny=std::max(0,int(std::floor(std::min({ay,by,cy})))),maxy=std::min(h-1,int(std::ceil(std::max({ay,by,cy}))));
        if(minx>maxx||miny>maxy)continue;
        const float area=(bx-ax)*(cy-ay)-(cx-ax)*(by-ay);if(std::fabs(area)<1e-6f)continue;
        const float shade=40+200*(t.z-zlo)/(zhi-zlo);
        for(int y=miny;y<=maxy;++y)for(int x=minx;x<=maxx;++x){
            const float fx=x+.5f,fy=y+.5f;
            const float w0=((bx-fx)*(cy-fy)-(cx-fx)*(by-fy))/area,w1=((cx-fx)*(ay-fy)-(ax-fx)*(cy-fy))/area,w2=1-w0-w1;
            if(w0<0||w1<0||w2<0)continue;
            auto* p=&image[(std::size_t(y)*w+x)*3];
            p[0]=std::uint8_t(std::min(255.f,shade*(t.transparent?1.2f:1.f)));p[1]=std::uint8_t(shade*(t.transparent?.5f:1.f));p[2]=std::uint8_t(shade*(t.transparent?.5f:1.f));
        }
    }
    // Grid every 500 units (thin green), origin cross (bright green).
    for(float gx=std::ceil(x0/500)*500;gx<=x1;gx+=500){int x=int(px_x(gx));if(x<0||x>=w)continue;for(int y=0;y<h;++y){auto* p=&image[(std::size_t(y)*w+x)*3];p[1]=std::uint8_t(gx==0?255:90);}}
    for(float gy=std::ceil(y0/500)*500;gy<=y1;gy+=500){int y=int(px_y(gy));if(y<0||y>=h)continue;for(int x=0;x<w;++x){auto* p=&image[(std::size_t(y)*w+x)*3];p[1]=std::uint8_t(gy==0?255:90);}}
    std::ofstream out(path,std::ios::binary);out<<"P6\n"<<w<<' '<<h<<"\n255\n";out.write(reinterpret_cast<const char*>(image.data()),std::streamsize(image.size()));
    std::cout<<"plan written "<<path<<" ("<<w<<"x"<<h<<", "<<tris.size()<<" triangles, z "<<zlo<<".."<<zhi<<")\n";
}

int main(int argc,char** argv) {
    if(argc<2) {std::cerr<<"Usage: scene_probe [--plan <out.ppm> <x0> <y0> <x1> <y1> <px>] <original.bdae> [module-node] | --level <asset-root> <manifest-relative> | --nodes|--anims|--dump <original.bdae>\n";return 2;}
    dh::foundation::OriginalScene scene;
    std::string error;
    const char* planPath=nullptr;float plan[4]{};int planPx=1024;
    if(std::string(argv[1])=="--plan") {
        if(argc<9) return 2;
        planPath=argv[2];for(int i=0;i<4;++i)plan[i]=std::stof(argv[3+i]);planPx=std::stoi(argv[7]);
        argv+=7;argc-=7;
    }
    const std::string mode=argv[1];
    if(mode=="--nodes"||mode=="--anims"||mode=="--dump") {
        if(argc!=3) return 2;
        const auto bytes=read_all(argv[2]);
        if(bytes.empty()) {std::cerr<<"Cannot open probe asset\n";return 2;}
        return dump(bytes,mode!="--anims",mode!="--nodes");
    }
    if(mode=="--level") {
        if(argc!=4) return 2;
        dh::foundation::AssetCatalog assets(argv[2]);
        if(!dh::foundation::load_level(assets,argv[3],scene,error)) {std::cerr<<error<<'\n';return 1;}
    } else if(argc>2) {
        const auto bytes=read_all(argv[1]);
        if(bytes.empty()) {std::cerr<<"Cannot open probe asset\n";return 2;}
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
    if(std::getenv("SCENE_PROBE_MATERIALS")) {
        // B109 diagnostic: per draw range, the authored material, technique, texture matrix and the decoded UV extent.
        for(std::size_t r=0;r<scene.mesh.ranges.size()&&r<scene.materials.size();++r) {
            const auto& range=scene.mesh.ranges[r];const auto& m=scene.materials[r];
            float ulo=1e30f,uhi=-1e30f,vlo=1e30f,vhi=-1e30f;
            for(std::size_t k=range.firstIndex;k<range.firstIndex+range.indexCount;++k){const auto& v=scene.mesh.vertices[scene.mesh.indices[k]];ulo=std::min(ulo,v.u);uhi=std::max(uhi,v.u);vlo=std::min(vlo,v.v);vhi=std::max(vhi,v.v);}
            std::cout<<"range["<<r<<"] tris="<<range.indexCount/3<<" material="<<m.id<<" diffuse="<<m.diffuse<<" alpha="<<m.alphaMap<<" effect="<<m.effectFile
                     <<" technique="<<m.technique<<" lit="<<range.material.levelLit<<" transparent="<<range.material.transparent<<" additive="<<range.material.additive
                     <<" tm=["<<m.textureMatrix[0]<<','<<m.textureMatrix[1]<<','<<m.textureMatrix[4]<<','<<m.textureMatrix[5]<<" t "<<m.textureMatrix[8]<<','<<m.textureMatrix[9]<<"]"
                     <<" uv=["<<ulo<<".."<<uhi<<" x "<<vlo<<".."<<vhi<<"]\n";
        }
    }
    if(planPath) write_plan(scene,planPath,plan[0],plan[1],plan[2],plan[3],planPx);
}
