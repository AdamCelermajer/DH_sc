#include "material_compare_v4.hpp"
#include <cstring>
namespace dh2::scene {
namespace {
int missing(std::string& e,const char* text){e=text;return -2;}
const MaterialParameterCompareV4* parameter(const MaterialCompareViewV4& v,std::uint16_t i){return i<v.parameters.size()?&v.parameters[i]:nullptr;}
bool bytes_valid(const MaterialParameterCompareV4& p,std::size_t n){return (!n||p.bytes)&&n<=p.byte_size;}
const std::uint8_t* matrix(const MaterialCompareViewV4& v,const MaterialParameterCompareV4& p,unsigned i){return i<p.matrices.size()?(p.matrices[i]?p.matrices[i]:v.source_identity_matrix68):nullptr;}
bool matrix_equal(const std::uint8_t* a,const std::uint8_t* b){if(a[64]&&b[64])return true;for(unsigned i=0;i<16;++i){float x,y;std::memcpy(&x,a+4*i,4);std::memcpy(&y,b+4*i,4);if(!(x==y))return false;}return true;}
int compare_parameters(bool& out,const MaterialCompareViewV4& a,const MaterialCompareViewV4& b,bool equal,std::string& e){
 if(a.passes.size()!=b.passes.size())return missing(e,"Required matching material pass count");
 for(unsigned j=0;j<a.passes.size();++j){const auto& ap=a.passes[j];const auto& bp=b.passes[j];
  if(!equal&&ap.shader_sort_id!=bp.shader_sort_id){out=ap.shader_sort_id<bp.shader_sort_id;return 0;}
  if(bp.active_indices.size()<ap.active_indices.size())return missing(e,"Required same reflected active parameter domain");int compared=0;
  for(unsigned k=0;k<ap.active_indices.size();++k){auto ai=ap.active_indices[k],bi=bp.active_indices[k];if((ai|bi)&0x8000)continue;
   const auto* x=parameter(a,ai);const auto* y=parameter(b,bi);if(!x||!y)return missing(e,"Required live material parameter directory");
   if(equal&&x->type!=y->type){out=false;return 0;}
   if(x->semantic==2){if(x->textures.size()<x->count||y->textures.size()<x->count)return missing(e,"Required same texture parameter identities");
    if(equal){for(unsigned i=0;i<x->count;++i)if(x->textures[i]!=y->textures[i]){out=false;return 0;}}
    else for(unsigned i=0;i<x->count;++i){if(x->textures[i]<y->textures[i]){out=true;return 0;}if(x->textures[i]>y->textures[i]){out=false;return 0;}}
   }else if(x->type==11){if(x->matrices.size()<x->count||y->matrices.size()<x->count)return missing(e,"Required same matrix parameter storage");
    if(equal){for(unsigned i=0;i<x->count;++i){auto xm=matrix(a,*x,i),ym=matrix(b,*y,i);if(!xm||!ym)return missing(e,"Required actual source identity matrix");if(!matrix_equal(xm,ym)){out=false;return 0;}}}
    else if(!compared)for(unsigned i=0;i<x->count;++i){auto xm=matrix(a,*x,i),ym=matrix(b,*y,i);if(!xm||!ym)return missing(e,"Required actual source identity matrix");compared=std::memcmp(xm,ym,68);}
   }else{const std::size_t n=std::size_t(x->count)*x->element_width;if(!x->element_width||!bytes_valid(*x,n)||!bytes_valid(*y,n))return missing(e,"Required typed runtime parameter byte span");
    if(equal){if(std::memcmp(x->bytes,y->bytes,n)){out=false;return 0;}}
    else if(!compared)compared=std::memcmp(x->bytes,y->bytes,n);
   }
  }
  if(!equal){const int kind=int(ap.state[0])-int(bp.state[0]);if(kind){out=kind<0;return 0;}if(compared){out=compared<0;return 0;}}
 }out=equal;return 0;
}
}
int material_equal_v4(bool& out,const MaterialCompareViewV4& a,const MaterialCompareViewV4& b,std::string& e){
 if(!a.hash_ready||!b.hash_ready)return missing(e,"Required original current material hash owner");if(a.source_hash!=b.source_hash){out=false;return 0;}if(a.passes.size()!=b.passes.size()){out=false;return 0;}
 for(unsigned i=0;i<a.passes.size();++i){if(a.passes[i].shader_identity!=b.passes[i].shader_identity||std::memcmp(a.passes[i].state.data(),b.passes[i].state.data(),32)){out=false;return 0;}}
 bool result=false;int rc=compare_parameters(result,a,b,true,e);if(!rc)out=result;return rc;
}
int material_less_v4(bool& out,const MaterialCompareViewV4& a,const MaterialCompareViewV4& b,std::string& e){
 if(!a.hash_ready||!b.hash_ready)return missing(e,"Required original current material hash owner");if(a.source_hash!=b.source_hash){out=a.source_hash<b.source_hash;return 0;}if(a.passes.size()!=b.passes.size()){
  // 35388c's count comparison supplies the flags consumed by353858.
  out=a.passes.size()<b.passes.size();return 0;}
 bool result=false;int rc=compare_parameters(result,a,b,false,e);if(!rc)out=result;return rc;
}
int material_parameter_hash_v4(std::uint32_t& out,const MaterialCompareViewV4& v,std::string& e){
 if(v.passes.size()!=1)return missing(e,"Required single-pass parameter hash domain");std::uint32_t ordinary=0,texture=0;
 const auto& p=v.passes.front();for(auto i:p.active_indices){if(i&0x8000)continue;auto x=parameter(v,i);if(!x)return missing(e,"Required live material hash directory");if(x->semantic==11||x->semantic==15)continue;
  if(x->semantic==2){if(x->textures.size()<x->count)return missing(e,"Required same retained texture identities");
   // Native ABI reconstruction: hash sizeof(ITexture*) bytes without an
   // ARM32 address surrogate or truncation. Polynomial source5c5ef0 unchanged.
   const auto* bytes=reinterpret_cast<const std::uint8_t*>(x->textures.data());
   for(std::size_t k=0;k<std::size_t(x->count)*sizeof(std::uintptr_t);++k)texture=texture*13+bytes[k];
   continue;}
  if(x->type==11){for(unsigned k=0;k<x->count;++k){auto m=matrix(v,*x,k);if(!m)return missing(e,"Required source matrix hash bytes");for(unsigned n=0;n<68;++n)ordinary=ordinary*13+m[n];}}
  else{const auto n=std::size_t(x->count)*x->element_width;if(!x->element_width||!bytes_valid(*x,n))return missing(e,"Required typed material hash bytes");for(std::size_t k=0;k<n;++k)ordinary=ordinary*13+x->bytes[k];}
}out=(v.source_hash&~0x00fff0ffu)|(ordinary&255u)|((texture&4095u)<<12);return 0;
}
int material_refresh_hash_v4(MaterialCompareViewV4& v,std::string& e){
 if(v.passes.size()!=1)return missing(e,"Required supported single-pass material hash owner");
 const auto& p=v.passes.front();const auto id=p.shader_sort_id;
 std::uint32_t hash=(std::uint32_t((id&255)^(id>>8))<<24)|(std::uint32_t(p.state[0]&15)<<8);
 auto projected=v;projected.source_hash=hash;
 int rc=material_parameter_hash_v4(hash,projected,e);if(rc)return rc;
 v.source_hash=hash;v.hash_ready=true;return 0;
}
}
