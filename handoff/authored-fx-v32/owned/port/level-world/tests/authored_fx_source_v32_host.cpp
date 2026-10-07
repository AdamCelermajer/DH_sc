#include "../../engine-animation/particle_resource_init_v32.hpp"
#include "../../engine-animation/particle_cloud_runtime_v1.hpp"
#include "../authored_fx_nonrender_geometry_v32.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <array>
#include <map>
using namespace dh2;
extern "C" unsigned dh2_fx_source_v32(unsigned,const unsigned char*,unsigned char*);
static unsigned checks;
static void check(bool x,const char* s){++checks;if(!x)throw std::runtime_error(s);}
struct Reader {std::ifstream f;explicit Reader(const std::string& p):f(p,std::ios::binary){check(bool(f),"proof file");}unsigned word(){unsigned w;f.read(reinterpret_cast<char*>(&w),4);check(bool(f),"proof word");return w;}std::vector<unsigned char> bytes(unsigned n){std::vector<unsigned char> v(n);f.read(reinterpret_cast<char*>(v.data()),n);check(bool(f),"proof bytes");return v;}};
static std::shared_ptr<const std::vector<unsigned char>> read(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f),"actual cache");return std::make_shared<const std::vector<unsigned char>>(std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>());}
struct Context {unsigned type{},shape{},renders{};};
int main(int argc,char**argv){try{if(argc!=3)return 1;const std::string cache=argv[1],ref=argv[2];
 Reader gold(ref+"/authored-fx-v32-source-gold.bin");check(gold.word()==0x32335846,"gold magic");unsigned count=gold.word();
 for(unsigned i=0;i<count;++i){unsigned op=gold.word();auto input=gold.bytes(gold.word());auto expected=gold.bytes(gold.word());std::vector<unsigned char> output(expected.size());check(dh2_fx_source_v32(op,input.data(),output.data())==expected.size(),"source output size");check(output==expected,"original instruction byte identity");}
 auto raw=read(cache+"/asset_03.bdae");animation::ParticleEmitterInput emitter;std::string error;check(animation::decode_particle_emitter(raw,1,emitter,error),error.c_str());auto owner=animation::ParticleGenerationOwner::create({});animation::ParticleCloudModelsV1 models{};check(animation::register_particle_cloud_models_v1(*owner,models),"same model registry");
 Reader init(ref+"/authored-fx-v32-init-gold.bin");std::map<std::string,std::vector<unsigned>> expected;for(unsigned n=init.word();n;--n){unsigned size=init.word(),words=init.word();auto name=init.bytes(size),value=init.bytes(words*4);std::vector<unsigned> v(words);std::memcpy(v.data(),value.data(),value.size());expected.emplace(reinterpret_cast<const char*>(name.data()),std::move(v));}
 std::map<std::string,std::array<unsigned,3>> missing;
 for(const auto& row:expected)if(row.first!="EmitterType"&&row.first!="RadiusLength"){auto& cell=missing[row.first];owner->register_parameter(owner->hash_name(row.first.c_str()),cell.data());}
 unsigned mapping=~0u;check(owner->register_parameter(owner->hash_name("AnimKeyMappingType"),&mapping),"mapping cell");Context context;
 animation::ParticleGenerationInitServices generation{&context,[](void* p,animation::ParticleGenerationOwner&,unsigned type){static_cast<Context*>(p)->type=type;return 0;},[](void* p,animation::ParticleGenerationOwner&,const char* name,unsigned value){check(std::strcmp(name,"RadiusLength")==0,"actual sphere prefix");static_cast<Context*>(p)->shape=value;return 0;}};
 animation::ParticleResourceInitServicesV2 services{&context,[](void*,animation::ParticleGenerationOwner& o,const char*name,const unsigned value[3]){auto lease=o.parameter(name);check(bool(lease),"same vector parameter");std::memcpy(lease.storage,value,12);return 0;},[](void* p,animation::ParticleGenerationOwner&,const animation::ParticleEmitterInput&){++static_cast<Context*>(p)->renders;return 0;}};
 check(!animation::initialize_particle_resource_v32(*owner,emitter,generation,services),"Dark Queen source initializer");check(context.renders==1,"render stage once");
 for(const auto& row:expected){if(row.first=="EmitterType")check(context.type==row.second[0],"source emitter type");else if(row.first=="RadiusLength")check(context.shape==row.second[0],"source shape");else {auto value=owner->parameter(row.first.c_str());check(bool(value),"retained parameter");check(!std::memcmp(value.storage,row.second.data(),row.second.size()*4),"original initializer parameter identity");}}
 check(mapping==emitter.record[0x64/4],"source mapping enum");auto rejected=emitter;rejected.record[0x88/4]=1;const auto before=models;check(animation::initialize_particle_resource_v32(*owner,rejected,generation,services)==-1,"unsupported axis1 continuation");check(!std::memcmp(&before,&models,sizeof models)&&context.renders==1,"rejected initializer atomic");
 for(unsigned index=5;index<10;++index){auto bytes=read(cache+"/asset_0"+std::to_string(index)+".bdae");resources::BresView image{};check(dh2_bres_open(&image,bytes->data(),bytes->size())==resources::BresError::ok,"source BRES");fx::AuthoredFxNonrenderGeometryV32 declaration;bool nonrender=false;check(fx::authored_fx_nonrender_geometry_v32(image,0,declaration,nonrender,error)&&nonrender,"same original null mesh declaration");check(declaration.fields[1]==15&&declaration.fields[2]==3,"retained declaration metadata");
  auto corrupt=*bytes;unsigned kind=2;const auto* rec=dh2_bres_library_item(&image,resources::Library::geometry,0);std::memcpy(corrupt.data()+(rec-image.bytes)+8,&kind,4);image.bytes=corrupt.data();check(!fx::authored_fx_nonrender_geometry_v32(image,0,declaration,nonrender,error),"unknown geometry remains required");
 }
 std::cout<<"PASS "<<checks<<" checks; original_records "<<count<<"; GPU=false live=false\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}

