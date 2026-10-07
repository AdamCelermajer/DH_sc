#include "canonical_dummy_owner_v14.hpp"
#include "canonical_point3d_globals_v1.hpp"
#include <cstdio>
#include <cstdlib>
using namespace dh2::world;
namespace {
unsigned checks{},rows{};void check(bool x){++checks;if(!x){std::fprintf(stderr,"FAIL %u\n",checks);std::exit(1);}}
bool assertion(void*,std::string&){return true;} // explicit source assertion policy fixture
void run(std::map<std::string,std::string>& attrs){
 ++rows;std::string error;auto pin=std::make_shared<int>(1);dh2::actor::RuntimeState runtime{};
 auto owner=std::make_shared<CanonicalDummyOwnerV14>(pin,runtime,GameObjectInitializationServicesV1{},DummyContinuationServicesV14{});
 check(owner->base().identity()==reinterpret_cast<std::uintptr_t>(owner.get()));check(owner->base().lifecycle().static84==1);
 owner->base().class_name20()="Dummy";auto actor=owner->properties();CanonicalPropertyMapV1 map({nullptr,&canonical_vec3_origin_v1(),assertion});
 check(map.init_properties(actor,error));check(map.load_defaults(actor,error));check(owner->base().lifecycle().static84==1);
 CanonicalSourceObjectRequestV1 request;request.source_lease=pin;request.source_context=&attrs;
 request.attribute=[](void* c,std::uint32_t,const char* n)->const char*{auto& a=*static_cast<std::map<std::string,std::string>*>(c);auto i=a.find(n);return i==a.end()?nullptr:i->second.c_str();};
 check(map.load_overrides(actor,request,error));check(owner->base().byte(0x87)!=nullptr);
 check(!owner->is_updatable()&&!owner->is_zonable()&&!owner->is_animated()&&!owner->is_interactive());
 auto receiver=CanonicalDummyOwnerV14::factory_receiver(owner,pin);bool game{};check(receiver.is_game_object(game,error)&&game);
 std::array<float,3> position{};check(receiver.position(position,error));check(position[0]==runtime.subobjects.position[0]);
 bool eligible{};check(!owner->init_post(eligible,error));check(!error.empty());
 check(!owner->destroy(error));check(error.find("destruction")!=std::string::npos);
}
}
int main(){
#include "canonical_dummy_declarations_v14.inc"
 check(rows==41);std::printf("Dummy actual parsed SWAMP declarations PASS %u rows %u checks; real constructor/default/typed fields; required initialization providers remain explicit\n",rows,checks);
}
