#include "condition_data_v1.hpp"

#include <algorithm>
#include <climits>
#include <cstring>
#include <limits>

namespace dh2::data::condition_data_v1 {namespace {
struct Reader {
    Bytes bytes;std::uint32_t offset=0;
    bool word(std::uint32_t& value){
        if(!bytes.data||offset>bytes.size||bytes.size-offset<4)return false;
        const auto* p=bytes.data+offset;offset+=4;
        value=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|
              (std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);return true;
    }
    bool text(std::string& value,std::uint32_t limit=255){
        std::uint32_t size=0;if(!word(size)||!size||size>limit||
            offset>bytes.size||size>bytes.size-offset)return false;
        const auto* p=bytes.data+offset;
        if(std::find(p,p+size,std::uint8_t(0))!=p+size)return false;
        value.assign(reinterpret_cast<const char*>(p),size);offset+=size;return true;
    }
    bool done()const{return offset==bytes.size;}
};
bool names(Bytes input,std::vector<std::string>& out){
    Reader r{input};std::uint32_t count=0;if(!r.word(count)||count>4096)return false;
    std::vector<std::string> parsed;parsed.reserve(count);
    for(std::uint32_t i=0;i<count;++i){std::string name;if(!r.text(name))return false;parsed.push_back(std::move(name));}
    if(!r.done())return false;
    out=std::move(parsed);return true;
}
bool schema(Bytes input,std::vector<std::vector<std::string>>& out){
    Reader r{input};std::vector<std::vector<std::string>> parsed;
    while(!r.done()){
        std::uint32_t count=0;if(!r.word(count)||count>16)return false;
        std::vector<std::string> fields;fields.reserve(count);
        for(std::uint32_t i=0;i<count;++i){std::string field;if(!r.text(field))return false;fields.push_back(std::move(field));}
        parsed.push_back(std::move(fields));if(parsed.size()>64)return false;
    }
    out=std::move(parsed);return true;
}
bool constants(Bytes input){
    Reader r{input};std::uint32_t groups=0;if(!r.word(groups)||groups>128)return false;
    for(std::uint32_t g=0;g<groups;++g){
        std::string group;if(!r.text(group))return false;
        std::uint32_t count=0;if(!r.word(count)||count>4096)return false;
        for(std::uint32_t i=0;i<count;++i){
            std::string name;std::uint32_t value=0;
            if(!r.text(name)||!r.word(value))return false;
        }
    }
    return r.done();
}
bool expected_schema(const std::vector<std::vector<std::string>>& fields){
    if(fields.size()!=28||fields[0]!=std::vector<std::string>{"Op"}||
       fields[1]!=std::vector<std::string>{"Op","Op1","Op2"}||
       fields[27]!=std::vector<std::string>{"Conds","Type"})return false;
    for(std::size_t i=2;i<=4;++i)
        if(fields[i]!=std::vector<std::string>{"Op","Quest","State"})return false;
    if(fields[5]!=std::vector<std::string>{"Op","Level","NotUsed1"})return false;
    for(std::size_t i=6;i<=22;++i)
        if(fields[i]!=std::vector<std::string>{"Op","Quest","State"})return false;
    for(std::size_t i=23;i<=26;++i)
        if(fields[i]!=std::vector<std::string>{"Op","Event","State"})return false;
    return true;
}
}

bool load(Bytes packed,Bytes row_names,Bytes row_schema,Bytes csts,Table& out,std::string& error){
    if(!packed.data||!row_names.data||!row_schema.data||!csts.data||
       packed.size>1024*1024||row_names.size>1024*1024||
       row_schema.size>1024*1024||csts.size>1024*1024){error="invalid condition table input";return false;}
    std::vector<std::string> parsed_names;std::vector<std::vector<std::string>> parsed_schema;
    if(!names(row_names,parsed_names)||!schema(row_schema,parsed_schema)||
       !expected_schema(parsed_schema)||!constants(csts)){
        error="condition names, schema, or constants malformed";return false;
    }
    Reader r{packed};std::uint32_t count=0;
    if(!r.word(count)||count!=parsed_names.size()||count>4096){error="condition row count mismatch";return false;}
    Table parsed;parsed.rows.reserve(count);
    for(std::uint32_t i=0;i<count;++i){
        std::uint32_t predicate_count=0;
        if(!r.word(predicate_count)||predicate_count>4096||
           std::uint64_t(predicate_count)*12+4>r.bytes.size-r.offset){error="condition row is truncated";return false;}
        Definition row;row.name=std::move(parsed_names[i]);row.predicates.reserve(predicate_count);
        for(std::uint32_t j=0;j<predicate_count;++j){
            std::uint32_t operation=0,quest=0,state=0;
            if(!r.word(operation)||!r.word(quest)||!r.word(state)){error="condition predicate is truncated";return false;}
            if(operation>2){error="condition operation is outside the recovered quest-state family";return false;}
            std::int32_t signed_quest=0,signed_state=0;
            std::memcpy(&signed_quest,&quest,sizeof(quest));std::memcpy(&signed_state,&state,sizeof(state));
            row.predicates.push_back({static_cast<std::int32_t>(operation),signed_quest,signed_state});
        }
        std::uint32_t type=0;if(!r.word(type)||type>std::uint32_t(INT32_MAX)){error="condition source Type is malformed";return false;}
        row.source_type=static_cast<std::int32_t>(type);parsed.rows.push_back(std::move(row));
    }
    if(!r.done()){error="condition table has trailing bytes";return false;}
    out=std::move(parsed);error.clear();return true;
}

const Definition* find(const Table& table,const char* name)noexcept{
    if(!name)return nullptr;
    for(const auto& row:table.rows)if(row.name==name)return &row;
    return nullptr;
}

Status evaluate(const Definition& definition,bool tested,QuestStateLookup lookup,
                void* context,EvalResult* out)noexcept{
    if(!out)return Status::invalid_argument;
    EvalResult result{};
    if(tested||definition.predicates.empty()){result.value=true;*out=result;return Status::complete;}
    if(!lookup){result.status=Status::service_unavailable;*out=result;return result.status;}
    for(const auto& predicate:definition.predicates){
        if(predicate.operation<0||predicate.operation>2){result.status=Status::unsupported_operation;*out=result;return result.status;}
        std::int32_t current=0;
        if(!lookup(context,predicate.quest_id,&current)){result.value=false;*out=result;return Status::complete;}
        ++result.predicates_evaluated;
        if(!quest_condition_eval_v1::compare_quest_state(predicate.operation,current,predicate.required_state)){
            result.value=false;*out=result;return Status::complete;
        }
    }
    result.value=true;*out=result;return Status::complete;
}
}
