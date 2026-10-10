package com.example.dh2;
//Actual desktop JVM -> shipping JNI exports. This declares only harness-used APIs.
final class NativeBridge {
 static native String initialize(Object assets);
 static native void resize(int width,int height);
 static native void draw();
 static native void configureFrontDevice(String manufacturer,String model);
 static native String bindIntroMovieV119(Object activity,Object assets);
 static native void closeIntroMovieV119();
 static native void introMovieEventV119(long generation,int event,String error);
 static native byte[] introMovieCacheV119(long generation);
 static native String loadOriginalFrontScreen(String directory,String screen);
 static native String consumeOriginalUiError();
 static native String originalMenuTouch(float x,float y,int action);
 static native long audioApplicationOwnerV42();
 static native void audioApplicationCloseV42(long owner);
 static native String audioApplicationShutdownV42(long owner);
 //Host runner has no mobile music-management API. This is an explicit platform
 //capability report, not a game/menu callback that accepts missing work.
 public static int isSupportMM(){return 0;}
}
