#pragma once
#include "swf_input_session_v1.hpp"
#include <array>
#include "swf_input_connection_v2.hpp"
#include <memory>
namespace dh2::ui {
class SwfInputSessionStatusV2 {
public:
 SwfInputSessionStatusV2()=default;~SwfInputSessionStatusV2();
 SwfInputSessionStatusV2(const SwfInputSessionStatusV2&)=delete;
 SwfInputSessionStatusV2&operator=(const SwfInputSessionStatusV2&)=delete;
 bool current()const noexcept;
 bool update(const std::int32_t*,std::size_t,std::uintptr_t,std::string&);
 bool frames(std::array<std::int32_t,5>&,std::size_t&dirty,std::string&);
 void release()noexcept;
 struct State;
private:std::shared_ptr<State>state_;friend class SwfInputSessionV2;
};
// Outer session owns the input's strong graph lease. Impl's startup/native
// provider retains only observers and a weak generation, preventing a cycle.
// Single core thread. This is an application adapter, not an ARM32 class ABI.
class SwfInputSessionV2 {
public:
 SwfInputSessionV2();~SwfInputSessionV2();
 SwfInputSessionV2(const SwfInputSessionV2&)=delete;
 SwfInputSessionV2&operator=(const SwfInputSessionV2&)=delete;
 // Detached candidate: failure preserves current graph. Observers are bound
 // before ANY shared/root construction via the frozen graph_start hook.
 bool load(const SwfInputSessionConfigV1&,std::string&);
 // Pin the exact generation/SwfMovie for the existing source status owner;
 // no raw reloadable Movie pointer is exposed. Call outside any core Scope.
 bool bind_status_hud(const char*verified_hud_sha256,SwfInputSessionStatusV2&,std::string&);
 void release()noexcept;
 bool bound()const noexcept;
 // Android must supply its real pointer-slot/button/mask policy. No ACTION
 // synthesis, 3D projection, frame flag or game touch consumption is inferred.
 bool cursor(const SwfCursor16&,std::uint32_t,std::string&);
 bool input(std::int32_t mask,std::uint32_t,std::string&);
 bool reset_focus(std::uint32_t,std::string&);
 bool focus(const char* actual_context_path,std::uint32_t,std::string&);
 bool enable(bool,std::uint32_t,std::string&);
 bool set_flags(std::uint32_t,std::string&);
 // Exact RenderFX.Update: signed milliseconds -> seconds, source advance,
 // then all-four pending-click tail. Caller supplies advanceFlag explicitly.
 bool update(std::int32_t milliseconds,bool advance_flag,std::string&);
 // Real driver queries and source FlashCamera arithmetic/publication. Caller
 // keeps authored camera state; input focus/drag and slot history survive.
 bool camera_update(FlashCamera40&,std::string&);
 // Read back constructor camera prefix before using it as a later caller
 // input; initial load performs the original update on an owned seed copy.
 bool camera_state(FlashCamera40&,std::string&)const;
 bool screen_to_logical(float point[2],std::string&);
 bool viewport_state(ViewportState64&,std::string&)const;
 bool snapshot(SwfInputState288&,std::uint32_t&selection,std::string&)const;
 bool display(const char* actual_clip_path,std::string&);
 // One exact facade Scope for connected manager/native graph operations.
 // During a native callback, supported input operations reenter this SAME
 // generation; a second movie/Scope/reload remains rejected by the facade.
 bool action_script(void*,bool(*)(void*,SwfAsGraph&,std::string&),std::string&);
 struct Generation;struct Provider;struct Control;
private:std::shared_ptr<Control> control_;
};
}
