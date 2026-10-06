#include "character_fx_floor_sync_v29.hpp"
#include <cassert>
#include <iostream>
static int calls;static int mode;
namespace dh2::fx {
// Deliberate query fixture: branch test only. Production calls the separately
// recovered actual PFWorld selector through character_fx_floor_query_v3.
bool character_fx_floor_query_v3(const navigation::CollisionWorld* w,const float* p,float* n,std::string& e){
 ++calls;assert(p[0]==37.f);if(!w){e="Required World";return false;}
 if(mode==1){n[0]=.25f;n[1]=.5f;n[2]=.75f;}
 if(mode==2)n[0]=n[1]=n[2]=0.f;
 return true;
}
}
int main(){using dh2::fx::character_fx_floor_sync_v29;std::string e;float p[]{37,12,3},n[3],zero[3]{},actual[]{.1f,.2f,.9f};
 dh2::navigation::CollisionWorld world{};
 assert(character_fx_floor_sync_v29(nullptr,p,zero,n,e)&&calls==0&&n[2]==0);
 assert(!character_fx_floor_sync_v29(nullptr,p,actual,n,e)&&calls==1);
 assert(character_fx_floor_sync_v29(&world,p,actual,n,e)&&calls==2&&n[0]==actual[0]&&n[2]==actual[2]);
 assert(character_fx_floor_sync_v29(&world,p,nullptr,n,e)&&calls==3&&n[2]==0);
 mode=1;assert(character_fx_floor_sync_v29(&world,p,actual,n,e)&&calls==4&&n[2]==.75f);
 mode=2;assert(character_fx_floor_sync_v29(&world,p,actual,n,e)&&calls==5&&n[2]==1);
 assert(character_fx_floor_sync_v29(&world,p,nullptr,n,e)&&calls==6&&n[2]==0);
 assert(!character_fx_floor_sync_v29(&world,nullptr,actual,n,e)&&calls==6);
 std::cout<<"PASS 8 exact branch checks; query fixture only\n";
}
