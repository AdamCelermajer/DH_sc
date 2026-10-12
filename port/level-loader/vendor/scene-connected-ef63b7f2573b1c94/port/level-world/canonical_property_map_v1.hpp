#pragma once
#include "canonical_object_factory_v1.hpp"
#include <map>
#include <variant>
namespace dh2::world {
struct CanonicalPropertyFieldServicesV1 {
 void* context{};
 bool(*read_bool)(void*,std::uint32_t,std::uint8_t&,std::string&){};
 bool(*write_bool)(void*,std::uint32_t,std::uint8_t,std::string&){};
 bool(*write_int)(void*,std::uint32_t,std::int32_t,std::string&){};
 bool(*write_float)(void*,std::uint32_t,float,std::string&){};
 bool(*write_string)(void*,std::uint32_t,const std::string&,std::string&){};
 bool(*write_vector3)(void*,std::uint32_t,const std::array<float,3>&,std::string&){};
 bool(*write_point2)(void*,std::uint32_t,const std::array<std::int32_t,2>&,std::string&){};
};
struct CanonicalPropertyActorV1 {
 const char* class_name{}; // actual ObjectBase+20 catalog name producer
 std::string* template_name{}; // SAME PropertyMap+4 / ObjectBase+8 field
 CanonicalPropertyFieldServicesV1 fields;
};
using CanonicalPropertyValueV1=std::variant<std::uint8_t,std::int32_t,float,std::string,std::array<float,3>,std::array<std::int32_t,2>>;
struct CanonicalPropertyDescriptorV1 {std::uint32_t source_offset{};CanonicalPropertyValueV1 default_value;};
struct CanonicalPropertySourceServicesV1 {
 void* context{};
 // GameObject::DeclareProperties reads genuine shared Point3D<float>::ZERO.
 // Explicit producer required; no guessed global constant is substituted.
 const std::array<float,3>* position_rotation_default{};
 // Whole source LoadTemplate51419c is assertion(9), not a template XML loader.
 bool(*load_template_assertion)(void*,std::string&){};
};
// One source shared class/template descriptor tree; no actor field storage.
// Supports exact recovered Character/Player/OpenableContainer/AnimatedDecor
// declarations. Other classes require their genuine DeclareProperties first.
class CanonicalPropertyMapV1 {
 using Descriptors=std::map<std::string,CanonicalPropertyDescriptorV1>;
 std::map<std::string,std::map<std::string,Descriptors>> classes_;
 std::map<std::string,std::string> incomplete_;
 CanonicalPropertySourceServicesV1 source_;
 bool write(CanonicalPropertyActorV1&,const CanonicalPropertyDescriptorV1&,const CanonicalPropertyValueV1&,std::string&);
 Descriptors* current(CanonicalPropertyActorV1&,std::string&);
public:
 explicit CanonicalPropertyMapV1(CanonicalPropertySourceServicesV1 source):source_(source){}
 bool init_properties(CanonicalPropertyActorV1&,std::string&);
 bool set_template(CanonicalPropertyActorV1&,const char*,std::string&);
 bool load_defaults(CanonicalPropertyActorV1&,std::string&);
 bool load_overrides(CanonicalPropertyActorV1&,const CanonicalSourceObjectRequestV1&,std::string&);
 bool set_property(CanonicalPropertyActorV1&,const char*,const char*,std::string&);
 bool set_template_parameter(CanonicalPropertyActorV1&,const char*,const char*,std::string&);
 std::size_t class_count()const noexcept{return classes_.size();}
};
}
