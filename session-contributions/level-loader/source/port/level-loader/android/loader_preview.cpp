// Isolated inspection renderer. Uses retained native loading and authored
// textures/material data; preview camera/shader are adapter choices, not a
// reconstruction claim for original lighting, objects, conditions or gameplay.
#include "../level_preparation_v1.hpp"
#include "../static_decor_inspection_v1.hpp"
#include "../../engine-textures/textures.hpp"
#include "../../engine-skinning/skinning.hpp"
#include <cstdlib>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <jni.h>
#include <unistd.h>
#include <algorithm>
#include <array>
#include <cerrno>
#include <cmath>
#include <cstddef>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using Matrix=std::array<float,16>;
using Point=std::array<float,3>;
constexpr const char* tag="DH2Loader";
struct Descriptor {
    int fd{-1};off64_t base{};std::uint64_t bytes{};
    ~Descriptor(){if(fd>=0)close(fd);}
    bool read(std::uint64_t at,void* dst,std::size_t count,std::string& error)const {
        if(at>bytes||count>bytes-at||at>std::uint64_t(std::numeric_limits<off64_t>::max()-base)){
            error="Cache read outside descriptor";return false;
        }
        std::size_t done=0;
        while(done<count){const auto got=pread64(fd,static_cast<char*>(dst)+done,count-done,base+at+done);
            if(got<0&&errno==EINTR)continue;
            if(got<=0){error="Cache descriptor short read";return false;}
            done+=std::size_t(got);
        }return true;
    }
};
struct Vertex {float p[3]{},uv[2]{},color[4]{1,1,1,1};};
struct Batch {GLuint vertices{},indices{},diffuse{},alpha{};GLsizei count{};Matrix world{};dh2::scene::Material material;};
struct Bounds {Point low{INFINITY,INFINITY,INFINITY},high{-INFINITY,-INFINITY,-INFINITY};
    void add(const Point& p){for(unsigned j=0;j<3;++j){low[j]=std::min(low[j],p[j]);high[j]=std::max(high[j],p[j]);}}
};
struct Candidate {
    dh2::loader::LevelPreparationV1::Borrow prepared;
    dh2::loader::FixedMapV1::Borrow map;
    dh2::loader::StaticDecorInspectionV1 decors;
    std::vector<Batch> batches;std::vector<GLuint> textures;
    Bounds bounds;std::vector<Bounds> module_bounds;
    unsigned instances{},triangles{},unclassified{},decor_instances{},skipped_decor_meshes{};
    std::vector<Bounds> entity_bounds;std::vector<std::string> entity_names;unsigned characters{},chests{},blocked_entities{},nonvisual_entities{};std::string next_constructor;
    bool procedural{};std::uint32_t seed{};
    ~Candidate(){for(auto& b:batches){glDeleteBuffers(1,&b.vertices);glDeleteBuffers(1,&b.indices);}for(auto t:textures)glDeleteTextures(1,&t);}
};
dh2::assets::ZipAssetPackV1 pack;
AAssetManager* preview_assets{};
std::unique_ptr<Candidate> active;
GLuint program{};int width=1,height=1;
GLint a_position{},a_uv{},a_color{},u_mvp{},u_texture{},u_color{},u_alpha{},u_ref{};
Point center{};float radius=1,yaw=-1.57f,pitch=1.1f,zoom=1;
bool reported=false,failed=false;
void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
void check(const char* operation){const auto error=glGetError();if(error!=GL_NO_ERROR)throw std::runtime_error(std::string(operation)+" GL error "+std::to_string(error));}
std::string string(JNIEnv* env,jstring s){require(s!=nullptr,"Missing text argument");const char* raw=env->GetStringUTFChars(s,nullptr);require(raw!=nullptr,"JNI text unavailable");std::string out(raw);env->ReleaseStringUTFChars(s,raw);return out;}
jstring result(JNIEnv* env,const std::string& s){__android_log_print(ANDROID_LOG_INFO,tag,"%s",s.c_str());return env->NewStringUTF(s.c_str());}
GLuint shader(GLenum type,const char* source){GLuint s=glCreateShader(type);glShaderSource(s,1,&source,nullptr);glCompileShader(s);GLint ok=0;glGetShaderiv(s,GL_COMPILE_STATUS,&ok);
    if(!ok){char log[1024]{};glGetShaderInfoLog(s,sizeof(log),nullptr,log);glDeleteShader(s);throw std::runtime_error(log);}return s;
}
void create_program(){
    constexpr const char* vertex=R"(attribute vec3 position;attribute vec2 texcoord;attribute vec4 color;
uniform mat4 mvp;uniform mat4 texture_matrix;varying vec2 uv;varying vec4 tint;
void main(){gl_Position=mvp*vec4(position,1.0);uv=(texture_matrix*vec4(texcoord,0.0,1.0)).xy;tint=color;})";
    constexpr const char* fragment=R"(precision mediump float;uniform sampler2D diffuse;uniform sampler2D alpha_map;
uniform vec4 material_color;uniform float has_alpha;uniform float alpha_ref;varying vec2 uv;varying vec4 tint;
void main(){vec4 c=texture2D(diffuse,uv)*tint*material_color;if(has_alpha>0.5)c.a*=texture2D(alpha_map,uv).r;
if(c.a<=max(alpha_ref,0.0039))discard;gl_FragColor=c;})";
    GLuint vs=shader(GL_VERTEX_SHADER,vertex),fs=shader(GL_FRAGMENT_SHADER,fragment);
    program=glCreateProgram();glAttachShader(program,vs);glAttachShader(program,fs);glLinkProgram(program);glDeleteShader(vs);glDeleteShader(fs);
    GLint ok=0;glGetProgramiv(program,GL_LINK_STATUS,&ok);require(ok,"Preview shader link failed");
    a_position=glGetAttribLocation(program,"position");a_uv=glGetAttribLocation(program,"texcoord");a_color=glGetAttribLocation(program,"color");
    u_mvp=glGetUniformLocation(program,"mvp");u_texture=glGetUniformLocation(program,"texture_matrix");u_color=glGetUniformLocation(program,"material_color");
    u_alpha=glGetUniformLocation(program,"has_alpha");u_ref=glGetUniformLocation(program,"alpha_ref");
    glUseProgram(program);glUniform1i(glGetUniformLocation(program,"diffuse"),0);glUniform1i(glGetUniformLocation(program,"alpha_map"),1);check("program");
}
void mount(AAssetManager* manager){
    auto* asset=AAssetManager_open(manager,"dh2-original-cache.zip",AASSET_MODE_RANDOM);require(asset,"Canonical cache missing from APK");
    auto owner=std::make_shared<Descriptor>();off64_t length{};owner->fd=AAsset_openFileDescriptor64(asset,&owner->base,&length);AAsset_close(asset);
    require(owner->fd>=0&&owner->base>=0&&length>0,"Cache must be stored uncompressed in APK");owner->bytes=length;
    dh2::assets::ZipBackingV1 backing;backing.owner=owner;backing.bytes=owner->bytes;
    backing.read=[owner](std::uint64_t at,void* dst,std::size_t n,std::string& e){return owner->read(at,dst,n,e);};
    std::string error;require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
    require(pack.entries().size()==6833,"Canonical cache directory count differs");
}
GLuint upload(const std::string& name,Candidate& next,std::map<std::string,GLuint>& cache){
    auto old=cache.find(name);if(old!=cache.end())return old->second;
    std::vector<std::uint8_t> rgba{255,255,255,255};unsigned w=1,h=1;
    if(!name.empty()){
        auto uri="data/3d/textures/"+name;std::transform(uri.begin(),uri.end(),uri.begin(),[](unsigned char c){return c>='A'&&c<='Z'?char(c+32):char(c);});
        bool found=false;std::vector<std::uint8_t> bytes;std::string error;require(pack.read(uri,found,bytes,error),error);
        // The supplied Android cache stores several alpha images only under
        // its pvr2_ filename. An explicit inspection alias keeps the authored
        // material intact and never overrides an existing authored resource.
        if(!found) {
            const auto slash=uri.find_last_of('/');const auto alternate=uri.substr(0,slash+1)+"pvr2_"+uri.substr(slash+1);
            require(pack.read(alternate,found,bytes,error),error);
            if(found) {
                __android_log_print(ANDROID_LOG_INFO,tag,"TEXTURE_SOURCE_ALIAS authored=%s resolved=%s",uri.c_str(),alternate.c_str());
                uri=alternate;
            }
        }
        require(found,"Texture missing: "+uri);
        dh2::textures::View view{};auto code=dh2_texture_open(bytes.data(),bytes.size(),&view);require(code==dh2::textures::Error::ok,"Texture header: "+uri);
        w=view.width;h=view.height;require(w<=8192&&h<=8192,"Texture size exceeds preview limit");rgba.resize(std::size_t(w)*h*4);
        require(dh2_texture_decode(&view,rgba.data(),rgba.size())==dh2::textures::Error::ok,"Texture decode: "+uri);
    }
    GLint limit=0;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&limit);require(w<=unsigned(limit)&&h<=unsigned(limit),"Texture exceeds GPU limit");
    GLuint texture=0;glGenTextures(1,&texture);next.textures.push_back(texture);glBindTexture(GL_TEXTURE_2D,texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.data());check("texture upload");
    cache[name]=texture;return texture;
}
#include "visible_entity_renderer_v30.inc"
void set_bounds(const Bounds& b){
    Point next_center{};float length=0;
    for(unsigned j=0;j<3;++j){require(std::isfinite(b.low[j])&&std::isfinite(b.high[j]),"Preview has no finite bounds");next_center[j]=(b.low[j]+b.high[j])*.5f;length+=(b.high[j]-b.low[j])*(b.high[j]-b.low[j]);}
    const float next_radius=std::sqrt(length)*.5f;require(std::isfinite(next_radius)&&next_radius>0,"Empty preview bounds");
    center=next_center;radius=next_radius;zoom=1;
}
std::string load(const std::string& identity,const std::string& definition,std::uint32_t seed){
    std::string error;
    const bool procedural=definition.size()>=9&&definition.compare(definition.size()-9,9,".rule.xml")==0;
    using namespace dh2::loader;
    LevelPreparationV1 preparation(pack);
    require(preparation.begin({identity,definition,procedural?LevelSourceKindV1::procedural:LevelSourceKindV1::fixed,seed},error),error);
    for(;;) {
        __android_log_print(ANDROID_LOG_INFO,tag,"SOURCE_PREPARATION_STAGE identity=%s stage=%s",identity.c_str(),level_preparation_stage_v1(preparation.stage()));
        const auto step=preparation.step();
        if(step==LevelPreparationStepV1::source_ready)break;
        if(step!=LevelPreparationStepV1::pending) {
            const auto cause=preparation.error();preparation.discard();throw std::runtime_error(cause);
        }
    }
    auto next=std::make_unique<Candidate>();next->prepared=preparation.latest_source();next->map=next->prepared.map();
    next->procedural=bool(next->prepared.procedural_modules());next->seed=seed;
    if(next->prepared.resolution().backup_used)__android_log_print(ANDROID_LOG_INFO,tag,
        "SOURCE_BACKUP_SELECTED identity=%s seed=%u definition=%s",identity.c_str(),seed,next->prepared.resolution().definition.c_str());
    if(next->prepared.procedural_modules())for(const auto& repair:next->prepared.procedural_modules()->reference_repairs)
        __android_log_print(ANDROID_LOG_INFO,tag,"SOURCE_REFERENCE_REPAIR identity=%s tile=%u property=%s authored=%s resolved=%s",
            identity.c_str(),repair.tile,repair.property.c_str(),repair.authored_uri.c_str(),repair.resolved_uri.c_str());
    require(prepare_static_decor_inspection_v1(pack,next->prepared.declarations(),next->decors,error),error);
    __android_log_print(ANDROID_LOG_INFO,tag,"STATIC_DECOR_INSPECTION identity=%s sources=%zu skipped=%zu runtime_objects=0 animation=0",
        identity.c_str(),next->decors.declarations.size(),next->decors.skipped.size());
    next->module_bounds.resize(next->map.modules().size());std::map<std::string,GLuint> images;
    auto append_geometry=[&](const FixedMapV1::Borrow& map,bool decor){
    for(const auto& item:map.instances()){
        if(item.kind==dh2::loader::MapGeometryKindV1::unclassified)++next->unclassified;
        // Floors/exits/minimap/root helpers remain retained but are inspection
        // metadata. Unclassified meshes remain explicit and are not presumed
        // to have the same original visibility policy as map mesh nodes.
        if(item.kind!=dh2::loader::MapGeometryKindV1::mesh)continue;
        if(!map.modules().at(item.module).authored_visible)continue;
        const auto& instance=map.scene().instances.at(item.scene_instance);const auto& asset=map.assets().at(item.asset);
        if(decor&&instance.controller>=0){++next->skipped_decor_meshes;continue;}
        require(instance.controller<0,"Skinned map mesh requires a gameplay renderer");
        ++next->instances;if(decor)++next->decor_instances;
        dh2::assets::Mesh mesh{};require(dh2_mesh_open(&mesh,&asset.view,instance.geometry)==dh2::assets::Error::ok,"Map mesh rejected");
        require(mesh.primitives==instance.materials.size(),"Map material binding count differs");
        for(unsigned p=0;p<mesh.primitives;++p){
            dh2::assets::Primitive primitive{};require(dh2_mesh_primitive(&mesh,p,&primitive)==dh2::assets::Error::ok,"Map primitive rejected");
            require(primitive.collada_type==0&&primitive.index_count%3==0&&mesh.vertices<=65536,"Preview unsupported topology");
            dh2::assets::Attribute position{},uv{},color{};require(dh2_mesh_attribute(&mesh,primitive.attributes[0],&position)==dh2::assets::Error::ok&&position.components>=3,"Map position missing");
            bool have_uv=dh2_mesh_attribute(&mesh,primitive.attributes[4],&uv)==dh2::assets::Error::ok&&uv.components>=2;
            bool have_color=dh2_mesh_attribute(&mesh,primitive.attributes[2],&color)==dh2::assets::Error::ok;
            std::vector<Vertex> vertices(mesh.vertices);
            for(unsigned k=0;k<mesh.vertices;++k){auto& v=vertices[k];float raw[4]{};require(dh2_attribute_read(&position,k,raw),"Position read failed");std::copy(raw,raw+3,v.p);
                Point placed{};for(unsigned r=0;r<3;++r){placed[r]=instance.world[12+r];for(unsigned c=0;c<3;++c)placed[r]+=instance.world[c*4+r]*v.p[c];require(std::isfinite(placed[r]),"Nonfinite placed vertex");}
                next->bounds.add(placed);
                const auto parent=decor?next->decors.declarations.at(item.module).module:item.module;
                if(parent!=no_source_v1)next->module_bounds.at(parent).add(placed);
                if(have_uv){require(dh2_attribute_read(&uv,k,raw),"UV read failed");std::copy(raw,raw+2,v.uv);}
                if(have_color){require(color.components<=4&&dh2_attribute_read(&color,k,raw),"Color read failed");for(unsigned j=0;j<color.components;++j)v.color[j]=raw[j]/(color.type==1?255.f:1.f);}
            }
            std::vector<std::uint16_t> indices(primitive.index_count);
            for(unsigned k=0;k<primitive.index_count;++k){std::uint32_t index{};require(dh2_index_read(&primitive,k,&index)&&index<mesh.vertices,"Index read/range failed");indices[k]=std::uint16_t(index);}
            next->batches.emplace_back();auto& b=next->batches.back();b.world=instance.world;b.material=map.scene().materials.at(instance.materials[p]);
            // The serialized instance binding selects its target material URI.
            // A primitive symbol need not equal that target's material ID (the
            // original lighthouse asset binds ColorMaterial to suffixed IDs).
            // Keep the resolved authored binding; never guess a material by symbol.
            if(b.material.id!=primitive.material)__android_log_print(ANDROID_LOG_INFO,tag,"MATERIAL_TARGET_ALIAS symbol=%s target=%s",primitive.material,b.material.id.c_str());
            b.diffuse=upload(b.material.diffuse,*next,images);b.alpha=upload(b.material.alpha_map,*next,images);
            b.count=primitive.index_count;next->triangles+=primitive.index_count/3;
            glGenBuffers(1,&b.vertices);glBindBuffer(GL_ARRAY_BUFFER,b.vertices);glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),GL_STATIC_DRAW);
            glGenBuffers(1,&b.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,b.indices);glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(std::uint16_t),indices.data(),GL_STATIC_DRAW);check("map buffers");
        }
    }
    };
    append_geometry(next->map,false);
    if(next->decors.map)append_geometry(next->decors.map,true);
    append_authored_entities(preview_assets,identity,*next,images);
    set_bounds(next->bounds);active=std::move(next);reported=false;failed=false;
    return identity+(procedural?" seed "+std::to_string(seed):"")+
        (active->prepared.resolution().backup_used?" [original backup]":"")+
        (active->prepared.procedural_modules()&&!active->prepared.procedural_modules()->reference_repairs.empty()?" [source references repaired]":"")+" | "+std::to_string(active->map.modules().size())+" modules | "+std::to_string(active->instances)+" mesh instances | "+std::to_string(active->batches.size())+" draws | "+std::to_string(active->textures.size())+" textures\nAuthored inspection: "+std::to_string(active->characters)+" characters, "+std::to_string(active->chests)+" chests; "+std::to_string(active->blocked_entities)+" visuals blocked; next constructor "+(active->next_constructor.empty()?"none reported":active->next_constructor)+". Gameplay pending. Unclassified geometry: "+std::to_string(active->unclassified)+
        (active->decors.declarations.empty()?"":" | Static authored decor: "+std::to_string(active->decors.declarations.size()))+
        (active->decors.skipped.empty()&&active->skipped_decor_meshes==0?"":" | Decor providers pending: "+std::to_string(active->decors.skipped.size()+active->skipped_decor_meshes));
}
Point cross(const Point& a,const Point& b){return {a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]};}
float dot(const Point& a,const Point& b){return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];}
void normalize(Point& p){float d=std::sqrt(dot(p,p));require(d>0,"Camera vector degenerate");for(auto& x:p)x/=d;}
Matrix camera(){const float aspect=float(width)/height,tangent=.41421356f;
    const float distance=radius*1.1f*std::sqrt(1+1/(tangent*tangent*std::min(1.f,aspect)*std::min(1.f,aspect)))*zoom;
    Point eye{center[0]+distance*std::cos(yaw)*std::cos(pitch),center[1]+distance*std::sin(yaw)*std::cos(pitch),center[2]+distance*std::sin(pitch)};
    Point f{center[0]-eye[0],center[1]-eye[1],center[2]-eye[2]};normalize(f);auto s=cross(f,{0,0,1});normalize(s);auto u=cross(s,f);
    Matrix view{s[0],u[0],-f[0],0,s[1],u[1],-f[1],0,s[2],u[2],-f[2],0,-dot(s,eye),-dot(u,eye),dot(f,eye),1};
    float near=std::max(1.f,distance-radius*1.5f),far=distance+radius*3;
    Matrix projection{1/(tangent*aspect),0,0,0,0,1/tangent,0,0,0,0,-(far+near)/(far-near),-1,0,0,-2*far*near/(far-near),0};return dh2::scene::multiply(projection,view);
}
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_LoaderPreviewActivity_initialize(JNIEnv* env,jclass,jobject manager,jstring identity,jstring definition,jint seed){
    // A new context invalidates all old names. Abandon handles before deleting
    // CPU owners so destructors cannot delete reused IDs in the new context.
    if(active){for(auto& b:active->batches){b.vertices=0;b.indices=0;}active->textures.clear();active.reset();}program=0;
    try{preview_assets=AAssetManager_fromJava(env,manager);mount(preview_assets);create_program();return result(env,load(string(env,identity),string(env,definition),std::uint32_t(seed)));}
    catch(const std::exception& e){failed=true;return result(env,std::string("Preparation failed: ")+e.what());}
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_LoaderPreviewActivity_reload(JNIEnv* env,jclass,jstring identity,jstring definition,jint seed){
    reported=false;
    try{return result(env,load(string(env,identity),string(env,definition),std::uint32_t(seed)));}
    catch(const std::exception& e){return result(env,std::string("Preparation failed; previous map retained: ")+e.what());}
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_LoaderPreviewActivity_resize(JNIEnv*,jclass,jint w,jint h){width=std::max(1,int(w));height=std::max(1,int(h));glViewport(0,0,width,height);reported=false;}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_LoaderPreviewActivity_orbit(JNIEnv*,jclass,jfloat dx,jfloat dy,jfloat z){if(!std::isfinite(dx)||!std::isfinite(dy)||!std::isfinite(z)||z<=0)return;yaw+=dx;pitch=std::clamp(pitch+dy,.1f,1.5f);zoom=std::clamp(zoom*z,.1f,5.f);}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_LoaderPreviewActivity_focus(JNIEnv* env,jclass,jint module){
    try{require(bool(active),"No active map");if(module<0){set_bounds(active->bounds);return result(env,"Whole map | "+active->map.sources().identity()+" | mobs/chests pending");}
        auto index=unsigned(module)%active->module_bounds.size();set_bounds(active->module_bounds.at(index));return result(env,"Module "+std::to_string(index)+" | "+active->map.modules().at(index).authored_name+" | mobs/chests pending");}
    catch(const std::exception& e){return result(env,e.what());}
}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_LoaderPreviewActivity_draw(JNIEnv*,jclass){
    glClearColor(.07f,.085f,.07f,1);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);if(!active||failed)return;
    try{const auto vp=camera();glUseProgram(program);glEnable(GL_DEPTH_TEST);glEnable(GL_BLEND);glEnableVertexAttribArray(a_position);glEnableVertexAttribArray(a_uv);glEnableVertexAttribArray(a_color);
        for(const auto& b:active->batches){auto matrix=dh2::scene::multiply(vp,b.world);glUniformMatrix4fv(u_mvp,1,GL_FALSE,matrix.data());
            b.material.backface?glEnable(GL_CULL_FACE):glDisable(GL_CULL_FACE);glCullFace(GL_BACK);glFrontFace(GL_CCW);
            glBlendFunc(GL_SRC_ALPHA,b.material.additive?GL_ONE:GL_ONE_MINUS_SRC_ALPHA);glDepthMask(b.material.additive?GL_FALSE:GL_TRUE);
            glUniformMatrix4fv(u_texture,1,GL_FALSE,b.material.texture_matrix);glUniform4fv(u_color,1,b.material.color);glUniform1f(u_alpha,b.material.alpha_map.empty()?0:1);glUniform1f(u_ref,b.material.alpha_ref);
            glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,b.diffuse);glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,b.alpha);
            glBindBuffer(GL_ARRAY_BUFFER,b.vertices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,b.indices);
            glVertexAttribPointer(a_position,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,p)));
            glVertexAttribPointer(a_uv,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,uv)));
            glVertexAttribPointer(a_color,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,color)));glDrawElements(GL_TRIANGLES,b.count,GL_UNSIGNED_SHORT,nullptr);
        }
        glDepthMask(GL_TRUE);glDisable(GL_BLEND);glDisable(GL_CULL_FACE);glDisable(GL_DEPTH_TEST);check("frame");
        if(!reported){__android_log_print(ANDROID_LOG_INFO,tag,"MAP_FRAME_OK identity=%s procedural=%d seed=%u modules=%zu meshes=%u draws=%zu triangles=%u textures=%zu viewport=%dx%d gameplay=0 active_objects=0 inspection_entities=%zu inspection_characters=%u inspection_chests=%u blocked_visuals=%u authored_nonvisual=%u static_decor_sources=%zu decor_meshes=%u decor_skipped=%zu",active->map.sources().identity().c_str(),active->procedural,active->seed,active->map.modules().size(),active->instances,active->batches.size(),active->triangles,active->textures.size(),width,height,active->entity_bounds.size(),active->characters,active->chests,active->blocked_entities,active->nonvisual_entities,active->decors.declarations.size(),active->decor_instances,active->decors.skipped.size()+active->skipped_decor_meshes);reported=true;}
    }catch(const std::exception& e){failed=true;__android_log_print(ANDROID_LOG_ERROR,tag,"FRAME_FAILED %s",e.what());}
}

extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_LoaderPreviewActivity_focusObject(JNIEnv* env,jclass,jint object){
 try{require(active&&!active->entity_bounds.empty(),"No authored objects rendered");const auto index=unsigned(object)%active->entity_bounds.size();set_bounds(active->entity_bounds[index]);zoom=1.8f;reported=false;return result(env,"Authored object: "+active->entity_names[index]+" | inspection pose; gameplay pending");}catch(const std::exception& e){return result(env,e.what());}
}
