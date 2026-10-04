#include "swf_text_font_platform_v1.hpp"
#include "swf_edit_text_connection_v1.hpp"
#include "gameswf/gameswf_font.h"
#include "gameswf/gameswf_render.h"
#include <map>
#include <tuple>
#include <stdexcept>
#include <algorithm>

namespace dh2::ui {
namespace {
struct FontPin { gameswf::gc_ptr<gameswf::font> value; };
struct DevicePin { std::string name;bool bold{},italic{}; };
bool require(bool b,const char* name,std::string& error){if(b)return true;error=std::string("source text platform requires ")+name;return false;}
}
struct SwfTextFontPlatformV1::Impl:std::enable_shared_from_this<Impl> {
    struct Pixels { SwfTexture texture;int channels{},pitch{};std::vector<std::uint8_t> bytes; };
    struct CoreImage { gameswf::gc_ptr<gameswf::bitmap_info> bitmap;std::shared_ptr<void> source; };
    struct Provider:gameswf::glyph_provider {
        std::weak_ptr<Impl> parent;
        gameswf::bitmap_info* get_char_image(gameswf::character_def*,Uint16 code,const tu_string& name,
            bool bold,bool italic,int size,gameswf::rect* bounds,float* advance) override {
            auto p=parent.lock();if(!p)return nullptr;
            p->failure.clear();
            if(!require(bounds&&advance,"glyph output storage",p->failure))return p->failed();
            const auto key=std::make_tuple(std::string(name.c_str(),name.size()),bold,italic);
            auto it=p->devices.find(key);
            if(it==p->devices.end()){
                auto f=std::make_shared<text_v1::Font>();f->name=std::get<0>(key);f->bold=bold;f->italic=italic;
                f->native=std::make_shared<DevicePin>(DevicePin{f->name,bold,italic});
                it=p->devices.emplace(key,std::move(f)).first;
            }
            text_v1::Glyph g;bool found{};
            if(!p->glyph(it->second,code,size,g,found,p->failure))return p->failed();
            *advance=g.advance;
            if(!g.image)return nullptr; // source font::get_glyph performs embedded fallback
            auto bind=p->text->display_services({});text_display_v2::GlyphBinding binding;
            if(!bind.bind_glyph(g,binding,p->failure))return p->failed();
            auto image=p->pixels.find(binding.bitmap.owner? p->texture_identity(g,p->failure):0);
            if(image==p->pixels.end()) {p->failure="source core glyph has no captured alpha upload";return p->failed();}
            auto core=p->core.find(image->first);
            if(core==p->core.end()) {
                // Materialize the facade's genuine bitmap object while reusing
                // the exact already-uploaded texture. No second raster, face,
                // or GPU upload is substituted for the source glyph.
                auto old=p->replay;p->replay=&image->second;
                struct Restore { Impl* p;const Pixels* old;~Restore(){p->replay=old;} } restore{p.get(),old};
                gameswf::gc_ptr<gameswf::bitmap_info> bitmap=gameswf::render::create_bitmap_info_alpha(
                    image->second.texture.width,image->second.texture.height,image->second.bytes.data());
                if(!require(bitmap!=nullptr,"active core alpha bitmap factory",p->failure))return p->failed();
                if(!require(bitmap->get_width()==image->second.texture.width&&bitmap->get_height()==image->second.texture.height,
                            "unchanged core bitmap dimensions",p->failure))return p->failed();
                core=p->core.emplace(image->first,CoreImage{bitmap,g.image}).first;
            }
            bounds->m_x_min=g.x0;bounds->m_x_max=g.x1;bounds->m_y_min=g.y0;bounds->m_y_max=g.y1;
            return core->second.bitmap.get_ptr();
        }
    };
    SwfFontServices fonts;SwfServices base;std::shared_ptr<void> lifetime;
    float scale;std::string failure;
    SwfTextPolicyV1 policy;
    std::vector<std::weak_ptr<SwfEditTextFieldV1>> buffered;
    std::unique_ptr<TextRenderOwnerV2> text;gameswf::gc_ptr<Provider> provider;
    std::map<std::tuple<std::string,bool,bool>,std::shared_ptr<text_v1::Font>> devices;
    std::map<gameswf::font*,std::weak_ptr<text_v1::Font>> projected;
    std::map<std::weak_ptr<void>,std::weak_ptr<FontPin>,std::owner_less<std::weak_ptr<void>>> font_pins;
    std::map<std::weak_ptr<void>,std::shared_ptr<FontPin>,std::owner_less<std::weak_ptr<void>>> cloned_core;
    static std::map<gameswf::player*,std::weak_ptr<Impl>>& players(){static std::map<gameswf::player*,std::weak_ptr<Impl>> table;return table;}
    std::map<std::uintptr_t,Pixels> pixels;std::map<std::uintptr_t,CoreImage> core;
    const Pixels* replay{};
    std::uintptr_t last_upload{};
    std::map<std::tuple<std::string,bool,bool,std::uint16_t,int>,std::uintptr_t> glyph_images;
    Impl(SwfFontServices f,SwfServices r,std::shared_ptr<void> owner,float s):fonts(f),base(std::move(r)),lifetime(std::move(owner)),scale(s){}
    gameswf::bitmap_info* failed(){if(fonts.diagnostic)fonts.diagnostic(fonts.context,failure.c_str());return nullptr;}
    bool glyph(const std::shared_ptr<text_v1::Font>& f,std::uint16_t code,int size,text_v1::Glyph& g,bool& found,std::string& error){
        last_upload=0;
        if(!text->glyph(f,code,size,g,found,error))return false;
        if(!g.image)return true;
        auto key=std::make_tuple(f->name,f->bold,f->italic,code,size);
        if(last_upload)glyph_images[key]=last_upload;
        auto it=glyph_images.find(key);
        if(!require(it!=glyph_images.end(),"captured genuine glyph texture",error))return false;
        image_ids[std::weak_ptr<void>(g.image)]=it->second;return true;
    }
    std::uintptr_t texture_identity(const text_v1::Glyph& g,std::string& error){
        std::uintptr_t id{};
        // V2 owns the image-to-SwfTexture registry. Read its actual emitted
        // texture through a local sink, never cast an arbitrary void owner.
        // The image registry below is also keyed by the actual upload texture;
        // match the retained image pin captured at glyph binding.
        for(const auto& item:core)if(item.second.source==g.image)return item.first;
        for(const auto& item:image_ids)if(!item.first.owner_before(g.image)&&!std::weak_ptr<void>(g.image).owner_before(item.first))return item.second;
        error="source glyph image is not registered with platform";return id;
    }
    std::map<std::weak_ptr<void>,std::uintptr_t,std::owner_less<std::weak_ptr<void>>> image_ids;
    static bool upload(void* c,int w,int h,unsigned channels,const std::uint8_t* bytes,int pitch,SwfTexture& out,std::string& error){
        auto& p=*static_cast<Impl*>(c);
        if(p.replay){auto& r=*p.replay;
            if(!require(channels==unsigned(r.channels)&&w==r.texture.width&&h==r.texture.height&&pitch==r.pitch&&bytes,
                        "matching core alpha replay",error))return false;
            for(int y=0;y<h;++y)if(!std::equal(bytes+std::size_t(y)*pitch,bytes+std::size_t(y)*pitch+w*channels,r.bytes.data()+std::size_t(y)*pitch)){
                error="core alpha bitmap changed source pixels";return false;}
            out=r.texture;return true;
        }
        if(!require(bool(p.base.image),"actual image upload service",error))return false;
        if(!p.base.image(p.base.context,w,h,channels,bytes,pitch,out,error))return false;
        if(channels==1&&w>0&&h>0&&pitch>=w&&bytes){
            Pixels pixels;pixels.texture=out;pixels.channels=channels;pixels.pitch=w;
            pixels.bytes.resize(std::size_t(w)*h);
            for(int y=0;y<h;++y)std::copy_n(bytes+std::size_t(y)*pitch,w,pixels.bytes.data()+std::size_t(y)*w);
            p.pixels[out.identity]=std::move(pixels);
            p.last_upload=out.identity;
        }
        return true;
    }
    static bool read(void*c,const char*n,std::vector<std::uint8_t>&o,std::string&e){auto&p=*static_cast<Impl*>(c);return require(bool(p.base.read),"resource reader",e)&&p.base.read(p.base.context,n,o,e);}
    static bool texture(void*c,const char*n,int w,int h,SwfTexture&o,std::string&e){auto&p=*static_cast<Impl*>(c);return require(bool(p.base.texture),"texture resolver",e)&&p.base.texture(p.base.context,n,w,h,o,e);}
    static bool draw(void*c,const SwfDraw&d,std::string&e){auto&p=*static_cast<Impl*>(c);return require(bool(p.base.draw),"draw sink",e)&&p.base.draw(p.base.context,d,e);}
    static bool native(void*c,const char*n,const std::vector<SwfValue>&a,SwfValue&o,std::string&e){auto&p=*static_cast<Impl*>(c);return require(bool(p.base.native_call),"native callback",e)&&p.base.native_call(p.base.context,n,a,o,e);}
    static bool stencil(void*c,const float*r,std::uint8_t b,bool&o,std::string&e){auto&p=*static_cast<Impl*>(c);return require(bool(p.base.stencil),"stencil query",e)&&p.base.stencil(p.base.context,r,b,o,e);}
    static void diagnostic(void*c,bool b,const char*n){auto&p=*static_cast<Impl*>(c);if(p.base.diagnostic)p.base.diagnostic(p.base.context,b,n);}
    static bool action(void*c,const char*n,const gameswf::fn_call&f,std::string&e){auto&p=*static_cast<Impl*>(c);return require(bool(p.base.native_action),"typed native callback",e)&&p.base.native_action(p.base.context,n,f,e);}
    static bool start(void*c,const SwfAsLease&l,std::string&e){auto&p=*static_cast<Impl*>(c);
        if(!require(l.player&&bool(l.owner),"actual retained startup player",e))return false;
        auto& table=players();for(auto it=table.begin();it!=table.end();)if(it->second.expired())it=table.erase(it);else++it;
        auto old=table[l.player].lock();if(old&&old.get()!=&p)return require(false,"one text platform per player",e);
        table[l.player]=p.shared_from_this();
        if(p.base.graph_start&&!p.base.graph_start(p.base.context,l,e)){table.erase(l.player);return false;}return true;}
    SwfServices service(){auto s=base;s.context=this;s.read=base.read?read:nullptr;s.texture=base.texture?texture:nullptr;
        s.image=upload;s.draw=base.draw?draw:nullptr;s.native_call=base.native_call?native:nullptr;s.stencil=base.stencil?stencil:nullptr;
        s.diagnostic=diagnostic;s.native_action=base.native_action?action:nullptr;s.graph_start=start;s.native_owner=shared_from_this();s.glyphs=provider.get_ptr();return s;}
};
SwfTextFontPlatformV1::SwfTextFontPlatformV1(SwfFontServices f,SwfServices s,std::shared_ptr<void> life,TextFontBackendsV2 b,float scale)
    :impl_(std::make_shared<Impl>(f,std::move(s),std::move(life),scale)) {
    if(!impl_->lifetime||!f.read)throw std::invalid_argument("source text platform requires retained actual font/resource owner");
    if(!b.bitmap_face)throw std::invalid_argument("source text platform requires actual bitmap font resolver");
    auto weak=std::weak_ptr<Impl>(impl_);auto prior=b.embedded_glyph;
    b.embedded_glyph=[weak,prior](const text_v1::Font& f,std::uint16_t code,TextEmbeddedGlyphV2& out,std::string& e){
        auto p=weak.lock();if(!p)return require(false,"live platform",e);
        auto it=p->font_pins.find(std::weak_ptr<void>(f.native));auto pin=it==p->font_pins.end()?nullptr:it->second.lock();
        if(pin){int index=-1;bool advance{};float value{};out.found=pin->value->source_embedded_glyph(code,index,advance,value);out.index=index;out.has_advance=advance;out.advance=value;return true;}
        if(prior)return prior(f,code,out,e);
        // Core device-only requests intentionally have no embedded tables.
        for(const auto& device:p->devices)if(device.second->native==f.native){out={};return true;}
        return require(false,"registered embedded font producer",e);
    };
    impl_->text=std::make_unique<TextRenderOwnerV2>(f,impl_->service(),impl_->lifetime,std::move(b),scale);
    impl_->provider=new Impl::Provider;impl_->provider->parent=impl_;
}
SwfTextFontPlatformV1::~SwfTextFontPlatformV1()=default;
SwfTextFontPlatformV1::SwfTextFontPlatformV1(std::shared_ptr<Impl> p):impl_(std::move(p)){}
std::shared_ptr<SwfTextFontPlatformV1> SwfTextFontPlatformV1::for_player(gameswf::player* player){auto it=Impl::players().find(player);auto p=it==Impl::players().end()?nullptr:it->second.lock();return p?std::shared_ptr<SwfTextFontPlatformV1>(new SwfTextFontPlatformV1(std::move(p))):nullptr;}
SwfServices SwfTextFontPlatformV1::services()const{return impl_->service();}
std::shared_ptr<text_v1::Font> SwfTextFontPlatformV1::font(gameswf::font* font,std::string& e){
    if(!require(font!=nullptr,"actual core font",e))return {};
    if(auto p=impl_->projected[font].lock()){
        p->name=font->get_name().c_str();p->bold=font->is_bold();p->italic=font->is_italic();
        p->define_font3=font->is_define_font3();p->metric58=font->get_descent();p->metric5c=font->get_leading();return p;}
    auto pin=std::make_shared<FontPin>();pin->value=font;
    auto p=std::make_shared<text_v1::Font>();p->native=pin;p->name=font->get_name().c_str();p->bold=font->is_bold();p->italic=font->is_italic();
    p->define_font3=font->is_define_font3();p->metric58=font->get_descent();p->metric5c=font->get_leading();
    impl_->projected[font]=p;impl_->font_pins[std::weak_ptr<void>(pin)]=pin;return p;
}
gameswf::font* SwfTextFontPlatformV1::core_font(const std::shared_ptr<text_v1::Font>& f,gameswf::player* player,std::string& e){
    if(!require(bool(f)&&player,"owned font projection/player",e))return nullptr;
    auto key=std::weak_ptr<void>(f->native);auto it=impl_->font_pins.find(key);
    if(it!=impl_->font_pins.end())if(auto pin=it->second.lock())return pin->value.get_ptr();
    for(auto i=impl_->cloned_core.begin();i!=impl_->cloned_core.end();)if(i->first.expired())i=impl_->cloned_core.erase(i);else++i;
    auto& pin=impl_->cloned_core[key];if(!pin){pin=std::make_shared<FontPin>();pin->value=new gameswf::font(player);}
    pin->value->set_name(f->name.c_str());pin->value->set_bold(f->bold);pin->value->set_italic(f->italic);pin->value->source_clone_metrics(f->metric58,f->metric5c);return pin->value.get_ptr();
}
gameswf::bitmap_info* SwfTextFontPlatformV1::core_bitmap(const text_v1::Glyph& g,std::string& e){
    auto identity=impl_->texture_identity(g,e);if(!identity)return nullptr;
    auto previous=impl_->core.find(identity);if(previous!=impl_->core.end())return previous->second.bitmap.get_ptr();
    auto it=impl_->pixels.find(identity);if(!require(it!=impl_->pixels.end(),"captured core glyph pixels",e))return nullptr;
    auto old=impl_->replay;impl_->replay=&it->second;
    struct Restore{Impl*p;const Impl::Pixels*old;~Restore(){p->replay=old;}}restore{impl_.get(),old};
    gameswf::gc_ptr<gameswf::bitmap_info> bitmap=gameswf::render::create_bitmap_info_alpha(it->second.texture.width,it->second.texture.height,it->second.bytes.data());
    if(!require(bitmap!=nullptr,"active core bitmap factory",e))return nullptr;
    return impl_->core.emplace(identity,Impl::CoreImage{bitmap,g.image}).first->second.bitmap.get_ptr();
}
text_v1::Services SwfTextFontPlatformV1::layout_services(text_v1::Services seed)const{
    auto p=impl_;auto result=p->text->layout_services(std::move(seed));
    result.root_scale_word=[p](float& out,std::string&){out=p->scale;return true;};
    result.glyph=[p](const auto& f,auto code,auto size,auto& g,bool& found,std::string& e){return p->glyph(f,code,size,g,found,e);};return result;
}
text_display_v2::Services SwfTextFontPlatformV1::display_services(text_display_v2::Services seed)const{return impl_->text->display_services(std::move(seed));}
float SwfTextFontPlatformV1::provider_scale()const{return impl_->scale;}
std::string SwfTextFontPlatformV1::error()const{return impl_->failure;}
SwfTextPolicyV1& SwfTextFontPlatformV1::policy(){return impl_->policy;}
bool SwfTextFontPlatformV1::diagnostic_available()const{return impl_->base.diagnostic!=nullptr;}
bool SwfTextFontPlatformV1::enqueue(std::weak_ptr<SwfEditTextFieldV1> field,std::string&e){if(!require(!field.expired(),"live queued text receiver",e))return false;impl_->buffered.push_back(std::move(field));return true;}
bool SwfTextFontPlatformV1::flush_buffered_text(std::string&e){
 e.clear();if(impl_->buffered.empty())return true;
 const bool previous_flushing=impl_->policy.flushing;impl_->policy.flushing=true;
 struct RestoreFlushing{SwfTextPolicyV1& policy;bool previous;~RestoreFlushing(){policy.flushing=previous;}} restore{impl_->policy,previous_flushing};
 for(std::size_t i=0;i<impl_->buffered.size();++i){auto field=impl_->buffered[i].lock();if(!require(bool(field),"live buffered text receiver",e))return false;
  try{field->display();}catch(const std::exception&x){e=x.what();return false;}}
 impl_->buffered.clear();return true;
}
}
