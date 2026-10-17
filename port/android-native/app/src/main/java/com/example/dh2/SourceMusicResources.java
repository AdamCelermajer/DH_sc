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
    private SourceMusicResources() {}

    static Resource resolve(int soundUid,String state) {
        if (soundUid==467 && ("ambient".equals(state)||"combat".equals(state)))
            return new Resource(soundUid,"SwampHubAmbientMusic",
                    "original-media/gameplay/music/swamp_"+state+"_music.wav",state,
                    6_138_880,6_096_650);
        if (soundUid>=484&&soundUid<=487&&
                ("ambient".equals(state)||"combat".equals(state)))
            return new Resource(soundUid,sharedLabel(soundUid),
                    "original-media/gameplay/music/crypt_"+state+"_music.wav",state,
                    5_140_480,5_105_117);
        if (soundUid==505&&"ambient".equals(state))
            return new Resource(soundUid,"SwampMerchantCampMusic",
                    "original-media/gameplay/music/swamp_merchant_camp_music.wav",state,
                    3_591_168,3_565_714);
        return null;
    }
    private static String sharedLabel(int soundUid){
        switch(soundUid){
            case 484:return "AbbeyRuinsOneAmbientMusic";
            case 485:return "PortalRoomAmbientMusic";
            case 486:return "CryptOneAmbientMusic";
            case 487:return "SecretCryptAmbientMusic";
            default:throw new IllegalArgumentException("Unsupported shared VXN UID: "+soundUid);
        }
    }
}
