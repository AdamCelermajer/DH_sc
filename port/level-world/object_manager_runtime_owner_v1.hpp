#pragma once

#include "game_object_zoning_visibility.hpp"
#include "room_zone_enrollment.hpp"
#include "packet_slot_owner_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <list>
#include <map>
#include <set>
#include <string>
#include <string_view>

namespace dh2::object_manager_runtime_owner_v1 {

using Address = std::uintptr_t;
using SourceHandle = std::int32_t;

static_assert(sizeof(SourceHandle) == 4, "source ObjectHandle key is ARM32 int");

// Canonical owned GameObject projection. These fields are the source-backed
// GameObject facts used by the existing zoning and RoomZone enrollment kernels.
// The projection is not an overlay on the original ARM32 object.
struct GameObject {
    Address identity{};
    SourceHandle source_handle{};
    // Exact ObjectManager key/name retained with the same source-handle row.
    // Empty means the caller has not yet supplied a source name.
    std::string name{};
    Address is_zonable_target_c4{};
    Address set_updating_target_3c{};
    Address room_zone_2f4{};
    Address visual_object_2d8{};
    float world_x{};
    float world_y{};
    std::uint8_t in_room_list_2ef{};
    std::uint8_t zoning_enabled_2ee{1};
    std::uint8_t in_zone_2f0{};
    std::uint8_t visible_80{1};
    // ObjectBase::setUpdating and SetVisible write these on the same source
    // row. They are distinct from the ObjectManager update-list membership.
    std::uint8_t updating_85{};
    std::uint8_t deleted_state_82{};
    // ObjectBase constructor's serialized +0x8a enabled byte. This belongs
    // to the same canonical GameObject row; Character construction publishes
    // it only after the source constructor completes.
    std::uint8_t serialized_flag_8a{};
    std::uint8_t serialized_flag_8a_ready{};
    // Mutable source-class facts used by the original ranged TargetList
    // predicates. These remain on the same ObjectManager row as the source
    // handle/provenance; readiness is published only by the class owner.
    std::int32_t container_state_394{};
    std::uint8_t container_state_ready{};
    // GameObject::GetInteractionRadius (+0x94) for source non-Character rows.
    // The radius is published only after the InitPost AABB is source-backed.
    float interaction_radius_94{};
    std::uint8_t interaction_radius_94_ready{};
    Address liftable_owner_390{};
    std::uint8_t liftable_state_394{};
    std::uint8_t liftable_state_ready{};
    std::int32_t trigger_count_3a8{};
    std::int32_t trigger_max_count_3b4{};
    std::int32_t trigger_pending_3b8{};
    std::uint8_t trigger_interactive_784{};
    std::uint8_t trigger_interactive_ready{};
    std::uint8_t no_room_member_2f8{};
    std::int32_t module_id_64{-1};
    std::uint8_t global_87{};
    // ObjectBase::__SaveObjects facts. +0x28 is the explicit save gate;
    // +0x81 is set by Delete/FakeRemove. A false readiness byte means the
    // source constructor/runtime path has not supplied these facts for this
    // exact manager row and save adapters must fail closed.
    std::uint8_t save_eligibility_28{};
    // ObjectBase lifetime lock byte, used by BackupObjectList::_Add/_Clear.
    std::uint8_t locked_29{};
    std::uint8_t locked_29_ready{};
    std::uint8_t deleted_81{};
    std::uint8_t save_facts_ready{};
    std::uint32_t source_module_index{};
    std::uint32_t source_record{};
    std::uint8_t source_provenance_ready{};
    // GameObject::InitFinal source fields: +0x2ed conditionally dispatches
    // SetEnabled(true); +0x254 receives the source name; +0x180 retains the
    // optional `target_node`; and the +0x288 LightSet name is constructor-empty
    // until a source data provider supplies it. These stay on this same row.
    std::uint8_t init_final_enable_2ed{};
    std::string init_final_name_254{};
    std::string init_final_light_set_name_288{};
    Address target_node_180{};
    // GameObject::CheckSpawnProbability source cache/threshold (+0x270/+0x274).
    // The roll belongs to the canonical GameObject row and starts unqueried.
    std::int32_t spawn_probability_result_270{-1};
    std::int32_t spawn_probability_274{100};

    // Optional borrowed live fields. When supplied, zoning/enrollment views
    // use these pointers instead of the value snapshot above. This lets a
    // renderer-owned actor remain the single position/membership owner while
    // the ObjectManager map preserves source key order and identity.
    struct LiveFields {
        float* world_x{};
        float* world_y{};
        Address* room_zone_2f4{};
        std::uint8_t* in_room_list_2ef{};
        std::uint8_t* zoning_enabled_2ee{};
        std::uint8_t* in_zone_2f0{};
        Address* visual_object_2d8{};
        std::uint8_t* visible_80{};
        std::uint8_t* updating_85{};
        std::uint8_t* deleted_state_82{};
        std::uint8_t* serialized_flag_8a{};
        std::int32_t* module_id_64{};
        std::uint8_t* global_87{};
        std::uint8_t* save_eligibility_28{};
        std::uint8_t* deleted_81{};
        std::uint8_t* locked_29{};
    } live_fields{};

    void bind_live_fields(const LiveFields& fields) noexcept {
        live_fields = fields;
    }
    bool read_save_base_facts(std::uint8_t* save_eligibility,
                              std::uint8_t* deleted) const noexcept {
        if (!save_facts_ready || !save_eligibility || !deleted) return false;
        *save_eligibility = live_fields.save_eligibility_28
            ? *live_fields.save_eligibility_28 : save_eligibility_28;
        *deleted = live_fields.deleted_81
            ? *live_fields.deleted_81 : deleted_81;
        return *save_eligibility <= 1 && *deleted <= 1;
    }

    dh2::game_object_zoning_visibility::GameObject zoning_view() noexcept;
    dh2::room_zone_enrollment::GameObject enrollment_view() noexcept;

private:
    friend class Owner;
    dh2::game_object_zoning_visibility::VisualObject visual_view_{};
};

enum class SourceInteractiveClass : std::uint8_t {
    container,
    liftable,
    trigger_object
};

// Evaluate the recovered virtual IsInteractive predicates from a complete
// canonical row. False means the class owner has not supplied every source
// field required by that predicate; callers must propagate that unavailable
// result instead of inventing a default.
bool evaluate_source_interactive(const GameObject& object,
                                 SourceInteractiveClass source_class,
                                 Address interacting_object,
                                 bool* interactive) noexcept;

enum class Status : std::uint8_t {
    ok,
    invalid_argument,
    duplicate_source_handle,
    duplicate_identity,
    source_handle_collision,
    source_handle_exhausted,
    not_found,
    allocation_failed,
    update_callback_failed
};

// The source ObjectManager stores ObjectListItem values in std::map<int,...>.
// GetObjectByName assigns the signed key from the manager's +0x4c counter and
// ObjectManager::Add fills that keyed item after duplicate-name resolution.
// InitObjectList and GetObjectsByType walk the tree in ascending signed-key
// order. This API models only that successful keyed registration; it does not
// implement name lookup, factory creation, ObjectHandle resolution, or removal
// from CharacterList. Map node addresses stay stable across unrelated inserts;
// erasing the object ends all borrowed-view lifetimes.
class Owner {
public:
    Owner() = default;
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) = delete;
    Owner& operator=(Owner&&) = delete;

    // NetworkInitLevel 0x340be0 always sets +0x1ac, including offline mode;
    // only the source online+5 route registers packet slot 3.
    void network_init_level(packet_slot_owner_v1::Owner& packets,
                            bool online_byte_5,
                            packet_slot_owner_v1::Callbacks callbacks) noexcept {
        if (online_byte_5) packets.register_slot(3, callbacks);
        network_initialized_1ac_ = 1;
    }
    // NetworkUnInitLevel 0x340bc4 unregisters slot 3 before clearing +0x1ac.
    // It neither Flushes GameObjects nor destroys the process packet registry.
    void network_uninit_level(packet_slot_owner_v1::Owner& packets) noexcept {
        packets.unregister_slot(3);
        network_initialized_1ac_ = 0;
    }
    std::uint8_t network_initialized_1ac() const noexcept {
        return network_initialized_1ac_;
    }

    Status add_object(SourceHandle source_handle, const GameObject& object,
                      GameObject** stored) noexcept;
    // ObjectManager::GetObjectByName uses exact strcmp for ordinary names.
    // Keep this index inside the same keyed owner; it is not a second registry.
    Status add_named_object(SourceHandle source_handle, std::string_view name,
                            const GameObject& object,
                            GameObject** stored) noexcept;
    // Mirrors GetObjectByName's exact-name fast path and create-on-miss
    // counter pre-increment. A created row is a name-only ObjectListItem
    // reservation until Add fills that same source key.
    Status get_or_reserve_named_handle(std::string_view name,
                                       bool create_if_missing,
                                       SourceHandle* source_handle,
                                       bool* created) noexcept;
    Status remove_object(SourceHandle source_handle, bool* removed) noexcept;
    // ObjectBase::Delete / ObjectManager::FakeRemove write the source +0x81
    // deletion byte on the same GameObject; they do not remove a parallel row.
    Status mark_deleted_81(Address identity, bool* changed) noexcept;
    // Exact ObjectBase::Delete byte order (+0x82=2 then +0x81=1),
    // projected into the existing ObjectManager row and any borrowed live
    // +0x81 field. This is logical deletion; the row and actor remain owned.
    Status delete_object(Address identity, bool* changed) noexcept;
    // ObjectBase::setUpdating and GameObject::SetVisible's false branch.
    // Both mutate only the existing canonical row, never its manager lifetime.
    Status set_updating(Address identity, std::uint8_t value) noexcept;
    Status set_visible(Address identity, std::uint8_t value) noexcept;
    // Source byte arithmetic: increment wraps; decrement stops at zero.
    Status lock_object(Address identity) noexcept;
    Status unlock_object(Address identity) noexcept;
    // ObjectBase::SetName changes the object name and the name stored under
    // its existing signed ObjectManager key. No object is removed/recreated.
    Status rename_object(Address identity, std::string_view name) noexcept;

    // Class-specific source constructor/update writers for fields consumed by
    // Container, LiftableObject, and TriggerObject's virtual predicates.
    // They mutate the existing canonical GameObject row and publish readiness
    // only after the complete source fact set is known.
    Status publish_container_state(Address identity, std::int32_t state) noexcept;
    Status publish_liftable_state(Address identity, Address owner,
                                  std::uint8_t state) noexcept;
    Status publish_trigger_state(Address identity, std::int32_t count,
                                 std::int32_t max_count,
                                 std::int32_t pending,
                                 std::uint8_t interactive) noexcept;

    // ObjectManager+0x2c active update list and +0x34 Spawn staging list.
    // Call the first only with the result of ObjectBase vtable+0x38 during
    // InitPost; call the second only when Spawn's same virtual query selected
    // its staging path. Neither operation infers eligibility or order from
    // the source-handle map, actor groups, or room membership.
    Status append_init_post_update_object(Address identity,
                                          bool source_is_updatable,
                                          bool* appended) noexcept;
    Status stage_spawn_update_object(Address identity,
                                     bool source_is_updatable,
                                     bool* staged) noexcept;
    // ObjectManager::Update splices the complete +0x34 list onto the tail of
    // +0x2c before traversing it. Actors staged during a later callback wait
    // until the next call, matching that one-pass boundary.
    void begin_update_frame() noexcept;
    bool update_object_at(std::size_t index, GameObject** object) noexcept;
    // Run the caller's existing ObjectBase::Update projection once for each
    // active list node, in native +0x2c order. The callback must not mutate
    // this list or erase its current object during traversal.
    using UpdateCallback = bool (*)(void*, GameObject*) noexcept;
    Status visit_update_objects(void* context, UpdateCallback callback,
                                std::size_t* visited) noexcept;
    std::size_t update_object_count() const noexcept { return update_objects_.size(); }
    std::size_t staged_update_object_count() const noexcept {
        return staged_update_objects_.size();
    }

    GameObject* find_by_source_handle(SourceHandle source_handle) noexcept;
    const GameObject* find_by_source_handle(SourceHandle source_handle) const noexcept;
    GameObject* find_by_identity(Address identity) noexcept;
    const GameObject* find_by_identity(Address identity) const noexcept;
    GameObject* find_by_name(std::string_view name) noexcept;
    const GameObject* find_by_name(std::string_view name) const noexcept;
    // Ordinary-name branch of GetObjectByName (0x34aca0): filter -1 accepts
    // every module; +0x87 accepts global objects. The filter is not a handle.
    // Duplicate names resolve in signed source-handle order. Name-only reserved
    // rows are skipped, as the original checks ObjectListItem's object pointer.
    GameObject* find_by_name(std::string_view name, std::int32_t module_filter) noexcept;
    const GameObject* find_by_name(std::string_view name, std::int32_t module_filter) const noexcept;

    // Each call returns the next live entry in the exact signed-key order used
    // by ObjectManager::InitPost/RoomZone::InitObjectList and GetObjectsByType.
    // The cursor stores only the last source key, so unrelated map insertions
    // do not invalidate it. Mutating/removing the current entry during a source
    // traversal is outside this bounded adapter contract.
    struct Cursor {
        SourceHandle last_source_handle{};
        bool started{};
        bool finished{};
    };
    void reset(Cursor* cursor) const noexcept;
    Status next(Cursor* cursor, GameObject** object) noexcept;

    // ObjectManager+0x88 no-room list. It is separate from the map traversal,
    // RoomZone member lists, and ObjectManager+0x60 CharacterList.
    Status add_no_room_object(Address identity, bool* added) noexcept;
    Status register_player_no_room(Address identity, bool* added) noexcept;
    Status remove_no_room_object(Address identity, bool* removed) noexcept;

    // RoomZone::Activate/DeActivate manage ObjectManager's independent active
    // room-object list (+0x80); this does not change RoomZone member lists.
    // Source AddRoomObjects asserts on duplicates, then still appends; its
    // DelRoomObjects counterpart removes every matching node.
    Status add_room_object(Address identity, bool* added) noexcept;
    Status remove_room_object(Address identity, bool* removed) noexcept;
    bool room_object_at(std::size_t index, Address* identity) const noexcept;
    std::size_t room_object_count() const noexcept { return room_objects_.size(); }
    // Borrow the canonical +0x80 active room-object list with a live iterator.
    // Next observes the current node's updated successor after synchronous
    // callbacks, as source RoomObjectList::Next does.
    struct RoomObjectCursor {
        const Owner* owner{};
        std::list<Address>::const_iterator current{};
        std::size_t visits{};
    };
    bool room_object_cursor_reset(RoomObjectCursor* cursor) const noexcept;
    bool room_object_cursor_at_end(const RoomObjectCursor* cursor) const noexcept;
    bool room_object_cursor_get(const RoomObjectCursor* cursor,
                                Address* identity) const noexcept;
    bool room_object_cursor_next(RoomObjectCursor* cursor) const noexcept;

    // Clear this bounded projection after the native ObjectManager::Flush
    // owner has retired native objects. This does not perform source virtual
    // destructors, Character-list pointer updates, queues/index cleanup, or
    // orphan-render flushing. It does not model the native +0x80 registry of
    // list-group pointers or its +0x88 re-register behavior. Borrowed actor
    // fields remain externally owned.
    void reset_after_native_flush() noexcept;

    void reset_visible_room_zone_count() noexcept { visible_room_zone_count_ = 0; }
    void add_visible_room_zone() noexcept { ++visible_room_zone_count_; }
    std::uint32_t visible_room_zone_count() const noexcept {
        return visible_room_zone_count_;
    }

    std::size_t object_count() const noexcept { return objects_.size(); }
    SourceHandle next_source_handle() const noexcept { return next_source_handle_; }
    bool empty() const noexcept {
        return objects_.empty() && no_room_objects_.empty() && room_objects_.empty() &&
            update_objects_.empty() && staged_update_objects_.empty();
    }
    std::size_t no_room_count() const noexcept { return no_room_objects_.size(); }
    bool no_room_at(std::size_t index, Address* identity) const noexcept;

private:
    using ObjectMap = std::map<SourceHandle, GameObject, std::less<SourceHandle>>;
    ObjectMap objects_;
    std::map<std::string, std::set<SourceHandle>, std::less<>> names_;
    // ObjectManager +0x4c: starts/resets to 1 and advances only on a
    // create-on-name-miss, before the keyed map slot is acquired.
    SourceHandle next_source_handle_{1};
    // Source AddNoRoomObject tail-inserts a GameObject*; RemoveNoRoomObject
    // removes the first equal pointer. Keep that list order independently.
    std::list<Address> no_room_objects_;
    std::list<Address> room_objects_;
    // Exact list order, keyed only through the canonical GameObject identity.
    std::list<Address> update_objects_;
    std::list<Address> staged_update_objects_;
    std::uint32_t visible_room_zone_count_{};
    std::uint8_t network_initialized_1ac_{};
};

} // namespace dh2::object_manager_runtime_owner_v1
