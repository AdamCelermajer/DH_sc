#include "model_renderer.hpp"
#include "scene.hpp"
#include "textures.hpp"
#include "animation.hpp"
#include "skinning.hpp"
#include "world.hpp"
#include "objects.hpp"
#include "animation_tables.hpp"
#include "animation_scheduler.hpp"
#include "animation_bank.hpp"
#include "class_tables.hpp"
#include "properties.hpp"
#include "vitals.hpp"
#include "combat_events.hpp"
#include "combat_result.hpp"
#include "health.hpp"
#include "combat_application.hpp"
#include "ai.hpp"
#include "aggro.hpp"
#include "navigation_objects.hpp"
#include "navigation_avoidance.hpp"
#include "navigation_producers.hpp"
#include "navigation_heading.hpp"
#include "actor_runtime.hpp"
#include "actor_blended_playback.hpp"
#include "physical_world.hpp"
#include "character_scene.hpp"
#include "character_state.hpp"
#include "character_timers.hpp"
#include "character_stance.hpp"
#include "character_controller_commands.hpp"
#include "character_path_commands.hpp"
#include "navigation_producers.hpp"
#include <GLES2/gl2.h>
#include <android/log.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <map>
#include <set>
#include <functional>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <cctype>
#include <cstring>

namespace model_renderer {
namespace {
using Matrix=std::array<float,16>;
using Vertex=dh2::objects::Vertex;
struct Draw{GLuint vertices=0,indices=0,diffuse=0,alpha=0;GLsizei count=0;unsigned node=0;dh2::scene::Material material;
 dh2::skinning::Skin skin;std::vector<Vertex> cpu_vertices;std::vector<std::array<float,3>> rest_positions;
 bool environment=false;Matrix placement{};};
std::vector<Draw> draws;std::vector<GLuint> images;GLuint program=0;
struct AggroStorage {
 std::vector<dh2::data::AggroEntry> outgoing,incoming;unsigned out_count=0,in_count=0;
 void initialize(unsigned capacity){outgoing.resize(capacity);incoming.resize(capacity);out_count=in_count=0;}
};
struct ObjectActor:dh2::objects::Record {
 dh2::data::AnimationScheduler scheduler;double cursor=0;unsigned completions=0;std::string state="Idle";
 dh2::data::PropertySheet base_class{};int class_id=-1;
 dh2::data::PropertyState properties;
 dh2::animation::EventCursor event_cursor;unsigned animation_events=0;
 dh2::data::CombatActorState combat_state;int combat_target=-1;bool pending_death=false;
 AggroStorage aggro;std::uint64_t identity=0;unsigned target_alive=0,target_sight=0;bool target_seeking=false,ai_attack=false;
 ObjectActor(const dh2::objects::Record& r):Record(r){}
};
struct ObjectGroup{dh2::objects::Resource resource;std::vector<Draw> draws;std::vector<ObjectActor> instances;std::map<int,dh2::animation::Player> clips;int animation_table=-1;};
std::uint64_t snapshot_checksum(const dh2::data::PropertySheet& sheet){
 std::uint64_t result=14695981039346656037ull;for(auto value:sheet)for(unsigned i=0;i<4;++i){result^=(std::uint32_t(value)>>(8*i))&255;result*=1099511628211ull;}return result;
}
std::vector<ObjectGroup> object_groups;
std::vector<ObjectActor> saved_actors;
struct PlayerCombat {
 dh2::data::PropertyState properties;dh2::data::CombatActorState life;
 int animation_table=-1,target=-1;unsigned attempts=0,received=0;
 bool pending_death=false;std::uint64_t death_target=0;
 AggroStorage aggro;
};
PlayerCombat prince_combat;
dh2::data::AiTables actor_ai_tables;bool enemy_ai_enabled=true;
std::map<int,dh2::animation::Player> prince_attack_clips;
dh2::data::AnimationBank prince_animation_bank;
dh2::data::PropertyRules actor_property_rules;
dh2::data::CombatRandom combat_random{0xD22026u,0};unsigned combat_hits=0;
dh2::data::AnimationTables actor_animation_tables;dh2::data::Dictionary actor_clip_table;dh2::data::AnimationRandom actor_random;
std::vector<dh2::objects::Record> world_objects;
int inspected_object=-1;
std::chrono::steady_clock::time_point object_epoch;
bool enabled=false;float center[3]{},radius=1,yaw=-1.57f,pitch=.35f,zoom=1;
dh2::scene::Scene current_scene;dh2::animation::Player player;
dh2::animation::Player walk_player;dh2::world::Level level;dh2::world::Point actor_position{};
bool world_mode=false,walking=false,resume_world=false;float move_x=0,move_y=0,heading=0;
unsigned movement_steps=0,blocked_steps=0;
unsigned native_heading_updates=0;
std::chrono::steady_clock::time_point last_frame;
std::chrono::steady_clock::time_point epoch;
// The touch-to-destination and follow-camera producers remain development
// controls. Actor pose/movement, body services and floor validation below use
// the recovered source pipeline and its original scene/Step/actor ordering.
dh2::physical::NativeWorld actor_world;
dh2::actor::RuntimeState prince_runtime{};
dh2::physical::NativeBody prince_body{};
dh2::visual::SceneBinding prince_visual;
dh2::actor::BlendedPlayback prince_locomotion;
dh2::character::State prince_state;
std::vector<dh2::character::Timer32> prince_timer_storage(20);
dh2::character::TimerStore32 prince_timers{prince_timer_storage.data(),0,20,0x100000001ull,0,0};
b2FilterData prince_initial_filter;
bool prince_scene_phase=false;
std::uint64_t pending_character_services=0;
dh2::character::Facts* active_prince_facts=nullptr;
void request_prince_death();
int prince_event(unsigned,std::uint64_t);
void prince_timer_expired(void*,std::uintptr_t,std::int32_t,dh2::character::Timer32*);
int prince_timer_grow(void*,dh2::character::TimerStore32*,std::uint32_t);
const dh2::character::TimerServices32 prince_timer_services{nullptr,prince_timer_expired,prince_timer_grow,0};
unsigned prince_event_cause=0;
dh2::character::Facts prince_facts();
void character_service(void*,dh2::character::State*,const dh2::character::Request*);
const dh2::character::Services prince_services{nullptr,character_service};
std::uint32_t prince_flags=0x2380,prince_move_type=0;
// Recovered controller constructor and shared BSS initial values. Original
// script/HUD producers will write these owned native gates as they are bound.
std::uint32_t controller_global_blocked=0,prince_controller_forced=0;
float scene_clock=0;bool native_actor_ready=false;
unsigned native_actor_frames=0,native_physics_steps=0;
std::vector<dh2::navigation::ObstacleEntry> live_obstacle_entries;
std::vector<unsigned> live_obstacle_floors,live_workspace_floors;
std::vector<dh2::navigation::PathSegment> live_path_segments,live_workspace_segments;
std::vector<dh2::navigation::AvoidanceActor> live_workspace_actors;
dh2::navigation::ObstacleRegistry live_registry{};
dh2::navigation::ControllerWorkspace live_workspace{};
dh2::navigation::MotionPolicy live_motion_policy{};
struct BodyOwner {
 dh2::physical::WorldObject services{};
 dh2::navigation::PhysicalContact contact{};
 dh2::physical::NativeBody* native=nullptr;
 unsigned additions=0,results=0;
 BodyOwner(){services.context=this;services.test=test;services.contact=collision;services.velocity=velocity;}
 static unsigned test(void* a,void* b,const dh2::physical::Filter*,const dh2::physical::Filter*){
  return dh2_nav_can_collide(&static_cast<BodyOwner*>(a)->contact,&static_cast<BodyOwner*>(b)->contact)==1;
 }
 static void collision(void* a,dh2::physical::ContactEvent event,void*,const float*,unsigned){
  auto& owner=*static_cast<BodyOwner*>(a);owner.additions+=event==dh2::physical::ContactEvent::add;
  owner.results+=event==dh2::physical::ContactEvent::result;
 }
 static void velocity(void* a,float* xy){
  const auto* n=static_cast<BodyOwner*>(a)->native;
  if(n&&n->body){const auto v=n->body->GetLinearVelocity();xy[0]=v.x*100.f;xy[1]=v.y*100.f;}
  else xy[0]=xy[1]=0;
 }
 void set_filter(const dh2::physical::CharacterBodyConfig& c){
  contact={1,0,1,1,{std::int16_t(c.shape.group_index),std::uint16_t(c.shape.category_bits),std::uint16_t(c.shape.mask_bits),1},{}};
 }
};
BodyOwner prince_body_owner;
std::vector<std::unique_ptr<BodyOwner>> decor_body_owners;
std::vector<dh2::physical::NativeBody> decor_bodies;
bool frozen=false,animation_failed=false,frozen_cursor_logged=false;int sampled_ms=0;
GLint position,texcoord,color,mvp,texture_matrix,material_color,has_alpha,alpha_ref;
void check(const char* operation){
  auto code=glGetError();if(code!=GL_NO_ERROR){char b[128];std::snprintf(b,sizeof(b),"%s GL error 0x%04x",operation,code);throw std::runtime_error(b);}
}
GLuint shader(GLenum type,const char* source){
  GLuint s=glCreateShader(type);glShaderSource(s,1,&source,nullptr);glCompileShader(s);GLint ok=0;glGetShaderiv(s,GL_COMPILE_STATUS,&ok);
  if(!ok){char log[2048]{};glGetShaderInfoLog(s,sizeof(log),nullptr,log);glDeleteShader(s);throw std::runtime_error(log);}return s;
}
void create_program(){
  const char* vs=R"(attribute vec3 position;attribute vec2 texcoord;attribute vec4 color;
uniform mat4 mvp;uniform mat4 texture_matrix;varying vec2 uv;varying vec4 tint;
void main(){gl_Position=mvp*vec4(position,1.0);uv=(texture_matrix*vec4(texcoord,0.0,1.0)).xy;tint=color;})";
  const char* fs=R"(precision mediump float;uniform sampler2D diffuse;uniform sampler2D alpha_map;
uniform float has_alpha;uniform float alpha_ref;uniform vec4 material_color;varying vec2 uv;varying vec4 tint;
void main(){vec4 c=texture2D(diffuse,uv)*tint*material_color;
if(has_alpha>0.5)c.a*=texture2D(alpha_map,uv).r;if(c.a<=max(alpha_ref,0.0039))discard;gl_FragColor=c;})";
  GLuint v=shader(GL_VERTEX_SHADER,vs),f=0;
  try{f=shader(GL_FRAGMENT_SHADER,fs);}catch(...){glDeleteShader(v);throw;}
  program=glCreateProgram();glAttachShader(program,v);glAttachShader(program,f);glLinkProgram(program);glDeleteShader(v);glDeleteShader(f);
  GLint ok=0;glGetProgramiv(program,GL_LINK_STATUS,&ok);if(!ok){char log[2048]{};glGetProgramInfoLog(program,sizeof(log),nullptr,log);glDeleteProgram(program);program=0;throw std::runtime_error(log);}
  position=glGetAttribLocation(program,"position");texcoord=glGetAttribLocation(program,"texcoord");color=glGetAttribLocation(program,"color");
  mvp=glGetUniformLocation(program,"mvp");texture_matrix=glGetUniformLocation(program,"texture_matrix");material_color=glGetUniformLocation(program,"material_color");
  has_alpha=glGetUniformLocation(program,"has_alpha");alpha_ref=glGetUniformLocation(program,"alpha_ref");
}
void release(std::vector<Draw>& batches,std::vector<GLuint>& textures){
  for(auto& b:batches){if(b.vertices)glDeleteBuffers(1,&b.vertices);if(b.indices)glDeleteBuffers(1,&b.indices);}
  for(auto t:textures)glDeleteTextures(1,&t);batches.clear();textures.clear();
}
void release_objects(std::vector<ObjectGroup>& groups){std::vector<GLuint> none;for(auto& group:groups)release(group.draws,none);groups.clear();}
std::vector<std::uint8_t> read(AAssetManager* assets,const std::string& name,const std::string& folder="textures"){
  auto path=name;
  if(folder=="textures")std::transform(path.begin(),path.end(),path.begin(),[](unsigned char c){return char(std::tolower(c));});
  auto* a=AAssetManager_open(assets,(folder+"/"+path).c_str(),AASSET_MODE_BUFFER);
  if(!a)throw std::runtime_error("Bundled asset missing: "+folder+"/"+name);
  const auto n=AAsset_getLength64(a);
  if(n<=0||n>32*1024*1024){AAsset_close(a);throw std::runtime_error("Texture exceeds size limit");}
  std::vector<std::uint8_t> bytes(n);std::size_t done=0;
  while(done<bytes.size()){const auto got=AAsset_read(a,bytes.data()+done,bytes.size()-done);if(got<=0){AAsset_close(a);throw std::runtime_error("Short asset read");}done+=got;}
  AAsset_close(a);return bytes;
}
GLuint upload(AAssetManager* assets,const std::string& name,std::map<std::string,GLuint>& cache,std::vector<GLuint>& owned){
  auto found=cache.find(name);if(found!=cache.end())return found->second;
  std::vector<std::uint8_t> rgba;unsigned w=1,h=1;
  if(name.empty())rgba={255,255,255,255};
  else{
    auto raw=read(assets,name);dh2::textures::View view{};
    if(dh2_texture_open(raw.data(),raw.size(),&view)!=dh2::textures::Error::ok)throw std::runtime_error("Texture header rejected: "+name);
    w=view.width;h=view.height;rgba.resize(std::size_t(w)*h*4);
    if(dh2_texture_decode(&view,rgba.data(),rgba.size())!=dh2::textures::Error::ok)throw std::runtime_error("Texture decode rejected: "+name);
  }
  GLint max=0;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&max);if(w>unsigned(max)||h>unsigned(max))throw std::runtime_error("Texture exceeds GPU limit");
  GLuint t=0;glGenTextures(1,&t);owned.push_back(t);glBindTexture(GL_TEXTURE_2D,t);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  // Original UVs wrap beyond [0,1]; fixture sizes are all powers of two.
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.data());check("Model texture upload");
  cache[name]=t;return t;
}
std::array<float,3> cross(const std::array<float,3>& a,const std::array<float,3>& b){return {a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]};}
void normalize(std::array<float,3>& a){float n=std::sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);for(float& x:a)x/=n;}
float dot(const std::array<float,3>& a,const std::array<float,3>& b){return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];}
Matrix camera(int width,int height){
  const float aspect=float(width)/height,tan=.41421356f;
  const float distance=radius*1.15f*std::sqrt(1+1/(tan*tan*std::min(1.f,aspect)*std::min(1.f,aspect)))*zoom;
  std::array<float,3> eye{center[0]+distance*std::cos(yaw)*std::cos(pitch),center[1]+distance*std::sin(yaw)*std::cos(pitch),center[2]+distance*std::sin(pitch)};
  std::array<float,3> f{center[0]-eye[0],center[1]-eye[1],center[2]-eye[2]};normalize(f);
  auto s=cross(f,{0,0,1});normalize(s);auto u=cross(s,f);
  Matrix view{s[0],u[0],-f[0],0,s[1],u[1],-f[1],0,s[2],u[2],-f[2],0,-dot(s,eye),-dot(u,eye),dot(f,eye),1};
  const float near=world_mode?50.f:std::max(.01f,distance-radius*1.5f),far=world_mode?12000.f:distance+radius*3;
  Matrix projection{1/(tan*aspect),0,0,0,0,1/tan,0,0,0,0,-(far+near)/(far-near),-1,0,0,-2*far*near/(far-near),0};
  return dh2::scene::multiply(projection,view);
}
}
void reset_context(){native_actor_ready=false;actor_world.clear();prince_body={};resume_world=resume_world||world_mode;if(world_mode){saved_actors.clear();for(const auto& group:object_groups)for(const auto& actor:group.instances)if(actor.kind==1)saved_actors.push_back(actor);}world_mode=false;move_x=move_y=0;draws.clear();images.clear();object_groups.clear();world_objects.clear();prince_locomotion=dh2::actor::BlendedPlayback{};prince_visual={};prince_attack_clips.clear();prince_animation_bank={};scene_clock=0;inspected_object=-1;current_scene={};player=dh2::animation::Player{};walk_player=dh2::animation::Player{};level={};program=0;enabled=false;}
void deactivate(){native_actor_ready=false;actor_world.clear();prince_body={};enabled=false;world_mode=false;resume_world=false;move_x=move_y=0;}
bool active(){return enabled;}
void set_enemy_ai(bool value){enemy_ai_enabled=value;__android_log_print(ANDROID_LOG_INFO,"DH2Native","Enemy AI configured | automatic melee %d",value);}
void orbit(float dx,float dy,float factor){yaw+=dx;pitch=std::clamp(pitch+dy,-1.4f,1.4f);zoom=std::clamp(zoom*factor,.35f,4.f);}
void set_time(int milliseconds){
  frozen_cursor_logged=false;
  // World inspection pauses the complete live actor pipeline at its composed
  // pose. Arbitrary millisecond sampling remains a separate model-preview tool.
  if(world_mode){
   frozen=milliseconds>=0;last_frame=std::chrono::steady_clock::now();
   std::uint64_t pose=14695981039346656037ull;
   auto digest=[&](const float* values,unsigned count){for(unsigned i=0;i<count;++i){std::uint32_t word;std::memcpy(&word,values+i,4);for(unsigned j=0;j<4;++j){pose^=(word>>(j*8))&255;pose*=1099511628211ull;}}};
   for(const auto& node:current_scene.graph){digest(node.translation,3);digest(node.quaternion,4);digest(node.scale,3);digest(node.world.data(),16);}
   const auto body=prince_body.body?prince_body.body->GetPosition():b2Vec2(actor_position[0]*.01f,actor_position[1]*.01f);
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Prince blended inspection | frozen %u | scene %u | Step %u | actor %u | clip %d | ms %d | body %.6g %.6g | pose %016llx",unsigned(frozen),unsigned(scene_clock),native_physics_steps,native_actor_frames,prince_locomotion.current_clip(),prince_locomotion.current_timeline().current_ms,body.x,body.y,static_cast<unsigned long long>(pose));
   return;
  }
  if(milliseconds<0){epoch=std::chrono::steady_clock::now()-std::chrono::milliseconds(sampled_ms-player.start);frozen=false;}
  else{sampled_ms=std::clamp(milliseconds,player.start,player.end);frozen=true;}
}
std::string load(const std::uint8_t* bytes,std::size_t size,AAssetManager* assets){
  std::vector<Draw> candidate;std::vector<GLuint> textures;
  try{
    if(!assets)throw std::runtime_error("Asset manager unavailable");
    dh2::resources::BresView view{};if(dh2_bres_open(&view,bytes,size)!=dh2::resources::BresError::ok)throw std::runtime_error("BRES header rejected");
    dh2::scene::Scene scene;std::string error;if(!dh2::scene::load(view,scene,error))throw std::runtime_error(error);
    dh2::animation::Player candidate_player;
    auto prince=std::find_if(scene.graph.begin(),scene.graph.end(),[](const dh2::scene::Node& n){return n.id=="prince_modular-node";});
    if(prince!=scene.graph.end()){
      // Preview equipment selection is explicit. The game normally chooses
      // modular controllers dynamically; the serialized scene lists a shadow.
      const auto node=unsigned(prince-scene.graph.begin());scene.instances.clear();
      for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::controller);++i){
        dh2::skinning::Skin skin;if(!dh2::skinning::load(view,i,scene,skin,error))throw std::runtime_error(error);
        if(skin.id.find("_default_warrior-mesh-skin")==std::string::npos)continue;
        dh2::assets::Mesh mesh{};dh2_mesh_open(&mesh,&view,skin.geometry);
        dh2::scene::Instance instance{prince->id,node,skin.geometry,prince->world,{}};instance.controller=i;
        for(unsigned j=0;j<mesh.primitives;++j){dh2::assets::Primitive primitive{};dh2_mesh_primitive(&mesh,j,&primitive);
          auto material=std::find_if(scene.materials.begin(),scene.materials.end(),[&](const dh2::scene::Material& m){return m.id==primitive.material;});
          if(material==scene.materials.end())throw std::runtime_error("Unresolved equipment material");instance.materials.push_back(material-scene.materials.begin());}
        scene.instances.push_back(std::move(instance));
      }
      if(scene.instances.size()!=4)throw std::runtime_error("Incomplete warrior equipment preview");
      auto clip=read(assets,"prince_menu_idle_knight.bdae","animations");
      if(!candidate_player.load(clip.data(),clip.size(),scene,error))throw std::runtime_error("Animation load failed: "+error);
      if(!candidate_player.sample(scene,0,error))throw std::runtime_error(error);
    }else if(!candidate_player.load(bytes,size,scene,error))throw std::runtime_error("Animation load failed: "+error);
    if(!program)create_program();std::map<std::string,GLuint> cache;
    float low[3]{INFINITY,INFINITY,INFINITY},high[3]{-INFINITY,-INFINITY,-INFINITY};unsigned total=0,triangles=0;
    for(const auto& instance:scene.instances){
      dh2::assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&view,instance.geometry)!=dh2::assets::Error::ok)throw std::runtime_error("Geometry rejected");
      if(mesh.primitives!=instance.materials.size())throw std::runtime_error("Material binding count differs from primitive count");
      for(unsigned j=0;j<mesh.primitives;++j){
        dh2::assets::Primitive p{};dh2_mesh_primitive(&mesh,j,&p);
        if(p.collada_type||p.index_count%3||mesh.vertices>65536||p.index_count>3000000)throw std::runtime_error("Unsupported primitive topology or size");
        if(total>1000000-mesh.vertices)throw std::runtime_error("Vertex budget exceeded");total+=mesh.vertices;triangles+=p.index_count/3;
        dh2::assets::Attribute a{},uv{},color_attribute{};
        if(dh2_mesh_attribute(&mesh,p.attributes[0],&a)!=dh2::assets::Error::ok||a.components<3)throw std::runtime_error("Missing position attribute");
        const bool have_uv=dh2_mesh_attribute(&mesh,p.attributes[4],&uv)==dh2::assets::Error::ok&&uv.components>=2;
        const bool have_color=dh2_mesh_attribute(&mesh,p.attributes[2],&color_attribute)==dh2::assets::Error::ok;
        Draw d;d.node=instance.node_index;d.material=scene.materials.at(instance.materials[j]);
        if(instance.controller>=0&&!dh2::skinning::load(view,instance.controller,scene,d.skin,error))throw std::runtime_error(error);
        if(d.material.id!=p.material)throw std::runtime_error("Material binding order/symbol mismatch");
        candidate.push_back(std::move(d));auto& batch=candidate.back();
        batch.diffuse=upload(assets,batch.material.diffuse,cache,textures);batch.alpha=upload(assets,batch.material.alpha_map,cache,textures);
        std::vector<Vertex> vertices(mesh.vertices);
        for(unsigned k=0;k<mesh.vertices;++k){auto& vertex=vertices[k];float raw[4]{};dh2_attribute_read(&a,k,raw);
          for(unsigned row=0;row<3;++row){float x=instance.world[12+row];for(unsigned c=0;c<3;++c)x+=instance.world[c*4+row]*raw[c];
            if(!std::isfinite(x))throw std::runtime_error("Nonfinite vertex position");vertex.p[row]=raw[row];
            if(batch.skin.nodes.empty()){low[row]=std::min(low[row],x);high[row]=std::max(high[row],x);}}
          if(have_uv){dh2_attribute_read(&uv,k,raw);std::copy(raw,raw+2,vertex.uv);}else vertex.uv[0]=vertex.uv[1]=0;
          std::fill(vertex.color,vertex.color+4,1);
          if(have_color){dh2_attribute_read(&color_attribute,k,raw);for(unsigned c=0;c<color_attribute.components;++c)vertex.color[c]=raw[c]/(color_attribute.type==1?255.f:1.f);}
        }
        if(!batch.skin.nodes.empty()){
          batch.rest_positions.resize(vertices.size());for(unsigned k=0;k<vertices.size();++k)std::copy(vertices[k].p,vertices[k].p+3,batch.rest_positions[k].begin());
          std::vector<dh2::skinning::Matrix> matrices;std::vector<std::array<float,3>> deformed;
          if(!dh2::skinning::palette(batch.skin,scene,matrices,error)||!dh2::skinning::positions(batch.skin,matrices,batch.rest_positions,deformed,error))throw std::runtime_error(error);
          for(unsigned k=0;k<vertices.size();++k)for(unsigned row=0;row<3;++row){vertices[k].p[row]=deformed[k][row];low[row]=std::min(low[row],deformed[k][row]);high[row]=std::max(high[row],deformed[k][row]);}
          batch.cpu_vertices=vertices;
        }
        std::vector<std::uint16_t> indices(p.index_count);for(unsigned k=0;k<p.index_count;++k){std::uint32_t x;dh2_index_read(&p,k,&x);indices[k]=x;}
        batch.count=p.index_count;glGenBuffers(1,&batch.vertices);glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),batch.skin.nodes.empty()?GL_STATIC_DRAW:GL_DYNAMIC_DRAW);
        glGenBuffers(1,&batch.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,batch.indices);glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*2,indices.data(),GL_STATIC_DRAW);check("Mesh buffer upload");
      }
    }
    const float extent[3]{high[0]-low[0],high[1]-low[1],high[2]-low[2]};
    float next_radius=std::sqrt(extent[0]*extent[0]+extent[1]*extent[1]+extent[2]*extent[2])*.5f;
    if(!std::isfinite(next_radius)||next_radius<.001f)throw std::runtime_error("Degenerate scene bounds");
    release_objects(object_groups);world_objects.clear();inspected_object=-1;release(draws,images);draws=std::move(candidate);images=std::move(textures);radius=next_radius;
    native_actor_ready=false;actor_world.clear();prince_body={};
    world_mode=false;resume_world=false;move_x=move_y=0;
    for(unsigned i=0;i<3;++i)center[i]=(low[i]+high[i])*.5f;
    yaw=-1.57f;pitch=.35f;zoom=1;enabled=true;
    current_scene=std::move(scene);player=std::move(candidate_player);epoch=std::chrono::steady_clock::now();sampled_ms=player.start;frozen=false;animation_failed=false;frozen_cursor_logged=false;
    char report[384];std::snprintf(report,sizeof(report),"3D upload OK | %zu draws | %u triangles | %zu textures\n%u animation tracks | %u skipped | Preview lighting. Drag to orbit.",draws.size(),triangles,images.size(),player.track_count(),player.skipped);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","%s | nodes %u | skipped nongeometry instances %u | skin draws %zu | segments %u",report,current_scene.nodes,current_scene.ignored_instances,std::count_if(draws.begin(),draws.end(),[](const Draw& d){return !d.skin.nodes.empty();}),player.segment_count());return report;
  }catch(const std::exception& e){release(candidate,textures);__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Model load failed: %s",e.what());return std::string("Model load failed: ")+e.what();}
}
void move_axis(float x,float y){
  if(!std::isfinite(x)||!std::isfinite(y))return;
  move_x=std::clamp(x,-1.f,1.f);move_y=std::clamp(y,-1.f,1.f);float length=std::hypot(move_x,move_y);
  if(length>1){move_x/=length;move_y/=length;}
  if(length>.08f)inspected_object=-1;
  if(world_mode&&length<.01f){
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player position %.4f %.4f %.4f | moved %u | blocked %u",actor_position[0],actor_position[1],actor_position[2],movement_steps,blocked_steps);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player facing | angle %.9g | native updates %u",heading,native_heading_updates);
  }
}
void focus_object(int index){
  inspected_object=-1;
  if(!world_mode||index<0||unsigned(index)>=world_objects.size())return;
  inspected_object=index;const auto& object=world_objects[index];
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Inspect object %d | %s | %s | room %u | position %.4f %.4f %.4f",index,object.model.c_str(),object.name.c_str(),object.room,object.position[0],object.position[1],object.position[2]);
  for(const auto& group:object_groups)for(const auto& actor:group.instances)if(actor.kind==1&&actor.room==object.room&&actor.name==object.name)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat actor state | %s | HP %d | MP %d | dead %u | combo %u | state %s | target %d | hits %u",actor.name.c_str(),actor.properties.resolved[36],actor.properties.resolved[41],actor.combat_state.dead,actor.combat_state.combo_hits,actor.state.c_str(),actor.combat_target,combat_hits);
}
std::string set_object_state(int index,const std::string& state){
 if(!world_mode||index<0||unsigned(index)>=world_objects.size())return "Actor state requires a valid object index";
 const auto& target=world_objects[index];
 for(auto& group:object_groups){for(auto& actor:group.instances){if(actor.room==target.room&&actor.name==target.name){
  if(actor.kind!=1)return "Scenery has no character state";
  if(actor.combat_state.dead&&state!="Died")return "Dead actor cannot enter a live state";
  if(state!="Idle"&&state!="Walk"&&state!="Attack"&&state!="Died")return "Actor state is not bundled";
  auto* sequence=dh2::data::animation_state(actor_animation_tables,group.animation_table,state);if(!sequence)return "Original actor state is absent";
  std::string error;if(!actor.scheduler.start(actor_animation_tables,sequence-actor_animation_tables.sequences.data(),actor_random,error))return error;
  actor.cursor=0;actor.completions=0;actor.state=state;actor.event_cursor={};actor.animation_events=0;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor state selected | index %d | %s | %s | state %s | clip %d | layers %zu",index,actor.name.c_str(),actor.model.c_str(),state.c_str(),actor.scheduler.clip().anim,actor.scheduler.frames().size());
  return "Original actor state: "+state;
 }}}
 return "Actor instance is absent";
}
std::string set_combat_target(int index,int target){
 if(!world_mode||index<0||unsigned(index)>=world_objects.size()||target<-2||(target>=0&&unsigned(target)>=world_objects.size())||index==target)return "Combat target indices are invalid";
 const auto& source=world_objects[index];
 if(target==-2&&prince_combat.life.dead)return "Combat target is dead";
 if(target>=0&&world_objects[target].kind!=1)return "Combat target is scenery";
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.room==source.room&&actor.name==source.name){
  if(actor.kind!=1||actor.combat_state.dead)return "Combat source is not a living character";
  if(target>=0){const auto& record=world_objects[target];for(const auto& other_group:object_groups)for(const auto& other:other_group.instances)if(other.room==record.room&&other.name==record.name&&other.combat_state.dead)return "Combat target is dead";}
  actor.combat_target=target;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat target selected | index %d | %s | target %d | supplied development target",index,actor.name.c_str(),target);
  return target==-1?"Combat target cleared":"Combat target selected";
 }
 return "Combat source is absent";
}
void add_combat_threat(AggroStorage& owner,AggroStorage& target,std::uint64_t owner_id,std::uint64_t target_id,float amount,unsigned facts){
 dh2::data::AggroTable outgoing{owner.outgoing.data(),owner.out_count,unsigned(owner.outgoing.size())},incoming{target.incoming.data(),target.in_count,unsigned(target.incoming.size())};
 std::uint32_t bits;std::memcpy(&bits,&amount,4);const dh2::data::AggroRequest request{&outgoing,&incoming,owner_id,target_id,bits,facts};dh2::data::AggroChange result{};
 if(dh2_aggro_apply(&result,&request,dh2::data::aggro_add)){enabled=false;__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Combat aggression application failed");return;}
 owner.out_count=outgoing.count;target.in_count=incoming.count;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat aggression | owner %llu | target %llu | amount bits %08x | delta bits %08x | outgoing %u | incoming %u | requests %u | callback services pending",static_cast<unsigned long long>(owner_id),static_cast<unsigned long long>(target_id),bits,result.returned_bits,owner.out_count,target.in_count,result.requests);
}
dh2::data::AiRangeResult actor_player_range(const ObjectActor& actor){
 const auto* npc=dh2::data::ai_props(actor_ai_tables,actor.properties.resolved[1]);const auto* prince=dh2::data::ai_props(actor_ai_tables,prince_combat.properties.resolved[1]);dh2::data::AiRangeResult result{};if(!npc||!prince)return result;
 const dh2::data::AiRangeRequest request{{actor.position[0],actor.position[1],actor.position[2]},{actor_position[0],actor_position[1],actor_position[2]},npc->melee_radius,prince->melee_radius,npc->view_radius};dh2_ai_range(&result,&request);return result;
}
void update_enemy(ObjectActor& actor,int table){
 if(!enemy_ai_enabled||frozen||actor.combat_state.dead)return;
 const auto* props=dh2::data::ai_props(actor_ai_tables,actor.properties.resolved[1]);if(!props||props->type!=4||props->script!="monster")return;
 auto range=actor_player_range(actor);
 if(actor.combat_target==-1&&!prince_combat.life.dead&&dh2::data::ai_enemy(actor_ai_tables,actor.properties.resolved[0],prince_combat.properties.resolved[0],false,true)){
  // Current scene has one hostile candidate. The original collision-query
  // backend/ordering is pending; supply that candidate's distance here.
  float distance;std::memcpy(&distance,&range.distance_bits,4);const float view=actor.aggro.out_count?props->view_radius:props->view_radius_no_aggro;
  if(view*view>distance){actor.combat_target=-2;actor.target_alive=1;actor.target_sight=range.sight;actor.target_seeking=true;
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Enemy spotted | %s | target Prince | AI %d | faction %d | distance squared bits %08x | view %.9g | original OnEnemySpotted target assignment",actor.name.c_str(),actor.properties.resolved[1],actor.properties.resolved[0],range.distance_bits,double(view));}
 }
 if(actor.combat_target!=-2)return;
 const unsigned facts=dh2::data::ai_target_present|dh2::data::ai_targetable|dh2::data::ai_callback_clears_dead|dh2::data::ai_callback_clears_sight|(prince_combat.life.dead?0u:dh2::data::ai_target_alive)|(range.sight?dh2::data::ai_target_sight:0u)|(range.melee?dh2::data::ai_target_melee_range:0u);
 const dh2::data::AiTargetRequest request{actor.state=="Attack"?5:3,facts,actor.target_alive,actor.target_sight};dh2::data::AiTargetResult result{};
 if(dh2_ai_target_update(&result,&request)){enabled=false;return;}actor.target_alive=result.alive;actor.target_sight=result.sight;
 auto select=[&](const char* name){auto* sequence=dh2::data::animation_state(actor_animation_tables,table,name);std::string error;if(!sequence||!actor.scheduler.start(actor_animation_tables,sequence-actor_animation_tables.sequences.data(),actor_random,error)){enabled=false;__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Enemy controller animation failed");return;}actor.state=name;actor.cursor=0;actor.completions=0;actor.event_cursor={};actor.animation_events=0;};
 if(!result.target_present){actor.combat_target=-1;if(actor.ai_attack){select("Idle");actor.ai_attack=false;}
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Enemy target cleared | %s | event %u | Prince dead %u | controller stopped | pursuit backend pending",actor.name.c_str(),result.events[result.event_count-1],prince_combat.life.dead);return;}
 if(result.event_count&&result.events[result.event_count-1]==17&&(actor.state!="Attack"||!actor.scheduler.active())){
  select("Attack");actor.ai_attack=true;__android_log_print(ANDROID_LOG_INFO,"DH2Native","Enemy melee selected | %s | target Prince | event 17 | distance squared bits %08x | clip %d | native controller adapter",actor.name.c_str(),range.distance_bits,actor.scheduler.clip().anim);
 }
 // Full pursuit, seeking, attack-delay/FSM and already-attacking target
 // switching remain separate reconstruction work.
}
void apply_actor_to_player(ObjectActor& attacker,const dh2::data::CombatEventAction& action){
 if(prince_combat.life.dead)return;
 if(attacker.ai_attack&&enemy_ai_enabled&&!actor_player_range(attacker).melee)return;
 // The prototype supplies state 3 when standing and 13 while walking.
 // Original SM_IsIdle(false) accepts both; do not suppress its hit reaction
 // merely because movement input is held. Full original FSM producers remain pending.
 const bool idle=dh2_character_state_is_idle(prince_state.current,0)==1;
 dh2::data::CombatantView av{attacker.properties.resolved.data(),-1,-1,0,0,0,5,attacker.combat_state.combo_hits},dv{prince_combat.properties.resolved.data(),-1,-1,0,0,0,prince_state.current,prince_combat.life.combo_hits};
 dh2::data::CombatResult result;dh2::data::MonsterApplication applied;auto ap=dh2::data::property_view(actor_property_rules,attacker.properties),dp=dh2::data::property_view(actor_property_rules,prince_combat.properties);const dh2::data::MonsterApplicationRequest request{&result,&ap,&dp,&attacker.combat_state,&prince_combat.life};
 const unsigned aggro_facts=dh2::data::aggro_owner_player|(attacker.combat_state.dead?dh2::data::aggro_target_dead:0u);
 if(dh2_combat_melee(&result,&av,&dv,&combat_random,action.offhand,0)||dh2_combat_apply_monster_to_player(&applied,&request,idle)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Player defender application failed");enabled=false;return;}
 if(applied.hit_called)add_combat_threat(prince_combat.aggro,attacker.aggro,0x100000001ull,attacker.identity,applied.threat,aggro_facts);
 ++combat_hits;++prince_combat.received;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Prince damage received | attacker %s | attempt %u | result %d %d %d %d %d %d %u %u %d %d | HP %d %d | dead %u | combo %u | RNG %u %u | statuses %u | low health armed %u | cue %u | checksum %016llx",attacker.name.c_str(),prince_combat.received,result.amount,result.dot_element,result.dot_duration,result.dot_amount,result.hp_leech,result.mp_leech,result.outcomes,result.mask,result.weapon_category,result.element,applied.health.before,applied.health.after,prince_combat.life.dead,attacker.combat_state.combo_hits,combat_random.seed,combat_random.calls,applied.status_requests,prince_combat.life.low_health_armed,applied.health.low_health_cue,static_cast<unsigned long long>(snapshot_checksum(prince_combat.properties.resolved)));
 if(applied.health.low_health_cue)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Player low health request | HP %d | maximum %d | audio pending",applied.health.after,prince_combat.properties.resolved[38]);
 if(applied.status_requests)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Player status services pending | requests %u",applied.status_requests);
 if(applied.health.kill_requested){prince_combat.pending_death=true;prince_combat.death_target=attacker.identity;move_x=move_y=0;if(native_actor_ready)request_prince_death();}
}
void apply_actor_attack(ObjectActor& attacker,const dh2::data::CombatEventAction& action){
 if(action.kind!=dh2::data::CombatEventKind::melee||attacker.combat_state.dead)return;
 if(attacker.combat_target==-2){apply_actor_to_player(attacker,action);return;}
 if(attacker.combat_target<0)return;
 const auto& target_record=world_objects.at(attacker.combat_target);ObjectActor* defender=nullptr;
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.room==target_record.room&&actor.name==target_record.name)defender=&actor;
 if(!defender||defender->kind!=1||defender->combat_state.dead)return;
 dh2::data::CombatantView av{attacker.properties.resolved.data(),-1,-1,0,0,0,5,attacker.combat_state.combo_hits},dv{defender->properties.resolved.data(),-1,-1,0,0,0,defender->state=="Attack"?5:-1,defender->combat_state.combo_hits};
 dh2::data::CombatResult result;dh2::data::MonsterApplication applied;
 auto ap=dh2::data::property_view(actor_property_rules,attacker.properties),dp=dh2::data::property_view(actor_property_rules,defender->properties);
 const dh2::data::MonsterApplicationRequest request{&result,&ap,&dp,&attacker.combat_state,&defender->combat_state};
 const unsigned aggro_facts=(defender->combat_state.dead?dh2::data::aggro_owner_dead:0u)|(attacker.combat_state.dead?dh2::data::aggro_target_dead:0u);
 if(dh2_combat_melee(&result,&av,&dv,&combat_random,action.offhand,0)||dh2_combat_apply_monster(&applied,&request)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Native combat application failed");enabled=false;return;}
 if(applied.hit_called)add_combat_threat(defender->aggro,attacker.aggro,defender->identity,attacker.identity,applied.threat,aggro_facts);
 ++combat_hits;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native combat hit | %s | target %s | hit %u | result %d %d %d %d %d %d %u %u %d %d | HP %d %d | dead %u | combo %u | RNG %u %u | statuses %u | threat %.9g",attacker.name.c_str(),defender->name.c_str(),combat_hits,result.amount,result.dot_element,result.dot_duration,result.dot_amount,result.hp_leech,result.mp_leech,result.outcomes,result.mask,result.weapon_category,result.element,applied.health.before,applied.health.after,defender->combat_state.dead,attacker.combat_state.combo_hits,combat_random.seed,combat_random.calls,applied.status_requests,double(applied.threat));
 if(applied.status_requests)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat status services pending | %s | requests %u",defender->name.c_str(),applied.status_requests);
 if(applied.health.kill_requested){defender->pending_death=true;__android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat death event queued | %s | event 2 | HP %d | dead %u | lifecycle %d",defender->name.c_str(),defender->properties.resolved[36],defender->combat_state.dead,defender->combat_state.lifecycle);}
}
ObjectActor* player_target(int index){
 if(index<0||unsigned(index)>=world_objects.size())return nullptr;
 const auto& record=world_objects[index];
 for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1&&actor.room==record.room&&actor.name==record.name&&!actor.combat_state.dead)return &actor;
 return nullptr;
}
bool player_reach(const ObjectActor& target){
 return actor_player_range(target).melee;
}
std::string player_attack(int supplied_target){
 if(!world_mode||!native_actor_ready||prince_combat.life.dead)return "Player is unavailable";
 if(prince_state.current==5)return "Attack is already in progress";
 int target=supplied_target;float nearest=INFINITY;
 if(target==-1)for(unsigned i=0;i<world_objects.size();++i)if(auto* actor=player_target(i);actor&&player_reach(*actor)){
  const float distance=std::hypot(actor->position[0]-actor_position[0],actor->position[1]-actor_position[1]);if(distance<nearest){nearest=distance;target=i;}
 }
 auto* defender=player_target(target);if(!defender||!player_reach(*defender))return "Walk closer to an enemy";
 prince_combat.target=target;
 const int accepted=prince_event(0xc354,defender->identity);
 if(accepted<0)return "Player attack state request failed";
 if(!accepted)return "Attack is cooling down";
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player attack selected | target %d | %s | root %d | clip %d | supplied unarmed equipment | native state %d",target,defender->name.c_str(),prince_state.current_animation,prince_locomotion.current_clip(),prince_state.current);
 return "Attacking";
}
std::array<int,6> player_vitals(){return {prince_combat.properties.resolved[36],prince_combat.properties.resolved[38],prince_combat.properties.resolved[41],prince_combat.properties.resolved[43],int(prince_combat.life.dead),int(prince_combat.life.low_health_armed)};}
namespace {
void player_authored_event(const dh2::animation::TriggeredEvent& event,int clip){
 const auto& frames=prince_locomotion.scheduler.frames();
 if(frames.empty())return;
 const dh2::data::CombatEventContext context{prince_state.current,int(frames.front().step),int(frames.back().step),0,-1};
 dh2::data::CombatEventAction action;
 if(dh2_combat_event_route(&action,&context,event.name))throw std::runtime_error("Player combat route failed");
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player animation event | clip %d | name %s | sequence %d | attack step %d | kind %d | lag %d | %s | Step %u | position %.4f %.4f %.4f",clip,event.name,action.sequence_step,action.attack_step,int(action.kind),event.lag_ms,prince_scene_phase?"scene before Step":"synchronous actor replay",native_physics_steps,prince_runtime.subobjects.position[0],prince_runtime.subobjects.position[1],prince_runtime.subobjects.position[2]);
 if(action.kind!=dh2::data::CombatEventKind::melee)return;
 auto* target=player_target(prince_combat.target);if(!target||!player_reach(*target))return;
 dh2::data::CombatantView av{prince_combat.properties.resolved.data(),-1,-1,0,0,0,prince_state.current,prince_combat.life.combo_hits},dv{target->properties.resolved.data(),-1,-1,0,0,0,target->state=="Attack"?5:-1,target->combat_state.combo_hits};
 auto ap=dh2::data::property_view(actor_property_rules,prince_combat.properties),dp=dh2::data::property_view(actor_property_rules,target->properties);
 dh2::data::CombatResult result;dh2::data::MonsterApplication applied;
 const dh2::data::MonsterApplicationRequest request{&result,&ap,&dp,&prince_combat.life,&target->combat_state};
 const unsigned aggro_facts=(target->combat_state.dead?dh2::data::aggro_owner_dead:0u)|(prince_combat.life.dead?dh2::data::aggro_target_dead:0u);
 if(dh2_combat_melee(&result,&av,&dv,&combat_random,action.offhand,0)||dh2_combat_apply_player_to_monster(&applied,&request))throw std::runtime_error("Player combat application failed");
 if(applied.hit_called)add_combat_threat(target->aggro,prince_combat.aggro,target->identity,0x100000001ull,applied.threat,aggro_facts);
 ++combat_hits;++prince_combat.attempts;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Prince combat hit | target %s | attempt %u | result %d %d %d %d %d %d %u %u %d %d | HP %d %d | dead %u | combo %u | RNG %u %u | statuses %u",target->name.c_str(),prince_combat.attempts,result.amount,result.dot_element,result.dot_duration,result.dot_amount,result.hp_leech,result.mp_leech,result.outcomes,result.mask,result.weapon_category,result.element,applied.health.before,applied.health.after,target->combat_state.dead,prince_combat.life.combo_hits,combat_random.seed,combat_random.calls,applied.status_requests);
 if(applied.health.kill_requested)target->pending_death=true;
 if(applied.status_requests)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat status services pending | %s | requests %u",target->name.c_str(),applied.status_requests);
}
}
namespace {
unsigned actor_virtual_service(void*,unsigned event,float* payload){
 using namespace dh2::subobjects;
 switch(event){
  // The reconstructed scene/body/floor bridge handles physical and position
  // events. The current renderer has no original game camera or auxiliary.
  case camera_get:if(payload)payload[0]=0;return 0;
  case camera_can_move:return 1;
  case visual_update:case visual_apply_rotation:case get_speed:return 1;
  case visual_sync_scaling:{
   std::string error;return prince_visual.update_world(current_scene,error)?1:~0u;
  }
  default:return ~0u;
 }
}
dh2::character::Facts prince_facts(){
 dh2::character::Facts facts{};facts.is_player=1;facts.stance_mask=210;
 // The current development player has no inventory weapon items. Supply the
 // same empty-equip-set predicate results, then execute the original getter.
 const dh2::character::StanceFacts16 equipment{dh2::character::stance_is_player,5,{0,0}};
 if(dh2_character_anim_stance(&facts.stance,&equipment)!=1)throw std::runtime_error("Character stance producer failed");
 if(auto* target=player_target(prince_combat.target))facts.target=target->identity;
 std::copy(prince_runtime.controller.heading.direction,prince_runtime.controller.heading.direction+3,facts.heading);
 facts.walk_threshold=.45f;facts.run_threshold=.85f;
 const float one=1;dh2::move::Speed movement{};
 if(dh2_move_speed(&movement,prince_combat.properties.resolved.data(),&one)||dh2_character_attack_speed(&facts.attack_speed,prince_combat.properties.resolved.data())!=1)throw std::runtime_error("Character property speed failed");
 facts.walk_speed=movement.walk_multiplier;
 auto sequence=[](const char* name){const auto* value=dh2::data::animation_state(actor_animation_tables,prince_combat.animation_table,name);return value?int(value-actor_animation_tables.sequences.data()):-1;};
 facts.idle=sequence("Idle");facts.walk=sequence("Walk");facts.run=sequence("Run");facts.attack_static=sequence("AttackStatic");facts.attack_moving=sequence("Attack");facts.death=sequence("Died");
 if(const auto* ai=dh2::data::ai_props(actor_ai_tables,prince_combat.properties.resolved[1]))facts.attack_delay=std::uint32_t(ai->attack_delay);
 facts.is_at_destination=dh2_nav_is_at_destination(&prince_runtime.controller,&prince_runtime.path)==1;
 facts.following_path=prince_runtime.path.count!=0;facts.has_ranged_weapon=0;
 return facts;
}
struct CharacterFactsScope {
 dh2::character::Facts* previous;
 explicit CharacterFactsScope(dh2::character::Facts& facts):previous(active_prince_facts){active_prince_facts=&facts;}
 ~CharacterFactsScope(){active_prince_facts=previous;}
};
void refresh_prince_facts(){if(active_prince_facts)*active_prince_facts=prince_facts();}
int prince_look_service(void*,const dh2::character::CharacterControlRequest32* request,
                       dh2::character::CharacterControlResponse16* response){
 using namespace dh2::character;
 if(request->service==control_target_position){
  for(const auto& group:object_groups)for(const auto& target:group.instances)if(target.identity==request->subject){
   // NPC visual target-node/cache ownership remains unfinished. This current
   // native object projection explicitly exposes its own world position.
   std::copy(target.position.begin(),target.position.end(),response->position);return 1;
  }
  return -1;
 }
 if(request->service==control_look_at_point&&request->subject==0x100000001ull){
  LookAtState16 look{{prince_runtime.subobjects.position[0],prince_runtime.subobjects.position[1],prince_runtime.subobjects.position[2]},prince_runtime.rotation.heading_angle};
  if(dh2_character_look_at_point(&look,request->position))return -1;
  if(look.heading_angle!=prince_runtime.rotation.heading_angle){++native_heading_updates;
   __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player attack facing | direction %.9g %.9g | angle %.9g | native updates %u",request->position[0]-look.position[0],request->position[1]-look.position[1],look.heading_angle,native_heading_updates);
  }
  prince_runtime.rotation.heading_angle=prince_runtime.controller.heading.angle=look.heading_angle;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character point facing applied | native LookAt and LookTowards | angle %.9g",look.heading_angle);return 1;
 }
 return -1;
}
void character_service(void*,dh2::character::State* state,const dh2::character::Request* request){
 using namespace dh2::character;std::string error;
 switch(request->service){
 case stop:{
  dh2::move::Policy policy{};dh2_move_policy(&policy,&state->flags);
  if(prince_body.body&&policy.position_from_physics&&dh2_native_body_stop(&prince_body,prince_runtime.subobjects.position))throw std::runtime_error("Character Stop body failed");
  dh2_nav_drop_path(&prince_runtime.path);
  std::copy(prince_runtime.subobjects.position,prince_runtime.subobjects.position+3,prince_runtime.subobjects.destination);
  prince_runtime.controller.path_requested=0;prince_runtime.controller.heading.active=0;
  std::fill(prince_runtime.controller.heading.direction,prince_runtime.controller.heading.direction+3,0);state->heading_active=0;refresh_prince_facts();break;
 }
 case pin:if(prince_body.body&&dh2_native_body_pin(&prince_body))throw std::runtime_error("Character pin failed");break;
 case unpin:if(prince_body.body&&dh2_native_body_unpin(&prince_body))throw std::runtime_error("Character unpin failed");break;
 case set_animation:
  state->current_animation=request->argument[0];
  if(!prince_locomotion.start(actor_animation_tables,state->current_animation,actor_random,prince_attack_clips,prince_visual,current_scene,1,error))throw std::runtime_error(error);
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character animation selected | state %d | flags %x | sequence %d | clip %d | %s",state->current,state->flags,state->current_animation,prince_locomotion.current_clip(),prince_scene_phase?"scene callback":"actor state service");
  break;
 case set_speed:
  if(!prince_locomotion.set_speed(request->scalar,error))throw std::runtime_error(error);
  if(state->current==4)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Locomotion clip: %s | original selected clip %d | timeline speed %.9g | authored restart",state->move_type==2?"Run":"Walk",prince_locomotion.current_clip(),prince_locomotion.current_timeline().scale);
  break;
 case stop_loop:prince_locomotion.stop_loop(false);break;
 case start_timer:{
  const auto id=dh2_character_timer_start(&prince_timers,std::uint32_t(request->argument[0]),request->argument[1],request->argument[2],std::uintptr_t(request->identity),&prince_timer_services);
  if(id<0)throw std::runtime_error("Character timer start failed: "+std::to_string(id));
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character timer started | state %d | slot %d | duration %u | repeat %d | event 0x%x | gate %x",state->current,id,std::uint32_t(request->argument[0]),request->argument[1],request->argument[2],state->attack_gate);break;
 }
 case swap_animation:{
  const auto before_root=prince_locomotion.scheduler.frames().empty()?-1:prince_locomotion.scheduler.frames().front().sequence;
  const auto before_clip=prince_locomotion.current_clip();const auto before_ms=prince_locomotion.current_timeline().current_ms;
  if(!prince_locomotion.swap(actor_animation_tables,request->argument[0],request->argument[1],actor_random,prince_attack_clips,prince_visual,current_scene,state->cached_speed,error))throw std::runtime_error(error);
  if(!prince_locomotion.scheduler.frames().empty())state->current_animation=prince_locomotion.scheduler.frames().front().sequence;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character animation swap | state %d | root %d %d | clip %d %d | ms %d %d | desired %d | old %d",state->current,before_root,state->current_animation,before_clip,prince_locomotion.current_clip(),before_ms,prince_locomotion.current_timeline().current_ms,request->argument[0],request->argument[1]);break;
 }
 case look_at:{
  const CharacterControlServices16 services{nullptr,prince_look_service};
  if(request->argument[0]==1){
   const ControllerCommandState32 controller{reinterpret_cast<std::uintptr_t>(&prince_state),0x100000001ull,controller_global_blocked,state->controller_locked,prince_controller_forced,0};
   if(dh2_character_controller_character(&controller,controller_look_object,request->identity,&services)!=1)throw std::runtime_error("Character controller LookAt failed");
  }else if(dh2_character_control(0x100000001ull,controller_look_object,request->identity,&services)!=1)throw std::runtime_error("Character LookAt failed");
  break;
 }
 case set_heading:{
  float direction[3];std::memcpy(direction,request->argument,12);
  if(dh2_nav_set_heading(&prince_runtime.controller.heading,direction,unsigned(request->scalar)))throw std::runtime_error("Character heading failed");
  prince_runtime.rotation.heading_angle=prince_runtime.controller.heading.angle;state->heading_active=prince_runtime.controller.heading.active;refresh_prince_facts();break;
 }
 case set_death_filter:case reset_filter:{
  if(!prince_body.body)break;
  auto* shape=prince_body.body->GetShapeList();if(!shape)throw std::runtime_error("Character shape missing");
  auto filter=prince_initial_filter;
  if(request->service==set_death_filter){filter.groupIndex=std::int16_t(request->argument[0]);filter.categoryBits=std::uint16_t(request->argument[1]);filter.maskBits=std::uint16_t(request->argument[2]);}
  shape->SetFilterData(filter);
  prince_body_owner.contact.primary={filter.groupIndex,filter.categoryBits,filter.maskBits,1};
  actor_world.backend()->Refilter(shape);
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character filter applied | state %d | group %d | category %u | mask %u | primary only",state->current,filter.groupIndex,filter.categoryBits,filter.maskBits);break;
 }
 case remove_body:
  actor_world.destroy(prince_body.body);prince_body.pinned=0;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character physical object removed | state %d | source event 22",state->current);break;
 case raise_event:{
  const unsigned event=unsigned(request->argument[0]);
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character state event | state %d | event %x | prior %d | flags %x",state->current,event,request->argument[1],state->flags);
  if(event==0x1d)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Player source state | previous %d | current %d | event 0x%x | flags %x | root %d | clip %d | position %.4f %.4f %.4f | Step %u",request->argument[1],state->current,prince_event_cause,state->flags,state->current_animation,prince_locomotion.current_clip(),prince_runtime.subobjects.position[0],prince_runtime.subobjects.position[1],prince_runtime.subobjects.position[2],native_physics_steps);
  if(event==0x3f&&prince_event(event,request->identity)<0)throw std::runtime_error("Character state event failed");
  if((event==0x2a||event==0x2b||event==0x2c)&&prince_event(event,request->identity)<0)throw std::runtime_error("Character state gate event failed");
  break;
 }
 default:{
  const auto bit=std::uint64_t(1)<<request->service;
  if(!(pending_character_services&bit)){pending_character_services|=bit;__android_log_print(ANDROID_LOG_INFO,"DH2Native","Character external service pending | service %u | state %d",request->service,state->current);}
  break;
 }
 }
 prince_flags=state->flags;prince_move_type=state->move_type;walking=state->current==4;
}
int prince_event(unsigned event,std::uint64_t payload){
 auto facts=prince_facts();CharacterFactsScope borrow(facts);const auto old=prince_event_cause;prince_event_cause=event;
 const int result=dh2_character_state_event(&prince_state,&facts,event,payload,&prince_services);prince_event_cause=old;return result;
}
int prince_timer_grow(void*,dh2::character::TimerStore32* store,std::uint32_t minimum){
 if(store!=&prince_timers||store->update_depth)return 0;
 prince_timer_storage.resize(std::max(minimum,store->capacity?store->capacity*2:20u));
 store->slots=prince_timer_storage.data();store->capacity=std::uint32_t(prince_timer_storage.size());return 1;
}
void prince_timer_expired(void*,std::uintptr_t owner,std::int32_t event,dh2::character::Timer32* timer){
 if(owner!=0x100000001ull||!timer)throw std::runtime_error("Character timer owner missing");
 const auto gate=prince_state.attack_gate;
 // Source Character/AI forwarding must reach the machine even when the AI
 // virtual expired callback is suppressed by the controller lock. Full
 // Prince AIS behavior is pending; its optional callback is not fabricated.
 if(event==0x2a&&!prince_state.controller_locked){
  constexpr auto ai_boundary=std::uint64_t(1)<<60;
  if(!(pending_character_services&ai_boundary)){pending_character_services|=ai_boundary;__android_log_print(ANDROID_LOG_INFO,"DH2Native","Character AI expiry service pending | event 0x2a | before state event");}
 }
 if(prince_event(unsigned(event),reinterpret_cast<std::uintptr_t>(timer))<0)throw std::runtime_error("Character timer state forwarding failed");
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character timer expired | slot %u | event 0x%x | elapsed %u | duration %u | gate %x %x | Step %u | before state update",timer->id,unsigned(event),timer->elapsed_ms,timer->duration_ms,gate,prince_state.attack_gate,native_physics_steps);
}
void character_playback_event(void*,dh2::actor::BlendedPlayback&,const dh2::actor::BlendedPlaybackEvent& blended){
 const auto& event=blended.event;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Blended character event | event 0x%x | clip %d | slot %u | phase %u | lag %d",event.handoff.event_id,event.clip,blended.slot,event.phase,event.handoff.lag_ms);
 // The full native six-event CharAI/AIS service binding remains unfinished.
 // Preserve synchronous authored damage and the existing FSM close boundary.
 if(event.handoff.event_id==0x28){
  const dh2::animation::TriggeredEvent trigger{event.handoff.lag_ms,event.handoff.payload};player_authored_event(trigger,event.clip);
 }else if(event.handoff.event_id==0x22){
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Character sequence closed | state %d | event 0x22 | clip %d | Step %u | post Step animator | position %.4f %.4f %.4f",prince_state.current,event.clip,native_physics_steps,prince_runtime.subobjects.position[0],prince_runtime.subobjects.position[1],prince_runtime.subobjects.position[2]);
  if(prince_event(0x22,0)<0)throw std::runtime_error("Character sequence close failed");
 }
}
void request_prince_death(){
 if(!prince_combat.pending_death)return;
 const auto facts=prince_facts();prince_state.animation_override=facts.death;
 const int accepted=prince_event(0xc358,prince_combat.death_target);
 if(accepted<0)throw std::runtime_error("Character death state request failed");
 prince_combat.pending_death=false;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player death animation selected | clip %d | dead %u | lifecycle %d | state %d",prince_locomotion.current_clip(),prince_combat.life.dead,prince_combat.life.lifecycle,prince_state.current);
}
void initialize_native_actor(AAssetManager* assets,bool restore){
 std::string error;float bounds[4];
 const bool restore_without_body=restore&&prince_state.current==12&&!prince_state.body_present;
 if(!level.native_floor||dh2_decor_level_world_bounds(bounds))throw std::runtime_error("Native actor world bounds missing");
 actor_world.load(bounds);decor_body_owners.clear();decor_bodies.clear();decor_bodies.reserve(84);
 prince_body={};prince_body_owner.native=&prince_body;
 // Load the original immutable factory pose and the same explicit default
 // warrior modules used by this prototype's equipment renderer.
 auto prince=read(assets,"prince_modular.bdae","models");dh2::resources::BresView view{};
 dh2::scene::Scene factory;
 if(dh2_bres_open(&view,prince.data(),prince.size())!=dh2::resources::BresError::ok||!dh2::scene::load(view,factory,error))throw std::runtime_error(error);
 const auto node=std::find_if(factory.graph.begin(),factory.graph.end(),[](const auto& n){return n.id=="prince_modular-node";});
 if(node==factory.graph.end())throw std::runtime_error("Native player modular node absent");
 factory.instances.clear();
 for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::controller);++i){
  dh2::skinning::Skin skin;if(!dh2::skinning::load(view,i,factory,skin,error))throw std::runtime_error(error);
  if(skin.id.find("_default_warrior-mesh-skin")==std::string::npos)continue;
  dh2::scene::Instance instance{node->id,unsigned(node-factory.graph.begin()),skin.geometry,node->world,{}};instance.controller=i;
  factory.instances.push_back(std::move(instance));
 }
 if(factory.instances.size()!=4)throw std::runtime_error("Native player equipment bounds incomplete");
 std::vector<dh2::physical::CharacterMeshEntry> entries;
 if(!dh2::physical::character_scene_entries(view,factory,entries,error))throw std::runtime_error(error);
 dh2::physical::CharacterMeshBoxInput mesh_input{};mesh_input.entries=entries.data();mesh_input.count=entries.size();
 std::copy(actor_position.begin(),actor_position.end(),mesh_input.placement.position);
 if(dh2_character_visual_scale(mesh_input.placement.scale,prince_combat.properties.base.data()+12))throw std::runtime_error("Original character visual scale rejected");
 dh2::physical::DecorSceneOutput mesh;
 if(dh2_character_mesh_box(&mesh,&mesh_input))throw std::runtime_error("Native player mesh bounds rejected");
 prince_runtime={};std::copy(actor_position.begin(),actor_position.end(),prince_runtime.subobjects.position);
 std::copy(actor_position.begin(),actor_position.end(),prince_runtime.subobjects.destination);
 dh2::physical::CharacterOwnerBoundsInput owner_input{};
 std::copy(mesh.mesh_box,mesh.mesh_box+6,owner_input.mesh_box);std::copy(actor_position.begin(),actor_position.end(),owner_input.position);
 owner_input.collision_scale=prince_combat.properties.resolved[16];
 dh2::physical::CharacterOwnerBounds owner_bounds{};
 if(dh2_character_owner_bounds(&owner_bounds,&owner_input))throw std::runtime_error("Original character owner bounds rejected");
 std::copy(owner_bounds.relative_box,owner_bounds.relative_box+6,prince_runtime.subobjects.local_bounds);
 std::copy(owner_bounds.absolute_box,owner_bounds.absolute_box+6,prince_runtime.subobjects.absolute_bounds);
 prince_runtime.subobjects.rotation=heading;prince_runtime.rotation.rotation[2]=heading;prince_runtime.rotation.heading_angle=heading;
 dh2::physical::CharacterBodyInput input{};input.owner=&prince_runtime;input.new_physical=&prince_body;input.character_type=1;input.is_player=1;
 const auto* box=prince_runtime.subobjects.absolute_bounds;
 input.absolute_bounds[0]=box[0];input.absolute_bounds[1]=box[1];input.absolute_bounds[2]=box[3];input.absolute_bounds[3]=box[4];
 input.position[0]=actor_position[0];input.position[1]=actor_position[1];
 dh2::physical::CharacterBodyConfig config{};
 if(dh2_character_body_config(&config,&input)||!config.enabled)throw std::runtime_error("Native player body definition rejected");
 prince_body_owner.set_filter(config);prince_body_owner.additions=prince_body_owner.results=0;
 prince_body={actor_world.create_character(config,&prince_body_owner.services),config.radius,config.pinned};
 if(!prince_body.body||dh2_native_body_refresh_view(&prince_runtime.body,&prince_body))throw std::runtime_error("Native player body creation failed");
 prince_initial_filter=prince_body.body->GetShapeList()->GetFilterData();
 live_obstacle_entries.assign(256,{});live_obstacle_floors.assign(level.native_floor->records.size()+1,0);
 live_registry={live_obstacle_entries.data(),0,unsigned(live_obstacle_entries.size()),live_obstacle_floors.data(),0,unsigned(live_obstacle_floors.size())};
 live_path_segments.resize(level.native_floor->graph.node_count+1);live_workspace_segments.resize(live_path_segments.size());
 live_workspace_floors.resize(live_obstacle_floors.size());live_workspace_actors.resize(1);
 live_workspace={live_workspace_segments.data(),unsigned(live_workspace_segments.size()),0,live_workspace_actors.data(),1,0,live_workspace_floors.data(),unsigned(live_workspace_floors.size()),0};
 prince_runtime.path.segments=live_path_segments.data();prince_runtime.path.capacity=live_path_segments.size();
 std::copy(actor_position.begin(),actor_position.end(),prince_runtime.path.target);
 dh2_nav_object_defaults(&prince_runtime.object);dh2_nav_motion_policy_defaults(&live_motion_policy);
 const dh2::navigation::ObjectInitRequest init{&level.native_floor->collision_world,&prince_runtime.object,0x100000001ull,{actor_position[0],actor_position[1],actor_position[2]},config.radius*100.f,0,0};
 if(dh2_nav_init_object(&init))throw std::runtime_error("Native player floor initialization failed");
 const dh2::navigation::ProducerFields fields{dh2::navigation::ProducerClass::character,1,config.radius,0,{box[0],box[1]},{box[3],box[4]}};
 const dh2::navigation::ProducerRequest producer{&level.native_floor->collision_world,&live_registry,&prince_runtime.object,0x100000001ull,&fields};
 if(dh2_nav_update_game_object(&producer))throw std::runtime_error("Native player obstacle initialization failed");
 std::copy(prince_runtime.object.motion.position,prince_runtime.object.motion.position+3,prince_runtime.subobjects.previous_position);
 std::copy(prince_runtime.object.motion.position,prince_runtime.object.motion.position+3,prince_runtime.subobjects.position);
 std::copy(prince_runtime.object.motion.position,prince_runtime.object.motion.position+3,actor_position.begin());
 // Activity recreation rebuilds a complete CPU playback session and explicitly
 // restarts its saved sequence below. It does not restore interrupted fades.
 prince_visual=dh2::visual::SceneBinding{};
 if(!prince_visual.bind(current_scene,error))throw std::runtime_error(error);
 prince_locomotion=dh2::actor::BlendedPlayback{};scene_clock=0;
 dh2::animation::RegistrationSet registration;
 for(int id:prince_animation_bank.registration_requests){
  const auto resource=prince_attack_clips.find(id);
  if(resource==prince_attack_clips.end()||!registration.append(id,dh2::data::animation_resource_identity(prince_animation_bank,id),&resource->second,error))throw std::runtime_error("Prince registration failed: "+error);
 }
 const int template_id=prince_animation_bank.template_clip_id;
 if(!registration.set_default(dh2::data::animation_resource_identity(prince_animation_bank,template_id),&prince_attack_clips.at(template_id),error))throw std::runtime_error(error);
 registration.refresh_indices();
 if(!prince_locomotion.compile_dynamic(prince_attack_clips,registration,factory,prince_visual,error))throw std::runtime_error("Prince dynamic compilation failed: "+error);
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Prince blended bank ready | resources %zu | occurrences %zu | targets %zu | template %d | engine %d | game clip %d",prince_attack_clips.size(),registration.occurrences().size(),prince_locomotion.transform_set().targets().size(),template_id,prince_locomotion.current_engine_clip(),prince_locomotion.current_clip());
 std::copy(actor_position.begin(),actor_position.end(),prince_visual.root.position);
 std::copy(mesh.effective_scale,mesh.effective_scale+3,prince_visual.root.scale);
 const float euler[3]{0,0,heading};if(!prince_visual.set_rotation(euler))throw std::runtime_error("Native player rotation rejected");
 prince_locomotion.observer={nullptr,character_playback_event};
 if(!restore){
  prince_state={};pending_character_services=0;prince_event_cause=0;
  prince_timer_storage.assign(20,{});prince_timers={prince_timer_storage.data(),0,20,0x100000001ull,0,0};
 }
 prince_state.body_present=1;
 if(prince_state.current==-1||prince_state.current==3||prince_state.current==4){
  // Development Activity recreation cancels held touch before restoring the
  // world. It supplies Idle through the recovered transition services.
  auto facts=prince_facts();CharacterFactsScope borrow(facts);
  if(dh2_character_state_transition(&prince_state,&facts,3,0,0,&prince_services)<0)throw std::runtime_error("Character Idle initialization failed");
 }else if(prince_state.current==5){
  const auto facts=prince_facts();
  if(prince_state.current_animation==facts.attack_moving){if(dh2_native_body_unpin(&prince_body))throw std::runtime_error("Attack body restoration failed");}
  else if(dh2_native_body_pin(&prince_body))throw std::runtime_error("Attack body restoration failed");
 }else if(prince_state.current==12){
  const dh2::character::Request filter{dh2::character::set_death_filter,{0,0x51c,3},0,0,0};
  character_service(nullptr,&prince_state,&filter);
  if(restore_without_body){actor_world.destroy(prince_body.body);prince_state.body_present=0;}
 }
 if(restore&&prince_state.current!=3&&prince_state.current!=4&&prince_state.current_animation>=0){
  if(!prince_locomotion.start(actor_animation_tables,prince_state.current_animation,actor_random,prince_attack_clips,prince_visual,current_scene,prince_state.cached_speed,error))throw std::runtime_error("Restored Prince sequence failed: "+error);
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player blended sequence restarted | state %d | sequence %d | clip %d | engine %d | development recreation",prince_state.current,prince_state.current_animation,prince_locomotion.current_clip(),prince_locomotion.current_engine_clip());
 }
 prince_flags=prince_state.flags;prince_move_type=prince_state.move_type;
 for(const auto& group:object_groups){
  if(group.instances.empty()||group.instances.front().kind!=2)continue;
  auto model=read(assets,group.instances.front().model,"actors");dh2::resources::BresView decor_view{};
  if(dh2_bres_open(&decor_view,model.data(),model.size())!=dh2::resources::BresError::ok)throw std::runtime_error("Decor collision model rejected");
  dh2::physical::DecorSceneMarker marker;
  if(!dh2::physical::decor_scene_marker(decor_view,marker,error))throw std::runtime_error(error);
  if(!marker.found)continue;
  for(const auto& instance:group.instances){
   dh2::physical::DecorSceneInput scene_input{};
   std::copy(instance.position.begin(),instance.position.end(),scene_input.position);
   std::copy(instance.rotation_degrees.begin(),instance.rotation_degrees.end(),scene_input.rotation_degrees);
   std::copy(instance.scale.begin(),instance.scale.end(),scene_input.scale);
   std::copy(marker.bounds,marker.bounds+6,scene_input.marker_bounds);std::copy(marker.parent_scale,marker.parent_scale+3,scene_input.marker_parent_scale);
   dh2::physical::DecorSceneOutput scene_output;
   if(dh2_decor_scene(&scene_output,&scene_input))throw std::runtime_error("Decor collision transform rejected");
   auto owner=std::make_unique<BodyOwner>();decor_bodies.push_back({});auto& body=decor_bodies.back();owner->native=&body;
   dh2::physical::DecorBodyInput decor_input{};decor_input.owner=owner.get();decor_input.new_physical=&body;decor_input.visual_present=decor_input.colbox_found=1;
   std::copy(scene_output.mesh_box,scene_output.mesh_box+6,decor_input.mesh_box);std::copy(instance.position.begin(),instance.position.end(),decor_input.position);
   dh2::physical::DecorBodyConfig definition;
   if(dh2_decor_body_config(&definition,&decor_input))throw std::runtime_error("Decor collision body definition rejected");
   owner->set_filter(definition.physical);body={actor_world.create_character(definition.physical,&owner->services),definition.physical.radius,definition.physical.pinned};
   if(!body.body)throw std::runtime_error("Decor collision body creation failed");
   decor_body_owners.push_back(std::move(owner));
  }
 }
 native_actor_ready=true;native_actor_frames=native_physics_steps=0;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native actor ready | genuine bodies %zu | decor colliders %zu | radius %.9g | source bounds %.6g %.6g %.6g %.6g | flags %x | scene then Step then actor",decor_bodies.size()+unsigned(bool(prince_body.body)),decor_bodies.size(),config.radius*100.f,box[0],box[1],box[3],box[4],prince_flags);
}
void advance_native_actor(unsigned dt_ms){
 std::string error;
 if(frozen)return; // Preserve the composed live pose, clocks and gameplay state.
 request_prince_death();
  scene_clock+=float(dt_ms);
  prince_scene_phase=true;
  if(!prince_locomotion.scene_phase(std::uint32_t(scene_clock),prince_attack_clips,prince_visual,current_scene,error))throw std::runtime_error(error);
  prince_scene_phase=false;
 actor_world.update(dt_ms);++native_physics_steps;
 // Original Character.Update executes CharTimers before AI, state machine,
 // animator and GameObject. ScriptManager blocking remains an explicit zero
 // fact for this development scene, which has no source script manager yet.
 if(dh2_character_timers_update(&prince_timers,dt_ms,0,&prince_timer_services)!=1)throw std::runtime_error("Character timer update failed");
 // Development touch input supplies the original controller facts. State
 // predicates/focus/blur decide eligibility, policy and authored animation.
 const bool input_active=std::hypot(move_x,move_y)>.08f;
 const bool was_heading=prince_state.heading_active!=0;
 if(!prince_state.controller_locked){
  if(input_active){
   const float input[3]{move_x,move_y,0};
   if(dh2_nav_set_heading(&prince_runtime.controller.heading,input,1))throw std::runtime_error("Native input heading rejected");
   prince_runtime.rotation.heading_angle=prince_runtime.controller.heading.angle;
   for(unsigned i=0;i<3;++i)prince_runtime.subobjects.destination[i]=prince_runtime.subobjects.position[i]+input[i]*1000.f;
  }else{
   prince_runtime.controller.heading.active=0;
   std::fill(prince_runtime.controller.heading.direction,prince_runtime.controller.heading.direction+3,0);
  }
  prince_state.heading_active=prince_runtime.controller.heading.active;
  if(input_active&&prince_event(0xc351,0)<0)throw std::runtime_error("Character Move request failed");
  if(prince_state.current==5&&was_heading!=(prince_state.heading_active!=0)&&prince_event(0x1c,0)<0)throw std::runtime_error("Character attack heading event failed");
 }
 auto facts=prince_facts();CharacterFactsScope borrow(facts);
 if(dh2_character_state_update(&prince_state,&facts,dt_ms,&prince_services)<0)throw std::runtime_error("Character state update failed");
 prince_flags=prince_state.flags;prince_move_type=prince_state.move_type;walking=prince_state.current==4;
 const float global_speed=(prince_state.current==4||prince_state.current==5)?prince_state.cached_speed:1.f;
 const bool moving=prince_state.current==4;
 const auto extra=prince_locomotion.completion.extra_ms;
 if(!prince_locomotion.animator_phase(actor_animation_tables,actor_random,prince_attack_clips,prince_visual,current_scene,global_speed,extra,error))throw std::runtime_error(error);
 dh2::move::Policy decoded{};dh2_move_policy(&decoded,&prince_flags);
 // Avoidance policy and the original game camera remain explicit integration
 // boundaries. The floor/path/root/body coordinators execute genuine source.
 const dh2::actor::RuntimePolicy policy{{1,0,0,decoded.position_from_physics},1,0,0,0,dh2::actor::base_virtual_speed};
 const dh2::subobjects::Services services{nullptr,actor_virtual_service};
 const dh2::actor::RuntimeRequest request{&prince_runtime,prince_body.body?&prince_body:nullptr,&prince_visual,&current_scene,&level.native_floor->collision_world,&level.native_floor->graph,&live_registry,&live_motion_policy,&live_workspace,nullptr,prince_combat.properties.resolved.data(),&policy,&services,nullptr,0x100000001ull,prince_flags,dt_ms};
 dh2::actor::RuntimeResult result{};
 if(dh2::actor::update_actor(result,request,error))throw std::runtime_error(error);
 const float dx=prince_runtime.subobjects.position[0]-actor_position[0],dy=prince_runtime.subobjects.position[1]-actor_position[1];
 if(moving){if(dx*dx+dy*dy>0.000001f)++movement_steps;else ++blocked_steps;++native_heading_updates;}
 std::copy(prince_runtime.subobjects.position,prince_runtime.subobjects.position+3,actor_position.begin());heading=prince_runtime.subobjects.rotation;
 ++native_actor_frames;
 const auto physical_position=prince_body.body?prince_body.body->GetPosition():b2Vec2(actor_position[0]*.01f,actor_position[1]*.01f);
 if(native_actor_frames==1||native_actor_frames%120==0)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Native actor frame | scene %u | Step %u | actor %u | source phase %u | clip %d | ms %d | replays %u | body %.6g %.6g | contacts %u %u | state %d | body present %d | timeline scale %.9g",prince_locomotion.root_timestamp,native_physics_steps,native_actor_frames,result.phase,prince_locomotion.current_clip(),prince_locomotion.current_timeline().current_ms,prince_locomotion.restarts,physical_position.x,physical_position.y,prince_body_owner.additions,prince_body_owner.results,prince_state.current,int(bool(prince_body.body)),prince_locomotion.current_timeline().scale);
}
}
std::string load_world(const std::uint8_t* descriptor,std::size_t size,AAssetManager* assets){
  std::vector<Draw> environment;std::vector<GLuint> textures;
  std::vector<ObjectGroup> candidate_groups;
  const bool restore=world_mode||resume_world;const auto previous=actor_position;
  const auto previous_heading=heading;
  const auto previous_random=actor_random;
  if(restore&&!object_groups.empty()){saved_actors.clear();for(const auto& group:object_groups)for(const auto& actor:group.instances)if(actor.kind==1)saved_actors.push_back(actor);}
  try{
    auto raw=read(assets,"crypt.bdae","worlds");dh2::resources::BresView view{};
    if(dh2_bres_open(&view,raw.data(),raw.size())!=dh2::resources::BresError::ok)throw std::runtime_error("World BRES rejected");
    dh2::world::Level candidate;std::string error;if(!dh2::world::load(view,descriptor,size,candidate,error))throw std::runtime_error(error);
    const auto records_data=read(assets,"character_properties_pyarray.bin","data"),names_data=read(assets,"character_properties_pyarraynames.bin","data"),fields_data=read(assets,"character_properties_pystructnames.bin","data"),model_names=read(assets,"character_models_dictionary_pyarraynames.bin","data"),model_values=read(assets,"character_models_dictionary_pyarray.bin","data");
    dh2::data::CharacterTable character_table;dh2::data::Dictionary model_table;
    if(!dh2::data::load_characters({records_data.data(),records_data.size()},{names_data.data(),names_data.size()},{fields_data.data(),fields_data.size()},character_table,error)||!dh2::data::load_dictionary({model_names.data(),model_names.size()},{model_values.data(),model_values.size()},model_table,error))throw std::runtime_error(error);
    const auto class_data=read(assets,"character_classes_pyarray.bin","data"),class_names=read(assets,"character_classes_pyarraynames.bin","data"),class_schema=read(assets,"character_classes_pystructnames.bin","data");dh2::data::ClassTables class_table;
    if(!dh2::data::load_classes({class_data.data(),class_data.size()},{class_names.data(),class_names.size()},{class_schema.data(),class_schema.size()},class_table,error))throw std::runtime_error(error);
    if(character_table.fields[19]!="Level"||character_table.fields[38]!="Max_HP"||character_table.fields[43]!="Max_MP")throw std::runtime_error("Original class property identifiers differ");
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Class tables ready | classes %zu | bytes %zu | cached base snapshots",class_table.rows.size(),class_table.data_consumed);
    dh2::data::PropertyRules property_rules;if(!dh2::data::load_property_rules(character_table,property_rules,error))throw std::runtime_error(error);
    dh2::data::AiTables ai_tables;std::array<std::vector<std::uint8_t>,6> ai_data;const char* ai_names[]={"ai_pyarray.bin","ai_pyarraynames.bin","ai_pystructnames.bin","ai_factions_pyarray.bin","ai_factions_pyarraynames.bin","ai_factions_pystructnames.bin"};for(unsigned i=0;i<6;++i)ai_data[i]=read(assets,ai_names[i],"data");
    if(!dh2::data::load_ai({ai_data[0].data(),ai_data[0].size()},{ai_data[1].data(),ai_data[1].size()},{ai_data[2].data(),ai_data[2].size()},{ai_data[3].data(),ai_data[3].size()},{ai_data[4].data(),ai_data[4].size()},{ai_data[5].data(),ai_data[5].size()},ai_tables,error))throw std::runtime_error(error);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","AI tables ready | configs %zu | factions %zu | automatic melee %d | pursuit and full FSM pending",ai_tables.rows.size(),ai_tables.factions.size(),enemy_ai_enabled);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Property rules ready | defaults row 0 | types row 1 | properties 224 | supplied sheets only");
    // Development probe of the packaged native result calculator. Its copied
    // RNG and supplied unarmed sheets never execute hits or mutate actors.
    const auto defender_row=std::find(character_table.names.begin(),character_table.names.end(),"Crypt_Skeleton");
    if(defender_row==character_table.names.end())throw std::runtime_error("Combat probe defender missing");
    dh2::data::PropertyState probe_defender;dh2::data::reset_properties(property_rules,probe_defender,&character_table.rows.at(defender_row-character_table.names.begin()));
    dh2::data::SpawnVitals probe_vitals;
    if(!dh2::data::recalc_properties_with_class(class_table,property_rules,probe_defender,error)||!dh2::data::initialize_spawn_vitals(property_rules,probe_defender,probe_vitals,error))throw std::runtime_error(error);
    const auto animation_data=read(assets,"animations_pyarray.bin","data"),animation_names=read(assets,"animations_pyarraynames.bin","data"),animation_fields=read(assets,"animations_pystructnames.bin","data"),clip_names=read(assets,"animations_dictionary_pyarraynames.bin","data"),clip_values=read(assets,"animations_dictionary_pyarray.bin","data");
    dh2::data::Dictionary clip_table;dh2::data::AnimationTables animation_tables;dh2::data::AnimationRandom animation_random;
    if(!dh2::data::load_dictionary({clip_names.data(),clip_names.size()},{clip_values.data(),clip_values.size()},clip_table,error)||!dh2::data::load_animation_tables({animation_data.data(),animation_data.size()},{animation_names.data(),animation_names.size()},{animation_fields.data(),animation_fields.size()},clip_table,animation_tables,error))throw std::runtime_error(error);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Animation tables ready | sequences %zu | characters %zu | clip paths %zu | bytes %zu",animation_tables.sequences.size(),animation_tables.characters.size(),clip_table.values.size(),animation_tables.data_consumed);
    const auto objects=read(assets,"crypt01.dact","worlds");std::vector<dh2::objects::Record> object_records;
    if(!dh2::objects::load_records(objects.data(),objects.size(),candidate.rooms,character_table,model_table,object_records,error))throw std::runtime_error(error);
    auto prince=read(assets,"prince_modular.bdae","models"),idle=read(assets,"prince_idle_shield.bdae","animations"),walk=read(assets,"prince_walk_1hand.bdae","animations");
    dh2::resources::BresView actor_view{};dh2_bres_open(&actor_view,prince.data(),prince.size());dh2::scene::Scene rest;
    if(!dh2::scene::load(actor_view,rest,error))throw std::runtime_error(error);
    dh2::animation::Player candidate_idle,candidate_walk;
    if(!candidate_idle.load(idle.data(),idle.size(),rest,error)||!candidate_walk.load(walk.data(),walk.size(),rest,error))throw std::runtime_error(error);
    const auto knight=std::find(character_table.names.begin(),character_table.names.end(),"KnightPlayerBase");if(knight==character_table.names.end())throw std::runtime_error("Original default player preset missing");
    PlayerCombat fresh_player;dh2::data::reset_properties(property_rules,fresh_player.properties,&character_table.rows.at(knight-character_table.names.begin()));dh2::data::SpawnVitals player_vitals;
    fresh_player.aggro.initialize(object_records.size()+1);
    if(!dh2::data::recalc_properties_with_class(class_table,property_rules,fresh_player.properties,error)||!dh2::data::initialize_spawn_vitals(property_rules,fresh_player.properties,player_vitals,error))throw std::runtime_error(error);
    fresh_player.animation_table=fresh_player.properties.resolved[2];
    const auto bank_bytes=read(assets,"prince-animation-bank.bin","data");
    dh2::data::AnimationBank candidate_bank;
    if(!dh2::data::load_animation_bank({bank_bytes.data(),bank_bytes.size()},candidate_bank,error))throw std::runtime_error("Prince metadata rejected: "+error);
    if(candidate_bank.character!="KnightPlayerBase"||int(candidate_bank.animation_table)!=fresh_player.animation_table||candidate_bank.template_clip_id!=1111)throw std::runtime_error("Prince metadata producer does not match player properties");
    std::map<int,dh2::animation::Player> candidate_player_clips;
    for(const auto& resource:candidate_bank.resources){
     dh2::data::AnimationStep ref;ref.anim=resource.clip_id;
     const auto* authored=dh2::data::animation_clip(ref,clip_table);
     if(!authored||*authored!=resource.authored_path||resource.asset.rfind("animations/",0)!=0)throw std::runtime_error("Prince bank dictionary path mismatch");
     const auto raw_clip=read(assets,resource.asset.substr(11),"animations");
     if(raw_clip.size()!=resource.bytes)throw std::runtime_error("Prince bank resource size mismatch");
     auto& playback=candidate_player_clips[resource.clip_id];
     if(!playback.load(raw_clip.data(),raw_clip.size(),rest,error,dh2::animation::MissingTargets::ignore))throw std::runtime_error("Prince bank resource rejected: "+error);
     __android_log_print(ANDROID_LOG_INFO,"DH2Native",resource.clip_id==1023?"Player death track ready | clip %d | tracks %u | unbound %u | attack_mainhand ms %d":"Player attack track ready | clip %d | tracks %u | unbound %u | attack_mainhand ms %d",resource.clip_id,playback.track_count(),playback.unbound,dh2_events_time(&playback.events.view(),"attack_mainhand"));
    }
    if(!program)create_program();std::map<std::string,GLuint> cache;unsigned triangles=0;
    for(const auto& object:object_records){
      int animation_table=-1;if(object.kind==1){const auto* id=dh2::data::property(character_table,object.character,"AnimTable");if(!id)throw std::runtime_error("Monster animation table property missing");animation_table=*id;}
      auto found=std::find_if(candidate_groups.begin(),candidate_groups.end(),[&](const ObjectGroup& group){return group.instances.front().model==object.model&&group.animation_table==animation_table;});
      auto start_actor=[&](ObjectGroup& group){
        group.instances.emplace_back(object);auto& actor=group.instances.back();
        if(object.kind!=1)return;
        actor.identity=0x100000002ull+(&object-object_records.data());actor.aggro.initialize(object_records.size()+1);
        const auto character=std::find(character_table.names.begin(),character_table.names.end(),object.character);const auto* class_id=dh2::data::property(character_table,object.character,"ClassID");
        if(character==character_table.names.end()||!class_id)throw std::runtime_error("Original monster class link absent");
        actor.class_id=*class_id;actor.base_class=character_table.rows.at(character-character_table.names.begin());
        if(!dh2::data::apply_class(class_table,actor.class_id,actor.base_class,error))throw std::runtime_error(error);
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Base class snapshot | %s | character %s | class %d | level raw %d | Max_HP raw %d | Max_MP raw %d | checksum %016llx | cached only",actor.name.c_str(),actor.character.c_str(),actor.class_id,actor.base_class[19],actor.base_class[38],actor.base_class[43],static_cast<unsigned long long>(snapshot_checksum(actor.base_class)));
        dh2::data::reset_properties(property_rules,actor.properties,&character_table.rows.at(character-character_table.names.begin()));
        if(!dh2::data::recalc_properties_with_class(class_table,property_rules,actor.properties,error))throw std::runtime_error(error);
        const auto& resolved=actor.properties.resolved;
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Resolved properties | %s | character %s | HP raw %d | Max_HP raw %d | MP raw %d | Max_MP raw %d | checksum %016llx | supplied sheets only",actor.name.c_str(),actor.character.c_str(),resolved[36],resolved[38],resolved[41],resolved[43],static_cast<unsigned long long>(snapshot_checksum(resolved)));
        dh2::data::SpawnVitals spawn;if(!dh2::data::initialize_spawn_vitals(property_rules,actor.properties,spawn,error))throw std::runtime_error(error);
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Spawn vitals | %s | character %s | HP raw %d | Max_HP raw %d | MP raw %d | Max_MP raw %d | first HP add raw %d | first MP add raw %d | second HP add raw %d | second MP add raw %d | checksum %016llx | passes 2",actor.name.c_str(),actor.character.c_str(),resolved[36],resolved[38],resolved[41],resolved[43],spawn.first_hp.raw_add,spawn.first_mp.raw_add,spawn.second_hp.raw_add,spawn.second_mp.raw_add,static_cast<unsigned long long>(snapshot_checksum(resolved)));
        dh2::data::CombatantView probe_attacker{resolved.data(),-1,-1,0,0,0,5,0},probe_target{probe_defender.resolved.data(),-1,-1,0,0,0,5,0};
        dh2::data::CombatRandom probe_random{0xD22026u+unsigned(character-character_table.names.begin())*4,0};
        dh2::data::CombatResult probe_result;
        if(dh2_combat_melee(&probe_result,&probe_attacker,&probe_target,&probe_random,0,0)!=0)throw std::runtime_error("Combat result probe failed");
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat result probe | %s | character %s | amount %d | dot element %d | dot duration %d | dot amount %d | HP leech %d | MP leech %d | outcomes %u | mask %u | category %d | element %d | seed %u | calls %u | validation only",actor.name.c_str(),actor.character.c_str(),probe_result.amount,probe_result.dot_element,probe_result.dot_duration,probe_result.dot_amount,probe_result.hp_leech,probe_result.mp_leech,probe_result.outcomes,probe_result.mask,probe_result.weapon_category,probe_result.element,probe_random.seed,probe_random.calls);
        // Run nonlethal/lethal health writes on independent copies. Kill and
        // lifecycle requests are recorded; live actor HP/state is untouched.
        for(const unsigned damage:{256u,16384u}){
          auto copy=actor.properties;auto health_view=dh2::data::property_view(property_rules,copy);
          const dh2::data::HealthRequest request{&health_view,damage,dh2::data::health_game_present|dh2::data::health_main_player_present,0,1};dh2::data::HealthChange change;
          if(dh2_health_hit(&change,&request)!=0)throw std::runtime_error("Health probe failed");
          __android_log_print(ANDROID_LOG_INFO,"DH2Native","Health probe | %s | character %s | damage %u | add %d | before %d | after %d | kill %u | armed %u | cue %u | lifecycle %d | dead skip %u | validation only",actor.name.c_str(),actor.character.c_str(),damage,change.raw_add,change.before,change.after,change.kill_requested,change.low_health_armed,change.low_health_cue,change.lifecycle_write,change.skipped_dead);
        }
        const auto* state=dh2::data::animation_state(animation_tables,animation_table,"Idle");if(!state)throw std::runtime_error("Monster original idle state missing");
        const int sequence=state-animation_tables.sequences.data();if(!actor.scheduler.start(animation_tables,sequence,animation_random,error))throw std::runtime_error(error);
        const auto* path=dh2::data::animation_clip(actor.scheduler.clip(),clip_table);if(!path)throw std::runtime_error("Monster original idle clip path missing");
        const auto separator=path->find_last_of("/\\");const auto filename=path->substr(separator==std::string::npos?0:separator+1);
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original idle selected | %s | table %d | sequence %d | step %u | clip %d | %s | speed %.4f | actor %s",object.model.c_str(),animation_table,sequence,actor.scheduler.frames().back().step,actor.scheduler.clip().anim,filename.c_str(),actor.scheduler.clip().speed,actor.name.c_str());
      };
      if(found!=candidate_groups.end()){start_actor(*found);continue;}
      candidate_groups.emplace_back();auto& group=candidate_groups.back();group.animation_table=animation_table;start_actor(group);
      auto model=read(assets,object.model,"actors");std::vector<std::uint8_t> clip;
      if(object.kind==1){
        const auto* path=dh2::data::animation_clip(group.instances.front().scheduler.clip(),clip_table);const auto separator=path->find_last_of("/\\");clip=read(assets,path->substr(separator==std::string::npos?0:separator+1),"actors");
      }
      if(!dh2::objects::load_resource(model.data(),model.size(),clip.empty()?nullptr:clip.data(),clip.size(),group.resource,error))throw std::runtime_error(object.model+": "+error);
      if(object.kind==1){
        std::set<int> ids;std::function<void(int,unsigned)> collect=[&](int id,unsigned depth){
          if(depth>=3||id<0||unsigned(id)>=animation_tables.sequences.size())throw std::runtime_error("Actor clip bank redirect outside limit");
          for(const auto& step:animation_tables.sequences[id].steps){if(step.redir==1)collect(step.anim,depth+1);else if(step.anim>=0)ids.insert(step.anim);else throw std::runtime_error("Actor clip bank contains an empty clip");}
        };
        for(const auto* state_name:{"Idle","Walk","Attack","Died"}){const auto* state=dh2::data::animation_state(animation_tables,animation_table,state_name);if(!state)throw std::runtime_error("Actor clip bank state missing");collect(state-animation_tables.sequences.data(),0);}
        for(int id:ids){dh2::data::AnimationStep ref;ref.anim=id;const auto* path=dh2::data::animation_clip(ref,clip_table);if(!path)throw std::runtime_error("Actor clip bank path missing");const auto separator=path->find_last_of("/\\");auto raw_clip=read(assets,path->substr(separator==std::string::npos?0:separator+1),"actors");auto& playback=group.clips[id];
          if(!playback.load(raw_clip.data(),raw_clip.size(),group.resource.rest_scene,error,dh2::animation::MissingTargets::ignore)||playback.end<=playback.start)throw std::runtime_error("Actor clip bank: "+error);
          __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor event track ready | %s | clip %d | groups %u | attack_mainhand ms %d",object.model.c_str(),id,playback.events.view().count,dh2_events_time(&playback.events.view(),"attack_mainhand"));
        }
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor clip bank ready | %s | clips %zu | independent instance clocks",object.model.c_str(),group.clips.size());
      }
      for(const auto& primitive:group.resource.primitives){Draw d;d.node=primitive.node;d.material=group.resource.scene.materials.at(primitive.material);group.draws.push_back(std::move(d));auto& batch=group.draws.back();
        batch.diffuse=upload(assets,batch.material.diffuse,cache,textures);batch.alpha=upload(assets,batch.material.alpha_map,cache,textures);batch.count=primitive.indices.size();
        glGenBuffers(1,&batch.vertices);glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);glBufferData(GL_ARRAY_BUFFER,primitive.vertices.size()*sizeof(Vertex),primitive.vertices.data(),primitive.skin.nodes.empty()?GL_STATIC_DRAW:GL_DYNAMIC_DRAW);
        glGenBuffers(1,&batch.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,batch.indices);glBufferData(GL_ELEMENT_ARRAY_BUFFER,primitive.indices.size()*2,primitive.indices.data(),GL_STATIC_DRAW);check("Object buffer upload");
      }
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Object resource %s | primitives %zu | tracks %u | unbound %u | unsupported %u | removed helpers %u",object.model.c_str(),group.draws.size(),group.resource.animation.track_count(),group.resource.animation.unbound,group.resource.animation.skipped,group.resource.removed_helpers);
    }
    for(const auto& instance:candidate.scene.instances){dh2::assets::Mesh mesh{};
      if(dh2_mesh_open(&mesh,&view,instance.geometry)!=dh2::assets::Error::ok||mesh.primitives!=instance.materials.size())throw std::runtime_error("World mesh/binding rejected");
      for(unsigned j=0;j<mesh.primitives;++j){dh2::assets::Primitive p{};dh2_mesh_primitive(&mesh,j,&p);
        if(p.collada_type||p.index_count%3||mesh.vertices>65536)throw std::runtime_error("World topology rejected");
        Draw batch;batch.environment=true;batch.placement=instance.world;batch.material=candidate.scene.materials.at(instance.materials[j]);
        if(batch.material.id!=p.material)throw std::runtime_error("World visual binding differs");
        environment.push_back(std::move(batch));auto& d=environment.back();d.diffuse=upload(assets,d.material.diffuse,cache,textures);d.alpha=upload(assets,d.material.alpha_map,cache,textures);
        dh2::assets::Attribute position_attribute{},uv{},color_attribute{};
        if(dh2_mesh_attribute(&mesh,p.attributes[0],&position_attribute)!=dh2::assets::Error::ok||position_attribute.components<3)throw std::runtime_error("World position missing");
        const bool have_uv=dh2_mesh_attribute(&mesh,p.attributes[4],&uv)==dh2::assets::Error::ok&&uv.components>=2;
        const bool have_color=dh2_mesh_attribute(&mesh,p.attributes[2],&color_attribute)==dh2::assets::Error::ok;
        std::vector<Vertex> vertices(mesh.vertices);for(unsigned k=0;k<mesh.vertices;++k){float v[4]{};dh2_attribute_read(&position_attribute,k,v);std::copy(v,v+3,vertices[k].p);
          if(have_uv){dh2_attribute_read(&uv,k,v);std::copy(v,v+2,vertices[k].uv);}std::fill(vertices[k].color,vertices[k].color+4,1);
          if(have_color){dh2_attribute_read(&color_attribute,k,v);for(unsigned c=0;c<color_attribute.components;++c)vertices[k].color[c]=v[c]/(color_attribute.type==1?255.f:1.f);}}
        std::vector<std::uint16_t> indices(p.index_count);for(unsigned k=0;k<p.index_count;++k){unsigned index;dh2_index_read(&p,k,&index);indices[k]=index;}
        d.count=p.index_count;triangles+=p.index_count/3;glGenBuffers(1,&d.vertices);glBindBuffer(GL_ARRAY_BUFFER,d.vertices);glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),GL_STATIC_DRAW);
        glGenBuffers(1,&d.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,d.indices);glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*2,indices.data(),GL_STATIC_DRAW);check("World buffer upload");
      }
    }
    const auto actor_report=load(prince.data(),prince.size(),assets);
    if(actor_report.find("Model load failed:")==0)throw std::runtime_error(actor_report);
    environment.insert(environment.end(),std::make_move_iterator(draws.begin()),std::make_move_iterator(draws.end()));draws=std::move(environment);
    images.insert(images.end(),textures.begin(),textures.end());textures.clear();
    object_groups=std::move(candidate_groups);world_objects=std::move(object_records);unsigned monsters=0,decors=0,object_triangles=0,object_draws=0;
    actor_animation_tables=std::move(animation_tables);actor_clip_table=std::move(clip_table);actor_random=restore?previous_random:animation_random;actor_property_rules=property_rules;actor_ai_tables=std::move(ai_tables);
    bool combat_resumed=false;
    if(restore){for(auto& group:object_groups)for(auto& actor:group.instances)if(actor.kind==1){
      auto saved=std::find_if(saved_actors.begin(),saved_actors.end(),[&](const ObjectActor& old){return old.room==actor.room&&old.name==actor.name&&old.model==actor.model;});
      if(saved!=saved_actors.end()){
        if(!group.clips.count(saved->scheduler.clip().anim))throw std::runtime_error("Restored actor clip is not bundled");
        actor=*saved;combat_resumed|=actor.combat_target!=-1||actor.combat_state.dead;
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat actor restored | %s | HP %d | dead %u | combo %u | state %s | cursor %.4f | events %u | aggro %u %u | target %d | AI attack %d",actor.name.c_str(),actor.properties.resolved[36],actor.combat_state.dead,actor.combat_state.combo_hits,actor.state.c_str(),actor.cursor,actor.animation_events,actor.aggro.out_count,actor.aggro.in_count,actor.combat_target,actor.ai_attack);
      }
    }}else{combat_random={0xD22026u,0};combat_hits=0;}
    saved_actors.clear();
    if(!restore)prince_combat=std::move(fresh_player);
    // Detach before replacing the explicit immutable bank owner.
    prince_locomotion=dh2::actor::BlendedPlayback{};
    prince_attack_clips=std::move(candidate_player_clips);prince_animation_bank=std::move(candidate_bank);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Player properties | KnightPlayerBase | HP %d | MP %d | checksum %016llx | attempts %u | attacking %d",prince_combat.properties.resolved[36],prince_combat.properties.resolved[41],static_cast<unsigned long long>(snapshot_checksum(prince_combat.properties.resolved)),prince_combat.attempts,int(prince_state.current==5));
    for(const auto& group:object_groups)for(const auto& object:group.instances){monsters+=object.kind==1;decors+=object.kind==2;object_triangles+=group.resource.triangles;object_draws+=group.draws.size();}
    level=std::move(candidate);player=std::move(candidate_idle);walk_player=std::move(candidate_walk);current_scene=std::move(rest);
    actor_position=restore?previous:level.spawn;
    world_mode=true;resume_world=false;walking=false;move_x=move_y=0;heading=restore?previous_heading:0;movement_steps=blocked_steps=0;
    radius=350;yaw=-1.57f;pitch=.75f;zoom=1;object_epoch=epoch=last_frame=std::chrono::steady_clock::now();sampled_ms=0;frozen=false;
    initialize_native_actor(assets,restore);
    char report[256];std::snprintf(report,sizeof(report),"Crypt | %u rooms | %u monsters | %u scenery objects\n%u triangles. Drag the movement control to walk.",level.rooms,monsters,decors,triangles+586+object_triangles);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Objects ready | monsters %u | decors %u | resources %zu | instance draws %u | triangles %u | character records %zu | model entries %zu | idle preview only",monsters,decors,object_groups.size(),object_draws,object_triangles,character_table.rows.size(),model_table.values.size());
    if(level.native_floor)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Native floors ready | records %zu | graph nodes %u | graph edges %u | selector collision controls height",level.native_floor->records.size(),level.native_floor->graph.node_count,level.native_floor->graph.edge_count);
    if(level.native_floor&&level.native_floor->sewn)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Native floor links ready | neighbour relations %u | validation references %u | graph-node search ready",level.native_floor->sewing.link_count,level.native_floor->graph.validation_count);
    if(level.native_floor&&level.native_floor->sewn){
      auto& floors=*level.native_floor;const auto& graph=floors.graph;
      std::vector<unsigned> first(floors.records.size()),last(floors.records.size()),path(graph.node_count);unsigned pairs=0,successful=0,segments=0;std::uint64_t digest=0xcbf29ce484222325ull;
      for(unsigned i=0;i<graph.node_count;++i){const auto& node=graph.nodes[i];if(!first[node.floor])first[node.floor]=node.id;last[node.floor]=node.id;}
      const auto append=[&](unsigned value){for(unsigned k=0;k<4;++k){digest^=(value>>(k*8))&255;digest*=0x100000001b3ull;}};
      const auto goal=[](void* target,unsigned id)->unsigned{return id==*static_cast<unsigned*>(target);};const auto valid=[](void*,unsigned)->unsigned{return 1;};
      for(unsigned a=0;a<first.size();++a)for(unsigned b=0;b<last.size();++b){
        unsigned target=last[b];const dh2::navigation::SearchTest test{goal,valid,valid,&target};dh2::navigation::SearchResult result{0,0,0,0,0,0,path.data(),unsigned(path.size()),0};
        if(!first[a]||!target||dh2::floors::search_nodes(floors,first[a],10000,test,result)!=0)throw std::runtime_error("Native graph route probe rejected");
        ++pairs;successful+=result.found!=0;segments+=result.path_count;append(first[a]);append(target);append(result.found);append(result.expanded);append(result.edges_examined);append(result.candidate_relaxations);append(result.non_goal_enqueues);append(result.path_count);
        for(unsigned i=0;i<result.path_count;++i)append(path[i]);
      }
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native graph route probe | floor pairs %u | successful %u | segments %u | state %016llx | endpoints and movement pending",pairs,successful,segments,static_cast<unsigned long long>(digest));
      dh2::floors::clear_route_cache(floors);pairs=successful=segments=0;digest=0xcbf29ce484222325ull;unsigned direct=0,graph_routes=0;path.resize(graph.node_count+1);dh2::navigation::RouteObject object{};
      const auto centroid=[](const dh2::collision::Triangle& triangle,float* point){for(unsigned k=0;k<3;++k)point[k]=static_cast<float>((double(triangle.points[0][k])+double(triangle.points[1][k])+double(triangle.points[2][k]))/3.);};
      for(unsigned a=0;a<floors.records.size();++a)for(unsigned b=0;b<floors.records.size();++b){
        float source[3],target[3];centroid(floors.records[a]->triangles.front(),source);centroid(floors.records[b]->triangles.back(),target);
        dh2::navigation::RouteResult result{0,0,0,0,{0,0,0,0,0,0,path.data(),unsigned(path.size()),0}};
        if(dh2::floors::route(floors,source,target,10000,&object,result)!=0)throw std::runtime_error("Native world route probe rejected");
        ++pairs;successful+=result.found!=0;direct+=result.kind==1;graph_routes+=result.kind==2;segments+=result.search.path_count;
        append(a);append(b);append(result.found);append(result.source);append(result.target);append(result.kind);append(result.search.found);append(result.search.expanded);append(result.search.edges_examined);append(result.search.candidate_relaxations);append(result.search.non_goal_enqueues);append(result.search.path_count);
        for(unsigned i=0;i<result.search.path_count;++i)append(path[i]);
      }
      dh2::floors::clear_route_cache(floors);
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native world route probe | floor pairs %u | successful %u | direct %u | graph %u | segments %u | state %016llx | smoothing and movement pending",pairs,successful,direct,graph_routes,segments,static_cast<unsigned long long>(digest));
      pairs=successful=segments=0;digest=0xcbf29ce484222325ull;unsigned owned=0;
      std::vector<dh2::navigation::PathSegment> refined(graph.node_count+1);dh2::navigation::PathObject actor{};actor.segments=refined.data();actor.capacity=refined.size();
      const auto append_bytes=[&](const void* raw,unsigned count){const auto* bytes=static_cast<const unsigned char*>(raw);for(unsigned i=0;i<count;++i){digest^=bytes[i];digest*=0x100000001b3ull;}};
      for(unsigned a=0;a<floors.records.size();++a)for(unsigned b=0;b<floors.records.size();++b){
        float target[3];centroid(floors.records[a]->triangles.front(),actor.position);centroid(floors.records[b]->triangles.back(),target);
        dh2::navigation::RouteResult result{0,0,0,0,{0,0,0,0,0,0,path.data(),unsigned(path.size()),0}};
        if(dh2::floors::find_path(floors,actor,target,10000,result)!=0)throw std::runtime_error("Native FindPath probe rejected");
        ++pairs;successful+=result.found!=0;owned+=actor.owned;segments+=actor.count;append(a);append(b);append_bytes(&result,40);append_bytes(path.data(),result.search.path_count*4);
        append_bytes(&actor,68);append(actor.count);append(actor.owned);append_bytes(actor.segments,actor.count*sizeof(dh2::navigation::PathSegment));
      }
      dh2::floors::clear_route_cache(floors);
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native FindPath probe | floor pairs %u | successful %u | owned %u | segments %u | state %016llx | position controller pending",pairs,successful,owned,segments,static_cast<unsigned long long>(digest));
      pairs=successful=0;digest=0xcbf29ce484222325ull;unsigned accepted=0,direction_valid=0;
      const dh2::navigation::MotionPolicy policy{1000.f,0};
      for(unsigned a=0;a<floors.records.size();++a)for(unsigned b=0;b<floors.records.size();++b){
        float source[3],target[3],point[3],direction[3];centroid(floors.records[a]->triangles.front(),source);centroid(floors.records[b]->triangles.back(),target);
        for(unsigned k=0;k<3;++k){point[k]=target[k];direction[k]=target[k]-source[k];}point[2]+=36;
        dh2::navigation::MotionObject object{0,8,a,a,{},{}};std::copy(source,source+3,object.position);dh2::navigation::PositionResult result{};
        const dh2::navigation::DirectionRequest request{&floors.collision_world,source,36.f,0,0};unsigned valid=0;
        if(dh2_nav_validate_position(&result,&floors.collision_world,&object,point,&policy)!=0||dh2_nav_validate_direction(&valid,direction,&request)!=0)throw std::runtime_error("Native motion probe rejected");
        ++pairs;successful+=result.valid;accepted+=result.kind==2;direction_valid+=valid;append(a);append(b);append_bytes(&result,24);append_bytes(point,12);append_bytes(&object,40);append(valid);append_bytes(direction,12);
      }
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native floor motion probe | floor pairs %u | position valid %u | accepted %u | direction valid %u | state %016llx | controller and dynamic obstacles pending",pairs,successful,accepted,direction_valid,static_cast<unsigned long long>(digest));
      pairs=accepted=0;digest=0xcbf29ce484222325ull;unsigned registered=0,relocated=0;
      for(unsigned a=0;a<floors.records.size();++a)for(unsigned b=0;b<floors.records.size();++b){
        dh2::navigation::NavigationObject object;dh2::navigation::ObstacleEntry entries[2]{};unsigned buckets[2]{};
        dh2::navigation::ObstacleRegistry registry{entries,0,2,buckets,0,2};float source[3],target[3],point[3];
        centroid(floors.records[a]->triangles.front(),source);centroid(floors.records[b]->triangles.back(),target);std::copy(target,target+3,point);point[2]+=36;
        const std::uint64_t key=0x100000001ull+a;
        dh2::navigation::ObjectInitRequest init{&floors.collision_world,&object,17+a,{},36,0,0};std::copy(source,source+3,init.position);
        const dh2::navigation::ObstacleInitRequest obstacle{&floors.collision_world,&registry,&object,key,1,36,1,0};
        const dh2::navigation::ObjectPositionRequest position{&floors.collision_world,&registry,&object,key,point,&policy};dh2::navigation::PositionResult result{};
        if(dh2_nav_object_defaults(&object)||dh2_nav_init_object(&init)||dh2_nav_init_obstacle(&obstacle)||dh2_nav_validate_object_position(&result,&position))throw std::runtime_error("Native obstacle registry probe rejected");
        ++pairs;accepted+=result.kind==2;registered+=registry.count==1;relocated+=result.parent_change!=0;
        append(a);append(b);append_bytes(&result,24);append_bytes(point,12);append_bytes(&object,64);append(registry.floor_count);append(registry.count);
        std::sort(buckets,buckets+registry.floor_count);append_bytes(buckets,registry.floor_count*4);
        for(unsigned floor=0;floor<registry.floor_count;++floor)for(unsigned i=0;i<registry.count;++i)if(entries[i].floor==buckets[floor])append_bytes(&entries[i],16);
      }
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native obstacle registry probe | floor pairs %u | accepted %u | registered %u | relocated %u | state %016llx | forces and controller pending",pairs,accepted,registered,relocated,static_cast<unsigned long long>(digest));
      digest=0xcbf29ce484222325ull;unsigned tested_floors=0,contributions=0,adjusted=0,turn_limited=0;
      for(unsigned floor=0;floor<floors.records.size();++floor){
        float source[3];centroid(floors.records[floor]->triangles.front(),source);
        dh2::navigation::AvoidanceActor actors[8]{};std::uint64_t keys[8];dh2::navigation::ObstacleEntry entries[8]{};unsigned buckets[2]{floor,(floor+1)%unsigned(floors.records.size())};
        dh2::navigation::ObstacleRegistry registry{entries,8,8,buckets,2,2};const float offsets[8][2]{{0,0},{30,10},{35,-15},{120,0},{20,5},{25,10},{20,-10},{30,0}};
        for(unsigned i=0;i<8;++i){
          auto& a=actors[i];keys[i]=0x100000001ull+i;dh2_nav_object_defaults(&a.object);const unsigned f=i==7?buckets[1]:floor;
          a.object.motion.room=a.object.motion.floor=f;a.object.motion.object_flags=i==5?8:i==6?4:14;a.object.user=keys[i];a.object.radius=36;a.object.obstacle_weight=1;a.object.obstacle_extent=36;
          for(unsigned k=0;k<3;++k){a.object.motion.position[k]=source[k]+(k<2?offsets[i][k]:0);a.target[k]=source[k]+(k==0?1000:0);a.path_target[k]=source[k]+(k==0?300:0);}a.has_path=1;
          a.physical={1,0,0,1,{std::int16_t(i==0||i==4?-3:0),1,65535,1},{0,0,0,0}};entries[i]={f,0,keys[i]};
        }
        const dh2::navigation::AvoidanceScene scene{&registry,actors,keys,8,0};dh2::navigation::ObstacleForce records[8]{};dh2::navigation::ForceBuffer buffer{records,0,8};const dh2::navigation::AvoidanceRequest request{&scene,keys[0],&buffer};dh2::navigation::ForceResult force{};
        if(dh2_nav_obstacle_force(&force,&request))throw std::runtime_error("Native force probe rejected");
        append(floor);append_bytes(&force,16);append_bytes(records,buffer.count*24);contributions+=force.count;buffer.count=0;float direction[3]{100,0,0};dh2::navigation::AvoidanceResult result{};
        if(dh2_nav_avoid_obstacles(&result,direction,&request))throw std::runtime_error("Native avoidance probe rejected");
        ++tested_floors;adjusted+=result.adjusted;turn_limited+=result.turn_limited;append_bytes(&result,32);append_bytes(direction,12);append_bytes(records,buffer.count*24);append(registry.floor_count);append(registry.count);
        std::sort(buckets,buckets+registry.floor_count);append_bytes(buckets,registry.floor_count*4);for(unsigned f=0;f<registry.floor_count;++f)for(unsigned i=0;i<registry.count;++i)if(entries[i].floor==buckets[f])append_bytes(&entries[i],16);
      }
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native avoidance probe | floors %u | force contributions %u | adjusted %u | turn limited %u | state %016llx | actor producers and controller pending",tested_floors,contributions,adjusted,turn_limited,static_cast<unsigned long long>(digest));
      digest=0xcbf29ce484222325ull;unsigned producer_floors=0,producer_registered=0,physical_radius_updates=0;
      for(unsigned floor=0;floor<floors.records.size();++floor){
        float source[3];centroid(floors.records[floor]->triangles.front(),source);
        dh2::navigation::NavigationObject object;dh2_nav_object_defaults(&object);
        object.user=17;object.motion.room=object.motion.floor=floor;object.radius=36;std::copy(source,source+3,object.motion.position);
        dh2::navigation::ObstacleEntry entries[1]{};unsigned buckets[1]{};
        dh2::navigation::ObstacleRegistry registry{entries,0,1,buckets,0,1};
        const dh2::navigation::ProducerFields fields{dh2::navigation::ProducerClass::character,1,.36f,0,{-36,-24},{36,24}};
        const dh2::navigation::ProducerRequest request{&floors.collision_world,&registry,&object,0x100000001ull,&fields};
        if(dh2_nav_update_game_object(&request))throw std::runtime_error("Native actor producer probe rejected");
        ++producer_floors;producer_registered+=bool(object.motion.object_flags&4);physical_radius_updates+=object.radius==36.f;
        append(floor);append_bytes(&object,64);append(registry.floor_count);append(registry.count);append_bytes(buckets,registry.floor_count*4);append_bytes(entries,registry.count*16);
      }
      __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native actor producer probe | floors %u | registered %u | physical radius updates %u | state %016llx | physical construction and controller pending",producer_floors,producer_registered,physical_radius_updates,static_cast<unsigned long long>(digest));
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native heading control | player movement and melee facing use recovered source | UpdatePath rotation subobjects and genuine physics active");
    }
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","World ready | rooms %u | visual draws %zu | navigation triangles %zu | idle tracks %u | walk tracks %u | position %.4f %.4f %.4f",level.rooms,draws.size(),level.floor.size(),player.track_count(),walk_player.track_count(),actor_position[0],actor_position[1],actor_position[2]);return std::string(report)+(combat_resumed?"\nNative combat resumed":"");
  }catch(const std::exception& e){native_actor_ready=false;actor_world.clear();prince_body={};enabled=false;world_mode=false;release_objects(candidate_groups);release(environment,textures);__android_log_print(ANDROID_LOG_ERROR,"DH2Native","World load failed: %s",e.what());return std::string("World load failed: ")+e.what();}
}
void draw(int width,int height){
  if(!enabled||!program)return;
  float frame_dt=0;
  if(world_mode){const auto now=std::chrono::steady_clock::now();
    const auto now_ms=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
    const auto previous_ms=std::uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(last_frame.time_since_epoch()).count());
    const unsigned dt_ms=now_ms-previous_ms;last_frame=now;
    // The original Application skips updates after a real-time gap >2000ms.
    // Its normal dt is unsigned milliseconds, with one world Step and no cap.
    if(dt_ms<=2000){if(!native_actor_ready)throw std::runtime_error("Native actor runtime is not initialized");advance_native_actor(dt_ms);frame_dt=float(dt_ms)*.001f;}
    const auto& target=inspected_object>=0?world_objects[inspected_object].position:actor_position;
    center[0]=target[0];center[1]=target[1];center[2]=target[2]+90;
  }
  const auto& playback=player;
  if(world_mode)sampled_ms=prince_locomotion.current_timeline().current_ms;
  if(playback.track_count()&&!animation_failed&&!world_mode){
    if(!frozen){const auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-epoch).count();sampled_ms=playback.start+elapsed%(playback.end-playback.start);}
    std::string error;if(!playback.sample(current_scene,sampled_ms,error)){animation_failed=true;__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Animation sample failed: %s",error.c_str());}
    if(world_mode&&!prince_visual.update_world(current_scene,error))throw std::runtime_error(error);
  }
  const auto projection=camera(width,height);glUseProgram(program);
  glUniform1i(glGetUniformLocation(program,"diffuse"),0);glUniform1i(glGetUniformLocation(program,"alpha_map"),1);
  glEnable(GL_DEPTH_TEST);glDepthFunc(GL_LEQUAL);glEnable(GL_BLEND);
  glEnableVertexAttribArray(position);glEnableVertexAttribArray(texcoord);glEnableVertexAttribArray(color);
  auto submit=[&](const Draw& b,const Matrix& transform){
    glUniformMatrix4fv(mvp,1,GL_FALSE,transform.data());
    b.material.backface?glEnable(GL_CULL_FACE):glDisable(GL_CULL_FACE);glCullFace(GL_BACK);glFrontFace(GL_CCW);
    glBlendFunc(GL_SRC_ALPHA,b.material.additive?GL_ONE:GL_ONE_MINUS_SRC_ALPHA);glDepthMask(b.material.additive?GL_FALSE:GL_TRUE);
    glUniformMatrix4fv(texture_matrix,1,GL_FALSE,b.material.texture_matrix);glUniform4fv(material_color,1,b.material.color);
    glUniform1f(has_alpha,b.material.alpha_map.empty()?0:1);glUniform1f(alpha_ref,b.material.alpha_ref);
    glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,b.diffuse);glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,b.alpha);
    glBindBuffer(GL_ARRAY_BUFFER,b.vertices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,b.indices);
    glVertexAttribPointer(position,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,p)));
    glVertexAttribPointer(texcoord,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,uv)));
    glVertexAttribPointer(color,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<void*>(offsetof(Vertex,color)));glDrawElements(GL_TRIANGLES,b.count,GL_UNSIGNED_SHORT,nullptr);
  };
  for(auto& b:draws){
    if(!b.skin.nodes.empty()){
      std::string error;std::vector<dh2::skinning::Matrix> matrices;std::vector<std::array<float,3>> deformed;
      if(!dh2::skinning::palette(b.skin,current_scene,matrices,error)||!dh2::skinning::positions(b.skin,matrices,b.rest_positions,deformed,error)){
        __android_log_print(ANDROID_LOG_ERROR,"DH2Native","Skin sample failed: %s",error.c_str());enabled=false;return;}
      for(unsigned k=0;k<b.cpu_vertices.size();++k)std::copy(deformed[k].begin(),deformed[k].end(),b.cpu_vertices[k].p);
      glBindBuffer(GL_ARRAY_BUFFER,b.vertices);glBufferSubData(GL_ARRAY_BUFFER,0,b.cpu_vertices.size()*sizeof(Vertex),b.cpu_vertices.data());
    }
    // Native actor joints already include owner * helper * authored graph.
    const auto transform=b.environment?dh2::scene::multiply(projection,b.placement):b.skin.nodes.empty()?dh2::scene::multiply(projection,current_scene.graph[b.node].world):projection;submit(b,transform);
  }
  if(world_mode)for(auto& group:object_groups){
    std::string error;
    auto render_actor=[&](const ObjectActor& instance){for(unsigned i=0;i<group.draws.size();++i){const auto& primitive=group.resource.primitives[i];const auto& batch=group.draws[i];
      if(!primitive.skin.nodes.empty()){glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);glBufferSubData(GL_ARRAY_BUFFER,0,primitive.vertices.size()*sizeof(Vertex),primitive.vertices.data());}
      auto transform=dh2::scene::multiply(projection,instance.placement);if(primitive.skin.nodes.empty())transform=dh2::scene::multiply(transform,group.resource.scene.graph[primitive.node].world);submit(batch,transform);
    }};
    if(group.animation_table<0){
      const auto& clip=group.resource.animation;int ms=clip.start;
      if(clip.track_count()){const auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-object_epoch).count();ms=frozen?std::clamp(sampled_ms,clip.start,clip.end):clip.start+elapsed%(clip.end-clip.start);}
      if(!dh2::objects::sample(group.resource,ms,error)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Object sample failed: %s",error.c_str());enabled=false;return;}
      for(const auto& instance:group.instances)render_actor(instance);
    }else for(auto& actor:group.instances){
      update_enemy(actor,group.animation_table);
      if(actor.pending_death){
        auto* sequence=dh2::data::animation_state(actor_animation_tables,group.animation_table,"Died");
        if(!sequence||!actor.scheduler.start(actor_animation_tables,sequence-actor_animation_tables.sequences.data(),actor_random,error)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Combat death animation failed");enabled=false;return;}
        actor.cursor=0;actor.completions=0;actor.state="Died";actor.event_cursor={};actor.animation_events=0;actor.pending_death=false;
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat death animation selected | %s | clip %d | dead %u",actor.name.c_str(),actor.scheduler.clip().anim,actor.combat_state.dead);
      }
      double remaining=frozen?0:frame_dt*1000;unsigned events=0;
      while(!frozen&&actor.scheduler.active()){
        const auto& clip=group.clips.at(actor.scheduler.clip().anim);const double duration=clip.end-clip.start,speed=actor.scheduler.clip().speed,needed=(duration-actor.cursor)/speed;
        struct EventContext{ObjectActor* actor;int clip,ms;};
        auto dispatch_events=[&](double next){
          const int previous=clip.start+int(std::clamp(actor.cursor,0.,duration)),current=clip.start+int(std::clamp(next,0.,duration));EventContext context{&actor,actor.scheduler.clip().anim,current};
          return dh2_events_update(&clip.events.view(),&actor.event_cursor,previous,current,clip.start,clip.end,[](const dh2::animation::TriggeredEvent* event,void* raw){
            auto& context=*static_cast<EventContext*>(raw);auto& actor=*context.actor;++actor.animation_events;
            __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor animation event | %s | state %s | clip %d | name %s | lag ms %d | time ms %d | count %u",actor.name.c_str(),actor.state.c_str(),context.clip,event->name,event->lag_ms,context.ms,actor.animation_events);
            // These actors currently have no equipment. CanRangeAttack first
            // checks resolved projectile property 32, then queries inventory.
            // Outer sequence and inner clip steps are distinct original args.
            const auto& frames=actor.scheduler.frames();
            const auto projectile=actor.properties.resolved[32];
            const dh2::data::CombatEventContext combat{actor.state=="Attack"?5:-1,int(frames.front().step),int(frames.back().step),projectile!=-1,projectile};
            dh2::data::CombatEventAction action;
            if(dh2_combat_event_route(&action,&combat,event->name)!=0){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Actor combat route failed");return;}
            if(action.kind!=dh2::data::CombatEventKind::none)
              __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor combat action | %s | state %s | clip %d | name %s | kind %d | sequence %d | attack step %d | offhand %d | capability %u | projectile %d | %s",actor.name.c_str(),actor.state.c_str(),context.clip,event->name,int(action.kind),action.sequence_step,action.attack_step,action.offhand,combat.can_range,combat.projectile,actor.combat_target==-1?"execution pending":"native target request");
            apply_actor_attack(actor,action);
          },&context);
        };
        const double next=remaining<needed?actor.cursor+remaining*speed:duration;
        if(!dispatch_events(next)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Actor event dispatch failed");enabled=false;return;}
        if(remaining<needed){actor.cursor=next;break;}
        remaining-=std::max(0.,needed);if(++events>64||!actor.scheduler.complete(actor_animation_tables,actor_random,error)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Actor completion failed: %s",error.c_str());enabled=false;return;}
        actor.cursor=actor.scheduler.active()?0:duration;actor.event_cursor={};++actor.completions;
        __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actor clip completed | %s | state %s | completions %u | active %d | next clip %d | layers %zu",actor.name.c_str(),actor.state.c_str(),actor.completions,actor.scheduler.active(),actor.scheduler.clip().anim,actor.scheduler.frames().size());
        if(remaining<=0)break;
      }
      const auto& clip=group.clips.at(actor.scheduler.clip().anim);const int ms=frozen?std::clamp(sampled_ms,clip.start,clip.end):clip.start+int(std::clamp(actor.cursor,0.,double(clip.end-clip.start)));
      if(!dh2::objects::sample(group.resource,clip,ms,error)){__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Actor sample failed: %s",error.c_str());enabled=false;return;}
      render_actor(actor);
    }
  }
  glDisableVertexAttribArray(position);glDisableVertexAttribArray(texcoord);glDisableVertexAttribArray(color);
  glDepthMask(GL_TRUE);glDisable(GL_BLEND);glDisable(GL_CULL_FACE);glDisable(GL_DEPTH_TEST);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);glBindBuffer(GL_ARRAY_BUFFER,0);
  const auto e=glGetError();if(e!=GL_NO_ERROR)__android_log_print(ANDROID_LOG_ERROR,"DH2Native","Model draw GL error 0x%04x",e);
  else if(player.track_count()&&frozen&&!animation_failed&&!frozen_cursor_logged){
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Animation frame rendered at %d ms",sampled_ms);frozen_cursor_logged=true;
  }
}
}
