#include "../projectile_lifecycle_runtime_v1.hpp"
#include "../projectile_factory_owner_v1.hpp"
#include "../../game-data/projectile_table_v1.hpp"

#include <cassert>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace projectile = dh2::projectile_lifecycle_runtime_v1;
namespace objects = dh2::object_manager_runtime_owner_v1;
namespace factory_owner = dh2::projectile_factory_owner_v1;

struct Fixture {
    std::vector<char> calls;
    objects::Address manager{};
    objects::GameObject* expected_owner{};
    objects::GameObject* expected_target{};
    const dh2::data::ProjectileRowV1* expected_source_record{};
    std::int32_t expected_projectile_id{4};
    projectile::ProjectileCallback expected_check_callback{0x1234};
    projectile::ProjectileCallback expected_hit_callback{0x5678};
    objects::Address expected_user_data{0x7788};
    float expected_angle{17.5f};
    bool fail_stop{};
    bool fail_info{};
    int float_calls{};
    int bool_calls{};
    int expire_type{};
    bool characters{true};
    bool enemy{true};
    bool owner_target_allowed{true};
    std::int32_t collision_result{1};
};

static projectile::Status set_manager(void* context, objects::GameObject& object,
                                      objects::Address manager) noexcept {
    auto& f = *static_cast<Fixture*>(context);
    assert(object.identity != 0);
    assert(manager == f.manager);
    f.calls.push_back('m');
    return projectile::Status::ok;
}

static projectile::Status set_updating(void* context, objects::GameObject&,
                                       bool enabled) noexcept {
    auto& f = *static_cast<Fixture*>(context);
    f.calls.push_back(enabled ? 'u' : 'U');
    return projectile::Status::ok;
}

static projectile::Status set_info_float(
    void* context, objects::GameObject&, const projectile::SetInfoArgs& args,
    float angle) noexcept {
    auto& f = *static_cast<Fixture*>(context);
    ++f.float_calls;
    f.calls.push_back('f');
    assert(args.projectile_id == f.expected_projectile_id);
    assert(args.owner == f.expected_owner);
    assert(args.target == f.expected_target);
    assert(args.collision_check_callback == f.expected_check_callback &&
           args.collision_hit_callback == f.expected_hit_callback);
    assert(args.user_data == f.expected_user_data);
    assert(args.source_record == f.expected_source_record);
    if (args.source_record != nullptr) {
        assert(args.source_record->row_index ==
               static_cast<std::uint32_t>(args.projectile_id));
        assert(!args.source_record->model_path.empty());
    }
    assert(angle == f.expected_angle);
    return f.fail_info ? projectile::Status::hook_failed : projectile::Status::ok;
}

static projectile::Status set_info_bool(
    void* context, objects::GameObject&, const projectile::SetInfoArgs&,
    bool flag) noexcept {
    auto& f = *static_cast<Fixture*>(context);
    ++f.bool_calls;
    f.calls.push_back(flag ? 'b' : 'B');
    return projectile::Status::ok;
}

static projectile::Status stop(void* context, objects::GameObject&) noexcept {
    auto& f = *static_cast<Fixture*>(context);
    f.calls.push_back('s');
    return f.fail_stop ? projectile::Status::hook_failed : projectile::Status::ok;
}

static projectile::Status set_active(void* context, objects::GameObject&,
                                     bool active) noexcept {
    static_cast<Fixture*>(context)->calls.push_back(active ? 'a' : 'A');
    return projectile::Status::ok;
}

static projectile::Status snap_to_floor(void* context,
                                        objects::GameObject&) noexcept {
    static_cast<Fixture*>(context)->calls.push_back('F');
    return projectile::Status::ok;
}

static projectile::Status target_position(void* context, objects::GameObject&,
                                          projectile::Point3f* out) noexcept {
    auto& f = *static_cast<Fixture*>(context);
    f.calls.push_back('p');
    assert(out != nullptr);
    *out = {1.25f, 2.5f, 3.75f};
    return projectile::Status::ok;
}

static projectile::Status impact_fx(void* context, objects::GameObject&,
                                    std::int32_t impact_type,
                                    const projectile::Point3f& position) noexcept {
    auto& f = *static_cast<Fixture*>(context);
    f.calls.push_back('X');
    f.expire_type = impact_type;
    assert(position.x == 1.25f && position.y == 2.5f && position.z == 3.75f);
    return projectile::Status::ok;
}

static bool is_character(void* context, objects::GameObject&) noexcept {
    return static_cast<Fixture*>(context)->characters;
}

static bool is_enemy(void* context, objects::GameObject&,
                     objects::GameObject&) noexcept {
    return static_cast<Fixture*>(context)->enemy;
}

static bool owner_target_allowed(void* context, objects::GameObject&,
                                 objects::GameObject&) noexcept {
    return static_cast<Fixture*>(context)->owner_target_allowed;
}

static std::int32_t check_collision(void* context, objects::GameObject&,
                                   projectile::ProjectileCallback callback,
                                   objects::Address user_data) noexcept {
    auto& f = *static_cast<Fixture*>(context);
    f.calls.push_back('c');
    assert(callback == 0xC1 && user_data == 0x7788);
    return f.collision_result;
}

static projectile::Status collision_hit(void* context, objects::GameObject&,
                                        projectile::ProjectileCallback callback,
                                        objects::Address user_data) noexcept {
    auto& f = *static_cast<Fixture*>(context);
    f.calls.push_back('h');
    assert(callback == 0xC2 && user_data == 0x7788);
    return projectile::Status::ok;
}

static projectile::Status collision_impact_fx(
    void* context, objects::GameObject&, objects::GameObject&,
    const projectile::Point2f& point) noexcept {
    auto& f = *static_cast<Fixture*>(context);
    f.calls.push_back('i');
    assert(point.x == 4.0f && point.y == 5.0f);
    return projectile::Status::ok;
}

static projectile::Hooks hooks(Fixture& fixture) {
    projectile::Hooks result{};
    result.context = &fixture;
    result.set_manager = set_manager;
    result.set_updating = set_updating;
    result.set_info_float = set_info_float;
    result.set_info_bool = set_info_bool;
    result.stop = stop;
    result.set_active = set_active;
    result.snap_to_floor = snap_to_floor;
    result.get_target_position = target_position;
    result.handle_impact_fx = impact_fx;
    result.is_character = is_character;
    result.is_enemy = is_enemy;
    result.owner_target_allowed = owner_target_allowed;
    result.check_collision = check_collision;
    result.on_collision_hit = collision_hit;
    result.handle_collision_impact_fx = collision_impact_fx;
    return result;
}

static objects::GameObject add(objects::Owner& owner, int handle,
                               objects::Address identity, std::string_view name) {
    objects::GameObject object{};
    object.identity = identity;
    objects::GameObject* stored = nullptr;
    assert(owner.add_named_object(handle, name, object, &stored) == objects::Status::ok);
    assert(stored != nullptr);
    return object;
}

static projectile::SpawnRequest request_for(std::string name = "Projectile_009") {
    projectile::SpawnRequest request{};
    request.projectile_id = 4;
    request.projectile_table_size = 12;
    request.factory_serial = 9;
    request.factory_product.source_handle = 77;
    request.factory_product.name = name;
    request.factory_product.object.identity = 300;
    request.projectile_manager = 0x99;
    request.owner_identity = 100;
    request.target_identity = 200;
    request.collision_check_callback = 0x1234;
    request.collision_hit_callback = 0x5678;
    request.user_data = 0x7788;
    request.angle_degrees = 17.5f;
    return request;
}

static void canonical_factory_reservation_bridge() {
    objects::Owner manager;
    (void)add(manager, 1, 100, "player");
    (void)add(manager, 2, 200, "target");
    factory_owner::Owner factory(manager);
    factory_owner::Request source{};
    source.projectile_id = 4;
    source.projectile_table_size = 12;
    source.source_serial = 9; // passed through from ProjectileManager::_Create
    source.projectile_manager = 0x99;
    source.owner_identity = 100;
    source.target_identity = 200;
    source.collision_check_callback = 0x1234;
    source.collision_hit_callback = 0x5678;
    source.user_data = 0x7788;
    source.angle_degrees = 17.5f;

    factory_owner::Reservation reservation{};
    assert(factory.reserve(source, &reservation) == factory_owner::Status::ok);
    assert(reservation.source_handle == 3 &&
           reservation.name == "Projectile_009" &&
           reservation.type == projectile::FactoryType::projectile &&
           manager.find_by_source_handle(3)->identity == 0);

    Fixture fixture{};
    fixture.manager = source.projectile_manager;
    fixture.expected_owner = manager.find_by_identity(source.owner_identity);
    fixture.expected_target = manager.find_by_identity(source.target_identity);
    projectile::FactoryProduct product{};
    product.source_handle = reservation.source_handle;
    product.name = reservation.name;
    product.type = reservation.type;
    product.object.identity = 0x300;
    const auto result = factory.spawn(&reservation, product, hooks(fixture));
    assert(result.status == factory_owner::Status::ok &&
           result.lifecycle.status == projectile::Status::ok &&
           result.lifecycle.registered && reservation.attempted);
    assert(result.lifecycle.object == manager.find_by_source_handle(3) &&
           result.lifecycle.object == manager.find_by_name("Projectile_009"));
    assert((fixture.calls == std::vector<char>{'m', 'u', 'f', 'a'}));
    assert(factory.spawn(&reservation, product, hooks(fixture)).status ==
           factory_owner::Status::already_attempted);

    factory_owner::Request laser = source;
    laser.laser_type = true;
    laser.source_serial = 4; // caller-owned laser-pool serial
    factory_owner::Reservation laser_reservation{};
    assert(factory.reserve(laser, &laser_reservation) == factory_owner::Status::ok);
    assert(laser_reservation.name == "LTProjectile_004" &&
           laser_reservation.type == projectile::FactoryType::laser_type_projectile);

    factory_owner::Request invalid = source;
    invalid.source_serial = 9;
    factory_owner::Reservation duplicate{};
    assert(factory.reserve(invalid, &duplicate) ==
           factory_owner::Status::reservation_failed);
}

static std::vector<std::uint8_t> read_asset(const std::string& root,
                                            const char* name) {
    std::ifstream input(root + "/" + name, std::ios::binary);
    assert(input);
    return {std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()};
}

static void source_table_factory_selection(const std::string& asset_data) {
    const auto records = read_asset(asset_data, "projectiles_pyarray.bin");
    const auto names = read_asset(asset_data, "projectiles_pyarraynames.bin");
    const auto schema = read_asset(asset_data, "projectiles_pystructnames.bin");
    const auto dict_names = read_asset(
        asset_data, "projectiles_dictionary_pyarraynames.bin");
    const auto dict_values = read_asset(
        asset_data, "projectiles_dictionary_pyarray.bin");
    const auto view = [](const std::vector<std::uint8_t>& bytes) {
        return dh2::data::Bytes{bytes.data(), bytes.size()};
    };

    dh2::data::ProjectileTableV1 table{};
    std::string error;
    assert(dh2::data::load_projectile_table_v1(
        view(records), view(names), view(schema), view(dict_names),
        view(dict_values), table, error));
    assert(error.empty() && table.rows.size() == 41);
    const auto* const arrow = dh2::data::projectile_row_v1(table, "ArrowDefault");
    const auto* const laser = dh2::data::projectile_row_v1(table, "BlueLaser");
    assert(arrow != nullptr && arrow->model_path ==
        "data/3D/projectiles/projectile_arrow_default.bdae");
    assert(!arrow->is_laser && arrow->velocity > 0.0f);
    assert(laser != nullptr && laser->is_laser && laser->model_path ==
        "data/3D/projectiles/projectile_dh2_lightbeam_blue.bdae");
    assert(dh2::data::projectile_row_v1(table, -1) == nullptr);

    objects::Owner manager;
    (void)add(manager, 1, 100, "player");
    factory_owner::Owner factory(manager);
    factory_owner::Request request{};
    request.projectile_id = static_cast<std::int32_t>(laser->row_index);
    request.projectile_table_size = 1;  // replaced from the source table
    request.laser_type = false;         // also replaced from the source row
    request.source_serial = 4;
    request.projectile_manager = 0x99;
    request.owner_identity = 100;
    factory_owner::Reservation reservation{};
    assert(factory.reserve_from_source(request, table, &reservation) ==
           factory_owner::Status::ok);
    assert(reservation.name == "LTProjectile_004" &&
           reservation.type == projectile::FactoryType::laser_type_projectile);
    assert(reservation.request.projectile_table_size == 41 &&
           reservation.request.laser_type && reservation.source_record_valid &&
           reservation.source_record.table_name == "BlueLaser" &&
           reservation.source_record.model_path == laser->model_path);

    Fixture launch{};
    launch.manager = request.projectile_manager;
    launch.expected_owner = manager.find_by_identity(request.owner_identity);
    launch.expected_projectile_id = request.projectile_id;
    launch.expected_source_record = &reservation.source_record;
    launch.expected_check_callback = request.collision_check_callback;
    launch.expected_hit_callback = request.collision_hit_callback;
    launch.expected_user_data = request.user_data;
    launch.expected_angle = request.angle_degrees;
    projectile::FactoryProduct product{};
    product.source_handle = reservation.source_handle;
    product.name = reservation.name;
    product.type = reservation.type;
    product.object.identity = 0x123456;
    auto result = factory.spawn(&reservation, product, hooks(launch));
    assert(result.status == factory_owner::Status::ok &&
           result.lifecycle.status == projectile::Status::ok &&
           result.lifecycle.object != nullptr);
    assert((launch.calls == std::vector<char>{'m', 'u', 'f', 'a'}));

    request.projectile_id = static_cast<std::int32_t>(arrow->row_index);
    request.source_serial = 9;
    request.laser_type = true;
    factory_owner::Reservation normal{};
    assert(factory.reserve_from_source(request, table, &normal) ==
           factory_owner::Status::ok);
    assert(normal.name == "Projectile_009" &&
           normal.type == projectile::FactoryType::projectile &&
           !normal.request.laser_type && normal.source_record_valid &&
           normal.source_record.model_path == arrow->model_path);

    auto mismatched = normal;
    mismatched.source_record.is_laser = true;
    projectile::FactoryProduct mismatched_product{};
    mismatched_product.source_handle = mismatched.source_handle;
    mismatched_product.name = mismatched.name;
    mismatched_product.type = mismatched.type;
    mismatched_product.object.identity = 0x123457;
    Fixture rejected{};
    rejected.manager = request.projectile_manager;
    const auto mismatch_result = factory.spawn(
        &mismatched, mismatched_product, hooks(rejected));
    const auto* const still_reserved = manager.find_by_source_handle(
        mismatched.source_handle);
    assert(mismatch_result.lifecycle.status ==
               projectile::Status::invalid_projectile_id &&
           still_reserved != nullptr && still_reserved->identity == 0 &&
           rejected.calls.empty());
}

int main(int argc, char** argv) {
    assert(projectile::factory_name(false, 9) == "Projectile_009");
    assert(projectile::factory_name(true, 9) == "LTProjectile_009");
    assert(projectile::factory_name(false, 1000) == "Projectile_1000");

    canonical_factory_reservation_bridge();
    if (argc > 1) source_table_factory_selection(argv[1]);

    objects::Owner owner;
    (void)add(owner, 1, 100, "player");
    (void)add(owner, 2, 200, "target");
    Fixture f{};
    f.manager = 0x99;
    f.expected_owner = owner.find_by_identity(100);
    f.expected_target = owner.find_by_identity(200);
    const auto service = hooks(f);

    auto request = request_for();
    auto result = projectile::spawn_fresh(owner, request, service);
    assert(result.status == projectile::Status::ok && result.registered);
    assert((f.calls == std::vector<char>{'m', 'u', 'f', 'a'}));
    assert(result.object == owner.find_by_source_handle(77));
    assert(result.object == owner.find_by_name("Projectile_009"));
    assert(result.object->name == "Projectile_009");
    assert(result.object->identity == 300);
    assert(f.float_calls == 1 && f.bool_calls == 0);

    // The post-move Projectile::Update slice retains separate dt reads,
    // wraps the signed lifetime like ARM SUB, then checks speed and range.
    float vertical_velocity = 100.0f;
    float vertical_rate = 0.0f;
    std::int32_t lifetime = 100;
    float max_distance = -1.0f;
    projectile::PostMoveFields post_move{
        &vertical_velocity, &vertical_rate, &lifetime, &max_distance,
        {0.0f, 0.0f, 0.0f}};
    auto update = projectile::after_game_object_update(
        owner, 77, post_move, 16, 17, {30.0f, 0.0f, 0.0f});
    assert(update.status == projectile::Status::ok);
    assert(update.disposition == projectile::UpdateStop::continue_update);
    assert(lifetime == 83 && vertical_velocity == 100.0f);

    // Lifetime expiry takes precedence over a simultaneous nonpositive speed.
    vertical_velocity = 1.0f;
    vertical_rate = 100.0f;
    lifetime = 16;
    max_distance = 0.0f;
    update = projectile::after_game_object_update(
        owner, 77, post_move, 100, 16, {0.0f, 0.0f, 0.0f});
    assert(update.disposition == projectile::UpdateStop::expire_type1);
    assert(update.stopped_at == projectile::Stage::update_lifetime);
    assert(lifetime == 0 && vertical_velocity == -9.0f);

    // After lifetime, speed expiry precedes max-distance expiry.
    lifetime = 100;
    update = projectile::after_game_object_update(
        owner, 77, post_move, 100, 5, {3.0f, 4.0f, 0.0f});
    assert(update.disposition == projectile::UpdateStop::expire_type1);
    assert(update.stopped_at == projectile::Stage::update_motion);
    assert(lifetime == 95);

    // Equal squared distance is the source's inclusive range-expiry boundary.
    vertical_velocity = 10.0f;
    vertical_rate = 0.0f;
    lifetime = 100;
    max_distance = 25.0f;
    update = projectile::after_game_object_update(
        owner, 77, post_move, 1, 1, {3.0f, 4.0f, 0.0f});
    assert(update.disposition == projectile::UpdateStop::expire_type1);
    assert(update.stopped_at == projectile::Stage::update_distance);

    // Signed lifetime underflow follows the ARM word operation exactly.
    lifetime = (-2147483647 - 1);
    max_distance = -1.0f;
    update = projectile::after_game_object_update(
        owner, 77, post_move, 0, 1, {0.0f, 0.0f, 0.0f});
    assert(update.disposition == projectile::UpdateStop::continue_update);
    assert(lifetime == 2147483647);

    // Source Spawn marks the object active last. Failed later hooks keep the
    // registered object and earlier effects rather than undoing ObjectManager.
    auto laser = request_for("LTProjectile_010");
    laser.projectile_id = 5;
    laser.factory_serial = 10;
    laser.laser_type = true;
    laser.factory_product.source_handle = 78;
    laser.factory_product.object.identity = 301;
    laser.factory_product.type = projectile::FactoryType::laser_type_projectile;
    laser.info_kind = projectile::SetInfoKind::flag_bool;
    laser.flag = true;
    result = projectile::spawn_fresh(owner, laser, service);
    assert(result.status == projectile::Status::ok && result.registered);
    assert((f.calls == std::vector<char>{'m', 'u', 'f', 'a', 'm', 'u', 'b', 'a'}));
    assert(result.object->source_handle == 78);
    assert(result.object->name == "LTProjectile_010");
    assert(f.bool_calls == 1);

    // Invalid table index and name/type mismatches fail before registration or hooks.
    const auto prior_calls = f.calls.size();
    auto invalid = request_for("Projectile_011");
    invalid.projectile_id = -1;
    assert(projectile::spawn_fresh(owner, invalid, service).status ==
           projectile::Status::invalid_projectile_id);
    invalid = request_for("Wrong_011");
    invalid.factory_serial = 11;
    assert(projectile::spawn_fresh(owner, invalid, service).status ==
           projectile::Status::factory_name_mismatch);
    invalid = request_for("LTProjectile_011");
    invalid.factory_serial = 11;
    invalid.laser_type = true;
    assert(projectile::spawn_fresh(owner, invalid, service).status ==
           projectile::Status::wrong_factory_type);
    assert(f.calls.size() == prior_calls && owner.object_count() == 4);

    // DeSpawn does not erase the canonical ObjectManager entry: it disables,
    // stops, then clears in-use in source order for pool reuse.
    f.calls.clear();
    result = projectile::despawn(owner, 77, service);
    assert(result.status == projectile::Status::ok && result.registered);
    assert((f.calls == std::vector<char>{'U', 's', 'A'}));
    assert(owner.find_by_source_handle(77) == result.object);
    objects::Address collision_target = 0x123456;
    std::uint8_t expired = 0;

    // Collision filters run before hit storage. An allowed hit stores target
    // and point before invoking the source check callback.
    std::uint8_t collision_blocked = 0;
    expired = 0;
    collision_target = 0;
    projectile::Point2f hit_point{};
    projectile::CollisionFields collision_fields{};
    collision_fields.collision_blocked = &collision_blocked;
    collision_fields.expired = &expired;
    collision_fields.collision_target = &collision_target;
    collision_fields.collision_point = &hit_point;
    collision_fields.owner_identity = 100;
    collision_fields.check_callback = 0xC1;
    collision_fields.hit_callback = 0xC2;
    collision_fields.user_data = 0x7788;
    projectile::CollisionRequest collision_request{200, {4.0f, 5.0f}};
    f.calls.clear();
    f.collision_result = 1;
    auto collision = projectile::on_collision(owner, 77, collision_fields,
                                               collision_request, service);
    assert(collision.status == projectile::Status::ok &&
           collision.source_return == 0);
    assert(collision.target == owner.find_by_identity(200));
    assert(collision_target == 200 && hit_point.x == 4.0f &&
           hit_point.y == 5.0f && expired == 0);
    assert((f.calls == std::vector<char>{'c'}));

    // Callback result 2 calls the optional hit callback, then collision FX.
    f.calls.clear();
    f.collision_result = 2;
    collision = projectile::on_collision(owner, 77, collision_fields,
                                        collision_request, service);
    assert(collision.status == projectile::Status::ok &&
           collision.source_return == 0);
    assert((f.calls == std::vector<char>{'c', 'h', 'i'}));

    // Non-enemy characters are filtered before the stored target changes.
    collision_target = 0xDEAD;
    f.enemy = false;
    collision = projectile::on_collision(owner, 77, collision_fields,
                                        collision_request, service);
    assert(collision.status == projectile::Status::ok &&
           collision.stopped_at == projectile::Stage::collision_relation);
    assert(collision_target == 0xDEAD);
    f.enemy = true;

    collision_fields.owner_target_restriction = true;
    f.owner_target_allowed = false;
    f.calls.clear();
    collision = projectile::on_collision(owner, 77, collision_fields,
                                        collision_request, service);
    assert(collision.status == projectile::Status::ok &&
           collision.stopped_at == projectile::Stage::collision_relation);
    assert(collision_target == 0xDEAD && f.calls.empty());
    f.owner_target_allowed = true;
    collision_fields.owner_target_restriction = false;

    // A callback result 3 runs OnExpire (including FX), then returns pending.
    f.calls.clear();
    expired = 0;
    collision_target = 0;
    f.collision_result = 3;
    collision_fields.align_to_floor = true;
    collision = projectile::on_collision(owner, 77, collision_fields,
                                        collision_request, service);
    assert(collision.status == projectile::Status::ok &&
           collision.source_return == 1 && expired == 1 &&
           collision_target == 0);
    assert((f.calls == std::vector<char>{'c', 'F', 'p', 'X'}));
    assert(f.expire_type == 1);

    // No check callback takes the default pending-expiry path. Disabled and
    // already-expired collisions return before resolving the target.
    f.calls.clear();
    collision_fields.check_callback = 0;
    expired = 0;
    collision = projectile::on_collision(owner, 77, collision_fields,
                                        collision_request, service);
    assert(collision.status == projectile::Status::ok &&
           collision.source_return == 1 && expired == 1);
    collision_blocked = 1;
    const auto object_count_before_gate = owner.object_count();
    collision = projectile::on_collision(owner, 77, collision_fields,
                                        {9999, {0.0f, 0.0f}}, service);
    assert(collision.status == projectile::Status::ok &&
           collision.stopped_at == projectile::Stage::collision_gate);
    assert(owner.object_count() == object_count_before_gate);
    collision_blocked = 0;
    assert(owner.find_by_name("Projectile_009") == result.object);

    // OnExpire optionally synchronizes to the floor, clears the collision
    // target, publishes the expiry byte, resolves the target position, then
    // runs impact FX. It does not despawn or remove the ObjectManager row.
    f.calls.clear();
    projectile::ExpireFields expire_fields{&collision_target, &expired};
    result = projectile::on_expire(owner, 77, true, 2, expire_fields, service);
    assert(result.status == projectile::Status::ok && result.registered);
    assert((f.calls == std::vector<char>{'F', 'p', 'X'}));
    assert(collision_target == 0 && expired == 1 && f.expire_type == 2);
    assert(owner.find_by_source_handle(77) == result.object);

    // Stop failure prevents the final in-use clear, retaining source ordering.
    f.calls.clear();
    f.fail_stop = true;
    result = projectile::despawn(owner, 77, service);
    assert(result.status == projectile::Status::hook_failed);
    assert(result.stopped_at == projectile::Stage::stop);
    assert((f.calls == std::vector<char>{'U', 's'}));
    f.fail_stop = false;

    // SetInfo failure occurs after named registration and SetUpdating but before
    // the source active byte is set. Do not invent rollback.
    f.calls.clear();
    f.fail_info = true;
    auto failed = request_for("Projectile_012");
    failed.factory_serial = 12;
    failed.factory_product.source_handle = 79;
    failed.factory_product.object.identity = 302;
    result = projectile::spawn_fresh(owner, failed, service);
    assert(result.status == projectile::Status::hook_failed && result.registered);
    assert(result.stopped_at == projectile::Stage::set_info);
    assert((f.calls == std::vector<char>{'m', 'u', 'f'}));
    assert(owner.find_by_name("Projectile_012") == result.object);

    return 0;
}
