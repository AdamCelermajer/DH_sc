#include "audio_application_manager_v42.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::audio;
unsigned checks{};void check(bool value,const char*why){++checks;if(!value)throw std::runtime_error(why);}
struct Fixture{std::string root;unsigned reads{},gates{},closes{};};
bool asset(void*raw,const char*uri,std::shared_ptr<const std::vector<std::uint8_t>>&bytes,std::string&error){
 auto&f=*static_cast<Fixture*>(raw);++f.reads;const std::string name=uri;
 auto file=f.root+(name.rfind("data/pydata/",0)==0?"/port/engine-audio/reference/source-bindings-v38/":"/.local-inputs/audio-v34/cache/")+name.substr(12);
 std::ifstream stream(file,std::ios::binary);if(!stream){error="Required exact asset "+name;return false;}
 bytes=std::make_shared<const std::vector<std::uint8_t>>(std::istreambuf_iterator<char>(stream),std::istreambuf_iterator<char>());return true;
}
class Control final:public AudioSessionControlOwnerV40{
 Fixture&fixture_;bool closed_{};
public:explicit Control(Fixture&f):fixture_(f){}
 bool tick(std::string&)override{return true;}
 bool shutdown(std::string&)override{closed_=true;++fixture_.closes;return true;}
 bool close_succeeded()const noexcept override{return closed_;}
 bool ready()const noexcept override{return false;}
};
std::unique_ptr<AudioSessionControlOwnerV40>control(void*raw,AudioMixerV34&,AudioClockV40&,AudioLifecycleGateV40&){return std::make_unique<Control>(*static_cast<Fixture*>(raw));}
int main(int argc,char**argv){try{if(argc!=2)return 2;auto fixture=std::make_shared<Fixture>();fixture->root=argv[1];AudioGameplaySourcesV40 sources;sources.context=fixture.get();sources.exact_assets={fixture.get(),asset};
 sources.gates={fixture.get(),[](void*raw,const dh2::sound::VoxPlay3DRequestV2&,dh2::sound::VoxPlay3DResponseV2&){++static_cast<Fixture*>(raw)->gates;return -1;}};
 std::string error;auto owner=AudioApplicationManagerV42::create(sources,fixture,false,error,control,fixture.get());check(bool(owner),error.c_str());check(owner->identity()!=0&&owner->runtime_on_producer()->source_data_initialized(),"actual manager/catalog constructed");check(fixture->reads==3,"exact three constructor streams");
 check(owner->precache_raw_uid(-1,error)&&fixture->reads==3,"negative raw UID source skip");check(owner->precache_raw_uid(999999,error)&&fixture->reads==3,"raw UID out of original bound source skip");
 check(owner->precache_raw_uid(33,error),error.c_str());check(fixture->reads==4,"raw UID33 exact pack sample loaded once without output");
 check(owner->precache_raw_uid(33,error)&&fixture->reads==4,"retained real sample cache slot returns without second read");
 check(!owner->precache_raw_uid(162,error)&&error.find("sfx_chest_opening.wav")!=std::string::npos,"interaction XML162 remains missing and not substituted");
 check(fixture->gates==0,"precache never calls fake Play3D/GS/clock/focus gates");check(owner->set_actual_disabled(true,error),"actual disabled setter");auto reads=fixture->reads;check(owner->precache_raw_uid(162,error)&&fixture->reads==reads,"actual disabled gate precedes load");check(owner->set_actual_disabled(false,error),"actual enabled setter");
 std::string foreign_error;bool foreign_result=true;std::thread foreign([&]{foreign_result=owner->precache_raw_uid(33,foreign_error);});foreign.join();check(!foreign_result,"foreign producer cannot mutate manager");
 AudioApplicationBorrowV42 captured{owner};check(captured.identity()==owner->identity(),"captured nullable shared manager identity");
 std::thread ui([&]{owner->request_output_close();});ui.join();check(!captured.precache_raw_uid(33,error),"independent UI close excludes new precache");check(owner->shutdown(error),error.c_str());check(fixture->closes==1&&owner->runtime_on_producer()==nullptr,"control closed joined and actual runtime released");owner.reset();captured.manager.reset();
 std::cout<<"PASS "<<checks<<" actual parsed manager/raw-UID bank/lifetime checks; explicit fake control, no World/driver acceptance\n";return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
