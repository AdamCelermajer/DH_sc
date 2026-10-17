#include "../projectile_manager_runtime_owner_v1.hpp"

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <cstdio>
#include <string>
#include <vector>

namespace manager = dh2::projectile_manager_runtime_owner_v1;
namespace projectile = dh2::projectile_lifecycle_runtime_v1;
namespace objects = dh2::object_manager_runtime_owner_v1;
namespace factory = dh2::projectile_factory_owner_v1;
namespace data = dh2::data;

struct Fixture {
    manager::Owner* owner{};
    objects::Address manager_identity{0x9900};
    std::vector<char> calls;
    int set_manager_calls{};
};

static projectile::Status set_manager(void* context, objects::GameObject& object,
                                      objects::Address identity) noexcept {
    auto& fixture = *static_cast<Fixture*>(context);
    assert(object.identity != 0 && identity == fixture.manager_identity);
    const auto pool = object.name.rfind("LTProjectile_", 0) == 0
        ? fixture.owner->laser_pool() : fixture.owner->ordinary_pool();
    // The source inserts the newly constructed object into its class pool
    // before calling Projectile::SetManager.
    assert(pool.source_capacity >= pool.slot_count &&
           pool.active_count == pool.slot_count &&
           pool.next_serial == pool.slot_count);
    ++fixture.set_manager_calls;
    fixture.calls.push_back('m');
    return projectile::Status::ok;
}

static projectile::Status set_updating(void* context, objects::GameObject&,
                                       bool enabled) noexcept {
    static_cast<Fixture*>(context)->calls.push_back(enabled ? 'u' : 'U');
    return projectile::Status::ok;
}

static projectile::Status set_info(void* context, objects::GameObject& object,
                                   const projectile::SetInfoArgs& args,
                                   float angle) noexcept {
    auto& fixture = *static_cast<Fixture*>(context);
    assert(object.identity != 0 && args.projectile_id >= 0);
    assert(args.source_record != nullptr &&
           args.source_record->row_index ==
               static_cast<std::uint32_t>(args.projectile_id));
    assert(angle == 0.0f);
    fixture.calls.push_back('i');
    return projectile::Status::ok;
}

static projectile::Status stop(void* context, objects::GameObject& object) noexcept {
    assert(object.identity != 0);
    static_cast<Fixture*>(context)->calls.push_back('s');
    return projectile::Status::ok;
}

static projectile::Status set_active(void* context, objects::GameObject& object,
                                     bool active) noexcept {
    assert(object.identity != 0);
    static_cast<Fixture*>(context)->calls.push_back(active ? 'a' : 'A');
    return projectile::Status::ok;
}

static projectile::Hooks hooks(Fixture& fixture) {
    projectile::Hooks result{};
    result.context = &fixture;
    result.set_manager = set_manager;
    result.set_updating = set_updating;
    result.set_info_float = set_info;
    result.stop = stop;
    result.set_active = set_active;
    return result;
}

static std::vector<std::uint8_t> read_file(const std::string& root,
                                           const char* name) {
    std::ifstream input(root + "/" + name, std::ios::binary);
    assert(input);
    return {std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()};
}

static data::Bytes view(const std::vector<std::uint8_t>& bytes) {
    return {bytes.data(), bytes.size()};
}

static data::ProjectileTableV1 read_table(const std::string& root) {
    const auto rows = read_file(root, "projectiles_pyarray.bin");
    const auto names = read_file(root, "projectiles_pyarraynames.bin");
    const auto schema = read_file(root, "projectiles_pystructnames.bin");
    const auto dict_names = read_file(root, "projectiles_dictionary_pyarraynames.bin");
    const auto dict_values = read_file(root, "projectiles_dictionary_pyarray.bin");
    data::ProjectileTableV1 table{};
    std::string error;
    assert(data::load_projectile_table_v1(view(rows), view(names), view(schema),
        view(dict_names), view(dict_values), table, error));
    assert(error.empty() && table.rows.size() == 41);
    return table;
}

static projectile::FactoryProduct product_for(
    const manager::Acquisition& acquisition, objects::Address identity) {
    objects::GameObject object{};
    object.identity = identity;
    object.source_handle = acquisition.source_handle;
    object.name = acquisition.factory_reservation.name;
    const auto type = acquisition.laser_type
        ? projectile::FactoryType::laser_type_projectile
        : projectile::FactoryType::projectile;
    return {acquisition.source_handle, acquisition.factory_reservation.name,
            type, std::move(object)};
}

static factory::Request launch_for(const data::ProjectileRowV1& row) {
    factory::Request request{};
    request.projectile_id = static_cast<std::int32_t>(row.row_index);
    request.source_serial = 1234; // Manager must replace with its own source counter.
    request.projectile_manager = 0x9900;
    request.info_kind = projectile::SetInfoKind::angle_float;
    return request;
}

static void independent_pools_and_reuse(const std::string& assets) {
    const auto table = read_table(assets);
    const auto* arrow = data::projectile_row_v1(table, "ArrowDefault");
    const auto* laser = data::projectile_row_v1(table, "BlueLaser");
    assert(arrow != nullptr && !arrow->is_laser && laser != nullptr && laser->is_laser);

    objects::Owner object_manager;
    manager::Owner owner(object_manager, 0x9900);
    Fixture fixture{};
    fixture.owner = &owner;
    auto call_hooks = hooks(fixture);

    manager::Acquisition ordinary{};
    assert(owner.acquire(launch_for(*arrow), table, &ordinary) == manager::Status::ok);
    assert(ordinary.kind == manager::AcquisitionKind::fresh_product &&
           ordinary.serial == 0 && ordinary.factory_reservation.name == "Projectile_000");
    auto ordinary_product = product_for(ordinary, 0x1001);
    auto spawned = owner.spawn(&ordinary, &ordinary_product, call_hooks);
    if (spawned.status != manager::Status::ok) {
        std::fprintf(stderr, "spawn status=%u lifecycle=%u stage=%u registered=%d\n",
            static_cast<unsigned>(spawned.status),
            static_cast<unsigned>(spawned.lifecycle.status),
            static_cast<unsigned>(spawned.lifecycle.stopped_at),
            spawned.lifecycle.registered ? 1 : 0);
    }
    assert(spawned.status == manager::Status::ok && spawned.lifecycle.registered);
    assert(owner.ordinary_pool().slot_count == 1 &&
           owner.ordinary_pool().source_capacity == 1 &&
           owner.ordinary_pool().next_serial == 1 &&
           owner.ordinary_pool().active_count == 1);
    manager::SlotSnapshot slot{};
    assert(owner.slot(false, 0, &slot) && slot.active && slot.serial == 0 &&
           slot.source_handle == ordinary.source_handle);
    assert(owner.laser_pool().slot_count == 0 && fixture.set_manager_calls == 1);

    const auto ordinary_handle = ordinary.source_handle;
    fixture.calls.clear();
    auto released = owner.despawn(ordinary_handle, call_hooks);
    assert(released.status == projectile::Status::ok &&
           fixture.calls == std::vector<char>({'U', 's', 'A'}));
    assert(owner.ordinary_pool().active_count == 0 &&
           owner.ordinary_pool().next_index == 0 &&
           owner.ordinary_pool().next_serial == 1);
    assert(owner.slot(false, 0, &slot) && !slot.active);

    manager::Acquisition reused{};
    assert(owner.acquire(launch_for(*arrow), table, &reused) == manager::Status::ok);
    assert(reused.kind == manager::AcquisitionKind::pooled_product &&
           reused.source_handle == ordinary_handle && reused.serial == 0);
    fixture.calls.clear();
    spawned = owner.spawn(&reused, nullptr, call_hooks);
    assert(spawned.status == manager::Status::ok && spawned.lifecycle.registered);
    assert(fixture.calls == std::vector<char>({'u', 'i', 'a'}));
    assert(fixture.set_manager_calls == 1 && owner.ordinary_pool().active_count == 1 &&
           owner.ordinary_pool().next_serial == 1);

    manager::Acquisition laser_acquisition{};
    assert(owner.acquire(launch_for(*laser), table, &laser_acquisition) ==
           manager::Status::ok);
    assert(laser_acquisition.kind == manager::AcquisitionKind::fresh_product &&
           laser_acquisition.serial == 0 &&
           laser_acquisition.factory_reservation.name == "LTProjectile_000");
    auto laser_product = product_for(laser_acquisition, 0x1002);
    spawned = owner.spawn(&laser_acquisition, &laser_product, call_hooks);
    assert(spawned.status == manager::Status::ok &&
           owner.laser_pool().slot_count == 1 &&
           owner.laser_pool().source_capacity == 1 &&
           owner.laser_pool().next_serial == 1 &&
           owner.laser_pool().active_count == 1);
    assert(owner.ordinary_pool().slot_count == 1 &&
           owner.ordinary_pool().next_serial == 1 &&
           fixture.set_manager_calls == 2);

    manager::Acquisition ordinary_growth{};
    assert(owner.acquire(launch_for(*arrow), table, &ordinary_growth) ==
           manager::Status::ok);
    assert(ordinary_growth.kind == manager::AcquisitionKind::fresh_product &&
           ordinary_growth.serial == 1 &&
           ordinary_growth.factory_reservation.name == "Projectile_001");
    auto second_ordinary_product = product_for(ordinary_growth, 0x1003);
    spawned = owner.spawn(&ordinary_growth, &second_ordinary_product, call_hooks);
    assert(spawned.status == manager::Status::ok &&
           owner.ordinary_pool().slot_count == 2 &&
           owner.ordinary_pool().source_capacity == 2 &&
           owner.ordinary_pool().next_serial == 2 &&
           owner.ordinary_pool().active_count == 2);
    assert(owner.laser_pool().slot_count == 1 &&
           owner.laser_pool().source_capacity == 1 &&
           owner.laser_pool().next_serial == 1);
}

int main(int argc, char** argv) {
    assert(argc == 2);
    independent_pools_and_reuse(argv[1]);
}
