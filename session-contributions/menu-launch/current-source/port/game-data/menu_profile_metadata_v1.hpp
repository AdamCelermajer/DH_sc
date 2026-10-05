#pragma once
#include "player_savegame_v1.hpp"
#include "player_profile_index_v1.hpp"
namespace dh2::data {
// Temporary metadata for the source menu's PlayerSavegame(slot,17,false).
// This is a display receipt, not a live gameplay Player/Character owner.
struct MenuProfileMetadataV1 {
    std::int32_t slot=-1,level=1,character_row=-1,selected_difficulty=0,unlocked_difficulty=0;
    std::string name;
    SavedLocationV1 location;
};
struct MenuProfileMetadataServicesV1 {
    void* context{};
    bool (*store_selected_difficulty)(void*,std::int32_t,std::string&){};
    // Required when QEST exists. The canonical owner must initialize/load its
    // actual quests with the source regular/rewound-volatile dispatch; both
    // act arrays arrive with LNAM values and must reflect reached QEST stores.
    bool (*load_quest_acts)(void*,Bytes,std::array<std::int32_t,3>& regular,
        std::array<std::int32_t,3>& volatile_acts,std::string&){};
};
// Actual metadata-reader order PNAM,PLVL,PCLS,PDFL,LNAM,LEPT,LUSP,QEST.
// Missing sections retain initialized native/global defaults. Some source
// parameterized-constructor fields (e.g. absent LNAM date) were uninitialized;
// this bounded receipt initializes them to zero and does not claim that as
// an original constructor literal. Malformed present
// sections fail. QEST never silently falls back to LNAM. Output publishes on
// success, while reached global difficulty/canonical service effects remain.
// Original Quest construction is delegated, not implemented by this receipt.
bool load_menu_profile_metadata_v1(Bytes,const CharacterTable&,std::int32_t slot,
    std::int32_t initial_difficulty,const MenuProfileMetadataServicesV1&,
    MenuProfileMetadataV1&,std::string&);
}
