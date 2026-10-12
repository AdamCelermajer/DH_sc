#pragma once
#include "edit_text_field_v1.hpp"
#include "swf_text_font_platform_v1.hpp"
namespace gameswf {struct edit_text_character;struct fn_call;struct as_textformat;}
namespace dh2::ui {
// Per native edit_text_character sidecar, owned by the versioned character.
// It never retains its player/root graph. The field/font/image projections
// pin only their actual source resources through synchronous callbacks.
struct SwfEditTextFieldV1 {
    struct Impl;std::unique_ptr<Impl> impl;
    explicit SwfEditTextFieldV1(gameswf::edit_text_character&);
    ~SwfEditTextFieldV1();
    void initialize();
    void set_text(const std::string&,bool html,bool write_variable);
    void format(bool html);
    void refresh();
    void get_format(const gameswf::fn_call&);
    void display_records();
    void display();
    void cursor();
    bool event(std::uint8_t id,std::uint8_t key);
    void display_records(const text_display_v2::Context&,bool supplied_matrix);
};
}
