#include "swf_font_resolver.hpp"

namespace {
using namespace dh2::ui;
constexpr std::size_t budget=4096;
std::size_t length(const char* s) noexcept {
    if(!s)return budget;
    for(std::size_t i=0;i<budget;++i)if(!s[i])return i;
    return budget;
}
bool equal(const char* a,const char* b) noexcept {
    for(std::size_t i=0;i<budget;++i){if(a[i]!=b[i])return false;if(!a[i])return true;}
    return false;
}
bool append(char* out,std::size_t& n,const char* s,std::size_t cap) noexcept {
    const auto k=length(s);if(k>=budget||n>=cap||k>=cap-n)return false;
    for(std::size_t i=0;i<k;++i)out[n+i]=s[i];n+=k;out[n]=0;return true;
}
int service(const FontResolveServices16& s,FontResolveService k,const char* text,
    char* buffer,std::size_t capacity,std::uintptr_t& value) noexcept {
    FontResolveRequest40 r{k,0,text,buffer,capacity,value};
    if(s.invoke(s.context,&r)!=0)return -2;
    value=r.value;return 0;
}
}
extern "C" int dh2_swf_font_resolve(dh2::ui::FontResolveOutput32* out,
    const dh2::ui::FontResolveInput24* in,const dh2::ui::FontResolveServices16* services) noexcept {
    using namespace dh2::ui;
    if(!out||!in||!services||!services->invoke||!out->bytes||!out->capacity||
        out->reserved||in->bold>1||in->italic>1||length(in->name)>=budget||
        length(in->base_path)>=budget)return -1;
    const auto source=*in;const auto s=*services;
    std::uintptr_t value=0;
    if(service(s,FontResolveService::debug_load,nullptr,nullptr,0,value))return -2;
    if(service(s,FontResolveService::debug_get_switch,"isTracingMenuFS",nullptr,0,value))return -2;
    char raw[budget]{},rewritten[budget]{};std::size_t n=0;
    bool direct=false,symbol=false;
    const char* file=nullptr;
    if(equal(source.name,"_sans")||equal(source.name,"_serif")){
        file="#/system/fonts/DroidSans.ttf";direct=true;
    }else if(equal(source.name,"_typewriter")){
        file=source.bold?(source.italic?"#/system/DroidSerif-BoldItalic.ttf":"#/system/DroidSans-Bold.ttf"):
            (source.italic?"#/system/DroidSerif-Italic.ttf":"#/system/DroidSans.ttf");direct=true;
    }else if(equal(source.name,"Symbol")){
        file="data/menus/SCT_Font_3.fnt";symbol=true;
    }else if(equal(source.name,"Arial")){
        file="data/wqy-zenhei.ttf";
    }else{
        value=0;if(service(s,FontResolveService::language,nullptr,nullptr,0,value))return -2;
        if(value==4)file="data/japanese.ttf";
        else if(value==5)file="data/nanumgothic.ttf";
        else if(value==6)file="data/wqy-zenhei.ttf";
    }
    if(!direct&&!append(raw,n,source.base_path,budget))return -1;
    if(file){if(!append(raw,n,file,budget))return -1;}
    else if(!append(raw,n,"data/",budget)||!append(raw,n,source.name,budget)||!append(raw,n,".ttf",budget))return -1;
    const char* result=raw;std::uint32_t found=1;
    if(!direct){
        value=0;
        if(service(s,FontResolveService::rewrite_path,raw,rewritten,budget,value))return -2;
        if(length(rewritten)>=budget)return -2;
        result=rewritten;
        if(!symbol){
            value=0;if(service(s,FontResolveService::open_read,result,nullptr,0,value))return -2;
            found=value!=0;
            if(found&&service(s,FontResolveService::close_read,nullptr,nullptr,0,value))return -2;
        }
    }
    const auto size=length(result);if(size>=out->capacity)return -1;
    for(std::size_t i=0;i<=size;++i)out->bytes[i]=result[i];
    out->length=size;out->found=found;return 0;
}
