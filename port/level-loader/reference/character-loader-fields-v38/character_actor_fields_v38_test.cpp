#include "retained_character_actor_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2::character;
struct Record {std::shared_ptr<RetainedCharacterActorV1> actor;};
int main(){auto world=std::make_shared<int>(1);auto record=std::make_shared<Record>();record->actor=std::make_shared<RetainedCharacterActorV1>(42,world,"Character",RetainedCharacterConstructionV7::fresh_canonical);
 auto alias=std::shared_ptr<const void>(record,record->actor.get());assert(alias.get()==record->actor.get());assert(reinterpret_cast<std::uintptr_t>(alias.get())!=42);
 CharacterLoaderFieldsV38 fields;std::string error="previous error";assert(record->actor->loader_fields_v38(alias,fields,error)&&error.empty());assert(fields.identity==42&&*fields.properties13c8==-1&&*fields.template13ca==-1&&*fields.master418==0);
 auto before=fields;auto foreign=std::make_shared<int>(7);assert(!record->actor->loader_fields_v38(foreign,fields,error)&&fields.identity==before.identity&&fields.properties13c8==before.properties13c8);assert(!record->actor->loader_fields_v38(record,fields,error));
 auto other=std::make_shared<RetainedCharacterActorV1>(43,world,"Character",RetainedCharacterConstructionV7::fresh_canonical);assert(!record->actor->loader_fields_v38(other,fields,error));
 auto existing=std::make_shared<RetainedCharacterActorV1>(44,world,"Character");assert(!existing->loader_fields_v38(existing,fields,error));
 CharacterModelFieldsV38 model;model.identity=123;model.master418=reinterpret_cast<const std::uintptr_t*>(1);assert(!record->actor->model_name_fields_v38(alias,model,error)&&model.identity==123&&model.master418==reinterpret_cast<const std::uintptr_t*>(1));
 std::weak_ptr<Record> weak=record;auto actor_weak=std::weak_ptr<RetainedCharacterActorV1>(record->actor);alias.reset();before.receiver_lease.reset();record.reset();assert(!weak.expired()&&!actor_weak.expired());*fields.template13ca=11;*fields.properties13c8=360;*fields.master418=99;assert(*fields.template13ca==11&&*fields.properties13c8==360&&*fields.master418==99);fields.receiver_lease.reset();assert(weak.expired()&&actor_weak.expired());
 std::cout<<"PASS actual_actor_cache_master=1 facade_identity_distinct=1 foreign_lease_rejected=1 fresh_adoption_guard=1 record_alias_lifetime=1 missing_properties_preserve_out=1\n";
}
