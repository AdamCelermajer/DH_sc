#include "text_layout_v1.hpp"
#include <cmath>
#include <limits>
#include <utility>

namespace dh2::ui::text_v1 {
namespace {
// Force the source's separately rounded soft-float operations even in an
// optimized build. No fused arithmetic or implicit double intermediate.
float add(float a,float b){volatile float c=a+b;return c;}
float sub(float a,float b){volatile float c=a-b;return c;}
float mul(float a,float b){volatile float c=a*b;return c;}
float div(float a,float b){volatile float c=a/b;return c;}
float fi(std::int32_t a){volatile float c=static_cast<float>(a);return c;}
bool fail(std::string& e,const char* t){e=t;return false;}
bool integer(float a,std::int32_t& out,std::string& e){
    if(!std::isfinite(a)||a<-2147483648.0f||a>=2147483648.0f)
        return fail(e,"text layout: unsafe source float-to-integer domain");
    out=static_cast<std::int32_t>(a);return true;
}
std::int32_t wrap(std::uint32_t a){return a<=0x7fffffff?static_cast<std::int32_t>(a):static_cast<std::int32_t>(std::int64_t(a)-0x100000000LL);}
std::int32_t atoi32(const std::string& s){
    std::size_t i=0;while(i<s.size()&&(s[i]==' '||s[i]=='\t'||s[i]=='\n'||s[i]=='\r'||s[i]=='\v'||s[i]=='\f'))++i;
    bool neg=false;if(i<s.size()&&(s[i]=='+'||s[i]=='-'))neg=s[i++]=='-';
    std::uint32_t v=0;for(;i<s.size()&&s[i]>='0'&&s[i]<='9';++i)v=v*10u+unsigned(s[i]-'0');
    return wrap(neg?0u-v:v);
}
std::string fold(std::string s){for(auto& c:s)if(c>='A'&&c<='Z')c=char(c+32);return s;}
void set(Tag& t,std::string key,std::string value){key=fold(std::move(key));for(auto& p:t.entries)if(p.first==key){p.second=std::move(value);return;}t.entries.emplace_back(std::move(key),std::move(value));}
const std::string* get(const Tag& t,const char* key){for(auto& p:t.entries)if(p.first==key)return &p.second;return nullptr;}
void style(Record& r,const Attributes& a,float x,float y){r.font=a.font;r.rgba=a.rgba;r.underline=a.underline;r.height=fi(a.size);r.x=x;r.y=y;r.has_x=r.has_y=r.has_font=true;}
void raise_line(State& s,float old,float y){
    for(std::size_t n=s.records.size();n&&s.records[n-1].y==old;--n)s.records[n-1].y=y;
}
bool next_unicode(const std::string& s,std::size_t& i,std::int32_t& code,std::string& e){
    if(i>=s.size()||s[i]==0){code=0;return true;}
    auto b=static_cast<unsigned char>(s[i++]);if(b<128){code=b;return true;}
    unsigned n=0,v=0;if(b>=0xc2&&b<=0xdf){n=1;v=b&31;}else if(b>=0xe0&&b<=0xef){n=2;v=b&15;}else if(b>=0xf0&&b<=0xf4){n=3;v=b&7;}else return fail(e,"text layout: unsafe source UTF-8 input");
    if(i+n>s.size())return fail(e,"text layout: truncated UTF-8 input");
    for(unsigned k=0;k<n;++k){auto c=static_cast<unsigned char>(s[i++]);if((c&0xc0)!=0x80)return fail(e,"text layout: invalid UTF-8 input");v=(v<<6)|(c&63);}
    if((n==1&&v<128)||(n==2&&v<2048)||(n==3&&v<65536)||v>0x10ffff||(v>=0xd800&&v<=0xdfff))return fail(e,"text layout: invalid UTF-8 code point");
    code=static_cast<std::int32_t>(v);return true;
}
}
bool parse_tag(Tag& t,const std::string& s,bool& parsed,std::string& e){
    parsed=false;if(s.find('\0')!=std::string::npos)return fail(e,"text tag: embedded NUL");
    if(!s.empty()&&s[0]=='/')return true;
    auto end=s.find(' ');if(end==std::string::npos){end=s.find('/');if(end==std::string::npos)end=s.size();}
    set(t,"name",s.substr(0,end));std::size_t start=0;
    for(;;){auto eq=s.find('=',start);if(eq==std::string::npos){parsed=true;return true;}
        auto space=s.rfind(' ',eq);if(space==std::string::npos)return fail(e,"text tag: unsafe backward attribute scan");
        auto last=eq;while(last&&s[last-1]==' ')--last;
        // If whitespace precedes '=', the source walks through it, then back
        // through the key to its preceding separator.
        space=s.rfind(' ',last?last-1:0);
        if(space==std::string::npos||last<=space)return fail(e,"text tag: unsafe backward attribute scan");
        auto dq=s.find('"',eq),sq=s.find('\'',eq),q=std::min(dq,sq);
        if(q==std::string::npos)return true;
        auto close=s.find(s[q],q+1);if(close==std::string::npos)return true;
        set(t,s.substr(space+1,last-space-1),s.substr(q+1,close-q-1));start=close;
    }
}
bool align_line(State& s,std::int32_t alignment,std::int32_t first,float x,std::string& e){
    if(!alignment)return true;
    auto extra=sub(sub(sub(sub(s.rect_max,s.rect_min),s.right),x),80.0f);
    if(alignment==2)extra=mul(extra,0.5f);else if(alignment!=1)extra=0;
    if(first<0||std::size_t(first)>s.records.size())return fail(e,"text layout: invalid source line record");
    for(std::size_t i=first;i<s.records.size();++i)if(s.records[i].has_x)s.records[i].x=add(s.records[i].x,extra);
    s.xcursor=add(s.xcursor,extra);return true;
}
bool append_text(State& s,const std::string& text,Attributes& a,bool entities,const Services& v,std::string& e){
    if(!a.font)return fail(e,"text layout: required font owner missing");
    float root{},units{},height{};
    if(!v.root_scale_word)return fail(e,"text layout: required root scale producer missing");
    if(!v.root_scale_word(root,e))return false;
    auto size=fi(a.size),scale=div(size,mul(root,1024.0f));
    if(a.font->define_font3)scale=div(scale,20.0f);
    if(!v.units_per_em||!v.font_height)return fail(e,"text layout: required font metrics producer missing");
    if(!v.units_per_em(a.font,units,e)||!v.font_height(a.font,height,e))return false;
    auto line=mul(size,div(height,units));if(line==0)line=fi(a.size);
    auto baseline=add(add(fi(a.size),s.y),mul(sub(a.font->metric5c,a.font->metric58),scale));
    Record r;
    if(!s.records.empty()){
        // The original copies only the preceding style, not its glyph array.
        r=s.records.back();r.glyphs.clear();
        if(baseline>r.y){raise_line(s,r.y,baseline);r.y=baseline;}
    }else r.y=baseline;
    auto margin=add(s.left,s.indent);if(!(margin>0))margin=0;
    style(r,a,add(margin,s.x),r.y);
    auto startx=r.x,starty=r.y,x=startx,y=starty;
    auto leading=add(mul(a.font->metric5c,scale),s.leading);
    s.xcursor=x;s.ycursor=y;
    std::int32_t previous=-1,count=0;
    for(std::size_t pos=0;;){std::int32_t code{};if(!next_unicode(text,pos,code,e))return false;if(!code)break;
        float kern{};if(!v.kerning)return fail(e,"text layout: required kerning producer missing");
        if(!v.kerning(a.font,previous,code,kern,e))return false;x=add(x,mul(kern,scale));
        auto next_previous=code;
        if(code==10||code==13){
            if(!(previous==13&&code==10)){
                s.records.push_back(r);if(!align_line(s,s.alignment,s.first_line,x,e))return false;
                margin=add(s.left,s.indent);if(!(margin>0))margin=0;x=margin;y=add(y,add(line,leading));
                r.glyphs.clear();style(r,a,x,y);
                s.first_line=s.word_record=static_cast<std::int32_t>(s.records.size());s.word_glyph=-1;
            }
            previous=next_previous;continue;
        }
        if(code==8){if(!r.glyphs.empty()){x=sub(x,r.glyphs.back().advance);r.glyphs.back().advance=0;}previous=code;continue;}
        float multiplier=1;
        if(code==17||code==32){multiplier=code==17?0.0f:1.0f;s.word_record=static_cast<std::int32_t>(s.records.size());s.word_glyph=static_cast<std::int32_t>(r.glyphs.size());code=next_previous=32;}
        else if(code==160)code=next_previous=32;
        else if(code==38&&entities&&text.compare(pos,5,"nbsp;")==0){pos+=5;code=next_previous=32;}
        Glyph g;bool found{};std::int32_t pixels{};if(!integer(div(fi(a.size),20.0f),pixels,e))return false;
        if(!v.glyph)return fail(e,"text layout: required glyph producer missing");
        if(!v.glyph(a.font,static_cast<std::uint16_t>(code),pixels,g,found,e))return false;
        if(!found){
            if(!v.diagnostic_state)return fail(e,"text layout: required original diagnostic state missing");
            if(v.diagnostic_state->missing_glyphs<10){
                ++v.diagnostic_state->missing_glyphs;
                if(!v.missing_glyph)return fail(e,"text layout: required missing-glyph diagnostic producer missing");
                if(!v.missing_glyph(a.font,code,e))return false;
            }
        }
        g.advance=mul(add(g.advance,s.letter_spacing),mul(scale,multiplier));if(std::uint32_t(code)>0x1000)g.advance=mul(g.advance,1.1f);
        if(!integer(div(fi(a.size),20.0f),pixels,e))return false;
        g.height=static_cast<std::int16_t>(std::uint16_t(pixels));g.code=static_cast<std::uint16_t>(code);r.glyphs.push_back(g);
        auto newx=add(x,g.advance);
        if(sub(sub(sub(s.rect_max,s.rect_min),s.right),80.0f)<=newx){
            s.records.push_back(r);y=add(y,add(line,leading));r.glyphs.clear();x=s.left;style(r,a,x,y);
            auto index=static_cast<std::int32_t>(s.records.size())-1;auto& old=s.records.back();float width=newx;
            if(s.word_glyph!=-1){
                if(s.word_record<0||s.word_record>index)return fail(e,"text layout: invalid source word record");
                auto& word=s.records[s.word_record];
                if(s.word_glyph<0||std::size_t(s.word_glyph)>=word.glyphs.size())return fail(e,"text layout: invalid source word glyph");
                width=sub(newx,word.glyphs[s.word_glyph].advance);
                auto begin=s.word_record==index?std::size_t(s.word_glyph)+1:0;
                for(auto i=begin;i<old.glyphs.size();++i){r.glyphs.push_back(old.glyphs[i]);x=add(x,old.glyphs[i].advance);width=sub(width,old.glyphs[i].advance);}
                old.glyphs.resize(s.word_record==index?std::size_t(s.word_glyph):0);
            }else if(!old.glyphs.empty()){
                r.glyphs.push_back(old.glyphs.back());x=add(x,old.glyphs.back().advance);width=sub(newx,old.glyphs.back().advance);old.glyphs.pop_back();
            }
            if(!align_line(s,s.alignment,s.first_line,width,e))return false;
            s.first_line=s.word_record=static_cast<std::int32_t>(s.records.size());s.word_glyph=-1;
        }else x=newx;
        if(count<s.cursor){s.xcursor=x;s.ycursor=y;}++count;
        auto extent=add(mul(scale,a.font->metric58),y);
        // Source comparisons deliberately retain their unordered NaN behavior.
        if(!(x>s.bounds[0]))s.bounds[0]=x;if(!(extent>s.bounds[2]))s.bounds[2]=extent;
        if(x>s.bounds[1])s.bounds[1]=x;if(extent>s.bounds[3])s.bounds[3]=extent;
        previous=next_previous;
    }
    s.xcursor=add(s.xcursor,mul(scale,a.font->metric5c));
    s.ycursor=sub(s.ycursor,add(fi(a.size),mul(sub(a.font->metric5c,a.font->metric58),scale)));
    s.records.push_back(std::move(r));s.x=add(s.x,sub(x,startx));s.y=add(s.y,sub(y,starty));return true;
}
bool append_image(State& s,const std::string& src,std::int32_t w,std::int32_t h,const Services& v,std::string& e){
    if(!v.image)return fail(e,"text layout: required image producer missing");
    Image image;if(!v.image(src,w,h,image,e))return false;if(!image.native)return true;
    if(w<=0)w=image.width;if(h<=0)h=image.height;
    Glyph g;g.image=image.native;g.advance=g.x1=mul(fi(w),20.0f);g.y1=mul(fi(h),20.0f);g.height=1024;g.code=65535;g.type=2;
    auto baseline=add(g.y1,s.y);Record r;
    if(!s.records.empty()){r=s.records.back();r.glyphs.clear();if(baseline>r.y){raise_line(s,r.y,baseline);r.y=baseline;}}
    else r.y=baseline;
    auto margin=add(s.left,s.indent);if(!(margin>0))margin=0;
    r.x=add(s.x,margin);r.font.reset();r.rgba=0xffffffff;r.underline=false;r.height=1024;r.has_x=r.has_y=true;r.has_font=false;
    s.x=add(s.x,g.advance);r.glyphs.push_back(std::move(g));s.records.push_back(std::move(r));return true;
}
bool parse_html(State& s,const Services& v,std::string& e){
    if(s.text.empty())return true;
    Attributes initial;initial.font=s.font;initial.rgba=s.rgba;if(!integer(s.text_height,initial.size,e))return false;
    std::vector<Attributes> stack{initial};std::int32_t paragraphs=0;
    // Own the byte storage across synchronous callbacks, preserving the source
    // fixed starting text pointer; mutation of that storage is not fabricated.
    const auto text=s.text;
    for(std::size_t pos=0;pos<text.size();){
        if(text[pos]!='<'){auto next=text.find('<',pos);if(next==std::string::npos)next=text.size();auto chunk=text.substr(pos,next-pos);if(!append_text(s,chunk,stack.back(),true,v,e))return false;pos=next;continue;}
        auto end=text.find('>',pos);if(end==std::string::npos)break;
        if(pos+1>=text.size())break;
        auto token=text.substr(pos+1,end-pos-1);pos=end+1;
        if(!token.empty()&&token[0]=='/'){if(stack.size()>1)stack.pop_back();continue;}
        if(token.size()>=512)return fail(e,"text layout: unsafe original HTML tag buffer domain");
        Tag tag;bool parsed{};if(!parse_tag(tag,token,parsed,e))return false;
        auto name=get(tag,"name");if(!name)continue;Attributes a=stack.back();
        if(*name=="p"){if(paragraphs&& !append_text(s,"\n",stack.back(),true,v,e))return false;++paragraphs;}
        else if(*name=="font"||*name=="b"||*name=="i"){
            auto face=get(tag,"face");
            if(*name!="font"||face){if(!v.clone_font)return fail(e,"text layout: required font clone producer missing");std::shared_ptr<Font> clone;if(!v.clone_font(a.font,clone,e))return false;if(!clone||!clone->native)return fail(e,"text layout: font clone owner missing");a.font=std::move(clone);if(*name=="b")a.font->bold=true;else if(*name=="i")a.font->italic=true;else a.font->name=*face;}
            if(*name=="font"){
                if(auto color=get(tag,"color");color&&!color->empty()){
                    std::uint32_t rgb=0xff000000;
                    if((*color)[0]=='#'){
                        unsigned shift=0;for(auto i=color->size();i>1;){auto c=(*color)[--i];if(c>='A'&&c<='Z')c=char(c+32);unsigned n=c>='0'&&c<='9'?unsigned(c-'0'):c>='a'&&c<='f'?unsigned(c-'a'+10):0; // invalid digits consume their nibble position
                            if(shift<32)rgb|=n<<shift;shift+=4;}
                    }else rgb|=std::uint32_t(atoi32(*color));
                    a.rgba=0xff000000u|((rgb&0xffu)<<16)|(rgb&0xff00u)|((rgb>>16)&0xffu);
                }
                if(auto size=get(tag,"size")){
                    if(size->find('%')!=std::string::npos){auto t=*size;if(!t.empty())t.pop_back();a.size=wrap(std::uint32_t(a.size)*std::uint32_t(atoi32(t)))/100;}
                    else if(!size->empty()&&((*size)[0]=='+'||(*size)[0]=='-')){std::int32_t delta;if(!integer(mul(fi(atoi32(size->substr(1))),20.0f),delta,e))return false;a.size=wrap(std::uint32_t(a.size)+((*size)[0]=='+'?std::uint32_t(delta):0u-std::uint32_t(delta)));}
                    else {auto n=atoi32(*size);if(n>0&&!integer(mul(fi(n),20.0f),a.size,e))return false;}
                }
            }
            stack.push_back(std::move(a));
        }else if(*name=="u"){a.underline=true;stack.push_back(std::move(a));}
        else if(*name=="img"){auto src=get(tag,"src"),w=get(tag,"width"),h=get(tag,"height");if(!append_image(s,src?*src:"",w?atoi32(*w):0,h?atoi32(*h):0,v,e))return false;}
    }
    return true;
}
bool format_text(State& s,bool html,const Services& v,std::string& e){
    s.cached_rect.fill(0xffffffff);s.records.clear();s.x=s.y=0;s.first_line=s.word_record=0;s.word_glyph=-1;s.bounds.fill(0);
    if(!s.font)return true;
    if(html){if(!parse_html(s,v,e))return false;}
    else {Attributes a;a.font=s.font;a.rgba=s.rgba;if(!integer(s.text_height,a.size,e)||!append_text(s,s.text,a,false,v,e))return false;}
    if(!align_line(s,s.alignment,s.first_line,s.x,e))return false;
    if(!s.multiline&&s.records.size()>1){
        float last=0;for(auto& r:s.records)if(r.has_y&&r.y>last)last=r.y;
        auto shift=add(mul(last,-0.5f),add(mul(s.records.front().height,-0.5f),s.records.front().y));
        for(auto& r:s.records)if(r.has_y)r.y=add(r.y,shift);
    }
    if(!v.preload_enabled)return fail(e,"text layout: required root preload producer missing");
    bool enabled{};if(!v.preload_enabled(enabled,e))return false;if(enabled){if(!v.preload)return fail(e,"text layout: required glyph preload producer missing");if(!v.preload(s,e))return false;}return true;
}
}
