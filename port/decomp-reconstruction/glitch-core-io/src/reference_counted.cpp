#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch {

// 0x31d584 _ZNK6glitch17IReferenceCounted4dropEv
bool IReferenceCounted::drop() const {
    int previous = referenceCounter;
    referenceCounter = previous - 1;
    if (previous - 1 != 0) return false;
    // The original calls the onDelete slot first, then the deleting destructor.
    const_cast<IReferenceCounted*>(this)->onDelete();
    delete const_cast<IReferenceCounted*>(this);
    return true;
}

}  // namespace glitch
