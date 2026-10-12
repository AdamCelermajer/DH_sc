#pragma once
#include <canonical_gameobject_base_owner_v1.hpp>
#include <memory>
namespace dh2::world {
// Concrete distinct ColBox owner recovered from the INLINED source factory
// 340fe0..341040: allocate380; GameObjectC1(20)38c398; dimension374/378/37c
// zero; ColBox vtables; static84=1. Semantic native fields, not ARM layout.
// No Floor/Decor/Zone subtype or zero-filled allocation parity is invented.
class CanonicalColBoxC1V70 final {
 CanonicalGameObjectBaseOwnerV1 base_;
 std::array<float,3> dimensions374_{};
public:
 CanonicalColBoxC1V70(std::shared_ptr<void> independent_owner,actor::RuntimeState&);
 CanonicalColBoxC1V70(const CanonicalColBoxC1V70&)=delete;
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 std::array<float,3>& source_dimensions374()noexcept{return dimensions374_;}
 const std::array<float,3>& source_dimensions374()const noexcept{return dimensions374_;}
 // C1-only authority. Original DeclareProperties/InitPost/D0 source closure
 // is not present in retained extracts; do not advertise a production entry
 // or substitute inherited-only properties/no-op lifecycle for this class.
};
}
