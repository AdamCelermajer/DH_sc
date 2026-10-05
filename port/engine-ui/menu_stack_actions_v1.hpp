#pragma once
#include "menu_stack_owner_v1.hpp"
#include <functional>
#include <memory>
#include <vector>
namespace dh2::ui {
struct CharacterMenuCallV1;
// Original packed AS tag, not upstream r1714's enum: 2 number, 3 as_string,
// 4 owned tu_string, 5 object/null, 6 property. Identity is a borrowed caller
// argument token; this module never dereferences or truncates it.
struct MenuStackActionValueV1 {
 std::uintptr_t identity;std::uint32_t source_type,reserved;
};
struct MenuStackActionCallV1 {
 const MenuStackActionValueV1* arguments;std::uint32_t count,reserved;
};
struct MenuStackActionServicesV1 {
 void* context;
 int (*instance)(void*,MenuStackV1**);
 // Genuine source to_xstring conversion. Returned bytes must survive the
 // full call, including instance/lifecycle/nested callbacks after conversion.
 int (*text)(void*,const MenuStackActionValueV1*,const char**);
};
struct MenuStackActionsGraphV1 {
 std::shared_ptr<void> owner;
 std::function<bool(MenuStackOwnerV1*&,std::string&)> instance;
 MenuStackServicesV1 lifecycle{};
};
class MenuStackActionsV1 {
 MenuStackActionsGraphV1 graph_;
public:
 explicit MenuStackActionsV1(MenuStackActionsGraphV1);
 // The argument token and its actual raw tag must be preserved by the AS
 // bridge. No result is read or written. to_text is invoked ONLY when reached.
 bool dispatch(const char* callback,const MenuStackActionCallV1&,
  const std::function<bool(const MenuStackActionValueV1&,std::string&,std::string&)>& to_text,
  std::string& error)const;
 // Root's retained AS bridge projection: semantic number remains original
 // tag2, string maps owned-string tag4, property6 remains deferred. No result
 // read/copy/write. Original tag3 is available through the raw overload only.
 bool dispatch(const char* callback,CharacterMenuCallV1&,std::string&)const;
};
}
extern "C" {
// op: 0 Push, 1 Pop, 2 PopAllAbove, 3 PopAllMenus. 0 delivered, -1 malformed,
// -2 required provider failure, then frozen stack -3/-4 continuations.
// Ignores extra arguments exactly as source. Push zero args is source-unsafe
// and is rejected before touching providers. AllAbove invalid signatures are
// genuine ignored paths and do not require instance/text/lifecycle providers.
int dh2_menu_stack_action_v1(std::uint32_t op,const dh2::ui::MenuStackActionCallV1*,
 const dh2::ui::MenuStackActionServicesV1*,const dh2::ui::MenuStackServicesV1*);
}
