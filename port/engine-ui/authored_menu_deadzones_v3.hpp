#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <vector>
namespace dh2::ui {
// Borrowed source RenderFX::CollectCharacters projection. Child order is the
// actual sprite display list order; identities are retained movie characters.
struct AuthoredMenuCharacterBorrowV3 {
 std::uintptr_t identity{};
 const char* name{};
 bool visible{},sprite{},focus_enabled{};
 std::vector<AuthoredMenuCharacterBorrowV3*> children;
};
bool authored_menu_collect_characters_v3(AuthoredMenuCharacterBorrowV3*,
 const char* substring,std::uint32_t source_flags,
 std::vector<AuthoredMenuCharacterBorrowV3*>&,std::string&);
struct AuthoredMenuDeadZoneV3 {float xmin{},xmax{},ymin{},ymax{};};
// 416a7c adds the ordered parent-chain translations from 4169e8 to the
// receiver's virtual bounding rectangle, then divides each edge by20.
AuthoredMenuDeadZoneV3 authored_menu_absolute_rectangle_v3(
 const AuthoredMenuDeadZoneV3& receiver_bounds,float parent_tx,float parent_ty);
struct AuthoredMenuDeadZoneServicesV3 {
 std::function<bool(std::string&)> debug;
 // GameSWFUtils::GetAbsoluteBoundingRect: actual parent translations and virtual
 // transformed bounds, then /20. This callback must borrow those producers.
 std::function<bool(AuthoredMenuCharacterBorrowV3&,AuthoredMenuDeadZoneV3&,std::string&)> bounds;
};
// MenuBase::RegisterDeadZones sets registered before Debug/search. Failure is
// a retained source mutation prefix; repeated invocation does not retry it.
bool authored_menu_register_deadzones_v3(std::uint8_t& registered,
 AuthoredMenuCharacterBorrowV3*,const AuthoredMenuDeadZoneServicesV3&,
 std::vector<AuthoredMenuDeadZoneV3>&,std::string&);
}
