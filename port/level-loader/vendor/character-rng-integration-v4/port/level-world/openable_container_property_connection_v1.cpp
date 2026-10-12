#include "openable_container_property_connection_v1.hpp"
namespace dh2::world {
namespace {
template<class F,class... A> bool base(F f,void* context,std::string& e,A&&... a){
    if(!f){e="Required SAME GameObject inherited property storage";return false;}
    return f(context,std::forward<A>(a)...,e);
}
}
bool OpenableContainerPropertyConnectionV1::read_bool(void* c,std::uint32_t o,std::uint8_t& v,std::string& e){
 auto& s=*static_cast<OpenableContainerPropertyConnectionV1*>(c);
 if(o==0x390){v=s.fields_.death_reset;return true;}if(o==0x70c){v=s.fields_.key_consume;return true;}
 return base(s.base_.read_bool,s.base_.context,e,o,v);
}
bool OpenableContainerPropertyConnectionV1::write_bool(void* c,std::uint32_t o,std::uint8_t v,std::string& e){
 auto& s=*static_cast<OpenableContainerPropertyConnectionV1*>(c);
 if(o==0x390){s.fields_.death_reset=v!=0;return true;}if(o==0x70c){s.fields_.key_consume=v!=0;return true;}
 return base(s.base_.write_bool,s.base_.context,e,o,v);
}
bool OpenableContainerPropertyConnectionV1::write_int(void* c,std::uint32_t o,std::int32_t v,std::string& e){
 auto& s=*static_cast<OpenableContainerPropertyConnectionV1*>(c);
 if(o==0x374){s.fields_.data374=v;return true;}if(o==0x708){s.fields_.key_qty=v;return true;}
 return base(s.base_.write_int,s.base_.context,e,o,v);
}
bool OpenableContainerPropertyConnectionV1::write_string(void* c,std::uint32_t o,const std::string& v,std::string& e){
 auto& s=*static_cast<OpenableContainerPropertyConnectionV1*>(c);
 if(o==0x378){s.fields_.data_desc=v;return true;}if(o==0x6f0){s.fields_.key_name=v;return true;}
 return base(s.base_.write_string,s.base_.context,e,o,v);
}
bool OpenableContainerPropertyConnectionV1::write_float(void* c,std::uint32_t o,float v,std::string& e){auto& s=*static_cast<OpenableContainerPropertyConnectionV1*>(c);return base(s.base_.write_float,s.base_.context,e,o,v);}
bool OpenableContainerPropertyConnectionV1::write_vector(void* c,std::uint32_t o,const std::array<float,3>& v,std::string& e){auto& s=*static_cast<OpenableContainerPropertyConnectionV1*>(c);return base(s.base_.write_vector3,s.base_.context,e,o,v);}
bool OpenableContainerPropertyConnectionV1::write_point(void* c,std::uint32_t o,const std::array<std::int32_t,2>& v,std::string& e){auto& s=*static_cast<OpenableContainerPropertyConnectionV1*>(c);return base(s.base_.write_point2,s.base_.context,e,o,v);}
CanonicalPropertyFieldServicesV1 OpenableContainerPropertyConnectionV1::services(){return {this,read_bool,write_bool,write_int,write_float,write_string,write_vector,write_point};}
}
