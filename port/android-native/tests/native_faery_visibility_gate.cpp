#include "../app/src/main/cpp/native_faery_visibility_gate.hpp"

#include <cstdlib>
#include <iostream>

int main() {
    using dh2::native::faery_visibility_gate::Evidence;
    using dh2::native::faery_visibility_gate::may_show;

    Evidence e{};
    if (may_show(e)) return EXIT_FAILURE;
    e = {1, 1, 1, 1, 1, 1, 1, 1};
    if (!may_show(e)) return EXIT_FAILURE;

    // Every source lifecycle boundary independently keeps the Faery hidden.
    std::uint8_t* phases[] = {&e.source_constructor, &e.init_post,
        &e.init_final, &e.physics, &e.faery_ai, &e.visual, &e.placement,
        &e.registered};
    for (auto* phase : phases) {
        *phase = 0;
        if (may_show(e)) return EXIT_FAILURE;
        *phase = 1;
    }

    std::cout << "{\"faery_hidden_until_all_lifecycle_providers\":true}\n";
    return EXIT_SUCCESS;
}
