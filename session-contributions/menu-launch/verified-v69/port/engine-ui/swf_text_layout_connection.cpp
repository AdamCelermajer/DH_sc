#include "swf_text_layout_connection.hpp"
#include "text_layout_v1.hpp"
#include "swf_hud_freetype_provider.hpp"
#include "gameswf/gameswf_text.h"
#include "gameswf/gameswf_object.h"
#include "gameswf/gameswf_freetype.h"
#include <cmath>
#include <map>
#include <tuple>
#include <utility>

namespace dh2::ui {
namespace {
using namespace text_v1;
struct NativeFont {gameswf::gc_ptr<gameswf::font> font;};
struct NativeGlyph {gameswf::glyph glyph;};
struct TextOwner final:gameswf::as_object {
    std::vector<std::shared_ptr<NativeFont>> fonts;
    explicit TextOwner(gameswf::player* player):as_object(player){}
};
std::uint32_t color(const gameswf::rgba& c){return c.m_r|(std::uint32_t(c.m_g)<<8)|(std::uint32_t(c.m_b)<<16)|(std::uint32_t(c.m_a)<<24);}
gameswf::rgba color(std::uint32_t c){return gameswf::rgba(c&255,(c>>8)&255,(c>>16)&255,(c>>24)&255);}
}
bool swf_original_text_layout(gameswf::edit_text_character* field,bool html,std::string& error){
    if(!field||!field->m_def){error="Required retained original text field absent";return false;}
    if(!field->m_font){field->m_text_glyph_records.resize(0);error.clear();return true;}
    gameswf::gc_ptr<TextOwner> owner=new TextOwner(field->get_player());
    std::map<std::tuple<std::string,bool,bool>,std::shared_ptr<NativeFont>> resolved;
    auto project=[&](gameswf::font* f){
        auto native=std::make_shared<NativeFont>();native->font=f;owner->fonts.push_back(native);
        auto value=std::make_shared<Font>();value->native=native;value->name=f->get_name().c_str();
        value->bold=f->is_bold();value->italic=f->is_italic();value->define_font3=f->is_define_font3();
        value->metric58=f->get_descent();value->metric5c=f->get_leading();
        resolved.emplace(std::make_tuple(value->name,value->bold,value->italic),native);return value;
    };
    auto real_font=[&](const std::shared_ptr<Font>& f){
        const auto key=std::make_tuple(f->name,f->bold,f->italic);auto found=resolved.find(key);
        if(found!=resolved.end())return found->second;
        auto native=std::make_shared<NativeFont>();native->font=new gameswf::font(field->get_player());
        native->font->set_name(f->name.c_str());native->font->set_bold(f->bold);native->font->set_italic(f->italic);
        owner->fonts.push_back(native);resolved.emplace(key,native);return native;
    };
    State state;state.text=field->m_text.c_str();state.font=project(field->m_font.get_ptr());
    state.rgba=color(field->m_color);state.text_height=field->m_text_height;
    state.rect_min=field->m_def->m_rect.m_x_min;state.rect_max=field->m_def->m_rect.m_x_max;
    state.left=field->m_left_margin;state.right=field->m_right_margin;
    state.indent=field->m_indent;state.leading=field->m_leading;state.alignment=field->m_alignment;
    state.multiline=field->m_def->m_multiline;state.cursor=field->m_cursor;
    Services services;
    auto metrics=[&](const std::shared_ptr<Font>& f,float& units,float& height,float& scale,std::string& e){
        return swf_hud_face_metrics(gameswf::get_glyph_provider(),f->name,f->bold,f->italic,units,height,scale,e);
    };
    services.root_scale_word=[&](float& scale,std::string& e){float units{},height{};return metrics(state.font,units,height,scale,e);};
    services.units_per_em=[&](const std::shared_ptr<Font>& f,float& units,std::string& e){float height{},scale{};return metrics(f,units,height,scale,e);};
    services.font_height=[&](const std::shared_ptr<Font>& f,float& height,std::string& e){float units{},scale{};return metrics(f,units,height,scale,e);};
    services.kerning=[&](const std::shared_ptr<Font>& f,std::int32_t previous,std::int32_t code,float& out,std::string&){out=real_font(f)->font->get_kerning_adjustment(previous,code);return true;};
    services.glyph=[&](const std::shared_ptr<Font>& f,std::uint16_t code,std::int32_t pixels,Glyph& out,bool& found,std::string& e){
        auto native=std::make_shared<NativeGlyph>();auto font=real_font(f);
        found=font->font->get_glyph(&native->glyph,code,pixels);
        out.advance=native->glyph.m_glyph_advance;out.index=static_cast<std::int16_t>(native->glyph.m_glyph_index);
        out.image=native;out.x0=native->glyph.m_bounds.m_x_min;out.x1=native->glyph.m_bounds.m_x_max;
        out.y0=native->glyph.m_bounds.m_y_min;out.y1=native->glyph.m_bounds.m_y_max;
        float units{},height{},scale{};
        return metrics(f,units,height,scale,e);
    };
    services.clone_font=[](const std::shared_ptr<Font>& f,std::shared_ptr<Font>& copy,std::string&){copy=std::make_shared<Font>(*f);return true;};
    services.image=[](const std::string&,std::int32_t,std::int32_t,Image&,std::string& e){e="Original inline text image owner not yet connected";return false;};
    // This Android provider rasterizes/uploads each requested glyph eagerly.
    // It has no deferred preload queue; this is an explicit host policy.
    services.preload_enabled=[](bool& enabled,std::string&){enabled=false;return true;};
    // Original first-ten missing-glyph counter is shared process storage.
    static const auto diagnostics=std::make_shared<DiagnosticState>();
    services.diagnostic_state=diagnostics;
    services.missing_glyph=[](const std::shared_ptr<Font>& f,std::int32_t code,std::string&){gameswf::log_error("Original text missing glyph %d in %s\n",code,f->name.c_str());return true;};
    const bool delivered=format_text(state,html,services,error);
    if(!delivered)return false;
    ::array<gameswf::text_glyph_record> records;
    for(const auto& r:state.records){
        if(r.underline){error="Original underline display owner not yet connected";return false;}
        gameswf::text_glyph_record record;
        record.m_style.m_font_id=r.font_id;record.m_style.m_font=r.font?real_font(r.font)->font.get_ptr():nullptr;
        record.m_style.m_color=color(r.rgba);record.m_style.m_text_height=r.height;
        record.m_style.m_x_offset=r.x;record.m_style.m_y_offset=r.y;
        record.m_style.m_has_x_offset=r.has_x;record.m_style.m_has_y_offset=r.has_y;
        for(const auto& g:r.glyphs){
            if(g.type==2){error="Original inline image display owner not yet connected";return false;}
            if(!g.image){error="Original native glyph lease absent";return false;}
            auto native=std::static_pointer_cast<NativeGlyph>(g.image);auto glyph=native->glyph;
            glyph.m_glyph_advance=g.advance;glyph.m_fontsize=g.height;record.m_glyphs.push_back(glyph);
        }
        records.push_back(record);
    }
    field->m_text_glyph_records=records;
    field->m_text_bounding_box.m_x_min=state.bounds[0];field->m_text_bounding_box.m_x_max=state.bounds[1];
    field->m_text_bounding_box.m_y_min=state.bounds[2];field->m_text_bounding_box.m_y_max=state.bounds[3];
    field->m_x=state.x;field->m_y=state.y;field->m_xcursor=state.xcursor;field->m_ycursor=state.ycursor;
    field->builtin_member("__dh2_text_fonts",gameswf::as_value(owner.get_ptr()));
    gameswf::as_value last_text;
    const bool reported=field->as_object::get_member("__dh2_text_reported",&last_text)&&last_text.to_tu_string()==field->m_text;
    if(!reported){
        if(state.text.find('\n')!=std::string::npos){
            gameswf::log_msg("Original multiline field | multiline %d | rect %.2f %.2f %.2f %.2f | text height %.2f | records %zu\n",
                state.multiline,field->m_def->m_rect.m_x_min,field->m_def->m_rect.m_x_max,
                field->m_def->m_rect.m_y_min,field->m_def->m_rect.m_y_max,state.text_height,state.records.size());
            for(std::size_t i=0;i<state.records.size()&&i<32;++i){const auto& r=state.records[i];
                gameswf::log_msg("Original multiline record | index %zu | xy %.2f %.2f | height %.2f | offsets %d %d | glyphs %zu\n",i,r.x,r.y,r.height,r.has_x,r.has_y,r.glyphs.size());}
        }
        std::string displayed;std::size_t glyph_count=0;std::uint32_t first_color=state.rgba;
        for(const auto& r:state.records){if(!glyph_count&&!r.glyphs.empty())first_color=r.rgba;
            for(const auto& g:r.glyphs){++glyph_count;if(g.code>=32&&g.code<127)displayed.push_back(static_cast<char>(g.code));}}
        gameswf::log_msg("Original text layout connected | html %d | records %d | glyphs %zu | rgba %08x | display %s | source %s\n",html,records.size(),glyph_count,first_color,displayed.c_str(),field->m_text.c_str());
        field->builtin_member("__dh2_text_reported",gameswf::as_value(field->m_text.c_str()));
    }
    error.clear();return true;
}
}
