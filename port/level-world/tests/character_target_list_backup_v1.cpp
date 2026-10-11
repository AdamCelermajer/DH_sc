#include "character_target_list_backup_v1.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <limits>
#include <string>

using namespace dh2::character_target_list_backup_v1;
using namespace dh2::target_search;

namespace {
struct Fixture {
    std::array<Object48, 3> objects{};
    std::map<Identity, std::uint32_t> locks{};
    Identity fail_resolve{};

    static bool resolve(void* raw, Identity identity, Object48** output,
                        std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        if (identity == self.fail_resolve) {
            error = "fixture resolve failure";
            return false;
        }
        for (auto& object : self.objects) {
            if (object.identity == identity) {
                *output = &object;
                return true;
            }
        }
        error = "unknown object identity";
        return false;
    }
    static bool lock(void* raw, Identity identity, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        Object48* object = nullptr;
        if (!resolve(raw, identity, &object, error) || !object) return false;
        ++self.locks[identity];
        return true;
    }
    static void unlock(void* raw, Identity identity) noexcept {
        auto& self = *static_cast<Fixture*>(raw);
        auto& count = self.locks[identity];
        if (count) --count;
    }
    ServicesV1 services() {
        return {this, resolve, lock, unlock};
    }
    static int search_service(void*, const Request24* request,
                              Response16* response) {
        *response = {};
        switch (request->service) {
        case is_character:
            response->word = request->subject == 999;
            return 0;
        case resolve_character:
            response->word = 0;
            return 0;
        case is_zonable:
            response->word = 0;
            return 0;
        case is_interactive:
            response->word = 1;
            return 0;
        case interaction_type:
            response->word = 8;
            return 0;
        case interaction_radius:
        case melee_radius:
            response->number = 0.f;
            return 0;
        default:
            return 1;
        }
    }
};

void require(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "%s\n", message);
        std::abort();
    }
}

void require_list(Owner& owner, const ServicesV1& services,
                  std::string_view name,
                  const std::array<Identity, 2>& expected,
                  std::size_t count) {
    Owner::ViewV1 view{};
    ObjectListMethods methods{};
    std::string error;
    require(owner.open(name, services, view, methods, error) == Status::complete,
            "named backup open failed");
    require(methods.reset(methods.context) == 0, "named backup reset failed");
    for (std::size_t i = 0; i < count; ++i) {
        std::uint32_t at_end = 1;
        Object48* object = nullptr;
        require(methods.at_end(methods.context, &at_end) == 0 && !at_end,
                "named backup ended before its source entries");
        require(methods.get(methods.context, &object) == 0 && object &&
                    object->identity == expected[i],
                "named backup did not return the same canonical GameObject row");
        require(methods.next(methods.context) == 0,
                "named backup next failed");
    }
    std::uint32_t at_end = 0;
    require(methods.at_end(methods.context, &at_end) == 0 && at_end,
            "named backup did not reach end");
}
} // namespace

int main() {
    Fixture fixture{};
    fixture.objects[0].identity = 101;
    fixture.objects[1].identity = 202;
    fixture.objects[2].identity = 303;
    const auto services = fixture.services();
    {
        Owner backup{};
        std::array<Target24, 2> targets{{{101, 4.f, 0.2f, 1, 0},
                                         {202, 9.f, 0.4f, 1, 0}}};
        List48 current{};
        current.heap = targets.data();
        current.count = 2;
        current.capacity = 2;
        std::string error;
        require(backup.backup("default", current, services, error) ==
                    Status::complete,
                "initial backup failed");
        require(backup.size("default") == 2 && fixture.locks[101] == 1 &&
                    fixture.locks[202] == 1,
                "backup did not retain each ObjectBase +0x29 lock");
        require_list(backup, services, "default", {101, 202}, 2);

        // Feed the named backup directly into the existing TargetList search
        // ObjectList seam; no second candidate registry is built.
        Owner::ViewV1 search_view{};
        ObjectListMethods methods{};
        require(backup.open("default", services, search_view, methods, error) ==
                    Status::complete,
                "search backup open failed");
        Object48 search_owner{};
        search_owner.identity = 999;
        search_owner.position[0] = search_owner.position[1] =
            search_owner.position[2] = 0.f;
        std::array<Target24, 4> search_heap{};
        List48 search{};
        Services16 search_services{&fixture, Fixture::search_service};
        require(dh2_target_list_init_filtered(&search, search_heap.data(),
                    static_cast<std::uint32_t>(search_heap.size()),
                    &search_owner, 1, 0, 1, &search_services) == 0,
                "search target list initialization failed");
        fixture.objects[0].position[1] = -1.f;
        fixture.objects[1].position[1] = -2.f;
        fixture.objects[0].visible = fixture.objects[1].visible = 1;
        require(dh2_target_search_object_list(&search, &methods, 3.f, 1.f,
                    &search_services) == 0 && search.count == 2,
                "TargetList search did not consume named backup candidates");
        require(dh2_target_list_init_filtered(&search, search_heap.data(),
                    static_cast<std::uint32_t>(search_heap.size()),
                    &search_owner, 0x7fffffffu, 3, 2, &search_services) == 0,
                "backup search rejected full-width filters or angle sorting");
        const float local_center[3]{0.f, -1.75f, 0.f};
        require(dh2_target_search_object_list_at(&search, &methods, .3f,
                    std::numeric_limits<float>::quiet_NaN(), local_center,
                    &search_services) == 0 && search.count == 1 &&
                    search.heap[0].identity == 202 &&
                    search_owner.position[1] == 0.f,
                "named candidate search lost explicit center, source cone, or filters");
        search.object_filter = 0x10003u;
        require(dh2_target_search_object_list(&search, &methods, 3.f, 3.1416f,
                    &search_services) == 0 && search.count == 0,
                "named candidate search truncated the full-width object filter");

        // A missing key is lazily materialized as an empty reusable list.
        require_list(backup, services, "missing", {0, 0}, 0);
        require(backup.size("missing") == 0,
                "missing source backup did not create an empty list");

        // A failed canonical lookup leaves the previous source snapshot intact.
        fixture.fail_resolve = 303;
        targets[0].identity = 303;
        current.count = 1;
        require(backup.backup("default", current, services, error) ==
                    Status::provider_failed && backup.size("default") == 2 &&
                    fixture.locks[101] == 1 && fixture.locks[202] == 1,
                "failed replacement damaged the retained snapshot");
        fixture.fail_resolve = 0;

        // Source BackupResults clears old refs before retaining the new list.
        targets[0].identity = 202;
        require(backup.backup("default", current, services, error) ==
                    Status::complete && backup.size("default") == 1 &&
                    fixture.locks[101] == 0 && fixture.locks[202] == 1,
                "replacement did not release and reacquire exact rows");
        require_list(backup, services, "default", {202, 0}, 1);
        require(backup.clear("default", error) == Status::complete &&
                    fixture.locks[202] == 0 && backup.size("default") == 0,
                "explicit clear did not release its canonical row lock");

        targets[0].identity = 101;
        require(backup.backup("default", current, services, error) ==
                    Status::complete && fixture.locks[101] == 1,
                "final retained row setup failed");
        Fixture next_level{};
        next_level.objects[0].identity = 101;
        const auto next_services = next_level.services();
        require(backup.retire(error) == Status::complete &&
                    fixture.locks[101] == 0,
                "Level retirement did not release old ObjectBase lock");
        require(backup.backup("default", current, next_services, error) ==
                    Status::complete && next_level.locks[101] == 1,
                "named backup did not rebind to the next Level generation");
        require(backup.retire(error) == Status::complete &&
                    next_level.locks[101] == 0,
                "second Level retirement leaked the rebound lock");
    }
    require(fixture.locks[101] == 0,
            "TargetList backup owner destruction leaked ObjectBase+0x29 lock");
    std::puts("{\"validation\":\"PASS\",\"named_snapshots\":true,\"canonical_rows\":true,\"source_locks_released\":true,\"mismatches\":0}");
    return 0;
}
