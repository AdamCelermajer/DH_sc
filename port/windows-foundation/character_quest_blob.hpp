#pragma once
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace dh::foundation {
inline constexpr std::size_t character_quest_blob_limit = 128 * 1024;

// CQPG envelope (little endian), owned by this character:
//   "CQPG" | u32 version | u32 rows | u32 name_size | name bytes | 6 buckets
//   bucket = u8 origin (0 unknown) [ i32 current_quest | u32 count==rows | count*i32 state ]
// v1 ends after the six buckets. v2 (Preview 14 / schema v4) appends the
// objective counter section (original QEST quantity20/completed14 per objective):
//   u32 counter_count | counter_count * { u8 collection | u8 difficulty | u32 row |
//   u32 objective | i32 quantity | u8 completed }, strictly sorted and unique.
// The save layer validates the envelope without loading game tables. The quest
// owner additionally checks its row count against the actual QuestTables when
// decoding. Empty data means unknown, never fresh progress.
inline constexpr std::uint32_t character_quest_blob_max_version = 2;
inline constexpr std::uint32_t character_quest_counter_limit = 4096;
inline constexpr std::uint32_t character_quest_objective_limit = 64;

struct CharacterQuestBlobLayout {
    std::uint32_t version = 0;
    std::uint32_t rows = 0;
    std::size_t body_end = 0; // first byte after the six buckets (v2 counter section starts here)
};

// Structural scan of the envelope: header, character identity and six buckets.
// Does not read the v2 counter section. Empty input is not an envelope.
inline bool scan_character_quest_blob(const std::string& character_id,
    const std::vector<std::uint8_t>& bytes, CharacterQuestBlobLayout& layout, std::string& error) {
    const auto fail=[&](){error="Invalid source quest progress envelope";return false;};
    if(bytes.size()>character_quest_blob_limit||bytes.size()<16||
       bytes[0]!='C'||bytes[1]!='Q'||bytes[2]!='P'||bytes[3]!='G')return fail();
    std::size_t at=4;
    const auto word=[&](std::uint32_t& out){
        if(bytes.size()-at<4)return false;
        out=std::uint32_t(bytes[at])|std::uint32_t(bytes[at+1])<<8|
            std::uint32_t(bytes[at+2])<<16|std::uint32_t(bytes[at+3])<<24;
        at+=4;return true;
    };
    std::uint32_t version{},rows{},name_size{};
    if(!word(version)||version<1||version>character_quest_blob_max_version||!word(rows)||rows>4096||
       !word(name_size)||name_size>4096||name_size>bytes.size()-at||name_size!=character_id.size())return fail();
    for(std::size_t i=0;i<name_size;++i)if(bytes[at+i]!=static_cast<std::uint8_t>(character_id[i]))return fail();
    at+=name_size;
    for(unsigned bucket=0;bucket<6;++bucket){
        if(at==bytes.size())return fail();
        const auto origin=bytes[at++];if(origin>3)return fail();
        if(!origin)continue;
        std::uint32_t current{},count{};
        if(!word(current)||!word(count)||count!=rows||
           (current!=UINT32_MAX&&current>=rows)||std::size_t(count)>(bytes.size()-at)/4)return fail();
        at+=std::size_t(count)*4;
    }
    layout={version,rows,at};
    error.clear();return true;
}

// One persisted objective counter (Moths 0..8, Lizman 0..5, Witch 0/1, ...).
// The key is (collection, difficulty, row, objective); no quest names here.
struct QuestObjectiveCounterV2 {
    std::uint8_t collection = 0;   // source regular=0, volatile=1
    std::uint8_t difficulty = 0;   // 0..2
    std::uint32_t row = 0;         // quest table row
    std::uint32_t objective = 0;   // objective index inside the quest, < 64
    std::int32_t quantity = 0;     // >= 0
    bool completed = false;
    friend bool operator==(const QuestObjectiveCounterV2& a, const QuestObjectiveCounterV2& b) noexcept {
        return a.collection==b.collection&&a.difficulty==b.difficulty&&a.row==b.row&&
               a.objective==b.objective&&a.quantity==b.quantity&&a.completed==b.completed;
    }
};

namespace detail {
inline std::uint64_t counter_key(const QuestObjectiveCounterV2& c) noexcept {
    return (std::uint64_t(c.collection)<<56)|(std::uint64_t(c.difficulty)<<48)|
           (std::uint64_t(c.row)<<16)|std::uint64_t(c.objective);
}
inline bool counter_valid(const QuestObjectiveCounterV2& c, std::uint32_t rows) noexcept {
    return c.collection<=1&&c.difficulty<3&&c.row<rows&&
           c.objective<character_quest_objective_limit&&c.quantity>=0;
}
}

// Reads the v2 counter section of a scanned envelope; v1 yields no counters.
inline bool decode_quest_counter_section(const std::vector<std::uint8_t>& bytes,
    const CharacterQuestBlobLayout& layout, std::vector<QuestObjectiveCounterV2>& out, std::string& error) {
    const auto fail=[&](const char* why){error=why;return false;};
    out.clear();
    if(layout.version==1){
        if(layout.body_end!=bytes.size())return fail("Invalid source quest progress envelope");
        error.clear();return true;
    }
    std::size_t at=layout.body_end;
    if(bytes.size()-at<4)return fail("Truncated source quest counter section");
    const std::uint32_t count=std::uint32_t(bytes[at])|std::uint32_t(bytes[at+1])<<8|
        std::uint32_t(bytes[at+2])<<16|std::uint32_t(bytes[at+3])<<24;
    at+=4;
    constexpr std::size_t entry_size=1+1+4+4+4+1;
    if(count>character_quest_counter_limit||std::size_t(count)>(bytes.size()-at)/entry_size||
       std::size_t(count)*entry_size!=bytes.size()-at)
        return fail("Invalid source quest counter section length");
    std::vector<QuestObjectiveCounterV2> staged;staged.reserve(count);
    const auto u32=[&](){
        const auto v=std::uint32_t(bytes[at])|std::uint32_t(bytes[at+1])<<8|
            std::uint32_t(bytes[at+2])<<16|std::uint32_t(bytes[at+3])<<24;at+=4;return v;};
    for(std::uint32_t i=0;i<count;++i){
        QuestObjectiveCounterV2 c;
        c.collection=bytes[at++];c.difficulty=bytes[at++];c.row=u32();c.objective=u32();
        c.quantity=static_cast<std::int32_t>(u32());
        const auto completed=bytes[at++];
        if(completed>1)return fail("Invalid source quest counter completed flag");
        c.completed=completed!=0;
        if(!detail::counter_valid(c,layout.rows))return fail("Source quest counter is outside its quest/objective bounds");
        if(!staged.empty()&&detail::counter_key(staged.back())>=detail::counter_key(c))
            return fail("Source quest counters must be unique and sorted");
        staged.push_back(c);
    }
    out=std::move(staged);error.clear();return true;
}

// Reads the counters of a blob owned by character_id. Empty or v1 blobs have none.
inline bool read_quest_counters(const std::string& character_id,const std::vector<std::uint8_t>& blob,
    std::vector<QuestObjectiveCounterV2>& out, std::string& error) {
    out.clear();
    if(blob.empty()){error.clear();return true;}
    CharacterQuestBlobLayout layout;
    if(!scan_character_quest_blob(character_id,blob,layout,error))return false;
    return decode_quest_counter_section(blob,layout,out,error);
}

// Replaces the counter section of an initialized envelope (any version). No
// counters writes the canonical v1 form, so v1 producers stay byte-identical.
// Counters must be strictly sorted by (collection,difficulty,row,objective).
// Requires a nonempty (initialized) envelope: counters never fabricate progress.
inline bool write_quest_counters(const std::string& character_id,std::vector<std::uint8_t>& blob,
    const std::vector<QuestObjectiveCounterV2>& counters,std::string& error) {
    CharacterQuestBlobLayout layout;
    if(blob.empty()){error="Quest counters require an initialized CQPG envelope";return false;}
    if(!scan_character_quest_blob(character_id,blob,layout,error))return false;
    if(counters.size()>character_quest_counter_limit){error="Too many source quest counters";return false;}
    for(std::size_t i=0;i<counters.size();++i){
        if(!detail::counter_valid(counters[i],layout.rows)){error="Source quest counter is outside its quest/objective bounds";return false;}
        if(i>0&&detail::counter_key(counters[i-1])>=detail::counter_key(counters[i])){error="Source quest counters must be unique and sorted";return false;}
    }
    std::vector<std::uint8_t> next(blob.begin(),blob.begin()+std::ptrdiff_t(layout.body_end));
    const auto put=[&](std::uint32_t v){for(unsigned i=0;i<4;++i)next.push_back(std::uint8_t(v>>(8*i)));};
    next[4]=counters.empty()?1:2;next[5]=next[6]=next[7]=0;
    if(!counters.empty()){
        put(std::uint32_t(counters.size()));
        for(const auto& c:counters){
            next.push_back(c.collection);next.push_back(c.difficulty);put(c.row);put(c.objective);
            put(static_cast<std::uint32_t>(c.quantity));next.push_back(c.completed?1:0);
        }
    }
    if(next.size()>character_quest_blob_limit){error="Source quest progress exceeds limit";return false;}
    blob=std::move(next);error.clear();return true;
}

inline bool validate_character_quest_blob(const std::string& character_id,
    const std::vector<std::uint8_t>& bytes, std::string& error) {
    if(bytes.empty()){error.clear();return true;}
    CharacterQuestBlobLayout layout;
    if(!scan_character_quest_blob(character_id,bytes,layout,error))return false;
    std::vector<QuestObjectiveCounterV2> counters;
    return decode_quest_counter_section(bytes,layout,counters,error);
}
}
