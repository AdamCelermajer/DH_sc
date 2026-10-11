#pragma once

#include "../../../../../level-world/character_apply_result.hpp"
#include "../../../../../level-world/character_runtime_factory_v1.hpp"
#include "../../../../../level-world/player_ai_timer_events_v1.hpp"
#include "../../../../../level-world/character_regeneration.hpp"
#include "../../../../../game-data/savegame_options_v1.hpp"
#include "../../../../../game-data/properties.hpp"

#include <cstdint>
#include <string>

namespace dh2::native::faery_combat_v1 {

// Same-Character composition for the source Character::HandleDots route.
// Runtime owns only the temporary Faery-side F_DotAttack adapter; the caller
// retains the Character scalar projection, property sheet, Debug, combat RNG,
// and every reached F_ApplyResult effect provider.
struct Bindings {
    character_runtime_factory_v1::Record* record = nullptr;
    const std::int32_t* resolved = nullptr;
    data::PropertyView* property_view = nullptr;
    const std::uint32_t* source_char_type = nullptr;
    const char* source_character_name = nullptr;
    const std::uint8_t* online = nullptr;
    const data::savegame_options_v1::Application* saved_options = nullptr;
    void* player_count_context = nullptr;
    std::int32_t (*player_count)(void*, std::int32_t*) = nullptr;
    data::CombatRandom* random = nullptr;
    debug_switches::Globals* debug_globals = nullptr;
    const debug_switches::Services* debug_services = nullptr;
    character_apply_result::Globals apply_globals{};
    character_apply_result::Services apply_services{};
    void* hit_for_context = nullptr;
    int (*hit_for)(void*, const character_apply_result::Request*,
                   character_apply_result::Reply*, std::string& error) = nullptr;
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    stale_character,
    dot_attack_failed,
    apply_result_failed,
};

class Runtime final {
public:
    explicit Runtime(Bindings);
    Runtime(const Runtime&) = delete;
    Runtime& operator=(const Runtime&) = delete;

    // Fills the existing timer Runtime's source providers. It does not create
    // another timer vector/runtime or change event dispatch ownership.
    Status attach(player_ai_timer_events_v1::Bindings&, std::string& error);
    const character_dot_attack::Result& last_dot_attack() const noexcept {
        return dot_result_;
    }
    const character_apply_result::Result& last_apply_result() const noexcept {
        return apply_result_;
    }

private:
    static std::int32_t dot_attack(void*, const character_dot_tick::AttackRequest*);
    static int apply_result(void*, const character_dot_tick::ApplyRequest*, std::string&);
    static std::int32_t apply_service(void*, const character_apply_result::Request*,
                                      character_apply_result::Reply*);
    static std::int32_t regen_service(void*, const character_regeneration::Request*,
                                      character_regeneration::Reply*);

    Bindings bindings_{};
    std::uintptr_t character_ = 0;
    character_apply_result::Actor actor_{};
    character_dot_attack::Actor dot_actor_{};
    character_dot_attack::CombatContext combat_context_{};
    character_dot_attack::Storage dot_storage_{};
    character_dot_attack::Runtime dot_runtime_{};
    character_regeneration::Result regen_hp_result_{}, regen_mp_result_{};
    character_dot_attack::Result dot_result_{};
    character_apply_result::Result apply_result_{};
    std::string last_service_error_;
    bool attached_ = false;
    bool busy_ = false;
};

} // namespace dh2::native::faery_combat_v1
