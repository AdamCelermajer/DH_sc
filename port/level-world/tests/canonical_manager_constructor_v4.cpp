#include "canonical_object_manager_v1.hpp"
#include <cassert>
#include <iostream>
int main(){
 dh2::world::CanonicalObjectManagerV1 manager({});std::string error;
 assert(manager.source_next_key4c()==1&&manager.source_count50()==0);
 assert(!manager.object(0)&&manager.characters().empty()&&manager.modules().empty());
 dh2::target_providers::Handle16 a{},b{};
 assert(manager.by_name("FirstActualName",-1,true,nullptr,a,error));
 assert(a.key==1&&manager.source_next_key4c()==2);
 assert(manager.by_name("SecondActualName",-1,true,nullptr,b,error));
 assert(b.key==2&&manager.source_next_key4c()==3);
 assert(manager.source_count50()==0&&!manager.object(1)&&!manager.object(2));
 std::cout<<"PASS source C1->Flush reserved null0, first real name1, ordered next2\n";
}
