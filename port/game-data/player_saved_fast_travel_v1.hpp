#pragma once
#include "player_savegame_v1.hpp"
#include <string>
namespace dh2::data::player_saved_fast_travel_v1 {
enum class Stage : std::uint32_t {not_started,text,decode,publish,complete};
enum class Decision : std::uint32_t {not_started,loaded,oversized_stopped};
struct Result {
 Stage stage=Stage::not_started;Decision decision=Decision::not_started;
 std::uint32_t source_caller=0,consumed=0,read_calls=0,string_reads=0,
 difficulty=0,characters=0,word_stores=0,completed_difficulties=0;
};
enum class Status {complete,invalid_argument,busy,failed};
class Runtime {
 PlayerSavegameV1* save_;bool busy_=false;
public:
 explicit Runtime(PlayerSavegameV1*);
 Status load(Bytes,Result*,std::string&);
};
// Whole __LoadFastTravelList452B@469b18. Borrows the sole Save's six +17c..193
// bitset words, initialized to zero by both original C1 bodies. Each valid
// difficulty publishes its low word followed by its high word. Strings longer
// than64 end the whole reader without changing the current/later bitsets.
// Invalid binary characters reach the source throw boundary before publishing
// that difficulty. Delivered earlier stores remain. Bounded spans, allocation
// failure, alias guards and reentry are native policies. No new fast-travel
// table, Save, profile, inventory, property, VM or timer owner is constructed.
}
