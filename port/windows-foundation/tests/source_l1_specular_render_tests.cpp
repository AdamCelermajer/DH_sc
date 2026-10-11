#include "../platform_win32.hpp"
#include "../source_l1_specular.hpp"
#include "../frame_perf.hpp"
#include <GL/gl.h>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool okay,const char* message){if(!okay)throw std::runtime_error(message);}
int main(){try{
    Window window;check(window.open("B114 private source specular probe",160,160),"window failed");Renderer renderer;renderer.setGlLoader(&Window::gl_proc);check(renderer.initialize(160,160),"renderer failed");renderer.setClearColor(0,0,0);
    const std::uint8_t black[4]{0,0,0,255},mask[4]{255,192,64,255};
    const auto diffuse=renderer.createTexture(1,1,black),specular=renderer.createTexture(1,1,mask);
    Mesh mesh;for(auto p:{Vec3{-8,-8,0},Vec3{8,-8,0},Vec3{8,8,0},Vec3{-8,8,0}}){Vertex v;v.position=p;v.normal={0,0,1};v.color={.1f,.2f,.3f,1};mesh.vertices.push_back(v);}mesh.indices={0,1,2,0,2,3};
    DrawRange range;range.indexCount=6;range.material.texture=diffuse;range.material.specularTexture=specular;range.material.glossiness=10;range.material.levelLit=true;range.material.doubleSided=true;range.material.lightingEnabled=false;mesh.ranges={range};
    Camera camera;camera.eye={0,0,3132};camera.target={0,0,0};camera.up={0,1,0};camera.verticalFovDegrees=1;camera.nearPlane=900;camera.farPlane=4200;
    LevelPointLight light;light.enabled=true;light.position={0,0,200};light.attenuation={.265625f,0,.132813f/1000000};light.specular={1,.5f,.25f};
    Mat4 placement=identity();
    const auto verify=[&](const char* label){renderer.setLevelLight(light);renderer.beginFrame(camera);renderer.draw(mesh,placement);glFinish();std::uint8_t pixel[4];glReadPixels(80,80,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel);std::array<float,4> source{};for(const unsigned v:{0u,2u}){const auto& input=mesh.vertices[v];const auto p=input.position;const Vec3 world{placement[0]*p.x+placement[4]*p.y+placement[8]*p.z+placement[12],placement[1]*p.x+placement[5]*p.y+placement[9]*p.z+placement[13],placement[2]*p.x+placement[6]*p.y+placement[10]*p.z+placement[14]};Vec3 normal;std::array<float,4> sample;check(source_l1_world_normal(placement,input.normal,normal)&&source_l1_specular(light,world,normal,camera.eye,mesh.ranges[0].material.glossiness,sample),"source expectation failed");for(unsigned i=0;i<3;++i)source[i]+=.5f*sample[i];}for(unsigned i=0;i<3;++i){const int expected=int(std::lround(std::clamp(source[i]*64.f/255,0.f,1.f)*255));if(std::abs(int(pixel[i])-expected)>3)throw std::runtime_error(std::string(label)+" channel="+std::to_string(i)+" actual="+std::to_string(pixel[i])+" expected="+std::to_string(expected));}check(glGetError()==GL_NO_ERROR,"GL error");std::cout<<label<<" pixel="<<int(pixel[0])<<','<<int(pixel[1])<<','<<int(pixel[2])<<'\n';};
    verify("source high attenuation blue-only colored light");
    light.position.z=1000;verify("same authored light at farther distance");light.position.z=200;
    light.attenuation={4,0,0};verify("source low attenuation control");
    light.specular={0,0,0};verify("zero light rejects white fallback");
    light.specular={1,.5f,.25f};light.attenuation={.265625f,0,.132813f/1000000};mesh.ranges[0].material.glossiness=256;for(auto& v:mesh.vertices)v.normal={0,0,.99f};verify("source exponent256 and unnormalized normal");
    placement[0]=2;placement[5]=3;placement[12]=40;placement[13]=-20;camera.eye={100,0,3132};camera.target={40,-20,0};light.position={40,-20,200};verify("source transformed mesh and off-axis eye");
    placement=identity();camera.eye={0,0,3132};camera.target={0,0,0};light.position={0,0,200};mesh.ranges[0].material.glossiness=10;for(auto& v:mesh.vertices)v.normal={0,0,1};
    mesh.staticGeometry=true;light.attenuation={.265625f,0,.132813f/1000000};light.specular={1,.5f,.25f};const auto beforeVbo=perf::FramePerf::get().counters().vboDraws;verify("static VBO same source transport");check(perf::FramePerf::get().counters().vboDraws>beforeVbo,"static test fell back from VBO");
    const auto rejected=[&](const char* label){renderer.setLevelLight(light);renderer.beginFrame(camera);renderer.draw(mesh);glFinish();std::uint8_t pixel[4];glReadPixels(80,80,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel);check(pixel[0]==0&&pixel[1]==0&&pixel[2]==0,label);};
    light.enabled=false;rejected("disabled light admitted specular");light.enabled=true;
    mesh.ranges[0].material.glossiness=0;rejected("missing Sp admitted specular");mesh.ranges[0].material.glossiness=10;
    mesh.ranges[0].material.color[3]=.5f;rejected("blended range admitted opaque specular");mesh.ranges[0].material.color[3]=1;
    mesh.ranges[0].material.alphaReference=.5f;rejected("alpha-tested range changed admission");mesh.ranges[0].material.alphaReference=0;
    SourceMaterialPass pass;pass.cull=false;mesh.ranges[0].material.sourcePass=pass;rejected("common source pass admitted L1 specular");mesh.ranges[0].material.sourcePass.reset();
    const std::uint8_t white[4]{255,255,255,255};const auto whiteTexture=renderer.createTexture(1,1,white);
    Mesh plain=mesh;plain.staticGeometry=false;plain.ranges[0].material={};plain.ranges[0].material.texture=whiteTexture;plain.ranges[0].material.doubleSided=true;plain.ranges[0].material.lightingEnabled=false;
    renderer.beginFrame(camera);renderer.draw(mesh);renderer.draw(plain);glFinish();std::uint8_t pixel[4];glReadPixels(80,80,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel);check(std::abs(int(pixel[0])-26)<=1&&std::abs(int(pixel[1])-51)<=1&&std::abs(int(pixel[2])-77)<=1,"L1 program leaked into following unlit material");
    check(renderer.withViewport(0,0,160,160,camera,[&]{renderer.draw(plain);}),"viewport failed");glFinish();glReadPixels(80,80,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel);check(std::abs(int(pixel[0])-26)<=1&&std::abs(int(pixel[1])-51)<=1,"specular changed independent viewport");renderer.destroyTexture(whiteTexture);
    GLint program=1;glGetIntegerv(0x8B8D,&program);check(program==0,"specular program leaked");
    renderer.invalidateStaticGeometry();renderer.destroyTexture(diffuse);renderer.destroyTexture(specular);
    std::cout<<"PASS actual production Renderer high/low/zero/VBO specular blue and authored color, negative gates, following unlit and viewport GL state\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
