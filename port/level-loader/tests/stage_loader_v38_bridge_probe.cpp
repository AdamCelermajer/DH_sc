#include "canonical_object_manager_v1.hpp"
#include "stage_loader_v38_stage10.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh2::loader;using namespace dh2::world;
static void check(bool b,const char* why){if(!b)throw std::runtime_error(why);}
// Explicit preparation API-shape fixtures, not real Module effects.
struct Level {struct LoadingFieldsV26{std::uint32_t* state130;};std::uint32_t state{10};bool loading_fields_v26(LoadingFieldsV26& out,std::string&){out={&state};return true;}};
struct Receiver {struct Base{std::uintptr_t identity()const{return 0x100;}} base_;unsigned loads{};Base& base(){return base_;}bool load(int,int,std::string&){++loads;return true;}};
struct Record {std::shared_ptr<Receiver> receiver=std::make_shared<Receiver>();};
struct Files {unsigned begins{};bool begin_module(std::uint32_t i,std::string&){check(i==0,"wrong occurrence");++begins;return true;}int load_borrow(){return 0;}};
struct Preparation {struct Status{bool root_complete{true},geometry_prepared{false};} status_;std::shared_ptr<Level> level_=std::make_shared<Level>();CanonicalObjectManagerV1 manager_{{}};std::shared_ptr<Files> files_=std::make_shared<Files>();std::vector<std::shared_ptr<Record>> modules_{std::make_shared<Record>()};const auto& level(){return level_;}auto& manager(){return manager_;}const auto& status(){return status_;}const auto& modules(){return modules_;}const auto& module_files(){return files_;}};
int main(){try{std::uint32_t state=10,progress=0,counter=0,current=999;auto actual_pin=std::make_shared<int>();CanonicalObjectManagerV1 manager({});std::string error;unsigned clears=0;
 CanonicalInitPostServicesV38<CanonicalObjectManagerV1> services;
 services.make_handle=[](const CanonicalObjectBorrowV1* actor,dh2::target_providers::Handle16& handle,std::string&){check(!actor,"class construction not supplied");handle={};return true;};
 services.resolve=[&](auto& handle,bool required,const CanonicalObjectBorrowV1*& out,std::string& e){return manager.resolve_handle_v4(handle,required,out,{},e);};
 services.clear_list=[&](std::uint32_t offset,std::string&){check(offset==0x2c||offset==0x44||offset==0x34,"clear ordering domain");++clears;return true;};
 Stage10BodyV38<CanonicalObjectManagerV1> body({actual_pin,1,{&progress,&state,&counter,&current}},actual_pin,manager,services);
 check(body.step()==LifecycleStepV36::pending&&current==1,"counter did not read actual map1c");
 dh2::target_providers::Handle16 inserted{};inserted.key=123;const CanonicalObjectBorrowV1* missing{};check(manager.resolve_handle_v4(inserted,false,missing,{},error),"actual late null insertion");
 auto result=LifecycleStepV36::pending;for(unsigned i=0;i<10&&result==LifecycleStepV36::pending;++i)result=body.step();check(result==LifecycleStepV36::complete&&current==1&&manager.source_map_size1c_v38()==2&&manager.source_init_phase7c_v38()==5&&clears==3&&state==10,"counter re-read or counterfeit dispatcher advancement");
 auto retained=std::make_shared<Preparation>();retained->manager_.source_init_phase7c_v38()=1;auto loader=retained_stage10_module_load_v38(retained,0);
 check(loader(0x100,error)==LifecycleStepV36::complete&&!retained->status().geometry_prepared&&retained->files_->begins==1&&retained->modules_[0]->receiver->loads==1,"source Module loading delayed until geometry");
 check(loader(0x100,error)==LifecycleStepV36::complete&&retained->files_->begins==1,"Module source replay");
 check(loader(0x222,error)==LifecycleStepV36::failed&&error.find("dynamic module-record")!=std::string::npos,"missing real dynamic module silently skipped");
 std::cout<<"PASS actual_map_counter_once=1 source_module_before_geometry=1 bridge_shape_checks=3 actual_module_effects_verified=0\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
