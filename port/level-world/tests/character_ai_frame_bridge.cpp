#include "../character_ai_frame.hpp"
#include "../character_ai_update.hpp"
#include "../character_script_update.hpp"
namespace dh2::character {
struct AIFrameFixtureBridge48 {
 AIFrameState32* frame;AIUpdateState80* update;AIUpdateServices24* update_services;
 ScriptUpdateState48* script;ScriptUpdateServices16* script_services;AIUpdateResult16* result;
};
static_assert(sizeof(AIFrameFixtureBridge48)==48);
}
extern "C" int dh2_ai_frame_fixture_on_update(void* opaque,dh2::character::AIFrameState32*,
 const dh2::character::AIFrameRequest16*,std::uint32_t* value){
 auto& b=*static_cast<dh2::character::AIFrameFixtureBridge48*>(opaque);*value=0;
 return dh2_character_ai_update(b.result,b.update,b.update_services);
}
extern "C" int dh2_ai_frame_fixture_selected(void* opaque,dh2::character::AIUpdateState80*,
 const dh2::character::AIUpdateRequest32*,std::uint32_t* value){
 auto& b=*static_cast<dh2::character::AIFrameFixtureBridge48*>(opaque);*value=0;
 const int status=dh2_character_script_update(b.script,0,b.script_services);
 b.frame->paused=b.script->paused;return status==1?0:3;
}
