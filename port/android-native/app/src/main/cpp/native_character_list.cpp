#include "native_character_list.hpp"

#include <algorithm>
#include <new>
#include <stdexcept>

namespace dh2::native::character_list {
namespace source = character::aggro::object_manager_list;
namespace runtime_factory = dh2::character_runtime_factory_v1;

namespace {
int factory_enroll(void* context, Owner::Character* character,
                   bool duplicate_resolved, bool* appended) {
    return static_cast<int>(static_cast<Owner*>(context)->enroll_after_add(
        character, duplicate_resolved, appended));
}

int factory_remove(void* context, Owner::Character* character,
                   std::size_t* removed) {
    return static_cast<int>(static_cast<Owner*>(context)->remove_after_remove(
        character, removed));
}
} // namespace

Owner::Owner() {
    source::initialize(&source_);
    cursor_ = {&source_};
    methods_ = source::object_list_methods(&cursor_);
}

Owner::Node* Owner::allocate(void* context) noexcept {
    auto& self = *static_cast<Owner*>(context);
    try {
        auto node = std::make_unique<Node>();
        auto* pointer = node.get();
        self.nodes_.push_back(std::move(node));
        return pointer;
    } catch (...) {return nullptr;}
}

void Owner::release(void* context, Node* node) {
    auto& self = *static_cast<Owner*>(context);
    const auto found = std::find_if(self.nodes_.begin(), self.nodes_.end(),
        [node](const auto& owned) {return owned.get() == node;});
    if (found == self.nodes_.end()) throw std::runtime_error("Character-list node is not owned");
    self.nodes_.erase(found);
}

Owner::Status Owner::enroll_after_add(Character* character, bool duplicate, bool* appended) {
    return source::append_after_add(&source_, {duplicate, character}, allocate, this, appended);
}

Owner::Status Owner::remove_after_remove(Character* character, std::size_t* removed) {
    return source::remove_after_remove(&source_, character, release, this, removed);
}

bool Owner::contains_identity(std::uintptr_t identity,
                              std::size_t* occurrences) const noexcept {
    if (occurrences) *occurrences = 0;
    if (identity == 0) return false;
    std::size_t count = 0;
    for (const auto& node : nodes_) {
        if (node && node->character) {
            const auto* character = static_cast<const Character*>(node->character);
            if (character->identity == identity) ++count;
        }
    }
    if (occurrences) *occurrences = count;
    return count != 0;
}

void Owner::clear() noexcept {
    nodes_.clear();
    source::initialize(&source_);
    cursor_ = {&source_};
}

runtime_factory::RosterServices factory_services(Owner& owner) noexcept {
    return {&owner, &factory_enroll, &factory_remove};
}

} // namespace dh2::native::character_list
