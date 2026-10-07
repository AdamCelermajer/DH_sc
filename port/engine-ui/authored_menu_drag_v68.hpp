#pragma once
#include "authored_menu_character_projection_v4.hpp"
#include <memory>
#include <functional>
#include "swf_event_dispatch.hpp"
namespace dh2::ui {
// Native DragAndDrop C1 owns the two empty vectors and nullable selected row.
// Characters are the same movie's weak receivers; this owns no player/movie.
class AuthoredMenuDragV68 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 AuthoredMenuDragV68();~AuthoredMenuDragV68();
 using Dimensions=std::function<bool(std::int32_t&,std::int32_t&,std::string&)>;
 bool add_drag(SwfAsGraph&,std::uintptr_t menu_fx,AuthoredMenuCharacterBorrowV3&,
               AuthoredMenuCharacterBorrowV3* limits,AuthoredMenuCharacterProjectionV4&,
               const Dimensions&,std::string&);
 bool add_drop(SwfAsGraph&,std::uintptr_t menu_fx,AuthoredMenuCharacterBorrowV3&,std::string&);
 bool reset_positions(SwfAsGraph&,std::string&);
 bool empty()const noexcept;
 bool event(SwfEvent48&,std::string&);
};
}
