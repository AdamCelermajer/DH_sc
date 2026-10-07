#include "retained_character_actor_v1.hpp"
#include "canonical_existing_actor_publication_v3.hpp"
#include <cassert>
#include <iostream>
int main(){
 using namespace dh2;using namespace dh2::world;using namespace dh2::character;
 std::string error;auto fixture_world=std::make_shared<int>(1);
 auto actor=std::make_shared<RetainedCharacterActorV1>(0x100000006ull,fixture_world,"Character");
 auto initial=actor->canonical(actor);assert(initial.set_name(initial.context,"ActualActor",error));
 actor->object=std::make_shared<ScriptCharacterObject>(initial.identity,"ActualActor",
  std::make_shared<data::PropertyState>(),std::make_shared<data::CombatActorState>(),std::array<float,3>{});
 CanonicalObjectBorrowV1 receiver;assert(actor->canonical_adoption_v2(actor,"ActualActor","",-1,receiver,error));
 CanonicalObjectManagerV1 manager({});CanonicalExistingActorPublicationV3 publication(manager);
 target_providers::Handle16 published{};assert(publication.publish({receiver,&actor->source_name(),&actor->source_archetype()},"ActualActor","",-1,false,published,error));
 const CanonicalObjectBorrowV1* out{};auto handle=published;manager.begin_frame(17);
 assert(manager.resolve_handle_v4(handle,true,out,{},error));
 assert(out&&out->identity==actor->object->identity&&handle.frame==17);
 handle.cached=0;assert(manager.resolve_handle_v4(handle,false,out,{},error)&&out);
 // Original NULL key does not read/write stale cached/frame fields.
 target_providers::Handle16 null{0,123,actor->object->identity};
 assert(manager.resolve_handle_v4(null,false,out,{},error)&&!out&&null.frame==123&&null.cached==actor->object->identity);
 assert(!manager.resolve_handle_v4(null,true,out,{},error));
 unsigned assertions{};auto actual_assert=[&](std::string&){++assertions;return true;};
 assert(manager.resolve_handle_v4(null,true,out,actual_assert,error)&&!out&&assertions==1);
 auto missing=target_providers::Handle16{88,0,0};const auto next=manager.source_next_key4c(),count=manager.source_count50();
 assert(manager.resolve_handle_v4(missing,false,out,{},error)&&!out&&missing.frame==17&&missing.cached==0);
 assert(manager.source_next_key4c()==next&&manager.source_count50()==count);
 // Assert failure preserves map/cache prefix reached before that callback.
 missing.frame=0;assert(!manager.resolve_handle_v4(missing,true,out,[](std::string& e){e="Actual assertion delivery failure";return false;},error));
 assert(missing.frame==17&&!missing.cached&&error=="Actual assertion delivery failure");
 auto cached=published;cached.key=999;cached.frame=17;
 assert(manager.resolve_handle_v4(cached,false,out,{},error)&&out->identity==actor->object->identity);
 cached.cached=0x1234;assert(!manager.resolve_handle_v4(cached,false,out,{},error)&&cached.cached==0x1234);
 std::cout<<"PASS source Handle cache/epoch/null/assert/map insertion prefixes over same retained actor; World/frame/assertion endpoints explicit fixture\n";
}
