#include "canonical_colbox_c1_v70.hpp"
#include <utility>
namespace dh2::world {
CanonicalColBoxC1V70::CanonicalColBoxC1V70(std::shared_ptr<void> owner,actor::RuntimeState& runtime):
 base_(reinterpret_cast<std::uintptr_t>(this),20,std::move(owner),runtime){
 // 341010/341014/341038 store0 at37c/374/378; the native dimensions cell
 // above is the same source storage for future actual ColBox continuations.
 base_.lifecycle().static84=1; // actual factory store341034
}
}

