// Host test for GameObject::_GetTargetListTop (0x38eb68) as seen by the Player
// skill VM: five values for a non-empty TargetList, the null Pointer alone for
// an empty one. Skill and spell scripts read
//   local target, _, _, gameobject = GetTargetListTop()
// and branch on the fourth value (IsCharacter), so the arity is behaviour.
#include "../player_skill_target_top_v1.hpp"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
struct Value {
    std::uint32_t type, reserved;
    float number;
    std::uint32_t boolean;
    const char* text;
    std::size_t text_bytes;
    std::uintptr_t identity;
};
static_assert(sizeof(Value) == 40, "source value record");

void check(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        std::exit(1);
    }
}

dh2::target_search::List48 list_over(std::array<dh2::target_search::Target24, 3>& heap,
                                     std::uint32_t count) {
    dh2::target_search::List48 list{};
    list.heap = heap.data();
    list.count = count;
    list.capacity = static_cast<std::uint32_t>(heap.size());
    return list;
}
} // namespace

int main() {
    using dh2::player_skill_target_top_v1::write;
    std::array<dh2::target_search::Target24, 3> heap{};

    // Empty list: pushPointer(nullptr), exactly one value, a Pointer (tag 2).
    {
        auto list = list_over(heap, 0);
        std::array<Value, 5> out{};
        out.fill(Value{99, 99, 99.f, 99, nullptr, 99, 99});
        std::uint32_t returned = 99;
        check(write(list, out.data(), 5, &returned) == 0, "empty list write failed");
        check(returned == 1, "empty list returns exactly one value");
        check(out[0].type == 2 && out[0].identity == 0, "empty list value is the null Pointer");
        check(out[1].type == 99, "empty list wrote past its single value");

        dh2::target_search::List48 never_searched{};
        returned = 99;
        check(write(never_searched, out.data(), 5, &returned) == 0 && returned == 1 &&
              out[0].type == 2 && out[0].identity == 0,
              "a list that was never searched is empty too");
        check(write(list, static_cast<Value*>(nullptr), 0, &returned) == 1 && returned == 0,
              "empty list without a result buffer must report failure");
    }

    // Character entry: userdata, distance, degrees, IsCharacter true, extra.
    {
        heap[0] = {0x100000042ull, 250.5f, 0.5f, 1u, 0u};
        auto list = list_over(heap, 1);
        std::array<Value, 5> out{};
        std::uint32_t returned = 0;
        check(write(list, out.data(), 5, &returned) == 0, "character entry write failed");
        check(returned == 5, "a non-empty list returns five values");
        check(out[0].type == 2 && out[0].identity == 0x100000042ull, "value 1 is the target identity");
        check(out[1].type == 3 && out[1].number == 250.5f, "value 2 is the distance");
        check(out[2].type == 3 &&
              out[2].number == 0.5f * dh2::player_skill_target_top_v1::degrees_per_radian(),
              "value 3 is the angle in degrees (single-precision 0x42652EE0 scale)");
        check(out[2].number > 28.64f && out[2].number < 28.65f, "degree scale is about 57.2958");
        check(out[3].type == 1 && out[3].boolean == 1, "value 4 is IsCharacter");
        check(out[4].type == 3 && out[4].number == 0.f, "value 5 is the extra TargetInfo word");
    }

    // Non-Character (breakable object) entry: IsCharacter false; only bit 0 counts.
    {
        heap[0] = {0x100000099ull, 10.f, 0.f, 0u, 0u};
        auto list = list_over(heap, 1);
        std::array<Value, 5> out{};
        std::uint32_t returned = 0;
        check(write(list, out.data(), 5, &returned) == 0 && returned == 5, "object entry write failed");
        check(out[3].type == 1 && out[3].boolean == 0, "object entry must report IsCharacter false");

        heap[0].flags = 0xfffffffeu;
        check(write(list, out.data(), 5, &returned) == 0 && out[3].boolean == 0,
              "only the low flag bit selects IsCharacter");
        heap[0].flags = 0x3u;
        check(write(list, out.data(), 5, &returned) == 0 && out[3].boolean == 1,
              "low flag bit set selects IsCharacter");
    }

    // The top of the heap is returned; later entries stay for PopTargetList.
    {
        heap[0] = {0xaaull, 1.f, 0.f, 1u, 0u};
        heap[1] = {0xbbull, 2.f, 0.f, 1u, 0u};
        auto list = list_over(heap, 2);
        std::array<Value, 5> out{};
        std::uint32_t returned = 0;
        check(write(list, out.data(), 5, &returned) == 0 && out[0].identity == 0xaaull,
              "GetTargetListTop must not consume or reorder the list");
        check(list.count == 2, "GetTargetListTop must not pop");
    }

    // The extra word is a float bit pattern, not an integer.
    {
        const float extra = 3.25f;
        std::uint32_t bits;
        std::memcpy(&bits, &extra, sizeof(bits));
        heap[0] = {0xccull, 1.f, 0.f, 1u, bits};
        auto list = list_over(heap, 1);
        std::array<Value, 5> out{};
        std::uint32_t returned = 0;
        check(write(list, out.data(), 5, &returned) == 0 && out[4].number == 3.25f,
              "extra word is pushed as a single-precision number");
    }

    // Capacity guards leave the result count at zero.
    {
        heap[0] = {0xddull, 1.f, 0.f, 1u, 0u};
        auto list = list_over(heap, 1);
        std::array<Value, 5> out{};
        std::uint32_t returned = 7;
        check(write(list, out.data(), 4, &returned) == 1 && returned == 0,
              "a four-value buffer cannot hold the five source values");
        check(write(list, static_cast<Value*>(nullptr), 5, &returned) == 1 && returned == 0,
              "a missing buffer is a failure, not a silent no-op");
    }
    std::puts("{\"validation\":\"PASS\",\"cases\":6}");
    return 0;
}
