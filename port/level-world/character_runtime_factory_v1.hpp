#pragma once

#include "character_constructor_owner_v1.hpp"
#include "character_net_state_owner_v1.hpp"
#include "character_aggro_character_list.hpp"
#include "character_faery_script_session_v1.hpp"
#include "character_coordinator.hpp"
#include "character_kill_field_14e4_v1.hpp"
#include "character_faery_visual_facade_v1.hpp"
#include "character_ai_initialization.hpp"
#include "object_manager_runtime_owner_v1.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace dh2::character_gameplay_save_v1 {
// SaveRef is borrowed from the existing gameplay Save association. The
// factory never constructs a PlayerSavegame or a LoadOwner.
struct SaveRef;
struct Result;
}

namespace dh2::character_physical_component_v1 { class Owner; }

namespace dh2::character_runtime_factory_v1 {

namespace ctor = character_constructor_owner_v1;
namespace net_state = character_net_state_owner_v1;
namespace manager = object_manager_runtime_owner_v1;
namespace aggro = character::aggro_search;
namespace gameplay_save = ::dh2::character_gameplay_save_v1;

enum class SourceSaveSlotKind : std::uint8_t {
    // Character constructor writes a null +0x14e8 Save pointer.
    constructor_null,
    // Non-player InitPost was source-classified and retains the null slot.
    nonplayer_null,
    // Player SaveRef is borrowed; Save and LoadOwner remain externally owned.
    player_save,
};

inline constexpr std::size_t component_count = 11;
class Record;

// Stable semantic owner references for one Character. Component providers
// populate these with pointers to the canonical native owners they adopt or
// create; this array is storage/indexing only, not a fabricated component.
struct ComponentStorage {
    struct Slot {
        ctor::Component component{};
        void* canonical_owner{};
        bool constructed{};
        // Provider sees the stable factory record while constructing each
        // canonical component. This avoids parallel per-Character registries.
        Record* character_record{};
    };
    std::array<Slot, component_count> slots{};

    void* find(ctor::Component component) const noexcept;
};

// Source ItemInventory constructor projection for Character+0x37c. The
// constructor creates two equipment sets with nine null ItemSlot pointers
// each, then Character binds the inventory's +4 Character pointer and the
// selected set reads as zero. This is constructor state only; item creation,
// Save loading, and inventory mutations belong to later source lifecycle
// providers. Player V4 remains its separate Save-backed owner.
struct ItemInventoryConstructorV1 {
    std::uintptr_t character{};
    std::array<std::array<void*, 9>, 2> equipment{};
    std::uint8_t selected_set{};
    bool constructed{};

    bool construct(std::uintptr_t identity) noexcept {
        if (!identity || constructed) return false;
        character = identity;
        equipment = {};
        selected_set = 0;
        constructed = true;
        return true;
    }
    void retire() noexcept {
        character = 0;
        equipment = {};
        selected_set = 0;
        constructed = false;
    }
};

// Component and lifecycle services must use the existing native semantic
// owners. A successful component call must set the slot to that canonical
// owner; a failed call must leave its own partial work rolled back.
struct Services {
    void* context{};
    int (*component)(void*, ctor::Component, ctor::Identity,
                     ComponentStorage::Slot*, std::string&){};
    int (*associate)(void*, ctor::Association, ctor::Identity,
                     const ComponentStorage&, std::string&){};
    int (*register_state)(void*, ctor::Identity, std::uint32_t,
                          std::string&){};
    void (*rollback)(void*, ctor::Action, std::uint32_t, ctor::Identity,
                     ComponentStorage&) noexcept{};
};

// The app owns this roster service through native_character_list::Owner.
// Keeping it as a narrow callback avoids a dependency from dh2_level_world
// back to the app shared library while still publishing the exact owned
// aggro_search::Character projection consumed by that roster.
struct RosterServices {
    void* context{};
    int (*enroll_after_add)(void*, aggro::Character*, bool duplicate_resolved,
                            bool* appended){};
    int (*remove_after_remove)(void*, aggro::Character*, std::size_t* removed){};
};

class Record final {
public:
    manager::GameObject game_object{};
    aggro::GameObject aggro_object{};
    aggro::Character character{};
    ComponentStorage components{};
    ctor::Owner constructor{};
    // One native composition owner supplies the source CharTimers and
    // CharStateMachine constructor slots. Both slots point to this same owner;
    // it owns the single timer store and FSM state for this Character.
    std::unique_ptr<character::Coordinator> coordinator;
    // One source ItemInventory constructor owner for this same Character.
    // It owns only the two empty source equipment-set projections at this
    // constructor stage; later InitEquipment providers must extend this owner.
    ItemInventoryConstructorV1 source_inventory{};
    // Retained Character+0x14e4 projection. Owner initializes it only after
    // the complete source Character constructor composition succeeds and
    // retires it with the source Character Record; network-spawn updates must
    // come from their own source-equivalent message path.
    character_kill_field_14e4_v1::State kill_field_14e4{};
    // The one AISFaery/CharAIScript session slot for this exact Character
    // Record. Its VM/session lifetime may not outlive this factory owner.
    character_faery_script_session_v1::Owner faery_script{};
    // Typed POCharacter/POFaerie body owner, bound to this Record's existing
    // Character and GameObject projections. This does not construct a second
    // ObjectManager or CharacterList row and does not emulate a native vtable.
    std::shared_ptr<character_physical_component_v1::Owner> pofaerie_physical;
    // Per-Character BDAE visual facade. The selected AssetData is shared;
    // placement, visibility, mutable pose, and clip cursor stay on this
    // canonical factory Record. Current objects::Resource still owns mutable
    // geometry buffers, so parsed geometry sharing awaits renderer separation.
    character_faery_visual_facade_v1::CharacterVisual faery_visual{};
    // Opaque identity returned by the source POFaerie constructor provider;
    // zero until that provider actually supplies the native source owner.
    std::uintptr_t source_pofaerie_identity{};
    net_state::Owner net_state{};
    std::uint64_t net_state_change_counter{};
    std::int16_t property_id_13c8{-1};
    std::string source_model_name;
    std::array<float, 3> source_owner_scale{1.0f, 1.0f, 1.0f};
    std::uint8_t nonplayer_init_post_prefix_stage{};
    bool nonplayer_init_post_selected{};
    gameplay_save::SaveRef* source_save_14e8{};
    SourceSaveSlotKind source_save_slot_kind{SourceSaveSlotKind::constructor_null};
    std::shared_ptr<void> source_save_lifetime;
    bool source_save_mask2_attempted{};
    bool source_save_mask2_complete{};
    // Character::InitPost's post-SG_Load(2) AI branch is independently
    // one-shot. It can complete before FX/scene-backed initialization.
    bool nonplayer_ai_postload_complete{};
    std::int32_t nonplayer_source_ai_id{-1};
    // Character+0x3ec is assigned one for AITable delayed-load actors.
    std::uint8_t source_delay_init_3ec{};
    // Character+0x539 suppresses level-owned kill/quest behavior for NPCs
    // created by the script/network CreateNPC path. Authored actors stay zero.
    std::uint8_t source_script_created_539{};
    // Post-AI InitPost FX/anim/sound calls are ordered and at-most-once after
    // an uncertain provider result. 0..4 are completed stages; attempted is
    // 0..3 while that operation is in flight/failed, or 0xff when none is.
    std::uint8_t nonplayer_init_post_effects_stage{};
    std::uint8_t nonplayer_init_post_effects_attempted{0xff};
    std::int32_t source_anim_fx_base_1488{};
    bool source_anim_fx_base_ready{};
    std::uintptr_t source_self_anim_fx_1484{};
    std::int32_t source_self_anim_fx_id{};
    ctor::Services constructor_services{};
    Services services{};
    manager::SourceHandle source_handle{};
    bool object_registered{};
    bool character_listed{};
    // Source Character+0x420 Faery Character link. Kept on the canonical
    // Character record so Faery placement does not invent a second link store.
    std::uintptr_t faery_character_420{};

    bool source_properties_ready() const noexcept { return source_properties_ready_; }
    bool init_post_complete() const noexcept { return init_post_complete_; }
    bool init_final_complete() const noexcept { return init_final_complete_; }

    // Resolve the one canonical CharAI owner installed in this Record's AI
    // component slot. This is a borrowed typed view, never a second owner.
    character_ai_initialization::State* canonical_ai() noexcept;
    const character_ai_initialization::State* canonical_ai() const noexcept;

private:
    friend class Owner;
    // Explicit source lifecycle barriers. Only Owner may advance these; create
    // deliberately leaves them false and callbacks cannot forge completion.
    bool source_properties_ready_{};
    bool init_post_complete_{};
    bool init_final_complete_{};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    service_unavailable,
    duplicate_source_handle,
    constructor_failed,
    object_manager_failed,
    roster_failed,
    cleanup_incomplete,
    properties_not_ready,
    init_post_not_complete,
    lifecycle_service_unavailable,
    init_post_failed,
    init_final_failed,
    not_found,
};

struct Result {
    ctor::Result constructor{};
    manager::Status object_manager_status{manager::Status::ok};
    int roster_status{};
    bool object_registered{};
    bool character_listed{};
    bool source_properties_ready{};
    bool init_post_complete{};
    bool init_final_complete{};
};

// Lifecycle callbacks are supplied by the caller that owns canonical source
// properties and runtime services. They are never run from create(). A failed
// callback leaves its stage pending so the caller can repair the service and
// retry; callback implementations own rollback of their own partial work.
struct LifecycleServices {
    void* context{};
    int (*init_post)(void*, Record&, std::string&){};
    int (*init_final)(void*, Record&, std::string&){};
};

// Owns stable semantic Character/GameObject projections and the constructor
// bookkeeping. It deliberately stops after constructor + ObjectManager map +
// CharacterList enrollment: Character::InitPost/InitFinal, physics, AI and
// visual initialization remain caller-owned services and are not implied.
class Owner final {
public:
    Owner(manager::Owner& object_manager, const RosterServices& roster) noexcept;
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) = delete;
    Owner& operator=(Owner&&) = delete;

    Status create(manager::SourceHandle source_handle,
                  const manager::GameObject& game_object_seed,
                  const aggro::GameObject& aggro_object_seed,
                  const Services& services, Record** output, Result* result,
                  std::string& error);
    Status retire(manager::SourceHandle source_handle, Result* result,
                  std::string& error) noexcept;
    // Call only after the caller has applied canonical source defaults,
    // template data, and XML overrides to this Character's property owner.
    Status mark_source_properties_ready(manager::SourceHandle source_handle,
                                        Result* result, std::string& error);
    // Set the source Character+0x539 bit after a CreateNPC owner has returned
    // its canonical Character; idempotent and never changes authored actors.
    Status mark_script_created(manager::SourceHandle source_handle,
                               std::string& error) noexcept;
    // Bind only an already constructed source player Save. The SaveRef, Save,
    // LoadOwner and their callback context must remain alive while this Record
    // is registered; lifetime retains that existing owner graph, never creates
    // a Save. Non-player Characters retain their constructor-null +0x14e8 slot.
    Status bind_player_save_slot(manager::SourceHandle source_handle,
                                 gameplay_save::SaveRef*,
                                 std::shared_ptr<void> lifetime,
                                 std::string& error);
    // Exact Character::SG_Load(2) wrapper. A classified null slot is a source
    // no-op; a player slot forwards mask 2 once through its existing loader.
    Status load_source_save_mask2(manager::SourceHandle source_handle,
                                 gameplay_save::Result*, std::string& error);
    // Executes each successful stage at most once. A failed callback can be
    // retried; InitFinal is gated on successful InitPost.
    Status run_init_post(manager::SourceHandle source_handle,
                         const LifecycleServices& services, Result* result,
                         std::string& error);
    Status run_init_final(manager::SourceHandle source_handle,
                          const LifecycleServices& services, Result* result,
                          std::string& error);
    Record* find(manager::SourceHandle source_handle) noexcept;
    const Record* find(manager::SourceHandle source_handle) const noexcept;
    // Identity lookup stays inside the same ObjectManager-keyed Record owner;
    // it is used by synchronous CharacterList consumers that receive a
    // Character identity rather than its original source handle.
    Record* find_character(std::uintptr_t identity) noexcept;
    const Record* find_character(std::uintptr_t identity) const noexcept;
    std::size_t size() const noexcept { return records_.size(); }

private:
    struct Bridge { Record* record{}; };
    static int component(void*, ctor::Component, ctor::Identity, std::string&);
    static int associate(void*, ctor::Association, ctor::Identity, std::string&);
    static int register_state(void*, ctor::Identity, std::uint32_t, std::string&);
    static void rollback(void*, ctor::Action, std::uint32_t,
                         ctor::Identity) noexcept;
    static void rollback_completed(Record&) noexcept;

    manager::Owner& object_manager_;
    RosterServices roster_{};
    std::map<manager::SourceHandle, std::unique_ptr<Record>> records_;
};

} // namespace dh2::character_runtime_factory_v1
