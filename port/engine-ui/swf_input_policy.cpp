#include "swf_input_policy.hpp"
#include <cstring>
extern "C" int dh2_ui_swf_note_assignment(dh2::ui::SwfInputHistoryFlags*f,const char*name){
 if(!f||!name)return -1;
 if(!std::strcmp(name,"onEnterFrame")){f->enter_e9=1;return 2;}
 if(std::strncmp(name,"on",2))return 0;
 constexpr const char*names[]{"onKeyPress","onRelease","onDragOver","onDragOut","onPress","onReleaseOutside","onRollout","onRollover"};
 for(const auto*n:names)if(!std::strcmp(name,n)){f->mouse9c=1;return 1;}return 0;
}
