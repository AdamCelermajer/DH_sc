// Slice 15 test: forwarding, vtable-slot and key-based paths of the animation_track
// applicator wrappers. The applicator vtable and CInterpreter helpers are not mapped,
// so this file supplies stand-ins that record their calls.
#include <cstdint>
#include <cstdio>

#include "glitch-scene-collada-gui/recon/slice_15.hpp"

namespace glitch::collada::recon_15 {

static int g_vtable148_calls = 0;
static int g_vtable164_calls = 0;
static void* g_last_value = nullptr;

int applicatorVtable148(CApplicatorInfo*) { ++g_vtable148_calls; return 7; }
int applicatorVtable148(CApplicatorInfo*, void* value) {
    ++g_vtable148_calls;
    g_last_value = value;
    return 8;
}
int applicatorVtable164(CApplicatorInfo*) { ++g_vtable164_calls; return 9; }
int applicatorVtable164(CApplicatorInfo*, void* value) {
    ++g_vtable164_calls;
    g_last_value = value;
    return 10;
}
void interpreterGetKeyBasedValueEx(const SAnimationAccessor&, int key, void* out) {
    auto* words = static_cast<std::uint32_t*>(out);
    words[0] = static_cast<std::uint32_t>(key) + 1u;
    words[1] = 0;
    words[2] = 0;
}

}  // namespace glitch::collada::recon_15

namespace r = glitch::collada::recon_15;

static int check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        return 1;
    }
    return 0;
}

int main() {
    int dummy_accessor = 0;
    int dummy_info = 0;
    const auto& accessor = *reinterpret_cast<const r::SAnimationAccessor*>(&dummy_accessor);
    auto* applicator = reinterpret_cast<r::CApplicatorInfo*>(&dummy_info);
    int failures = 0;

    // Single-slot applyValue: returns the vtable +0x94 result.
    failures += check(r::applyValue_62390c(nullptr, nullptr, applicator) == 7, "applyValue_62390c vtable148");
    failures += check(r::applyValue_623924(nullptr, nullptr, applicator) == 7, "applyValue_623924 vtable148");

    // Forwarder: the V wrapper returns what the Ex body returns.
    int direct = r::applyKeyBasedValueEx_62388c(accessor, 3, 4, 0.5f, nullptr, applicator);
    int via_wrapper = r::applyKeyBasedValue_6238d0(accessor, 3, 4, 0.5f, nullptr, applicator);
    failures += check(direct == via_wrapper, "applyKeyBasedValue_6238d0 forwards to applyKeyBasedValueEx_62388c");

    // Key-based Ex body: local 12-byte buffer filled by the interpreter, then passed to the slot.
    failures += check(direct == 8, "applyKeyBasedValueEx_62388c returns vtable148 result");
    failures += check(r::g_last_value != nullptr, "applyKeyBasedValueEx_62388c passes its local buffer");

    // Second key-based forwarder pair with the 164 slot.
    int pos = r::applyKeyBasedValueEx_6239d8(accessor, 1, nullptr, applicator);
    failures += check(pos == 8, "applyKeyBasedValueEx_6239d8 returns vtable148 result");

    // Unreconstructed blend path returns a zero stub.
    failures += check(r::applyKeyBasedValueEx_623954(accessor, 0, nullptr, applicator) == 0,
                      "applyKeyBasedValueEx_623954 stub returns zero");

    if (failures == 0) std::printf("slice_15 test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
