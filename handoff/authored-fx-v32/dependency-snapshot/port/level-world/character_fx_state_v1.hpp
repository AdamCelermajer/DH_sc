#pragma once
#include "character_fx_kernels_v1.hpp"
namespace dh2::fx {
struct FxState96V1 {
 std::uintptr_t callback{},set_identity{},anchor{},visual{};
 std::int32_t file{-1},loop{},timer{-1};float speed{1};
 std::uint32_t visible{1},looping{},orient_once{},orient_with_anchor{},scale_with_anchor{},fixed_rotation{};
 float position[3]{},rotation[3]{};
};
enum class FxOperationV1:std::uint32_t {sync=1,speed,loop,start,rotation,visible,end_callback,end_sample,
 anchor_dead,anchor_disabled,app_dt,anchor_stationary,debug_load,debug_set,debug_instance,completed};
struct FxRequest32V1 {FxOperationV1 operation;std::uint32_t argument{};std::uintptr_t identity{};float scalar{};std::uint32_t reserved{};std::uintptr_t payload{};};
using FxInvokeV1=int(*)(void*,FxState96V1*,FxRequest32V1*);
struct FxServices16V1 {void* context{};FxInvokeV1 invoke{};};
static_assert(sizeof(FxState96V1)==96&&sizeof(FxRequest32V1)==32&&sizeof(FxServices16V1)==16);
}
extern "C" {
// Services may synchronously mutate the live state. Failure preserves the
// delivered source prefix; malformed native contracts reject before mutation.
int dh2_fx_set_anim_v1(dh2::fx::FxState96V1*,const dh2::fx::FxData32V1*,std::uintptr_t callback,const dh2::fx::FxServices16V1*);
int dh2_fx_play_v1(dh2::fx::FxState96V1*,const float* position,const float* nullable_rotation,std::uintptr_t nullable_anchor,const dh2::fx::FxData32V1* nullable_data,std::uintptr_t callback,const dh2::fx::FxServices16V1*);
// DropAnimatedFX's cleanup AFTER active removal/free-vector insertion.
int dh2_fx_drop_reset_v1(dh2::fx::FxState96V1*,const dh2::fx::FxServices16V1*);
int dh2_fx_handle_loop_end_v1(dh2::fx::FxState96V1*,const dh2::fx::FxServices16V1*);
int dh2_fx_update_v1(dh2::fx::FxState96V1*,const dh2::fx::FxServices16V1*);
}
