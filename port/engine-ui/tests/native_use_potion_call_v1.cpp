#include "../native_use_potion_call_v1.hpp"
#include <cstdint>

int main(){
 std::int32_t player=-1;
 if(!dh2::ui::native_use_potion_player_v1(0,17,player)||player!=0)return 1;
 if(!dh2::ui::native_use_potion_player_v1(1,2,player)||player!=2)return 2;
 if(dh2::ui::native_use_potion_player_v1(2,17,player)||player!=2)return 3;
 return 0;
}
