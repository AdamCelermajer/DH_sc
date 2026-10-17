#pragma once
#include <cstdint>
#include <string>
namespace dh2::character_init_final_v1 {
struct Visual {std::uintptr_t identity=0;std::int32_t light_set_40=0;};
struct State {
 std::uintptr_t character=0;
 std::uint8_t initialized_1395=0;
 std::int32_t spawn_probability_274=0;
 Visual* visual_2d8=nullptr;
 std::uint8_t property_194_byte_3a8=0;
};
enum class Operation : std::uint32_t {spawn_probability,base_init_final,is_faerie,
 is_follower,local_player,look_at_vector,target_position,set_position,is_player,
 light_set,char_ai_init_final,is_local_player,update_all_skills,recalculate,
 property_int,save};
struct Request {
 Operation operation{};std::uintptr_t subject=0;
 std::uint32_t argument0=0,argument1=0;
 const char* name=nullptr;
 const float* vector=nullptr;
};
struct Reply {
 std::int32_t word=0;
 std::uintptr_t character_660=0;
 float vector[3]{};
 const float* target=nullptr;
};
struct Services {
 void* context=nullptr;
 int (*invoke)(void*,State&,const Request&,Reply&,std::string&)=nullptr;
};
enum class Status {complete,invalid_argument,busy,failed};
enum class Decision {not_started,already_initialized,probability_rejected,initialized};
struct Result {
 Decision decision=Decision::not_started;
 std::uint32_t entered_calls=0,last_operation=0,stores=0;
 std::uintptr_t captured_character=0,local_character=0;
};
class Runtime {
 State* state_;Services services_;bool busy_=false;
public:
 Runtime(State*,Services={});
 Status initialize(Result*,std::string&);
};
// Whole original Character::InitFinal588B@3b4978. Latch1395 precedes the
// spawn-probability query/gate. Base InitFinal, classification/locality,
// look/target-position, LightSetManager lookup, CharAI OnInitFinal, skill
// updates, full recalc, property194 and SG_Save are mandatory reached bodies.
// Position addition is binary32 in source order; queries receive captured this
// while mutable Character fields/visual pointers are reread at source sites.
// No owner, VM, timer, inventory, Save, quest log or additional frame is added.
// Providers must execute their source operations on the same existing graph.
// Successful SG_Save no-ops are invalid when a Save exists. Failure retains
// latch/writes/provider effects; a later explicit call observes the real latch
// and skips, without a fabricated failure flag or automatic retry.
// One thread; borrowed state/visual/provider/target storage survives dispatch.
// Same Runtime reentry or provider replacement/destruction is forbidden.
}
