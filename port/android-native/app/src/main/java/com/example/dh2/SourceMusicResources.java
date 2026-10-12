package com.example.dh2;

/** Source Sounds rows whose music assets and state data are retained locally. */
final class SourceMusicResources {
    static final class Resource {
        final int soundUid, payloadBytes, sampleFrames;
        final String label, assetPath, state;
        Resource(int soundUid,String label,String assetPath,String state,
                 int payloadBytes,int sampleFrames){
            this.soundUid=soundUid;this.label=label;this.assetPath=assetPath;
            this.state=state;this.payloadBytes=payloadBytes;this.sampleFrames=sampleFrames;
        }
        boolean sameSegment(Resource other){
            return state.equals(other.state)&&assetPath.equals(other.assetPath);
        }
    }
    /** Asset names are under original-media/gameplay/music; combatAsset is null for single-track rows. */
    private static final class Row {
        final int soundUid, payloadBytes, sampleFrames;
        final String label, ambientAsset, combatAsset;
        Row(int soundUid,String label,String ambientAsset,String combatAsset,
            int payloadBytes,int sampleFrames){
            this.soundUid=soundUid;this.label=label;this.ambientAsset=ambientAsset;
            this.combatAsset=combatAsset;this.payloadBytes=payloadBytes;this.sampleFrames=sampleFrames;
        }
    }
    // From *-music-provenance.json. UIDs 485-487 have no provenance row and keep the cathedral crypt WAVs.
    private static final Row[] ROWS={
        new Row(467,"SwampHubAmbientMusic","swamp_ambient_music.wav","swamp_combat_music.wav",6_138_880,6_096_650),
        new Row(468,"SwampWitchCaveAmbientMusic","swamp_witch_ambient_music.wav","swamp_witch_combat_music.wav",6_138_880,6_096_650),
        new Row(470,"WaterTempleCaveAmbientMusic","water_temple_cave_ambient_music.wav","water_temple_cave_combat_music.wav",6_138_880,6_096_650),
        new Row(471,"DarkWoodHubAmbientMusic","darkwood_ambient_music.wav","darkwood_combat_music.wav",5_846_016,5_805_166),
        new Row(472,"RootDenOneAmbientMusic","root_den_ambient_music.wav","root_den_combat_music.wav",5_846_016,5_805_166),
        new Row(474,"EarthTempleAmbientMusic","earth_temple_ambient_music.wav","earth_temple_combat_music.wav",5_846_016,5_805_166),
        new Row(475,"InfectedVillageAmbientMusic","infected_village_ambient_music.wav","infected_village_combat_music.wav",5_846_016,5_805_166),
        new Row(477,"GothicusCastleHubAmbientMusic","gothicus_no_ambiance_ambient_music.wav","gothicus_no_ambiance_combat_music.wav",4_757_504,4_724_364),
        new Row(484,"AbbeyRuinsOneAmbientMusic","abbey_safezone_ambient_music.wav","abbey_safezone_combat_music.wav",5_140_480,5_105_117),
        new Row(485,"PortalRoomAmbientMusic","crypt_ambient_music.wav","crypt_combat_music.wav",5_140_480,5_105_117),
        new Row(486,"CryptOneAmbientMusic","crypt_ambient_music.wav","crypt_combat_music.wav",5_140_480,5_105_117),
        new Row(487,"SecretCryptAmbientMusic","crypt_ambient_music.wav","crypt_combat_music.wav",5_140_480,5_105_117),
        new Row(488,"RedDesertHubAmbientMusic","red_desert_ambient_music.wav","red_desert_combat_music.wav",4_472_832,4_441_600),
        new Row(489,"RedDesertCaveOneAmbientMusic","red_desert_cave_ambient_music.wav","red_desert_cave_combat_music.wav",4_472_832,4_441_600),
        new Row(492,"LighthouseTempleAmbientMusic","lighthouse_ambient_music.wav","lighthouse_combat_music.wav",4_472_832,4_441_600),
        new Row(493,"FireTempleAmbientMusic","fire_temple_ambient_music.wav","fire_temple_combat_music.wav",4_472_832,4_441_600),
        new Row(494,"IcyCliffHubAmbientMusic","icy_cliff_ambient_music.wav","icy_cliff_combat_music.wav",5_231_616,5_195_283),
        new Row(496,"IcyCaveTwoAmbientMusic","icy_cavern_ambient_music.wav","icy_cavern_combat_music.wav",5_231_616,5_195_283),
        new Row(501,"WindTempleAmbientMusic","wind_temple_ambient_music.wav","wind_temple_combat_music.wav",5_231_616,5_195_283),
        new Row(502,"UnderworldHubAmbientMusic","underworld_ambient_music.wav","underworld_combat_music.wav",4_198_400,4_169_150),
        new Row(503,"MachineRoomsAmbientMusic","machine_rooms_ambient_music.wav","machine_rooms_combat_music.wav",4_198_400,4_169_150),
        new Row(504,"DarkDungeonTempleAmbientMusic","dark_temple_ambient_music.wav","dark_temple_combat_music.wav",4_198_400,4_169_150),
        new Row(505,"SwampMerchantCampMusic","swamp_merchant_camp_music.wav",null,3_591_168,3_565_714),
        new Row(506,"BloodSandsMusic","blood_sands_music.wav",null,3_591_168,3_565_714),
        new Row(507,"KingsGraveMusic","kings_grave_music.wav",null,3_591_168,3_565_716),
        new Row(508,"GatedCastleVillageMusic","gated_castle_village_music.wav",null,3_697_664,3_671_470),
        new Row(509,"NortherlandMusic","northerland_music.wav",null,3_697_664,3_671_470),
    };
    private SourceMusicResources() {}

    static Resource resolve(int soundUid,String state) {
        if(!"ambient".equals(state)&&!"combat".equals(state))return null;
        for(Row row:ROWS){
            if(row.soundUid!=soundUid)continue;
            String asset="ambient".equals(state)?row.ambientAsset:row.combatAsset;
            if(asset==null)return null;
            return new Resource(soundUid,row.label,"original-media/gameplay/music/"+asset,
                    state,row.payloadBytes,row.sampleFrames);
        }
        return null;
    }
}
