#include "../retained_character_actor_v1.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include "../canonical_gameobject_base_owner_v1.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>
using namespace dh2::character;
int main(){
 const auto& catalog=dh2::world::canonical_factories_v1();
 const auto character=std::find_if(catalog.begin(),catalog.end(),[](const auto& entry){return std::string(entry.name)=="Character";});
 assert(character!=catalog.end()&&character->original_address==0x340800);
 assert(std::string(catalog.front().name)=="Decor");
 auto pin=std::make_shared<int>(7);std::weak_ptr<int> weak=pin;
 auto actor=std::make_shared<RetainedCharacterActorV1>(0x100000123ull,pin,"Character");
 pin.reset();assert(!weak.expired());
 auto borrow=actor->canonical(actor);assert(borrow.identity==0x100000123ull);
 assert(borrow.shared_handle==&actor->shared_handle());
 assert(borrow.shared_handle->key==0&&borrow.shared_handle->cached==borrow.identity&&borrow.shared_handle->frame==UINT32_MAX);
 assert(*borrow.type_f4==0&&*borrow.room64==-1&&borrow.across_rooms87==nullptr);
 std::uint8_t across{};std::string unavailable;
 assert(!borrow.read_across_rooms87(borrow.context,across,unavailable));
 std::string error;assert(borrow.set_name(borrow.context,"CryptMonster",error));
 assert(borrow.set_archetype(borrow.context,"Goblin",error));
 assert(actor->source_name()=="CryptMonster"&&actor->source_archetype()=="Goblin");
 std::uintptr_t converted{};assert(borrow.as_character(borrow.context,converted,error)&&converted==borrow.identity);
 actor->publish_across_rooms(0);borrow=actor->canonical(actor);
 assert(borrow.read_across_rooms87(borrow.context,across,unavailable)&&across==0);
 assert(borrow.across_rooms87&&*borrow.across_rooms87==0);
 *borrow.shared_handle={19,23,borrow.identity};assert(actor->shared_handle().key==19);
 assert(!actor->construct_graph({}, {}, {}));assert(!actor->error().empty());
 assert(!actor->object&&!actor->machine&&!actor->session);
 auto properties=actor->properties();std::uint8_t flag=99;
 assert(properties.fields.read_bool(properties.fields.context,0x84,flag,error)&&flag==0);
 assert(properties.fields.read_bool(properties.fields.context,0x83,flag,error)&&flag==0);
 assert(properties.fields.write_bool(properties.fields.context,0x83,1,error));
 assert(properties.fields.read_bool(properties.fields.context,0x83,flag,error)&&flag==1);
 assert(!properties.fields.read_bool(properties.fields.context,0x80,flag,error));
 dh2::world::CanonicalPropertySourceServicesV1 source{};
 source.position_rotation_default=&dh2::world::canonical_vec3_origin_v1();
 dh2::world::CanonicalPropertyMapV1 map(source);
 assert(map.init_properties(properties,error));assert(map.load_defaults(properties,error));
 assert(properties.fields.read_bool(properties.fields.context,0x80,flag,error)&&flag==1);
 assert(properties.fields.read_bool(properties.fields.context,0x1430,flag,error)&&flag==1);
 std::array<float,3> position{},scale{};
 assert((actor->source_position(position,error)&&position==std::array<float,3>{}));
 assert((actor->source_scale(scale,error)&&scale==std::array<float,3>({1,1,1})));
 assert(map.set_property(properties,"position","12,34,56",error));
 assert((actor->source_position(position,error)&&position==std::array<float,3>({12,34,56})));
 assert(map.set_property(properties,"name","ActualXmlName",error));
 assert(actor->source_name()=="ActualXmlName");
 assert(!properties.fields.write_int(properties.fields.context,0x1434,7,error));
 // The batching borrower reads this same source byte from both generic and
 // Character receivers. Its property default is false and authored overrides
 // must remain live in the backing source cell.
 {
 dh2::actor::RuntimeState generic_runtime{};
 dh2::world::CanonicalGameObjectBaseOwnerV1 generic(0x200000123ull,20,weak.lock(),generic_runtime);
 assert(generic.byte(0x83)&&*generic.byte(0x83)==0);
 auto generic_properties=generic.properties();
 assert(generic_properties.fields.write_bool(generic_properties.fields.context,0x83,1,error));
 assert(generic.byte(0x83)&&*generic.byte(0x83)==1);
 auto fresh=std::make_shared<RetainedCharacterActorV1>(0x300000123ull,weak.lock(),"Character",RetainedCharacterConstructionV7::fresh_canonical);
 auto fresh_properties=fresh->properties();
 assert(map.init_properties(fresh_properties,error)&&map.load_defaults(fresh_properties,error));
 assert(fresh->source_bool_field(0x83)&&*fresh->source_bool_field(0x83)==0);
 assert(fresh_properties.fields.write_bool(fresh_properties.fields.context,0x83,1,error));
 assert(fresh->source_bool_field(0x83)&&*fresh->source_bool_field(0x83)==1);
 }
 actor->close();actor->close();assert(!weak.expired());
 borrow.lease.reset();actor.reset();assert(weak.expired());
 std::cout<<"retained Character constructor/borrow/lifetime checks passed\n";
}
