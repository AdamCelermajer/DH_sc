#pragma once
#include <string>
namespace gameswf {struct fn_call;}
namespace dh2::ui {
// Original NativePlaySoundFX (0x43ae10) argument gate. A rejected shape is
// a source no-op; accepted UTF-8 text is then looked up in Arrays::Sounds.
bool swf_menu_sound_argument(const gameswf::fn_call&,std::string& name);
}
