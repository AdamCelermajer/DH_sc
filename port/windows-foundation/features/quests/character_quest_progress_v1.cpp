#include "character_quest_progress_v1.hpp"
#include "../../character_quest_blob.hpp"
#include <algorithm>
#include <cstring>

namespace dh::foundation {
namespace {
bool fail(std::string& error, const char* message) { error = message; return false; }

void put_u32(std::vector<std::uint8_t>& out, std::uint32_t value) {
    for (unsigned i=0;i<4;++i) out.push_back(static_cast<std::uint8_t>(value>>(i*8)));
}
void put_i32(std::vector<std::uint8_t>& out, std::int32_t value) {
    std::uint32_t raw{}; std::memcpy(&raw,&value,sizeof raw); put_u32(out,raw);
}
struct Reader {
    const std::vector<std::uint8_t>& bytes;
    std::size_t at{};
    bool u8(std::uint8_t& out) {
        if (at>=bytes.size()) return false;
        out=bytes[at++]; return true;
    }
    bool u32(std::uint32_t& out) {
        if (bytes.size()-at<4) return false;
        out=std::uint32_t(bytes[at])|std::uint32_t(bytes[at+1])<<8|
            std::uint32_t(bytes[at+2])<<16|std::uint32_t(bytes[at+3])<<24;
        at+=4; return true;
    }
    bool i32(std::int32_t& out) {
        std::uint32_t raw{}; if(!u32(raw))return false;
        std::memcpy(&out,&raw,sizeof out); return true;
    }
    bool raw_string(std::uint32_t size,std::string& out) {
        if(size>bytes.size()-at)return false;
        out.assign(reinterpret_cast<const char*>(bytes.data()+at),size);at+=size;return true;
    }
};
bool valid_policy_category(const dh2::data::QuestDefinitionV51& definition,
    std::int32_t state, CharacterQuestCategoryV1 category,
    const CharacterQuestLogPolicyV1& policy) {
    const bool debug=definition.priority==policy.debug_priority;
    if(policy.display_all_quests)return !debug;
    if(policy.display_all_debug_only_quests)return debug;
    if(debug)return false;
    if(category==CharacterQuestCategoryV1::assigned)return state>=6&&state<=12;
    return state>12;
}
constexpr std::int32_t source_string_sentinel=1835016;
} // namespace

bool CharacterQuestProgressV1::belongs_to(const CharacterState& character) const noexcept {
    return !character_id_.empty() && character.id==character_id_;
}
bool CharacterQuestProgressV1::bind_character(const CharacterState& character,std::string& error) {
    if(character.id.empty())return fail(error,"Quest progress requires a named CharacterState owner");
    if(!character_id_.empty()&&character_id_!=character.id)
        return fail(error,"Quest progress is already bound to another CharacterState");
    for(const auto& collection:buckets_)for(const auto& bucket:collection)
        if(bucket.origin!=Origin::unknown)return fail(error,"Only unknown Quest progress may bind an owner");
    character_id_=character.id;error.clear();return true;
}
bool CharacterQuestProgressV1::valid_id(const CharacterQuestIdV1& id) const noexcept {
    if(id.collection>1||id.difficulty<0||id.difficulty>=3||id.row<0)return false;
    const auto& bucket=buckets_[id.collection][std::size_t(id.difficulty)];
    return bucket.origin!=Origin::unknown&&std::size_t(id.row)<bucket.states.size();
}

bool CharacterQuestProgressV1::initialize_fresh(const CharacterState& character,
    const dh2::data::QuestTablesPersistenceV51& tables,std::string& error) {
    if(character.id.empty()||!tables.ready()||tables.rows().size()>maximum_quest_rows)
        return fail(error,"Fresh generic Quest initialization requires a named CharacterState and bounded original Quest table");
    for(const auto& collection:buckets_)for(const auto& bucket:collection)
        if(bucket.origin!=Origin::unknown)return fail(error,"Generic Quest progress is already initialized");
    std::array<std::array<Bucket,3>,2> staged{};
    for(auto& collection:staged)for(auto& bucket:collection){
        bucket.origin=Origin::fresh_source_initialized;
        bucket.current_quest=-1; bucket.states.reserve(tables.rows().size());
        for(const auto& definition:tables.rows())bucket.states.push_back(definition.state);
    }
    character_id_=character.id;
    buckets_=std::move(staged);
    error.clear(); return true;
}

bool CharacterQuestProgressV1::encode(
    const dh2::data::QuestTablesPersistenceV51& tables,
    std::vector<std::uint8_t>& output,std::string& error) const {
    if(character_id_.empty()||character_id_.size()>4096||!tables.ready()||tables.rows().size()>maximum_quest_rows)
        return fail(error,"Quest progress codec requires a bound character and original table");
    std::vector<std::uint8_t> staged{'C','Q','P','G'};
    put_u32(staged,codec_version);
    put_u32(staged,static_cast<std::uint32_t>(tables.rows().size()));
    put_u32(staged,static_cast<std::uint32_t>(character_id_.size()));
    staged.insert(staged.end(),character_id_.begin(),character_id_.end());
    for(const auto& collection:buckets_)for(const auto& bucket:collection){
        staged.push_back(static_cast<std::uint8_t>(bucket.origin));
        if(bucket.origin==Origin::unknown){
            if(!bucket.states.empty()||bucket.current_quest!=-1)
                return fail(error,"Unknown Quest bucket contains fabricated progress");
            continue;
        }
        if(bucket.origin!=Origin::fresh_source_initialized&&bucket.origin!=Origin::modified&&
           bucket.origin!=Origin::loaded)
            return fail(error,"Quest progress bucket has an invalid origin");
        if(bucket.states.size()!=tables.rows().size())
            return fail(error,"Initialized Quest progress row count differs from original table");
        put_i32(staged,bucket.current_quest);
        put_u32(staged,static_cast<std::uint32_t>(bucket.states.size()));
        for(auto state:bucket.states)put_i32(staged,state);
    }
    output=std::move(staged);error.clear();return true;
}

bool CharacterQuestProgressV1::decode(const CharacterState& character,
    const dh2::data::QuestTablesPersistenceV51& tables,
    const std::vector<std::uint8_t>& bytes,std::string& error) {
    if(character.id.empty()||!tables.ready()||tables.rows().size()>maximum_quest_rows)
        return fail(error,"Quest progress decode requires a named CharacterState and bounded original table");
    Reader reader{bytes};
    if(bytes.size()<16||std::memcmp(bytes.data(),"CQPG",4)!=0)
        return fail(error,"Quest progress codec magic is invalid");
    reader.at=4;
    std::uint32_t version{},rows{},name_size{};
    std::string encoded_name;
    // Schema v4: CQPG v2 appends objective counters (character_quest_blob.hpp);
    // this state-only model reads v1 and v2 buckets and ignores the counters
    // (the quest runtime owns them via read_quest_counters/write_quest_counters).
    if(!reader.u32(version)||version<codec_version||version>character_quest_blob_max_version||!reader.u32(rows)||
       rows!=tables.rows().size()||!reader.u32(name_size)||name_size>4096||
       !reader.raw_string(name_size,encoded_name)||encoded_name!=character.id)
        return fail(error,"Quest progress codec version/table/Character identity mismatch");
    std::array<std::array<Bucket,3>,2> staged{};
    for(auto& collection:staged)for(auto& bucket:collection){
        std::uint8_t origin{};
        if(!reader.u8(origin)||origin>static_cast<std::uint8_t>(Origin::loaded))
            return fail(error,"Quest progress codec bucket origin is invalid");
        bucket.origin=static_cast<Origin>(origin);
        if(bucket.origin==Origin::unknown)continue;
        if(bucket.origin!=Origin::fresh_source_initialized&&
           bucket.origin!=Origin::modified&&bucket.origin!=Origin::loaded)
            return fail(error,"Quest progress codec bucket origin is invalid");
        bucket.origin=Origin::loaded;
        std::uint32_t count{};
        if(!reader.i32(bucket.current_quest)||!reader.u32(count)||count!=rows)
            return fail(error,"Quest progress codec initialized bucket is truncated or has wrong row count");
        if(bucket.current_quest < -1 || (bucket.current_quest>=0&&std::uint32_t(bucket.current_quest)>=rows))
            return fail(error,"Quest progress codec currentquest row is outside source table");
        bucket.states.reserve(count);
        for(std::uint32_t row=0;row<count;++row){std::int32_t state{};
            if(!reader.i32(state))return fail(error,"Quest progress codec state array is truncated");
            bucket.states.push_back(state);
        }
    }
    if(version==1){
        if(reader.at!=bytes.size())return fail(error,"Quest progress codec has trailing bytes");
    }else{
        std::vector<QuestObjectiveCounterV2> counters;std::string counter_error;
        if(!decode_quest_counter_section(bytes,CharacterQuestBlobLayout{version,rows,reader.at},counters,counter_error))
            return fail(error,"Quest progress codec v2 counter section is invalid");
    }
    character_id_=character.id;buckets_=std::move(staged);error.clear();return true;
}

bool CharacterQuestProgressV1::bucket(const CharacterState& character,
    std::uint32_t collection,std::int32_t difficulty,BucketView& out,std::string& error) const {
    if(!belongs_to(character))return fail(error,"Quest progress belongs to a different CharacterState");
    if(collection>1||difficulty<0||difficulty>=3)return fail(error,"Quest source collection/difficulty is outside original bounds");
    const auto& source=buckets_[collection][std::size_t(difficulty)];
    out={source.origin,source.current_quest,source.origin==Origin::unknown?nullptr:&source.states};
    error.clear();return true;
}

bool CharacterQuestProgressV1::query(const CharacterState& character,
    const CharacterQuestIdV1& id,CharacterQuestStateV1& out,std::string& error) const {
    if(!belongs_to(character))return fail(error,"Quest progress belongs to a different CharacterState");
    if(!valid_id(id))return fail(error,"Quest state is unknown or source row identity is outside original bounds");
    const auto& source=buckets_[id.collection][std::size_t(id.difficulty)];
    out.state=source.states[std::size_t(id.row)];
    if(source.current_quest>=0)out.current_quest_row=source.current_quest;else out.current_quest_row.reset();
    error.clear();return true;
}

bool CharacterQuestProgressV1::record_source_state(const CharacterState& character,
    const CharacterQuestIdV1& id,std::int32_t state,std::string& error) {
    if(!belongs_to(character))return fail(error,"Quest progress belongs to a different CharacterState");
    if(!valid_id(id))return fail(error,"Quest source state update targets an unknown/uninitialized row");
    buckets_[id.collection][std::size_t(id.difficulty)].states[std::size_t(id.row)]=state;
    buckets_[id.collection][std::size_t(id.difficulty)].origin=Origin::modified;
    error.clear();return true;
}

bool CharacterQuestProgressV1::filter(const CharacterState& character,
    const dh2::data::QuestTablesPersistenceV51& tables,std::uint32_t collection,
    std::int32_t difficulty,CharacterQuestCategoryV1 category,
    const CharacterQuestLogPolicyV1& policy,const CharacterQuestTextV1& text,
    std::vector<CharacterQuestPageRowV1>& output,bool& title_sorted,
    std::string& error) const {
    title_sorted=false;
    if(!belongs_to(character))return fail(error,"Quest progress belongs to a different CharacterState");
    if(!tables.ready()||tables.rows().size()>maximum_quest_rows||collection>1||difficulty<0||difficulty>=3)
        return fail(error,"Quest query requires original table and valid source collection/difficulty");
    const auto& source=buckets_[collection][std::size_t(difficulty)];
    if(source.origin==Origin::unknown)return fail(error,"Quest source state is unavailable until explicit initialization/load");
    if(source.states.size()!=tables.rows().size())return fail(error,"Quest state row count no longer matches original definitions");
    std::vector<CharacterQuestPageRowV1> staged;
    for(std::size_t row_index=0;row_index<tables.rows().size();++row_index){
        const auto& definition=tables.rows()[row_index];
        const auto state=source.states[row_index];
        if(!valid_policy_category(definition,state,category,policy))continue;
        CharacterQuestPageRowV1 row;
        row.id={collection,difficulty,static_cast<std::int32_t>(row_index)};
        row.source_name=definition.name;row.text_ids=definition.text_fields;
        row.priority=definition.priority;row.act=definition.act;row.target_level=definition.target_level;
        row.repeatable=definition.repeatable!=0;row.state=state;
        row.current=source.current_quest==static_cast<std::int32_t>(row_index);
        const auto title_id=row.text_ids[0];
        if(title_id<0||title_id==source_string_sentinel) {
            row.title="not specified";
        } else if(text) {
            std::string localized;
            if(!text(character,title_id,localized,error))return false;
            row.title=std::move(localized);
        } else {
            title_sorted=false;
        }
        staged.push_back(std::move(row));
    }
    const bool all_titles=std::all_of(staged.begin(),staged.end(),
        [](const auto& row){return row.title.has_value();});
    if(all_titles){
        std::stable_sort(staged.begin(),staged.end(),[](const auto& left,const auto& right){
            return *left.title<*right.title;
        });
        title_sorted=true;
    }
    output=std::move(staged);error.clear();return true;
}

bool CharacterQuestProgressV1::activate(const CharacterState& character,
    const dh2::data::QuestTablesPersistenceV51& tables,const CharacterQuestIdV1& id,
    const CharacterQuestLogPolicyV1& policy,std::string& error) {
    if(!belongs_to(character))return fail(error,"Quest progress belongs to a different CharacterState");
    if(!tables.ready()||id.collection>1||id.difficulty<0||id.difficulty>=3||id.row<0||
       std::size_t(id.row)>=tables.rows().size()||!valid_id(id))
        return fail(error,"Quest activation source row is unknown or outside original bounds");
    const auto& definition=tables.rows()[std::size_t(id.row)];
    const auto& bucket=buckets_[id.collection][std::size_t(id.difficulty)];
    if(!valid_policy_category(definition,bucket.states[std::size_t(id.row)],
                              CharacterQuestCategoryV1::assigned,policy))
        return fail(error,"Source Make Active action requires an explicitly Assigned Quest");
    buckets_[id.collection][std::size_t(id.difficulty)].current_quest=id.row;
    buckets_[id.collection][std::size_t(id.difficulty)].origin=Origin::modified;
    error.clear();return true;
}

} // namespace dh::foundation
