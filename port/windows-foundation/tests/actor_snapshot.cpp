#include "../original_character.hpp"
#include "../asset_catalog.hpp"
#include "../content_paths.hpp"
#include "../texture_loader.hpp"
#include "../platform_win32.hpp"
#include "../render_queue.hpp"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace dh::foundation;
int main(int argc,char**argv){try{
    if(argc!=7 && argc!=8)throw std::runtime_error("usage: actor_snapshot model-assets texture-assets model template idle capture.ppm [camera-heading-degrees]");
    AssetCatalog modelAssets(argv[1]),textureAssets(argv[2]);CharacterVisualConfig config;config.model_path=argv[3];config.template_clip_path=argv[4];config.clips={{"preview",argv[5]}};config.allow_missing_animation_targets=true;
    CharacterVisual actor;std::string error;if(!actor.load(modelAssets,config,error)||!actor.update(.1,error))throw std::runtime_error(error);
    Window window;if(!window.open("Original actor material inspection",640,480))throw std::runtime_error(window.error());Renderer renderer;if(!renderer.initialize(640,480))throw std::runtime_error("renderer");
    std::vector<std::uint32_t> handles;
    for(std::size_t i=0;i<actor.mutable_meshes().size();++i){const auto& material=actor.original_materials()[i];TextureImage image;if(!load_texture(resolve_content_path(textureAssets,material.diffuse),image,error))throw std::runtime_error(error);auto handle=renderer.createTexture(image.width,image.height,image.rgba.data());if(!handle)throw std::runtime_error("texture upload");handles.push_back(handle);for(auto& range:actor.mutable_meshes()[i].ranges)range.material.texture=handle;}
    Vec3 lo{INFINITY,INFINITY,INFINITY},hi{-INFINITY,-INFINITY,-INFINITY};for(const auto& mesh:actor.meshes())for(const auto& v:mesh.vertices){lo.x=std::min(lo.x,v.position.x);lo.y=std::min(lo.y,v.position.y);lo.z=std::min(lo.z,v.position.z);hi.x=std::max(hi.x,v.position.x);hi.y=std::max(hi.y,v.position.y);hi.z=std::max(hi.z,v.position.z);}
    Camera camera;camera.target={(lo.x+hi.x)/2,(lo.y+hi.y)/2,(lo.z+hi.z)/2};const auto extent=std::max({hi.x-lo.x,hi.y-lo.y,hi.z-lo.z,1.f});camera.up={0,0,1};camera.eye={camera.target.x+extent*.5f,camera.target.y-extent*1.4f,camera.target.z+extent*.3f};camera.farPlane=10000;
    if(argc==8){const auto angle=std::stod(argv[7])*3.141592653589793/180;camera.eye={camera.target.x+float(std::sin(angle))*extent*1.4f,camera.target.y-float(std::cos(angle))*extent*1.4f,camera.target.z+extent*.3f};}
    renderer.beginFrame(camera);RenderQueue queue;for(const auto& mesh:actor.meshes())queue.submit(mesh);queue.flush(renderer,camera);renderer.endFrame();
    std::vector<unsigned char> pixels(640*480*3);glPixelStorei(GL_PACK_ALIGNMENT,1);glReadBuffer(GL_BACK);glReadPixels(0,0,640,480,GL_RGB,GL_UNSIGNED_BYTE,pixels.data());std::ofstream output(argv[6],std::ios::binary);output<<"P6\n640 480\n255\n";for(int y=479;y>=0;--y)output.write(reinterpret_cast<const char*>(pixels.data()+y*640*3),640*3);if(!output)throw std::runtime_error("capture write");window.swap();for(auto handle:handles)renderer.destroyTexture(handle);std::cout<<"captured "<<argv[6]<<"\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
