#include "../projectile_product_owner_v1.hpp"
#include "../character_skill_combat_roll_v1.hpp"

#include <cassert>

namespace {
struct DeliveryFixture {
    std::uint32_t callbacks{};
    std::uint32_t applied{};
    std::uintptr_t projectile{};
    std::uintptr_t target{};
    std::uintptr_t script{};

    static bool is_character(void*, dh2::object_manager_runtime_owner_v1::GameObject&) noexcept {
        return true;
    }
    static bool is_enemy(void*, dh2::object_manager_runtime_owner_v1::GameObject&,
                         dh2::object_manager_runtime_owner_v1::GameObject&) noexcept {
        return true;
    }
    static std::int32_t check(void* raw,
        dh2::object_manager_runtime_owner_v1::GameObject& projectile,
        dh2::projectile_lifecycle_runtime_v1::ProjectileCallback callback,
        dh2::object_manager_runtime_owner_v1::Address user_data) noexcept {
        auto& f=*static_cast<DeliveryFixture*>(raw);
        assert(projectile.identity==f.projectile&&callback==0x3daff4&&user_data==f.script);
        ++f.callbacks;
        return 0; // Source Check queues +0x3d1; Update calls Hit next.
    }
    static dh2::projectile_lifecycle_runtime_v1::Status hit(void* raw,
        dh2::object_manager_runtime_owner_v1::GameObject& projectile,
        dh2::projectile_lifecycle_runtime_v1::ProjectileCallback callback,
        dh2::object_manager_runtime_owner_v1::Address user_data) noexcept {
        auto& f=*static_cast<DeliveryFixture*>(raw);
        assert(projectile.identity==f.projectile&&callback==0x3dae7c&&user_data==f.script);
        ++f.callbacks;
        // Stand-in for the retained VM's OnDelayedSkill -> existing
        // SkillCombatRoll native -> same F_ApplyResult owner.
        dh2::character_skill_combat_roll_v1::Request request{
            0x101,2,2,3,dh2::character_skill_combat_roll_v1::userdata_type,f.target};
        const dh2::character_skill_combat_roll_v1::Services services{
            &f,metadata,resolve,nullptr,nullptr,nullptr,nullptr,attack,apply};
        dh2::character_skill_combat_roll_v1::Result result{};
        std::string error;
        return dh2::character_skill_combat_roll_v1::execute(
            &request,&services,&result,error)==
                dh2::character_skill_combat_roll_v1::Status::complete&&
            result.attack_count==1 ?
                dh2::projectile_lifecycle_runtime_v1::Status::ok :
                dh2::projectile_lifecycle_runtime_v1::Status::hook_failed;
    }
    static int metadata(void*,std::uintptr_t character,std::uint32_t index,
        std::uint32_t* type,std::uint32_t* flags,std::int32_t* element,
        std::string&) noexcept {
        assert(character==0x101&&index==2);*type=2;*flags=0;*element=4;return 0;
    }
    static int resolve(void* raw,std::uintptr_t userdata,std::uintptr_t* target,
        std::string&) noexcept {
        auto& f=*static_cast<DeliveryFixture*>(raw);assert(userdata==f.target);
        *target=f.target;return 0;
    }
    static int attack(void* raw,std::uintptr_t attacker,std::uintptr_t target,
        std::uint32_t,std::int32_t element,dh2::data::CombatResult* result,
        std::string&) noexcept {
        auto& f=*static_cast<DeliveryFixture*>(raw);
        assert(attacker==0x101&&target==f.target&&element==4);
        result->amount=37;return 0;
    }
    static int apply(void* raw,std::uintptr_t attacker,std::uintptr_t target,
        std::uint32_t,dh2::data::CombatResult* result,std::string&) noexcept {
        auto& f=*static_cast<DeliveryFixture*>(raw);
        assert(attacker==0x101&&target==f.target&&result->amount==37);
        ++f.applied;return 0;
    }
    static dh2::projectile_lifecycle_runtime_v1::Status impact(void*,
        dh2::object_manager_runtime_owner_v1::GameObject&,
        dh2::object_manager_runtime_owner_v1::GameObject&,
        const dh2::projectile_lifecycle_runtime_v1::Point2f&) noexcept {
        return dh2::projectile_lifecycle_runtime_v1::Status::ok;
    }
    dh2::projectile_lifecycle_runtime_v1::Hooks hooks() {
        dh2::projectile_lifecycle_runtime_v1::Hooks h{};h.context=this;
        h.is_character=is_character;h.is_enemy=is_enemy;
        h.check_collision=check;h.on_collision_hit=hit;
        h.handle_collision_impact_fx=impact;return h;
    }
};
}

int main() {
    using namespace dh2;
    object_manager_runtime_owner_v1::Owner objects;
    object_manager_runtime_owner_v1::GameObject character{};
    character.identity=0x101;
    character.world_x=12.0f;
    character.world_y=34.0f;
    object_manager_runtime_owner_v1::GameObject* stored=nullptr;
    assert(objects.add_named_object(1,"Player",character,&stored)==
        object_manager_runtime_owner_v1::Status::ok);
    assert(stored&&stored->identity==character.identity);
    object_manager_runtime_owner_v1::GameObject target{};
    target.identity=0x404;
    assert(objects.add_named_object(2,"Target",target,&stored)==
        object_manager_runtime_owner_v1::Status::ok);

    data::ProjectileTableV1 table{};
    data::ProjectileRowV1 row{};
    row.row_index=0;
    row.model_path="data/3d/projectiles/projectile_arrow_default.bdae";
    row.model_dictionary_name="Projectiles";
    row.velocity=1.0f;
    table.rows.push_back(row);

    projectile_product_owner_v1::Owner projectiles(objects,table,0x202);
    skill_projectile_delivery_v1::Launch launch{};
    launch.character=character.identity;
    launch.skill_script=0x303;
    launch.skill_index=2;
    launch.projectile_id=0;
    launch.collision_check_callback=0x3daff4;
    launch.collision_hit_callback=0x3dae7c;
    const auto spawned=projectiles.spawn(launch);
    assert(spawned.status==projectile_product_owner_v1::Status::ok&&
        spawned.projectile&&objects.find_by_identity(spawned.projectile));
    const auto* payload=projectiles.find(spawned.projectile);
    assert(payload&&payload->source_handle>=0&&payload->projectile_id==0&&
        payload->skill_index==2&&
        payload->owner_identity==character.identity&&payload->user_data==0x303&&
        payload->check_callback==0x3daff4&&payload->hit_callback==0x3dae7c&&
        payload->active&&payload->manager_identity==0x202);
    assert(objects.staged_update_object_count()==1);
    DeliveryFixture delivery{};
    delivery.projectile=spawned.projectile;delivery.target=target.identity;
    delivery.script=0x303;
    const auto contact=projectiles.on_collision(spawned.projectile,target.identity,
        {1.5f,2.5f},delivery.hooks());
    assert(contact.status==projectile_lifecycle_runtime_v1::Status::ok&&
        contact.source_return==1&&contact.projectile&&contact.target&&
        delivery.callbacks==1&&delivery.applied==0&&
        payload->collision_expired==1&&
        payload->collision_target_identity==target.identity);
    // Projectile::Update consumes that pending target on the next owner tick.
    // Exercise its retained delayed-hit callback boundary after the contact
    // kernel, rather than fabricating the source-impossible Check result 2.
    const auto delayed=DeliveryFixture::hit(&delivery,*contact.projectile,
        payload->hit_callback,payload->user_data);
    assert(delayed==projectile_lifecycle_runtime_v1::Status::ok&&
        delivery.callbacks==2&&delivery.applied==1);
    assert(projectiles.set_target(spawned.projectile,character.identity)==
        projectile_product_owner_v1::Status::ok);
    assert(projectiles.find(spawned.projectile)->target_identity==character.identity);

    const auto reused=projectiles.spawn(launch);
    assert(reused.status==projectile_product_owner_v1::Status::ok&&
        reused.projectile&&reused.projectile!=spawned.projectile&&
        projectiles.manager().ordinary_pool().active_count==2);
}
