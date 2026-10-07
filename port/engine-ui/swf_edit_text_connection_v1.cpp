#include "swf_edit_text_connection_v1.hpp"
#include "edit_text_format_v1.hpp"
#include "edit_text_event_v1.hpp"
#include "gameswf/gameswf_text.h"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_environment.h"
#include <map>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <cmath>

namespace dh2::ui {
namespace {
std::uint32_t packed(const gameswf::rgba& c){return c.m_r|(unsigned(c.m_g)<<8)|(unsigned(c.m_b)<<16)|(unsigned(c.m_a)<<24);}
gameswf::rgba color(unsigned c){return {Uint8(c),Uint8(c>>8),Uint8(c>>16),Uint8(c>>24)};}
text_display_v2::Matrix matrix(const gameswf::matrix& m){text_display_v2::Matrix o{};for(unsigned i=0;i<6;++i)o[i]=m.m_[i/3][i%3];return o;}
gameswf::matrix core_matrix(const text_display_v2::Matrix& m){gameswf::matrix o;for(unsigned i=0;i<6;++i)o.m_[i/3][i%3]=m[i];return o;}
void checked(bool b,const std::string& e){if(!b)throw std::runtime_error(e.empty()?"Required source edit-text service unavailable":e);}
struct InlineImage {gameswf::gc_ptr<gameswf::bitmap_info> bitmap;};
std::shared_ptr<text_v1::DiagnosticState> process_diagnostics(){static auto p=std::make_shared<text_v1::DiagnosticState>();return p;}
}
struct SwfEditTextFieldV1::Impl {
    gameswf::edit_text_character& field;
    std::shared_ptr<SwfTextFontPlatformV1> platform;
    edit_text_v1::State state;
    std::map<std::weak_ptr<void>,std::shared_ptr<InlineImage>,std::owner_less<std::weak_ptr<void>>> inline_images;
    Impl(gameswf::edit_text_character& f):field(f),platform(SwfTextFontPlatformV1::for_player(f.get_player())){
        if(!platform)throw std::runtime_error("edit text requires its retained source font platform before construction");
        state.definition=std::make_shared<edit_text_v1::Definition>();definition();
    }
    void definition(){
        if(!field.m_def)throw std::runtime_error("edit text requires actual owned definition");
        auto& d=*state.definition;auto& s=*field.m_def;
        d.default_text=s.m_default_text.c_str();d.variable=s.m_var_name.c_str();d.text_height=s.m_text_height;
        d.font=s.m_font?project(s.m_font):nullptr;d.rgba=packed(s.m_color);
        d.rectangle={s.m_rect.m_x_min,s.m_rect.m_x_max,s.m_rect.m_y_min,s.m_rect.m_y_max};
        d.left=s.m_left_margin;d.right=s.m_right_margin;d.indent=s.m_indent;d.leading=s.m_leading;
        d.maximum=s.m_max_length;d.alignment=s.m_alignment;d.grid_fit=s.m_grid_fit;
        d.word_wrap=s.m_word_wrap;d.multiline=s.m_multiline;d.password=s.m_password;d.readonly=s.m_readonly;
        d.auto_size=s.m_auto_size;d.no_select=s.m_no_select;d.border=s.m_border;d.html=s.m_html;d.use_outlines=s.m_use_outlines;
    }
    std::shared_ptr<text_v1::Font> project(gameswf::font* f){std::string e;auto p=platform->font(f,e);checked(bool(p),e);return p;}
    void import_simple(){definition();auto& q=state.layout;
        q.text=std::string(field.m_text.c_str(),field.m_text.size());q.font=field.m_font?project(field.m_font.get_ptr()):nullptr;
        q.rgba=packed(field.m_color);q.text_height=field.m_text_height;q.alignment=field.m_alignment;
        q.left=field.m_left_margin;q.right=field.m_right_margin;q.indent=field.m_indent;q.leading=field.m_leading;
        q.letter_spacing=field.m_letter_spacing;q.cursor=field.m_cursor;q.x=field.m_x;q.y=field.m_y;
        q.xcursor=field.m_xcursor;q.ycursor=field.m_ycursor;state.focus=field.m_has_focus;state.background=packed(field.m_background_color);
    }
    void export_simple(){auto& q=state.layout;std::string e;
        field.m_text=tu_string(q.text.data(),q.text.size());field.m_color=color(q.rgba);field.m_text_height=q.text_height;
        field.m_font=q.font?platform->core_font(q.font,field.get_player(),e):nullptr;if(q.font)checked(field.m_font!=nullptr,e);
        field.m_alignment=gameswf::edit_text_character_def::alignment(q.alignment);
        field.m_left_margin=q.left;field.m_right_margin=q.right;field.m_indent=q.indent;field.m_leading=q.leading;
        field.m_letter_spacing=q.letter_spacing;field.m_cursor=q.cursor;field.m_x=q.x;field.m_y=q.y;
        field.m_xcursor=q.xcursor;field.m_ycursor=q.ycursor;field.m_has_focus=state.focus;field.m_background_color=color(state.background);
        field.m_text_bounding_box.m_x_min=q.bounds[0];field.m_text_bounding_box.m_x_max=q.bounds[1];
        field.m_text_bounding_box.m_y_min=q.bounds[2];field.m_text_bounding_box.m_y_max=q.bounds[3];
    }
    void export_records(){export_simple();std::string e;
        field.m_text_glyph_records.resize(state.layout.records.size());
        for(std::size_t i=0;i<state.layout.records.size();++i){auto& a=state.layout.records[i];auto& b=field.m_text_glyph_records[i];
            b.m_style.m_font=a.font?platform->core_font(a.font,field.get_player(),e):nullptr;if(a.font)checked(b.m_style.m_font!=nullptr,e);
            b.m_style.m_font_id=a.font_id;b.m_style.m_color=color(a.rgba);b.m_style.m_text_height=a.height;
            b.m_style.m_x_offset=a.x;b.m_style.m_y_offset=a.y;b.m_style.m_has_x_offset=a.has_x;b.m_style.m_has_y_offset=a.has_y;
            b.m_glyphs.resize(a.glyphs.size());
            for(std::size_t j=0;j<a.glyphs.size();++j){auto& x=a.glyphs[j];auto& y=b.m_glyphs[j];
                y.m_glyph_index=x.index;y.m_glyph_advance=x.advance;y.m_fontsize=std::uint16_t(x.height);
                y.m_bounds.m_x_min=x.x0;y.m_bounds.m_x_max=x.x1;y.m_bounds.m_y_min=x.y0;y.m_bounds.m_y_max=x.y1;
                y.m_bitmap_info=nullptr;y.m_shape_glyph=nullptr;
                if(x.image){auto pin=inline_images.find(std::weak_ptr<void>(x.image));
                    y.m_bitmap_info=pin!=inline_images.end()?pin->second->bitmap.get_ptr():platform->core_bitmap(x,e);
                    checked(y.m_bitmap_info!=nullptr,e);
                }
            }
        }
    }
    template<class Call> bool external(Call&& call){export_simple();const bool ok=call();import_simple();return ok;}
    edit_text_v1::Services services(){
        text_v1::Services s;
        s.kerning=[this](const auto& font,int a,int b,float& out,std::string& e){return external([&]{auto* f=platform->core_font(font,field.get_player(),e);if(!f)return false;out=f->get_kerning_adjustment(a,b);return true;});};
        s.preload_enabled=[this](bool& out,std::string&){out=platform->policy().auto_preload;return true;};
        s.preload=[this](auto& state,std::string& e){auto& callback=platform->policy().preload;
            if(!callback){e="source enabled auto-preload requires actual glyph/cache producer";return false;}return callback(state,e);};
        s.diagnostic_state=process_diagnostics();
        s.missing_glyph=[this](const auto&,int code,std::string& e){return external([&]{auto p=platform->services();if(!platform->diagnostic_available()){e="source missing-glyph diagnostic unavailable";return false;}
            auto message="source edit text missing glyph "+std::to_string(code);p.diagnostic(p.context,true,message.c_str());return true;});};
        s.image=[this](const std::string& name,int,int,text_v1::Image& out,std::string& e){return external([&]{
            auto* resource=field.find_exported_resource(name.c_str());
            if(!resource)return true;auto* bitmap=dynamic_cast<gameswf::bitmap_character_def*>(resource);
            if(!bitmap){e="source inline image export is not a bitmap resource";return false;}
            auto image=std::make_shared<InlineImage>();image->bitmap=bitmap->get_bitmap_info();
            if(!image->bitmap){e="source inline image bitmap unavailable";return false;}
            out.native=image;out.width=image->bitmap->get_width();out.height=image->bitmap->get_height();inline_images[std::weak_ptr<void>(image)]=image;return true;
        });};
        auto owned=platform->layout_services(std::move(s));
        auto units=owned.units_per_em;owned.units_per_em=[this,units](const auto& f,float& out,std::string& e){return external([&]{return units(f,out,e);});};
        auto height=owned.font_height;owned.font_height=[this,height](const auto& f,float& out,std::string& e){return external([&]{return height(f,out,e);});};
        auto glyph=owned.glyph;owned.glyph=[this,glyph](const auto& f,auto code,auto size,auto& out,bool& found,std::string& e){return external([&]{return glyph(f,code,size,out,found,e);});};
        edit_text_v1::Services result;result.layout=std::move(owned);
        result.read_bound=[this](const std::string& name,edit_text_v1::BoundRead& out,std::string&){
            export_simple();gameswf::gc_ptr<gameswf::as_object> parent=field.get_parent();tu_string path,var=name.c_str();
            if(gameswf::as_environment::parse_path(name.c_str(),&path,&var))parent=parent?parent->find_target(path.c_str()):nullptr;
            auto value=std::make_shared<gameswf::as_value>();out.found=parent&&parent->get_member(var,value.get());out.self=value->to_object()==&field;
            out.to_text=[this,value](std::string& s,std::string&){export_simple();s=value->to_tu_string().c_str();import_simple();return true;};import_simple();return true;
        };
        result.write_bound=[this](const std::string& name,const std::string& text,std::string&){export_simple();gameswf::gc_ptr<gameswf::as_object> parent=field.get_parent();tu_string path,var=name.c_str();
            if(gameswf::as_environment::parse_path(name.c_str(),&path,&var))parent=parent?parent->find_target(path.c_str()):nullptr;
            if(parent)parent->set_member(var,text.c_str());import_simple();return true;};
        result.initialize_fill=[this](std::string&){field.m_dummy_style.clear();field.m_dummy_style.push_back(gameswf::fill_style());return true;};return result;
    }
};
SwfEditTextFieldV1::SwfEditTextFieldV1(gameswf::edit_text_character& f):impl(std::make_unique<Impl>(f)){}
SwfEditTextFieldV1::~SwfEditTextFieldV1()=default;
void SwfEditTextFieldV1::initialize(){std::string e;checked(edit_text_v1::initialize(impl->state,impl->services(),e),e);impl->export_records();}
void SwfEditTextFieldV1::set_text(const std::string& input,bool html,bool write){impl->import_simple();std::string e;auto s=impl->services();bool ok=write?edit_text_v1::set_text_value(impl->state,input,html,s,e):edit_text_v1::set_text(impl->state,input,html,s,e);impl->export_records();checked(ok,e);}
void SwfEditTextFieldV1::format(bool html){impl->import_simple();std::string e;auto ok=edit_text_v1::format(impl->state,html,impl->services(),e);impl->export_records();checked(ok,e);}
void SwfEditTextFieldV1::refresh(){impl->import_simple();std::string e;auto ok=edit_text_v1::refresh_bound(impl->state,impl->services(),e);impl->export_records();checked(ok,e);}
void SwfEditTextFieldV1::get_format(const gameswf::fn_call& fn){impl->import_simple();std::string e;edit_text_v1::FormatWriter w;
    w.construct=[this,&fn](std::string&){gameswf::as_global_textformat_ctor(fn);impl->import_simple();return true;};
    w.intern=[](const std::string& input,std::string& out,std::string&){out=input;return true;};
    w.write=[this,&fn](const char* key,const edit_text_v1::FormatValue& value,std::string& error){impl->export_simple();auto* object=fn.result?fn.result->to_object():nullptr;
        if(!object){error="source getTextFormat constructor returned no object";return false;}
        gameswf::as_value v;if(auto p=std::get_if<double>(&value))v.set_double(*p);else if(auto p=std::get_if<bool>(&value))v.set_bool(*p);else v.set_tu_string(std::get<std::string>(value).c_str());
        object->set_member(key,v);impl->import_simple();return true;};checked(edit_text_v1::get_format(impl->state,w,e),e);
}
void SwfEditTextFieldV1::display_records(){display_records({},false);}
void SwfEditTextFieldV1::display_records(const text_display_v2::Context& request,bool supplied_matrix){impl->import_simple();std::string e;text_display_v2::Services seed;
    seed.resolve_font=[](auto& state,std::size_t i,std::string& error){if(i>=state.records.size()){error="source record removed";return false;}if(!state.records[i].font&&state.records[i].font_id>=0){error="required source record font resolver unavailable";return false;}return true;};
    seed.transform_color=[this](unsigned input,unsigned& output,std::string&){auto cx=impl->field.get_world_cxform();output=packed(cx.transform(color(input)));return true;};
    seed.shape=[this](const auto& m,const auto& font,std::int16_t index,float pixel,unsigned rgba,std::string& error){auto* f=impl->platform->core_font(font,impl->field.get_player(),error);if(!f)return false;
        auto* shape=f->get_glyph_by_index(index);if(!shape)return true;array<gameswf::fill_style> fills;fills.resize(1);fills[0].set_color(color(rgba));array<gameswf::line_style> lines;
        shape->display(core_matrix(m),impl->field.get_world_cxform(),pixel,fills,lines,gameswf::render_handler::BLEND_NORMAL);return true;};
    auto source=impl->platform->display_services(std::move(seed));auto bind=source.bind_glyph;auto bitmap=source.bitmap;
    source.bind_glyph=[this,bind](const auto& g,auto& out,std::string& error){auto found=impl->inline_images.find(std::weak_ptr<void>(g.image));if(found==impl->inline_images.end())return bind(g,out,error);
        out.bitmap={g.image,found->second->bitmap->get_width(),found->second->bitmap->get_height()};return true;};
    source.bitmap=[this,bitmap](const auto& m,const auto& b,const auto& r,const auto& uv,unsigned rgba,std::string& error){auto found=impl->inline_images.find(std::weak_ptr<void>(b.owner));if(found==impl->inline_images.end())return bitmap(m,b,r,uv,rgba,error);
        gameswf::rect rect,tex;rect.m_x_min=r[0];rect.m_x_max=r[1];rect.m_y_min=r[2];rect.m_y_max=r[3];tex.m_x_min=uv[0];tex.m_x_max=uv[1];tex.m_y_min=uv[2];tex.m_y_max=uv[3];
        gameswf::render::draw_bitmap(core_matrix(m),found->second->bitmap.get_ptr(),rect,tex,color(rgba));return true;};
    text_display_v2::Context context=request;auto world=impl->field.get_world_matrix();if(supplied_matrix)world.concatenate(core_matrix(request.matrix));context.matrix=matrix(world);context.provider_scale=impl->platform->provider_scale();context.pixel_scale=impl->field.get_pixel_scale();
    checked(text_display_v2::display(impl->state.layout,context,source,e),e);
}
namespace {
struct OuterConnection {
 SwfEditTextFieldV1& receiver;SwfEditTextFieldV1::Impl& impl;
 edit_text_display_v1::State state;std::vector<std::shared_ptr<edit_text_display_v1::Owner>> parents;
 std::map<gameswf::character*,std::shared_ptr<edit_text_display_v1::Owner>> owners;
 SwfDraw draw;
 void refresh(){auto&f=impl.field;state.border=f.m_def->m_border;state.grid_fit=f.m_def->m_grid_fit;state.focus=f.m_has_focus;state.callback_present=f.m_display_callback!=nullptr;
  state.rectangle={f.m_def->m_rect.m_x_min,f.m_def->m_rect.m_x_max,f.m_def->m_rect.m_y_min,f.m_def->m_rect.m_y_max};state.background=packed(f.m_background_color);state.xcursor=f.m_xcursor;state.ycursor=f.m_ycursor;state.text_height=f.m_text_height;
  parents.clear();auto* character=static_cast<gameswf::character*>(&f);std::shared_ptr<edit_text_display_v1::Owner> previous;
  while(character){auto& next=owners[character];if(!next)next=std::make_shared<edit_text_display_v1::Owner>();next->effect=character->m_source_effect_v1;next->parent.reset();if(previous)previous->parent=next;else state.owner=next;parents.push_back(next);previous=next;character=character->get_parent();}
 }
 edit_text_display_v1::Services services(){using Command=edit_text_display_v1::Command;edit_text_display_v1::Services s;
  s.policy=[this](auto&out,auto& e){refresh();auto* player=impl.field.get_player();auto* root=player?player->get_root():nullptr;
   if(!root){e="Source text display requires SAME actual player/root";return false;}
   auto&p=impl.platform->policy();out={root->source_text_buffering_v98(),root->source_text_flushing_v98(),p.render_cache,p.filter_engine,impl.platform->provider_scale()};return true;};
  s.renderer_present=[this](bool&out,auto&){out=impl.platform->services().draw!=nullptr;return true;};
  s.world_matrix=[this](auto&out,auto&){out=matrix(impl.field.get_world_matrix());return true;};
  s.renderer=[this](const Command&c,std::string&e){auto service=impl.platform->services();
   if(c.kind==Command::grid_fit||c.kind==Command::cache_set||c.kind==Command::cache_draw){auto&feature=impl.platform->policy().renderer_feature;if(!feature){e="source text renderer feature provider unavailable";return false;}const bool ok=feature(c,e);refresh();return ok;}
   if(c.kind==Command::matrix){std::copy(c.transform.begin(),c.transform.end(),draw.matrix.value);return true;}
   if(c.kind==Command::fill_color||c.kind==Command::line_color){auto&f=c.kind==Command::fill_color?draw.fill:draw.line;f.kind=SwfFill::color;for(unsigned i=0;i<4;++i)f.rgba[i]=std::uint8_t(c.rgba>>(i*8));return true;}
   if(c.kind==Command::line_width){draw.line_width=c.width;return true;}
   draw.kind=c.kind==Command::mesh?SwfDraw::triangle_strip:SwfDraw::line_strip;draw.xy=c.xy;
   if(!service.draw){e="source text draw sink unavailable";return false;}const bool ok=service.draw(service.context,draw,e);refresh();return ok;
  };
  s.enqueue=[this](auto& e){auto* player=impl.field.get_player();auto* root=player?player->get_root():nullptr;
   if(!root){e="Source text enqueue requires SAME actual player/root";return false;}
   //Original792e10 retains the actual native character, not a sidecar snapshot.
   root->source_queue_text_v98(impl.field);return true;};
  s.cache_validate=[this](bool&out,std::string&e){auto&cache=impl.platform->policy().cache_validate;if(!cache){e="source text render-cache validator unavailable";return false;}const bool ok=cache(out,e);refresh();return ok;};
  s.records=[this](const auto&ctx,bool supplied,std::string&){receiver.display_records(ctx,supplied);refresh();return true;};
  s.cosine=[](float f,float&out,auto&){out=std::cos(f);return true;};s.sine=[](float f,float&out,auto&){out=std::sin(f);return true;};
  s.display_callback=[this](auto&){impl.field.do_display_callback();refresh();return true;};return s;
 }
};
}
void SwfEditTextFieldV1::display(){OuterConnection connection{*this,*impl};connection.refresh();std::string e;checked(edit_text_display_v1::display(connection.state,connection.services(),e),e);}
void SwfEditTextFieldV1::cursor(){OuterConnection connection{*this,*impl};connection.refresh();std::string e;checked(edit_text_display_v1::cursor(connection.state,connection.services(),e),e);}
bool SwfEditTextFieldV1::event(std::uint8_t id,std::uint8_t key){
 edit_text_event_v1::State state;auto refresh=[&]{auto&f=impl->field;state.readonly=f.m_def->m_readonly;state.focus=f.m_has_focus;state.cursor=f.m_cursor;state.text=std::string(f.m_text.c_str(),f.m_text.size());};
 auto publish=[&]{impl->field.m_has_focus=state.focus;impl->field.m_cursor=state.cursor;};refresh();edit_text_event_v1::Services s;
 s.active=[&](auto&){publish();auto*root=impl->field.get_player()->get_root();if(!root)throw std::runtime_error("source focus requires current root");root->set_active_entity(&impl->field);refresh();return true;};
 s.handler=[&](const char*name,auto&){publish();gameswf::as_value method;if(impl->field.get_member(name,&method)){
  gameswf::as_environment env(impl->field.get_player());env.push(gameswf::as_value());gameswf::call_method(method,&env,&impl->field,1,env.get_top_index());}refresh();return true;};
 s.listener=[&](bool add,auto&){publish();auto*root=impl->field.get_player()->get_root();if(!root)throw std::runtime_error("source focus requires current root");if(add)root->m_keypress_listener.add(&impl->field);else root->m_keypress_listener.remove(&impl->field);refresh();return true;};
 s.format=[&](auto&){publish();format(false);refresh();return true;};s.set_value=[&](const auto&text,auto&){publish();set_text(text,false,true);refresh();return true;};
 bool accepted{};std::string e;const auto ok=edit_text_event_v1::event(state,id,key,s,accepted,e);publish();checked(ok,e);return accepted;
}
}
