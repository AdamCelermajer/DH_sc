package com.example.dh2;
final class NativeBridge {
    static { System.loadLibrary("dh2_native"); }
    static native String buildInfo();
    static native String initialize(android.content.res.AssetManager assets);
    static native String loadTexture(byte[] encoded);
    static native String loadModel(byte[] encoded,android.content.res.AssetManager assets);
    static native String loadWorld(byte[] encoded,android.content.res.AssetManager assets,String filesDirectory);
    static native String loadOriginalHealthPanel(String filesDirectory);
    static native String consumeOriginalUiError();
    static native void moveAxis(float x,float y);
    static native void focusObject(int index);
    static native String objectState(int index,String state);
    static native String combatTarget(int index,int target);
    static native String playerAttack(int target);
    static native int[] playerVitals();
    static native void enemyAi(boolean enabled);
    static native void orbit(float dx,float dy,float zoom);
    static native void animationTime(int milliseconds);
    static native void resize(int width,int height);
    static native void draw();
}
