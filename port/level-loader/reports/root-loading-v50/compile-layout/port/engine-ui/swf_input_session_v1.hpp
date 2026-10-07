#pragma once
#include "swf_movie.hpp"
#include "swf_input_connection_v2.hpp"
#include <memory>
namespace dh2::ui {
struct SwfInputSessionConfigV1 {
 std::vector<std::string> shared;
 std::string movie;
 // Independent backend owner: MUST NOT own this session or strong graph
 // handles (those would form a cycle). It retains driver/context/glyphs.
 // Required even
 // when the underlying facade has no typed native callback registrations.
 std::shared_ptr<void> provider_owner;
 SwfServices movie_services;
 SwfViewportDriver driver;
 ViewportState64 viewport{};
 FlashCamera40 camera{};
 // Caller supplies original context, receiver policy and selection producer.
 // Empty path selects this exact root movie, not an arbitrary HUD button.
 std::string context_path;
 std::uint32_t flags{},selection{};
 // This 2D owner supplies genuine source advance itself: advance and
 // scene_local_mouse must be null. Native acceptance/event remain required
 // services when reached; null providers produce explicit prefix failures.
 SwfInputCoreServices input_services;
};
// Outer session owns the input's strong graph lease. Impl's startup/native
// provider retains only observers and a weak generation, preventing a cycle.
// Single core thread. This is an application adapter, not an ARM32 class ABI.
class SwfInputSessionV1 {
public:
 SwfInputSessionV1();~SwfInputSessionV1();
 SwfInputSessionV1(const SwfInputSessionV1&)=delete;
 SwfInputSessionV1&operator=(const SwfInputSessionV1&)=delete;
 // Detached candidate: failure preserves current graph. Observers are bound
 // before ANY shared/root construction via the frozen graph_start hook.
 bool load(const SwfInputSessionConfigV1&,std::string&);
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
