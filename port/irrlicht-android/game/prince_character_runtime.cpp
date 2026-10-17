#include "prince_character_runtime.hpp"

#include "prince_actor.hpp"
#include "../../asset-payloads/payloads.hpp"
#include "../../engine-animation/animation_registration.hpp"
#include "../../game-data/animation_bank.hpp"
#include "../../game-data/animation_tables.hpp"
#include "../../game-data/class_tables.hpp"
#include "../../game-data/data.hpp"
#include "../../game-data/properties.hpp"
#include "../../level-world/actor_blended_playback.hpp"
#include "../../level-world/character_coordinator.hpp"
#include "../../level-world/character_scene.hpp"
#include "../../level-world/character_stance.hpp"
#include "../../level-world/move_state.hpp"
#include "../../scene-materials/scene.hpp"
#include "../../level-world/visual_motion.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>

namespace dh2::irrlicht_game {
namespace {

constexpr std::uintptr_t kCharacterOwner = 0x4952524c5357414dull;
constexpr char kAnimationAssetRoot[] = "dh2/prince-animation/";
constexpr char kPlayerName[] = "KnightPlayerBase";

bool read(PrinceAssetReader reader, void* context, const std::string& path,
          std::vector<std::uint8_t>& output, std::string& error) {
    if (!reader || !reader(context, path.c_str(), &output, &error) ||
        output.empty()) {
        if (error.empty()) error = "Prince source asset is absent: " + path;
        return false;
    }
    return true;
}

int source_sequence_id(const data::AnimationTables& tables, int table,
                       const char* name) {
    const auto* sequence = data::animation_state(tables, table, name);
    return sequence ? static_cast<int>(sequence - tables.sequences.data()) : -1;
}

bool recover_unbound_clip_range(const std::uint8_t* bytes, std::size_t size,
                               animation::Player& player, std::string& error) {
    resources::BresView view{};
    if (dh2_bres_open(&view, bytes, size) != resources::BresError::ok) {
        error = "source clip BRES could not be reopened for its timeline";
        return false;
    }
    const auto accessors = dh2_bres_library_count(&view, resources::Library::animation);
    const auto segments = dh2_animation_segments(&view);
    if (!accessors && !segments) {
        // Some bank resources are intentionally registered with no scene
        // animation payload (for example a shared effect/actor clip). Keep the
        // registration identity; the source mesh has no key timeline to sample.
        player.start = 0;
        player.end = 0;
        return true;
    }
    if (!accessors || !segments || segments > 256) {
        error = "source clip has no bounded animation time accessors";
        return false;
    }
    std::int32_t start = 0, end = 0;
    for (std::uint32_t segment = 0; segment < segments; ++segment) {
        assets::Animation accessor{};
        if (dh2_animation_open(&accessor, &view, 0,
                               static_cast<std::int32_t>(segment)) != assets::Error::ok ||
            accessor.segment_end <= accessor.segment_start ||
            (segment && accessor.segment_start != end)) {
            error = "source unbound clip segments are invalid or not contiguous";
            return false;
        }
        if (!segment) start = accessor.segment_start;
        end = accessor.segment_end;
    }
    player.start = start;
    player.end = end;
    return true;
}

} // namespace

struct PrinceCharacterRuntime::Impl {
    PrinceActor* actor = nullptr;
    PrinceAssetReader reader = nullptr;
    void* reader_context = nullptr;
    data::Dictionary clip_table;
    data::AnimationTables animation_tables;
    data::AnimationBank animation_bank;
    actor::ClipBank clips;
    data::AnimationRandom random{};
    actor::BlendedPlayback playback;
    character::Coordinator coordinator{kCharacterOwner};
    data::CharacterTable character_table;
    data::ClassTables class_table;
    data::PropertyRules property_rules;
    data::PropertyState player_properties;
    character::Facts base_facts{};
    PrinceCharacterBodyServices body_services{};
    std::array<float, 3> heading{};
    std::array<float, 3> owner_position{};
    std::string callback_error;
    std::uint32_t idle_common_updates = 0;
    std::uint32_t external_state_events = 0;
    std::uint32_t playback_events = 0;
    std::uint32_t timeline_less_resources = 0;
    bool body_services_bound = false;
    bool initialized = false;

    static character::Facts facts_callback(void* context) {
        const auto& self = *static_cast<Impl*>(context);
        auto result = self.base_facts;
        std::copy(self.heading.begin(), self.heading.end(), result.heading);
        result.is_at_destination = 1;
        result.following_path = 0;
        return result;
    }

    static void service_callback(void* context, character::State* state,
                                 const character::Request* request) {
        auto& self = *static_cast<Impl*>(context);
        if (!state || state != &self.coordinator.state || !request ||
            !self.initialized || !self.callback_error.empty()) return;
        std::string error;
        switch (request->service) {
        case character::stop:
            self.heading = {};
            state->heading_active = 0;
            self.coordinator.refresh_facts();
            if (state->body_present &&
                (!self.body_services_bound || !self.body_services.stop ||
                 !self.body_services.stop(self.body_services.context, error))) {
                self.callback_error = error.empty()
                    ? "Source Character Stop body service failed"
                    : "Source Character Stop body service failed: " + error;
            }
            break;
        case character::pin:
        case character::unpin: {
            const auto callback = request->service == character::pin
                ? self.body_services.pin : self.body_services.unpin;
            if (!state->body_present || !self.body_services_bound || !callback ||
                !callback(self.body_services.context, error)) {
                const char* action = request->service == character::pin ? "Pin" : "Unpin";
                self.callback_error = error.empty()
                    ? std::string("Source Character ") + action + " body service failed"
                    : std::string("Source Character ") + action + " body service failed: " + error;
            }
            break;
        }
        case character::set_animation:
            state->current_animation = request->argument[0];
            if (!self.playback.start(self.animation_tables, state->current_animation,
                    self.random, self.clips, self.actor->visual_binding(),
                    self.actor->scene(), 1.0f, error))
                self.callback_error = "Source Character animation selection failed: " + error;
            break;
        case character::set_speed:
            if (!self.playback.set_speed(request->scalar, error))
                self.callback_error = "Source Character animation speed failed: " + error;
            break;
        case character::swap_animation:
            if (!self.playback.swap(self.animation_tables, request->argument[0],
                    request->argument[1], self.random, self.clips,
                    self.actor->visual_binding(), self.actor->scene(),
                    state->cached_speed, error))
                self.callback_error = "Source Character animation swap failed: " + error;
            if (self.callback_error.empty() && !self.playback.scheduler.frames().empty())
                state->current_animation = self.playback.scheduler.frames().front().sequence;
            break;
        case character::stop_loop:
            self.playback.stop_loop(false);
            break;
        case character::start_timer:
            if (self.coordinator.start_timer(
                    static_cast<std::uint32_t>(request->argument[0]),
                    request->argument[1], request->argument[2],
                    static_cast<std::uintptr_t>(request->identity)) < 0)
                self.callback_error = "Source Character timer could not be started";
            break;
        case character::set_heading: {
            std::memcpy(self.heading.data(), request->argument,
                        sizeof(self.heading));
            state->heading_active = request->scalar != 0.0f;
            self.coordinator.refresh_facts();
            break;
        }
        case character::raise_event: {
            const auto event = static_cast<std::uint32_t>(request->argument[0]);
            if (event == 0x3f || event == 0x2a || event == 0x2b || event == 0x2c) {
                if (self.coordinator.event(event, request->identity) < 0)
                    self.callback_error = "Source Character event dispatch failed";
            } else {
                // 0x1d is the recovered state-change notification sent to the
                // external game layer. It is recorded here; no AI/UI consumer
                // is fabricated by this SWAMP source slice.
                ++self.external_state_events;
            }
            break;
        }
        case character::idle_common_update:
            // The source FSM requests this external Character/AI update on
            // every Idle tick. Keep the missing game-layer callback explicit.
            ++self.idle_common_updates;
            break;
        default:
            self.callback_error = "Character service is outside the SWAMP locomotion slice: " +
                                  std::to_string(request->service);
            break;
        }
    }

    static void playback_event(void* context, actor::BlendedPlayback& source,
                               const actor::BlendedPlaybackEvent& event) {
        auto& self = *static_cast<Impl*>(context);
        ++self.playback_events;
        if (event.event.handoff.event_id == 0x22 &&
            self.coordinator.state.current == 5 &&
            self.coordinator.event(0x22, 0) < 0) {
            self.callback_error = "Source Character sequence-close event failed";
        }
        (void)source;
    }

    bool fail_callback(std::string& error) {
        if (callback_error.empty()) return false;
        error = callback_error;
        return true;
    }

    bool load_data(PrinceAssetReader asset_reader, void* context,
                   const char* name, std::vector<std::uint8_t>& bytes,
                   std::string& error) {
        return read(asset_reader, context,
                    std::string(kAnimationAssetRoot) + "data/" + name,
                    bytes, error);
    }

    bool initialize(PrinceActor& prince, PrinceAssetReader asset_reader,
                    void* context, std::string& error) {
        if (!asset_reader) {
            error = "Prince source asset reader is missing";
            return false;
        }
        actor = &prince;
        reader = asset_reader;
        reader_context = context;

        std::vector<std::uint8_t> raw_animation_records, raw_animation_names,
            raw_animation_fields, raw_clip_names, raw_clip_values, raw_bank,
            raw_character_records, raw_character_names, raw_character_fields,
            raw_class_records, raw_class_names, raw_class_fields;
        const std::array<std::pair<const char*, std::vector<std::uint8_t>*>, 12> inputs{{
            {"animations_pyarray.bin", &raw_animation_records},
            {"animations_pyarraynames.bin", &raw_animation_names},
            {"animations_pystructnames.bin", &raw_animation_fields},
            {"animations_dictionary_pyarraynames.bin", &raw_clip_names},
            {"animations_dictionary_pyarray.bin", &raw_clip_values},
            {"prince-animation-bank.bin", &raw_bank},
            {"character_properties_pyarray.bin", &raw_character_records},
            {"character_properties_pyarraynames.bin", &raw_character_names},
            {"character_properties_pystructnames.bin", &raw_character_fields},
            {"character_classes_pyarray.bin", &raw_class_records},
            {"character_classes_pyarraynames.bin", &raw_class_names},
            {"character_classes_pystructnames.bin", &raw_class_fields},
        }};
        for (const auto& input : inputs)
            if (!load_data(asset_reader, context, input.first, *input.second, error))
                return false;

        const data::Bytes animation_name_bytes{raw_clip_names.data(), raw_clip_names.size()};
        const data::Bytes animation_value_bytes{raw_clip_values.data(), raw_clip_values.size()};
        if (!data::load_dictionary(animation_name_bytes, animation_value_bytes,
                                   clip_table, error)) {
            error = "Prince animation dictionary: " + error;
            return false;
        }
        if (!data::load_animation_tables(
                {raw_animation_records.data(), raw_animation_records.size()},
                {raw_animation_names.data(), raw_animation_names.size()},
                {raw_animation_fields.data(), raw_animation_fields.size()},
                clip_table, animation_tables, error)) {
            error = "Prince animation tables: " + error;
            return false;
        }
        if (!data::load_animation_bank({raw_bank.data(), raw_bank.size()},
                                       animation_bank, error)) {
            error = "Prince AnimationBank: " + error;
            return false;
        }

        if (animation_bank.character != kPlayerName ||
            animation_bank.animation_table != 48 ||
            animation_bank.animation_set_id != 12302 ||
            animation_bank.template_clip_id != 1111 ||
            animation_bank.resources.size() != 116 ||
            animation_bank.registration_requests.size() != 158) {
            error = "Prince source AnimationBank does not match its checked identity and order";
            return false;
        }
        if (!data::load_characters(
                {raw_character_records.data(), raw_character_records.size()},
                {raw_character_names.data(), raw_character_names.size()},
                {raw_character_fields.data(), raw_character_fields.size()},
                character_table, error)) {
            error = "Prince Character properties: " + error;
            return false;
        }
        if (!data::load_classes(
                {raw_class_records.data(), raw_class_records.size()},
                {raw_class_names.data(), raw_class_names.size()},
                {raw_class_fields.data(), raw_class_fields.size()},
                class_table, error)) {
            error = "Prince Character class table: " + error;
            return false;
        }
        if (!data::load_property_rules(character_table, property_rules, error)) {
            error = "Prince Character property rules: " + error;
            return false;
        }
        const auto* player_row = data::property(character_table, kPlayerName, "ClassID");
        const auto player_name = std::find(character_table.names.begin(),
                                           character_table.names.end(), kPlayerName);
        if (!player_row || player_name == character_table.names.end()) {
            error = "Source KnightPlayerBase property row is missing";
            return false;
        }
        data::reset_properties(property_rules, player_properties,
                               &character_table.rows.at(player_name - character_table.names.begin()));
        if (!data::recalc_properties_with_class(class_table, property_rules,
                                                player_properties, error)) return false;
        const int animation_table = player_properties.resolved[2];
        if (animation_table != static_cast<int>(animation_bank.animation_table)) {
            error = "Resolved Prince animation table differs from the authored bank";
            return false;
        }

        const int idle = source_sequence_id(animation_tables, animation_table, "Idle");
        const int walk = source_sequence_id(animation_tables, animation_table, "Walk");
        const int run = source_sequence_id(animation_tables, animation_table, "Run");
        if (idle < 0 || walk < 0 || run < 0) {
            error = "Source Prince Idle, Walk, or Run sequence is missing";
            return false;
        }

        clips.clear();
        for (const auto& resource : animation_bank.resources) {
            data::AnimationStep reference;
            reference.anim = resource.clip_id;
            const auto* path = data::animation_clip(reference, clip_table);
            if (!path || *path != resource.authored_path ||
                resource.asset.rfind("animations/", 0) != 0) {
                error = "Prince bank resource dictionary identity differs";
                return false;
            }
            std::vector<std::uint8_t> raw;
            if (!read(asset_reader, context,
                    std::string(kAnimationAssetRoot) + resource.asset,
                    raw, error)) {
                error = "Prince bank clip " + std::to_string(resource.clip_id) +
                        ": " + error;
                return false;
            }
            if (raw.size() != resource.bytes) {
                error = "Prince bank animation byte size differs for clip " +
                        std::to_string(resource.clip_id);
                return false;
            }
            auto [it, inserted] = clips.try_emplace(resource.clip_id);
            if (!inserted || !it->second.load(raw.data(), raw.size(), prince.scene(),
                    error, animation::MissingTargets::ignore)) {
                if (error.empty()) error = "invalid animation interval";
                error = "Prince bank clip " + std::to_string(resource.clip_id) +
                        ": " + error;
                return false;
            }
            // The original library keeps the serialized timeline even when a
            // model has no bound target channels. Player::load intentionally
            // retains only bound tracks, so restore the exact source segment
            // interval for these still-registered no-op clips.
            if (it->second.end <= it->second.start &&
                it->second.track_count() == 0 &&
                !recover_unbound_clip_range(raw.data(), raw.size(), it->second,
                                            error)) {
                error = "Prince bank clip " + std::to_string(resource.clip_id) +
                        ": " + error;
                return false;
            }
            if (it->second.end <= it->second.start &&
                it->second.track_count() == 0 &&
                it->second.segment_count() == 0) {
                ++timeline_less_resources;
            } else if (it->second.end <= it->second.start) {
                error = "Prince bank clip " + std::to_string(resource.clip_id) +
                        ": non-positive source animation interval";
                return false;
            }
        }
        if (clips.size() != animation_bank.resources.size()) {
            error = "Prince source clip bank lost a registered resource";
            return false;
        }

        animation::RegistrationSet registration;
        for (const auto clip : animation_bank.registration_requests) {
            const auto found = clips.find(clip);
            const auto identity = data::animation_resource_identity(animation_bank, clip);
            if (found == clips.end() || !identity ||
                !registration.append(clip, identity, &found->second, error)) {
                if (error.empty()) error = "Prince animation registration resource is missing";
                return false;
            }
        }
        const auto default_clip = clips.find(animation_bank.template_clip_id);
        const auto default_identity = data::animation_resource_identity(
            animation_bank, animation_bank.template_clip_id);
        if (default_clip == clips.end() || !default_identity ||
            !registration.set_default(default_identity, &default_clip->second, error))
            return false;
        registration.refresh_indices();
        auto& visual = prince.visual_binding();
        float visual_scale[3]{};
        if (dh2_character_visual_scale(visual_scale,
                player_properties.base.data() + 12) != 0) {
            error = "Source Prince visual scale rejected";
            return false;
        }
        std::copy(visual_scale, visual_scale + 3, visual.root.scale);
        if (!playback.compile_dynamic(clips, registration, prince.scene(), visual,
                                      error)) {
            error = "Prince BlendedPlayback compilation: " + error;
            return false;
        }
        playback.observer = {this, playback_event};

        base_facts = {};
        base_facts.is_player = 1;
        base_facts.stance_mask = 210;
        const character::StanceFacts16 equipment{
            character::stance_is_player, 5, {0, 0}};
        if (dh2_character_anim_stance(&base_facts.stance, &equipment) != 1) {
            error = "Source Prince stance producer failed";
            return false;
        }
        base_facts.walk_threshold = .45f;
        base_facts.run_threshold = .85f;
        base_facts.idle = idle;
        base_facts.walk = walk;
        base_facts.run = run;
        base_facts.attack_static = source_sequence_id(animation_tables, animation_table,
                                                "AttackStatic");
        base_facts.attack_moving = source_sequence_id(animation_tables, animation_table,
                                               "Attack");
        base_facts.death = source_sequence_id(animation_tables, animation_table, "Died");
        const float authored_speed = 1.0f;
        move::Speed speed{};
        if (dh2_move_speed(&speed, player_properties.resolved.data(),
                           &authored_speed) != 0 ||
            dh2_character_attack_speed(&base_facts.attack_speed,
                player_properties.resolved.data()) != 1) {
            error = "Source Prince Character speed producer failed";
            return false;
        }
        base_facts.walk_speed = speed.walk_multiplier;
        base_facts.attack_delay = 0;
        base_facts.is_at_destination = 1;
        coordinator.state = {};
        coordinator.bind({this, facts_callback, {this, service_callback},
                          nullptr, nullptr});
        initialized = true;
        if (coordinator.transition(3) < 0 || fail_callback(error)) return false;
        return true;
    }
};

PrinceCharacterRuntime::PrinceCharacterRuntime() : impl_(std::make_unique<Impl>()) {}
PrinceCharacterRuntime::~PrinceCharacterRuntime() = default;
PrinceCharacterRuntime::PrinceCharacterRuntime(PrinceCharacterRuntime&&) noexcept = default;
PrinceCharacterRuntime& PrinceCharacterRuntime::operator=(PrinceCharacterRuntime&&) noexcept = default;

bool PrinceCharacterRuntime::load(PrinceActor& actor, PrinceAssetReader reader,
                                  void* context, std::string& error) {
    error.clear();
    if (!impl_ || !actor.ready()) {
        error = "Prince source Character requires a ready actor";
        return false;
    }
    if (impl_->body_services_bound || impl_->initialized) {
        error = "Prince source Character cannot be reloaded while initialized or body-bound";
        return false;
    }
    return impl_->initialize(actor, reader, context, error);
}

bool PrinceCharacterRuntime::bind_body_services(
    const PrinceCharacterBodyServices& services, std::string& error) {
    error.clear();
    if (!impl_ || !impl_->initialized || impl_->body_services_bound ||
        impl_->coordinator.state.body_present || !services.context ||
        !services.stop || !services.pin || !services.unpin ||
        (impl_->coordinator.state.current != 3 &&
         impl_->coordinator.state.current != 4)) {
        error = "Source Character body services require an unbound Idle/Move runtime and live callbacks";
        return false;
    }
    impl_->body_services = services;
    impl_->body_services_bound = true;
    // The caller binds only after successfully creating the body in its world.
    impl_->coordinator.state.body_present = 1;
    return true;
}

void PrinceCharacterRuntime::unbind_body_services() {
    if (!impl_ || !impl_->body_services_bound) return;
    // Clear the source fact before releasing the borrowed callback context.
    impl_->coordinator.state.body_present = 0;
    impl_->body_services = {};
    impl_->body_services_bound = false;
}

bool PrinceCharacterRuntime::set_input(float x, float y, bool accepted,
                                       std::string& error) {
    error.clear();
    if (!impl_ || !impl_->initialized || !std::isfinite(x) || !std::isfinite(y)) {
        error = "Prince input facts are invalid";
        return false;
    }
    if (accepted) impl_->heading = {x, y, 0.0f};
    else impl_->heading = {};
    impl_->coordinator.state.heading_active = accepted &&
        (std::fabs(x) > .01f || std::fabs(y) > .01f);
    if (impl_->coordinator.state.heading_active) {
        const float euler[3]{0.0f, 0.0f, std::atan2(x, -y)};
        if (!impl_->actor->visual_binding().set_rotation(euler)) {
            error = "Prince source heading rotation was rejected";
            return false;
        }
    }
    return true;
}

bool PrinceCharacterRuntime::request_move(std::string& error) {
    error.clear();
    if (!impl_ || !impl_->initialized) {
        error = "Prince Character runtime is not initialized";
        return false;
    }
    if (!impl_->coordinator.state.heading_active ||
        impl_->coordinator.state.current != 3) return true;
    return impl_->coordinator.event(0xc351, 0) >= 0 &&
           !impl_->fail_callback(error);
}

bool PrinceCharacterRuntime::scene_phase(std::uint32_t absolute_ms,
                                         std::string& error) {
    error.clear();
    return impl_ && impl_->initialized &&
        impl_->playback.scene_phase(absolute_ms, impl_->clips,
            impl_->actor->visual_binding(), impl_->actor->scene(), error) &&
        !impl_->fail_callback(error);
}

bool PrinceCharacterRuntime::update_timers(std::uint32_t dt_ms,
                                           std::string& error) {
    error.clear();
    if (!impl_ || !impl_->initialized ||
        impl_->coordinator.update_timers(dt_ms, 0) != 1 ||
        impl_->fail_callback(error)) {
        if (error.empty()) error = "Prince Character timer update failed";
        return false;
    }
    return true;
}

bool PrinceCharacterRuntime::update_state(std::uint32_t dt_ms,
                                          std::string& error) {
    error.clear();
    if (!impl_ || !impl_->initialized ||
        impl_->coordinator.update_state(dt_ms) < 0 ||
        impl_->fail_callback(error)) {
        if (error.empty()) error = "Prince source Character state update failed";
        return false;
    }
    return true;
}

bool PrinceCharacterRuntime::animator_phase(std::string& error) {
    error.clear();
    if (!impl_ || !impl_->initialized) {
        error = "Prince Character runtime is not initialized";
        return false;
    }
    const float speed = impl_->coordinator.state.current == 4
        ? impl_->coordinator.state.cached_speed : 1.0f;
    const auto extra = impl_->playback.completion.extra_ms;
    if (!impl_->playback.animator_phase(impl_->animation_tables, impl_->random,
            impl_->clips, impl_->actor->visual_binding(), impl_->actor->scene(),
            speed, extra, error) || impl_->fail_callback(error)) return false;
    return true;
}

bool PrinceCharacterRuntime::update_pose(const float owner_position[3],
                                         std::string& error) {
    error.clear();
    if (!impl_ || !impl_->initialized || !owner_position ||
        !std::isfinite(owner_position[0]) || !std::isfinite(owner_position[1]) ||
        !std::isfinite(owner_position[2])) {
        error = "Prince source owner position is invalid";
        return false;
    }
    std::copy(owner_position, owner_position + 3,
              impl_->actor->visual_binding().root.position);
    std::copy(owner_position, owner_position + 3, impl_->owner_position.begin());
    if (!impl_->actor->deform(error)) return false;
    return true;
}

std::int32_t PrinceCharacterRuntime::state_id() const {
    return impl_ ? impl_->coordinator.state.current : -1;
}
std::uint32_t PrinceCharacterRuntime::state_flags() const {
    return impl_ ? impl_->coordinator.state.flags : 0;
}
bool PrinceCharacterRuntime::copy_resolved_properties(
    std::array<std::int32_t, 224>& output) const {
    if (!impl_ || !impl_->initialized) return false;
    output = impl_->player_properties.resolved;
    return true;
}
std::int32_t PrinceCharacterRuntime::sequence_id() const {
    return impl_ ? impl_->coordinator.state.current_animation : -1;
}
std::int32_t PrinceCharacterRuntime::idle_sequence_id() const {
    return impl_ ? impl_->base_facts.idle : -1;
}
std::int32_t PrinceCharacterRuntime::walk_sequence_id() const {
    return impl_ ? impl_->base_facts.walk : -1;
}
std::uint32_t PrinceCharacterRuntime::registered_resource_count() const {
    return impl_ ? static_cast<std::uint32_t>(impl_->clips.size()) : 0;
}
std::uint32_t PrinceCharacterRuntime::registration_occurrence_count() const {
    return impl_ ? static_cast<std::uint32_t>(impl_->animation_bank.registration_requests.size()) : 0;
}
std::uint32_t PrinceCharacterRuntime::timeline_less_resource_count() const {
    return impl_ ? impl_->timeline_less_resources : 0;
}
std::int32_t PrinceCharacterRuntime::clip_id() const {
    return impl_ ? impl_->playback.current_clip() : -1;
}
std::int32_t PrinceCharacterRuntime::engine_clip_id() const {
    return impl_ ? impl_->playback.current_engine_clip() : -1;
}
float PrinceCharacterRuntime::timeline_speed() const {
    return impl_ ? impl_->playback.current_timeline().scale : 0.0f;
}
std::uint32_t PrinceCharacterRuntime::idle_common_update_calls() const {
    return impl_ ? impl_->idle_common_updates : 0;
}
std::uint32_t PrinceCharacterRuntime::external_state_events() const {
    return impl_ ? impl_->external_state_events : 0;
}
bool PrinceCharacterRuntime::body_present() const {
    return impl_ && impl_->coordinator.state.body_present != 0;
}
bool PrinceCharacterRuntime::body_services_bound() const {
    return impl_ && impl_->body_services_bound;
}
bool PrinceCharacterRuntime::ready() const { return impl_ && impl_->initialized; }

} // namespace dh2::irrlicht_game
