#pragma once
#include "player_savegame_v1.hpp"
#include "quest_savegame_v1.hpp"
#include <string>
namespace dh2::data::player_saved_quests_v1 {
struct StreamRef {std::uintptr_t identity=0;};
enum class Operation : std::uint32_t {none,tell,seek,read_unsigned,read_signed,quest_data,assert_mode,log_assert,complete};
struct Request {
 Operation operation=Operation::none;StreamRef* stream=nullptr;
 quest_savegame_v1::QuestSavegame* log=nullptr;
 quest_savegame_v1::QuestRef* quest=nullptr;
 // Source word readers write directly to this live destination. The provider
 // must not retain the pointer after its synchronous call returns.
 void* destination=nullptr;
 std::uint64_t offset=0;
 std::int32_t difficulty=0,ordinal=0,index=0;
 std::uint32_t source_caller=0,virtual_slot=0;
 std::uint8_t flag=0;
};
struct Reply {std::uint64_t position=0;std::int32_t word=0;};
struct Services {
 void* context=nullptr;
 // Zero delivers the actual reached source operation. Failure/throw retains
 // provider-written words, Quest effects and its single stream cursor prefix.
 std::int32_t (*invoke)(void*,const Request&,Reply&,std::string&)=nullptr;
};
struct Bindings {
 PlayerSavegameV1* save=nullptr;
 quest_savegame_v1::QuestSavegame* log_b8=nullptr;
 quest_savegame_v1::QuestSavegame* log_118=nullptr;
 StreamRef* stream=nullptr;Services services{};
};
enum class Status {complete,invalid_argument,busy,failed};
struct Result {
 Operation operation=Operation::none;
 std::uint32_t source_caller=0,service_calls=0,read_words=0,quest_calls=0,
 matched_groups=0,mismatched_groups=0,tail_read_returns=0,copied_word50=0,
 null_quests=0,assert_mode_reads=0,assert_logs=0,tell_calls=0,seek_calls=0,
 log=0,difficulty=0,declared_count=0,captured_count=0;
 std::int32_t ordinal=0,index=0;
 std::uint64_t told_position=0,seek_position=0;
};
class Runtime {
 Bindings bindings_;bool busy_=false;
 Status execute(std::uint32_t method,std::uint32_t log,std::int32_t ordinal,
                std::int32_t difficulty,std::uint32_t flag,Result*,std::string&);
public:
 explicit Runtime(Bindings);
 Status load(Result*,std::string&); // PlayerSavegame::__LoadQuests
 Status load_quests(std::uint32_t log,Result*,std::string&);
 Status unpack_quests(std::uint32_t log,std::int32_t difficulty,
                      std::uint32_t flag,Result*,std::string&);
 Status unpack_quest(std::uint32_t log,std::int32_t ordinal,
                     std::int32_t difficulty,std::uint32_t flag,Result*,std::string&);
};
// Whole QEST80B, LoadQuests52B, UnpackQuests196B and UnpackQuest328B callers.
// Both log controls must be this Save's actual source_quest_log_b8/118 owners.
// They borrow the canonical +44/LNAM act arrays; their
// published vectors/QuestRefs remain the existing QuestSavegame owner's stores.
// QEST captures only low32 of tell() before an absolute uint64 seek, then reads
// the second log from that same position. Each group captures vector length
// before reading count; mismatch leaves all remaining group bytes unread.
// A packet's streamed index replaces UnpackQuest's ordinal argument. Null Quest
// slots retain the source assertion behavior and do not consume Quest data.
// Complete source Quest::_loadQuestData112B and stream/assert bodies remain
// mandatory real providers; this adapter owns no cursor, Quest, vector, Save,
// profile, property, inventory, VM or timer. Failed delivery preserves prefix
// effects. Bounds, output aliases, exceptions and reentry are native policies.
// Controls/backing live on one owning thread. Callbacks may mutate fields/live
// vectors as the source allows, but must not destroy/rebind these owners/runtime.
}
