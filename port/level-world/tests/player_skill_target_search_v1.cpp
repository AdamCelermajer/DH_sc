#include "../character_target_search.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>

namespace {
using namespace dh2::target_search;

struct World {
    std::array<Object48, 4> objects{};
    std::array<Entry16, 4> entries{};
    Entry16 head{};
    Room16 room{};
    Room16 sentinel{};
    Registry8 registry{};
    List40 list{};
    std::array<Target24, 4> heap{};
    std::array<bool, 4> enemy{false, true, true, false};
    std::array<bool, 4> dead{false, false, false, true};
    std::array<bool, 4> player_character{true, false, false, false};
    std::array<bool, 4> neutral{false, false, false, false};
    std::array<bool, 4> friend_character{false, false, false, true};
    std::array<bool, 4> merchant{false, false, false, false};
    std::array<bool, 4> cleaner{false, false, false, false};
    std::array<bool, 4> flag_2fa{false, false, false, false};
    std::array<std::int32_t, 4> character_state{};
    bool last_entry_is_game_object = false;
    std::int32_t object_interaction_type = 8;
    unsigned interaction_type_calls = 0;
    Services16 services{this, invoke};

    World() {
        objects[0].identity = 99; // The live player Character.
        objects[0].visible = objects[0].has_target_position = 1;
        objects[0].character_word1314 = 1;
        objects[0].rotation = 0.0f;

        // A frontal target is deliberately farther away than the side target:
        // FrontalFirst must win over distance. The remaining two are a friend
        // in front and a dead enemy in front, both rejected by the source path.
        const float points[3][3]{{0.0f, -4.0f, 0.0f},
                                 {2.0f, -3.0f, 0.0f},
                                 {0.0f, -1.0f, 0.0f}};
        for (std::size_t i = 1; i < objects.size(); ++i) {
            objects[i].identity = 99 + i;
            objects[i].visible = objects[i].has_target_position = 1;
            for (int axis = 0; axis < 3; ++axis) {
                objects[i].position[axis] = points[i - 1][axis];
                objects[i].target_position[axis] = points[i - 1][axis];
            }
        }
        head.next = &entries[0];
        for (std::size_t i = 0; i < entries.size(); ++i)
            entries[i] = {i + 1 < entries.size() ? &entries[i + 1] : &head,
                          &objects[i]};
        room = {&sentinel, &head};
        sentinel = {&room, nullptr};
        registry = {&sentinel};
    }

    static int invoke(void* raw, const Request24* request, Response16* response) {
        auto& world = *static_cast<World*>(raw);
        *response = {};
        const auto index = [identity = request->other ? request->other : request->subject]() {
            return identity >= 99 && identity <= 102 ? std::size_t(identity - 99) : 4u;
        }();
        switch (static_cast<Service>(request->service)) {
        case resolve_character:
            if (index < world.objects.size() && !(world.last_entry_is_game_object && index == 3))
                response->word = reinterpret_cast<std::uintptr_t>(&world.objects[index]);
            break;
        case is_character: response->word = request->subject == 99; break;
        case is_player: response->word = index < world.player_character.size() && world.player_character[index]; break;
        case is_dead: response->word = index < world.dead.size() && world.dead[index]; break;
        case is_enemy: response->word = index < world.enemy.size() && world.enemy[index]; break;
        case is_neutral: response->word = index < world.neutral.size() && world.neutral[index]; break;
        case is_friend: response->word = index < world.friend_character.size() && world.friend_character[index]; break;
        case is_merchant: response->word = index < world.merchant.size() && world.merchant[index]; break;
        case is_cleaner: response->word = index < world.cleaner.size() && world.cleaner[index]; break;
        case has_character_flag_2fa: response->word = index < world.flag_2fa.size() && world.flag_2fa[index]; break;
        case dh2::target_search::character_state:
            if (index < world.character_state.size())
                response->word = static_cast<std::uintptr_t>(static_cast<std::intptr_t>(world.character_state[index]));
            break;
        case interaction_type:
            if (request->other != 99) return 1;
            ++world.interaction_type_calls;
            response->word = world.object_interaction_type;
            break;
        case is_zonable: response->word = 0; break;
        case is_interactive: response->word = 1; break;
        case interaction_radius: response->number = 0.0f; break;
        case melee_radius: response->number = 0.0f; break;
        default: return 1;
        }
        return 0;
    }
};

[[noreturn]] void fail(const char* message) {
    std::fprintf(stderr, "player skill target search: %s\n", message);
    std::abort();
}
}

int main() {
    World world;
    if (dh2_target_list_init(&world.list, world.heap.data(), world.heap.size(),
                             &world.objects[0], 2, &world.services))
        fail("could not initialize FrontalFirst list");
    if (dh2_target_search(&world.list, &world.registry, 8.0f,
                          3.1415927410125732f, &world.services))
        fail("could not search the current character room");
    if (world.list.count != 2) fail("Enemy filter must reject friend and dead Character");

    Target24 top{};
    if (world.list.heap[0].identity != 100)
        fail("FrontalFirst must select the frontal Bashdown target before the nearer side target");
    if (dh2_target_pop(&world.list, &top) || top.identity != 100)
        fail("GetTargetListTop/PopTargetList must return the same selected Character");
    if (dh2_target_pop(&world.list, &top) || top.identity != 101)
        fail("the next valid Enemy must remain in the target list");
    if (dh2_target_pop(&world.list, &top) != 2)
        fail("target list must be empty after both selected Characters are popped");

    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 2, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 0)
        fail("neutral filter must reject every non-neutral Character");
    world.neutral[1] = true;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 2, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 1 || world.list.heap[0].identity != 100)
        fail("neutral filter must select only source-classified neutral Characters");

    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 4, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 1 || world.list.heap[0].identity != 102)
        fail("friend filter must follow its source branch even for a dead Character");
    world.character_state[0] = 15;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 8, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 0)
        fail("dead-friend filter must reject when owner state is 15");
    world.character_state[0] = 0;
    world.character_state[3] = 16;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 8, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 0)
        fail("dead-friend filter must reject when candidate state is 16");
    world.character_state[3] = 0;

    world.player_character[2] = true;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 0x80, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 1 || world.list.heap[0].identity != 101)
        fail("player mask must select a source-classified player Character");
    world.player_character[2] = false;
    world.merchant[1] = true;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 0x10, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 1 || world.list.heap[0].identity != 100)
        fail("merchant mask must select a source-classified merchant Character");
    world.cleaner[2] = true;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 0x20, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 1 || world.list.heap[0].identity != 101)
        fail("cleaner mask must select a source-classified cleaner Character");
    world.flag_2fa[3] = true;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 0x40, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 1 || world.list.heap[0].identity != 102)
        fail("2FA mask must select a Character with the source flag byte set");

    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 1, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 0.2f, &world.services) ||
        world.list.count != 1 || world.list.heap[0].identity != 100)
        fail("cone filter must exclude the side target while keeping the frontal Enemy");
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 1, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f,
                          std::numeric_limits<float>::quiet_NaN(), &world.services) ||
        world.list.count != 2)
        fail("source rejection comparisons must accept every otherwise valid target for a NaN cone");

    constexpr std::uint32_t all_characters = 0x7fffffffu;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], all_characters, 0xffffffffu, 1,
                                      &world.services) ||
        world.list.character_flags != all_characters || world.list.object_filter != 0xffffffffu)
        fail("constructor must preserve both full-width source filter values");
    if (dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 3)
        fail("full-width all-character sentinel must bypass the character classification branches");

    world.last_entry_is_game_object = true;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 0, 3, 1,
                                      &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 1 || world.list.heap[0].identity != 102 ||
        world.interaction_type_calls != 0)
        fail("object filter 3 must accept the source-valid non-Character object");
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 0, 0, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 1 || world.list.heap[0].identity != 102 ||
        world.interaction_type_calls != 1)
        fail("object filter 0 must call InteractionType and accept type 8");
    world.interaction_type_calls = 0;
    world.object_interaction_type = -1;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 0, 1, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 0 || world.interaction_type_calls != 1)
        fail("object filter 1 must call InteractionType and reject type -1");
    world.object_interaction_type = 7;
    world.interaction_type_calls = 0;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 0, 1, 1, &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 1 || world.list.heap[0].identity != 102 ||
        world.interaction_type_calls != 1)
        fail("object filter 1 must accept interaction types other than -1");
    world.object_interaction_type = 8;
    world.interaction_type_calls = 0;
    if (dh2_target_list_init_filtered(&world.list, world.heap.data(), world.heap.size(),
                                      &world.objects[0], 0, 0x10003u, 1,
                                      &world.services) ||
        dh2_target_search(&world.list, &world.registry, 8.0f, 3.1416f, &world.services) ||
        world.list.count != 0)
        fail("object filter must preserve high bits instead of truncating to 3");

    World centered;
    const float explicit_center[3]={2.0f,-3.0f,0.0f};
    if (dh2_target_list_init(&centered.list,centered.heap.data(),centered.heap.size(),
                             &centered.objects[0],1,&centered.services) ||
        dh2_target_search_at(&centered.list,&centered.registry,0.25f,3.1416f,
                             explicit_center,&centered.services) ||
        centered.list.count!=1 || centered.list.heap[0].identity!=101 ||
        centered.list.owner!=&centered.objects[0] ||
        centered.objects[0].position[0]!=0.0f ||
        centered.objects[0].position[1]!=0.0f)
        fail("explicit search center must select locally without replacing/moving its owner");

    std::puts("player skill target search PASS: source character masks, full-width filters, cone, priority, and pop order");
}
