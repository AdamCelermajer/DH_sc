#include "text_display_v2.hpp"
#include <cmath>

namespace dh2::ui::text_display_v2 {
namespace {
bool required(bool available,const char* name,std::string& error) {
    if(available)return true;
    error=std::string("text display requires ")+name;return false;
}
float finite(float v) { return std::isfinite(v)?v:0.0f; }
Matrix positioned(const Matrix& base,float x,float y,float scale) {
    Matrix m=base;
    // The original fast path requires BOTH diagonal elements to equal one.
    // General scale does not sanitize its four linear elements; only the
    // fast path individually sanitizes its two scaled diagonal elements.
    if(base[1]==0 && base[3]==0 && base[0]==1 && base[4]==1) {
        m[2]=finite(x+base[2]);m[5]=finite(y+base[5]);
        m[0]=finite(scale*base[0]);m[4]=finite(scale*base[4]);
    } else {
        m[2]=finite((x*base[0]+y*base[1])+base[2]);
        m[5]=finite((x*base[3]+y*base[4])+base[5]);
        m[0]*=scale;m[1]*=scale;m[3]*=scale;m[4]*=scale;
    }
    return m;
}
}
bool display(text_v1::State& state,const Context& c,const Services& s,std::string& error) {
    error.clear();
    if(!c.renderer_present)return true;
    float x=0,y=0;
    const float denominator=c.provider_scale*1024.0f;
    // These are twice the byte argument times the provider scale; they are
    // NOT pixel sizes divided by scale. Keep the original operation order.
    const float expansion_x=(float(c.argument1)+float(c.argument1))*c.provider_scale;
    const float expansion_y=(float(c.argument2)+float(c.argument2))*c.provider_scale;
    const float expansion_both=(float(c.argument0)+float(c.argument0))*c.provider_scale;
    for(std::size_t i=0;i<state.records.size();++i) {
        if(!required(bool(s.resolve_font),"font resolution",error)||!s.resolve_font(state,i,error))return false;
        if(i>=state.records.size())return required(false,"stable current record",error);
        // Pin only style metadata. Copying the complete glyph vector here
        // would allocate and duplicate each field on every displayed frame.
        text_v1::Record record;
        { const auto& source=state.records[i];record.font=source.font;
          record.has_font=source.has_font;record.height=source.height;
          record.has_x=source.has_x;record.has_y=source.has_y;
          record.x=source.x;record.y=source.y;record.rgba=source.rgba; }
        if(!record.font && record.has_font)continue;
        float scale=record.height/denominator;
        if(record.font && record.font->define_font3)scale/=20.0f;
        if(record.has_x)x=record.x;
        if(record.has_y)y=record.y;
        std::uint32_t color{};
        if(!required(bool(s.transform_color),"world color transform",error)||
           !s.transform_color(record.rgba,color,error))return false;
        if(c.override_rgba) {
            const auto override=*c.override_rgba;
            color=(override&0x00ffffff)|((((color>>24)*(override>>24))/255)<<24);
        }
        for(std::size_t j=0;;++j) {
            if(i>=state.records.size())return required(false,"stable current record",error);
            if(j>=state.records[i].glyphs.size())break;
            const auto g=state.records[i].glyphs[j];
            const Matrix matrix=positioned(c.matrix,x,y,scale);
            if(g.index==-1 && !g.image) {
                static const float box[]={32,32,480,32,480,-656,32,-656,32,32};
                if(!required(bool(s.line),"line renderer",error)||!s.line(matrix,color,box,5,error))return false;
            } else {
                if(state.records[i].underline) {
                    if(i>=state.records.size() || j>=state.records[i].glyphs.size())
                        return required(false,"stable current glyph",error);
                    const float points[]={0,60,state.records[i].glyphs[j].advance/scale,60};
                    if(!required(bool(s.line),"line renderer",error)||!s.line(matrix,color,points,2,error))return false;
                }
                if(!g.image) {
                    if(g.index>=0 && (!required(bool(record.font),"live embedded font",error)||
                       !required(bool(s.shape),"embedded glyph renderer",error)||
                       !s.shape(matrix,record.font,g.index,c.pixel_scale,record.rgba,error)))return false;
                } else {
                    GlyphBinding binding;
                    if(!required(bool(s.bind_glyph),"owned glyph binding",error)||!s.bind_glyph(g,binding,error))return false;
                    if(!required(bool(binding.bitmap.owner),"owned bitmap",error))return false;
                    if(g.type==2) {
                        const Rect rect={0,g.x1,-g.y1,0},uv={0,1,0,1};
                        if(!required(bool(s.bitmap),"bitmap renderer",error)||!s.bitmap(matrix,binding.bitmap,rect,uv,color,error))return false;
                    } else if(g.x1!=0 && g.y1!=0) {
                        Rect rect={-g.x0,g.x1-g.x0,-g.y0,g.y1-g.y0},uv{};
                        const float factor=1024.0f/float(std::uint16_t(g.height));
                        float factor_x{},factor_y{};
                        if(!c.freetype_cache && !c.bitmap_cache) {
                            // This branch uses fractional bounds as direct UVs.
                            uv={0,g.x1,0,g.y1};
                            factor_x=float(binding.bitmap.width)*factor;
                            factor_y=float(binding.bitmap.height)*factor;
                        } else {
                            if(!required(c.freetype_cache,"live FreeType cache on cached draw",error))return false;
                            const bool ft=c.freetype_cache && binding.bitmap.owner==c.freetype_atlas;
                            if(!ft && !required(c.bitmap_cache,"live bitmap cache on non-FreeType atlas",error))return false;
                            if(!required(bool(s.region),"glyph atlas region",error)||
                               !s.region(ft,g,binding,{},uv,error))return false;
                            const float original_width=uv[1]-uv[0],original_height=uv[3]-uv[2];
                            factor_x=factor*original_width;factor_y=factor*original_height;
                            bool filtered=false;
                            auto filter=[&](Filter f,float ex,float ey) {
                                Rect offset{};
                                if(!s.region(true,g,binding,f,offset,error))return false;
                                uv={offset[0],original_width+offset[0],offset[2],original_height+offset[2]};
                                const float w=uv[1]-uv[0],h=uv[3]-uv[2];
                                rect[1]+=ex/w;rect[3]+=ey/h;
                                uv[1]-=(1.0f-g.x1)*w-ex;
                                uv[3]-=(1.0f-g.y1)*h-ey;
                                filtered=true;return true;
                            };
                            if(ft && (c.argument1||c.argument2)) {
                                if(!filter({0,c.argument1,c.argument2},expansion_x,expansion_y))return false;
                            } else if(ft && c.argument0) {
                                if(!filter({c.argument0,0,0},expansion_both,expansion_both))return false;
                            }
                            if(!filtered) {
                                uv[1]-=(1.0f-g.x1)*original_width;
                                uv[3]-=(1.0f-g.y1)*original_height;
                            }
                            uv[0]/=float(binding.bitmap.width);uv[2]/=float(binding.bitmap.height);
                            uv[1]/=float(binding.bitmap.width);uv[3]/=float(binding.bitmap.height);
                        }
                        if(record.font && record.font->define_font3){factor_x*=20.0f;factor_y*=20.0f;}
                        rect[0]*=factor_x;rect[1]*=factor_x;rect[2]*=factor_y;rect[3]*=factor_y;
                        if(!required(bool(s.bitmap),"bitmap renderer",error)||!s.bitmap(matrix,binding.bitmap,rect,uv,color,error))return false;
                    }
                }
            }
            // Source callbacks can change the advance; do not reuse g.advance.
            if(i>=state.records.size() || j>=state.records[i].glyphs.size())
                return required(false,"stable current glyph",error);
            x+=state.records[i].glyphs[j].advance;
        }
    }
    return true;
}
}
