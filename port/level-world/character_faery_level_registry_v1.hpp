#pragma once

#include "character_faery_live_binding_v1.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::character_faery_level_registry_v1 {

namespace placement = character_faery_placement_v1;
namespace binding = character_faery_live_binding_v1;

// Borrowed graph for one source Character in the active Level. The binding
// certifies factory Character/GameObject/AI, AISFaery session, POFaerie,
// physical and visual identities, plus the same GameObject position owner.
struct Entry {
    placement::Identity character{};
    binding::Binding* graph{};
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    list_service_failed,
    missing_character_graph,
    graph_incomplete,
    roster_changed,
    placement_failed,
};

struct Result {
    std::uint32_t classified_characters{};
    std::uint32_t faeries{};
    std::uint32_t followers{};
    placement::Result placement{};
};

// Validates the complete classified Level roster without changing world,
// Save, skills, or AI state. The returned status is suitable for the UI
// transaction's prepare phase; callers still commit placement before Save.
class Owner final {
public:
    Owner(std::vector<Entry> entries, placement::Services level_services);
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;

    Status preflight(Result*, std::string& error);
    Status place(placement::Identity explicit_player_character,
                 Result*, std::string& error);

private:
    struct Member {
        placement::Identity cursor{};
        placement::Identity character{};
        placement::Identity next{};
        bool faery{};
        bool follower{};
        std::uint8_t faery_queries{};
    };

    static int dispatch(void*, const placement::Request&, placement::Reply*);
    Entry* find(placement::Identity) noexcept;
    const Member* find_cursor(placement::Identity) const noexcept;
    Status snapshot(Result&, std::string&);

    std::vector<Entry> entries_;
    placement::Services level_services_{};
    std::vector<Member> members_;
    placement::Identity list_end_{};
    placement::Runtime runtime_;
    bool busy_{};
};

} // namespace dh2::character_faery_level_registry_v1
