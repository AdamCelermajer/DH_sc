#pragma once
// Additive V2 based on frozen V1; V1 source/proof remains unchanged.
#include "swf_input_connection.hpp"
#include "swf_input_history.hpp"
#include "swf_viewport_connection.hpp"
#include <memory>
#include <string>
namespace gameswf {struct character;}
namespace dh2::ui {
class SwfInputConnectionV2 {
public:
 SwfInputConnectionV2()=default;~SwfInputConnectionV2();
 SwfInputConnectionV2(const SwfInputConnectionV2&)=delete;
 SwfInputConnectionV2&operator=(const SwfInputConnectionV2&)=delete;
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
 // V2 uses this SAME viewport for input, source camera publication and draw.
 // Within the exact graph Scope; retain focus/drag/cursors across resize.
 bool camera_update(FlashCamera40&,std::string&);
 bool viewport_state(ViewportState64&,std::string&)const;
 bool screen_to_logical(float[2],std::string&);
 bool display_rectangle(float[4],std::string&);
 struct State;
private:std::shared_ptr<State> state_;
};
}
