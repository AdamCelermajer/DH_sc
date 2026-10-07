#pragma once
namespace dh2::loader {
// Host constructor completion journal, not a native actor/world field. Only
// actual factory C1 invocation advances this; identity0 never implies D0.
enum class CanonicalConstructorStateV89 {prepared,constructing,completed,failed};
}
