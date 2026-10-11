#pragma once
#include <array>
namespace dh::foundation {
// B068 cutscene framing. A scripted cutscene clip (PlayActorAnim / CSAnim state 14) is authored together with its camera clip in the
// shared cutscene frame at unit scale: reference P1 945-955 s (Intro_Witch) shows the Prince left and Rene right only when both
// are drawn at unit scale; the 0.9 gameplay scale (base props 12/13 * 0.009) pulled the actor root offsets toward the waypoint
// and put Rene at the screen centre. Ordinary poses keep the actor scale. Native source for the scale: Character+288 (InitPost
// 0x3b4d60), synced by VisualObject::SyncScaling 0x472860; the native cutscene exemption is NOT recovered (reference evidence only).
inline std::array<float,3> source_actor_draw_scale(bool scripted_clip_pose,const std::array<float,3>& actor_scale){
    return scripted_clip_pose?std::array<float,3>{1.f,1.f,1.f}:actor_scale;
}
}
