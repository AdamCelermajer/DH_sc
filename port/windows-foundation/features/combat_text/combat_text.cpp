#include "combat_text.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>

namespace dh::foundation {
namespace {
struct SourceFontAdvance {std::uint32_t code;float advance;};
struct SourceFontPair {std::uint32_t first,second;float adjustment;};
#include "style_data.inc"
using Services=dh2::character::skills::CombatTextServicesV1;
struct Sink {const Services* source;std::vector<CombatTextDisplayEvent>* output;};
bool source_layout(HudGlyphRun& run,float height,std::string& error){
    // Source Font512 has zero embedded glyphs: its original device-font
    // advances come from the supplied FreeType face, not an invented table.
    if(advances512.empty())return true;
    float old=0,next=0;std::uint32_t previous=UINT32_MAX;
    for(auto& glyph:run.glyphs){
        for(const auto& pair:pairs512)if(pair.first==previous&&pair.second==glyph.codepoint){next+=pair.adjustment*height/1024.f;break;}
        const auto entry=std::find_if(std::begin(advances512),std::end(advances512),[&](auto& a){return a.code==glyph.codepoint;});
        if(entry==std::end(advances512)){error="Combat source Font512 has no requested glyph";return false;}
        glyph.x+=next-old;old+=glyph.advance;glyph.advance=entry->advance*height/1024.f;next+=glyph.advance;previous=glyph.codepoint;
    }
    run.advance=next;return true;
}
}
const CombatTextStyle* original_combat_text_style(const std::string& name)noexcept{
    for(const auto& style:sourceStyles)if(name==style.name)return &style;return nullptr;
}
const char* original_combat_text_font_resource()noexcept{return sourceCombatFontResource;}
float original_combat_text_frame_rate()noexcept{return sourceCombatFrameRate;}
bool select_combat_text(const PlayableCombatResolution& resolution,const Services& original,
                       std::vector<CombatTextDisplayEvent>& output,std::string& error){
    output.clear();Sink sink{&original,&output};Services s{};s.context=&sink;
    if(original.follower)s.follower=[](void*p,std::uintptr_t a,bool*v){auto&x=*static_cast<Sink*>(p);return x.source->follower(x.source->context,a,v);};
    if(original.position)s.position=[](void*p,std::uintptr_t a,float*v){auto&x=*static_cast<Sink*>(p);return x.source->position(x.source->context,a,v);};
    if(original.height)s.height=[](void*p,std::uintptr_t a,float*v){auto&x=*static_cast<Sink*>(p);return x.source->height(x.source->context,a,v);};
    if(original.property)s.property=[](void*p,std::uintptr_t a,int k,std::int32_t*v){auto&x=*static_cast<Sink*>(p);return x.source->property(x.source->context,a,k,v);};
    if(original.dual_wield)s.dual_wield=[](void*p,std::uintptr_t a,bool*v){auto&x=*static_cast<Sink*>(p);return x.source->dual_wield(x.source->context,a,v);};
    if(original.is_player)s.is_player=[](void*p,std::uintptr_t a,bool*v){auto&x=*static_cast<Sink*>(p);return x.source->is_player(x.source->context,a,v);};
    if(original.constant)s.constant=[](void*p,const char*g,const char*k,std::int32_t*v){auto&x=*static_cast<Sink*>(p);return x.source->constant(x.source->context,g,k,v);};
    if(original.localized)s.localized=[](void*p,std::int32_t k,const char**v){auto&x=*static_cast<Sink*>(p);return x.source->localized(x.source->context,k,v);};
    s.enqueue=[](void*p,const dh2::character::skills::CombatTextRequestV1*q){
        if(!q||!q->style)return 1;auto&sink=*static_cast<Sink*>(p);CombatTextDisplayEvent event;
        event.position={q->position[0],q->position[1],q->position[2]};event.style=q->style;event.argb=q->color;event.numeric=q->numeric;
        event.text=q->numeric?std::to_string(q->number):(q->text?q->text:"");sink.output->push_back(std::move(event));return 0;
    };
    const auto result=dh2::character::skills::character_combat_text_v1(resolution.melee.original,
        static_cast<std::uintptr_t>(resolution.attacker),static_cast<std::uintptr_t>(resolution.victim),s);
    if(result<0){error="Original scrolling-combat-text reached a missing/failed actor, design or localization service";return false;}
    error.clear();return true;
}
bool CombatTextPresenter::enqueue(const CombatTextDisplayEvent&event,const Project&project,float sx,float sy,std::string&error){
    const auto*style=original_combat_text_style(event.style);
    if(!style||!project||!std::isfinite(sx)||!std::isfinite(sy)||sx<=0||sy<=0){error="Original combat style/project/viewport unavailable";return false;}
    if(event.text.size()>46){error="Combat text exceeds original queue string capacity";return false;}
    float x=0,y=0;if(!project(event.position,x,y,error))return false;
    if(!std::isfinite(x)||!std::isfinite(y)||std::abs(x)>=2147483647.f||std::abs(y)>=2147483647.f||std::abs(x/sx)>=2147483647.f||std::abs(y/sy)>=2147483647.f){error="Combat projection outside source integer domain";return false;}
    for(auto&ctx:contexts_)if(!ctx.active){ctx={};ctx.active=true;ctx.event=event;ctx.style=style;ctx.x=static_cast<std::int32_t>(static_cast<std::int32_t>(x)/sx);ctx.y=static_cast<std::int32_t>(static_cast<std::int32_t>(y)/sy);error.clear();return true;}
    error.clear();return true; // genuine bounded full-queue drop
}
bool CombatTextPresenter::update_ms(std::uint32_t dt,std::int32_t interval,std::string&error){
    if(interval<=0){error="Combat source frame interval must be positive";return false;}
    for(auto&ctx:contexts_)if(ctx.active){
        const auto sum=std::uint32_t(ctx.timer)+dt;std::memcpy(&ctx.timer,&sum,4);
        while(ctx.timer>interval&&ctx.active){ctx.timer-=interval;++ctx.frame;if(ctx.frame>=ctx.style->frame_count)ctx.active=false;}
    }
    error.clear();return true;
}
bool CombatTextPresenter::geometry(HudGlyphFont&font,float sx,float sy,std::vector<CombatTextGlyph>&output,std::string&error)const{
    if(!std::isfinite(sx)||!std::isfinite(sy)||sx<=0||sy<=0){error="Combat viewport scale invalid";return false;}
    std::vector<CombatTextGlyph> result;
    for(const auto&ctx:contexts_)if(ctx.active){
        const auto&s=*ctx.style;const auto&m=s.frames[ctx.frame];HudGlyphRun run;
        const float rasterScale=std::max(std::hypot(m[0]*sx,m[1]*sy),std::hypot(m[2]*sx,m[3]*sy));
        if(!font.raster(ctx.event.text,static_cast<int>(s.height),rasterScale,run,error)||!source_layout(run,s.height,error))return false;
        const float spare=s.bounds[1]-(s.bounds[0]+run.advance)-4.f;
        float bx=s.bounds[0]+2.f;if(s.align==2)bx+=spare*.5f;else if(s.align==1)bx+=spare;
        const float by=s.bounds[2]+s.height+(s.font_metrics[2]-s.font_metrics[1])*s.height/1024.f;
        const auto color=static_cast<std::uint32_t>(ctx.event.argb);
        // Native field cxform keeps alpha multiplier1 and adds ARGB alpha.
        // Source field color alpha255 therefore saturates at1; texture glyph
        // coverage remains its own alpha. Do not invent a linear lifetime fade.
        const std::array<float,4> rgba{float((color>>16)&255)/255.f,float((color>>8)&255)/255.f,float(color&255)/255.f,1.f};
        for(auto&glyph:run.glyphs){if(!glyph.width||!glyph.height)continue;CombatTextGlyph quad;quad.rgba=rgba;
            const float x=bx+glyph.x,y=by+glyph.y;
            const float points[8]{x,y,x+glyph.width,y,x+glyph.width,y+glyph.height,x,y+glyph.height};
            for(unsigned i=0;i<4;++i){quad.xy[i*2]=(ctx.x+m[0]*points[i*2]+m[2]*points[i*2+1]+m[4])*sx;quad.xy[i*2+1]=(ctx.y+m[1]*points[i*2]+m[3]*points[i*2+1]+m[5])*sy;}
            quad.glyph=std::move(glyph);result.push_back(std::move(quad));
        }
    }
    output=std::move(result);error.clear();return true;
}
void CombatTextPresenter::clear()noexcept{for(auto&ctx:contexts_)ctx.active=false;}
std::size_t CombatTextPresenter::active_count()const noexcept{std::size_t n=0;for(const auto&ctx:contexts_)if(ctx.active)++n;return n;}
} // namespace dh::foundation
