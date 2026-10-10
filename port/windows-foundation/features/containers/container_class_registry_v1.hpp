#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace dh::foundation::containers {

// Implementation family selected by an authored gametype. New families (traps,
// triggers, shrines, doors) register here with their own interaction type and
// implementation; nothing else in the loader names a specific class.
enum class ContainerFamilyV1 : std::uint8_t { openable, destructible };

struct ContainerClassV1 {
    std::string gametype;             // authored GameObject gametype attribute
    ContainerFamilyV1 family;
    // Original GetInteractionType result (vtable+144): 0 openable chest,
    // 8 destructible (attack). Context-button routing reads this value.
    std::int32_t interaction_type;
};

class ContainerClassRegistryV1 {
public:
    // Registers the Preview 16 families: OpenableContainer (0), DestructibleContainer (8).
    ContainerClassRegistryV1();
    // Extension point. Returns false when the gametype is already registered.
    bool add(ContainerClassV1 entry);
    const ContainerClassV1* find(const std::string& gametype) const;
    // Authored gametypes that are not containers and are handled by another owner
    // (population for Character/SpawnPoint, structural Module). Never reported.
    static bool handled_elsewhere(const std::string& gametype);
    const std::vector<ContainerClassV1>& classes() const noexcept { return classes_; }

private:
    std::vector<ContainerClassV1> classes_;
};

} // namespace dh::foundation::containers
