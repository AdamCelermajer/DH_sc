// VoxEngineInternal lifetime: the constructor, the destructor and Initialize.
// The data-object release helpers are in voxengine_internal_data_release.cpp.
#include "vox-audio/deps/vox_file_system.hpp"
#include "vox-audio/vox/driver.hpp"
#include "vox-audio/vox/driver_android.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/native_sub_decoder.hpp"
#include "vox-audio/vox/priority_bank.hpp"
#include "vox-audio/vox/stream.hpp"
#include "vox-audio/vox/voxengine_internal.hpp"

namespace vox {

// vox::CreateDriver (0x88fac4), defined in files-xml-zip-stream/create_driver.cpp.
DriverAndroid* CreateDriver();

// 0x869a94 _ZN3vox17VoxEngineInternalC1Ev, and 0x869d8c C2 (same body).
// Partial. The port's member initializers in voxengine_internal.hpp give the zeroed fields.
// This body sets what those do not:
//   - master fade (+0xe0) and the 32 group fades (+0xf4, 20-byte stride) to current 0, target 1,
//     elapsed 0, duration 0, flag clear (the second loop in the original clears the flags it
//     set in the first);
//   - the priority bank manager (+0xdc, bank count 8), created with new, as the original does
//     with VoxAlloc(0x14) and the 8-argument constructor;
//   - the default 3D parameters (empty body in the binary).
// The four handle containers (+0x08 data objects, +0x28 pending data, +0x78 emitters, +0x98
// pending emitters) start their id counters at 1: the next_id word at container +0x18 is
// set to 1 in the binary (0x869a94 writes 1 at +0x20, +0x40, +0x90 and +0xb0).
// Not reproduced: the intrusive list sentinel at +0x43c/+0x440 (no member in the port),
// the word at +0x598 and the word at +0x59c (no owner identified), and the reserve() calls
// for the emitter and data pointer vectors (capacity only).
VoxEngineInternal::VoxEngineInternal() {
    master_fade_ = VoxGainFade{0.0f, 1.0f, 0.0f, 0.0f, false};  // +0xe0..+0xf0
    for (VoxGainFade& fade : group_fades_) {
        fade = VoxGainFade{0.0f, 1.0f, 0.0f, 0.0f, false};       // +0xf4 + 20*i
    }
    data_objects_.SetFirstHandleId(1);    // +0x08 container, next id at +0x20
    data_pending_.SetFirstHandleId(1);    // +0x28 container, next id at +0x40
    emitters_.SetFirstHandleId(1);        // +0x78 container, next id at +0x90
    emitter_pending_.SetFirstHandleId(1); // +0x98 container, next id at +0xb0
    priority_bank_ = new PriorityBankManager(8);                  // +0xdc
    _SetDefault3DParameters();
}

// 0x86be68 _ZN3vox17VoxEngineInternalD1Ev, and 0x86c02c D2 (same body).
// Partial. The original also frees the nodes of the intrusive list at +0x43c and the buffer at
// +0x598 (both owners are not identified, so the port frees neither). The members (containers,
// mutexes, the data queue) are destroyed after this body, as in the original.
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
    FileSystemInterface::DestroyInstance();
    s_voxEngineInternal = nullptr;
}

// 0x86934c _ZN3vox17VoxEngineInternal10InitializeEv
// Creates the driver with CreateDriver when +0x54c is empty, then returns
// FileSystemInterface::GetInstance() (asm: a tail call after the null test).
FileSystemInterface* VoxEngineInternal::Initialize() {
    if (driver_ == nullptr) {
        driver_ = CreateDriver();
    }
    return FileSystemInterface::GetInstance();
}

}  // namespace vox
