#pragma once

// P16 LEVELS: registry of authored GameObject classes (the gametype attribute).
// Maps each class to the runtime consumer that exists in this source tree, so a level
// load can log every class with no consumer exactly once, and the coverage tool can
// report the same status. Status describes code that exists today; it is not a claim
// about original behaviour. Add a row here when a consumer is wired (general systems
// only: never add map names).

#include "actor_definitions.hpp"

#include <string>
#include <vector>

namespace dh::foundation::levels {

enum class ClassStatus {
    // Instantiated by the population/source loaders (actor_population, actor_lighting).
    consumed,
    // A consumer exists in this target (dh-foundation sources) but is not wired into the level runtime yet.
    library_only,
    // A canonical consumer exists in port/level-loader or port/android-native, but dh-foundation does not compile it.
    ported_elsewhere,
    // No consumer in any port (or structural, not a class).
    unsupported,
};

struct ClassInfo {
    const char* gametype;
    ClassStatus status;
    const char* consumer; // source file(s) holding the consumer, for the report
};

// Known gametype attribute values in the device data (scene/*.mlx, 3d/modules/**/mgp|mvp).
const std::vector<ClassInfo>& class_registry();
ClassStatus class_status(const std::string& gametype);
const char* class_status_name(ClassStatus status);

// Logs one line per unsupported or library-only class that occurs in `definitions`,
// once per process (static set), with its declaration count. Returns the number of
// newly logged classes. Output goes to std::cerr with the prefix "Unsupported class".
std::size_t log_unsupported_classes_once(const std::vector<ActorDefinition>& definitions);

} // namespace dh::foundation::levels
