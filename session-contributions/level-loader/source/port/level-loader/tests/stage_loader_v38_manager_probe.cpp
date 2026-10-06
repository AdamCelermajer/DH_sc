#include "canonical_object_manager_v1.hpp"
#include <stdexcept>
#include <iostream>
using namespace dh2::world;
static void check(bool ok,const char* reason){if(!ok)throw std::runtime_error(reason);}
int main(){try{CanonicalObjectManagerV1 manager({});std::string error;
 check(manager.source_init_phase7c_v38()==0&&manager.source_map_size1c_v38()==1&&manager.source_count50()==0,"genuine constructor phase/map/count");
 dh2::target_providers::Handle16 handle{};check(manager.by_name("unpublished",-1,true,nullptr,handle,error),"name allocation");
 check(manager.source_map_size1c_v38()==2&&manager.source_count50()==0,"map1c replaced by published50");
 const CanonicalObjectBorrowV1* actor{};dh2::target_providers::Handle16 negative{};negative.key=-7;
 check(manager.resolve_handle_v4(negative,false,actor,{},error)&&!actor,"negative null source operator[]");
 std::int32_t key{};check(manager.source_ordered_begin_v38(key,actor)&&key==-7&&!actor,"actual signed map begin");
 check(manager.source_ordered_next_v38(key,key,actor)&&key==0&&!actor,"reserved null entry omitted");
 check(manager.source_ordered_next_v38(key,key,actor)&&key==handle.key&&!actor,"unpublished allocated entry omitted");
 check(!manager.source_ordered_next_v38(key,key,actor)&&manager.source_map_size1c_v38()==3,"map tail/size");
 manager.source_init_phase7c_v38()=3;check(manager.source_init_phase7c_v38()==3,"shadow InitPost phase");
 std::cout<<"PASS actual_manager_projection_checks=8 source_map1c=3 published_count50=0 actual_init_phase7c=3\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
