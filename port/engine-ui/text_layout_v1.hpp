#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh2::ui::text_v1 {
// Native, owning projection of the original dynamic text records. Font and
// image identities stay alive across layout, callbacks and retained records.
// This is not the stock GameSWF HTML formatter or a serialized ARM object.
struct Font {
    std::shared_ptr<void> native;
    std::string name;
    bool bold{}, italic{}, define_font3{};
    float metric58{}, metric5c{}; // original font descent / leading
};
struct Attributes {
    std::shared_ptr<Font> font;
    std::int32_t size{12};
    std::uint32_t rgba{0xff000000};
    bool underline{};
};
struct Glyph {
    float advance{512}, x0{}, x1{}, y0{}, y1{};
    std::shared_ptr<void> image;
    std::int16_t height{}, index{-1};
    std::uint16_t code{};
    std::uint8_t type{};
};
struct Record {
    std::int32_t font_id{-1};
    std::shared_ptr<Font> font;
    std::uint32_t rgba{0xffffffff};
    bool underline{};
    float x{}, y{}, height{1};
    bool has_x{}, has_y{}, has_font{true};
    std::vector<Glyph> glyphs;
};
struct State {
    std::string text;
    std::shared_ptr<Font> font;
    std::uint32_t rgba{0xff000000};
    float text_height{240}, rect_min{}, rect_max{4000};
    float left{}, right{}, indent{}, leading{}, letter_spacing{};
    std::int32_t alignment{}, cursor{}, first_line{}, word_record{}, word_glyph{-1};
    bool multiline{true};
    float x{}, y{}, xcursor{}, ycursor{};
    std::array<float,4> bounds{}; // xmin,xmax,ymin,ymax
    std::array<std::uint32_t,4> cached_rect{};
    std::vector<Record> records;
};
struct Image {
    std::shared_ptr<void> native;
    std::int32_t width{},height{};
};
struct DiagnosticState { std::uint32_t missing_glyphs{}; };
struct Services {
    // All callbacks deliver real producers. A missing glyph/image is a valid
    // delivered source miss; a missing service is a required-provider error.
    std::function<bool(float&,std::string&)> root_scale_word;
    std::function<bool(const std::shared_ptr<Font>&,float&,std::string&)> units_per_em;
    std::function<bool(const std::shared_ptr<Font>&,float&,std::string&)> font_height;
    std::function<bool(const std::shared_ptr<Font>&,std::int32_t,std::int32_t,float&,std::string&)> kerning;
    std::function<bool(const std::shared_ptr<Font>&,std::uint16_t,std::int32_t,Glyph&,bool&,std::string&)> glyph;
    std::function<bool(const std::shared_ptr<Font>&,std::shared_ptr<Font>&,std::string&)> clone_font;
    std::function<bool(const std::string&,std::int32_t,std::int32_t,Image&,std::string&)> image;
    std::function<bool(bool&,std::string&)> preload_enabled;
    std::function<bool(State&,std::string&)> preload;
    // Original process-wide first-ten diagnostic counter. The graph/platform
    // owner shares this across its fields; the layout never silently invents
    // a replacement counter/logger when a genuine missing glyph is delivered.
    std::shared_ptr<DiagnosticState> diagnostic_state;
    std::function<bool(const std::shared_ptr<Font>&,std::int32_t,std::string&)> missing_glyph;
};
struct Tag { std::vector<std::pair<std::string,std::string>> entries; };
// Partial hash writes on malformed quoted attributes are retained, as in
// parse_tag. Unsafe source-domain inputs are explicit errors.
bool parse_tag(Tag&,const std::string&,bool& parsed,std::string&);
bool align_line(State&,std::int32_t,std::int32_t,float,std::string&);
bool append_text(State&,const std::string&,Attributes&,bool,const Services&,std::string&);
bool append_image(State&,const std::string&,std::int32_t,std::int32_t,const Services&,std::string&);
bool parse_html(State&,const Services&,std::string&);
bool format_text(State&,bool,const Services&,std::string&);
}
