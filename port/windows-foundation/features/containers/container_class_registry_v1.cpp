#include "container_class_registry_v1.hpp"

namespace dh::foundation::containers {

ContainerClassRegistryV1::ContainerClassRegistryV1() {
    classes_.push_back({"OpenableContainer", ContainerFamilyV1::openable, 0});
    classes_.push_back({"DestructibleContainer", ContainerFamilyV1::destructible, 8});
}

bool ContainerClassRegistryV1::add(ContainerClassV1 entry) {
    if (find(entry.gametype)) return false;
    classes_.push_back(std::move(entry));
    return true;
}

const ContainerClassV1* ContainerClassRegistryV1::find(const std::string& gametype) const {
    for (const auto& entry : classes_)
        if (entry.gametype == gametype) return &entry;
    return nullptr;
}

bool ContainerClassRegistryV1::handled_elsewhere(const std::string& gametype) {
    return gametype == "Character" || gametype == "SpawnPoint" || gametype == "Module" ||
           gametype.empty();
}

} // namespace dh::foundation::containers
