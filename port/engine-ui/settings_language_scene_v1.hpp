#pragma once
#include <cstdint>
#include <cstddef>
namespace dh2::ui {
struct SettingsCharacterNode16V1 {SettingsCharacterNode16V1* next;std::uintptr_t character;};
struct SettingsSceneObject24V1 {std::uintptr_t identity;const std::uint32_t* type;std::uint8_t* localization_valid;};
struct SettingsObjectNode32V1 {SettingsObjectNode32V1* parent;SettingsObjectNode32V1* left;SettingsObjectNode32V1* right;SettingsSceneObject24V1* object;};
// Borrowed source-order graph required by Application::SetLanguage. The
// Character ring must contain the real current Level Characters in Object
// Manager insertion order, not aggro/search Character projections. The
// ObjectManager fields must describe its live in-order map traversal; re-read
// first/end after Character callbacks because those callbacks can mutate the
// manager. Character/object identities and their owner records must survive
// the entire synchronous call. Do not represent a missing graph as empty.
struct SettingsLanguageScene24V1 {SettingsCharacterNode16V1* characters;SettingsObjectNode32V1* object_end;SettingsObjectNode32V1* object_first;};
enum class SettingsSceneOperationV1:std::uint32_t {is_player=1,is_merchant=2,refresh_inventory=3,is_game_object=4,refresh_item=5};
struct SettingsSceneRequest16V1 {SettingsSceneOperationV1 operation;std::uint32_t reserved;std::uintptr_t identity;};
struct SettingsSceneServices16V1 {void* context;int (*invoke)(void*,const SettingsSceneRequest16V1*,std::uint32_t* result);};
static_assert(sizeof(SettingsCharacterNode16V1)==16&&sizeof(SettingsSceneObject24V1)==24&&sizeof(SettingsObjectNode32V1)==32&&sizeof(SettingsLanguageScene24V1)==24&&sizeof(SettingsSceneRequest16V1)==16&&sizeof(SettingsSceneServices16V1)==16);
// Required callback sequence: is_player; if false, is_merchant; if either is
// true, refresh_inventory. Then, for each live ObjectManager tree object,
// is_game_object; if true and type==3, refresh_item; if true and type==14,
// clear localization_valid. The caller must keep one canonical Character,
// inventory, ObjectManager/GameObject, item-text, and Localization owner for
// these identities. In particular, a flat aggro roster or the zoning-only
// ObjectManager projection lacks this contract's Character predicates and
// type-14 cache byte, so neither is sufficient by itself.
}
// Exact setLanguage object-manager traversal and type14 invalidation prefix.
// Character virtual predicates and genuine inventory/item localization remain
// mandatory source services. 0 delivered,-1 malformed graph,-2 missing/rejected
// required service. Mutation/reentry is synchronous, with live type reload.
extern "C" int dh2_settings_v1_refresh_language_scene(dh2::ui::SettingsLanguageScene24V1*,const dh2::ui::SettingsSceneServices16V1*) noexcept;
