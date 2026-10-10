#include "../../../android-native/app/src/main/cpp/source_campaign_character_fsm_v101.hpp"

#include <memory>
#include <string>
#include <type_traits>

using Record = dh2::world::CanonicalCharacterCandidateRecordV60;
using Borrow = model_renderer::SourceCharacterPathBorrowV105;
using RecordPathBorrow = bool (*)(
    const std::shared_ptr<Record>&, Borrow&, std::string&);

static_assert(std::is_same_v<
    decltype(static_cast<RecordPathBorrow>(
        &model_renderer::borrow_source_campaign_character_path_v105)),
    RecordPathBorrow>);
static_assert(std::is_same_v<decltype(Borrow::record_lease), std::shared_ptr<void>>);
static_assert(std::is_same_v<decltype(Borrow::context_lease), std::shared_ptr<void>>);
static_assert(std::is_same_v<decltype(Borrow::character), std::uintptr_t>);
static_assert(std::is_same_v<decltype(Borrow::machine), dh2::character::NativeFsm24*>);
static_assert(std::is_same_v<decltype(Borrow::path), dh2::navigation::PathObject*>);

// Compile-only public contract: the record overload returns a typed path
// pointer plus record/FSM leases and identity. Runtime users still need an
// actual initialized canonical record to exercise the loan.
