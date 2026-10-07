#pragma once
#include "../engine-ui/settings_language_scene_v2.hpp"
#include <map>
#include <list>
#include <memory>
#include <string>
namespace dh2::world {
struct ObjectManagerLanguageBorrowV1 {
 std::int32_t source_handle_key{};
 std::uintptr_t identity{};
 std::shared_ptr<void> owner;
 const std::uint32_t* source_type_f4{};
 std::uint8_t* type14_localization_valid819{};
 // Actual source GetHandle→AsChar result; null is a genuine non-Character.
 std::uintptr_t as_character{};
};
// Source ObjectManager constructor/registration projection. Borrows canonical
// objects; owns only registry/list nodes. Same signed source handle-key order,
// and actual AsChar registration insertion order. No extra mutable actors.
class ObjectManagerLanguageRegistryV1 {
 struct Entry {ObjectManagerLanguageBorrowV1 borrow;ui::SettingsSceneObject24V1 object;ui::SettingsObjectNode32V1 node;};
 std::map<std::int32_t,Entry> objects_;
 std::list<ui::SettingsCharacterNode16V1> characters_;
 ui::SettingsCharacterNode16V1 character_end_{};
 ui::SettingsObjectNode32V1 object_end_{};
 ui::SettingsLanguageScene24V1 scene_{};
 void relink()noexcept;
public:
 ObjectManagerLanguageRegistryV1();
 ObjectManagerLanguageRegistryV1(const ObjectManagerLanguageRegistryV1&)=delete;
 ObjectManagerLanguageRegistryV1& operator=(const ObjectManagerLanguageRegistryV1&)=delete;
 ObjectManagerLanguageRegistryV1(ObjectManagerLanguageRegistryV1&&)=delete;
 ObjectManagerLanguageRegistryV1& operator=(ObjectManagerLanguageRegistryV1&&)=delete;
 // Called after real ObjectManager Add has assigned/published the same handle
 // and resolved duplicate-name deletion. Does not replace source Spawn/Add.
 bool source_added(ObjectManagerLanguageBorrowV1,std::string&);
 bool source_removed(std::int32_t,std::string&);
 void source_flush()noexcept;
 ui::SettingsLanguageScene24V1& scene()noexcept{return scene_;}
 std::size_t object_count()const noexcept{return objects_.size();}
 std::size_t character_count()const noexcept{return characters_.size();}
};
}
