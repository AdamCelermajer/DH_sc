#include "../item_manager_pool_v1.hpp"

#include <cstdio>
#include <cstdlib>

using namespace dh2::item_manager_pool_v1;

namespace {
unsigned checks{};
void check(bool value, const char* message) {
    ++checks;
    if (!value) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        std::exit(1);
    }
}
void precache(Owner& owner, std::int32_t category_id, std::uintptr_t first_shell) {
    std::array<std::uintptr_t, slots_per_category> shells{};
    for (std::size_t i = 0; i < shells.size(); ++i) shells[i] = first_shell + i;
    std::string error;
    check(precache_category(&owner, category_id, shells, error) == Status::complete,
          "PreCache binds the five stable source-named shell identities");
}
void spawn(Owner& owner, std::int32_t category_id, std::uintptr_t shell,
           std::uintptr_t item, std::uint32_t lock,
           std::uintptr_t expected_eviction = 0) {
    SpawnPlan plan{};
    std::string error;
    check(prepare_spawn(&owner, category_id, item, &plan, error) == Status::complete,
          "source category/slot plan");
    check(plan.evicted_item_identity == expected_eviction,
          "five-slot source round-robin selected the expected eviction");
    if (expected_eviction) {
        const auto* group = find_category(&owner, category_id);
        check(group && plan.slot_index < group->slots.size(), "selected category slot");
        const auto old_shell = group->slots[plan.slot_index].shell_identity;
        check(de_spawn(&owner, old_shell, expected_eviction, error) == Status::complete,
              "source DeSpawn disables and clears the occupied slot");
    }
    check(bind_shell(&owner, &plan, shell, error) == Status::complete,
          "pre-cache binds stable shell identity");
    check(activate(&owner, &plan, shell, item, lock, error) == Status::complete,
          "source Spawn/InitAgain activates the pooled shell");
}
}

int main() {
    Owner owner;
    precache(owner, 7, 101);
    const auto* first_name = source_object_name(&owner, 7, 0);
    const auto* fifth_name = source_object_name(&owner, 7, 4);
    check(first_name && *first_name == "ItemObject_07_00" && fifth_name &&
          *fifth_name == "ItemObject_07_04",
          "PreCache reproduces the source category/slot object names");
    std::array<std::uintptr_t, slots_per_category> conflicting_shells{201, 202, 203, 204, 205};
    std::string error;
    check(precache_category(&owner, 7, conflicting_shells, error) ==
          Status::identity_conflict,
          "repeated PreCache cannot replace stable source shell identities");
    SpawnPlan missing{};
    check(prepare_spawn(&owner, 8, 800, &missing, error) == Status::not_precached,
          "Spawn fails closed before a source category has all five cached shells");
    for (std::uintptr_t i = 1; i <= slots_per_category; ++i)
        spawn(owner, 7, 100 + i, 200 + i, 0);
    check(owner.categories.size() == 1, "one AudioVisualID creates one category pool");
    check(owner.categories[0].slots.size() == 5, "source pre-cache has five shells");
    check(owner.categories[0].next_slot == 0, "five spawns wrap the category cursor");
    // Source Interact first transfers the ItemInstance to the Player's
    // canonical inventory, then ItemManager::DeSpawn only clears the pooled
    // ItemObject association. Reusing that shell must not erase/duplicate the
    // already-transferred identity.
    std::vector<std::uintptr_t> canonical_inventory;
    SpawnPlan reuse{};
    check(prepare_spawn(&owner, 7, 999, &reuse, error) == Status::complete &&
          reuse.evicted_item_identity == 201,
          "round-robin selects the canonical world Item attached to the shell");
    check(owner.categories[0].next_slot == 1,
          "Spawn advances the source cursor before DeSpawn and InitAgain");
    const auto* prior_category = find_category(&owner, 7);
    check(prior_category && prior_category->slots[reuse.slot_index].shell_identity == 101,
          "the selected source slot retains its stable ItemObject shell");
    canonical_inventory.push_back(201); // V4 move transferred the same identity.
    check(de_spawn(&owner, 101, 201, error) == Status::complete,
          "Interact de-spawns the shell after the canonical V4 transfer");
    check(canonical_inventory.size() == 1 && canonical_inventory.front() == 201 &&
          !enabled(&owner, 201),
          "de-spawn clears only the pooled reference; transferred Item stays owned once");
    check(bind_shell(&owner, &reuse, 101, error) == Status::complete &&
          activate(&owner, &reuse, 101, 999, 0, error) == Status::complete,
          "the de-spawned shell can be reused for the next source Item");
    check(canonical_inventory.size() == 1 && canonical_inventory.front() == 201 &&
          enabled(&owner, 999),
          "shell reuse does not duplicate or lose the prior V4 inventory Item");
    check(!enabled(&owner, 201), "evicted item is disabled");
    check(enabled(&owner, 999), "new item is enabled in the reused shell");
    check(passes_disabled_save_gate(&owner, 999),
          "enabled ItemObject passes the source Disabled() save gate");

    check(set_pickup_lock(&owner, 999, 5000, 27), "source drop starts its five-second player-scoped lock");
    check(pickup_locked(&owner, 999), "pickup is blocked while source lock is positive");
    check(pickup_locked_for_player(&owner,999,27),
          "Interact rejects the PlayerInfo ID recorded by DropInventory");
    check(!pickup_locked_for_player(&owner,999,28),
          "Interact allows another PlayerInfo ID during the source timer");
    advance_pickup_locks(&owner, 4999);
    check(pickup_locked(&owner, 999), "source lock remains for the first 4999 ms");
    check(pickup_locked_for_player(&owner,999,27),
          "the source player remains blocked until its timer reaches zero");
    advance_pickup_locks(&owner, 1);
    check(!pickup_locked(&owner, 999), "source lock reaches zero at 5000 ms");
    check(!pickup_locked_for_player(&owner,999,27),
          "expired source timer no longer rejects its recorded player");

    check(de_spawn(&owner, 101, 999, error) == Status::complete,
          "source de-spawn keeps the shell but clears its item and lock");
    check(!enabled(&owner, 999) && !passes_disabled_save_gate(&owner, 999),
          "disabled ItemObject fails the source save gate");
    check(owner.categories[0].slots[0].shell_identity == 101,
          "DeSpawn retains the pre-cached ItemObject shell identity");
    check(source_object_name(&owner, 7, 0) &&
          *source_object_name(&owner, 7, 0) == "ItemObject_07_00",
          "DeSpawn preserves the exact source shell name for later reuse");

    precache(owner, 8, 801);
    spawn(owner, 8, 801, 901, 0);
    check(owner.categories.size() == 2, "AudioVisualID uses an independent pool");
    SpawnPlan failed_init{};
    check(prepare_spawn(&owner, 8, 902, &failed_init, error) == Status::complete &&
          failed_init.slot_index == 1 && owner.categories[1].next_slot == 2,
          "a valid Spawn attempt reserves its source slot before InitAgain");
    check(activate(&owner, &failed_init, 9999, 902, 0, error) ==
              Status::identity_conflict && owner.categories[1].next_slot == 2,
          "failed shell initialization does not rewind the already-advanced cursor");
    spawn(owner, 8, 803, 903, 0);
    check(owner.categories[1].slots[2].item_identity == 903 &&
          owner.categories[1].next_slot == 3,
          "the next successful spawn follows the source cursor after a failed InitAgain");
    std::printf("PASS item_manager_pool_v1 checks=%u\n", checks);
    return 0;
}
