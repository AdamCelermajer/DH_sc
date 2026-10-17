#pragma once

#include "character_faery_placement_v1.hpp"
#include "character_faery_script_session_v1.hpp"
#include "character_runtime_factory_v1.hpp"
#include "game_object_position_owner_v1.hpp"

#include <string>

namespace dh2::character_faery_live_binding_v1 {

// A borrowed certificate for one source-owned Faery graph. The graph validator
// must resolve the AISFaery, POFaerie, physical and visual owners from this same
// Character record; non-null pointers alone are not proof of that association.
// No Player, Save, Character or component owner is created here.
struct Components {
    const void* ais_faery{};
    const character_faery_script_session_v1::Session* script_session{};
    character_faery_script_session_v1::Services script_services{};
    const void* pofaerie{};
    const void* physical{};
    const void* visual{};
};

struct Binding {
    character_runtime_factory_v1::Record* character{};
    game_object_position_owner_v1::Owner* position{};
    game_object_position_owner_v1::Services position_services{};
    Components components{};
    character_faery_placement_v1::Services placement{};
    void* validation_context{};
    int (*validate_graph)(void*, const character_runtime_factory_v1::Record&,
                          const game_object_position_owner_v1::Owner&,
                          const Components&, std::string&){};
};

enum class Status {
    complete,
    invalid_argument,
    graph_incomplete,
    graph_stale,
    placement_failed,
};

// Read-only graph certificate shared by the single-actor and Level-wide
// placement owners. A successful result means all providers refer to one
// complete canonical Character; it does not itself place or mutate anything.
Status validate_binding(const Binding&, std::string& error);
Status validate_character_binding(const Binding&, bool faery,
                                  std::string& error);

// Calls the existing source-order placement kernel only when a real factory
// record has completed its Character lifecycle and the app confirms all
// Faery-specific component links. The binding is borrowed and must outlive the
// owner. This deliberately has no synthetic Character/player fallback.
class Owner final {
public:
    explicit Owner(Binding binding) noexcept
        : binding_(binding), runtime_({this, &Owner::dispatch}) {}
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;

    Status place(std::uintptr_t explicit_player_character,
                 character_faery_placement_v1::Result* result,
                 std::string& error);

private:
    Status validate(std::string& error) const;
    static int dispatch(void*, const character_faery_placement_v1::Request&,
                        character_faery_placement_v1::Reply*);

    Binding binding_{};
    character_faery_placement_v1::Runtime runtime_{};
};

} // namespace dh2::character_faery_live_binding_v1
