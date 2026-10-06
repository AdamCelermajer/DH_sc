#include "../canonical_object_factory_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
struct Actor {dh2::target_providers::Handle16 handle{0,UINT32_MAX,0};std::uint32_t type{};std::uint8_t across{};std::int32_t room{-1};std::string name,archetype;};
static bool name(void* p,const char* s,std::string&){static_cast<Actor*>(p)->name=s;return true;}
static bool archetype(void* p,const char* s,std::string&){static_cast<Actor*>(p)->archetype=s;return true;}
static bool character(void* p,std::uintptr_t& out,std::string&){out=reinterpret_cast<std::uintptr_t>(p);return true;}
static CanonicalObjectBorrowV1 borrow(std::shared_ptr<Actor> a){return {reinterpret_cast<std::uintptr_t>(a.get()),a,&a->handle,&a->type,&a->across,&a->room,a.get(),name,archetype,character};}
struct Services {unsigned destroyed{},network{};bool fail_network{};};
static bool destroy(void* p,CanonicalObjectBorrowV1& a,std::string&){++static_cast<Services*>(p)->destroyed;a.lease.reset();return true;}
static bool network(void* p,CanonicalObjectBorrowV1&,std::string& e){auto& s=*static_cast<Services*>(p);++s.network;if(s.fail_network){e="fixture network failure";return false;}return true;}
int main(){
 Services s;CanonicalObjectManagerServicesV1 services;services.context=&s;services.destroy_duplicate=destroy;services.assign_network_id=network;
 CanonicalObjectManagerV1 manager(services);std::string error;dh2::target_providers::Handle16 out{};
 auto first=std::make_shared<Actor>();assert(manager.add(borrow(first),"Skeleton","Character",3,true,out,error));assert(out.key==0&&out.cached==reinterpret_cast<std::uintptr_t>(first.get())&&out.frame==0);assert(first->handle.key==0&&first->room==3);assert(manager.source_count50()==1&&manager.source_next_key4c()==1);assert(manager.characters().size()==1);
 auto duplicate=std::make_shared<Actor>();manager.begin_frame(15);assert(manager.add(borrow(duplicate),"Skeleton","Character",3,true,out,error));assert(out.key==0&&out.frame==15&&out.cached==reinterpret_cast<std::uintptr_t>(first.get()));assert(s.destroyed==1&&s.network==1&&manager.source_count50()==1);assert(first->handle.frame==0); // duplicate resolves local handle only
 assert(manager.get_handle(0,out,error)&&out.frame==15&&first->handle.frame==15);
 auto other=std::make_shared<Actor>();s.fail_network=true;assert(!manager.add(borrow(other),"Ghost","Character",3,true,out,error));assert(manager.source_count50()==2&&manager.source_next_key4c()==2&&manager.object(1));assert(other->handle.key==1&&other->room==3&&manager.characters().size()==2); // network failure preserves Add prefix
 error.clear();assert(manager.by_name("Ghost",3,true,nullptr,out,error)&&out.key==1&&out.cached==0&&out.frame==UINT32_MAX);
 assert(!manager.by_name("LocalPlayer",3,false,nullptr,out,error));assert(error.find("GetLocalPlayer")!=std::string::npos);
 assert(canonical_factories_v1().size()==33&&std::string(canonical_factories_v1()[25].name)=="OpenableContainer"&&canonical_factories_v1()[25].original_address==0x340da4);
 std::cout<<"canonical source map/Add tests PASS; receiver/network fixtures; production factory unavailable\n";
}
