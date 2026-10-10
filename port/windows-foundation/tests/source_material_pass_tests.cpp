#include "../source_material_pass.hpp"
#include "../asset_catalog.hpp"
#include "../original_character.hpp"
#include "../platform_win32.hpp"
#include "../actor_lighting.hpp"
#include "../render_queue.hpp"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>
using namespace dh::foundation;
void check(bool ok,const std::string& reason){if(!ok)throw std::runtime_error(reason);}
int red_pixel(Renderer& renderer,const Mesh& mesh){Camera camera;camera.eye={0,0,5};camera.target={0,0,0};
 renderer.beginFrame(camera);renderer.draw(mesh);renderer.endFrame();glFinish();unsigned char pixel[4]{};
 glReadBuffer(GL_BACK);glReadPixels(64,64,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel);check(glGetError()==GL_NO_ERROR,"GPU test GL error");return pixel[0];}
int main(int argc,char**argv){try{
 check(argc==2,"Expected shared assets root");AssetCatalog assets(argv[1]);const std::string model="original-cache/data/3d/characters/lizardman/lizardman.bdae";
 auto bytes=assets.read(model);dh2::resources::BresView view{};check(dh2_bres_open(&view,bytes.data(),bytes.size())==dh2::resources::BresError::ok,"Source BRES");std::string error;
 CommonMaterialPass diffuse,alpha,additive,unknown;
 check(resolve_common_material_pass(view,"diffuse",diffuse,error)==CommonMaterialPassResult::applied,error);
 check(diffuse.technique=="default"&&diffuse.vertexDefines.find("LIGHTING")==std::string::npos,"Source construction technique changed");
 check(!diffuse.state.blend&&diffuse.state.depthTest&&diffuse.state.depthWrite&&diffuse.state.cull&&!diffuse.state.alphaTest,"Source diffuse pass states");
 check(resolve_common_material_pass(view,"alpha",alpha,error)==CommonMaterialPassResult::applied,error);
 check(alpha.state.blend&&alpha.state.blendSource==GL_SRC_ALPHA&&alpha.state.blendDestination==GL_ONE_MINUS_SRC_ALPHA&&!alpha.state.depthWrite,"Source alpha states");
 check(resolve_common_material_pass(view,"additive",additive,error)==CommonMaterialPassResult::applied,error);
 check(additive.state.blend&&additive.state.blendSource==GL_ONE&&additive.state.blendDestination==GL_ONE&&!additive.state.depthWrite,"Source additive states");
 unknown.technique="retained";check(resolve_common_material_pass(view,"Material__38",unknown,error)==CommonMaterialPassResult::notCommon&&error.empty()&&unknown.technique=="retained","Unknown GLES family changed");
 CharacterVisual actor;CharacterVisualConfig config;config.model_path=model;check(actor.load(assets,config,error),error);
 check(actor.original_materials()[0].id=="diffuse"&&actor.original_materials()[0].technique=="default"&&actor.meshes()[0].ranges[0].material.sourcePass.has_value(),"Actor did not bind source pass");
 check(!actor.meshes()[0].ranges[0].material.lightingEnabled,"Actual COMMON default actor enabled invented lighting");
 check(classifySourceVertexLighting(diffuse.vertexShader,diffuse.vertexDefines)==SourceVertexLighting::CommonUnlit,"Actual source preamble not proven unlit");
 check(classifySourceVertexLighting(diffuse.vertexShader,"#ifdef UNKNOWN\n#define LIGHTING\n#endif\n")==SourceVertexLighting::Unsupported,"Conditional preamble guessed");
 check(classifySourceVertexLighting(diffuse.vertexShader,"#define LIGHTING\n")==SourceVertexLighting::CommonLit,"Known lit branch guessed unlit");
 // A real body-derived fixture removes only Color0 admission. Its source
 // shader/selected technique remains unchanged; absent VC must not add light.
 dh2::scene::Scene authored;check(dh2::scene::load(view,authored,error),error);dh2::assets::Mesh body{};bool foundBody=false;
 for(const auto& instance:authored.instances)if(instance.controller>=0){check(dh2_mesh_open(&body,&view,instance.geometry)==dh2::assets::Error::ok,"Body source geometry");foundBody=true;break;}
 check(foundBody,"Body skin instance absent");dh2::assets::Primitive bodyPrimitive{};check(dh2_mesh_primitive(&body,0,&bodyPrimitive)==dh2::assets::Error::ok&&bodyPrimitive.attributes[2]>=0,"Source body fixture lacks original color stream");
 bytes[body.buffers+14]=255;
 const auto fixtureDir=std::filesystem::temp_directory_path()/("dh-common-unlit-"+std::to_string(GetCurrentProcessId()));std::filesystem::create_directories(fixtureDir);
 const auto fixtureFile=fixtureDir/"without-color.bdae";{std::ofstream file(fixtureFile,std::ios::binary);file.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());check(bool(file),"Unlit fixture write");}
 AssetCatalog fixtureAssets(fixtureDir);CharacterVisual noColor;config.model_path="without-color.bdae";check(noColor.load(fixtureAssets,config,error),error);
 check(!noColor.meshes()[0].ranges[0].material.lightingEnabled&&noColor.original_materials()[0].technique=="default","COMMON default without vertex color enabled generic light");
 std::filesystem::remove(fixtureFile);std::filesystem::remove(fixtureDir);
 Window window;check(window.open("Source COMMON render pass test",128,128),window.error());Renderer renderer;check(renderer.initialize(128,128),"Renderer init");
 const std::uint8_t pixels[]{255,0,0,128,255,0,0,128,255,0,0,128,255,0,0,128};auto texture=renderer.createTexture(2,2,pixels);check(texture!=0,"Texture upload");
 Mesh mesh;mesh.vertices={{{-2,-2,0}},{{2,-2,0}},{{0,2,0}}};DrawRange range;range.indexCount=3;range.material.texture=texture;range.material.lightingEnabled=false;range.material.sourcePass=diffuse.state;mesh.ranges={range};
 mesh.vertices[0].color[3]=.25f;mesh.vertices[1].color[3]=.25f;mesh.vertices[2].color[3]=.25f;
 check(!renderer.isTransparent(mesh,mesh.ranges[0]),"Atlas/vertex alpha overrode opaque source pass");
 const int opaque=red_pixel(renderer,mesh);check(opaque>240,"Source opaque diffuse incorrectly alpha blended");
 mesh.ranges[0].material.sourcePass=additive.state;check(renderer.isTransparent(mesh,mesh.ranges[0]),"Source additive omitted transparent queue");
 const int additiveRed=red_pixel(renderer,mesh);check(additiveRed>240,"Source additive ONE/ONE incorrectly weighted by alpha");
 mesh.ranges[0].material.sourcePass=alpha.state;const int alphaRed=red_pixel(renderer,mesh);check(alphaRed>25&&alphaRed<90,"Source alpha blend factors not applied");
 mesh.ranges[0].material.sourcePass.reset();check(renderer.isTransparent(mesh,mesh.ranges[0]),"Unknown family fallback changed");
 auto changedState=diffuse.state;changedState.depthTest=false;changedState.depthWrite=false;changedState.depthFunction=GL_ALWAYS;changedState.frontFace=GL_CW;changedState.cullFace=GL_FRONT;changedState.alphaTest=true;
 mesh.ranges[0].material.sourcePass=changedState;renderer.draw(mesh);
 mesh.ranges[0].material.sourcePass.reset();renderer.draw(mesh);
 GLint state=0;glGetIntegerv(GL_DEPTH_FUNC,&state);check(state==GL_LEQUAL,"Source depth function leaked into fallback");
 check(glIsEnabled(GL_DEPTH_TEST)&&glIsEnabled(GL_CULL_FACE)&&!glIsEnabled(GL_ALPHA_TEST),"Source enable state leaked into fallback");
 glGetIntegerv(GL_FRONT_FACE,&state);check(state==GL_CCW,"Source winding leaked into fallback");glGetIntegerv(GL_CULL_FACE_MODE,&state);check(state==GL_BACK,"Source cull face leaked into fallback");
 GLboolean writes=GL_FALSE;glGetBooleanv(GL_DEPTH_WRITEMASK,&writes);check(writes==GL_TRUE,"Source depth mask leaked after draw");
 mesh.ranges[0].material.sourcePass=changedState;renderer.draw(mesh);Camera baseline;renderer.beginFrame(baseline);
 check(glIsEnabled(GL_DEPTH_TEST)&&!glIsEnabled(GL_ALPHA_TEST)&&!glIsEnabled(GL_BLEND),"Source state leaked across frames");
 glGetIntegerv(GL_DEPTH_FUNC,&state);check(state==GL_LEQUAL,"Source depth function leaked across frames");glGetIntegerv(GL_FRONT_FACE,&state);check(state==GL_CCW,"Source winding leaked across frames");
 // Bind caller textures onto immutable source mesh/range data. Actual GPU
 // output must change while preserving original opaque/alpha pass behavior.
 const std::uint8_t bluePixels[]{0,0,255,255,0,0,255,255,0,0,255,255,0,0,255,255};
 const auto blueTexture=renderer.createTexture(2,2,bluePixels);check(blueTexture!=0,"Override texture upload");
 mesh.ranges[0].material.sourcePass=diffuse.state;
 const auto* vertexBuffer=mesh.vertices.data();const auto* rangeBuffer=mesh.ranges.data();
 RenderQueue queue;queue.submit(mesh);
 check(!queue.submit(mesh,identity(),{},error)&&!error.empty()&&queue.size()==1,"Invalid overrides partially changed queue");queue.clear();
 Camera queueCamera;queueCamera.eye={0,0,5};queueCamera.target={0,0,0};
 const auto queuedBlue=[&](){renderer.beginFrame(queueCamera);check(queue.submit(mesh,identity(),{blueTexture},error),error);queue.flush(renderer,queueCamera);renderer.endFrame();glFinish();unsigned char pixel[4]{};glReadBuffer(GL_BACK);glReadPixels(64,64,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel);check(glGetError()==GL_NO_ERROR,"Override GPU readback");check(pixel[0]<10,"Original red texture remained active");return int(pixel[2]);};
 const int queuedOpaque=queuedBlue();check(queuedOpaque>240,"Queued texture override lost opaque source pass");
 mesh.ranges[0].material.sourcePass=alpha.state;const int queuedAlpha=queuedBlue();check(queuedAlpha>45&&queuedAlpha<90,"Override texture lost source alpha/vertex-alpha blend");
 check(mesh.vertices.data()==vertexBuffer&&mesh.ranges.data()==rangeBuffer&&mesh.ranges[0].material.texture==texture,"Queued override mutated source mesh/material");
 check(mesh.ranges[0].material.sourcePass->blendDestination==GL_ONE_MINUS_SRC_ALPHA&&!mesh.ranges[0].material.sourcePass->depthWrite,"Override modified source pass");
 check(queue.size()==0,"Override queue did not drain");
 mesh.ranges[0].material.sourcePass=diffuse.state;mesh.ranges.push_back(mesh.ranges[0]);
 check(!queue.submit(mesh,identity(),{blueTexture},error)&&queue.size()==0,"Multirange mismatch was accepted");
 renderer.beginFrame(queueCamera);check(queue.submit(mesh,identity(),{blueTexture,texture},error)&&queue.size()==2,error);
 queue.flush(renderer,queueCamera);renderer.endFrame();glFinish();unsigned char multiPixel[4]{};glReadBuffer(GL_BACK);glReadPixels(64,64,1,1,GL_RGBA,GL_UNSIGNED_BYTE,multiPixel);
 check(multiPixel[0]>240&&multiPixel[2]<10,"Per-range textures or opaque submission order changed");
 mesh.ranges.clear();renderer.beginFrame(queueCamera);
 check(queue.submit(mesh,identity(),{blueTexture},error)&&queue.size()==1,"Whole-mesh texture override rejected");
 queue.flush(renderer,queueCamera);renderer.endFrame();glFinish();unsigned char wholePixel[4]{};glReadBuffer(GL_BACK);glReadPixels(64,64,1,1,GL_RGBA,GL_UNSIGNED_BYTE,wholePixel);
 check(wholePixel[2]>25&&wholePixel[0]<10&&mesh.ranges.empty(),"Whole-mesh override failed or fabricated source ranges");
 renderer.destroyTexture(blueTexture);
 renderer.destroyTexture(texture);std::cout<<"PASS source COMMON diffuse/alpha/additive, actor binding, unknown GLES retention, GPU opaque="<<opaque<<" additive="<<additiveRed<<" alpha="<<alphaRed<<" queuedOverride="<<queuedOpaque<<'/'<<queuedAlpha<<"\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
