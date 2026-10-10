package com.example.dh2;
import java.nio.file.*;
import java.util.Locale;

public final class HostRunner {
 static {System.loadLibrary("dh2_native");}
 private static native long configureAssets(String root,String cache);
 private static native String createSurface(int width,int height);
 private static native String capture(String path);
 private static native String snapshot();
 public static final class Assets {public final long nativeHandle;Assets(long handle){nativeHandle=handle;}}
 public static final class Activity {
  public int platformLanguageV119(){return 0;}
  public void startSourceIntroV119(long generation,int language,String name){
   //Exercise the genuine media request/cache/lifecycle, selecting its supported
   //skip event. Codec playback is explicitly outside this menu-startup scenario.
   byte[] bytes=NativeBridge.introMovieCacheV119(generation);
   System.out.println("HOST_PLATFORM intro="+name+" cached_bytes="+(bytes==null?0:bytes.length)+" action=skip");
   NativeBridge.introMovieEventV119(generation,2,null);
  }
 }
 static void check(String error){if(error!=null&&!error.isEmpty())throw new IllegalStateException(error);}
 public static void main(String[] args)throws Exception {
  if(args.length!=3)throw new IllegalArgumentException("assets-root cache.zip output-directory");
  Locale.setDefault(Locale.ENGLISH);Path output=Paths.get(args[2]);Files.createDirectories(output);
  //A fresh scenario uses a separate host-only private save directory.
  Path saves=output.resolve("private-"+java.util.UUID.randomUUID());Files.createDirectory(saves);
  System.out.println("HOST_SCENARIO fresh_private_directory="+saves);
  long handle=configureAssets(args[0],args[1]);if(handle==0)throw new IllegalStateException("Host asset owner failed");
  Assets assets=new Assets(handle);Activity activity=new Activity();long owner=0;int code=0;
  try {
   check(createSurface(854,480));NativeBridge.resize(854,480);
   NativeBridge.configureFrontDevice("Desktop","Mesa GLES2");
   check(NativeBridge.bindIntroMovieV119(activity,assets));
   String initialized=NativeBridge.initialize(assets);System.out.println("INITIALIZE "+initialized);
   if(initialized==null||initialized.contains("failed")||initialized.contains("GL error"))throw new IllegalStateException(initialized);
   owner=NativeBridge.audioApplicationOwnerV42();
   String start=NativeBridge.loadOriginalFrontScreen(saves.toString(),"main");System.out.println("START "+start);
   if(start==null||start.contains("failed"))throw new IllegalStateException(start);
   String previous="";int readyFrames=0;long deadline=System.nanoTime()+45_000_000_000L;
   for(int frame=0;frame<1200&&System.nanoTime()<deadline;++frame){
    NativeBridge.draw();String status=snapshot();
    if(!status.equals(previous)){System.out.println("STATE "+status);previous=status;}
    String error=NativeBridge.consumeOriginalUiError();
    if(error!=null){String captureError=capture(output.resolve("failure.png").toString());
     if(captureError!=null)System.err.println("CAPTURE "+captureError);throw new IllegalStateException(error);}
    if(status.contains("\"startup_ready\":true")&&status.contains("\"main_state_active\":true")&&++readyFrames>=30){
     check(capture(output.resolve("menu.png").toString()));
     System.out.println("RESULT startup_and_30_real_menu_frames=PASS media_playback=false audible_output=false gameplay=false");return;}
    Thread.sleep(16);
   }
   check(capture(output.resolve("timeout.png").toString()));throw new IllegalStateException("Host startup deadline reached");
  } catch(Throwable failure){failure.printStackTrace();code=1;}
  finally {
   NativeBridge.closeIntroMovieV119();if(owner==0)owner=NativeBridge.audioApplicationOwnerV42();
   if(owner!=0){NativeBridge.audioApplicationCloseV42(owner);
    String error=NativeBridge.audioApplicationShutdownV42(owner);if(error!=null){System.err.println("SHUTDOWN "+error);code=1;}}
   if(code!=0)System.exit(code);
  }
 }
}
