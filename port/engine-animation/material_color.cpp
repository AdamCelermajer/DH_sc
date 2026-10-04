#include "material_color.hpp"
#include <cstddef>
#include <cstring>
namespace {
bool overlaps(const void* a,std::size_t an,const void* b,std::size_t bn){
    const auto aa=reinterpret_cast<std::uintptr_t>(a),bb=reinterpret_cast<std::uintptr_t>(b);
    return an&&bn&&(aa<=bb?bb-aa<an:aa-bb<bn);
}
bool valid_span(const void* p,std::size_t n){return p&&n<=UINTPTR_MAX-reinterpret_cast<std::uintptr_t>(p);}
std::uint32_t source_unsigned(float f){
    std::uint32_t w;std::memcpy(&w,&f,4);const auto e=(w>>23)&255;
    if((w>>31)||e<127)return 0;
    if(e>158)return e==255&&(w&0x7fffff)?0:UINT32_MAX;
    const auto m=(w&0x7fffff)|0x800000;
    return e>=150?m<<(e-150):m>>(150-e);
}
int validate(std::uint8_t* out,const dh2::animation::ColorAccessor24* a,std::uint32_t k,std::uint32_t n,std::uint32_t r,bool interpolate){
    if(!valid_span(out,4)||!a||reinterpret_cast<std::uintptr_t>(a)%alignof(dh2::animation::ColorAccessor24)||!valid_span(a,24)||overlaps(out,4,a,24))return -1;
    if(a->has_default>1||!a->count||!valid_span(a->values,a->count)||k>=a->count||n>=a->count||r>=a->count||overlaps(out,4,a->values,a->count))return -1;
    if(a->default_value&&(!valid_span(a->default_value,4)||overlaps(out,4,a->default_value,4)))return -1;
    if(interpolate&&a->has_default&&!a->default_value)return -2;
    return 0;
}
int sample(std::uint8_t* out,const dh2::animation::ColorAccessor24* a,std::uint32_t k,std::uint32_t n,std::uint32_t r,float fraction,int mode){
    const int status=validate(out,a,k,n,r,mode!=0);if(status)return status;
    std::uint8_t result[4];std::memcpy(result,out,4);
    const bool use_default=a->has_default&&(mode!=0||a->default_value);
    if(use_default)std::memcpy(result,a->default_value,3);
    std::uint8_t value=a->values[k];
    if(mode){
        const int first=mode==2?std::uint8_t(a->values[k]-a->values[r]):int(a->values[k]);
        const int last=mode==2?std::uint8_t(a->values[n]-a->values[r]):int(a->values[n]);
        volatile float product=fraction*float(last-first);
        volatile float sum=float(first)+product;
        value=std::uint8_t(source_unsigned(sum));
    }
    result[use_default?3:0]=value;std::memcpy(out,result,4);return 0;
}
}
extern "C" int dh2_material_alpha_key(std::uint8_t* out,const dh2::animation::ColorAccessor24* a,std::uint32_t key){return sample(out,a,key,key,key,0,0);}
extern "C" int dh2_material_alpha_between(std::uint8_t* out,const dh2::animation::ColorAccessor24* a,std::uint32_t key,std::uint32_t next,float fraction){return sample(out,a,key,next,key,fraction,1);}
extern "C" int dh2_material_alpha_delta(std::uint8_t* out,const dh2::animation::ColorAccessor24* a,std::uint32_t reference,std::uint32_t key,std::uint32_t next,float fraction){return sample(out,a,key,next,reference,fraction,2);}
extern "C" int dh2_material_color_blend(std::uint8_t* out,const std::uint8_t* values,const float* weights,std::int32_t count){
    if(!valid_span(out,4)||count<0)return -1;
    const auto size=std::size_t(count)*4;
    if(count&&(!valid_span(values,size)||!valid_span(weights,size)||reinterpret_cast<std::uintptr_t>(weights)%alignof(float)||overlaps(out,4,values,size)||overlaps(out,4,weights,size)))return -1;
    std::uint8_t result[4]{};
    if(count==1)std::memcpy(result,values,4);
    else for(unsigned j=0;j<4;++j){
        float sum=0;
        for(std::int32_t i=0;i<count;++i){volatile float product=float(values[std::size_t(i)*4+j])*weights[i];volatile float next=sum+product;sum=next;}
        result[j]=std::uint8_t(source_unsigned(sum));
    }
    std::memcpy(out,result,4);return 0;
}
extern "C" int dh2_material_color_set(dh2::animation::ColorParameter32* p,std::uint32_t element,const std::uint8_t* color){
    if(!p||reinterpret_cast<std::uintptr_t>(p)%alignof(dh2::animation::ColorParameter32)||!valid_span(p,32)||!valid_span(color,4)||overlaps(p,32,color,4))return -1;
    if(p->type!=8&&p->type!=16)return -2;
    if(element>=p->element_count)return 0;
    bool same=true;
    if(p->type==16){std::uint32_t packed;std::memcpy(&packed,color,4);same=p->words[0]==packed;p->words[0]=packed;}
    else {
        std::uint32_t factor_word=0x3b808081;float factor;std::memcpy(&factor,&factor_word,4);
        std::uint32_t converted[4];
        for(unsigned j=0;j<4;++j){volatile float v=float(color[j])*factor;float previous;std::memcpy(&previous,&p->words[j],4);same=same&&(previous==v);float value=v;std::memcpy(&converted[j],&value,4);}
        std::memcpy(p->words,converted,16);
    }
    if(!same)p->stamp_c=p->stamp_10=-1;
    return 1;
}
