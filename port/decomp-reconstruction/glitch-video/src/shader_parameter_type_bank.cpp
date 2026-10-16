// EShaderParameterType.cpp's static table: the shared-string names of the shader parameter types, kept in the anonymous-namespace
// class SNextType (SNextType::StringsBank, ELF 0x9f6a78, 63 records) and released by the init step registered for it.
#include "glitch-core-io/core/shared_string_heap_ops.hpp"

namespace glitch::video {
namespace {

struct SNextType {
    // One shared-string record per E_SHADER_PARAMETER_TYPE, null until the name is first needed (filled by the file's global
    // constructor and guessShaderParameterType 0x5e2284, which are other rows).
    static core::detail::SSharedStringData* StringsBank[63];
};
core::detail::SSharedStringData* SNextType::StringsBank[63] = {};

// 0x5e1828 _ZN6glitch5video12_GLOBAL__N_14initEb
// The init step of the table: `false` is the teardown. Every record is taken out of the bank, and one whose use count drops to
// zero is released from the shared string heap (SData::release 0x6a4d9c). `true` does nothing here (the bank is filled lazily).
[[maybe_unused]] void init(bool initialize) {
    if (initialize) return;
    for (core::detail::SSharedStringData*& slot : SNextType::StringsBank) {
        core::detail::SSharedStringData* data = slot;
        slot = nullptr;
        if (data != nullptr && --data->useCount == 0) core::detail::sharedStringDataRelease(data);
    }
}

}  // namespace
}  // namespace glitch::video
