#pragma once

#include "character_aggro_character_list.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::character::aggro::object_manager_list {

// Normalized std::list<ObjectBase*> node. This is deliberately not an overlay
// on the ARM32 node: the original layout is next/previous/value (3 x 4 bytes),
// while this host structure uses native pointer width.
struct Node {
  Node* next{};
  Node* previous{};
  void* character{};
};

struct Owner {
  Node sentinel{};
  std::size_t character_count{};
  // App-side roster proof uses this epoch to reject bindings retained across
  // Add/Remove/clear. It is a semantic generation, not an engine ABI field.
  std::uint64_t mutation_generation{};
};

enum class Status {
  ok,
  invalid_argument,
  malformed_links,
  allocation_failed,
  free_failed,
};

struct AddProjection {
  // True only when GetObjectByName returned an ObjectHandle that resolved to
  // an existing GameObject. Add returns through its duplicate path then.
  bool duplicate_resolved{};
  // Result of the subsequently-created object's ObjectHandle -> Character*
  // conversion. Null means the object is not a Character; no list node is
  // allocated for it.
  void* character{};
};

using AllocateNode = Node* (*)(void* context);
using FreeNode = void (*)(void* context, Node* node);

// Borrowed live-link view for the current flat owner. This cursor never
// overlays an ABI std::list node and never snapshots the ring: Next reads the
// current normalized owner Node's `next` link when called.
struct CharacterCursor {
  const Owner* owner{};
  Node* current{};
  std::size_t steps{};
  bool initialized{};
};

// Initialize the owner-owned circular sentinel exactly as ObjectManager's
// constructor initializes +0x60/+0x64. The owner owns its nodes, not Characters.
Status initialize(Owner* owner) noexcept;

// Apply only ObjectManager::Add's CharacterList projection after the preceding
// source lookup/object registration has completed. Unique non-Character
// objects do not modify this list. Resolved-name duplicates never append.
Status append_after_add(Owner* owner, const AddProjection& projection,
                        AllocateNode allocate, void* context,
                        bool* appended) noexcept;

// ObjectManager::Remove first resolves the GameObject and Character pointer,
// then removes every list node with that Character value while preserving the
// order of other entries. The owner frees each node; it does not destroy the
// Character object.
Status remove_after_remove(Owner* owner, void* character,
                           FreeNode release, void* context,
                           std::size_t* removed) noexcept;

aggro_search::Status cursor_reset(void* context) noexcept;
aggro_search::Status cursor_at_end(void* context, std::uint32_t* output) noexcept;
aggro_search::Status cursor_get(void* context, aggro_search::GameObject** output) noexcept;
aggro_search::Status cursor_get_char(void* context,
                                     aggro_search::Character** output) noexcept;
aggro_search::Status cursor_next(void* context) noexcept;

// Direct adapter for the ObjectListMethods overload. The returned descriptor
// borrows the supplied cursor; both must remain stable for the synchronous
// search call. All callbacks re-read the current node rather than copying the
// owner's links.
aggro_character_list::ObjectListMethods object_list_methods(
    CharacterCursor* cursor) noexcept;

bool validate(const Owner* owner) noexcept;

}  // namespace dh2::character::aggro::object_manager_list
