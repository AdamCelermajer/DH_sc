#include "canonical_existing_actor_publication_v3.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::world;
namespace target_providers=dh2::target_providers;
struct Receiver {
 std::string name="CryptSkeleton",archetype="Character";
 target_providers::Handle16 handle{};std::uint32_t type=0;std::int32_t room=-1;
 static bool name_write(void* p,const char* s,std::string&){return static_cast<Receiver*>(p)->name==s;}
 static bool archetype_write(void* p,const char* s,std::string&){return static_cast<Receiver*>(p)->archetype==s;}
 static bool character(void* p,std::uintptr_t& out,std::string&){out=reinterpret_cast<std::uintptr_t>(p);return true;}
 CanonicalExistingActorV3 borrow(const std::shared_ptr<Receiver>& lease){CanonicalObjectBorrowV1 b;
  b.identity=reinterpret_cast<std::uintptr_t>(this);b.lease=lease;b.shared_handle=&handle;b.type_f4=&type;b.room64=&room;b.context=this;
  b.set_name=name_write;b.set_archetype=archetype_write;b.as_character=character;
  return {b,&name,&archetype};}
};
int main(){
 CanonicalObjectManagerV1 manager({});CanonicalExistingActorPublicationV3 publication(manager);
 auto actor=std::make_shared<Receiver>();std::string error;target_providers::Handle16 out{};
 assert(!publication.publish(actor->borrow(actor),"different","Character",-1,false,out,error));
 assert(manager.source_count50()==0&&!publication.attempted(reinterpret_cast<std::uintptr_t>(actor.get())));
 assert(publication.publish(actor->borrow(actor),actor->name.c_str(),actor->archetype.c_str(),-1,false,out,error));
 assert(manager.source_count50()==1&&manager.characters().size()==1&&actor->handle.cached==reinterpret_cast<std::uintptr_t>(actor.get()));
 assert(!publication.publish(actor->borrow(actor),actor->name.c_str(),actor->archetype.c_str(),-1,false,out,error));
 auto second=std::make_shared<Receiver>();second->name="CryptGhost";
 assert(!publication.publish(second->borrow(second),second->name.c_str(),second->archetype.c_str(),-1,true,out,error));
 assert(manager.source_count50()==2&&manager.characters().size()==2); // genuine Add prefix before missing network provider
 assert(!publication.publish(second->borrow(second),second->name.c_str(),second->archetype.c_str(),-1,true,out,error));
 assert(manager.source_count50()==2);
 std::cout<<"canonical existing actor adoption fixture PASS: preflight, same Handle, Add prefix, no replay\n";
}
