#include "../../renderer.hpp"
#include "../../platform_win32.hpp"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <limits>
#include <cmath>
#include <stdexcept>
using namespace dh::foundation;
template<class T> static T read(std::ifstream& in) {
    T value{};in.read(reinterpret_cast<char*>(&value),sizeof(value));
    if(!in)throw std::runtime_error("Truncated actual source packet fixture");return value;
}
static void check(bool value,const char* error) { if(!value)throw std::runtime_error(error); }
struct Packet {Mesh mesh;Mat4 world;};
int main(int argc,char** argv) { try {
    check(argc==3,"actual source packet fixture and rendered PPM output required");
    std::ifstream in(argv[1],std::ios::binary);check(bool(in),"Missing original render fixture");
    check(read<std::uint32_t>(in)==0x46584631,"Actual packet fixture identity");
    Window window;check(window.open("DH2 original FX renderer stream test",320,240),"Native WGL context");
    Renderer renderer;check(renderer.initialize(320,240),"Root Renderer initialized");
    std::map<std::uint32_t,std::uint32_t> textures;auto texture_count=read<std::uint32_t>(in);
    check(texture_count<256,"Original texture count bound");
    for(unsigned t=0;t<texture_count;++t) {
        auto id=read<std::uint32_t>(in),width=read<std::uint32_t>(in),height=read<std::uint32_t>(in),size=read<std::uint32_t>(in);
        check(width&&height&&size==std::uint64_t(width)*height*4&&size<128*1024*1024,"Original RGBA texture layout");
        std::vector<std::uint8_t> image(size);in.read(reinterpret_cast<char*>(image.data()),size);check(bool(in),"Original texture pixels");
        auto handle=renderer.createTexture(width,height,image.data());check(handle!=0,"Original texture uploaded through root Renderer");
        textures.emplace(id,handle);
    }
    auto count=read<std::uint32_t>(in);check(count&&count<4096,"Original packet count");std::vector<Packet> packets;
    Vec3 minimum{std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()};
    Vec3 maximum{-minimum.x,-minimum.y,-minimum.z};std::uint64_t vertices=0,indices=0;
    for(unsigned p=0;p<count;++p) {
        Packet packet;auto v=read<std::uint32_t>(in),i=read<std::uint32_t>(in);
        check(v&&v<1000000&&i&&i<6000000&&i%3==0,"Original triangles");
        static_assert(sizeof(Vertex)==48);packet.mesh.vertices.resize(v);packet.mesh.indices.resize(i);
        in.read(reinterpret_cast<char*>(packet.mesh.vertices.data()),v*48);
        in.read(reinterpret_cast<char*>(packet.mesh.indices.data()),i*4);check(bool(in),"Original vertex/index streams");
        packet.world=read<Mat4>(in);DrawRange range;range.indexCount=i;range.material.color=read<std::array<float,4>>(in);
        auto texture=read<std::uint32_t>(in);range.material.texture=texture?textures.at(texture):0;
        range.material.alphaReference=read<float>(in);auto flags=read<std::uint32_t>(in);
        range.material.transparent=flags&1;range.material.doubleSided=flags&2;
        range.material.additive=flags&4;range.material.lightingEnabled=flags&8;
        SourceMaterialPass pass;pass.blendSource=read<std::uint32_t>(in);pass.blendDestination=read<std::uint32_t>(in);
        pass.depthFunction=read<std::uint32_t>(in);pass.cullFace=read<std::uint32_t>(in);pass.frontFace=read<std::uint32_t>(in);
        auto state=read<std::uint32_t>(in);pass.blend=state&1;pass.depthTest=state&2;pass.depthWrite=state&4;
        pass.cull=state&8;pass.alphaTest=state&16;range.material.sourcePass=pass;packet.mesh.ranges.push_back(range);
        for(auto index:packet.mesh.indices) {
            check(index<v,"Actual source index range");const auto& x=packet.mesh.vertices[index].position;const auto& m=packet.world;
            Vec3 world{m[0]*x.x+m[4]*x.y+m[8]*x.z+m[12],m[1]*x.x+m[5]*x.y+m[9]*x.z+m[13],m[2]*x.x+m[6]*x.y+m[10]*x.z+m[14]};
            minimum.x=std::min(minimum.x,world.x);minimum.y=std::min(minimum.y,world.y);minimum.z=std::min(minimum.z,world.z);
            maximum.x=std::max(maximum.x,world.x);maximum.y=std::max(maximum.y,world.y);maximum.z=std::max(maximum.z,world.z);
        }
        vertices+=v;indices+=i;packets.push_back(std::move(packet));
    }
    // Diagnostic framing changes only the camera, never original geometry.
    Camera camera;camera.target={(minimum.x+maximum.x)/2,(minimum.y+maximum.y)/2,(minimum.z+maximum.z)/2};
    float extent=std::max({maximum.x-minimum.x,maximum.y-minimum.y,maximum.z-minimum.z,1.f});
    camera.eye={camera.target.x,camera.target.y,camera.target.z+extent*2.5f};camera.up={0,1,0};
    camera.nearPlane=.1f;camera.farPlane=extent*20;renderer.beginFrame(camera);
    std::vector<std::uint8_t> before(320*240*4),after(before.size());glReadPixels(0,0,320,240,GL_RGBA,GL_UNSIGNED_BYTE,before.data());
    for(const auto& packet:packets)renderer.drawRange(packet.mesh,packet.mesh.ranges[0],packet.world);
    renderer.endFrame();glFinish();glReadPixels(0,0,320,240,GL_RGBA,GL_UNSIGNED_BYTE,after.data());
    check(glGetError()==GL_NO_ERROR,"Root Renderer GL state/stream upload");unsigned changed=0;
    for(std::size_t p=0;p<after.size();p+=4)if(before[p]!=after[p]||before[p+1]!=after[p+1]||before[p+2]!=after[p+2])++changed;
    check(changed>10,"Original mesh/particle pass changes actual framebuffer");
    std::ofstream ppm(argv[2],std::ios::binary);ppm<<"P6\n320 240\n255\n";
    for(int y=239;y>=0;--y)for(unsigned x=0;x<320;++x)ppm.write(reinterpret_cast<const char*>(after.data()+(y*320+x)*4),3);
    check(bool(ppm),"Original FX framebuffer receipt");for(const auto& texture:textures)renderer.destroyTexture(texture.second);
    std::cout<<"{\"validation\":\"PASS\",\"actual_packets\":"<<count<<",\"original_textures\":"<<texture_count
             <<",\"vertices\":"<<vertices<<",\"indices\":"<<indices<<",\"changed_pixels\":"<<changed
             <<",\"native_wgl_renderer\":true,\"campaign_live\":false}\n";
} catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;} }
