#pragma once
#include "swf_cursor_input.hpp"
#include "swf_input_history.hpp"
#include "swf_viewport_connection.hpp"
#include <memory>
#include <string>
namespace gameswf {struct character;}
namespace dh2::ui {
struct SwfInputCoreServices {
 std::shared_ptr<void> owner;
 void* context{};
 std::uintptr_t native_receiver{};
 bool(*can_handle_event)(void*,SwfEvent48&,bool& accepted,std::string&){};
 bool(*native_event)(void*,SwfEvent48&,std::string&){};
 // Genuine source root::advance(float,bool) provider required by update().
 // Stock root::advance and its mouse prefix are not an accepted substitute.
 bool(*advance)(void*,gameswf::root*,float,bool,std::string&){};
 // Required only if caller binds a genuine source scene-node identity.
 bool(*scene_local_mouse)(void*,std::uintptr_t,gameswf::character*,float[2],std::string&){};
};
class SwfInputConnection {
public:
 SwfInputConnection()=default;~SwfInputConnection();
 SwfInputConnection(const SwfInputConnection&)=delete;
 SwfInputConnection&operator=(const SwfInputConnection&)=delete;
 // Within exact facade Scope. History was bound before shared/root startup.
 // Source context/flags/selection/native receiver come from their real owner.
 // All four initial cursors are constructor zeros, enabled1, strongslotsnull.
 bool bind(SwfViewportLease,const ViewportState64&,const SwfViewportDriver&,
           std::shared_ptr<SwfInputHistory>,gameswf::character* context,
           std::uint32_t flags,std::uint32_t& selection,const SwfInputCoreServices&,std::string&);
 void release()noexcept;
 bool bound()const noexcept;
 bool focus(gameswf::character*,std::uint32_t,std::string&);
 bool reset_focus(std::uint32_t,std::string&);
 bool input(std::int32_t mask,std::uint32_t,std::string&);
 bool cursor(const SwfCursor16&,std::uint32_t,std::string&);
 bool update(std::int32_t milliseconds,bool source_advance_flag,std::string&);
 bool graphic(gameswf::character*,std::uint32_t,std::string&);
 bool enable(bool,std::uint32_t,std::string&);
 bool set_flags(std::uint32_t,std::string&);
 // Source 3D attachment is an explicit ownership projection; this adapter's
 // observed native 2D constructors initialize the source scene identity0.
 bool scene_binding(gameswf::character*,std::uintptr_t,std::string&);
 bool snapshot(SwfInputState288&,std::string&)const;
 bool raw_cursor(float xy[2],std::int32_t& index,std::string&)const;
 bool viewport_rectangle(const std::int32_t xywh[4],std::string&);
 struct State;
private:std::shared_ptr<State> state_;
};
}
