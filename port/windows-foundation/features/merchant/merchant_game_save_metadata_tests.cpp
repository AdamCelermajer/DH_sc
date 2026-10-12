#include "../../game_save.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool ok,const std::string& why){if(!ok)throw std::runtime_error(why);}
int main(){try{
    GameSave save;save.version=5;save.level_uri="original/test";save.controlled_actor_id=1;
    save.character=make_default_character("hero","Hero","warrior");
    save.character.stats.health=10;save.character.stats.max_health=10;
    save.character.stats.resource=0;save.character.stats.max_resource=0;
    save.character.inventory.push_back({"merchant-instance","WornMallet",1,
      SourceItemMetadata{240,1,{"Power_Frost","Power_Fire"}}});
    PersistedPlayableActor player;player.actor.id=1;player.actor.definition_id="KnightPlayerBase";
    player.actor.faction_id=-1;player.actor.health=10;player.actor.max_health=10;
    player.actor.persistent_character_id=save.character.id;
    player.combat.sheets.resolved[0]=-1;player.combat.sheets.resolved[36]=2560;
    player.combat.sheets.resolved[38]=2560;player.combat.sheets.resolved[41]=0;
    player.combat.sheets.resolved[43]=0;player.traits.is_player=true;
    save.actors.push_back(std::move(player));
    const auto path=std::filesystem::temp_directory_path()/"dh2-b123-game-save-metadata-test.save";
    std::string error;check(save_game(path,save,error),error);
    GameSave loaded;check(load_game(path,loaded,error),error);std::filesystem::remove(path);
    const auto& item=loaded.character.inventory.at(0);
    check(loaded.version==5&&item.source&&item.source->value==240&&item.source->identified==1&&
      item.source->power_names==std::vector<std::string>({"Power_Frost","Power_Fire"}),
      "GameSave v5 failed to round-trip generated item metadata/order");
    save.character.inventory[0].source->identified=2;
    check(!validate_game_save(save,error)&&error.find("identified")!=std::string::npos,
      "invalid generated identification flag passed profile/checkpoint validation");
    std::cout<<"PASS GameSave v5 production roundtrip preserves generated value, identified, ordered source power names; invalid metadata rejected\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
