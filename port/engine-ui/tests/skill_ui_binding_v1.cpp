#include "../skill_ui_binding_v1.hpp"

int main(){
 if(!dh2::ui::skill_ui_binding_v1(0x1234,0x1234,0x1234))return 1;
 if(dh2::ui::skill_ui_binding_v1(0x1234,0x5678,0x1234))return 2;
 if(dh2::ui::skill_ui_binding_v1(0x1234,0x1234,0x5678))return 3;
 if(dh2::ui::skill_ui_binding_v1(0,0,0))return 4;
 return 0;
}
