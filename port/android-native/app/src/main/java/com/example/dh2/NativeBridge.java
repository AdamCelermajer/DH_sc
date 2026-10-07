package com.example.dh2;
final class NativeBridge {
    static native String resourceBudgetReport();
    static native void debugCastProbe(boolean enabled);
    static { System.loadLibrary("dh2_native"); }
    static native String buildInfo();
    static native String initialize(android.content.res.AssetManager assets);
    static native void audioActivityV40(long owner,long sequence,boolean resumed,boolean windowFocused,boolean granted,boolean destroyed);
    static native boolean audioSourceReadyV40();
    static native long audioApplicationReserveV42();
    static native long audioApplicationOwnerV42();
    static native void audioApplicationCloseV42(long expectedOwner);
    static native String audioApplicationShutdownV42(long expectedOwner);
    // Original Android isSupportMM()I has one return register initialized -1;
    // every manufacturer/model branch returns it (music-support-dex-v1.json).
    static int isSupportMM(){return -1;}
    static native String loadTexture(byte[] encoded);
    static native String loadModel(byte[] encoded,android.content.res.AssetManager assets);
    static native String loadWorld(byte[] encoded,android.content.res.AssetManager assets,String filesDirectory,String selectedMlx);
    static native String loadOriginalHealthPanel(String filesDirectory);
    static native void configureFrontDevice(String manufacturer,String model);
    static native String loadOriginalFrontScreen(String filesDirectory,String screen);
    static native String originalMenuTouch(float x,float y,int action);
    static native String originalMenuPointer(int phase,int pointer,float x,float y);
    static native String consumeOriginalMenuAudio();
    static native String consumeOriginalMenuSound();
    static native String consumeFrontLaunch();
    static native boolean sourceCampaignSceneActive();
    static native String bindIntroMovieV119(MainActivity activity,android.content.res.AssetManager assets);
    static native void closeIntroMovieV119();
    static native void introMovieEventV119(long generation,int event,String error);
    static native byte[] introMovieCacheV119(long generation);
    static native String consumeOriginalUiError();
    static native void moveAxis(float x,float y);
    static native String authoredHudTouch(int phase,int pointer,float x,float y);
    static native void focusObject(int index);
    static native String objectState(int index,String state);
    static native String combatTarget(int index,int target);
    static native String playerAttack(int target);
    static native String playerEquipmentAction(int operation,int index,int slot);
    static native int[] playerVitals();
    // Original HUD members followed by equipped skill ids/levels and faery state.
    static native int[] playerGameplayHud();
    static native String playerGameplayAction(int operation,int index);
    static native String[] playerGameplayIcons();
    static native int[] menuIcon(String name);
    static native String playerCharacterSnapshot();
    static native String playerCharacterAction(int operation,int index,int slot);
    static native void characterPanelOpen(boolean open);
    static native boolean authoredCharacterMenuBack();
    static native boolean authoredCharacterMenuIsOpen();
    static native void enemyAi(boolean enabled);
    static native void orbit(float dx,float dy,float zoom);
    static native void animationTime(int milliseconds);
    static native void resize(int width,int height);
    static native void draw();
}
