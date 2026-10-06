#include "canonical_receiver_transport_v1.hpp"
#include "canonical_cached_file_v1.hpp"
#include "canonical_level_context_v1.hpp"
#include "canonical_level_config_module_v1.hpp"
#include "level_config_publication_v2.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2;using namespace dh2::loader;using namespace dh2::world;
static void check(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
static assets::ZipAssetPackV1 pack(const char* path){
 auto f=std::make_shared<std::ifstream>(path,std::ios::binary|std::ios::ate);check(bool(*f),"cache unavailable");
 assets::ZipBackingV1 b;b.owner=f;b.bytes=std::uint64_t(f->tellg());b.read=[f](std::uint64_t at,void* out,std::size_t n,std::string& e){f->clear();f->seekg(std::streamoff(at));f->read(static_cast<char*>(out),std::streamsize(n));if(!*f){e="cache read failure";return false;}return true;};
 assets::ZipAssetPackV1 z;std::string e;check(z.mount(std::move(b),"com.gameloft.android.GAND.GloftD2SS/files/",e),e);return z;
}
struct Names {std::vector<std::uint8_t> raw;std::vector<std::vector<std::string>> groups;};
static std::shared_ptr<Names> names(const assets::ZipAssetPackV1& z){
 auto out=std::make_shared<Names>();std::string e;bool found=false;check(z.read("data/pydata/sounds_pyarraynames.bin",found,out->raw,e)&&found,e);std::size_t at=0;
 auto word=[&](){check(at<=out->raw.size()&&out->raw.size()-at>=4,"name word range");std::uint32_t v=0;for(unsigned i=0;i<4;++i)v|=std::uint32_t(out->raw[at++])<<(8*i);return v;};
 while(at<out->raw.size()){const auto count=word();check(count<65536,"name count range");std::vector<std::string> group;for(unsigned i=0;i<count;++i){auto n=word();check(n<=out->raw.size()-at,"name text range");group.emplace_back(reinterpret_cast<const char*>(out->raw.data()+at),n);at+=n;}out->groups.push_back(std::move(group));}
 check(out->groups.size()==5&&out->groups.back().size()==183,"original sound-name groups changed");return out;
}
struct Slot {std::shared_ptr<CanonicalLevelContextV1> s_level;};
struct Context {std::shared_ptr<void> provider=std::make_shared<int>(1);std::shared_ptr<Slot> slot=std::make_shared<Slot>();std::shared_ptr<Names> arrays;std::shared_ptr<CanonicalLevelConfigV1> config;unsigned debug{},sets{},constructs{};};
static LevelConfigPublicationBorrowV2 publication(std::shared_ptr<CanonicalLevelContextV1> level,std::shared_ptr<Context> c){
 auto f=level->config_fields();return {f.level_owner,f.config38,f.music11c,f.safezone120,f.ambient124,c->arrays,&c->arrays->groups.back(),[c](std::uintptr_t identity){return c->config&&c->config->identity()==identity?c->config.get():nullptr;}};
}
static bool construct(void* p,const CanonicalFactoryEntryV1& entry,const CanonicalSourceObjectRequestV1& q,CanonicalClassReceiverV1& out,std::string& error){
 auto& c=*static_cast<Context*>(p);++c.constructs;
 if(std::string(entry.name)!="LevelConfig"){error="actual non-LevelConfig construction remains unavailable: ";error+=entry.name;return false;}
 const auto* original=canonical_source_entry_v1(q);if(!original){error="required retained original config source";return false;}
 LevelConfigServicesV1 services;services.owner=c.provider;
 // Explicit Debug continuation fixture; actual source init_post still owns
 // its four calls, one-shot stores, color conversion and SetLevelConfig call.
 services.debug_switch=[&c](const char* key,bool& value,std::string& e){if(std::string(key)!="isTracingLevel"){e="unexpected debug query";return false;}++c.debug;value=false;return true;};
 services.set_level_config=[&c](std::uintptr_t identity,std::string& e){++c.sets;CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_canonical_level_v1({c.slot,&c.slot->s_level},current,e))return false;if(!current){e="actual current Level required";return false;}auto f=current.level()->config_fields();
  LevelConfigPublicationBorrowV2 b{f.level_owner,f.config38,f.music11c,f.safezone120,f.ambient124,c.arrays,&c.arrays->groups.back(),[&c](std::uintptr_t id){return c.config&&c.config->identity()==id?c.config.get():nullptr;}};return level_config_publication_v2(b,identity,e);};
 c.config=std::make_shared<CanonicalLevelConfigV1>(c.provider,services);auto actual=c.config;out=canonical_class_receiver_v1(actual);
 out.init_post=[actual](std::string& e){return actual->init_post(e);};out.is_game_object=[](bool& value,std::string&){value=CanonicalLevelConfigV1::is_game_object();return true;};return true;
}
int main(int argc,char** argv){if(argc!=2)return 2;try{
 auto archive=pack(argv[1]);auto c=std::make_shared<Context>();c->arrays=names(archive);std::string error;unsigned checks=0;
 check(CanonicalLevelContextV1::create({"SWAMP","data/scene/001_swamp.mlx"},c->provider,c->slot->s_level,error),error);auto level=c->slot->s_level;auto fields=level->config_fields();*fields.ambient124=137;
 std::array<float,3> zero{};CanonicalPropertySourceServicesV1 props{nullptr,&zero,[](void*,std::string&){return true;},[](void*,const char*,std::string&){return true;}};
 CanonicalPropertyMapV1 map(props);CanonicalReceiverTransportV1 transport(map,{c,c.get(),construct,nullptr});CanonicalObjectManagerServicesV1 ms;
 ms.assign_network_id=[](void*,CanonicalObjectBorrowV1&,std::string&){return true;}; // declared offline network-assignment fixture
 CanonicalObjectManagerV1 manager(ms);unsigned parses=0;
 CanonicalCachedFileV1 file(archive,manager,transport.services(),{[&](bool ok,std::string&){++parses;return ok;},[](std::string&){return true;}},{c},ObjectEntryRouteV1::level);
 for(unsigned n=0;file.attempts().empty()&&n<100;++n){const auto step=file.step("data/scene/001_swamp.mlx","Level");check(step==LevelFileWalkStepV1::pending,file.error());}
 check(file.attempts().size()==1&&file.attempts()[0]->factory_attempt()->stage()==CanonicalFactoryStageV1::complete&&manager.source_count50()==1&&c->constructs==1&&c->debug==4&&c->sets==1,"actual original LevelConfig did not complete first source prefix");++checks;
 check(*fields.config38==c->config->identity()&&*fields.music11c==-1&&*fields.safezone120==-1&&*fields.ambient124==137,"publication did not use SAME Level fields or unknown/empty sound semantics");
 check(*c->config->string(0x168)=="SwampHubAmbientMusic"&&*c->config->string(0x180)=="SwampMerchantCampMusic"&&c->config->string(0x1b0)->empty(),"original authored music references changed");++checks;
 const auto* source=canonical_source_entry_v1(file.attempts()[0]->source().request());
 if(!(source&&*c->config->string(0x150)==*source->source().attribute("scriptFile")&&(*c->config->vector(0x1e0))[1]==14.f&&(*c->config->vector(0x1cc))[1]==0.5f/255.f))std::cerr<<"script150="<<*c->config->string(0x150)<<" fog1e0="<<(*c->config->vector(0x1e0))[1]<<" ambient1cc="<<(*c->config->vector(0x1cc))[1]<<" source="<<(source!=nullptr)<<'\n';
 check(source&&*c->config->string(0x150)==*source->source().attribute("scriptFile")&&(*c->config->vector(0x1e0))[1]==14.f&&(*c->config->vector(0x1cc))[1]==0.5f/255.f,"actual source script/color or early InitPost conversion changed");++checks;
 check(file.step("data/scene/001_swamp.mlx","Level")==LevelFileWalkStepV1::failed&&file.attempts().size()==2&&c->constructs==2&&*fields.config38==c->config->identity(),"next unavailable Module lost reached config prefix");
 const auto diagnostic=file.error();check(file.step("data/scene/001_swamp.mlx","Level")==LevelFileWalkStepV1::failed&&file.error()==diagnostic&&parses==1&&c->debug==4&&c->sets==1,"failed source replayed actual config initialization");++checks;
 auto b=publication(level,c);*fields.music11c=11;*fields.safezone120=12;*fields.ambient124=13;
 check(!level_config_publication_v2(b,0,error)&&*fields.config38==0&&*fields.music11c==11&&*fields.safezone120==12&&*fields.ambient124==13,"NULL publication lost original pointer-store prefix");++checks;
 auto missing=b;missing.arrays_owner.reset();check(!level_config_publication_v2(missing,c->config->identity(),error)&&*fields.config38==c->config->identity()&&*fields.music11c==11,"missing arrays lost pointer prefix or wrote IDs");++checks;
 auto other=std::make_shared<CanonicalLevelConfigV1>(c->provider,LevelConfigServicesV1{});auto a=other->properties();
 check(a.fields.write_string(a.fields.context,0x168,c->arrays->groups.back()[0],error),error);b.config=[other](std::uintptr_t id){return id==other->identity()?other.get():nullptr;};
 check(level_config_publication_v2(b,other->identity(),error)&&*fields.music11c==0&&*fields.safezone120==12&&*fields.ambient124==13,"optional empty strings reset previous IDs");++checks;
 check(a.fields.write_string(a.fields.context,0x1b0,c->arrays->groups.back()[1],error)&&a.fields.write_string(a.fields.context,0x180,c->arrays->groups.back()[2],error),error);
 check(level_config_publication_v2(b,other->identity(),error)&&*fields.music11c==0&&*fields.ambient124==1&&*fields.safezone120==2,"current original-name catalog IDs/order changed");++checks;
 std::cout<<"{\"validation\":\"PASS\",\"config_source_checks\":"<<checks<<",\"actual_level_config_constructor\":true,\"actual_config_publication\":true,\"sound_names_provider_fixture\":true,\"debug_network_parser_fixtures\":true,\"first_missing_type_after_config\":\"Module\",\"full_loader_verified\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
