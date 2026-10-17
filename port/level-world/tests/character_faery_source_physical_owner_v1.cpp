#include "../character_faery_source_physical_owner_v1.hpp"

#include "../character_faery_visual_facade_v1.hpp"

#include <cassert>
#include <cmath>
#include <memory>
#include <string>

namespace owner = dh2::character_faery_source_physical_owner_v1;
namespace facade = dh2::character_faery_visual_facade_v1;
namespace component = dh2::character_physical_component_v1;
namespace physical = dh2::physical;
namespace factory = dh2::character_runtime_factory_v1;

namespace {
class Pose final : public facade::CharacterPose {
public:
    dh2::objects::Resource& bdae_resource() noexcept override {
        return resource;
    }
    bool sample(std::int32_t, std::uint32_t, std::string&) override {
        return true;
    }
    dh2::objects::Resource resource{};
};

class Asset final : public facade::AssetData {
public:
    std::shared_ptr<facade::CharacterPose> create_character_pose()
        const override {
        return std::make_shared<Pose>();
    }
};
}

int main() {
    factory::Record record;
    const auto identity = reinterpret_cast<std::uintptr_t>(&record.game_object);
    record.game_object.identity = identity;
    record.character.identity = identity;
    record.constructor.identity = identity;
    record.source_handle = 44;
    record.object_registered = true;
    record.character_listed = true;
    record.game_object.world_x = 125.0f;
    record.game_object.world_y = -50.0f;
    record.faery_visual.character = identity;
    record.faery_visual.asset = std::make_shared<facade::Asset>(
        facade::Asset{7, 3, 0, "faery.bdae", std::make_shared<Asset>()});
    record.faery_visual.pose = record.faery_visual.asset->renderer_data
        ->create_character_pose();
    record.faery_visual.clip_sequence = 0;
    record.game_object.visual_object_2d8 = reinterpret_cast<std::uintptr_t>(
        &record.faery_visual);

    physical::NativeWorld world;
    const float world_bounds[4]{-100.0f, -100.0f, 100.0f, 100.0f};
    world.load(world_bounds);
    const auto ground_bodies = world.backend()->GetBodyCount();
    const physical::CharacterOwnerBounds source_bounds{
        {-36.0f, -30.0f, 36.0f, 30.0f, 0.0f, 0.0f},
        {89.0f, -80.0f, 161.0f, -20.0f, 0.0f, 0.0f}, 0, 1};
    std::string error;

    assert(facade::ready(record.faery_visual, identity));
    assert(owner::initialize_from_revive(record, world, source_bounds, error) ==
           owner::Status::complete);
    assert(error.empty() && record.pofaerie_physical &&
           record.source_pofaerie_identity &&
           world.backend()->GetBodyCount() == ground_bodies + 1);
    const auto physical_identity = record.source_pofaerie_identity;
    const auto& config = record.pofaerie_physical->config();
    assert(config.enabled && config.po_character && config.pinned &&
           config.shape.kind == 0 && config.shape.sensor == 0 &&
           config.shape.group_index == 0 &&
           config.shape.category_bits == 0x100 && config.shape.mask_bits == 0 &&
           std::fabs(config.radius - 0.36f) < 0.0001f);

    component::Transform observed{};
    assert(record.pofaerie_physical->observe(&observed) ==
           component::Status::complete);
    assert(std::fabs(observed.game_x - 125.0f) < 0.001f &&
           std::fabs(observed.game_y + 50.0f) < 0.001f);

    record.game_object.world_x = 240.0f;
    record.game_object.world_y = 80.0f;
    assert(owner::initialize_from_revive(record, world, source_bounds, error) ==
           owner::Status::already_complete);
    assert(error.empty() && record.source_pofaerie_identity == physical_identity &&
           world.backend()->GetBodyCount() == ground_bodies + 1);
    assert(record.pofaerie_physical->observe(&observed) ==
           component::Status::complete);
    assert(std::fabs(observed.game_x - 240.0f) < 0.001f &&
           std::fabs(observed.game_y - 80.0f) < 0.001f);

    auto changed_bounds = source_bounds;
    changed_bounds.absolute_box[0] = 0.0f;
    changed_bounds.absolute_box[3] = 80.0f;
    assert(owner::initialize_from_revive(record, world, changed_bounds, error) ==
           owner::Status::complete);
    assert(error.empty() && record.pofaerie_physical &&
           world.backend()->GetBodyCount() == ground_bodies + 1 &&
           std::fabs(record.pofaerie_physical->config().radius - 0.4f) <
               0.0001f);

    assert(owner::detach(record, error) == owner::Status::complete);
    assert(!record.pofaerie_physical && !record.source_pofaerie_identity &&
           world.backend()->GetBodyCount() == ground_bodies);

    assert(owner::initialize_from_revive(record, world, source_bounds, error) ==
           owner::Status::complete);
    assert(record.source_pofaerie_identity &&
           world.backend()->GetBodyCount() == ground_bodies + 1);
    assert(owner::detach(record, error) == owner::Status::complete);
    assert(world.backend()->GetBodyCount() == ground_bodies);

    record.game_object.visual_object_2d8 = 0;
    assert(owner::initialize_from_revive(record, world, source_bounds, error) ==
           owner::Status::visual_unavailable);
    assert(world.backend()->GetBodyCount() == ground_bodies);
    return 0;
}
