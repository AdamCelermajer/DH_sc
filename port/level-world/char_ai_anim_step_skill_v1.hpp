#pragma once

#include <cstdint>

namespace dh2::char_ai_anim_step_skill_v1 {
enum class Status : std::uint8_t { complete, invalid_argument, invalid_source_fact };
struct State {
    std::uint32_t skill_state;
    std::uint32_t animation_depth;
    std::uint32_t step_index;
    std::uint32_t step_count;
    std::uint8_t* step_started_208;
    const std::uint8_t* end_pending_209;
};
struct Result { std::uint8_t matched=0, stop_loop=0; };

// CharAI::_OnAnimStepBegin_SkillSpell (IDA 0x3d3dd4): in states 6/7,
// ANIM_GetStepIndex/Count are read. Step zero is ignored unless CharAnimator
// depth is exactly 1; every other valid step sets AI+0x208. If AI+0x209 is
// already set, source calls ANIM_StopLoop(true). The renderer supplies the
// retained sequence depth and source AI bytes; no original object layout is
// projected here.
inline Status apply(const State* state,Result* output) noexcept {
    if(!state||!output||!state->step_started_208||!state->end_pending_209)
        return Status::invalid_argument;
    *output={};
    if(state->skill_state!=6&&state->skill_state!=7)return Status::complete;
    if(state->animation_depth>=3||state->step_count==0||
       state->step_index>=state->step_count)
        return Status::invalid_source_fact;
    if(state->step_index==0&&state->animation_depth!=1)return Status::complete;
    *state->step_started_208=1;
    output->matched=1;
    output->stop_loop=*state->end_pending_209!=0;
    return Status::complete;
}
} // namespace dh2::char_ai_anim_step_skill_v1
