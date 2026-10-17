#include "character_aggro_object_manager_list.hpp"

#include <limits>

namespace dh2::character::aggro::object_manager_list {
namespace {

constexpr std::size_t kTraversalLimit = 1u << 20;

CharacterCursor* cursor(void* context) noexcept {
  return static_cast<CharacterCursor*>(context);
}

int cursor_reset_callback(void* context) noexcept {
  return static_cast<int>(cursor_reset(context));
}

int cursor_at_end_callback(void* context, std::uint32_t* output) noexcept {
  return static_cast<int>(cursor_at_end(context, output));
}

int cursor_get_callback(void* context, aggro_search::GameObject** output) noexcept {
  return static_cast<int>(cursor_get(context, output));
}

int cursor_get_char_callback(void* context, aggro_search::Character** output) noexcept {
  return static_cast<int>(cursor_get_char(context, output));
}

int cursor_next_callback(void* context) noexcept {
  return static_cast<int>(cursor_next(context));
}

}  // namespace

Status initialize(Owner* owner) noexcept {
  if (owner == nullptr) return Status::invalid_argument;
  if (owner->mutation_generation == std::numeric_limits<std::uint64_t>::max())
    return Status::malformed_links;
  ++owner->mutation_generation;
  owner->sentinel.next = &owner->sentinel;
  owner->sentinel.previous = &owner->sentinel;
  owner->sentinel.character = nullptr;
  owner->character_count = 0;
  return Status::ok;
}

bool validate(const Owner* owner) noexcept {
  if (owner == nullptr || owner->sentinel.next == nullptr ||
      owner->sentinel.previous == nullptr ||
      owner->sentinel.character != nullptr ||
      owner->character_count > kTraversalLimit) {
    return false;
  }

  const Node* previous = &owner->sentinel;
  const Node* current = owner->sentinel.next;
  std::size_t visited = 0;
  while (current != &owner->sentinel) {
    if (current == nullptr || current->previous != previous ||
        current->next == nullptr || current->character == nullptr ||
        visited >= owner->character_count || visited >= kTraversalLimit) {
      return false;
    }
    previous = current;
    current = current->next;
    ++visited;
  }
  return visited == owner->character_count &&
         owner->sentinel.previous == previous &&
         owner->sentinel.previous->next == &owner->sentinel;
}

Status append_after_add(Owner* owner, const AddProjection& projection,
                        AllocateNode allocate, void* context,
                        bool* appended) noexcept {
  if (owner == nullptr || appended == nullptr) return Status::invalid_argument;
  *appended = false;
  if (!validate(owner)) return Status::malformed_links;
  if (projection.duplicate_resolved || projection.character == nullptr) {
    return Status::ok;
  }
  if (allocate == nullptr) return Status::invalid_argument;
  if (owner->character_count == std::numeric_limits<std::size_t>::max() ||
      owner->mutation_generation == std::numeric_limits<std::uint64_t>::max()) {
    return Status::malformed_links;
  }

  Node* node = allocate(context);
  if (node == nullptr) return Status::allocation_failed;
  Node* tail = owner->sentinel.previous;
  node->next = &owner->sentinel;
  node->previous = tail;
  node->character = projection.character;
  tail->next = node;
  owner->sentinel.previous = node;
  ++owner->character_count;
  ++owner->mutation_generation;
  *appended = true;
  return validate(owner) ? Status::ok : Status::malformed_links;
}

Status remove_after_remove(Owner* owner, void* character,
                           FreeNode release, void* context,
                           std::size_t* removed) noexcept {
  if (owner == nullptr || removed == nullptr) return Status::invalid_argument;
  *removed = 0;
  if (!validate(owner)) return Status::malformed_links;
  if (character == nullptr) return Status::ok;
  if (release == nullptr) return Status::invalid_argument;

  // Find whether this operation mutates the list before unlinking anything.
  // At the practically unreachable epoch limit, fail atomically instead of
  // wrapping and allowing an old population binding to appear current.
  bool has_match = false;
  for (const Node* probe = owner->sentinel.next; probe != &owner->sentinel;
       probe = probe->next) {
    if (probe->character == character) { has_match = true; break; }
  }
  if (has_match && owner->mutation_generation ==
                       std::numeric_limits<std::uint64_t>::max()) {
    return Status::malformed_links;
  }

  Node* current = owner->sentinel.next;
  const std::size_t initial_count = owner->character_count;
  std::size_t visited = 0;
  while (current != &owner->sentinel) {
    if (current == nullptr || visited >= initial_count ||
        visited >= kTraversalLimit) {
      return Status::malformed_links;
    }
    Node* next = current->next;
    if (current->character == character) {
      current->previous->next = next;
      next->previous = current->previous;
      --owner->character_count;
      ++*removed;
      release(context, current);
    }
    current = next;
    ++visited;
  }
  if (*removed) ++owner->mutation_generation;
  return validate(owner) ? Status::ok : Status::malformed_links;
}

aggro_search::Status cursor_reset(void* context) noexcept {
  auto* view = cursor(context);
  if (view == nullptr || view->owner == nullptr || !validate(view->owner)) {
    return aggro_search::invalid_topology;
  }
  view->current = view->owner->sentinel.next;
  view->steps = 0;
  view->initialized = true;
  return aggro_search::complete;
}

aggro_search::Status cursor_at_end(void* context, std::uint32_t* output) noexcept {
  auto* view = cursor(context);
  if (view == nullptr || output == nullptr || !view->initialized ||
      view->owner == nullptr || view->current == nullptr) {
    return aggro_search::invalid_argument;
  }
  *output = view->current == &view->owner->sentinel ? 1U : 0U;
  return aggro_search::complete;
}

aggro_search::Status cursor_get(void* context, aggro_search::GameObject** output) noexcept {
  auto* view = cursor(context);
  if (view == nullptr || output == nullptr || !view->initialized ||
      view->owner == nullptr || view->current == nullptr ||
      view->current == &view->owner->sentinel) {
    return aggro_search::invalid_argument;
  }
  auto* character = static_cast<aggro_search::Character*>(view->current->character);
  if (character == nullptr || character->object == nullptr) {
    return aggro_search::invalid_topology;
  }
  *output = character->object;
  return aggro_search::complete;
}

aggro_search::Status cursor_get_char(void* context,
                                     aggro_search::Character** output) noexcept {
  auto* view = cursor(context);
  if (view == nullptr || output == nullptr || !view->initialized ||
      view->owner == nullptr || view->current == nullptr ||
      view->current == &view->owner->sentinel) {
    return aggro_search::invalid_argument;
  }
  auto* character = static_cast<aggro_search::Character*>(view->current->character);
  if (character == nullptr || character->object == nullptr) {
    return aggro_search::invalid_topology;
  }
  *output = character;
  return aggro_search::complete;
}

aggro_search::Status cursor_next(void* context) noexcept {
  auto* view = cursor(context);
  if (view == nullptr || !view->initialized || view->owner == nullptr ||
      view->current == nullptr || view->current == &view->owner->sentinel ||
      view->steps >= kTraversalLimit) {
    return aggro_search::invalid_argument;
  }
  // Read the live link only now, after the consumer's candidate callbacks.
  view->current = view->current->next;
  ++view->steps;
  return view->current == nullptr ? aggro_search::invalid_topology
                                  : aggro_search::complete;
}

aggro_character_list::ObjectListMethods object_list_methods(
    CharacterCursor* view) noexcept {
  return {view, cursor_reset_callback, cursor_at_end_callback, cursor_get_callback,
          cursor_get_char_callback, cursor_next_callback};
}

}  // namespace dh2::character::aggro::object_manager_list
