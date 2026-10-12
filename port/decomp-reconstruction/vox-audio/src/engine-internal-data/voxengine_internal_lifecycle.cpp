// VoxEngineInternal lifetime: the constructor, the destructor and Initialize.
// The data-object release helpers are in voxengine_internal_data_release.cpp.
#include "vox-audio/deps/vox_file_system.hpp"
#include "vox-audio/vox/driver.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/native_sub_decoder.hpp"
#include "vox-audio/vox/priority_bank.hpp"
#include "vox-audio/vox/stream.hpp"
#include "vox-audio/vox/voxengine_internal.hpp"

namespace vox {

// 0x869a94 _ZN3vox17VoxEngineInternalC1Ev, and 0x869d8c C2 (same body).
// Partial. The port's member initializers in voxengine_internal.hpp give the zeroed fields.
// This body sets what those do not:
//   - master fade (+0xe0) and the 32 group fades (+0xf4, 20-byte stride) to current 0, target 1,
//     elapsed 0, duration 0, flag clear (the second loop in the original clears the flags it
//     set in the first);
//   - the priority bank manager (+0xdc, bank count 8), created with new, as the original does
//     with VoxAlloc(0x14) and the 8-argument constructor;
//   - the default 3D parameters (empty body in the binary).
// Not reproduced: the four HandlableContainer id counters start at 1 in the original (the
// port's counters start at 0, and the container has no setter), the reserve() calls for the
// emitter and data vectors (the port has no such vectors), the list head at +0x43c and the
// +0x598 pointer (their owners are not identified), and the two-word stamp table, which the
// member initializer already zeroes.
VoxEngineInternal::VoxEngineInternal() {
    master_fade_ = VoxGainFade{0.0f, 1.0f, 0.0f, 0.0f, false};  // +0xe0..+0xf0
    for (VoxGainFade& fade : group_fades_) {
        fade = VoxGainFade{0.0f, 1.0f, 0.0f, 0.0f, false};       // +0xf4 + 20*i
    }
    priority_bank_ = new PriorityBankManager(8);                  // +0xdc
    _SetDefault3DParameters();
}

// 0x86be68 _ZN3vox17VoxEngineInternalD1Ev, and 0x86c02c D2 (same body).
// Partial. The original also calls VoxNativeSubDecoder::Clean (the call's argument is a
// decompiler artifact; Clean is static here) and FileSystemInterface::DestroyInstance, which the
// port does not have. The buffer at +0x598 is freed in the original; its owner is not identified.
// The members (containers, mutexes, the data queue) are destroyed after this body, as in the
// original.
VoxEngineInternal::~VoxEngineInternal() {
    _ReleaseAllDatasource();
    VoxNativeSubDecoder::Clean();
    delete priority_bank_;  // +0xdc, created with new in the constructor
    priority_bank_ = nullptr;
    if (driver_ != nullptr) {  // +0x54c, created by CreateDriver in the original
        Destruct(driver_);
        VoxFree(driver_);
        driver_ = nullptr;
    }
    s_voxEngineInternal = nullptr;
}

// 0x86934c _ZN3vox17VoxEngineInternal10InitializeEv
// Partial. The original creates the driver with CreateDriver when +0x54c is empty, and then
// returns FileSystemInterface::GetInstance(). CreateDriver is not ported, so the driver stays null.
// GetInstance has no side effects, so the port omits that call (the slot is void).
void VoxEngineInternal::Initialize() {
    // CreateDriver(this) (not ported): driver_ stays null until it is.
}

}  // namespace vox
