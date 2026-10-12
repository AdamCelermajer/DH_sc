// Test for recon_v2 wave 14: empty and constant-return particle attribute hooks.
#include <cstdio>

#include "glitch-scene-collada-gui/recon/v2/wave_14.hpp"

namespace rv = glitch::scene::recon_v2_14;

static int failures = 0;

#define CHECK(cond)                                                   \
    do {                                                              \
        if (!(cond)) {                                                \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                               \
        }                                                             \
    } while (0)

int main() {
    int sentinel = 0;
    int self = 0;

    rv::IParticleEmitter_serializeAttributes(&self, &sentinel, &sentinel);
    CHECK(sentinel == 0);  // empty body must not write attributes

    CHECK(rv::IParticleEmitter_deserializeAttributes(&self, 5, &sentinel, &sentinel) == 0);
    CHECK(sentinel == 0);

    rv::IParticleAffector_serializeAttributes(&self, &sentinel, &sentinel);
    CHECK(sentinel == 0);

    CHECK(rv::IParticleAffector_deserializeAttributes(&self, 3, &sentinel, &sentinel) == 0);
    CHECK(sentinel == 0);

    if (failures == 0) {
        std::printf("wave_14 tests: all passed\n");
        return 0;
    }
    std::printf("wave_14 tests: %d failure(s)\n", failures);
    return 1;
}
