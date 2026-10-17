#include "player_savegame_v1.hpp"
#include <cstdint>
namespace dh2::data {
SavedStateArrayV1::~SavedStateArrayV1() {
    if(words&&memory.release) {
        try {memory.release(memory.context,words);}catch(...) {}
    }
}
namespace {
bool overlap(const void* a,std::size_t n,const void* b,std::size_t m) {
    if(!a||!b||!n||!m)return false;
    const auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    return n>UINTPTR_MAX-x||m>UINTPTR_MAX-y||(x<y+m&&y<x+n);
}
bool fill(SavedStateArrayV1& array,SavedStateTableV1 table,
          const SavedLevelStateServicesV1& services,std::string& error) {
    if(array.words)return true; // Pointer presence, including nonnull size0.
    if(!services.count) {error="actual source table count required";return false;}
    std::uint32_t count=0;
    const auto read_count=[&]() {
        if(!services.count(services.context,table,&count,error))return false;
        if(count>65536){error="saved state table exceeds native bound";return false;}
        return true;
    };
    if(!read_count())return false;
    if(!services.memory.allocate||!services.memory.release) {
        error="source tagged array allocator/release required";return false;
    }
    auto* words=static_cast<std::int32_t*>(services.memory.allocate(services.memory.context,std::size_t(count)*4,0));
    if((count&&!words)||(words&&reinterpret_cast<std::uintptr_t>(words)%alignof(std::int32_t))) {
        if(words)services.memory.release(services.memory.context,words);
        error="saved state allocator returned invalid backing";return false;
    }
    array.words=words;array.count=count;array.memory=services.memory;
    // Publish +68/+74 before the first default read. Counts are reread after
    // allocation and after every store; providers borrow live table storage.
    if(!read_count())return false;
    if(!count)return true;
    if(!services.default_word){error="actual source table default word required";return false;}
    for(std::uint32_t row=0;row<count;++row) {
        if(row>=array.count||!array.words){error="changed table exceeds published saved state allocation";return false;}
        std::int32_t word=0;
        if(!services.default_word(services.context,table,row,&word,error))return false;
        array.words[row]=word;
        if(!read_count())return false;
    }
    return true;
}
}
bool PlayerSavegameV1::initialize_level_states(const SavedLevelStateServicesV1& services,std::string& error) {
    if(overlap(&error,sizeof(error),this,sizeof(*this))||overlap(&error,sizeof(error),&services,sizeof(services)))return false;
    if(level_states_busy_){error="saved level initialization reentry";return false;}
    for(auto* arrays:{&level_states_,&world_map_states_})for(auto& array:*arrays)
        if(overlap(&error,sizeof(error),array.words,std::size_t(array.count)*4)){return false;}
    level_states_busy_=true;
    struct Guard {bool& value;~Guard(){value=false;}} guard{level_states_busy_};
    try {
        for(std::uint32_t difficulty=0;difficulty<3;++difficulty) {
            if(!fill(level_states_[difficulty],SavedStateTableV1::levels,services,error)||
               !fill(world_map_states_[difficulty],SavedStateTableV1::world_map,services,error))return false;
        }
    }catch(...) {if(error.empty())error="source saved state provider threw";return false;}
    error.clear();return true;
}
} // namespace dh2::data
