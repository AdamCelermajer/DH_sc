// P16 CONTEXT: interaction-type provider registry. A test double chest/barrel/NPC registers its type; the OOI owner reads it.
#include "interactable_registry_v1.hpp"

#include <cstdio>

using namespace dh::foundation;

namespace {
int failures = 0;
void check(bool ok, const char* name) {
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++failures;
}
constexpr ActorId player = 1;
constexpr ActorId chest = 100;
constexpr ActorId barrel = 101;
} // namespace

int main() {
    InteractableRegistryV1 registry;
    std::string error;
    InteractableProviderV1 empty;
    check(!registry.upsert({chest, false, {0, 80, 0}, 0.0f, true, empty}, error) && !error.empty(),
          "provider without interaction_type is rejected");
    InteractableProviderV1 chest_provider;
    chest_provider.interaction_type = [](ActorId) { return 0; };   // OpenableContainer
    check(registry.upsert({chest, false, {0, 80, 0}, 0.0f, true, chest_provider}, error), "chest registers");
    InteractableProviderV1 barrel_provider;
    barrel_provider.interaction_type = [](ActorId) { return 8; };  // DestructibleContainer
    check(registry.upsert({barrel, false, {0, 40, 0}, 0.0f, true, barrel_provider}, error), "barrel registers");
    check(registry.size() == 2, "two interactables registered");

    const auto list = registry.candidates(player);
    bool chest_type_ok = false, barrel_type_ok = false;
    for (const auto& c : list) {
        if (c.id == chest) chest_type_ok = c.interaction_type == 0 && !c.is_character;
        if (c.id == barrel) barrel_type_ok = c.interaction_type == 8;
    }
    check(chest_type_ok && barrel_type_ok, "candidates carry each provider's type for the viewer");

    // Viewer-dependent provider (source vt+144(obj, owner)): a chest that only opens for the viewer.
    InteractableProviderV1 locked;
    locked.interaction_type = [](ActorId viewer) { return viewer == player ? 0 : -1; };
    registry.upsert({102, false, {0, 10, 0}, 0.0f, true, locked}, error);
    bool viewer_ok = false, other_ok = false;
    for (const auto& c : registry.candidates(player)) if (c.id == 102) viewer_ok = c.interaction_type == 0;
    for (const auto& c : registry.candidates(7)) if (c.id == 102) other_ok = c.interaction_type == -1;
    check(viewer_ok && other_ok, "provider is evaluated per viewer");

    registry.erase(barrel);
    check(registry.size() == 2, "erase removes one entry");
    registry.clear();
    check(registry.size() == 0 && registry.candidates(player).empty(), "clear empties the registry");

    std::printf("%s\n", failures == 0 ? "interactable registry tests passed" : "interactable registry tests FAILED");
    return failures == 0 ? 0 : 1;
}
