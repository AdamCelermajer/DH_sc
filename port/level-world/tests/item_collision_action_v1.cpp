#include "../item_collision_action_v1.hpp"

#include <cstdio>
#include <cstdlib>

using namespace dh2::item_collision_action_v1;
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

struct InteractContext {
    Owner* pool{};
    std::uintptr_t shell{};
    std::uintptr_t observed_character{};
    bool requeue{};
    bool completes{true};
};

bool interact(void* raw, std::uintptr_t character, std::string& error) {
    auto& context = *static_cast<InteractContext*>(raw);
    context.observed_character = character;
    if (context.requeue) {
        if (queue_deferred_interact(context.pool, context.shell, 0xdef, error) !=
            Status::complete)
            return false;
    }
    if (!context.completes) error = "inventory rejected the source Interact";
    return context.completes;
}
}

int main() {
    Owner pool;
    std::array<std::uintptr_t, slots_per_category> shells{101, 102, 103, 104, 105};
    std::string error;
    check(precache_category(&pool, 7, shells, error) == Status::complete,
          "ItemManager::PreCache binds stable shells before contact callbacks");
    SpawnPlan plan{};
    check(prepare_spawn(&pool, 7, 201, &plan, error) == Status::complete &&
          bind_shell(&pool, &plan, shells[0], error) == Status::complete &&
          activate(&pool, &plan, shells[0], 201, 0, error) == Status::complete,
          "source Spawn activates the contacted ItemObject shell");

    BeginResult begin{};
    BeginRequest request{true, false, 0xabc, -1, true, false, true};
    check(on_collision_begins(request, &pool, shells[0], &begin, error) ==
              Status::complete && begin.action == Action::deferred_interact &&
          begin.deferred_character_identity == 0xabc &&
          deferred_interact_for_shell(&pool, shells[0]) == 0xabc,
          "moving Character contact schedules the source +0x2e4 target");

    request.character_moving = false;
    check(on_collision_begins(request, &pool, shells[0], &begin, error) ==
              Status::complete && begin.action == Action::none &&
          deferred_interact_for_shell(&pool, shells[0]) == 0xabc,
          "nonmoving contact does not overwrite an existing source target");
    request.item_has_been_looted = true;
    check(on_collision_begins(request, &pool, shells[0], &begin, error) ==
              Status::complete && begin.action == Action::none,
          "looted ItemObject returns before the interaction branch");

    check(de_spawn(&pool, shells[0], 201, error) == Status::complete &&
          deferred_interact_for_shell(&pool, shells[0]) == 0xabc,
          "ItemManager::DeSpawn retains the independent +0x2e4 field");
    for (std::uintptr_t i = 0; i < slots_per_category; ++i) {
        const auto item = std::uintptr_t(202) + i;
        check(prepare_spawn(&pool, 7, item, &plan, error) == Status::complete &&
              bind_shell(&pool, &plan, shells[plan.slot_index], error) == Status::complete &&
              activate(&pool, &plan, shells[plan.slot_index], item, 0, error) == Status::complete,
              "source round-robin continues through shell reuse");
    }
    check(plan.slot_index == 0 &&
          deferred_interact_for_shell(&pool, shells[0]) == 0xabc,
          "five-slot shell reuse retains the pending source target");

    InteractContext context{&pool, shells[0], 0, true, true};
    UpdateResult update{};
    check(update_deferred_interact(&pool, shells[0], interact, &context,
              &update, error) == Status::complete && update.invoked &&
          update.callback_completed && update.character_identity == 0xabc &&
          context.observed_character == 0xabc &&
          deferred_interact_for_shell(&pool, shells[0]) == 0,
          "Update passes Character to Interact then clears a reentrant rewrite");

    check(queue_deferred_interact(&pool, shells[0], 0x123, error) ==
              Status::complete,
          "source collision can queue the next one-shot interaction");
    context.requeue = false;
    context.completes = false;
    check(update_deferred_interact(&pool, shells[0], interact, &context,
              &update, error) == Status::complete && update.invoked &&
          !update.callback_completed && error == "inventory rejected the source Interact" &&
          deferred_interact_for_shell(&pool, shells[0]) == 0,
          "Update clears +0x2e4 after a failed Interact callback");

    request.item_has_been_looted = false;
    request.interaction_type = 4;
    request.character_moving = false;
    request.item_is_character_target = true;
    check(on_collision_begins(request, &pool, shells[0], &begin, error) ==
              Status::complete && begin.action == Action::show_tooltip,
          "non--1 local-player target contact requests the source tooltip action");
    request.character_is_local_player = false;
    check(on_collision_begins(request, &pool, shells[0], &begin, error) ==
              Status::complete && begin.action == Action::none,
          "remote target contact does not show the local-player tooltip");
    check(on_collision_ends(true, false) ==
              Action::hide_tooltip_and_clear_character &&
          on_collision_ends(true, true) == Action::none &&
          on_collision_ends(false, false) == Action::none,
          "collision end hides and clears only for a non-target Character");

    std::printf("PASS item_collision_action_v1 checks=%u\n", checks);
    return 0;
}
