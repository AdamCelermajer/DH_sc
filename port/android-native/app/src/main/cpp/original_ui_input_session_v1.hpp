#pragma once
#include "swf_input_session_v2.hpp"
namespace dh2::android_ui {
struct OriginalUiFrameV1 {
 const std::int32_t* player_properties{};
 std::size_t property_count{};
 std::uintptr_t player_character{};
 std::int32_t milliseconds{};
 bool source_advance_flag{};
 // Optional explicit manager batch, executed inside the exact graph Scope.
 // Absence selects the bounded status/input/frame adapter, not full manager
 // parity. The provider must implement its real required native game calls.
 void* manager_context{};
 bool(*manager_update)(void*,ui::SwfAsGraph&,std::string&){};
};
// Outer Android GL-thread owner. Actual APK/GPU/font/text/settings/native
// providers are supplied by their independent owner, not copied or fabricated.
// The prior OriginalUiSession remains available until caller migration.
class OriginalUiInputSessionV1 {
public:
 OriginalUiInputSessionV1();~OriginalUiInputSessionV1();
 OriginalUiInputSessionV1(const OriginalUiInputSessionV1&)=delete;
 OriginalUiInputSessionV1&operator=(const OriginalUiInputSessionV1&)=delete;
 bool load(const ui::SwfInputSessionConfigV1&,int surface_width,int surface_height,
           std::int32_t renderer_orientation,std::int32_t initial_milliseconds,
           bool initial_source_advance_flag,const char* verified_hud_sha256,std::string&);
 bool resize(int,int,std::int32_t renderer_orientation,std::string&);
 // Already routed native source cursor/mask, no invented Android ACTION map.
 bool cursor(const ui::SwfCursor16&,std::uint32_t,std::string&);
 bool input(std::int32_t,std::uint32_t,std::string&);
 bool reset_focus(std::uint32_t,std::string&);
 bool frame(const OriginalUiFrameV1&,std::string&);
 bool action_script(void*,bool(*)(void*,ui::SwfAsGraph&,std::string&),std::string&);
 bool active()const noexcept;
 void release()noexcept;
 struct State;
private:struct Control;std::shared_ptr<Control>control_;
};
}
