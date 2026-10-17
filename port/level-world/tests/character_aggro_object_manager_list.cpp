#include "../character_aggro_object_manager_list.hpp"

#include <array>
#include <cstdio>
#include <vector>

namespace om = dh2::character::aggro::object_manager_list;

namespace {

struct Arena {
  std::array<om::Node, 16> nodes{};
  std::size_t used{};
  std::vector<om::Node*> released;
  bool fail_allocation{};
};

om::Node* allocate(void* context) {
  auto* arena = static_cast<Arena*>(context);
  if (arena->fail_allocation || arena->used == arena->nodes.size()) return nullptr;
  return &arena->nodes[arena->used++];
}

void release(void* context, om::Node* node) {
  static_cast<Arena*>(context)->released.push_back(node);
}

bool check(bool condition, const char* label) {
  if (condition) return true;
  std::fprintf(stderr, "FAIL: %s\n", label);
  return false;
}

std::vector<void*> values(const om::Owner& owner) {
  std::vector<void*> result;
  for (auto* node = owner.sentinel.next; node != &owner.sentinel; node = node->next) {
    result.push_back(node->character);
  }
  return result;
}

bool add_projection_semantics() {
  om::Owner owner{};
  Arena arena{};
  bool appended = false;
  bool ok = check(om::initialize(&owner) == om::Status::ok, "initialize circular owner list");
  ok &= check(om::append_after_add(&owner, {false, reinterpret_cast<void*>(0x101)},
                                   allocate, &arena, &appended) == om::Status::ok && appended,
              "new Character appends");
  auto* first = owner.sentinel.next;
  ok &= check(om::append_after_add(&owner, {false, nullptr}, allocate, &arena, &appended) ==
                  om::Status::ok && !appended,
              "non-Character object does not allocate or append");
  ok &= check(om::append_after_add(&owner, {true, reinterpret_cast<void*>(0x202)},
                                   allocate, &arena, &appended) == om::Status::ok && !appended,
              "resolved name duplicate exits without appending");
  ok &= check(om::append_after_add(&owner, {false, reinterpret_cast<void*>(0x101)},
                                   allocate, &arena, &appended) == om::Status::ok && appended,
              "source Add has no Character pointer deduplication gate");
  ok &= check(owner.character_count == 2 && values(owner) ==
                  std::vector<void*>({reinterpret_cast<void*>(0x101),
                                      reinterpret_cast<void*>(0x101)}),
              "append preserves duplicates and tail order");
  ok &= check(first->previous == &owner.sentinel &&
              owner.sentinel.previous->next == &owner.sentinel && om::validate(&owner),
              "sentinel links remain reciprocal");

  Arena failed{};
  failed.fail_allocation = true;
  const auto before = values(owner);
  ok &= check(om::append_after_add(&owner, {false, reinterpret_cast<void*>(0x303)},
                                   allocate, &failed, &appended) ==
                  om::Status::allocation_failed && !appended && values(owner) == before,
              "allocation failure leaves the list unchanged");
  return ok;
}

bool remove_all_matches_preserving_order() {
  om::Owner owner{};
  Arena arena{};
  bool ok = check(om::initialize(&owner) == om::Status::ok, "initialize removal owner");
  const auto a = reinterpret_cast<void*>(0xa);
  const auto b = reinterpret_cast<void*>(0xb);
  const auto c = reinterpret_cast<void*>(0xc);
  bool appended = false;
  for (void* value : {a, b, a, c}) {
    ok &= check(om::append_after_add(&owner, {false, value}, allocate, &arena, &appended) ==
                    om::Status::ok && appended,
                "populate source-order list fixture");
  }
  auto* first_a = owner.sentinel.next;
  auto* second_a = first_a->next->next;
  std::size_t removed = 99;
  ok &= check(om::remove_after_remove(&owner, a, release, &arena, &removed) == om::Status::ok &&
              removed == 2,
              "Remove deletes every matching node");
  ok &= check(values(owner) == std::vector<void*>({b, c}),
              "Remove preserves the order of remaining Characters");
  ok &= check(arena.released == std::vector<om::Node*>({first_a, second_a}),
              "Remove releases each matching node in encounter order");
  ok &= check(owner.character_count == 2 && om::validate(&owner),
              "removal leaves valid reciprocal links and count");
  removed = 99;
  ok &= check(om::remove_after_remove(&owner, reinterpret_cast<void*>(0xd), release, &arena,
                                      &removed) == om::Status::ok && removed == 0 &&
              values(owner) == std::vector<void*>({b, c}),
              "missing Character removal is a no-op");
  return ok;
}

bool malformed_input_is_rejected() {
  om::Owner owner{};
  bool appended = false;
  bool ok = check(om::initialize(&owner) == om::Status::ok, "initialize validation owner");
  owner.sentinel.next = nullptr;
  Arena arena{};
  ok &= check(om::append_after_add(&owner, {false, reinterpret_cast<void*>(0x101)},
                                   allocate, &arena, &appended) == om::Status::malformed_links,
              "append rejects malformed source-list topology");
  return ok;
}

bool borrowed_cursor_reads_live_links() {
  using namespace dh2::character::aggro_search;
  om::Owner owner{};
  om::Node first{}, skipped{};
  GameObject first_object{}, skipped_object{};
  Character first_character{}, skipped_character{};
  first_character = {1, &first_object, 0, 0};
  skipped_character = {2, &skipped_object, 0, 0};
  bool ok = check(om::initialize(&owner) == om::Status::ok, "initialize borrowed cursor owner");
  first = {&skipped, &owner.sentinel, &first_character};
  skipped = {&owner.sentinel, &first, &skipped_character};
  owner.sentinel.next = &first;
  owner.sentinel.previous = &skipped;
  owner.character_count = 2;
  om::CharacterCursor view{&owner};
  const auto methods = om::object_list_methods(&view);
  std::uint32_t at_end = 1;
  GameObject* object = nullptr;
  Character* character = nullptr;
  ok &= check(methods.reset(methods.context) == complete &&
              methods.at_end(methods.context, &at_end) == complete && at_end == 0,
              "reset observes the current source-list head");
  ok &= check(methods.get(methods.context, &object) == complete && object == &first_object &&
              methods.get_char(methods.context, &character) == complete &&
              character == &first_character,
              "Get/GetChar project the live Character node");
  // Simulate a source callback unlinking the second node after the first Get.
  // The cursor must observe first.next at Next time, not a captured sequence.
  first.next = &owner.sentinel;
  owner.sentinel.previous = &first;
  skipped.previous = nullptr;
  ok &= check(methods.next(methods.context) == complete &&
              methods.at_end(methods.context, &at_end) == complete && at_end == 1,
              "Next reads the mutated live link after callbacks");
  ok &= check(methods.get(methods.context, &object) == invalid_argument,
              "Get rejects the sentinel position");
  return ok;
}

}  // namespace

int main() {
  const bool ok = add_projection_semantics() && remove_all_matches_preserving_order() &&
                  malformed_input_is_rejected() && borrowed_cursor_reads_live_links();
  if (!ok) return 1;
  std::puts("object-manager Character-list projection checks passed");
  return 0;
}
