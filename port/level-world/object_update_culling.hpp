#pragma once

#include <cstdint>

namespace dh2::object_update_culling {

// Logical live projections, not ARM/renderer overlays. The original caller
// uses ObjectBase+0x86; the remote leaf reads +0x110 and, only for -1, +0x118.
struct Object {
    std::uintptr_t identity;
    std::uint32_t remote_word_110;
    std::uint8_t culling_phase_86;
    std::uint8_t remote_byte_118;
    std::uint8_t reserved[2];
};
struct Aabb { std::uint32_t minimum[3], maximum[3]; };
struct Plane { std::uint32_t normal[3], distance; };
// Source getViewFrustum returns borrowed storage. Plane i starts at source
// return+0x0c+i*0x10 (normal xyz followed by D); all six remain live.
struct Frustum { std::uintptr_t identity; Plane planes[6]; };
struct CameraRoot { std::uintptr_t identity; };
struct CameraVisual { std::uintptr_t identity; CameraRoot* root_8; };
struct Level { std::uintptr_t identity; CameraVisual* camera_128; };
struct Globals { std::uintptr_t application; };

enum class Operation : std::uint32_t {
    get_online_byte,
    is_remotely_updated,
    get_current_level,
    get_view_frustum,
};
struct Request { Operation operation; std::uintptr_t object, subject; };
struct Response { std::uint32_t raw; Level* level; const Frustum* frustum; };
struct Services {
    void* context;
    // Synchronous mandatory reached provider; zero means success. GetOnline
    // returns the exact byte domain; the remote virtual return is ignored by
    // this caller. GetCurrentLevel may return null. Camera/root/frustum cannot
    // be invented or null on a taken nonnull-Level path. Exceptions are errors.
    std::int32_t (*invoke)(void*, Object*, const Request*, Response*);
};
enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    invalid_source_fact = 2,
    service_unavailable = 3,
    service_failed = 4,
    reentrant_call = 5,
};
struct Result {
    std::uint32_t can_update, service_calls, phase_writes, planes_tested;
    std::uint32_t last_distance_bits;
    std::uint32_t captured_aabb[6];
    std::uintptr_t captured_frustum;
};
struct RemoteResult { std::uint32_t raw; };

// Complete original TestCullingBeforeUpdate caller, with GetOnline, remote
// virtual, GetCurrentLevel and camera virtual+0x144 as explicit providers.
// There is no Visual/root read on the direct Object path; the camera chain is
// Level+0x128 -> CameraVisual+8 -> CameraRoot -> getViewFrustum.
// AABB is read only for phase==1 and is captured before GetCurrentLevel.
// Globals is required only on that path. Null/malformed reached camera facts
// fail explicitly; prior source effects are retained without rollback.
// One owning thread. Borrowed storage and identities remain alive/stable in
// address through return; fields/pointers may change in providers. Distinct
// logical projections and control/output objects must not overlap. The service
// table is copied. Recursive evaluation of the same Object is rejected; an
// independent Object/output may be evaluated synchronously.
Status evaluate(Object*, const Aabb*, const Globals*, const Services*, Result*);

// Complete original20B ObjectBase::IsRemotelyUpdated. Returns raw +0x118 only
// when +0x110 is exactly 0xffffffff, otherwise returns 1. No normalization.
Status is_remotely_updated(const Object*, RemoteResult*);

}  // namespace dh2::object_update_culling
