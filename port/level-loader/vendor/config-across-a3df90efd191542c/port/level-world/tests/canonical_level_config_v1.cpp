#include "canonical_level_config_module_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
int main(){
 auto pin=std::make_shared<int>(1);int debug=0,sets=0;std::string error;
 LevelConfigServicesV1 services{pin,[&](const char* key,bool& value,std::string&){assert(std::string(key)=="isTracingLevel");++debug;value=false;return true;},[&](std::uintptr_t identity,std::string&){assert(identity);++sets;return true;}};
 auto receiver=std::make_shared<CanonicalLevelConfigV1>(pin,services);auto borrow=receiver->canonical(receiver);*borrow.class_name20="LevelConfig";
 assert(*borrow.type_f4==4&&*borrow.room64==-1&&borrow.shared_handle->cached==receiver->identity());
 std::uint8_t across=77;assert(borrow.read_across_rooms87(borrow.context,across,error)&&across==0); // documented modern safety field
 std::array<float,3> zero{};int vector_logs=0;
 CanonicalPropertySourceServicesV1 source{&vector_logs,&zero,[](void*,std::string&){return true;},[](void* c,const char* message,std::string&){assert(std::string(message)=="NEED TO CHECK Venkat Vikram ********** ********** ************");++*static_cast<int*>(c);return true;}};
 CanonicalPropertyMapV1 properties(source);auto fields=receiver->properties();
 assert(properties.init_properties(fields,error)&&properties.load_defaults(fields,error));
 assert(*receiver->string(0x120)=="LevelConfig"&&*receiver->string(0x138)=="level_config"&&receiver->string(0x30)->empty());
 assert(*receiver->integer(0x2a0)==5242880&&*receiver->integer(0x2a4)==655360);
 assert((*receiver->vector(0x1cc))[0]==255&&receiver->dfog_colors().empty());
 assert(properties.set_property(fields,"dfog_colors","12,24,36",error)&&vector_logs==1&&receiver->dfog_colors().empty());
 assert(receiver->init_post(error)&&debug==4&&sets==1);
 assert(*receiver->string(0x24c)=="data/3D/camera/CameraTests.bdae"&&*receiver->string(0x27c)=="PlayerCamera_Default");
 assert((*receiver->vector(0x1cc))[0]==1&&(*receiver->vector(0x1cc))[2]==1);
 assert(receiver->init_post(error)&&debug==4&&sets==1);
 // Regression for the actual ordering: manager reads room/global before name.
 // A real Config precedes a freshly constructed type7 GameObject base. This
 // exercises publication only, not the derived chest visual/InitPost pipeline.
 CanonicalObjectManagerV1 manager({});dh2::target_providers::Handle16 config_handle{},chest_handle{},found{};
 assert(manager.add(borrow,"level_config","LevelConfig",-1,false,config_handle,error));
 dh2::actor::RuntimeState chest_runtime;
 auto chest=std::make_shared<CanonicalGameObjectBaseOwnerV1>(0x12345678,7,pin,chest_runtime);
 auto chest_borrow=chest->canonical(chest);
 assert(manager.add(chest_borrow,"chest_room_zero","OpenableContainer",0,false,chest_handle,error));
 assert(config_handle.key==1&&chest_handle.key==2&&manager.source_count50()==2);
 assert(manager.by_name("chest_room_zero",0,false,nullptr,found,error)&&found.key==2);
 assert(manager.by_name("level_config",-1,false,nullptr,found,error)&&found.key==1);
 // Do not broaden the repair to another receiver's unproduced fields.
 assert(!manager.by_name("chest_room_zero",3,false,nullptr,found,error));
 std::uint8_t missing80=77;assert(!fields.fields.read_bool(fields.fields.context,0x80,missing80,error)&&missing80==77);
 CanonicalLevelConfigV1 missing(pin,{});assert(!missing.init_post(error));assert(*missing.byte(0x29c)==1);assert(missing.init_post(error));
 std::cout<<"PASS LevelConfig original defaults, separate inherited names, vector debug stub, four Debug queries, one-shot prefix\n";
}
