#pragma once
#include "canonical_object_manager_v1.hpp"
namespace dh2::world {
// Actual native receiver cells are lent through the same journal as Remove.
// Pointer absence is an unproduced field, never an assumed zero/false value.
struct ObjectUpdateActorV102 {
 CanonicalObjectBorrowV1 object;
 std::function<std::uint8_t*(std::uint32_t)> byte;
 std::function<const std::uintptr_t*(std::uint32_t)> pointer;
 std::function<std::int32_t*(std::uint32_t)> integer;
 std::function<std::uint32_t*(std::uint32_t)> word;
};
struct ObjectUpdateStorageV102 {
 std::shared_ptr<void> owner;
 std::list<std::uintptr_t>* active2c{};
 std::list<std::uintptr_t>* pending34{};
 std::list<std::uintptr_t>* deletion3c{};
 std::list<std::uintptr_t>* conditions44{};
 std::list<std::uintptr_t>* characters70{};
 std::uint32_t *updates58{},*updates5c{};
 std::uint8_t *online_fc{},*online_fd{};
};
struct ObjectUpdateServicesV102 {
 std::shared_ptr<void> owner;
 std::function<bool(ObjectUpdateStorageV102&,std::string&)> storage;
 std::function<bool(std::uintptr_t,ObjectUpdateActorV102&,std::string&)> actor;
 std::function<bool(bool&,std::string&)> online;
 // Empty identity is genuine Application.GetCurrentLevel NULL. Positive
 // fields borrow the actual Level144/198, independent of loading stage130.
 std::function<bool(std::shared_ptr<void>&,const std::uint8_t*&,const std::uint8_t*&,std::string&)> level;
 std::function<bool(std::string&)> rooms;
 std::function<bool(std::uintptr_t,std::string&)> zone_entered;
 std::function<bool(const ObjectUpdateActorV102&,bool&,std::string&)> deferred,is_character,character_virtual28,remotely_updated;
 std::function<bool(const ObjectUpdateActorV102&,std::string&)> update_ai,update,unload_script;
 std::function<bool(const ObjectUpdateActorV102&,bool,std::string&)> test_enable,test_disable;
 std::function<bool(const ObjectUpdateActorV102&,bool,std::string&)> remove;
 std::function<bool(const ObjectUpdateActorV102&,std::uintptr_t&,std::string&)> resolve_handle_false,resolve_character;
 std::function<bool(std::string&)> reset_debug_switches;
};
bool borrow_object_update_storage_v102(const std::shared_ptr<CanonicalObjectManagerV1>&,
 ObjectUpdateStorageV102&,std::string&);
bool process_next_start_v102(CanonicalObjectManagerV1&,
 const std::function<bool(std::uintptr_t,std::string&)>&,std::string&);
bool update_remote_objects_v102(CanonicalObjectManagerV1&,float,
 const ObjectUpdateServicesV102&,std::string&);
// Whole original34a620 ordering. No map sweep, extra actor clocks or implicit
// success for a missing positive receiver. Failure retains source prefixes.
bool canonical_object_update_v102(CanonicalObjectManagerV1&,float,
 const ObjectUpdateServicesV102&,std::string&);
}
