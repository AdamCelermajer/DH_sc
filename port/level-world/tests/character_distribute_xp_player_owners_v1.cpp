#include "../character_distribute_xp_player_owners_v1.hpp"

#include <cstdio>
#include <limits>
#include <stdexcept>

namespace h = dh2::player_manager_host_level;
namespace l = dh2::player_locality_v1;
namespace f = dh2::player_manager_friendly_v1;
namespace o = dh2::character_distribute_xp_player_owners_v1;

void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    dh2::character::NativePlayerCharacterOwnerV1 character;
    dh2::object_manager_runtime_owner_v1::Owner object_manager;
    dh2::object_manager_runtime_owner_v1::GameObject game_object;
    float position[2]{12.5f, -8.25f};
    dh2::data::PlayerSavegameV1 save;
    dh2::data::PropertyState properties;
    dh2::data::PropertyRules property_rules;
    std::uintptr_t character_660{};
    dh2::character_level_member::IntMember level_member{};
    h::PlayerInfoProjection player{};
    h::PlayerInfoProjection* entries[1]{};
    h::PlayerRegistry registry{};
    l::MatchingLocalFields matching_fields{};
    l::Matching matching{};
    l::Services locality{};
    f::Services friendly{};

    Fixture() {
        const auto character_id = character.identity();
        save.set_character(character_id);
        check(character.bind_session(&character_660, save, properties, nullptr,
                                     error),
              "bind one canonical Character/Save/property owner");
        game_object.identity = character_id;
        game_object.bind_live_fields({&position[0], &position[1]});
        dh2::object_manager_runtime_owner_v1::GameObject* stored = nullptr;
        check(object_manager.add_object(31, game_object, &stored) ==
                  dh2::object_manager_runtime_owner_v1::Status::ok && stored,
              "register the same Character identity in the canonical ObjectManager");
        player = {0x10001008, 0, &level_member};
        entries[0] = &player;
        registry = {0x10001000, entries, 1, &player};
        l::construct_matching_local_fields(matching_fields);
        matching_fields.member_3638 = 7;
        matching = {0x10008000, &matching_fields.active_c};
        locality.context = this;
        locality.online = [](void*, std::uint8_t* value) -> std::int32_t {
            *value = 0;
            return 0;
        };
        locality.acquire_matching = [](void* raw, l::Matching** value) -> std::int32_t {
            *value = &static_cast<Fixture*>(raw)->matching;
            return 0;
        };
        locality.character_660 = [](void* raw, l::PlayerInfo* selected,
                                    std::uintptr_t* value) -> std::int32_t {
            auto& self = *static_cast<Fixture*>(raw);
            if (selected != &self.player) return 1;
            *value = self.character_660;
            return 0;
        };
        locality.internal_id_player = [](void* raw, const l::Registry* registry,
                                         std::int32_t id, std::uint32_t,
                                         l::PlayerInfo** value) -> std::int32_t {
            auto& self = *static_cast<Fixture*>(raw);
            if (registry != &self.registry) return 1;
            *value = id == self.player.internal_id ? &self.player : nullptr;
            return 0;
        };
        locality.player_virtual_is_local = [](void* raw, l::PlayerInfo* selected,
                                               std::int32_t* value) -> std::int32_t {
            auto& self = *static_cast<Fixture*>(raw);
            l::Result result{};
            if (selected != &self.player ||
                l::cnet_player_is_local(selected, &self.locality, &result) !=
                    l::Status::complete) return 1;
            *value = result.value;
            return 0;
        };
        locality.member_1a0 = [](void* raw, l::PlayerInfo* selected,
                                 std::int32_t* value) -> std::int32_t {
            auto& self = *static_cast<Fixture*>(raw);
            if (selected != &self.player) return 1;
            *value = 7;
            return 0;
        };
        locality.matching_member_id = [](void* raw, l::Matching* selected,
                                         std::int32_t* value) -> std::int32_t {
            auto& self = *static_cast<Fixture*>(raw);
            if (selected != &self.matching) return 1;
            *value = l::local_member_id(self.matching_fields);
            return 0;
        };
        friendly = {&locality, this,
            [](void* raw, f::PlayerInfo* selected,
               std::int32_t* value) -> std::int32_t {
                auto& self = *static_cast<Fixture*>(raw);
                if (selected != &self.player) return 1;
                *value = self.player.internal_id;
                return 0;
            }};
    }
    std::string error;
    o::Binding binding() {
        return {&registry, &friendly, &locality, &character, &object_manager,
                &property_rules};
    }
};

int main() {
    try {
        Fixture fixture;
        auto binding = fixture.binding();
        o::Result result{};
        check(o::resolve(&binding, 0, &result, fixture.error) == o::Status::complete,
              "friendly ordinal resolves the source-selected Character owners");
        check(result.player == &fixture.player &&
              result.character_identity == fixture.character.identity() &&
              result.savegame == &fixture.save &&
              result.savegame->character() == result.character_identity &&
              result.properties.resolved == fixture.properties.resolved.data() &&
              result.is_local == 1 && result.world_x == 12.5f &&
              result.world_y == -8.25f,
              "PlayerInfo, Character, Save, properties, locality and live GameObject position share owners");

        fixture.position[0] = 19.0f;
        check(o::resolve(&binding, 0, &result, fixture.error) == o::Status::complete &&
              result.world_x == 19.0f && result.world_y == -8.25f,
              "resolved XP position follows the canonical live actor coordinates");
        fixture.position[0] = 12.5f;

        const auto prior = result;
        check(o::resolve(&binding, 1, &result, fixture.error) ==
                  o::Status::player_selection_failed &&
              result.player == prior.player &&
              result.character_identity == prior.character_identity,
              "missing friendly ordinal fails closed without replacing output");

        fixture.character_660 += 8;
        check(o::resolve(&binding, 0, &result, fixture.error) ==
                  o::Status::character_owner_mismatch &&
              result.character_identity == prior.character_identity,
              "unowned PlayerInfo Character identity is rejected atomically");
        fixture.character_660 = fixture.character.identity();

        auto* live_object = fixture.object_manager.find_by_identity(
            fixture.character.identity());
        check(live_object != nullptr, "find canonical GameObject identity");
        live_object->live_fields.world_x = nullptr;
        check(o::resolve(&binding, 0, &result, fixture.error) ==
                  o::Status::game_object_missing,
              "snapshot-only GameObject coordinates are rejected as stale");
        live_object->live_fields.world_x = &fixture.position[0];
        fixture.position[1] = std::numeric_limits<float>::infinity();
        check(o::resolve(&binding, 0, &result, fixture.error) ==
                  o::Status::invalid_position,
              "non-finite live GameObject coordinates are rejected");
        fixture.position[1] = -8.25f;

        fixture.matching_fields.member_3638 = 8;
        check(o::resolve(&binding, 0, &result, fixture.error) == o::Status::complete &&
              result.is_local == 0,
              "IsLocalPlayer result comes from the existing CMatching owner");
        std::puts("PASS: friendly PlayerInfo joins canonical Character, Save, properties, locality and live GameObject position");
        return 0;
    } catch (const std::exception& exception) {
        std::fprintf(stderr, "%s\n", exception.what());
        return 1;
    }
}
