#include "native_batch_material_values_v113.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::world {namespace {
struct Reader {
 const resources::BresView& v;
 const std::uint8_t* at(std::uint64_t p,std::uint64_t n)const{if(!v.bytes||p>v.size||n>v.size-p)throw std::runtime_error("Actual batch material parameter range");return v.bytes+p;}
 std::uint32_t word(std::uint64_t p)const{auto b=at(p,4);return b[0]|(std::uint32_t(b[1])<<8)|(std::uint32_t(b[2])<<16)|(std::uint32_t(b[3])<<24);}
 std::string text(std::uint32_t p)const{if(!p)throw std::runtime_error("Actual batch material parameter name absent");auto b=at(p,1);auto e=static_cast<const std::uint8_t*>(std::memchr(b,0,v.size-p));if(!e)throw std::runtime_error("Actual batch material parameter name unterminated");return {reinterpret_cast<const char*>(b),reinterpret_cast<const char*>(e)};}
};
unsigned width(std::uint32_t type){switch(type){case 0:case 1:case 4:case 15:return 4;case 2:case 5:return 8;case 3:case 6:return 12;case 7:case 16:return 16;case 8:return 16;case 9:return 36;case 10:return 64;default:return 0;}}
void decode_value(const Reader& r,std::uint64_t q,bool definition,NativeBatchMaterialValueV113& value){
 value.source_type=r.word(q+(definition?4:8));value.semantic=definition?r.word(q+8):0;
 const auto arrays=r.word(q+12),counts=r.word(q+16),data=r.word(q+20);r.at(counts,std::uint64_t(arrays)*4);r.at(data,std::uint64_t(arrays)*4);const auto scalar_width=width(value.source_type);
 if(value.source_type==17||value.source_type==19){if(arrays!=1)throw std::runtime_error("Actual source light URL array shape unsupported");const auto n=r.word(counts);value.element_counts.push_back(n);r.at(data,std::uint64_t(n)*4);for(unsigned i=0;i<n;++i){const auto p=r.word(data+4*i);value.light_urls.push_back(p?r.text(p):std::string{});}return;}
 for(unsigned a=0;a<arrays;++a){const auto n=r.word(counts+4*a);value.element_counts.push_back(n);const auto payload=arrays==1&&scalar_width?data:r.word(data+4*a);
  if(scalar_width){auto bytes=r.at(payload,std::uint64_t(n)*scalar_width);value.bytes.insert(value.bytes.end(),bytes,bytes+std::uint64_t(n)*scalar_width);}
  else if(value.source_type==11){r.at(payload,std::uint64_t(n)*4);for(unsigned k=0;k<n;++k){const auto index=r.word(payload+4*k);if(index==UINT32_MAX){value.images.emplace_back();continue;}auto item=dh2_bres_library_item(&r.v,resources::Library::image,index);if(!item)throw std::runtime_error("Actual material image binding absent");auto path=r.text(r.word(item-r.v.bytes+8));const auto slash=path.find_last_of("/\\");value.images.push_back(path.substr(slash==std::string::npos?0:slash+1));}}
 }
}
}
bool decode_batch_material_values_v113(const resources::BresView& image,const std::string& material,NativeBatchMaterialValuesV113& out,std::string& e)try{
 Reader r{image};unsigned matches{};NativeBatchMaterialValuesV113 result;
 for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::material);++i){auto row=dh2_bres_library_item(&image,resources::Library::material,i);if(!row)throw std::runtime_error("Actual material row absent");const auto p=row-image.bytes;if(r.text(r.word(p))!=material)continue;++matches;
  const auto count=r.word(p+16),base=r.word(p+20);r.at(base,std::uint64_t(count)*24);
  for(unsigned j=0;j<count;++j){const auto q=base+24*j;auto name=r.text(r.word(q));NativeBatchMaterialValueV113 value;value.source_type=r.word(q+8);
   decode_value(r,q,false,value);
   if(!result.emplace(std::move(name),std::move(value)).second)throw std::runtime_error("Duplicate actual material parameter binding");
  }
 }
 if(matches!=1)throw std::runtime_error("Actual material uniform source not uniquely selected");out=std::move(result);e.clear();return true;
}catch(const std::exception& failure){e=failure.what();return false;}
bool decode_batch_effect_values_v113(const resources::BresView& image,const std::string& uri,const std::string& technique,NativeBatchMaterialValuesV113& out,std::string& e)try{
 if(uri.empty()||uri[0]!='#')throw std::runtime_error("Actual effect fragment absent");Reader r{image};unsigned matches{};
 for(unsigned i=0;i<dh2_bres_library_count(&image,resources::Library::effect);++i){auto row=dh2_bres_library_item(&image,resources::Library::effect,i);if(!row)throw std::runtime_error("Actual effect definition absent");auto p=row-image.bytes;if(r.text(r.word(p))!=uri.substr(1))continue;
  // SProfileGLES2 definitions are the separate +40/+44 table. The
  // +16/+20 COMMON profile has different parameter order (skybox sampler
  // is COMMON row0 but GLES2 row5), so it cannot resolve SPass bindings.
  std::vector<std::string> names;const auto count=r.word(p+40),base=r.word(p+44);r.at(base,std::uint64_t(count)*24);
  for(unsigned j=0;j<count;++j){auto q=base+24*j;auto name=r.text(r.word(q));names.push_back(name);NativeBatchMaterialValueV113 v;decode_value(r,q,true,v);if(!out.effect_defaults.emplace(name,std::move(v)).second)throw std::runtime_error("Duplicate actual effect default");}
  const auto techniques=r.word(p+32),table=r.word(p+36);r.at(table,std::uint64_t(techniques)*12);
  for(unsigned j=0;j<techniques;++j){auto t=table+12*j;if(technique.empty()?j!=0:r.text(r.word(t))!=technique)continue;++matches;if(r.word(t+4)!=1)throw std::runtime_error("Selected source multipass uniform binding required");auto pass=r.word(t+8);r.at(pass,116);const auto samplers=r.word(pass+108),bindings=r.word(pass+112);r.at(bindings,std::uint64_t(samplers)*12);
   for(unsigned k=0;k<samplers;++k){const auto q=bindings+12*k,scope=r.word(q+4),index=r.word(q+8);if(scope!=0)throw std::runtime_error("Actual nonlocal GLES2 parameter binding producer required");if(index>=names.size())throw std::runtime_error("Actual sampler parameter index outside GLES2 effect");record_batch_sampler_binding_v113(out,r.text(r.word(q)),names[index]);}
  }
 }
 if(matches!=1)throw std::runtime_error("Actual effect uniform technique not uniquely resolved");e.clear();return true;
}catch(const std::exception& failure){e=failure.what();return false;}
}
