#include "../monster_external_script_session.hpp"

#include <cstdio>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

using namespace dh2::monster_external_script;

namespace {
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

constexpr char common[] = "\n";
constexpr char external[] =
    "AddToVFTable('OnEnemySpotted','monster_OnEnemySpotted')\n"
    "AddToVFTable('OnTargetOutOfRange','monster_OnTargetOutOfRange')\n"
    "function monster_OnEnemySpotted()\n"
    " RegisterSummon(7,4); RegisterSummon(7,2); RegisterSummon(8)\n"
    " RegisterSummon(-1); RegisterSummon(9.5); RegisterSummon('bad')\n"
    " RegisterSummon(16,99)\n"
    "end\nfunction monster_OnTargetOutOfRange() end\n";

Source source(const char* text) { return {text, std::strlen(text)}; }

// Mirrors NativeMonsterInitialization member order: the Level projection is
// declared before Session, so Session destruction happens while the borrowed
// service pointer is still valid.
struct RetainedVmOwner {
    std::shared_ptr<dh2::character_oid_cache_v1::Owner> level_projection;
    Session session;
};
}

int main() {
    try {
        auto runtime_projection = std::make_shared<dh2::character_oid_cache_v1::Owner>();
        require(runtime_projection->begin_level(0x987654321ull, 16) ==
                    dh2::character_oid_cache_v1::Status::complete,
                "real Level identity/cache setup failed");
        Services services{};
        services.owner = 0x1234;
        services.current_level_oid_cache = runtime_projection.get();
        RetainedVmOwner vm_owner;
        vm_owner.level_projection = runtime_projection;
        std::string error;
        const auto initialized = vm_owner.session.initialize(source(common), source(external), services, error);
        if (initialized != Status::complete)
            throw std::runtime_error("configured VM initialization failed: " + error);
        // EGL recreation retires the RoomZone runtime but the logical Level
        // and session remain. The session-owned projection keeps the provider
        // alive after the runtime's reference is dropped.
        runtime_projection.reset();
        require(vm_owner.session.dispatch(Event::enemy_spotted, 0x5678, error) == Status::complete,
                "configured RegisterSummon call failed in the original VM trampoline");
        dh2::character_oid_cache_v1::Result value{};
        require(vm_owner.level_projection->count(7, &value) == dh2::character_oid_cache_v1::Status::complete && value.value == 4,
                "VM RegisterSummon did not retain max(existing,requested)");
        require(vm_owner.level_projection->count(8, &value) == dh2::character_oid_cache_v1::Status::complete && value.value == 1,
                "VM RegisterSummon did not apply default count one");
        require(vm_owner.level_projection->count(9, &value) == dh2::character_oid_cache_v1::Status::complete && value.value == 0,
                "VM RegisterSummon accepted a non-integer argument");
        require(vm_owner.level_projection->entry_count() == 2,
                "VM RegisterSummon inserted an invalid or out-of-range CharacterTable ID");
        const auto level_generation=vm_owner.level_projection->generation();
        require(vm_owner.level_projection->clear_level(0x987654321ull,level_generation)==
                    dh2::character_oid_cache_v1::Status::complete&&
                    vm_owner.level_projection->count(7,&value)==
                    dh2::character_oid_cache_v1::Status::no_level,
                "terminal Level retirement must clear the cache while its VM still retains storage");
        require(vm_owner.session.dispatch(Event::enemy_spotted,0x5678,error)==
                    Status::script_error&&
                    error.find("RegisterSummon requires the current Level Character OID cache provider")!=
                    std::string::npos,
                "retired Level VM must fail closed while its retained provider remains alive");

        Services missing_services{};
        missing_services.owner = 0x1234;
        Session missing;
        require(missing.initialize(source(common), source(external), missing_services, error) == Status::complete &&
                    missing.dispatch(Event::enemy_spotted, 0x5678, error) == Status::script_error &&
                    error.find("RegisterSummon requires the current Level Character OID cache provider") !=
                        std::string::npos,
                "unwired production session did not fail closed with the precise Level-provider requirement");
        std::puts("PASS RegisterSummon VM dispatch, retained Level lifetime, teardown, and missing-provider failure");
        return 0;
    } catch (const std::exception& exception) {
        std::fprintf(stderr, "RegisterSummon VM session: %s\n", exception.what());
        return 1;
    }
}
