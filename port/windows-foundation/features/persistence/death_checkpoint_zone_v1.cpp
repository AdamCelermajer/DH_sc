#include "death_checkpoint_zone_v1.hpp"

#include <cmath>
#include <exception>

namespace dh::foundation::persistence::death_flow {
namespace {
bool finite(const std::array<float,3>& p) {
    for(float value:p)if(!std::isfinite(value))return false;
    return true;
}
bool same_position(const std::array<float,3>& a,const std::array<float,3>& b) {
    return a[0]==b[0]&&a[1]==b[1]&&a[2]==b[2];
}
bool invoke(bool (*fn)(void*,std::string&),void* context,const char* missing,std::string& error) {
    if(!fn){error=missing;return false;}
    if(!fn(context,error)){if(error.empty())error=missing;return false;}
    error.clear();return true;
}
bool save_level_checkpoint(
    const DeathCheckpointLevelSaveV1& request,
    const DeathCheckpointZoneServicesV1& services,
    DeathCheckpointZoneReceiptV1& receipt,std::string& error) {
    receipt={};
    if(request.source_level_state<=33||!request.level_savegame_present||
       !request.local_character_present||request.local_character_is_dead){
        receipt.result=DeathCheckpointZoneResultV1::skipped_level_or_character_gate;error.clear();return true;
    }
    if(!finite(request.checkpoint_key)||!finite(request.current_character_position)){
        receipt.result=DeathCheckpointZoneResultV1::failed;error="Level checkpoint received a nonfinite source position";return false;
    }
    if(request.level_uri.empty()||request.checkpoint_name.empty()){
        receipt.result=DeathCheckpointZoneResultV1::failed;error="Level checkpoint URI and checkpoint name are required";return false;
    }
    if(!services.record_checkpoint_positions||!services.save_player_qest_checkpoint||
       !services.capture_current_game_save||!services.save_level_checkpoint){
        receipt.result=DeathCheckpointZoneResultV1::failed;error="Source checkpoint producer is incompletely bound";return false;
    }
    const auto& revive=request.arrival?request.checkpoint_key:request.current_character_position;
    try {
        if(!services.record_checkpoint_positions(services.context,request.checkpoint_name,
                request.checkpoint_key,revive,error)){
            if(error.empty())error="Character checkpoint-key/revive-position write failed";
            receipt.result=DeathCheckpointZoneResultV1::failed;return false;
        }
        error.clear();
        if(!invoke(services.save_player_qest_checkpoint,services.context,
                "Character::SG_SaveCheckpoint is required before LevelSavegame checkpoint save",error)){
            receipt.result=DeathCheckpointZoneResultV1::failed;return false;
        }
        GameSave game;
        if(!services.capture_current_game_save(services.context,game,error)){
            if(error.empty())error="Current per-level GameSave capture failed after QEST checkpoint save";
            receipt.result=DeathCheckpointZoneResultV1::failed;return false;
        }
        DeathCheckpointSnapshotV1 snapshot;
        if(!capture_death_checkpoint_snapshot_v1(game,request.level_uri,request.checkpoint_name,
                request.checkpoint_key,revive,
                services.capture_services,snapshot,error)){
            receipt.result=DeathCheckpointZoneResultV1::failed;return false;
        }
        if(!services.save_level_checkpoint(services.context,game,snapshot,error)){
            if(error.empty())error="LevelSavegame::SaveCheckPoint failed after player QEST save";
            receipt.result=DeathCheckpointZoneResultV1::failed;return false;
        }
        receipt.snapshot=std::move(snapshot);receipt.complete=true;
        receipt.result=DeathCheckpointZoneResultV1::saved;error.clear();return true;
    } catch(const std::exception& ex){receipt.result=DeathCheckpointZoneResultV1::failed;error=ex.what();return false;}
      catch(...){receipt.result=DeathCheckpointZoneResultV1::failed;error="Level checkpoint provider threw after a source-order prefix";return false;}
}
}

bool run_death_checkpoint_level_start_v1(
    const DeathCheckpointLevelSaveV1& request,bool valid_existing_checkpoint,
    DeathCheckpointLevelStartStateV1& state,const DeathCheckpointZoneServicesV1& services,
    DeathCheckpointZoneReceiptV1& receipt,std::string& error) {
    receipt={};
    if(state.complete){receipt.result=DeathCheckpointZoneResultV1::skipped_initial_complete;error.clear();return true;}
    if(valid_existing_checkpoint&&!request.arrival){
        state.complete=true;receipt.result=DeathCheckpointZoneResultV1::skipped_existing_checkpoint;error.clear();return true;
    }
    if(!save_level_checkpoint(request,services,receipt,error))return false;
    state.complete=receipt.complete&&receipt.result==DeathCheckpointZoneResultV1::saved;
    return true;
}

bool run_death_checkpoint_zone_contact_v1(
    const DeathCheckpointZoneContactV1& contact,const DeathCheckpointZoneServicesV1& services,
    DeathCheckpointZoneReceiptV1& receipt,std::string& error) {
    receipt={};
    if(!contact.contact_object_zonable){receipt.result=DeathCheckpointZoneResultV1::skipped_not_zonable;error.clear();return true;}
    if(!contact.contact_character_is_player||!contact.contact_is_local_player_zero||!contact.local_character_present){
        receipt.result=DeathCheckpointZoneResultV1::skipped_not_local_player;error.clear();return true;
    }
    if(!finite(contact.zone_position)||!finite(contact.current_checkpoint_key)||!finite(contact.current_character_position)){
        receipt.result=DeathCheckpointZoneResultV1::failed;error="CheckpointZone received a nonfinite source position";return false;
    }
    if(same_position(contact.zone_position,contact.current_checkpoint_key)){
        receipt.result=DeathCheckpointZoneResultV1::skipped_same_checkpoint_key;error.clear();return true;
    }
    DeathCheckpointLevelSaveV1 request;
    request.source_level_state=contact.source_level_state;
    request.local_character_present=contact.local_character_present;
    request.local_character_is_dead=contact.local_character_is_dead;
    request.level_savegame_present=contact.level_savegame_present;
    request.level_uri=contact.level_uri;request.checkpoint_name=contact.zone_name;
    request.checkpoint_key=contact.zone_position;request.current_character_position=contact.current_character_position;
    return save_level_checkpoint(request,services,receipt,error);
}

} // namespace dh::foundation::persistence::death_flow
