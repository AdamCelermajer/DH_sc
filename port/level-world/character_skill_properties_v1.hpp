#pragma once
#include "../game-data/properties.hpp"
#include "../script-runtime/script_runtime.h"
namespace dh2::character::skills {
// Borrowed genuine owner and shared CharProperties::s_temp. No synthesized
// default skill sheet. Tables/rules/owner/providers outlive VM finalizers.
struct SkillPropertyBindingsV1 {
 data::PropertyView* owner{};
 data::PropertySheet* temporary{};
 const data::ClassTables* classes{};
 void* context{};
 int(*external)(void*,std::uintptr_t,std::int32_t** sheet){};
 // Mandatory original RecalcProperties(true) for external-sheet mutations.
 int(*recalculate)(void*,data::PropertyView*){};
 // Mandatory uncached mode0 class application to owner.resolved. This is not
 // apply_class_to_base; original source may resolve linear properties here.
 int(*normal_class)(void*,data::PropertyView*,std::int32_t){};
};
int skill_clear_properties_v1(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
int skill_set_property_v1(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
int skill_apply_class_v1(void*,const dh2_script_value*,std::uint32_t,dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
int skill_property_bind_v1(dh2_script_vm*,SkillPropertyBindingsV1*);
}
