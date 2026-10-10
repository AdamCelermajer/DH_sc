package com.example.dh2;
final class NativeBridge {
    static { System.loadLibrary("dh2_native"); }
    static native String buildInfo();
    static native void modDirectory(String directory);
    static native void runtimeDirectory(String directory);
    static native String profileSlot(int selectedSlot);
    static native byte[] readAsset(String name,android.content.res.AssetManager assets) throws java.io.IOException;
    static native String initialize(android.content.res.AssetManager assets);
    /** Loads GSInit's authored splash stage without constructing/showing the main menu. */
    static native String loadOpeningSplash(String directory,android.content.res.AssetManager assets);
    static native String loadFrontScreen(String directory,android.content.res.AssetManager assets);
    /** Returns the one persisted source Language option, or -1 before settings load. */
    static native int frontLanguage();
    static native String returnToMainMenu(String directory,android.content.res.AssetManager assets);
    /** Routes Android system Back through the retained gameplay menu owner. */
    static native String backToHud();
    /** Sends UI input in GLSurfaceView-local physical pixels (top-left origin); action uses MotionEvent DOWN/UP/MOVE/CANCEL values. */
    static native String menuTouch(float x,float y,int action);
    /** True only while the retained authored front menu is on EnterName. */
    static native boolean menuTextInputActive();
    /** Delivers a GameSWF key code on the GL thread while EnterName is active. */
    static native String menuKey(int gameSwfKeyCode,boolean down);
    static native int consumeMenuLaunch(int[] request);
    static native String startMenuGame(int slot,android.content.res.AssetManager assets,int debugLevelRow,
                                       boolean hasNumericDifficulty,int requestedDifficulty);
    static native String consumeMenuAudio();
    static native String consumeMenuSound();
    static native String loadTexture(byte[] encoded);
    static native String loadModel(byte[] encoded,android.content.res.AssetManager assets);
    static native String loadWorld(byte[] encoded,android.content.res.AssetManager assets);
    static native void moveAxis(float x,float y);
    static native void stopMovement();
    /** Routes an independent two-pointer gesture to the active source CameraLevel zoom owner. */
    static native void cameraPinchZoom(float previousDistance,float currentDistance);
    /** Routes a single-pointer logical-stage drag to the active source CameraLevel pan offset. */
    static native void cameraTouchPan(int deltaX,int deltaY);
    static native void focusObject(int index);
    static native String objectState(int index,String state);
    static native String spawnCharacter(String exactName);
    static native String debugCharacterHit(String exactName,int rawDamage);
    static native String debugPlayerSkillCooldown(int delayMs);
    static native String debugPlayerSkillCheck(int slot);
    static native String debugPlayerMana(int rawAmount);
    static native String debugPlayerScalar(int value,boolean write);
    static native String debugPlayerDeath();
    static native String combatTarget(int index,int target);
    static native String playerAttack(int target);
    static native int[] playerVitals();
    static native void enemyAi(boolean enabled);
    static native void orbit(float dx,float dy,float zoom);
    static native void animationTime(int milliseconds);
    static native void resize(int width,int height);
    static native void draw();
}
