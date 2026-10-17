#pragma once
#include "quest_runtime_fields_v1.hpp"
#include "../quest-data/quests.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace dh2::data::quest_table_bindings_v1 {
using PyDataRef=quest_runtime_fields_v1::PyDataRef;
struct State;
struct Input {
    // Borrow the already existing immutable packed table and retain its owner.
    // This adapter never copies file bytes or creates/imports a Lua dataset.
    dh2_quest_table table{};
    std::shared_ptr<const void> packed_owner;
    const std::uint8_t* names=nullptr;std::size_t names_size=0;
    std::shared_ptr<const void> names_owner;
};
struct ListRef {
    const PyDataRef* row=nullptr;std::uint32_t kind=0;
    const dh2_quest_list* definition=nullptr;
};
struct StubRef {
    const PyDataRef* row=nullptr;std::uint32_t offset=0;
    const dh2_quest_objective* definition=nullptr;
};
struct Span {const std::uint8_t* data=nullptr;std::uint32_t size=0;};
class View {
    std::shared_ptr<const State> state_;
    explicit View(std::shared_ptr<const State> state):state_(std::move(state)){}
    friend class Owner;
public:
    View()=default;
    explicit operator bool() const noexcept{return bool(state_);}
    std::uint32_t count() const noexcept;
    // Source-stride semantic identity anchors; never reinterpret packed bytes
    // or access these anchors as native Structs::v2Quest fields/vtables.
    std::uintptr_t rows_identity() const noexcept;
    const PyDataRef* row(std::uint32_t) const noexcept;
    const PyDataRef* resolve(std::uintptr_t source_row_identity) const noexcept;
    const dh2_quest_record* record(const PyDataRef&) const noexcept;
    const char* definition_name(std::uint32_t) const noexcept;
    const ListRef* list(const PyDataRef&,std::uint32_t kind) const noexcept;
    const ListRef* resolve_list(std::uintptr_t native_list_identity) const noexcept;
    const StubRef* resolve_stub(std::uintptr_t source_stub_identity) const noexcept;
    bool read_word(const PyDataRef&,std::uint32_t source_offset,std::uintptr_t*,std::string&) const;
    bool list_record(const ListRef&,std::uint32_t index,dh2_quest_span*,std::string&) const;
    bool objective(const ListRef&,std::uint32_t index,dh2_quest_objective*,std::string&) const;
    bool bytes(const dh2_quest_span&,Span*,std::string&) const;
};
class Owner {
    std::shared_ptr<const State> state_;
public:
    // Atomic adaptation using the reused port/quest-data decoder. A rejected
    // import preserves the current generation and every retained old View.
    bool load(const Input&,std::string&);
    View borrow() const noexcept{return View(state_);}
};
// A View owns a lease on its one immutable definition generation. Its row/list/
// stub/name borrows remain stable until the last retained View is destroyed.
// Each actual Quest factory/session must retain that same View while Record
// py_data_68 points at its controls. No new quest gameplay authority is created.
} // namespace dh2::data::quest_table_bindings_v1
