package com.example.dh2;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

final class NativeBridge {
    static { System.loadLibrary("dh2_native"); }
    private static volatile Context applicationContext;

    static void bindApplicationContext(Context context) {
        applicationContext = context == null ? null : context.getApplicationContext();
    }

    /** Mirrors the original DungeonHunter2.isWifiAlive active-network type check. */
    @SuppressWarnings("deprecation")
    static boolean platformWifiAlive() {
        Context context = applicationContext;
        if (context == null) return false;
        try {
            ConnectivityManager manager =
                    (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
            if (manager == null) return false;
            NetworkInfo active = manager.getActiveNetworkInfo();
            return active != null && active.getType() == ConnectivityManager.TYPE_WIFI;
        } catch (RuntimeException unavailable) {
            return false;
        }
    }

    static native String buildInfo();
    static native void debugScenario(String scenario);
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
    /**
     * Gameplay multi-touch: one finger event in GLSurfaceView-local pixels.
     * {@code pointer} is the source finger index 0..3 from {@link GameplayTouchSlotsV1};
     * the HUD sees it before the camera pan/pinch, as MenuManager::onEvent precedes ZoomHandler.
     */
    static native String menuTouchPointer(float x,float y,int action,int pointer);
    /** True only while the retained authored front menu is on EnterName. */
    static native boolean menuTextInputActive();
    /** Delivers a GameSWF key code on the GL thread while EnterName is active. */
    static native String menuKey(int gameSwfKeyCode,boolean down);
    static native NativeLaunchRequest consumeMenuLaunch();
    static native String startMenuGame(int slot,android.content.res.AssetManager assets,int debugLevelRow,
                                       boolean hasNumericDifficulty,int requestedDifficulty);
    /** Debug-only: selects or creates a campaign profile slot (desktop select_or_create_source_profile). Returns -1 on failure. */
    static native int debugSelectOrCreateProfile(android.content.res.AssetManager assets);
    /** Dispatches one deferred NativeGoToZone request on the GL owner thread. */
    static native String transitionLevel(String destination,double rawEntryPoint,
                                         android.content.res.AssetManager assets);
    /** Returns (once) the first native frame-path failure, or an empty string. */
    static native String consumeFrameError();
    static native String consumeMenuAudio();
    static native String consumeMenuSound();
    /**
     * Native gameplay SFX engine (OpenSL ES output + mixer consuming the shared
     * queue's dh2fx commands). Returns false when no output could be opened; the
     * caller then keeps its Java effect path. Commands return the engine
     * Outcome ordinal, or -1 while the engine is not open.
     */
    static native boolean sfxOpen(int sampleRate,int bufferFrames,android.content.res.AssetManager assets);
    static native int sfxCommand(String command);
    static native int sfxControl(String control);
    static native void sfxSetActive(boolean active);
    static native void sfxClose();
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
