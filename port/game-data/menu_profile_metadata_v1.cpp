#include "menu_profile_metadata_v1.hpp"
namespace dh2::data {
bool load_menu_profile_metadata_v1(Bytes bytes,const CharacterTable& characters,
    std::int32_t slot,std::int32_t initial,const MenuProfileMetadataServicesV1& services,
    MenuProfileMetadataV1& output,std::string& error){
    PlayerProfileIndexV1 index;
    if(!index.load(bytes,error))return false;
    auto view=index.borrow();PlayerSavegameV1 saved;saved.set_slot(slot);
    // The parameterized source constructor initializes level1; the separate
    // blank PlayerSavegameV1 constructor correctly initializes level0.
    const std::uint8_t level_one[4]={1,0,0,0};std::size_t consumed=0;
    if(!saved.load_level({level_one,4},consumed,error))return false;
    MenuProfileMetadataV1 candidate;candidate.slot=slot;candidate.selected_difficulty=initial;
    auto section=[&](const char* tag,auto reader){
        if(!view.section(tag))return true;
        consumed=0;
        if(reader(view.payload(tag)))return true;
        error=std::string("Menu profile ")+tag+": "+error;return false;
    };
    if(!section("PNAM",[&](Bytes b){return saved.load_name(b,consumed,error);}) ||
       !section("PLVL",[&](Bytes b){return saved.load_level(b,consumed,error);}) ||
       !section("PCLS",[&](Bytes b){return saved.load_class(b,characters.names,consumed,error);}))return false;
    struct Context{const MenuProfileMetadataServicesV1& services;std::int32_t& selected;};
    Context context{services,candidate.selected_difficulty};
    auto store=[](void* raw,std::int32_t selected,std::string& error){
        auto& context=*static_cast<Context*>(raw);context.selected=selected;
        if(!context.services.store_selected_difficulty){error="CurrentDifficulty store provider unavailable";return false;}
        return context.services.store_selected_difficulty(context.services.context,selected,error);
    };
    if(!section("PDFL",[&](Bytes b){return saved.load_difficulty(b,&context,store,consumed,error);}) ||
       !section("LNAM",[&](Bytes b){return saved.load_location(b,consumed,error);}) ||
       !section("LEPT",[&](Bytes b){return saved.load_entry_points(b,consumed,error);}) ||
       !section("LUSP",[&](Bytes b){return saved.load_spawn_points(b,consumed,error);}))return false;
    candidate.location=saved.location();
    if(view.section("QEST")){
        if(!services.load_quest_acts){error="Menu profile QEST requires canonical quest loader";return false;}
        if(!services.load_quest_acts(services.context,view.payload("QEST"),
            candidate.location.current_acts,candidate.location.volatile_acts,error))return false;
    }
    candidate.name=saved.name();candidate.level=saved.level();candidate.character_row=saved.class_id();
    candidate.unlocked_difficulty=saved.unlocked_difficulty();
    output=std::move(candidate);error.clear();return true;
}
}
