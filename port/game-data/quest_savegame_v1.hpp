#pragma once
#include <array>
#include <cstdint>
#include <vector>

namespace dh2::data::quest_savegame_v1 {
// These are borrows of fields in the actual Quest owner, not a Quest body.
struct QuestFields {
    std::int32_t row_8=-1;
    std::uintptr_t definition_name_14=0,character_60=0;
};
struct QuestRef {std::uintptr_t identity=0;QuestFields* fields=nullptr;};

// Sole source QuestSavegame vector/field owner. The factory backs stable Quest
// objects; these vectors hold its published ownership leases. No alternate
// quest list, VM, inventory, property sheet or timer store is introduced.
struct QuestSavegame {
    std::array<std::vector<QuestRef*>,3> quests;
    std::array<std::uint8_t,3> byte_28{};
    std::array<std::int32_t,3> word_2c{{-1,-1,-1}},word_38{{-1,-1,-1}};
    // Existing Save LNAM projection already owns these embedded +44 words.
    // Require that canonical backing; never allocate a second act matrix.
    std::array<std::int32_t,3>& word_44;
    std::array<std::int32_t,3> word_50{{1,1,1}};
    std::uintptr_t character_5c=0;
    explicit QuestSavegame(std::array<std::int32_t,3>& canonical_act_words):word_44(canonical_act_words){}
    QuestSavegame(const QuestSavegame&)=delete;
    QuestSavegame& operator=(const QuestSavegame&)=delete;
};
enum class Status : std::uint32_t {
    complete,invalid_argument,service_unavailable,service_failed,
    missing_projection,source_fault,unsafe_storage,reentrant
};
enum class Operation : std::uint32_t {
    none,construct,table_count,resize,table_rows,allocate,quest_construct,
    bind_character,owner_children,definition_name,bind_row,assign_pydata,
    reinit,publish,quest_destruct,deallocate,clear_vectors
};
struct Result {
    Status status=Status::complete;Operation last_operation=Operation::none;
    std::uint32_t difficulty=0,row=0,service_calls=0,published=0,reinitialized=0,destroyed=0;
    std::uintptr_t allocation=0;
};
struct Services {
    void* context=nullptr;
    std::int32_t (*table_count)(void*,std::uint32_t*)=nullptr;
    // Fresh actual table base, captured before allocation for each new Quest.
    std::int32_t (*table_rows)(void*,std::uintptr_t*)=nullptr;
    std::int32_t (*allocate)(void*,std::uint32_t logical_bytes,std::uint32_t tag,std::uintptr_t*)=nullptr;
    std::int32_t (*quest_construct)(void*,std::uintptr_t,std::int32_t difficulty,QuestRef**)=nullptr;
    std::int32_t (*owner_children)(void*,QuestRef*)=nullptr;
    std::int32_t (*definition_name)(void*,std::uint32_t row,std::uintptr_t*)=nullptr;
    std::int32_t (*assign_pydata)(void*,QuestRef*,std::uintptr_t actual_row)=nullptr;
    std::int32_t (*reinit)(void*,QuestRef*)=nullptr;
    std::int32_t (*quest_destruct)(void*,QuestRef*)=nullptr;
    std::int32_t (*deallocate)(void*,std::uintptr_t)=nullptr;
};
class Runtime {
    QuestSavegame& save_;Services services_;bool busy_=false;
public:
    Runtime(QuestSavegame& save,Services services):save_(save),services_(services){}
    Runtime(const Runtime&)=delete;Runtime& operator=(const Runtime&)=delete;
    // Source C1/C2 have identical fields. Requires fresh empty vectors.
    Status construct(Result*);
    // Whole 412-byte InitQuests caller. Quest C1/children/AssignPyData/ReInit
    // remain mandatory real providers; this module does not claim their bodies.
    // Existing vectors use fresh table count, including its unsafe null/short
    // source accesses. Failed providers retain all reached stores/publications.
    Status init_quests(Result*);
    // Whole source D1 ownership order: destroy/deallocate each published Quest,
    // clear its slot, then release vector storage in reverse difficulty order.
    // Caller must finish this explicit closure before destroying factory/context.
    Status destroy(Result*);
};
} // namespace dh2::data::quest_savegame_v1
