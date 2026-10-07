// Flat registry and callback fixtures only; production owns its real StateInfo.
#include "../character_state_owner_frame.hpp"
using namespace dh2::character;
namespace {
#include "../character_state_owner_data.inc"
struct Fixture {
 StateOwnerInfo40 infos[20];NativeFsm24 native;StateOwnerMachine40 machine;
 std::uint32_t dt;
 static int outer(void* p,NativeFsm24*,const NativeFsmRequest32* r,std::uint32_t* out){if(r->service==fsm_engine_dt)*out=static_cast<Fixture*>(p)->dt;return 0;}
 static int unavailable(void*,StateOwnerMachine40*,const StateOwnerUpdateRequest24*){return 1;}
 Fixture(State* s,std::uint32_t delta):native{s,0xa123456789abcdefull,std::uint32_t(s->current!=-1),0},machine{&native,infos,20,s->current,0,0},dt(delta){for(unsigned i=0;i<20;++i)infos[i]=source_infos[i];}
};
std::uint32_t updates=0;
}
extern "C" int dh2_state_owner_frame_update_fixture(State* s,const Facts* f,std::uint32_t dt,const Services* bodies){
 if(!s||!f||!bodies)return -1;
 Fixture fixture(s,dt);StateOwnerFrameContext56 context{&fixture.machine,f,bodies,{&fixture,Fixture::outer},{&fixture,Fixture::unavailable}};
 ++updates;return dh2_character_state_owner_frame(&context);
}
extern "C" std::uint32_t dh2_state_owner_frame_update_count(){return updates;}
