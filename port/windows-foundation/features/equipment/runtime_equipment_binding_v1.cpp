#include "runtime_equipment_binding_v1.hpp"
#include "source_equipment_material_binding.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include <algorithm>
#include <set>

namespace dh::foundation::equipment_menu {
namespace {
bool same_owner(const std::weak_ptr<const void>& a, const std::weak_ptr<const void>& b) {
    return !a.owner_before(b) && !b.owner_before(a);
}
dh2::skinning::VisualAssetResultV6 source_visual_read(void* context, const char* uri,
        std::vector<std::uint8_t>& bytes, std::string& error) {
    try {
        bytes = read_content(*static_cast<const AssetCatalog*>(context), uri);
        return dh2::skinning::VisualAssetResultV6::found;
    } catch (const std::exception& e) {
        error = e.what();
        return dh2::skinning::VisualAssetResultV6::failed;
    }
}
bool valid_slots(const std::vector<std::string>& slots) {
    return slots.size() == 9 && std::set<std::string>(slots.begin(), slots.end()).size() == 9;
}
bool contains_id(const std::vector<std::string>& ids, const std::string& value) {
    return std::find(ids.begin(), ids.end(), value) != ids.end();
}
bool source_slot_for(const EquipmentBinding& binding, const std::vector<std::string>& slots,
                     unsigned& output, std::string& error) {
    if (binding.source_slot >= 0) {
        if (binding.source_slot >= 9) { error = "CharacterState source equipment slot is outside 0..8"; return false; }
        output = static_cast<unsigned>(binding.source_slot); error.clear(); return true;
    }
    const auto found = std::find(slots.begin(), slots.end(), binding.slot);
    if (found != slots.end()) { output = static_cast<unsigned>(found - slots.begin()); error.clear(); return true; }
    if (binding.slot == "main_hand") { output = 1; error.clear(); return true; }
    if (binding.slot == "off_hand") { output = 2; error.clear(); return true; }
    error = "CharacterState equipment binding needs an explicit source_slot or source slot name";
    return false;
}
}

struct RuntimeEquipmentBindingV1::Impl {
    CombatSession* session{};
    CharacterState* character{};
    const AssetCatalog* body_assets{};
    const AssetCatalog* weapon_assets{};
    const OriginalPropertyDatabase* database{};
    ActorId actor_id = invalid_actor_id;
    PlayableActorWorld* world{};
    ActorState* actor{};
    const OriginalCombatProperties* combat{};
    const CharacterVisual* visual{};
    const dh2::scene::Scene* scene{};
    std::weak_ptr<const void> session_lease;
    RuntimeEquipmentOptionsV1 options;
    dh2::data::ItemTable items;
    std::unique_ptr<dh2::skinning::VisualSkinOwnerV6> source_skin;
    SourceEquipmentImageLeaseV1 source_skin_image;
    EquipmentAttachmentSet attachments;
    std::unique_ptr<EquipmentAdapter> adapter;
    std::unique_ptr<Presenter> presenter;
    std::uint64_t render_revision{};
    bool render_change_pending{};

    bool current(std::string& error) const {
        const auto fail = [&](const char* text) { error = text; return false; };
        // session is a borrowed raw address. Test the independently retained
        // control block first, before even reading that address after owner
        // teardown or detach.
        if (session_lease.expired())
            return fail("Runtime equipment CombatSession actor lease has expired");
        if (!session || !character || !world || !actor || !combat || !visual || !scene)
            return fail("Runtime equipment binding is incomplete");
        const auto current_lease = session->actor_binding_lease();
        if (current_lease.expired() || !same_owner(current_lease, session_lease))
            return fail("Runtime equipment binding requires the same live CombatSession actor lease");
        if (session->player_id() != actor_id || session->world() != world ||
            session->actor(actor_id) != actor || world->find_actor(actor_id) != actor ||
            world->combat_properties(actor_id) != combat ||
            session->retained_actor_visual_borrow(actor_id) != visual ||
            visual->retained_scene_borrow() != scene)
            return fail("Runtime equipment binding is stale after actor/session/Scene replacement");
        return same_equipment_projection(error);
    }

    const dh2::data::Item* item(const std::string& definition) const {
        return dh2::data::item(items, dh2::data::item_id(items, definition));
    }

    bool same_equipment_projection(std::string& error) const {
        std::map<unsigned, std::pair<std::string, std::string>> projected, live;
        for (const auto& binding : character->equipment) {
            unsigned slot{};
            if (!source_slot_for(binding, options.slots, slot, error)) return false;
            const auto found = std::find_if(character->inventory.begin(), character->inventory.end(),
                [&](const auto& row) { return row.instance_id == binding.item_instance_id; });
            if (found == character->inventory.end() || !item(found->definition_id)) {
                error = "Shared CharacterState equipment references an unavailable source instance";
                return false;
            }
            if (!projected.emplace(slot, std::make_pair(binding.item_instance_id,
                                                        found->definition_id)).second) {
                error = "Shared CharacterState duplicates a source equipment slot";
                return false;
            }
        }
        for (const auto& binding : actor->equipment) {
            if (!binding.item_instance_id) {
                error = "Runtime equipment cannot project a player slot without an actual item instance ID";
                return false;
            }
            unsigned slot = 9;
            const auto slot_name = std::find(options.slots.begin(), options.slots.end(), binding.slot);
            if (slot_name != options.slots.end()) slot = static_cast<unsigned>(slot_name-options.slots.begin());
            else if (binding.slot == "main_hand") slot = 1;
            else if (binding.slot == "off_hand") slot = 2;
            else {
                const auto state_binding = std::find_if(character->equipment.begin(), character->equipment.end(),
                    [&](const auto& projected_binding) {
                        return projected_binding.item_instance_id == *binding.item_instance_id;
                    });
                if (state_binding == character->equipment.end() ||
                    !source_slot_for(*state_binding, options.slots, slot, error)) {
                    error = "CombatSession player equipment has no explicit source slot projection";
                    return false;
                }
            }
            const auto* row = item(binding.definition_id);
            if (!row || !live.emplace(slot, std::make_pair(*binding.item_instance_id,
                                                           binding.definition_id)).second) {
                error = "CombatSession player equipment has an invalid or duplicate source slot";
                return false;
            }
        }
        if (live != projected) {
            error = "CombatSession player equipment and shared CharacterState projection differ";
            return false;
        }
        error.clear();
        return true;
    }

    bool publish(const ActorState& candidate_actor, const OriginalCombatProperties& candidate,
                 std::string& error) {
        if (session->world() != world || session->actor(actor_id) != actor ||
            world->combat_properties(actor_id) != combat) {
            error = "Runtime equipment publication lost its same-player world owner";
            return false;
        }
        const auto* live_traits = world->traits(actor_id);
        if (!live_traits) { error = "Runtime equipment player traits are unavailable"; return false; }
        PlayableActorTraits traits = *live_traits;
        traits.main_item.reset();
        std::array<const dh2::data::ItemRecord164*, 2> facts{};
        for (const auto& binding : candidate_actor.equipment) {
            for (unsigned hand = 0; hand < 2; ++hand) {
                const unsigned slot = hand + 1;
                if (binding.slot != options.slots[slot]) continue;
                if (facts[hand]) { error = "Runtime equipment candidate duplicates a main/off source slot"; return false; }
                const auto* row = item(binding.definition_id);
                if (!row) { error = "Runtime equipment candidate references a missing ItemTable row"; return false; }
                facts[hand] = &row->record;
            }
        }
        if (facts[0]) traits.main_item = *facts[0];
        OriginalCombatProperties publication = candidate;
        if (!world->update_combat_properties(actor_id, std::move(publication), std::move(traits), error))
            return false;
        error.clear();
        return true;
    }

    bool stage_visuals(const CharacterState& next, const ActorState&,
                       std::vector<EquipmentVisualDefinition>& definitions,
                       std::string& error) {
        if (!visual || !source_skin || visual->retained_scene_borrow() != scene) {
            error = "Runtime equipment source appearance lost the exact session Scene";
            return false;
        }
        SourceEquipmentAppearancePlan plan;
        if (!prepare_source_equipment_appearance(next, items, options.slots, *source_skin, plan, error))
            return false;
        std::vector<EquipmentVisualDefinition> next_definitions;
        for (const auto& step : plan.steps) {
            if (step.weapon_mode < 0 || !step.equipped) continue;
            EquipmentVisualDefinition definition;
            definition.id = step.category;
            definition.model_uri = step.weapon_uri;
            definition.anchor_name = step.anchor;
            next_definitions.push_back(std::move(definition));
        }
        definitions = std::move(next_definitions);
        error.clear();
        return true;
    }

    bool refresh_source_appearance(std::string& error) {
        if (!source_skin || !visual || visual->retained_scene_borrow() != scene) {
            error = "Runtime equipment source appearance lost the exact session Scene";
            return false;
        }
        SourceEquipmentAppearancePlan plan;
        if (!prepare_source_equipment_appearance(*character, items, options.slots,
                *source_skin, plan, error)) return false;
        return apply_source_equipment_appearance(*source_skin, plan,
                options.source_appearance_debug, error);
    }
};

RuntimeEquipmentBindingV1::RuntimeEquipmentBindingV1() = default;
RuntimeEquipmentBindingV1::~RuntimeEquipmentBindingV1() = default;

bool RuntimeEquipmentBindingV1::bind(CombatSession& session, CharacterState& character,
        const AssetCatalog& item_assets, const AssetCatalog& body_assets,
        const AssetCatalog& weapon_assets, const OriginalPropertyDatabase& database,
        RuntimeEquipmentOptionsV1 options, std::string& error) {
    error.clear();
    if (impl_) { error = "Runtime equipment binding can only be initialized once"; return false; }
    if (!valid_slots(options.slots)) { error = "Runtime equipment requires nine distinct source slots"; return false; }
    if (options.table_root.empty()) { error = "Runtime equipment source ItemTable root is required"; return false; }
    try {
        auto next = std::make_unique<Impl>();
        next->session = &session; next->character = &character;
        next->body_assets = &body_assets; next->weapon_assets = &weapon_assets;
        next->database = &database; next->options = std::move(options);
        next->actor_id = session.player_id(); next->world = session.world();
        next->actor = session.actor(next->actor_id);
        next->combat = next->world ? next->world->combat_properties(next->actor_id) : nullptr;
        next->visual = session.retained_actor_visual_borrow(next->actor_id);
        next->scene = next->visual ? next->visual->retained_scene_borrow() : nullptr;
        next->session_lease = session.actor_binding_lease();
        if (next->session_lease.expired() || !next->world || !next->actor || !next->combat ||
            !next->visual || !next->scene) {
            error = "Runtime equipment requires the live CombatSession player, world sheets, and retained visual";
            return false;
        }
        const bool character_names_source_profile =
            contains_id(database.characters.names, character.class_id);
        const bool actor_names_source_profile =
            contains_id(database.characters.names, next->actor->definition_id);
        const bool source_profile_match = actor_names_source_profile &&
            character.class_id == next->actor->definition_id;
        const bool legacy_class_match = !character_names_source_profile &&
            contains_id(database.classes.names, character.class_id) &&
            character.class_id == next->actor->class_id;
        if (!source_profile_match && !legacy_class_match) {
            error = "Shared CharacterState class must match the player's source definition or a validated legacy ClassTables ID";
            return false;
        }
        if (character.inventory.size() > character_collection_limit) {
            error = "Shared CharacterState inventory exceeds source menu limits";
            return false;
        }
        std::set<std::string> instance_ids;
        for (const auto& instance : character.inventory) {
            if (instance.instance_id.empty() || !instance.quantity ||
                !instance_ids.insert(instance.instance_id).second || instance.definition_id.empty()) {
                error = "Shared CharacterState has an invalid source ItemTable instance";
                return false;
            }
        }
        const auto read_table = [&](const char* suffix) {
            return item_assets.read(std::filesystem::path(next->options.table_root) / suffix);
        };
        const auto records = read_table("loot_table_pyarray.bin");
        const auto names = read_table("loot_table_pyarraynames.bin");
        const auto schema = read_table("loot_table_pystructnames.bin");
        if (!dh2::data::load_items({records.data(), records.size()}, {names.data(), names.size()},
                {schema.data(), schema.size()}, next->items, error)) return false;
        for (const auto& instance : character.inventory)
            if (!next->item(instance.definition_id)) {
                error = "Shared CharacterState item is absent from loaded source ItemTable";
                return false;
            }

        if (!bind_source_equipment_render_skin(body_assets, *next->visual,
                {const_cast<AssetCatalog*>(&body_assets), source_visual_read},
                next->source_skin, next->source_skin_image, error))
            return false;
        EquipmentAdapterOptions adapter_options;
        adapter_options.slots = next->options.slots;
        adapter_options.online_requirements_bypass = next->options.online_requirements_bypass;
        adapter_options.dual_wield = next->options.dual_wield;
        adapter_options.one_hand_two_hander = next->options.one_hand_two_hander;
        adapter_options.powers = std::move(next->options.powers);
        // SortByValueAndClass name tie-break uses the same localized ItemName provider as the menu rows.
        adapter_options.item_name = [state = next.get()](const InventoryItem& i, const dh2::data::Item& d,
                std::string& name, std::string& e) {
            return state->options.menu.item_name ? state->options.menu.item_name(i, d, name, e) : true;
        };
        adapter_options.assets = &weapon_assets;
        adapter_options.body = next->visual;
        adapter_options.attachments = &next->attachments;
        adapter_options.visuals = [state = next.get()](const CharacterState& c, const ActorState& a,
                std::vector<EquipmentVisualDefinition>& defs, std::string& e) {
            return state->stage_visuals(c, a, defs, e);
        };
        adapter_options.publish_combat = [state = next.get()](const ActorState& actor_candidate,
                const OriginalCombatProperties& candidate, std::string& e) {
            return state->publish(actor_candidate, candidate, e);
        };
        next->adapter = std::make_unique<EquipmentAdapter>(character, *next->actor, *next->combat,
            next->items, database, std::move(adapter_options));
        next->options.menu.slots = next->options.slots;
        // IsEquippableBy class gate: only a source CharacterTable class row (legacy profiles are unrestricted).
        if (actor_names_source_profile) next->options.menu.player_class_id = next->actor->definition_id;
        next->options.menu.online_requirements_bypass = next->options.online_requirements_bypass;
        next->presenter = std::make_unique<Presenter>(character, next->items,
            next->combat->sheets, *next->adapter, next->options.menu);
        if (!next->current(error)) return false;
        // Rebuild the session's render model from the already-restored shared
        // state. This intentionally bypasses EquipmentAdapter::refresh(): a
        // restore bind must not recalculate or mutate inventory/properties.
        std::vector<EquipmentVisualDefinition> initial_definitions;
        if (!next->stage_visuals(character, *next->actor, initial_definitions, error) ||
            !next->attachments.load(weapon_assets, initial_definitions, error) ||
            !next->attachments.update(*next->visual, error)) return false;
        if (!next->refresh_source_appearance(error)) return false;
        next->render_revision = 1;
        next->render_change_pending = true;
        impl_ = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
}

bool RuntimeEquipmentBindingV1::ready(std::string& error) const {
    if (!impl_) { error = "Runtime equipment binding is not initialized"; return false; }
    return impl_->current(error);
}
bool RuntimeEquipmentBindingV1::uses_character_state(const CharacterState& character,
        std::string& error) const {
    if (!ready(error)) return false;
    if (impl_->character != &character) {
        error = "Runtime equipment page must borrow the binding's same CharacterState";
        return false;
    }
    error.clear(); return true;
}

const dh2::data::ItemTable* RuntimeEquipmentBindingV1::item_table(std::string& error) const {
    if (!ready(error)) return nullptr;
    error.clear(); return &impl_->items;
}
const dh2::data::Item* RuntimeEquipmentBindingV1::item_for_instance(const std::string& id,
        std::string& error) const {
    if (!ready(error)) return nullptr;
    const auto found = std::find_if(impl_->character->inventory.begin(), impl_->character->inventory.end(),
        [&](const auto& row) { return row.instance_id == id; });
    if (found == impl_->character->inventory.end()) { error = "Runtime equipment instance is not owned"; return nullptr; }
    const auto* result = impl_->item(found->definition_id);
    if (!result) { error = "Runtime equipment instance definition is absent from source ItemTable"; return nullptr; }
    error.clear(); return result;
}
bool RuntimeEquipmentBindingV1::select_slot(unsigned slot, std::string& error) {
    return ready(error) && impl_->presenter->select_slot(slot, error);
}
bool RuntimeEquipmentBindingV1::select_instance(const std::string& id, std::string& error) {
    return ready(error) && impl_->presenter->select_instance(id, error);
}
bool RuntimeEquipmentBindingV1::selected_items(std::vector<OwnedSelection>& out, std::string& error) const {
    return ready(error) && impl_->presenter->view_for_selected_slot(out, error);
}
bool RuntimeEquipmentBindingV1::frame(character_menu::Frame& frame, std::string& error) const {
    return ready(error) && impl_->presenter->frame(frame, error);
}
Presenter* RuntimeEquipmentBindingV1::presenter_for_composition(std::string& error) const {
    if (!ready(error)) return nullptr;
    error.clear(); return impl_->presenter.get();
}
NativeEquipmentActions RuntimeEquipmentBindingV1::menu_actions(std::string& error) {
    if (!ready(error)) return {};
    NativeEquipmentActions actions;
    actions.equip = [this](const std::string& id, unsigned slot, std::string& action_error) {
        return equip_to_slot(id, slot, action_error);
    };
    actions.unequip = [this](unsigned slot, std::string& action_error) {
        return unequip(slot, action_error);
    };
    error.clear();
    return actions;
}
bool RuntimeEquipmentBindingV1::equip_selected(std::string& error) {
    if (!ready(error)) return false;
    if (!impl_->presenter->equip_selected(error)) return false;
    ++impl_->render_revision; impl_->render_change_pending = true;
    if (!impl_->refresh_source_appearance(error)) return false;
    error.clear(); return true;
}
bool RuntimeEquipmentBindingV1::unequip_selected_slot(std::string& error) {
    if (!ready(error)) return false;
    if (!impl_->presenter->unequip_selected_slot(error)) return false;
    ++impl_->render_revision; impl_->render_change_pending = true;
    if (!impl_->refresh_source_appearance(error)) return false;
    error.clear(); return true;
}
bool RuntimeEquipmentBindingV1::equip_to_slot(const std::string& id, unsigned slot, std::string& error) {
    if (!ready(error)) return false;
    if (!impl_->adapter->equip_to_slot(id, slot, error)) return false;
    ++impl_->render_revision; impl_->render_change_pending = true;
    if (!impl_->refresh_source_appearance(error)) return false;
    error.clear(); return true;
}
bool RuntimeEquipmentBindingV1::auto_equip(const std::string& id, std::string& error) {
    if (!ready(error)) return false;
    if (!impl_->adapter->auto_equip(id, error)) return false;
    ++impl_->render_revision; impl_->render_change_pending = true;
    if (!impl_->refresh_source_appearance(error)) return false;
    error.clear(); return true;
}
bool RuntimeEquipmentBindingV1::auto_equip_slot(unsigned slot, std::string& error) {
    if (!ready(error)) return false;
    if (!impl_->adapter->auto_equip_slot(slot, error)) return false;
    ++impl_->render_revision; impl_->render_change_pending = true;
    if (!impl_->refresh_source_appearance(error)) return false;
    error.clear(); return true;
}
bool RuntimeEquipmentBindingV1::auto_equip_all(std::string& error) {
    if (!ready(error)) return false;
    if (!impl_->adapter->auto_equip_all(error)) return false;
    ++impl_->render_revision; impl_->render_change_pending = true;
    if (!impl_->refresh_source_appearance(error)) return false;
    error.clear(); return true;
}
bool RuntimeEquipmentBindingV1::unequip(unsigned slot, std::string& error) {
    if (!ready(error)) return false;
    if (!impl_->adapter->unequip(slot, error)) return false;
    ++impl_->render_revision; impl_->render_change_pending = true;
    if (!impl_->refresh_source_appearance(error)) return false;
    error.clear(); return true;
}
bool RuntimeEquipmentBindingV1::sample_render_pose(std::string& error) {
    if (!ready(error)) return false;
    std::vector<Mat4> previous;
    previous.reserve(impl_->attachments.attachments().size());
    for (const auto& attachment : impl_->attachments.attachments())
        previous.push_back(attachment.socket_world);
    if (!impl_->attachments.update(*impl_->visual, error)) return false;
    const auto& current = impl_->attachments.attachments();
    bool changed = previous.size() != current.size();
    for (std::size_t i = 0; !changed && i < current.size(); ++i)
        changed = previous[i] != current[i].socket_world;
    if (changed) { ++impl_->render_revision; impl_->render_change_pending = true; }
    error.clear(); return true;
}
bool RuntimeEquipmentBindingV1::with_preview_borrow(
        const RuntimeEquipmentPreviewBorrowCallbackV1& callback, std::string& error) {
    if (!impl_) { error = "Runtime equipment binding is not initialized"; return false; }
    // Retain the exact lease token before any raw Session access. current()
    // repeats the expiry check and validates Session/actor/world/Scene identity.
    const auto lease = impl_->session_lease.lock();
    if (!lease) { error = "Runtime equipment CombatSession actor lease has expired"; return false; }
    if (!callback) { error = "Equipment preview callback is required"; return false; }
    if (!impl_->current(error)) return false;
    if (!impl_->source_skin || impl_->visual->retained_scene_borrow() != impl_->scene) {
        error = "Equipment preview lost its source skin owner or retained Scene";
        return false;
    }
    // This refresh reads the already-advanced same CharacterVisual pose. It
    // neither advances the visual clock nor mutates Session animation state.
    std::vector<Mat4> previous_sockets;
    previous_sockets.reserve(impl_->attachments.attachments().size());
    for (const auto& attachment : impl_->attachments.attachments())
        previous_sockets.push_back(attachment.socket_world);
    if (!impl_->attachments.update(*impl_->visual, error)) return false;
    const auto& current_attachments = impl_->attachments.attachments();
    bool attachment_pose_changed = previous_sockets.size() != current_attachments.size();
    for (std::size_t i = 0; !attachment_pose_changed && i < current_attachments.size(); ++i)
        attachment_pose_changed = previous_sockets[i] != current_attachments[i].socket_world;
    if (attachment_pose_changed) {
        ++impl_->render_revision;
        impl_->render_change_pending = true;
    }
    const std::vector<dh2::skinning::VisualDrawViewV32>* views = nullptr;
    if (!impl_->source_skin->draw_views(views, error) || !views || views->empty()) {
        if (error.empty()) error = "Equipment preview source skin owner has no drawable body views";
        return false;
    }
    if (!impl_->current(error)) return false;
    RuntimeEquipmentPreviewBorrowV1 borrow;
    borrow.revision = impl_->render_revision;
    borrow.actor_id = impl_->actor_id;
    borrow.class_id = impl_->character->class_id;
    borrow.visual = impl_->visual;
    borrow.same_scene = impl_->scene;
    borrow.source_views = views;
    borrow.attachments = &impl_->attachments;
    try {
        const bool accepted = callback(borrow, error);
        if (!accepted && error.empty()) error = "Equipment preview consumer rejected the synchronous source borrow";
        if (accepted) error.clear();
        return accepted;
    } catch (const std::exception& e) {
        error = std::string("Equipment preview callback threw: ") + e.what();
        return false;
    } catch (...) {
        error = "Equipment preview callback threw an unknown exception";
        return false;
    }
}
bool RuntimeEquipmentBindingV1::with_preview_packets(
        const SourceEquipmentRenderServicesV1& services,
        const RuntimeEquipmentPreviewPacketsCallbackV1& callback,
        std::string& error) {
    if (!callback) { error = "Equipment preview packet callback is required"; return false; }
    if (!impl_) { error = "Runtime equipment binding is not initialized"; return false; }
    const auto lease = impl_->session_lease.lock();
    if (!lease) {
        error = "Runtime equipment CombatSession actor lease has expired";
        return false;
    }
    if (!impl_->current(error)) return false;
    if (!impl_->source_skin_image.retention || !impl_->source_skin_image.bytes ||
        !impl_->source_skin_image.byte_count ||
        impl_->source_skin_image.image.bytes != impl_->source_skin_image.bytes ||
        impl_->source_skin_image.image.size != impl_->source_skin_image.byte_count ||
        impl_->source_skin_image.material_scene == nullptr) {
        error = "Equipment preview requires its pinned source body image lease";
        return false;
    }
    return with_preview_borrow([&](const RuntimeEquipmentPreviewBorrowV1& borrowed,
                                   std::string& callback_error) {
        EquipmentPreviewSourceV1 source;
        source.revision = borrowed.revision;
        source.actor_id = borrowed.actor_id;
        source.class_id = borrowed.class_id;
        source.visual = borrowed.visual;
        source.same_scene = borrowed.same_scene;
        source.source_views = borrowed.source_views;
        source.attachments = borrowed.attachments;

        RuntimeEquipmentPreviewPacketsV1 frame;
        if (!compose_runtime_equipment_preview_frame_v1(source, frame.presentation,
                                                        callback_error)) return false;
        SourceEquipmentRenderBridgeV1 renderer(*impl_->source_skin,
            impl_->source_skin_image, services);
        if (!renderer.prepare(frame.packets, callback_error)) return false;
        if (!frame.packets || frame.packets->packets.empty()) {
            callback_error = "Equipment preview source renderer produced no actual draw packets";
            return false;
        }
        try {
            const bool accepted = callback(frame, callback_error);
            if (!accepted && callback_error.empty())
                callback_error = "Equipment preview packet consumer rejected the source frame";
            return accepted;
        } catch (const std::exception& e) {
            callback_error = std::string("Equipment preview packet callback threw: ") + e.what();
            return false;
        } catch (...) {
            callback_error = "Equipment preview packet callback threw an unknown exception";
            return false;
        }
    }, error);
}
bool RuntimeEquipmentBindingV1::take_render_change(RuntimeEquipmentRenderChangeV1& out,
        std::string& error) {
    if (!ready(error)) return false;
    if (!impl_->render_change_pending) { error.clear(); return false; }
    out.revision = impl_->render_revision; out.actor_id = impl_->actor_id;
    out.same_session_visual = impl_->visual; out.same_scene = impl_->scene;
    out.attachments = &impl_->attachments;
    impl_->render_change_pending = false;
    error.clear(); return true;
}

} // namespace dh::foundation::equipment_menu
