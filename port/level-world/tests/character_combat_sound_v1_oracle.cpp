#include "../character_combat_sound_v1.hpp"
using namespace dh2::character;
struct Fixture{CombatSoundRowV1 row;const unsigned* in;unsigned* out;};
extern "C" int dh2_test_combat_sound_v1(const unsigned* in,unsigned* out){
 static const int d[]{100,101},h[]{200,201},a[]{300,301},b[]{400,401};
 Fixture f{{{d,in[0]},{h,in[1]},{a,in[2]},{b,in[3]},static_cast<unsigned char>(in[4]),static_cast<unsigned char>(in[5])},in,out};
 out[0]=0;CombatSoundServicesV1 s{&f,
 [](void* p,std::uintptr_t,const CombatSoundRowV1** r){*r=&static_cast<Fixture*>(p)->row;return 0;},
 [](void* p,unsigned* r){*r=static_cast<Fixture*>(p)->in[7];return 0;},
 [](void* p,std::uintptr_t,bool* r){*r=static_cast<Fixture*>(p)->in[6]!=0;return 0;},
 [](void*,std::uintptr_t* r){*r=1;return 0;},
 [](void*,unsigned n,unsigned* r){*r=n-1;return 0;},
 [](void*,std::uintptr_t,std::array<float,3>* r){*r={1,2,3};return 0;},
 [](void* p,const CombatSoundPlayV1* r){auto& x=*static_cast<Fixture*>(p);if(r->source_bool||r->source_integer!=1||r->source_float0!=-1||r->source_float1!=-1||r->position!=std::array<float,3>{1,2,3})return -1;x.out[1+x.out[0]++]=static_cast<unsigned>(r->sound_id);return 0;}};
 dh2::data::CombatResult result{};result.amount=static_cast<int>(in[8]);CombatSoundOutputV1 output{};
 return character_combat_sound_v1(&output,&result,1,2,in[9]!=0,&s);
}
