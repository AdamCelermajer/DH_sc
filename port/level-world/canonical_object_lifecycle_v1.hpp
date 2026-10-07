#pragma once
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <string>
namespace dh2::world {
class CanonicalObjectManagerV1;struct CanonicalObjectBorrowV1;
// These are borrows of SAME original storage absent from the small map/Add
// projection, not permission to make fresh empty containers for teardown.
enum class ObjectManagerContainerV1:unsigned {list24=0x24,list3c=0x3c,list44=0x44,list80=0x80,tree98=0x98,treeb0=0xb0,treec8=0xc8,treee0=0xe0,tree108=0x108,list128=0x128,list130=0x130,tree148=0x148,tree164=0x164,tree17c=0x17c,tree194=0x194};
struct ObjectManagerNativeStorageV1 {
 std::shared_ptr<void> owner;std::uintptr_t manager_identity{};
 std::list<std::uintptr_t> *orphan4{},*list2c{},*characters70{},*no_room88{},*objects90{},*network100{};
 std::list<std::uint16_t>* removed120{};
 std::uint32_t *word54{},*word58{},*word138{},*word13c{},*word140{};std::uint8_t* byte160{};
 // Erase/reset this reached actual container only. This is storage cleanup,
 // never a whole Remove/Flush callback, gameplay leaf or guessed absence.
 std::function<bool(ObjectManagerContainerV1,std::string&)> clear;
 // Source AddRoomObjects3427a0 registers the SAME now-empty no_room88 list
 // address in real list80 after list80 clears. No copied room-list snapshot.
 std::function<bool(std::string&)> register_no_room88;
};
struct ObjectManagerGameObjectFieldsV1 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 std::uintptr_t* room_zone2f4{};std::uint8_t *no_room2f8{},*orphan2fc{};
};
struct CanonicalObjectLifecycleV1 {
 std::shared_ptr<void> owner; // independent; containing Level/World weak
 std::function<bool(CanonicalObjectManagerV1&,std::string&)> require_quiescent;
 std::function<bool(CanonicalObjectManagerV1&,ObjectManagerNativeStorageV1&,std::string&)> native_storage;
 // Exact ObjectHandle GameObject conversion: GetObject(false), real virtual20.
 // false conversion is source NULL; do not infer it from class name/type_f4.
 std::function<bool(const CanonicalObjectBorrowV1&,bool&,ObjectManagerGameObjectFieldsV1&,std::string&)> game_object;
 std::function<bool(const ObjectManagerGameObjectFieldsV1&,std::uintptr_t,std::string&)> room_remove;
 std::function<bool(const CanonicalObjectBorrowV1&,std::string&)> flush_target_list,object_delete,ai_update_pointers,ai_remove_from_group;
 std::function<bool(bool&,std::string&)> online_byte5;
 std::function<bool(const CanonicalObjectBorrowV1&,bool&,std::string&)> character_virtual28;
 std::function<bool(const CanonicalObjectBorrowV1&,std::uint16_t&,std::string&)> network_id108;
 // Real class D0 portion INCLUDING source-position ConditionData clear and
 // derived/base Lua/scene/network teardown. Must not free the borrowed host
 // allocation or retire its journal before manager/transport/draw unpublish.
 std::function<bool(const CanonicalObjectBorrowV1&,std::string&)> class_d0;
 // Real orphan queue owns native receiver after map erase. Existing Main
 // release journal must admit/pin it; adding an integer to orphan4 is not D0.
 std::function<bool(const CanonicalObjectBorrowV1&,std::string&)> orphan_admit;
 // Borrow existing native orphan identity after its map entry was erased.
 // No loader-owned orphan receiver registry or manufactured object.
 std::function<bool(std::uintptr_t,CanonicalObjectBorrowV1&,std::string&)> native_receiver;
 // Host journal retirement ONLY after native bodies and actual publication
 // checks. Does no functional teardown; pending/orphan aliases stay pinned.
 std::function<bool(CanonicalObjectManagerV1&,std::string&)> retire_after_unpublication;
};
}
