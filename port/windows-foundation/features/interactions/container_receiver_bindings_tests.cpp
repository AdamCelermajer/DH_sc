#include "container_source_pipeline_v21.hpp"

#include "../../../level-world/canonical_point3d_globals_v1.hpp"
#include "../../../level-world/tests/destructible_data_fixture_v16.inc"
#include "../../../engine-animation/events.hpp"
#include "../../../engine-resources/resources.hpp"

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>

namespace {
bool assertion(void*, std::string&) { return true; }
void collect_event(const dh2::animation::TriggeredEvent* event, void* context) {
    assert(event && event->name);
    static_cast<std::vector<std::string>*>(context)->emplace_back(event->name);
}
std::string original_act1_data_desc() {
    std::string value;
    auto run_declaration = [&](std::map<std::string, std::string>& attrs) {
        if (attrs["name"] == "_prim_DestructibleContainer03") value = attrs["data_desc"];
    };
#include "../../../level-world/tests/destructible_declarations_v16.inc"
    return value;
}
}

int main() {
    using namespace dh::foundation::interactions;
    using namespace dh2::world;
    std::string error;
    OpenableContainerServicesV1 missing_loot_services;
    assert(!bind_container_live_loot_v104(missing_loot_services,
        std::shared_ptr<dh2::character::WorldLootGameplayV23>{},
        nullptr,
        std::shared_ptr<void>{}, {}, error));
    assert(error == "Interactions require initialized SAME WorldLootGameplayV23 owner" &&
           !missing_loot_services.drop_loot_table);
    OpenableAnimationBindingV21 atomic_openable_animation;
    auto absent_animator = [](std::shared_ptr<RetainedGenericAnimatorV21>&,
        std::shared_ptr<RetainedGameObjectVisualV1>&, std::string& e) {
        e = "test provider must not run during preconstruction";
        return false;
    };
    assert(!prepare_openable_source_pipeline_v21(missing_loot_services, absent_animator,
        std::shared_ptr<dh2::character::WorldLootGameplayV23>{}, nullptr,
        std::shared_ptr<void>{}, {}, atomic_openable_animation, error));
    assert(error == "Interactions require initialized SAME WorldLootGameplayV23 owner" &&
           !missing_loot_services.bind_timeline_callbacks &&
           !missing_loot_services.play_animation && !missing_loot_services.scene_flags &&
           !missing_loot_services.drop_loot_table && !atomic_openable_animation.slot);
    ContainerPresentationServicesV16 missing_breakable_loot_services;
    assert(!bind_container_live_loot_v104(missing_breakable_loot_services,
        std::shared_ptr<dh2::character::WorldLootGameplayV23>{},
        nullptr,
        std::shared_ptr<void>{}, {}, error));
    assert(error == "Interactions require initialized SAME WorldLootGameplayV23 owner" &&
           !missing_breakable_loot_services.drop_loot_table);
    DestructibleContainerServicesV16 missing_breakable_pipeline;
    DestructibleAnimationBindingV21 atomic_breakable_animation;
    assert(!prepare_destructible_source_pipeline_v21(missing_breakable_pipeline, absent_animator,
        std::shared_ptr<dh2::character::WorldLootGameplayV23>{}, nullptr,
        std::shared_ptr<void>{}, {}, atomic_breakable_animation, error));
    assert(error == "Interactions require initialized SAME WorldLootGameplayV23 owner" &&
           !missing_breakable_pipeline.bind_callbacks &&
           !missing_breakable_pipeline.common.play_animation &&
           !missing_breakable_pipeline.common.scene_flags &&
           !missing_breakable_pipeline.common.drop_loot_table &&
           !atomic_breakable_animation.slot);

    // Inspect the actual source chest BRES event and named-clip bounds. This
    // records the authored marker time instead of deriving it from map art or
    // scheduling a guessed delay.
    const std::string chest_bres_path =
        ".local-inputs/publication/checkpoint/reference/openable-container-v1/cache/go_chest_swamp.bdae";
    std::ifstream chest_file(chest_bres_path, std::ios::binary);
    assert(chest_file);
    std::vector<std::uint8_t> chest_bytes{
        std::istreambuf_iterator<char>(chest_file), std::istreambuf_iterator<char>()};
    dh2::resources::BresView chest_bres{};
    assert(dh2_bres_open(&chest_bres, chest_bytes.data(), chest_bytes.size()) ==
           dh2::resources::BresError::ok);
    dh2::animation::EventTrack chest_events;
    assert(chest_events.load(chest_bres, error));
    const auto opened_ms = dh2_events_time(&chest_events.view(), "opened");
    const auto fx_ms = dh2_events_time(&chest_events.view(), "fx");
    assert(opened_ms >= 0);
    std::int32_t activate_start = -1, activate_end = -1;
    for (std::uint32_t i = 0; i < dh2_bres_library_count(
             &chest_bres, dh2::resources::Library::animation_clip); ++i) {
        const auto* record = dh2_bres_library_item(
            &chest_bres, dh2::resources::Library::animation_clip,
            static_cast<std::int32_t>(i));
        assert(record);
        std::uint32_t name_offset{};
        std::memcpy(&name_offset, record, sizeof(name_offset));
        assert(name_offset < chest_bres.size);
        const auto* name = reinterpret_cast<const char*>(chest_bres.bytes + name_offset);
        const auto* end = static_cast<const char*>(std::memchr(name, 0, chest_bres.size - name_offset));
        assert(end);
        if (std::string(name, end) == "activate") {
            std::memcpy(&activate_start, record + 4, sizeof(activate_start));
            std::memcpy(&activate_end, record + 8, sizeof(activate_end));
        }
    }
    assert(activate_start >= 0 && activate_end >= activate_start &&
           opened_ms >= activate_start && opened_ms <= activate_end);
    dh2::animation::EventCursor marker_cursor;
    std::vector<std::string> delivered_markers;
    std::int32_t marker_delivery_ms = -1;
    for (std::int32_t ms = activate_start + 1; ms <= activate_end; ++ms) {
        assert(dh2_events_update(&chest_events.view(), &marker_cursor, ms - 1,
            ms, activate_start, activate_end, collect_event, &delivered_markers));
        if (!delivered_markers.empty()) { marker_delivery_ms = ms; break; }
    }
    assert(delivered_markers == std::vector<std::string>{"opened"});
    assert(marker_delivery_ms >= opened_ms && marker_delivery_ms <= opened_ms + 1);
    assert(dh2_events_update(&chest_events.view(), &marker_cursor, marker_delivery_ms,
        activate_end, activate_start, activate_end, collect_event, &delivered_markers));
    assert(delivered_markers == std::vector<std::string>{"opened"});
    auto table = std::make_shared<DestructibleContainerTableV16>();
    assert(table->load(destructible_group0_records_bin, sizeof destructible_group0_records_bin,
                       destructible_group0_names_bin, sizeof destructible_group0_names_bin, error));
    assert(table->size() == 37);

    auto lease = std::make_shared<int>(1);
    dh2::actor::RuntimeState runtime{};
    DestructibleContainerServicesV16 source_services;
    source_services.owner = lease;
    source_services.table = table;
    source_services.constant = [](const char* group, const char* key, std::int32_t& id, std::string&) {
        assert(std::string(group) == "v2QuestObjectiveType" &&
               std::string(key) == "DestroyGameObject");
        id = 17;
        return true;
    };
    source_services.raise_quest = [](const DestructibleQuestEventV16& event, std::string&) {
        assert(event.id == 17 && event.actor == 0x777);
        return true;
    };
    unsigned animator_provider_calls = 0;
    ContainerAnimatorProviderV21 missing_visual_provider =
        [&](std::shared_ptr<RetainedGenericAnimatorV21>&,
            std::shared_ptr<RetainedGameObjectVisualV1>&, std::string& e) {
            ++animator_provider_calls;
            e = "actual retained visual is not available in this receiver test";
            return false;
        };
    DestructibleAnimationBindingV21 destructible_animation;
    assert(prepare_destructible_animation_v21(source_services, missing_visual_provider,
                                               destructible_animation, error));
    assert(source_services.bind_callbacks && source_services.common.play_animation &&
           source_services.common.scene_flags && source_services.animation_count &&
           source_services.play_index && animator_provider_calls == 0);

    OpenableContainerServicesV1 openable_services;
    OpenableAnimationBindingV21 openable_animation;
    assert(prepare_openable_animation_v21(openable_services, missing_visual_provider,
                                           openable_animation, error));
    assert(openable_services.bind_timeline_callbacks && openable_services.play_animation &&
           openable_services.scene_flags && animator_provider_calls == 0);

    auto receiver = std::make_shared<CanonicalDestructibleContainerV16>(
        lease, runtime, GameObjectInitializationServicesV1{}, source_services);
    destructible_animation.attach(receiver);
    assert(destructible_animation.slot && destructible_animation.slot->lock() == receiver);
    receiver->base().class_name20() = "DestructibleContainer";

    dh2::actor::RuntimeState openable_runtime{};
    auto openable_receiver = std::make_shared<CanonicalOpenableContainerV1>(
        lease, openable_runtime, openable_services);
    openable_animation.attach(openable_receiver);
    assert(openable_animation.slot && openable_animation.slot->lock() == openable_receiver);
    assert(animator_provider_calls == 0);

    // This authored source row comes from the captured Swamp declaration set;
    // it is intentionally not inferred from the map preview or pot appearance.
    auto actor_fields = receiver->properties();
    CanonicalPropertyMapV1 property_map({nullptr, &canonical_vec3_origin_v1(), assertion});
    assert(property_map.init_properties(actor_fields, error));
    assert(property_map.load_defaults(actor_fields, error));
    const auto authored_data_desc = original_act1_data_desc();
    assert(authored_data_desc == "Swamp_Normal_DestructibleBarrel");
    assert(property_map.set_property(actor_fields, "data_desc", authored_data_desc.c_str(), error));

    CanonicalObjectManagerV1 manager({});
    auto published = receiver->canonical(receiver);
    dh2::target_providers::Handle16 handle{};
    assert(manager.add(published, "_prim_DestructibleContainer03",
                       "DestructibleContainer", 0, false, handle, error));
    const auto* same_published = manager.object(handle.key);
    assert(same_published && same_published->identity == receiver->base().identity());

    LiveReceiverServices live;
    assert(bind_destructible_receiver_v16(live,
        [receiver](std::uintptr_t identity, DestructibleBorrowV16& out, std::string& e) {
            if (identity != receiver->base().identity()) {
                e = "test resolver identity mismatch";
                return false;
            }
            out.receiver = receiver;
            return true;
        }, error));
    const auto handler = live.authored_receivers.find(1);
    assert(handler != live.authored_receivers.end());
    auto wrong_type_borrow = *same_published;
    const std::uint32_t wrong_type = 7;
    wrong_type_borrow.type_f4 = &wrong_type;
    assert(!handler->second(wrong_type_borrow, 0x777, error));
    assert(error.find("published GO_ID1") != std::string::npos);
    // Existing source owner reaches its true first missing dependency: a
    // retained visual/root required by source SetState. The bridge must not
    // report readiness or fabricate an animation/effect when that is absent.
    assert(!handler->second(*same_published, 0x777, error));
    assert(error.find("source SetState requires actual visual/root") != std::string::npos);

    bool refused_duplicate = !bind_destructible_receiver_v16(live,
        [](std::uintptr_t, DestructibleBorrowV16&, std::string&) { return true; }, error);
    assert(refused_duplicate && error.find("already bound") != std::string::npos);

    std::cout << "Source go_chest_swamp.bdae activate=[" << activate_start << ',' << activate_end
                 << "] opened=" << opened_ms << "ms dispatched=" << marker_delivery_ms
                 << "ms fx=" << fx_ms << "ms; "
                 "GO_ID1 dispatch borrowed the same published native DestructibleContainer; "
                 "openable/destructible V21+DropLoot composition fails atomically without the same ready WorldLootGameplayV23 owner; "
                 "source action stopped at the missing retained visual, with no fabricated readiness\n";
}
