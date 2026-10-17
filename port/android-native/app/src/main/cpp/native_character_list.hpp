#pragma once

#include "../../../../../../port/level-world/character_aggro_object_manager_list.hpp"
#include "../../../../../../port/level-world/character_runtime_factory_v1.hpp"
#include <memory>
#include <vector>

namespace dh2::native::character_list {

// Native ownership for the bounded ObjectManager Character-list projection.
// Enrollment is called after a real native Character exists. The caller
// supplies the resolved-name duplicate fact; full ObjectManager map/factory
// registration is outside this adapter. Nodes are owned; Characters borrowed.
class Owner final {
public:
    using Source = character::aggro::object_manager_list::Owner;
    using Node = character::aggro::object_manager_list::Node;
    using Status = character::aggro::object_manager_list::Status;
    using Character = character::aggro_search::Character;

    Owner();
    Owner(const Owner&) = delete;
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&&) = delete;
    Owner& operator=(Owner&&) = delete;

    Status enroll_after_add(Character*, bool duplicate_resolved, bool* appended);
    Status remove_after_remove(Character*, std::size_t* removed);
    bool contains_identity(std::uintptr_t identity,
                          std::size_t* occurrences = nullptr) const noexcept;
    // Native teardown clears owned nodes and resets the cursor. It does not
    // claim original ObjectManager destructor/map/group cleanup behavior.
    void clear() noexcept;
    const character::aggro_character_list::ObjectListMethods& methods() const noexcept {return methods_;}
    const Source& source() const noexcept {return source_;}
    std::size_t owned_nodes() const noexcept {return nodes_.size();}

private:
    static Node* allocate(void*) noexcept;
    static void release(void*, Node*);
    Source source_{};
    character::aggro::object_manager_list::CharacterCursor cursor_{};
    character::aggro_character_list::ObjectListMethods methods_{};
    std::vector<std::unique_ptr<Node>> nodes_;
};

// Direct bridge for the selected level-world Character factory. It borrows
// this roster owner; the factory owns the Character projection it enrolls.
dh2::character_runtime_factory_v1::RosterServices factory_services(
    Owner& owner) noexcept;

} // namespace dh2::native::character_list
