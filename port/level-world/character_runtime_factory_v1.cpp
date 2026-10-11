#include "character_runtime_factory_v1.hpp"
#include "character_gameplay_save_v1.hpp"

#include <new>

namespace dh2::character_runtime_factory_v1 {
namespace {

constexpr std::size_t slot_index(ctor::Component component) noexcept {
    return static_cast<std::size_t>(component);
}

bool valid_component(ctor::Component component) noexcept {
    return slot_index(component) < component_count;
}

void copy_lifecycle_state(Result* result, const Record& record) noexcept {
    if (!result) return;
    result->object_registered = record.object_registered;
    result->character_listed = record.character_listed;
    result->source_properties_ready = record.source_properties_ready();
    result->init_post_complete = record.init_post_complete();
    result->init_final_complete = record.init_final_complete();
}

} // namespace

void* ComponentStorage::find(ctor::Component component) const noexcept {
    if (!valid_component(component)) return nullptr;
    const auto& slot = slots[slot_index(component)];
    return slot.constructed ? slot.canonical_owner : nullptr;
}

character_ai_initialization::State* Record::canonical_ai() noexcept {
    return const_cast<character_ai_initialization::State*>(
        static_cast<const Record*>(this)->canonical_ai());
}

const character_ai_initialization::State* Record::canonical_ai() const noexcept {
    const auto identity = constructor.identity;
    const auto& slot = components.slots[slot_index(ctor::Component::ai)];
    if (!identity || character.identity != identity ||
        game_object.identity != identity ||
        slot.component != ctor::Component::ai || !slot.constructed ||
        slot.character_record != this || !slot.canonical_owner)
        return nullptr;
    auto* state = static_cast<const character_ai_initialization::State*>(slot.canonical_owner);
    return state && state->identity && state->owner_04 == identity
        ? state : nullptr;
}

Owner::Owner(manager::Owner& object_manager,
             const RosterServices& roster) noexcept
    : object_manager_(object_manager), roster_(roster) {}

int Owner::component(void* context, ctor::Component component_id,
                     ctor::Identity identity, std::string& error) {
    auto* record = static_cast<Record*>(context);
    if (!record || record->character.identity != identity ||
        !valid_component(component_id)) {
        error = "Character component request does not match its stable owner";
        return 1;
    }
    auto& slot = record->components.slots[slot_index(component_id)];
    if (slot.constructed || slot.canonical_owner) {
        error = "Character component was already owned";
        return 1;
    }
    slot.character_record = record;
    if (component_id == ctor::Component::inventory) {
        if (!record->source_inventory.construct(identity)) {
            slot = {component_id, nullptr, false, nullptr};
            error = "Canonical source ItemInventory constructor rejected Character identity";
            return 1;
        }
        slot.component = component_id;
        slot.canonical_owner = &record->source_inventory;
        slot.constructed = true;
        return 0;
    }
    if (component_id == ctor::Component::timers) {
        try {
            // CharTimers and CharStateMachine are composed by one canonical
            // native owner. Create it at the source CharTimers callsite; the
            // later state-machine step publishes this same owner, never a
            // second timer vector or parallel FSM.
            record->coordinator = std::make_unique<character::Coordinator>(identity);
        } catch (...) {
            record->coordinator.reset();
            slot = {component_id, nullptr, false, nullptr};
            error = "Canonical Character timer/state owner allocation failed";
            return 1;
        }
        slot.component = component_id;
        slot.canonical_owner = record->coordinator.get();
        slot.constructed = true;
        return 0;
    }
    if (component_id == ctor::Component::state_machine) {
        if (!record->coordinator || record->coordinator->owner() != identity) {
            slot = {component_id, nullptr, false, nullptr};
            error = "Character state-machine slot has no same-Record timer owner";
            return 1;
        }
        slot.component = component_id;
        slot.canonical_owner = record->coordinator.get();
        slot.constructed = true;
        return 0;
    }
    if (component_id == ctor::Component::net_state_primary ||
        component_id == ctor::Component::net_state_secondary) {
        const auto role = component_id == ctor::Component::net_state_primary
            ? net_state::Role::outgoing_primary
            : net_state::Role::incoming_secondary;
        const auto status = net_state::construct_component(&record->net_state,
            role, identity, &record->net_state_change_counter);
        if (status != net_state::Status::complete) {
            slot = {component_id, nullptr, false, nullptr};
            error = "Character NetState component violated source construction order";
            return 1;
        }
        slot.component = component_id;
        slot.canonical_owner = component_id == ctor::Component::net_state_primary
            ? static_cast<void*>(&record->net_state.primary)
            : static_cast<void*>(&record->net_state.secondary);
        slot.constructed = true;
        return 0;
    }
    int status = 1;
    try {
        status = record->services.component(record->services.context,
            component_id, identity, &slot, error);
    } catch (...) {
        status = 1;
    }
    if (status == 0 && slot.canonical_owner) {
        slot.component = component_id;
        slot.constructed = true;
        return 0;
    }
    if (status == 0) error = "Canonical Character component provider returned no owner";
    slot = {component_id, nullptr, false, nullptr};
    return 1;
}

int Owner::associate(void* context, ctor::Association association,
                     ctor::Identity identity, std::string& error) {
    auto* record = static_cast<Record*>(context);
    if (!record || record->character.identity != identity) {
        error = "Character association does not match its stable owner";
        return 1;
    }
    try {
        return record->services.associate(record->services.context, association,
            identity, record->components, error);
    } catch (...) {
        return 1;
    }
}

int Owner::register_state(void* context, ctor::Identity identity,
                          std::uint32_t state, std::string& error) {
    auto* record = static_cast<Record*>(context);
    if (!record || record->character.identity != identity) {
        error = "Character state registration does not match its stable owner";
        return 1;
    }
    try {
        return record->services.register_state(record->services.context,
                                                identity, state, error);
    } catch (...) {
        return 1;
    }
}

void Owner::rollback(void* context, ctor::Action action, std::uint32_t value,
                     ctor::Identity identity) noexcept {
    auto* record = static_cast<Record*>(context);
    if (!record || record->character.identity != identity) return;
    if (action == ctor::Action::component &&
        (value == std::uint32_t(ctor::Component::net_state_primary) ||
         value == std::uint32_t(ctor::Component::net_state_secondary))) {
        const auto role = value == std::uint32_t(ctor::Component::net_state_primary)
            ? net_state::Role::outgoing_primary
            : net_state::Role::incoming_secondary;
        const auto status = net_state::destroy_component(&record->net_state,
            role, identity);
        if (status == net_state::Status::complete)
            record->components.slots[value] = {
                static_cast<ctor::Component>(value), nullptr, false, nullptr};
        return;
    }
    if (action == ctor::Action::component &&
        (value == std::uint32_t(ctor::Component::timers) ||
         value == std::uint32_t(ctor::Component::state_machine))) {
        if (value == std::uint32_t(ctor::Component::timers))
            record->coordinator.reset();
        if (value < component_count)
            record->components.slots[value] = {
                static_cast<ctor::Component>(value), nullptr, false, nullptr};
        return;
    }
    if (action == ctor::Action::component &&
        value == std::uint32_t(ctor::Component::inventory)) {
        record->source_inventory.retire();
        if (value < component_count)
            record->components.slots[value] = {
                static_cast<ctor::Component>(value), nullptr, false, nullptr};
        return;
    }
    record->services.rollback(record->services.context, action, value, identity,
                              record->components);
    if (action == ctor::Action::component && value < component_count) {
        record->components.slots[value] = {
            static_cast<ctor::Component>(value), nullptr, false};
    }
}

void Owner::rollback_completed(Record& record) noexcept {
    // Character constructor dependencies are retired in reverse source order.
    for (std::uint32_t state = ctor::registered_state_count; state > 0; --state) {
        const auto id = state - 1;
        if ((record.constructor.registered_states & (1u << id)) != 0) {
            rollback(&record, ctor::Action::state, id, record.character.identity);
        }
    }
    for (std::size_t i = sizeof(ctor::source_steps) /
                          sizeof(ctor::source_steps[0]); i > 0; --i) {
        const auto& step = ctor::source_steps[i - 1];
        rollback(&record, step.action, step.value, record.character.identity);
    }
    if (record.kill_field_14e4.constructor_write_seen)
        (void)character_kill_field_14e4_v1::retire(
            &record.kill_field_14e4, record.character.identity);
    // The Faery adapter borrows the already-constructed controller and
    // controllable component addresses. Retire that binding with the same
    // canonical Record, after the source component rollback sequence.
    character_faery_controller_v1::retire(record.faery_controller);
    record.constructor = {};
}

Status Owner::create(manager::SourceHandle source_handle,
                     const manager::GameObject& game_object_seed,
                     const aggro::GameObject& aggro_object_seed,
                     const Services& services, Record** output, Result* result,
                     std::string& error) {
    error.clear();
    if (output) *output = nullptr;
    if (result) *result = {};
    if (!output || !result || !services.component || !services.associate ||
        !services.register_state || !services.rollback ||
        !roster_.enroll_after_add || !roster_.remove_after_remove) {
        error = "Character factory requires complete component and roster services";
        return Status::service_unavailable;
    }
    if (records_.find(source_handle) != records_.end()) {
        error = "Character source handle is already owned";
        return Status::duplicate_source_handle;
    }

    std::unique_ptr<Record> owned;
    try {
        owned = std::make_unique<Record>();
    } catch (...) {
        error = "Character factory allocation failed";
        return Status::object_manager_failed;
    }
    Record& record = *owned;
    record.source_handle = source_handle;
    record.game_object = game_object_seed;
    record.aggro_object = aggro_object_seed;
    const auto object_identity = reinterpret_cast<std::uintptr_t>(&record.game_object);
    record.game_object.identity = object_identity;
    record.aggro_object.identity = object_identity;
    // Character is a GameObject-derived source object. All three semantic
    // projections therefore share the one stable GameObject identity.
    record.character.identity = object_identity;
    record.character.object = &record.aggro_object;
    for (std::size_t i = 0; i < component_count; ++i) {
        record.components.slots[i].component = static_cast<ctor::Component>(i);
    }
    record.services = services;
    record.constructor_services = {&record, &Owner::component, &Owner::associate,
        &Owner::register_state, &Owner::rollback};

    try {
        const auto inserted = records_.emplace(source_handle, std::move(owned));
        if (!inserted.second) {
            error = "Character source handle is already owned";
            return Status::duplicate_source_handle;
        }
    } catch (...) {
        error = "Character factory registry allocation failed";
        return Status::object_manager_failed;
    }
    Record& stable = *records_.find(source_handle)->second;

    const auto constructed = ctor::construct(&stable.constructor,
        stable.character.identity, &stable.constructor_services,
        &result->constructor, error);
    if (constructed != ctor::Status::complete) {
        records_.erase(source_handle); // constructor owner already rolled back
        return constructed == ctor::Status::service_unavailable
            ? Status::service_unavailable : Status::constructor_failed;
    }
    if (character_kill_field_14e4_v1::character_constructor_completed(
            &stable.kill_field_14e4, stable.character.identity) !=
        character_kill_field_14e4_v1::Status::complete) {
        error = "Character+0x14e4 constructor field projection rejected its canonical Record";
        rollback_completed(stable);
        records_.erase(source_handle);
        return Status::constructor_failed;
    }

    manager::GameObject* manager_object = nullptr;
    result->object_manager_status = object_manager_.add_object(source_handle,
        stable.game_object, &manager_object);
    if (result->object_manager_status != manager::Status::ok) {
        rollback_completed(stable);
        records_.erase(source_handle);
        error = "ObjectManager rejected the constructed Character";
        return Status::object_manager_failed;
    }
    stable.object_registered = true;
    result->object_registered = true;

    bool appended = false;
    try {
        result->roster_status = roster_.enroll_after_add(roster_.context,
            &stable.character, false, &appended);
    } catch (...) {
        result->roster_status = -1;
    }
    stable.character_listed = result->character_listed = appended;
    if (result->roster_status != 0 || !appended) {
        error = "CharacterList enrollment failed after ObjectManager registration";
        if (appended) {
            std::size_t removed_characters = 0;
            int removal_status = -1;
            try {
                removal_status = roster_.remove_after_remove(roster_.context,
                    &stable.character, &removed_characters);
            } catch (...) {
                removal_status = -1;
            }
            if (removal_status != 0 || removed_characters == 0)
                return Status::cleanup_incomplete;
            stable.character_listed = result->character_listed = false;
        }
        bool removed = false;
        result->object_manager_status = object_manager_.remove_object(
            source_handle, &removed);
        if (result->object_manager_status == manager::Status::ok && removed) {
            stable.object_registered = false;
            result->object_registered = false;
            rollback_completed(stable);
            records_.erase(source_handle);
            return Status::roster_failed;
        }
        return Status::cleanup_incomplete; // preserve stable storage for retry
    }

    *output = &stable;
    return Status::complete;
}

Status Owner::retire(manager::SourceHandle source_handle, Result* result,
                     std::string& error) noexcept {
    if (result) *result = {};
    error.clear();
    auto found = records_.find(source_handle);
    if (found == records_.end()) return Status::not_found;
    Record& record = *found->second;
    copy_lifecycle_state(result, record);

    if (record.faery_script.stage() !=
            character_faery_script_session_v1::Stage::empty &&
        record.faery_script.stage() !=
            character_faery_script_session_v1::Stage::closed) {
        error = "Character cannot retire before its AISFaery session is closed";
        return Status::cleanup_incomplete;
    }
    if (record.pofaerie_physical ||
        record.source_pofaerie_identity) {
        error = "Character cannot retire before its POFaerie physical owner is detached";
        return Status::cleanup_incomplete;
    }

    if (record.character_listed) {
        std::size_t removed = 0;
        int status = -1;
        try {
            status = roster_.remove_after_remove(roster_.context,
                                                  &record.character, &removed);
        } catch (...) {
            status = -1;
        }
        if (status != 0 || removed == 0) {
            error = "CharacterList could not retire the Character";
            return Status::cleanup_incomplete;
        }
        record.character_listed = false;
        if (result) result->character_listed = false;
    }
    if (record.object_registered) {
        bool removed = false;
        const auto status = object_manager_.remove_object(source_handle, &removed);
        if (status != manager::Status::ok || !removed) {
            error = "ObjectManager could not retire the Character GameObject";
            return Status::cleanup_incomplete;
        }
        record.object_registered = false;
        if (result) result->object_manager_status = status;
    }
    rollback_completed(record);
    records_.erase(found);
    return Status::complete;
}

Status Owner::mark_source_properties_ready(manager::SourceHandle source_handle,
                                          Result* result,
                                          std::string& error) {
    if (result) *result = {};
    error.clear();
    auto found = records_.find(source_handle);
    if (found == records_.end()) return Status::not_found;
    Record& record = *found->second;
    record.source_properties_ready_ = true;
    copy_lifecycle_state(result, record);
    return Status::complete;
}

Status Owner::mark_script_created(manager::SourceHandle source_handle,
                                  std::string& error) noexcept {
    error.clear();
    auto* record = find(source_handle);
    if (!record) {
        error = "script-created Character handle is not owned by this factory";
        return Status::not_found;
    }
    record->source_script_created_539 = 1;
    return Status::complete;
}

Status Owner::bind_player_save_slot(manager::SourceHandle source_handle,
                                    gameplay_save::SaveRef* save,
                                    std::shared_ptr<void> lifetime,
                                    std::string& error) {
    error.clear();
    const auto found = records_.find(source_handle);
    if (found == records_.end()) return Status::not_found;
    Record& record = *found->second;
    if (!save || !lifetime || record.source_save_14e8 ||
        record.source_save_slot_kind != SourceSaveSlotKind::constructor_null ||
        record.nonplayer_init_post_selected || !save->identity || !save->save ||
        save->identity != reinterpret_cast<std::uintptr_t>(save->save) ||
        !save->loader || &save->loader->save() != save->save ||
        save->save->character() != record.character.identity ||
        save->quest_character_174 !=
            &save->save->source_quest_log_118().character_5c ||
        save->quest_character_114 !=
            &save->save->source_quest_log_b8().character_5c ||
        *save->quest_character_174 != record.character.identity ||
        *save->quest_character_114 != record.character.identity) {
        error = "Player Save must be the same Character's live Save/LoadOwner graph";
        return Status::invalid_argument;
    }
    record.source_save_14e8 = save;
    record.source_save_slot_kind = SourceSaveSlotKind::player_save;
    record.source_save_lifetime = std::move(lifetime);
    return Status::complete;
}

Status Owner::load_source_save_mask2(manager::SourceHandle source_handle,
                                     gameplay_save::Result* output,
                                     std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output) return Status::invalid_argument;
    const auto found = records_.find(source_handle);
    if (found == records_.end()) return Status::not_found;
    Record& record = *found->second;
    if (record.source_save_slot_kind == SourceSaveSlotKind::constructor_null ||
        (record.source_save_slot_kind == SourceSaveSlotKind::nonplayer_null &&
         record.source_save_14e8) ||
        (record.source_save_slot_kind == SourceSaveSlotKind::player_save &&
         (!record.source_save_14e8 || !record.source_save_lifetime))) {
        error = "Character +0x14e8 Save slot is not classified or has stale backing";
        return Status::service_unavailable;
    }
    if (record.source_save_mask2_attempted) {
        error = "Character SG_Load(2) was already attempted";
        return Status::init_post_failed;
    }
    if (record.source_save_slot_kind == SourceSaveSlotKind::player_save) {
        const auto* save = record.source_save_14e8;
        if (!save->identity || !save->save ||
            save->identity != reinterpret_cast<std::uintptr_t>(save->save) ||
            !save->loader || &save->loader->save() != save->save ||
            save->save->character() != record.character.identity ||
            save->quest_character_174 !=
                &save->save->source_quest_log_118().character_5c ||
            save->quest_character_114 !=
                &save->save->source_quest_log_b8().character_5c ||
            *save->quest_character_174 != record.character.identity ||
            *save->quest_character_114 != record.character.identity) {
            error = "Player Save association changed after Character binding";
            return Status::service_unavailable;
        }
    }
    record.source_save_mask2_attempted = true;
    try {
        gameplay_save::Runtime runtime(
            gameplay_save::Character{record.character.identity,
                                     &record.source_save_14e8});
        if (runtime.load(2, output, error) != gameplay_save::Status::complete ||
            output->captured_character != record.character.identity ||
            output->mask != 2 ||
            (record.source_save_slot_kind == SourceSaveSlotKind::nonplayer_null &&
             (output->captured_save || output->provider_calls || output->load_calls)) ||
            (record.source_save_slot_kind == SourceSaveSlotKind::player_save &&
             (output->captured_save != record.source_save_14e8->identity ||
              output->load_calls != 1))) {
            if (error.empty()) error = "Character SG_Load(2) owner receipt mismatch";
            return Status::init_post_failed;
        }
    } catch (...) {
        error = "Character SG_Load(2) runtime failed";
        return Status::init_post_failed;
    }
    record.source_save_mask2_complete = true;
    error.clear();
    return Status::complete;
}

Status Owner::run_init_post(manager::SourceHandle source_handle,
                            const LifecycleServices& services,
                            Result* result, std::string& error) {
    if (result) *result = {};
    error.clear();
    auto found = records_.find(source_handle);
    if (found == records_.end()) return Status::not_found;
    Record& record = *found->second;
    if (record.init_post_complete_) {
        copy_lifecycle_state(result, record);
        return Status::complete;
    }
    if (!record.source_properties_ready_) {
        error = "Character InitPost requires source properties and XML overrides to be ready";
        copy_lifecycle_state(result, record);
        return Status::properties_not_ready;
    }
    if (!services.init_post) {
        error = "Character InitPost lifecycle service is unavailable";
        copy_lifecycle_state(result, record);
        return Status::lifecycle_service_unavailable;
    }
    int status = -1;
    try {
        status = services.init_post(services.context, record, error);
    } catch (...) {
        error = "Character InitPost lifecycle service threw an exception";
    }
    if (status != 0) {
        if (error.empty()) error = "Character InitPost lifecycle service failed";
        copy_lifecycle_state(result, record);
        return Status::init_post_failed;
    }
    record.init_post_complete_ = true;
    error.clear();
    copy_lifecycle_state(result, record);
    return Status::complete;
}

Status Owner::run_init_final(manager::SourceHandle source_handle,
                             const LifecycleServices& services,
                             Result* result, std::string& error) {
    if (result) *result = {};
    error.clear();
    auto found = records_.find(source_handle);
    if (found == records_.end()) return Status::not_found;
    Record& record = *found->second;
    if (record.init_final_complete_) {
        copy_lifecycle_state(result, record);
        return Status::complete;
    }
    if (!record.init_post_complete_) {
        error = "Character InitFinal requires successful InitPost";
        copy_lifecycle_state(result, record);
        return Status::init_post_not_complete;
    }
    if (!services.init_final) {
        error = "Character InitFinal lifecycle service is unavailable";
        copy_lifecycle_state(result, record);
        return Status::lifecycle_service_unavailable;
    }
    int status = -1;
    try {
        status = services.init_final(services.context, record, error);
    } catch (...) {
        error = "Character InitFinal lifecycle service threw an exception";
    }
    if (status != 0) {
        if (error.empty()) error = "Character InitFinal lifecycle service failed";
        copy_lifecycle_state(result, record);
        return Status::init_final_failed;
    }
    record.init_final_complete_ = true;
    error.clear();
    copy_lifecycle_state(result, record);
    return Status::complete;
}

Record* Owner::find(manager::SourceHandle source_handle) noexcept {
    const auto found = records_.find(source_handle);
    return found == records_.end() ? nullptr : found->second.get();
}

const Record* Owner::find(manager::SourceHandle source_handle) const noexcept {
    const auto found = records_.find(source_handle);
    return found == records_.end() ? nullptr : found->second.get();
}

Record* Owner::find_character(std::uintptr_t identity) noexcept {
    if (!identity) return nullptr;
    for (auto& entry : records_) {
        if (entry.second && entry.second->character.identity == identity &&
            entry.second->game_object.identity == identity)
            return entry.second.get();
    }
    return nullptr;
}

const Record* Owner::find_character(std::uintptr_t identity) const noexcept {
    if (!identity) return nullptr;
    for (const auto& entry : records_) {
        if (entry.second && entry.second->character.identity == identity &&
            entry.second->game_object.identity == identity)
            return entry.second.get();
    }
    return nullptr;
}

} // namespace dh2::character_runtime_factory_v1
