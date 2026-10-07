#pragma once
#include "menu_stack_v1.hpp"
#include <functional>
#include <memory>
#include <string>
namespace dh2::ui {
struct CharacterMenuCallV1;
struct MenuRolloverValueV1 {std::uintptr_t identity;std::uint32_t reserved[2];};
struct MenuRolloverCallV1 {const MenuRolloverValueV1* arguments;std::uint32_t count,reserved;};
// These are the SAME mutable RenderFX projections used by menu-stack callers.
// The source array is selected fresh after all argument conversions.
struct MenuRolloverBindingsV1 {MenuStackRenderV1* renders[4];};
struct MenuRolloverServicesV1 {
 void* context;
 int (*number)(void*,const MenuRolloverValueV1*,double*);
 // Original imported __aeabi_d2iz conversion is an explicit ABI service.
 int (*integer)(void*,double,std::int32_t*);
 int (*boolean)(void*,const MenuRolloverValueV1*,std::uint32_t*);
 int (*instance)(void*,MenuRolloverBindingsV1**);
};
struct MenuRolloverGraphV1 {
 std::shared_ptr<void> owner;
 std::function<bool(double,std::int32_t&,std::string&)> integer;
 std::function<bool(MenuRolloverBindingsV1*&,std::string&)> instance;
};
class MenuRolloverInputV1 {
 MenuRolloverGraphV1 graph_;
public:
 explicit MenuRolloverInputV1(MenuRolloverGraphV1);
 // Preserve caller result and argument objects; conversions are synchronous.
 bool dispatch(CharacterMenuCallV1&,std::string&)const;
};
}
extern "C" {
// Complete NativeChangeRolloverInputBehavior43cdac + SetInputBehavior7a7c98.
// 0 delivered; -1 malformed before providers; -2 required provider failure;
// -3 source-invalid selected renderer after the complete conversion prefix.
int dh2_menu_rollover_input_v1(const dh2::ui::MenuRolloverCallV1*,
 const dh2::ui::MenuRolloverServicesV1*);
}
