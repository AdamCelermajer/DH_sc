#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation {
inline constexpr std::size_t character_quest_blob_limit = 128 * 1024;

// The save layer validates the portable CQPG envelope without loading game
// tables. The quest owner additionally checks its row count against the actual
// QuestTables when decoding. Empty data means unknown, never fresh progress.
inline bool validate_character_quest_blob(const std::string& character_id,
    const std::vector<std::uint8_t>& bytes, std::string& error) {
    if(bytes.empty()){error.clear();return true;}
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
    if(!word(version)||version!=1||!word(rows)||rows>4096||!word(name_size)||
       name_size>4096||name_size>bytes.size()-at||name_size!=character_id.size())return fail();
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
    if(at!=bytes.size())return fail();
    error.clear();return true;
}
}
