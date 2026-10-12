#pragma once
#include "data.hpp"
#include <utility>
namespace dh2::data {
struct AnimationStep {
 bool anchor_fx=false,cam_dir=false,move_go=false,swoosh=false;
 std::int32_t anim=-1,blend_out=0,cam=-1,fx=-1,redir=0,sound=-1;
 float speed=1;
 std::vector<std::int32_t> random_cam;
};
struct AnimationSequence {
 std::int32_t loop=0,type=0;
 std::vector<AnimationStep> steps;
};
struct CameraAnimationSet {
 std::vector<std::int32_t> cam_anims;
 std::int32_t crit=-1,idle=-1,shake=-1,template_id=-1;
};
struct CharacterAnimations {
 // Interact and Spells are arrays; all other schema fields are scalars.
 std::array<std::vector<std::int32_t>,37> fields;
};
struct AnimationTables {
 std::vector<std::string> sequence_names,camera_names,character_names,state_names;
 std::vector<AnimationSequence> sequences;
 std::vector<CameraAnimationSet> cameras;
 std::vector<CharacterAnimations> characters;
 std::size_t sequence_end=0,camera_end=0,data_consumed=0;
};
// Entire original AnimTable, CamAnimSetTable and CharAnimTable stream.
// References with Redir=1 address AnimTable; other Anim references address
// AnimDict. Type/Loop are retained as original values, not executed here.
bool load_animation_tables(Bytes records,Bytes names,Bytes fields,const Dictionary& clips,AnimationTables&,std::string&);
const AnimationSequence* animation_state(const AnimationTables&,std::int32_t character,const std::string& state,std::size_t variant=0);
const std::string* animation_clip(const AnimationStep&,const Dictionary& clips);
struct AnimationRandom {std::uint32_t seed=1,calls=0;};
struct AnimationStart {
 // Original CharAnimator permits three nested sequence layers (0..2).
 std::vector<std::pair<std::int32_t,std::uint32_t>> layers;
 AnimationStep step;
};
// Reconstructs initial selection only. Completion callbacks, sequence advance,
// blending, FX, audio and movement events remain outside this function.
bool choose_animation_start(const AnimationTables&,std::int32_t sequence,AnimationRandom&,AnimationStart&,std::string&,bool random_enabled=true);
}
extern "C" std::uint32_t dh2_animation_random(std::uint32_t* seed,std::uint32_t* calls,std::uint32_t count);
