#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace dh::foundation {

using ObjectId = std::uint64_t;
constexpr ObjectId invalid_object_id = 0;

// Position and scale use scene units; rotation is XYZ Euler angles in radians.
struct Transform {
    std::array<float, 3> position{0.0f, 0.0f, 0.0f};
    std::array<float, 3> rotation{0.0f, 0.0f, 0.0f};
    std::array<float, 3> scale{1.0f, 1.0f, 1.0f};
};

// References are asset-relative identifiers, resolved by the asset loader.
// An empty model denotes an invisible anchor or a future gameplay object.
struct VisualReference {
    std::string model;
    std::string material;
    std::string animation;
    bool visible = true;
};

struct WorldObject {
    ObjectId id = invalid_object_id;
    std::string name;
    Transform transform;
    VisualReference visual;
    // Feature-owned, versioned persistent components. The world/save layer
    // preserves bytes; the owning feature validates and decodes its own state.
    // Animation callbacks, borrowed pointers and render handles are transient.
    std::map<std::string,std::vector<std::uint8_t>> state_components;
};
inline constexpr std::size_t world_object_component_limit=64;
inline constexpr std::size_t world_object_state_limit=64u*1024u;
bool validate_world_object(const WorldObject&,std::string& error);

class World final {
public:
    // IDs are assigned monotonically and never reused within a loaded world.
    // Returns zero on invalid transforms or exhausted IDs, without a mutation.
    ObjectId spawn(std::string name, VisualReference visual = {},
                   Transform transform = {});
    // Authored IDs are supplied by the level owner, not reassigned at load.
    bool bind(WorldObject object,std::string* error=nullptr);
    bool remove(ObjectId id);
    const WorldObject* find(ObjectId id) const noexcept;
    // Update through these methods to keep object IDs immutable.
    bool set_transform(ObjectId id, const Transform& transform);
    bool set_visual(ObjectId id, VisualReference visual);

    // Input IDs must be nonzero and unique. Validation and construction happen
    // before replacing the current world, so a failed load preserves it.
    bool replace(std::vector<WorldObject> objects, std::string* error = nullptr);
    // Clear begins a new ID namespace; references to the old scene must be dropped.
    void clear() noexcept;

    std::size_t size() const noexcept { return objects_.size(); }
    bool empty() const noexcept { return objects_.empty(); }
    // Deterministic ID order; pointers survive spawn, but not removal/replacement.
    const std::map<ObjectId, WorldObject>& objects() const noexcept { return objects_; }

private:
    std::map<ObjectId, WorldObject> objects_;
    ObjectId next_id_ = 1;
};

} // namespace dh::foundation
