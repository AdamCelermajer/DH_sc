#pragma once
#include "text_layout_v1.hpp"

namespace dh2::ui::edit_text_v1 {
// Owned projections, not ARM layouts. A definition is shared by its genuine
// instances, so border/multiline/word-wrap/type writes affect that definition.
struct Definition {
    std::string default_text, variable;
    std::shared_ptr<text_v1::Font> font;
    float text_height{240}, left{}, right{}, indent{}, leading{};
    std::array<float,4> rectangle{};
    std::uint32_t rgba{0xff000000}, grid_fit{};
    std::int32_t maximum{}, alignment{};
    bool word_wrap{}, multiline{}, password{}, readonly{}, auto_size{},
         no_select{}, border{}, html{}, use_outlines{};
};
struct State {
    State(){layout.cached_rect.fill(0xffffffff);}
    std::shared_ptr<Definition> definition;
    text_v1::State layout;
    std::uint32_t background{0xffffffff};
    bool focus{}, needs_update{};
};
struct BoundRead {
    bool found{}, self{};
    // Retains the exact returned AS value. Original to_string calls conversion
    // twice on a mismatch; an object's conversion can mutate the field.
    std::function<bool(std::string&,std::string&)> to_text;
};
struct Services {
    text_v1::Services layout;
    // These are the complete original parse_path -> parent/find_target ->
    // get/set_member operations. The core adapter must execute them on the
    // current borrowed graph, including setter/watch reentry.
    std::function<bool(const std::string&,BoundRead&,std::string&)> read_bound;
    std::function<bool(const std::string&,const std::string&,std::string&)> write_bound;
    // Real dummy fill-array construction follows bound publication in init.
    std::function<bool(std::string&)> initialize_fill;
};
// Source failures retain the effects already performed, including truncation
// before a reached malformed UTF-8/layout/provider failure. Services can mutate
// the field/definition synchronously; subsequent operations reread them.
bool format(State&,bool html,const Services&,std::string&);
bool set_text(State&,const std::string&,bool html,const Services&,std::string&);
bool set_text_value(State&,const std::string&,bool html,const Services&,std::string&);
bool refresh_bound(State&,const Services&,std::string&);
bool initialize(State&,const Services&,std::string&);
}
