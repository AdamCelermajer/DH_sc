#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
namespace dh2::world {
// Context forwarding only; derived owners retain the actual mutable fields.
template<class T> CanonicalPropertyActorV1 canonical_family_fields_v15(T& s){
 auto a=s.base().properties();a.fields.context=&s;
 a.fields.read_bool=[](void* p,std::uint32_t o,std::uint8_t& v,std::string& e){return static_cast<T*>(p)->read_bool(o,v,e);};
 a.fields.write_bool=[](void* p,std::uint32_t o,std::uint8_t v,std::string& e){return static_cast<T*>(p)->write_bool(o,v,e);};
 a.fields.write_int=[](void* p,std::uint32_t o,std::int32_t v,std::string& e){return static_cast<T*>(p)->write_int(o,v,e);};
 a.fields.write_string=[](void* p,std::uint32_t o,const std::string& v,std::string& e){return static_cast<T*>(p)->write_string(o,v,e);};
 a.fields.write_float=[](void* p,std::uint32_t o,float v,std::string& e){auto x=static_cast<T*>(p)->base().properties().fields;return x.write_float(x.context,o,v,e);};
 a.fields.write_vector3=[](void* p,std::uint32_t o,const std::array<float,3>& v,std::string& e){auto x=static_cast<T*>(p)->base().properties().fields;return x.write_vector3(x.context,o,v,e);};
 a.fields.write_point2=[](void* p,std::uint32_t o,const std::array<std::int32_t,2>& v,std::string& e){auto x=static_cast<T*>(p)->base().properties().fields;return x.write_point2(x.context,o,v,e);};return a;
}
}
