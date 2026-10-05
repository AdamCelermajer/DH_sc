#include "player_savegame_v1.hpp"
#include <cstdio>
#include <cstring>
int main(){dh2::data::PlayerSavegameV1 save;std::string error;unsigned cases=0;
 for(unsigned difficulty=0;difficulty<3;++difficulty)for(unsigned value:{0u,1u,4u,5u,0x7fffffffu,0x80000000u,0xffffffffu}){
  for(unsigned i=0;i<3;++i)if(!save.set_current_faery(17+i,i,error))return 1;
  if(!save.set_current_faery(value,difficulty,error))return 2;
  for(unsigned i=0;i<3;++i){unsigned expected=i==difficulty?value:17+i;int word;std::memcpy(&word,&expected,4);if(save.current_faery(i)!=word)return 3;}
  for(auto initialized:save.faeries_initialized())if(initialized)return 4;++cases;
 }
 auto before=save.current_faeries();if(save.set_current_faery(0,3,error)||before!=save.current_faeries())return 5;
 std::printf("PASS native same Save setter%u original ARM cases; no initialization/unlock, malformed tier guard\n",cases);return 0;
}
