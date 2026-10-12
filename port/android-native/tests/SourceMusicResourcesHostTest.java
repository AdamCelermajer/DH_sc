package com.example.dh2;

public final class SourceMusicResourcesHostTest {
    public static void main(String[] args) {
        check(467,"ambient","swamp_ambient_music.wav",6_138_880,6_096_650);
        check(467,"combat","swamp_combat_music.wav",6_138_880,6_096_650);
        check(484,"ambient","abbey_safezone_ambient_music.wav",5_140_480,5_105_117);
        check(485,"combat","crypt_combat_music.wav",5_140_480,5_105_117);
        check(486,"ambient","crypt_ambient_music.wav",5_140_480,5_105_117);
        check(487,"combat","crypt_combat_music.wav",5_140_480,5_105_117);
        check(505,"ambient","swamp_merchant_camp_music.wav",3_591_168,3_565_714);
        require(SourceMusicResources.resolve(505,"combat")==null,"safe-zone combat state unsupported");
        require(SourceMusicResources.resolve(466,"ambient")==null,"unknown UID rejected");
        require(SourceMusicResources.resolve(999,"ambient")==null,"unknown UID rejected");
        require(SourceMusicResources.resolve(486,"dead")==null,"unknown state rejected");
        System.out.println("PASS: source UID/state music resource resolver");
    }

    private static void check(int uid,String state,String filename,int payload,int frames) {
        SourceMusicResources.Resource resource=SourceMusicResources.resolve(uid,state);
        require(resource!=null,"supported source UID/state resolves");
        require(resource.assetPath.equals("original-media/gameplay/music/"+filename),"asset path");
        require(resource.payloadBytes==payload,"payload bytes");
        require(resource.sampleFrames==frames,"sample frame count");
        require(state.equals(resource.state),"retained state");
    }
    private static void require(boolean value,String message) {
        if(!value)throw new AssertionError(message);
    }
}
