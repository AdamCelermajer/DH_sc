#include "../app/src/main/cpp/player_gameplay_audio.hpp"

#include <array>
#include <cstdlib>
#include <iostream>
#include <string>

namespace {
void check(bool actual, bool expected, const char* message) {
    if (actual != expected) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}
}

int main() {
    using namespace dh2::player_gameplay_audio;
    std::string cue;
    constexpr char chain_lightning_cue[] = "sfx_skill_mage_thunder_braid_add_2";

    check(enqueue(chain_lightning_cue, sizeof(chain_lightning_cue) - 1, false), true,
          "retained chain-lightning victim cue is accepted");
    check(consume(cue), true, "queued gameplay cue reaches the Android poll bridge");
    check(cue == "dh2fx,play,0,118,0,0,sfx_skill_mage_thunder_braid_add_2.wav", true,
          "play command carries original Sounds UID and normalized PCM filename");
    check(consume(cue), false, "cue is delivered once");

    check(enqueue_crypt_music_state(486,"ambient"),true,
          "committed Crypt source UID queues its ambient VXN state");
    check(consume(cue)&&cue=="dh2music,486,ambient",true,
          "Crypt music state reaches the existing Android polling bridge");
    check(enqueue_crypt_music_state(486,"combat"),true,
          "VXN combat state is transportable for a future source combat trigger");
    check(consume(cue)&&cue=="dh2music,486,combat",true,
          "combat state is preserved without inventing a trigger");
    check(source_vxn_music_resource(484)&&
          std::string(source_vxn_music_resource(484)->source_label)=="AbbeyRuinsOneAmbientMusic"&&
          source_vxn_music_resource(485)&&
          std::string(source_vxn_music_resource(485)->source_label)=="PortalRoomAmbientMusic"&&
          source_vxn_music_resource(486)&&
          std::string(source_vxn_music_resource(486)->source_label)=="CryptOneAmbientMusic"&&
          source_vxn_music_resource(487)&&
          std::string(source_vxn_music_resource(487)->source_label)=="SecretCryptAmbientMusic",
          true,"all four retained source labels resolve to the shared cathedral VXN bank");
    check(enqueue_source_vxn_music_state(484,"ambient")&&consume(cue)&&
          cue=="dh2music,484,ambient",true,
          "shared-bank source UID 484 resolves through the current music command");
    check(enqueue_source_vxn_music_state(487,"combat")&&consume(cue)&&
          cue=="dh2music,487,combat",true,
          "shared-bank source UID 487 retains its selected state");
    check(source_vxn_music_resource(467)&&
          std::string(source_vxn_music_resource(467)->source_label)=="SwampHubAmbientMusic"&&
          enqueue_source_vxn_music_state(467,"ambient")&&consume(cue)&&
          cue=="dh2music,467,ambient",true,
          "SWAMP LevelConfig hub label resolves to its source VXN ambient state");
    check(enqueue_source_vxn_music_state(467,"combat")&&consume(cue)&&
          cue=="dh2music,467,combat",true,
          "SWAMP source VXN combat state reaches the same music queue");
    check(source_music_resource(505)&&
          std::string(source_music_resource(505)->source_label)=="SwampMerchantCampMusic"&&
          enqueue_source_music_state(505,"ambient")&&consume(cue)&&
          cue=="dh2music,505,ambient",true,
          "SWAMP safe-zone ADPCM loop resolves by its source Sounds UID");
    check(!source_vxn_music_resource(505)&&
          !enqueue_source_music_state(505,"combat"),true,
          "single-state safe-zone loop rejects unsupported VXN combat state");
    check(enqueue_crypt_music_state(487,"ambient"),false,
          "another level's VXN UID cannot be mislabeled as Crypt");
    check(enqueue_source_vxn_music_state(488,"ambient")||
          source_vxn_music_resource(488)!=nullptr,false,
          "unmapped VXN IDs stay unsupported");
    check(enqueue_crypt_music_state(486,"boss"),false,
          "unknown VXN states remain fail-closed");

    check(enqueue_play(chain_lightning_cue,sizeof(chain_lightning_cue)-1,0x11,true,80),true,
          "catalogued looping/fade-in gameplay sound is accepted");
    check(consume(cue),true,"looping sound reaches playback");
    check(cue=="dh2fx,play,17,118,80,1,sfx_skill_mage_thunder_braid_add_2.wav",true,
          "source loop and millisecond fade are preserved");
    check(enqueue("sfx_unmapped_enemy_grunt", 22, false), false,
          "unmapped source labels remain fail-closed");
    check(consume(cue), false, "rejected cues never enter the playback queue");

    check(enqueue_play(chain_lightning_cue,sizeof(chain_lightning_cue)-1,0x11,true,0),true,
          "first owner starts its sound");
    check(enqueue_play(chain_lightning_cue,sizeof(chain_lightning_cue)-1,0x22,true,0),true,
          "second owner can play the same sound ID");
    check(enqueue_stop(chain_lightning_cue,sizeof(chain_lightning_cue)-1,0x11,200.75f),true,
          "first owner can stop its sound with a fade");
    check(consume(cue)&&cue=="dh2fx,play,17,118,0,1,sfx_skill_mage_thunder_braid_add_2.wav",true,
          "first owner's start is delivered");
    check(consume(cue)&&cue=="dh2fx,play,34,118,0,1,sfx_skill_mage_thunder_braid_add_2.wav",true,
          "second owner's start is delivered independently");
    check(consume(cue)&&cue=="dh2fx,stop,17,118,200",true,
          "stop retains only its owner's ID and truncated source fade milliseconds");
    check(consume(cue),false,"stop command is delivered once");

    using namespace dh2::engine_audio::spatial_playback_contract_v1;
    check(listener_profile(0)->anchor==3&&listener_profile(0)->max_distance==2500&&
          listener_profile(0)->orientation==0&&listener_profile(0)->reference_distance==3150&&
          listener_profile(0)->rolloff_factor==1.0f&&listener_profile(0)->up_vector==0&&
          listener_profile(1)->anchor==2&&listener_profile(1)->max_distance==1800&&
          listener_profile(1)->orientation==2&&listener_profile(1)->reference_distance==1000&&
          listener_profile(2)->anchor==2&&listener_profile(3)->anchor==1&&
          listener_profile(4)->anchor==0&&listener_profile(-1)==nullptr&&
          listener_profile(5)==nullptr,
          true,"five decoded Listener records preserve selectors and distance parameters");
    Request spatial{};
    spatial.sound_id=118;spatial.has_sound_id=true;
    spatial.emitter_position={3.0f,4.0f,0.0f};spatial.has_emitter_position=true;
    spatial.listener={{0.0f,0.0f,0.0f},{0.0f,0.0f,1.0f},{0.0f,1.0f,0.0f}};
    spatial.has_listener=true;
    spatial.emitter_parameters.by_source_id={50.0f,1.0f,1.0f};
    spatial.has_emitter_parameters=true;
    spatial.has_distance_model=true;
    spatial.raw_parameter_0=1;
    check(enqueue_spatial_play(chain_lightning_cue,sizeof(chain_lightning_cue)-1,
          0x33,false,10,spatial),true,
          "complete source ID0/1/2/3 and listener request reaches the shared audio queue");
    check(consume(cue),true,"typed spatial request reaches the existing JNI poll queue");
    check(cue=="dh2fx,play,51,118,10,0,sfx_skill_mage_thunder_braid_add_2.wav,1,1465,2930",true,
          "transport combines source model-2 distance gain with the exact Q14 pan outputs");

    spatial.distance_model_id=3;
    check(enqueue_spatial_play(chain_lightning_cue,sizeof(chain_lightning_cue)-1,
          0x33,false,0,spatial),false,
          "unsupported distance models fail closed before queueing");
    check(consume(cue),false,"unsupported distance models create no audio command");

    const auto* armored=character_sound_row(1);
    check(armored!=nullptr,true,"source CharSounds row resolves by Character sound-table ID");
    check(std::string(armored->name)=="ArmoredHuman"&&armored->death.count==1&&
          armored->death.ids[0]==28&&armored->hurt.ids==std::array<std::uint16_t,3>{29,30,31}&&
          armored->impact_vs_flesh.ids==std::array<std::uint16_t,3>{120,121,122}&&
          armored->impact_vs_metal.ids==std::array<std::uint16_t,3>{117,118,119}&&
          !armored->is_flesh&&armored->is_metallic,
          true,"CharSounds table preserves original lists and flags");
    check(character_sound_row(0)->death.count==0&&character_sound_row(2)->hurt.count==0&&
          character_sound_row(-1)==nullptr&&character_sound_row(3)==nullptr,
          true,"empty and out-of-range CharSounds rows stay bounded");
    check(character_sound_id_or_default(-1)==2&&character_sound_id_or_default(3)==2&&
          character_sound_id_or_default(1)==1&&
          character_sound_row_for_character(-1)==character_sound_row(2),
          true,"Character+0x1018 uses source default-row fallback for invalid IDs");
    constexpr std::array<std::array<std::uint16_t,2>,10> source_map{{
        {{28,50}},{{29,38}},{{30,39}},{{31,40}},{{117,41}},
        {{118,42}},{{119,43}},{{120,44}},{{121,45}},{{122,46}},
    }};
    for(const auto& mapping:source_map){
        const auto* entry=find_source_sound_id(mapping[0]);
        check(entry&&entry->uid==mapping[1],true,
              "every CharSounds ID resolves to its original sound-pack UID");
    }
    const auto marker_sound=resolve_sfx_marker_sound("skill_mage_thunder_braid_add_2");
    check(source_sound_names_v1.size()==638&&marker_sound.source&&
          marker_sound.source->id==540&&marker_sound.playback&&
          marker_sound.playback->uid==118&&
          marker_sound.status==SourceSoundStatusV1::playable,true,
          "full SDD name table resolves a retained animation sound to its source row");
    bool retained_catalog_ids_match=true;
    for(const auto& entry:catalog)
        retained_catalog_ids_match=retained_catalog_ids_match&&
            entry.source_sound_id<source_sound_names_v1.size()&&
            source_sound_names_v1[entry.source_sound_id].name==entry.label;
    check(retained_catalog_ids_match,true,
          "every retained playback asset carries its exact source Sounds row ID");
    const auto unavailable=resolve_source_sound_name("AbbeyRuinsOneAmbientMusic");
    check(unavailable.status==SourceSoundStatusV1::known_but_unavailable&&
          unavailable.source&&unavailable.source->id==0&&!unavailable.playback,true,
          "known source Sounds names remain distinct from unavailable playback backends");
    const auto marker_unavailable=resolve_sfx_marker_sound("serpent_attack");
    check(marker_unavailable.status==SourceSoundStatusV1::known_but_unavailable&&
          marker_unavailable.source&&marker_unavailable.source->id==514,true,
          "known animation SFX without local PCM is not mistaken for a source-name miss");
    const auto double_prefixed_marker=resolve_sfx_marker_sound(
        "sfx_skill_rogue_arc_burst_shot");
    check(double_prefixed_marker.status==SourceSoundStatusV1::playable&&
          double_prefixed_marker.source&&double_prefixed_marker.source->id==542,true,
          "animation markers containing an authored sfx_ sound label resolve exactly");
    check(resolve_source_sound_name("DefinitelyNotASourceSound").status==
              SourceSoundStatusV1::not_in_source_table,true,
          "names absent from the complete SDD table remain source no-match");

    spatial.sound_id=28;
    spatial.distance_model_id=observed_default_distance_model_id;
    check(enqueue_spatial_source_sound(28,0x44,false,0,spatial),true,
          "CharSounds source ID routes into the existing positional queue");
    check(consume(cue)&&cue=="dh2fx,play,68,50,0,0,sfx_character_death.wav,1,1465,2930",true,
          "spatial bridge sends resolved sound-pack UID and packaged PCM file");
    check(enqueue_spatial_source_sound(27,0x44,false,0,spatial),false,
          "unmapped CharSounds IDs fail closed");
    check(consume(cue),false,"unknown source IDs never reach playback");

    {
        std::lock_guard<std::mutex> lock(queue_mutex());
        auto& pending=queue();
        pending.clear();
        Command stop{};
        stop.kind=Kind::stop;
        pending.push_back(stop);
        const auto stale_at=std::chrono::steady_clock::now()-cue_lifetime-
                            std::chrono::seconds(1);
        for(std::size_t i=1;i<queue_limit;++i){
            Command expired{};
            expired.kind=Kind::play;
            expired.queued_at=stale_at;
            pending.push_back(expired);
        }
    }
    check(enqueue_play(chain_lightning_cue,sizeof(chain_lightning_cue)-1,0x55,false,0),true,
          "expired play cues behind a stop cannot consume the entire queue capacity");
    check(queue().size()==2,true,"pruning preserves stop order and the fresh cue");
    check(consume(cue)&&cue.rfind("dh2fx,stop,",0)==0,true,
          "queued stop remains ahead of the newly accepted cue");
    check(consume(cue)&&cue=="dh2fx,play,85,118,0,0,sfx_skill_mage_thunder_braid_add_2.wav",true,
          "fresh cue survives expired backlog pruning");

    {
        std::lock_guard<std::mutex> lock(queue_mutex());
        auto& pending=queue();
        pending.clear();
        Command ordinary{};
        ordinary.kind=Kind::stop;
        for(std::size_t i=0;i<queue_limit;++i)pending.push_back(ordinary);
    }
    check(enqueue_stop_all(500),true,
          "source lifecycle StopAll can use one reserved slot beyond the ordinary cap");
    check(queue().size()==queue_limit+1,true,
          "reserved StopAll preserves all 64 queued commands");
    check(enqueue_stop_all(500),true,
          "an identical pending tail StopAll coalesces without another slot");
    check(queue().size()==queue_limit+1,true,
          "coalesced StopAll does not grow the queue");
    check(enqueue_play(chain_lightning_cue,sizeof(chain_lightning_cue)-1,0x55,false,0),false,
          "ordinary playback cannot consume the reserved control slot");
    for(std::size_t i=0;i<queue_limit;++i)
        check(consume(cue),true,"ordinary queued commands remain ahead of StopAll");
    check(consume(cue)&&cue=="dh2fx,stopAll,500",true,
          "reserved StopAll remains FIFO after the existing ordinary commands");
    check(enqueue_stop_all(-1),false,"negative StopAll fade is rejected");

    std::cout << "PASS: native gameplay cue and CharSounds UID routing\n";
}
