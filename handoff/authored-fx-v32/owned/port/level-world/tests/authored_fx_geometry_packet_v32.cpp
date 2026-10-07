#include "../authored_fx_geometry_packet_v7.hpp"
#include "../character_authored_resource_v32.hpp"
#include "../character_authored_fx_forces_v4.hpp"
#include "../source_fx_node_matrix_v4.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstring>
using namespace dh2;
static unsigned checks,mesh_packets,particle_packets;
static void check(bool x,const char* why){++checks;if(!x)throw std::runtime_error(why);}
static bool camera(void*,float* m,float* p,std::string&){const float id[]{1.f,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};std::copy_n(id,16,m);p[0]=17;p[1]=29;p[2]=500;return true;}
static bool driver(void*,std::uint32_t& out,std::string&){out=8;return true;}
static void packet(const skinning::VisualDrawPartV6& source,unsigned material,bool particle){fx::AuthoredFxGeometryPacketV7 p;std::string e;check(fx::authored_fx_geometry_packet_v7(source,material,p,e),e.c_str());if(particle)++particle_packets;else ++mesh_packets;const auto& primitive=source.geometry->primitives.front();check(p.vertices.size()==source.positions.size(),"Actual vertex count");check(p.indices.size()==primitive.indices.size(),"Actual index count");check(p.source_color_missing==(primitive.attributes[2]<0),"Source color absence");for(unsigned i=0;i<p.vertices.size();++i)check(!std::memcmp(p.vertices[i].p,source.positions[i].data(),12),"Same source/skinned/baked position bytes");for(unsigned i=0;i<p.indices.size();++i)check(p.indices[i]==primitive.indices[i],"Actual index narrowing");auto wrong=*source.geometry;wrong.primitives.front().engine_type=3;auto bad=source;bad.geometry=&wrong;auto before=p.vertices;check(!fx::authored_fx_geometry_packet_v7(bad,material,p,e),"Impossible collada0/engine3 rejected");check(p.vertices.size()==before.size()&&!std::memcmp(p.vertices.data(),before.data(),p.vertices.size()*sizeof(objects::Vertex)),"Packet rejection atomic");}
int main(int argc,char** argv){try{if(argc!=2)return 1;const std::string dir=argv[1];std::ifstream list(dir+"/resources.txt");check(bool(list),"Actual manifest");std::string name;unsigned accepted=0,required=0;
 while(std::getline(list,name)){if(!name.empty()&&name.back()=='\r')name.pop_back();std::ifstream file(dir+"/"+name,std::ios::binary);check(bool(file),"Actual resource");auto raw=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;fx::CharacterAuthoredResourceFactoryV32 factory({nullptr,camera,driver},forces.factory());auto api=factory.factory();std::shared_ptr<fx::CharacterParticleFxResourceV2> base;scene::Scene unused;std::string e;if(!api.create(api.context,raw,unused,base,e)){++required;continue;}auto resource=std::dynamic_pointer_cast<fx::CharacterAuthoredCompositeFxResourceV4>(base);check(bool(resource),"Actual composite");math::Matrix4f outer{};const float position[]{113,227,331},rotation[]{.1f,.2f,.3f},scale[]{1.2f,.9f,1.1f};fx::source_fx_trs_matrix_v4(outer,position,rotation,scale);bool complete=true;
  for(int ms=0;ms<std::max(600,resource->end_ms()+2000);ms+=32){if(!resource->sample_animation(std::min(ms,resource->end_ms()),e)||!resource->source_scene_frame_v4(ms,32,outer,e)){complete=false;break;}std::vector<fx::CharacterFxMeshDrawSourceV4> meshes;std::vector<fx::CharacterParticleDrawSourceV3> particles;check(resource->mesh_draw_sources_v4(meshes,e)&&resource->draw_sources_v3(particles,e),e.c_str());for(const auto& m:meshes)packet(m.part,m.material,false);for(const auto& p:particles)packet(p.part,p.material,true);}
  if(complete)++accepted;else ++required;
 }
 check(accepted==45&&required==0,"Expected explicit resource coverage");check(mesh_packets&&particle_packets,"Actual mesh and generated particle packets");std::cout<<"PASS "<<checks<<" checks; resources "<<accepted<<"; mesh_packets "<<mesh_packets<<"; particle_packets "<<particle_packets<<"; GPU=false live=false\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}

