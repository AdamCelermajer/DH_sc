#include "fresh_player_profile_v1.hpp"
#include <algorithm>
#include <map>
#include <limits>
#include <stdexcept>
namespace dh2::data {
namespace {
using Buffer=std::vector<std::uint8_t>;
void word(Buffer& b,std::uint32_t v){for(unsigned i=0;i<4;++i)b.push_back(std::uint8_t(v>>(8*i)));}
void text(Buffer& b,const std::string& s){word(b,std::uint32_t(s.size()+1));b.insert(b.end(),s.begin(),s.end());b.push_back(0);}
}
bool fresh_player_profile_v1(const CharacterTable& characters,const char* character,
    const char* name,std::uint32_t real_time,std::uint32_t saved_date,FreshPlayerProfileV1& output,std::string& error){
    if(!character||!name){error="Missing fresh player metadata";return false;}
    // These are the three authored playable bases, not the smaller ClassTable
    // archetype dictionary. Their row IDs come from the genuine CharacterTable.
    const std::string key(character);
    if(key!="KnightPlayerBase"&&key!="RoguePlayerBase"&&key!="MagePlayerBase"){
        error="Non-playable fresh player base";return false;
    }
    auto row=std::find(characters.names.begin(),characters.names.end(),key);
    if(row==characters.names.end()||std::size_t(row-characters.names.begin())>=characters.rows.size()){
        error="Fresh player base absent from CharacterTable";return false;
    }
    try{
        FreshPlayerProfileV1 next;next.name=name;next.character=key;
        next.character_row=std::int32_t(row-characters.names.begin());
        if(next.name.size()>=std::numeric_limits<std::uint32_t>::max()){
            error="Fresh player name exceeds source stream range";return false;
        }
        next.seeds={real_time,real_time+21371u,real_time+86186u};
        next.saved_date=saved_date;
        // SG_Load(1) registers these seven original metadata sections. Source
        // Savegame::saveAll traverses its string-key map in ascending tag order.
        std::map<std::string,Buffer> sections;
        text(sections["PNAM"],next.name);word(sections["PLVL"],1);
        text(sections["PCLS"],next.character);
        word(sections["PDFL"],0);word(sections["PDFL"],0);
        // SG_SetSaveDate stores time(nullptr) at +0x38; __SaveLevelName
        // writes that word before the per-difficulty level/seed/current-act triples.
        auto& location=sections["LNAM"];word(location,next.saved_date);
        for(auto seed:next.seeds){word(location,41);word(location,seed);word(location,1);}
        for(unsigned i=0;i<3;++i)word(sections["LEPT"],0);
        sections["LUSP"]={1,1,1};
        word(next.bytes,std::uint32_t(sections.size()));
        for(const auto& section:sections){
            word(next.bytes,std::uint32_t(section.second.size()));
            next.bytes.insert(next.bytes.end(),section.first.begin(),section.first.end());
            next.bytes.insert(next.bytes.end(),section.second.begin(),section.second.end());
        }
        output=std::move(next);error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
