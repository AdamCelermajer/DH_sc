#pragma once
#include <cstdint>
namespace dh2::animation {
// Actual PSRandom::Rand62fe78 word authority. Seed initialization is supplied
// by the owning source particle context/constructor, never a default here.
// Null is a malformed native borrow and returns NaN, not an accepted draw.
extern "C" double dh2_particle_random_v1(std::int32_t* source_seed);
}
