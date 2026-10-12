package com.example.dh2;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.content.Intent;
import android.content.Context;
import android.content.BroadcastReceiver;
import android.content.IntentFilter;
import android.content.pm.ApplicationInfo;
import android.os.Build;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.MotionEvent;
import android.view.Gravity;
import android.view.PixelCopy;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.widget.*;
import java.util.Arrays;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class MainActivity extends Activity {
    private GLSurfaceView surface;
    private FrameLayout viewport;
    private volatile ImageView gameplayLoadingOverlay;
    private volatile Bitmap gameplayLoadingSnapshot;
    private volatile int gameplayStartGeneration;
    private boolean pendingStartAfterSplashFrame;
    private int pendingStartSlot=-1;
    private boolean pendingStartHasNumericDifficulty;
    private int pendingStartDifficulty;
    private boolean awaitingFirstWorldFrame;
    private boolean awaitingRestoredMenuFrame;
    private TextView status;
    private String[] assets=new String[0];
    private final FrontMenuTouchGestureV1 frontMenuTouchGesture=new FrontMenuTouchGestureV1();
    private int frontMenuTouchPointer=-1;
    private volatile int selected;
    private volatile boolean ready;
    private MovementControl movement;
    private Button attack;
    private TextView vitals;
    private volatile String loadedAsset;
    private volatile String baseReport;
    private volatile boolean pendingActorCommand;
    private volatile boolean enemyAi=true;
    private volatile int inspectionTimeMs=-1;
    private int metadataSlot=-1;
    private boolean inspectionMode;
    private int debugLevelRow=-1;
    private FrontAudio frontAudio;
    private volatile IntroCinematicView introCinematic;
    private OpeningSequenceV1 openingSequence;
    // Read on the GL owner after its menu frame loads settings; suppresses a
    // UI-thread post on every frame while still tracking in-game language edits.
    private int appliedIntroLanguage = -1;
    private boolean frontMenuReady;
    private boolean titleMusicStarted;
    private int menuGameplaySlot=-1;
    private boolean menuHasNumericDifficulty;
    private int menuRequestedDifficulty;
    private volatile boolean menuReturnRetryPending;
    private int menuReturnRetryAttempts;
    private volatile boolean attackDisplayPending;
    private BroadcastReceiver debugAttackReceiver;
    @Override public void onCreate(Bundle state) {
        setTheme(android.R.style.Theme_Material_NoActionBar);super.onCreate(state);
        NativeBridge.bindApplicationContext(getApplicationContext());
        frontAudio=new FrontAudio(this);
        enemyAi=state!=null?state.getBoolean("enemyAi",true):getIntent().getBooleanExtra("enemy_ai",true);
        inspectionTimeMs=state!=null?state.getInt("inspectionTimeMs",-1):getIntent().getIntExtra("time_ms",-1);
        menuGameplaySlot=state!=null?state.getInt("menuGameplaySlot",-1):-1;
        menuHasNumericDifficulty=state!=null&&state.getBoolean("menuHasNumericDifficulty",false);
        menuRequestedDifficulty=state!=null?state.getInt("menuRequestedDifficulty",0):0;
        boolean debugBuild=(getApplicationInfo().flags&ApplicationInfo.FLAG_DEBUGGABLE)!=0;
        debugLevelRow=debugBuild?getIntent().getIntExtra("debug_level_row",-1):-1;
        if(debugLevelRow!=-1&&debugLevelRow!=23&&debugLevelRow!=4){Log.w("DH2Native","Ignoring unsupported debug LevelList row "+debugLevelRow);debugLevelRow=-1;}
        if(debugLevelRow!=-1)Log.i("DH2Native","Debug LevelList row override armed | row "+debugLevelRow+" | profile save unchanged");
        // Debug builds only: drive the same move-kill/move-defend checks as the desktop runtime.
        if(debugBuild&&getIntent().getStringExtra("debug_scenario")!=null)
            NativeBridge.debugScenario(getIntent().getStringExtra("debug_scenario"));
        inspectionMode=getIntent().hasExtra("world")||getIntent().hasExtra("model")||getIntent().hasExtra("texture");
        boolean resumeOpeningCinematic=state!=null&&state.getBoolean("openingCinematicPending",false);
        int openingCinematicPositionMs=state!=null?state.getInt("openingCinematicPositionMs",0):0;
        boolean playOpeningCinematic=!inspectionMode&&(state==null||resumeOpeningCinematic)&&!getIntent().getBooleanExtra("skip_intro",false);
        long openingNow=android.os.SystemClock.uptimeMillis();
        int openingStage=inspectionMode?OpeningSequenceV1.MENU:
                getIntent().getBooleanExtra("skip_intro",false)?OpeningSequenceV1.SPLASH:
                state!=null?state.getInt("openingSequenceStage",playOpeningCinematic?OpeningSequenceV1.INTRO:OpeningSequenceV1.MENU):
                (playOpeningCinematic?OpeningSequenceV1.INTRO:OpeningSequenceV1.MENU);
        openingSequence=OpeningSequenceV1.restore(openingStage,
                getIntent().getBooleanExtra("skip_intro",false)?0L:
                        state!=null?state.getLong("openingSplashElapsedMs",0L):0L,openingNow);
        if(!inspectionMode){
            // This is a landscape game. Android 17 keeps orientation requests
            // available to apps explicitly categorized as games; retain the
            // source layout instead of rotating a SurfaceView into portrait.
            setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
            // Apply edge-to-edge before the first layout so the startup movie,
            // menu stage, and GL surface agree on the same drawable bounds.
            configureImmersiveLandscapeWindow();
        }
        if(Build.VERSION.SDK_INT>=33)getOnBackInvokedDispatcher().registerOnBackInvokedCallback(android.window.OnBackInvokedDispatcher.PRIORITY_DEFAULT,this::handleBack);
        metadataSlot=state!=null?state.getInt("metadataSlot",-1):getIntent().getIntExtra("profile_slot",-1);
        LinearLayout layout=new LinearLayout(this);layout.setOrientation(LinearLayout.VERTICAL);
        layout.setBackgroundColor(Color.rgb(24,27,32));
        if(inspectionMode)layout.setOnApplyWindowInsetsListener((view,insets)->{
            if(android.os.Build.VERSION.SDK_INT>=30){
                android.graphics.Insets bars=insets.getInsets(android.view.WindowInsets.Type.systemBars());
                view.setPadding(bars.left,bars.top,bars.right,bars.bottom);
            }else view.setPadding(insets.getSystemWindowInsetLeft(),insets.getSystemWindowInsetTop(),insets.getSystemWindowInsetRight(),insets.getSystemWindowInsetBottom());
            return insets;
        });
        TextView title=new TextView(this);title.setText("Dungeon Hunter 2 - Native 3D source");
        title.setTextColor(Color.WHITE);title.setTextSize(18);title.setPadding(16,12,16,8);layout.addView(title);
        status=new TextView(this);status.setTextColor(Color.rgb(210,220,230));status.setPadding(16,8,16,8);
        status.setText(NativeBridge.buildInfo());
        try{
            String[] textures=getAssets().list("textures"),models=getAssets().list("models"),worlds=getAssets().list("worlds");
            Arrays.sort(textures);Arrays.sort(models);java.util.ArrayList<String> names=new java.util.ArrayList<>();
            names.add("ui/original-main-menu");
            for(String name:worlds)if(name.endsWith(".dwld"))names.add("worlds/"+name);
            for(String name:models)names.add("models/"+name);
            for(String name:textures)names.add("textures/"+name);assets=names.toArray(new String[0]);
        }
        catch(Exception e){Log.e("DH2Native","Asset listing failed",e);status.setText(e.toString());}
        String requested=state!=null?state.getString("asset"):null;
        pendingActorCommand=state!=null?state.getBoolean("pendingPlayerAttack",false):getIntent().getBooleanExtra("player_attack",false);
        if(requested==null&&getIntent().getStringExtra("model")!=null)requested="models/"+getIntent().getStringExtra("model");
        if(requested==null&&getIntent().getStringExtra("texture")!=null)requested="textures/"+getIntent().getStringExtra("texture");
        if(requested==null&&getIntent().getStringExtra("world")!=null)requested="worlds/"+getIntent().getStringExtra("world");
        if(requested!=null){for(int i=0;i<assets.length;i++)if(assets[i].equals(requested))selected=i;}
        Spinner picker=new Spinner(this);
        ArrayAdapter<String> adapter=new ArrayAdapter<>(this,android.R.layout.simple_spinner_item,assets);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);picker.setAdapter(adapter);
        picker.setSelection(selected);
        layout.addView(picker);layout.addView(status);
        if(!inspectionMode){title.setVisibility(View.GONE);picker.setVisibility(View.GONE);status.setVisibility(View.GONE);}
        surface=new GLSurfaceView(this);surface.setContentDescription("DH2 native texture viewport");surface.setEGLContextClientVersion(2);
        surface.setEGLConfigChooser(8,8,8,8,16,0);
        surface.setOnTouchListener(new View.OnTouchListener(){
            float x,y;
            int pinchPointerA=-1,pinchPointerB=-1;
            int panPointer=-1;
            boolean swfPointerActive,gestureCancelled;
            float previousPinchDistance;
            float panLastX,panLastY,panRemainderX,panRemainderY;
            private int pointerIndex(MotionEvent event,int pointerId){
                for(int i=0;i<event.getPointerCount();i++)if(event.getPointerId(i)==pointerId)return i;
                return -1;
            }
            private float pinchDistance(MotionEvent event,int first,int second){
                int a=pointerIndex(event,first),b=pointerIndex(event,second);
                if(a<0||b<0)return -1;
                return CameraTouchGestureV1.drawableDistance(
                        event.getX(a),event.getY(a),event.getX(b),event.getY(b));
            }
            private void endPinch(){pinchPointerA=pinchPointerB=-1;previousPinchDistance=0;}
            private void beginPan(MotionEvent event,int index){
                panPointer=event.getPointerId(index);panLastX=event.getX(index);panLastY=event.getY(index);
                panRemainderX=panRemainderY=0;
            }
            private void updatePan(MotionEvent event){
                int index=pointerIndex(event,panPointer);if(index<0)return;
                panRemainderX+=CameraTouchGestureV1.drawableDelta(event.getX(index),panLastX);
                panRemainderY+=CameraTouchGestureV1.drawableDelta(event.getY(index),panLastY);
                panLastX=event.getX(index);panLastY=event.getY(index);
                int dx=Math.round(panRemainderX),dy=Math.round(panRemainderY);
                panRemainderX-=dx;panRemainderY-=dy;
                if(dx!=0||dy!=0)surface.queueEvent(()->NativeBridge.cameraTouchPan(dx,dy));
            }
            private void endPan(){panPointer=-1;panRemainderX=panRemainderY=0;}
            @Override public boolean onTouch(View view,MotionEvent event){
                if("ui/original-main-menu".equals(loadedAsset)){
                    final int action=event.getActionMasked(),actionIndex=event.getActionIndex();
                    final FrontMenuTouchGestureV1.Action input;
                    if(action==MotionEvent.ACTION_DOWN)input=FrontMenuTouchGestureV1.Action.DOWN;
                    else if(action==MotionEvent.ACTION_MOVE)input=FrontMenuTouchGestureV1.Action.MOVE;
                    else if(action==MotionEvent.ACTION_UP)input=FrontMenuTouchGestureV1.Action.UP;
                    else if(action==MotionEvent.ACTION_CANCEL)input=FrontMenuTouchGestureV1.Action.CANCEL;
                    else if(action==MotionEvent.ACTION_POINTER_DOWN)input=FrontMenuTouchGestureV1.Action.POINTER_DOWN;
                    else if(action==MotionEvent.ACTION_POINTER_UP)input=FrontMenuTouchGestureV1.Action.POINTER_UP;
                    else return true;
                    if(action==MotionEvent.ACTION_DOWN)frontMenuTouchPointer=event.getPointerId(0);
                    final boolean activePointerLifted=action==MotionEvent.ACTION_POINTER_UP&&
                            event.getPointerId(actionIndex)==frontMenuTouchPointer;
                    final FrontMenuTouchGestureV1.Decision decision=
                            frontMenuTouchGesture.accept(input,activePointerLifted);
                    if(decision==FrontMenuTouchGestureV1.Decision.SUPPRESS){
                        if(action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_CANCEL)frontMenuTouchPointer=-1;
                        return true;
                    }
                    int pointerIndex=event.findPointerIndex(frontMenuTouchPointer);
                    if(pointerIndex<0&&event.getPointerCount()>0)pointerIndex=0;
                    if(action==MotionEvent.ACTION_POINTER_UP&&activePointerLifted)pointerIndex=actionIndex;
                    if(pointerIndex<0){frontMenuTouchPointer=-1;return true;}
                    final int forwardedAction=decision==FrontMenuTouchGestureV1.Decision.CANCEL?
                            MotionEvent.ACTION_CANCEL:action;
                    final float px=event.getX(pointerIndex),py=event.getY(pointerIndex);
                    if(decision==FrontMenuTouchGestureV1.Decision.CANCEL||
                            action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_CANCEL)
                        frontMenuTouchPointer=-1;
                    return forwardUiTouch(view,px,py,forwardedAction,true);
                }
                if(!inspectionMode&&loadedAsset!=null&&loadedAsset.startsWith("worlds/")){
                    final int action=event.getActionMasked();
                    if(action==MotionEvent.ACTION_DOWN){
                        gestureCancelled=false;swfPointerActive=true;beginPan(event,0);
                    }
                    if(action==MotionEvent.ACTION_POINTER_DOWN){
                        if(event.getPointerCount()==2&&pinchPointerA<0){
                            int added=event.getActionIndex();
                            pinchPointerA=event.getPointerId(added==0?1:0);
                            pinchPointerB=event.getPointerId(added);
                            previousPinchDistance=pinchDistance(event,pinchPointerA,pinchPointerB);
                            // A pinch takes ownership from the original SWF pointer.
                            // Cancel it once and suppress this whole gesture until a fresh DOWN.
                            if(swfPointerActive){
                                int active=pointerIndex(event,panPointer);
                                if(active<0)active=added==0?1:0;
                                forwardUiTouch(view,event.getX(active),event.getY(active),MotionEvent.ACTION_CANCEL,false);
                                swfPointerActive=false;
                            }
                            gestureCancelled=true;endPan();
                        }else if(pinchPointerA<0){
                            if(swfPointerActive){
                                int active=pointerIndex(event,panPointer);
                                if(active<0)active=0;
                                forwardUiTouch(view,event.getX(active),event.getY(active),MotionEvent.ACTION_CANCEL,false);
                                swfPointerActive=false;
                            }
                            gestureCancelled=true;endPan();
                        }
                    }else if(action==MotionEvent.ACTION_MOVE&&pinchPointerA>=0){
                        float distance=pinchDistance(event,pinchPointerA,pinchPointerB);
                        if(distance>=0&&previousPinchDistance>0){
                            final float before=previousPinchDistance,after=distance;
                            surface.queueEvent(()->NativeBridge.cameraPinchZoom(before,after));
                            previousPinchDistance=distance;
                        }
                    }else if(action==MotionEvent.ACTION_MOVE&&!gestureCancelled&&pinchPointerA<0&&event.getPointerCount()==1&&panPointer>=0){
                        updatePan(event);
                    }else if(action==MotionEvent.ACTION_POINTER_UP){
                        int lifted=event.getPointerId(event.getActionIndex());
                        boolean endingPinch=pinchPointerA>=0&&(lifted==pinchPointerA||lifted==pinchPointerB);
                        if(endingPinch)endPinch();
                        int remaining=event.getPointerCount()-1;
                        int resumedPan=CameraTouchGestureV1.resumedPanPointerIndex(
                                endingPinch,event.getPointerCount(),event.getActionIndex());
                        if(resumedPan>=0){
                            // ZoomHandler returns to its one-pointer pan path after one finger lifts.
                            // Keep the SWF touch canceled while restoring only the camera drag.
                            beginPan(event,resumedPan);gestureCancelled=false;
                        }else if(gestureCancelled||remaining!=1)endPan();
                    }else if(action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_CANCEL){
                        endPinch();endPan();gestureCancelled=false;
                    }
                    if(swfPointerActive&&(action==MotionEvent.ACTION_DOWN||action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_MOVE||action==MotionEvent.ACTION_CANCEL)){
                        if(action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_CANCEL)swfPointerActive=false;
                        return forwardUiTouch(view,event.getX(),event.getY(),action,false);
                    }
                    if(action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_CANCEL)gestureCancelled=false;
                    return true;
                }
                if(event.getActionMasked()==MotionEvent.ACTION_DOWN){x=event.getX();y=event.getY();return true;}
                if(event.getActionMasked()==MotionEvent.ACTION_MOVE){
                    if(assets[selected].startsWith("worlds/"))return true;
                    float dx=(event.getX()-x)*.008f,dy=(event.getY()-y)*.008f;x=event.getX();y=event.getY();
                    surface.queueEvent(()->NativeBridge.orbit(dx,dy,1));return true;
                }
                if(event.getActionMasked()==MotionEvent.ACTION_UP){view.performClick();return true;}return true;
            }
        });
        surface.setRenderer(new GLSurfaceView.Renderer(){
            long lastVitals;
            boolean firstSurfaceFrame;
            boolean pendingSurfaceLoad;
            @Override public void onSurfaceCreated(GL10 gl,EGLConfig config){
                ready=false;loadedAsset=null;
                java.io.File external=getExternalFilesDir(null);
                java.io.File mods=ModDirectoryPolicy.prepare(external,getFilesDir());
                if(mods==null)Log.e("DH2Native","Could not create app-scoped mod directory");
                else if(external!=null&&!mods.getParentFile().equals(external))
                    Log.w("DH2Native","External app storage unavailable; using private mod directory");
                NativeBridge.modDirectory(mods==null?"":mods.getAbsolutePath());
                NativeBridge.runtimeDirectory(getFilesDir().getAbsolutePath());
                Log.i("DH2Native",NativeBridge.profileSlot(metadataSlot));
                String initialization=NativeBridge.initialize(getAssets());Log.i("DH2Native",initialization);
                if(initialization==null||!initialization.startsWith("Renderer: ")){show(initialization);return;}
                ready=true;
                NativeBridge.enemyAi(enemyAi);
                firstSurfaceFrame=true;pendingSurfaceLoad=true;
            }
            @Override public void onSurfaceChanged(GL10 gl,int w,int h){NativeBridge.resize(w,h);firstSurfaceFrame=true;}
            @Override public void onDrawFrame(GL10 gl){if(!ready)return;
                if(firstSurfaceFrame){
                    firstSurfaceFrame=false;
                    if(pendingSurfaceLoad&&assets.length>0&&
                            (menuGameplaySlot>=0&&assets[selected].startsWith("worlds/")||
                                    openingSequence!=null&&openingSequence.usesSplashStage())){
                        pendingSurfaceLoad=false;
                        if(menuGameplaySlot>=0&&assets[selected].startsWith("worlds/"))
                            beginMenuGameStart(menuGameplaySlot,menuHasNumericDifficulty,menuRequestedDifficulty);
                        else loadOpeningSplash();
                        NativeBridge.draw();
                        afterNativeFrame();
                        return;
                    }
                    if(!pendingSurfaceLoad){
                        NativeBridge.draw();
                        afterNativeFrame();
                        return;
                    }
                    // GLSurfaceView's surfaceChanged waits for this frame on
                    // the UI thread. Acknowledge it before decoding the menu
                    // or world so input focus and lifecycle remain responsive.
                    gl.glClearColor(.08f,.09f,.11f,1);gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
                    if(pendingSurfaceLoad){pendingSurfaceLoad=false;surface.post(()->surface.queueEvent(()->{
                        if(menuGameplaySlot>=0&&assets[selected].startsWith("worlds/")){
                            beginMenuGameStart(menuGameplaySlot,menuHasNumericDifficulty,menuRequestedDifficulty);
                        }
                        else if(assets.length>0){
                            if(openingSequence!=null&&openingSequence.usesSplashStage())loadOpeningSplash();
                            else loadSelected();
                        }else show("No bundled asset fixtures");
                    }));}return;
                }
                NativeBridge.draw();
                afterNativeFrame();
                if("ui/opening-splash".equals(loadedAsset)&&openingSequence!=null&&
                        openingSequence.awaitsIntroLanguage()&&introCinematic!=null){
                    int language=NativeBridge.frontLanguage();
                    if(language>=0&&language<8&&language!=appliedIntroLanguage){
                        appliedIntroLanguage=language;IntroCinematicView cinematic=introCinematic;
                        runOnUiThread(()->cinematic.setSourceLanguageIndex(language));
                    }
                }
                if("ui/opening-splash".equals(loadedAsset)&&openingSequence!=null&&
                        openingSequence.mayLoadMenu(android.os.SystemClock.uptimeMillis()))
                    loadAuthoredMenuAfterOpeningSplash();
                if("ui/original-main-menu".equals(loadedAsset)&&!frontMenuReady)
                    runOnUiThread(()->{frontMenuReady=true;startTitleMusicIfReady();});
                String audio=NativeBridge.consumeMenuAudio();
                if(!audio.isEmpty()&&(introCinematic==null||audio.startsWith("volume,")))runOnUiThread(()->{
                    frontAudio.control(audio);
                    if(audio.startsWith("volume,")){
                        IntroCinematicView cinematic=introCinematic;
                        if(cinematic!=null)cinematic.setMusicVolume(frontAudio.musicVolume());
                    }
                });
                String effect;
                do{effect=NativeBridge.consumeMenuSound();if(!effect.isEmpty()&&introCinematic==null){final String cue=effect;runOnUiThread(()->frontAudio.effect(cue));}}while(!effect.isEmpty());
                NativeLaunchRequest launch=NativeBridge.consumeMenuLaunch();if(launch!=null){if(launch.kind==NativeLaunchRequest.START_GAME){beginMenuGameStart(launch.slot,launch.hasNumericDifficulty,launch.requestedDifficulty);}else if(launch.kind==NativeLaunchRequest.LEVEL_TRANSITION){String transition=NativeBridge.transitionLevel(launch.destinationName,launch.rawEntryPoint,getAssets());baseReport=transition;if(transition!=null&&transition.startsWith("Level transition queued:")){loadedAsset="worlds/"+launch.destinationName;Log.i("DH2Native",transition);}else Log.e("DH2Native",transition==null?"NativeGoToZone returned no result":transition);}}long now=android.os.SystemClock.uptimeMillis();if(now-lastVitals>=200&&loadedAsset!=null&&loadedAsset.startsWith("worlds/")){lastVitals=now;int[] values=NativeBridge.playerVitals();if(attackDisplayPending&&values[6]!=5){attackDisplayPending=false;show(baseReport+"\n"+(values[4]!=0?"Defeated":"Attack finished."));}runOnUiThread(()->{vitals.setText(String.format(java.util.Locale.ROOT,"HP %.1f / %.1f   MP %.1f / %.1f%s",values[0]/256f,values[1]/256f,values[2]/256f,values[3]/256f,values[4]!=0?"   Defeated":""));vitals.setTextColor(values[4]!=0||values[5]==0?Color.rgb(255,150,150):Color.WHITE);});}}
    });
        surface.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        viewport=new FrameLayout(this);viewport.addView(surface,new FrameLayout.LayoutParams(-1,-1));
        vitals=new TextView(this);vitals.setContentDescription("Player health and mana");vitals.setTextSize(14);vitals.setTextColor(Color.WHITE);vitals.setPadding(12,8,12,8);vitals.setBackgroundColor(Color.argb(160,15,20,25));vitals.setVisibility(View.GONE);
        FrameLayout.LayoutParams vitalsLayout=new FrameLayout.LayoutParams(-2,-2,Gravity.TOP|Gravity.LEFT);viewport.addView(vitals,vitalsLayout);
        movement=new MovementControl();movement.setVisibility(View.GONE);
        int padSize=(int)(160*getResources().getDisplayMetrics().density);
        FrameLayout.LayoutParams padLayout=new FrameLayout.LayoutParams(padSize,padSize,Gravity.BOTTOM|Gravity.LEFT);
        padLayout.leftMargin=padLayout.bottomMargin=(int)(16*getResources().getDisplayMetrics().density);viewport.addView(movement,padLayout);
        attack=new Button(this);attack.setText("Attack");attack.setContentDescription("Attack nearby enemy");attack.setVisibility(View.GONE);
        FrameLayout.LayoutParams attackLayout=new FrameLayout.LayoutParams((int)(112*getResources().getDisplayMetrics().density),(int)(64*getResources().getDisplayMetrics().density),Gravity.BOTTOM|Gravity.RIGHT);attackLayout.rightMargin=attackLayout.bottomMargin=(int)(24*getResources().getDisplayMetrics().density);viewport.addView(attack,attackLayout);
        final int basePadMargin=(int)(16*getResources().getDisplayMetrics().density);
        final int baseAttackMargin=(int)(24*getResources().getDisplayMetrics().density);
        final int overlayInsetCushion=(int)(8*getResources().getDisplayMetrics().density);
        // The original menu camera uses the complete device bounds. Keep
        // gameplay overlays on that same surface and respect Android insets.
        final int[] safeInsets={0,0,0,0};
        final Runnable positionGameplayOverlays=()->{
            int width=viewport.getWidth(),height=viewport.getHeight();
            if(width<=0||height<=0)return;
            int safeLeft=safeInsets[0],safeTop=safeInsets[1];
            int safeRight=safeInsets[2],safeBottom=safeInsets[3];
            FrameLayout.LayoutParams p=(FrameLayout.LayoutParams)movement.getLayoutParams();
            p.leftMargin=Math.max(basePadMargin,safeLeft+overlayInsetCushion);
            p.bottomMargin=Math.max(basePadMargin,safeBottom+overlayInsetCushion);
            movement.setLayoutParams(p);
            FrameLayout.LayoutParams a=(FrameLayout.LayoutParams)attack.getLayoutParams();
            a.rightMargin=Math.max(baseAttackMargin,safeRight+overlayInsetCushion);
            a.bottomMargin=Math.max(baseAttackMargin,safeBottom+overlayInsetCushion);
            attack.setLayoutParams(a);
            FrameLayout.LayoutParams v=(FrameLayout.LayoutParams)vitals.getLayoutParams();
            v.leftMargin=Math.max(basePadMargin,safeLeft+overlayInsetCushion);
            v.topMargin=Math.max(basePadMargin,safeTop+overlayInsetCushion);
            vitals.setLayoutParams(v);
        };
        viewport.addOnLayoutChangeListener((view,left,top,right,bottom,oldLeft,oldTop,oldRight,oldBottom)->positionGameplayOverlays.run());
        androidx.core.view.ViewCompat.setOnApplyWindowInsetsListener(viewport,(view,insets)->{
            androidx.core.graphics.Insets safe=insets.getInsets(
                    androidx.core.view.WindowInsetsCompat.Type.displayCutout()|
                    androidx.core.view.WindowInsetsCompat.Type.systemGestures());
            safeInsets[0]=safe.left;safeInsets[1]=safe.top;safeInsets[2]=safe.right;safeInsets[3]=safe.bottom;
            positionGameplayOverlays.run();
            return insets;
        });
        attack.setOnClickListener(v->surface.queueEvent(()->{String report=NativeBridge.playerAttack(-1);attackDisplayPending=report.equals("Attacking")||attackDisplayPending;Log.i("DH2Native","Player input | "+report);show(baseReport+"\n"+report);}));
        layout.addView(viewport,new LinearLayout.LayoutParams(-1,0,1));
        if(playOpeningCinematic){
            introCinematic=new IntroCinematicView(this,()->runOnUiThread(this::finishOpeningCinematic));
            frontAudio.setCinematic(introCinematic);
            viewport.addView(introCinematic,new FrameLayout.LayoutParams(-1,-1));
        }
        TextView attribution=new TextView(this);
        attribution.setText("Dungeon Hunter 2 © Gameloft SE. Published by GOAT Games Company Limited.");
        attribution.setTextColor(Color.rgb(175,185,195));
        attribution.setTextSize(11);
        attribution.setGravity(Gravity.CENTER);
        attribution.setPadding(12,6,12,8);
        layout.addView(attribution,new LinearLayout.LayoutParams(-1,-2));
        if(!inspectionMode)attribution.setVisibility(View.GONE);
        picker.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener(){
            @Override public void onItemSelected(AdapterView<?> p,View v,int position,long id){selected=position;if(ready)surface.queueEvent(MainActivity.this::loadSelected);}
            @Override public void onNothingSelected(AdapterView<?> p){}
        });
        setContentView(layout);
        androidx.core.view.ViewCompat.requestApplyInsets(viewport);
        if(!inspectionMode)getWindow().getDecorView().post(this::enterImmersiveLandscape);
        if(introCinematic!=null)introCinematic.startPlayback(openingCinematicPositionMs);
        // Debug-only shell command invokes the same playerAttack entry point
        // as the button without am start pausing the Activity/held joystick.
        // DUMP is a system/shell permission, not granted to ordinary apps.
        if((getApplicationInfo().flags&ApplicationInfo.FLAG_DEBUGGABLE)!=0){
            debugAttackReceiver=new BroadcastReceiver(){
                @Override public void onReceive(Context context,Intent intent){
                    if(!ready||loadedAsset==null||!loadedAsset.startsWith("worlds/"))return;
                    if("com.example.dh2.DEBUG_RELOAD_WORLD".equals(intent.getAction())){
                        final String world=loadedAsset;
                        surface.queueEvent(()->{
                            try {
                                byte[] encoded=NativeBridge.readAsset(world,getAssets());
                                String report=NativeBridge.loadWorld(encoded,getAssets());
                                baseReport=world+"\n"+report;Log.i("DH2Native","World reload command applied | "+report);
                                show(baseReport);surface.requestRender();
                            } catch(java.io.IOException e) {Log.e("DH2Native","World reload failed",e);show("World reload failed: "+e.getMessage());}
                        });
                        return;
                    }
                    if("com.example.dh2.DEBUG_SPAWN_CHARACTER".equals(intent.getAction())){
                        final String name=intent.getStringExtra("character_name");
                        surface.queueEvent(()->{String report=NativeBridge.spawnCharacter(name);Log.i("DH2Native","Spawn command applied | "+report);show(baseReport+"\n"+report);surface.requestRender();});
                        return;
                    }
                    if("com.example.dh2.DEBUG_CHARACTER_HIT".equals(intent.getAction())){
                        final String name=intent.getStringExtra("character_name");
                        final int damage=intent.getIntExtra("raw_damage",0);
                        surface.queueEvent(()->{String report=NativeBridge.debugCharacterHit(name,damage);Log.i("DH2Native","Debug Character hit command applied | "+report);surface.requestRender();});
                        return;
                    }
                    if("com.example.dh2.DEBUG_ANIMATION_TIME".equals(intent.getAction())){
                        final int time=intent.getIntExtra("time_ms",-1);
                        inspectionTimeMs=time;
                        surface.queueEvent(()->{NativeBridge.animationTime(time);Log.i("DH2Native","Animation time command applied | time "+time);surface.requestRender();});
                        return;
                    }
                    final int target=intent.getIntExtra("player_target_index",-1);
                    if("com.example.dh2.DEBUG_PLAYER_SCALAR".equals(intent.getAction())){
                        final int value=intent.getIntExtra("raw_value",0);
                        final boolean write=intent.getBooleanExtra("write",false);
                        surface.queueEvent(()->{String report=NativeBridge.debugPlayerScalar(value,write);Log.i("DH2Native","Player scalar command applied | "+report);surface.requestRender();});
                        return;
                    }
                    if("com.example.dh2.DEBUG_PLAYER_MANA".equals(intent.getAction())){
                        final int amount=intent.getIntExtra("raw_amount",0);
                        surface.queueEvent(()->{String report=NativeBridge.debugPlayerMana(amount);Log.i("DH2Native","Player mana command applied | "+report);surface.requestRender();});
                        return;
                    }
                    if("com.example.dh2.DEBUG_PLAYER_DEATH".equals(intent.getAction())){
                        surface.queueEvent(()->{String report=NativeBridge.debugPlayerDeath();Log.i("DH2Native","Player death fixture applied | "+report);surface.requestRender();});
                        return;
                    }
                    if("com.example.dh2.DEBUG_PLAYER_SKILL_CHECK".equals(intent.getAction())){
                        final int slot=intent.getIntExtra("skill_slot",0);
                        surface.queueEvent(()->{String report=NativeBridge.debugPlayerSkillCheck(slot);Log.i("DH2Native","Player skill check command applied | "+report);surface.requestRender();});
                        return;
                    }
                    if("com.example.dh2.DEBUG_PLAYER_SKILL_COOLDOWN".equals(intent.getAction())){
                        final int delay=intent.getIntExtra("delay_ms",3500);
                        surface.queueEvent(()->{String report=NativeBridge.debugPlayerSkillCooldown(delay);Log.i("DH2Native","Player skill cooldown command applied | "+report);surface.requestRender();});
                        return;
                    }
                    surface.queueEvent(()->{String report=NativeBridge.playerAttack(target);attackDisplayPending=report.equals("Attacking")||attackDisplayPending;Log.i("DH2Native","Player command applied | "+report);show(baseReport+"\n"+report);});
                }
            };
            IntentFilter filter=new IntentFilter("com.example.dh2.DEBUG_PLAYER_ATTACK");
            filter.addAction("com.example.dh2.DEBUG_ANIMATION_TIME");
            filter.addAction("com.example.dh2.DEBUG_SPAWN_CHARACTER");
            filter.addAction("com.example.dh2.DEBUG_CHARACTER_HIT");
            filter.addAction("com.example.dh2.DEBUG_RELOAD_WORLD");
            filter.addAction("com.example.dh2.DEBUG_PLAYER_SKILL_COOLDOWN");
            filter.addAction("com.example.dh2.DEBUG_PLAYER_SKILL_CHECK");
            filter.addAction("com.example.dh2.DEBUG_PLAYER_MANA");
            filter.addAction("com.example.dh2.DEBUG_PLAYER_SCALAR");
            filter.addAction("com.example.dh2.DEBUG_PLAYER_DEATH");
            if(Build.VERSION.SDK_INT>=33)registerReceiver(debugAttackReceiver,filter,"android.permission.DUMP",null,Context.RECEIVER_EXPORTED);
            else registerReceiver(debugAttackReceiver,filter,"android.permission.DUMP",null);
        }
    }
    private void show(String text){runOnUiThread(()->status.setText(text));}
    @SuppressWarnings("deprecation")
    private void enterImmersiveLandscape(){
        configureImmersiveLandscapeWindow();
        if(Build.VERSION.SDK_INT>=30){
            WindowInsetsController controller=getWindow().getInsetsController();
            if(controller!=null){
                controller.hide(WindowInsets.Type.systemBars());
                controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        }else getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN|View.SYSTEM_UI_FLAG_HIDE_NAVIGATION|View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }
    private void configureImmersiveLandscapeWindow(){
        androidx.core.view.WindowCompat.setDecorFitsSystemWindows(getWindow(),false);
        if(Build.VERSION.SDK_INT>=28){
            android.view.WindowManager.LayoutParams attributes=getWindow().getAttributes();
            if(attributes.layoutInDisplayCutoutMode!=android.view.WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES){
                attributes.layoutInDisplayCutoutMode=android.view.WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
                getWindow().setAttributes(attributes);
            }
        }
    }
    @Override public void onWindowFocusChanged(boolean hasFocus){
        super.onWindowFocusChanged(hasFocus);
        if(frontAudio!=null)frontAudio.onWindowFocusChanged(hasFocus);
        if(hasFocus&&!inspectionMode)enterImmersiveLandscape();
    }
    @Override public void onConfigurationChanged(android.content.res.Configuration configuration){
        super.onConfigurationChanged(configuration);
        if(!inspectionMode){
            enterImmersiveLandscape();
            // Let GLSurfaceView deliver its normal onSurfaceChanged callback.
            // That callback updates the native viewport before the next frame,
            // so touch remains in the same local-pixel space as rendered UI.
            if(surface!=null)surface.requestLayout();
        }
    }
    private boolean forwardUiTouch(View view,float x,float y,int action,boolean recoverMenuOnError){
        // Keep input in GLSurfaceView-local pixels. Native input bounds follow
        // the active full-surface viewport and the SWF applies its stage mapping.
        surface.queueEvent(()->{
            String reply=NativeBridge.menuTouch(x,y,action);
            if("GAMEPLAY_MAIN_MENU_REQUESTED".equals(reply)){
                movement.stop();
                for(int i=0;i<assets.length;i++)if("ui/original-main-menu".equals(assets[i])){selected=i;break;}
                surface.queueEvent(()->loadSelected(true));
                return;
            }
            if(reply!=null&&!reply.isEmpty()){
                final String actionName=action==MotionEvent.ACTION_DOWN?"DOWN":
                        action==MotionEvent.ACTION_UP?"UP":
                        action==MotionEvent.ACTION_MOVE?"MOVE":
                        action==MotionEvent.ACTION_CANCEL?"CANCEL":"OTHER";
                Log.w("DH2Native","Native UI touch failure | action="+actionName+" | "+reply);
                if(recoverMenuOnError){
                    runOnUiThread(()->android.widget.Toast.makeText(MainActivity.this,reply,android.widget.Toast.LENGTH_LONG).show());
                    // Recover on the GL thread after the failed ActionScript
                    // scope unwinds; this reload rebinds the retained SWF renderer.
                    surface.queueEvent(()->NativeBridge.loadFrontScreen(
                            getFilesDir().getAbsolutePath(),getAssets()));
                }
            }
        });
        if(action==MotionEvent.ACTION_UP)view.performClick();
        return true;
    }
    private void finishOpeningCinematic(){
        // MediaPlayer completion is posted to the view handler. The activity
        // may be destroyed before that queued callback runs; don't restart
        // title audio from a torn-down window.
        if(!OpeningFlowGate.mayApplyCinematicCompletion(isFinishing(),isDestroyed()))return;
        IntroCinematicView current=introCinematic;
        if(current==null)return;
        current.dispose();
        if(current.getParent() instanceof FrameLayout)((FrameLayout)current.getParent()).removeView(current);
        introCinematic=null;
        frontAudio.setCinematic(null);
        if(openingSequence!=null)openingSequence.introFinished(android.os.SystemClock.uptimeMillis());
        if(surface!=null)surface.requestRender();
        startTitleMusicIfReady();
    }
    private void loadOpeningSplash(){
        String report=NativeBridge.loadOpeningSplash(getFilesDir().getAbsolutePath(),getAssets());
        loadedAsset=report.startsWith("Original opening splash ready")?"ui/opening-splash":null;
        baseReport=report;frontMenuReady=false;
        Log.i("DH2Native",report);
        surface.requestRender();
    }
    private void beginMenuGameStart(int slot,boolean hasNumericDifficulty,int requestedDifficulty){
        pendingStartSlot=slot;
        pendingStartHasNumericDifficulty=hasNumericDifficulty;
        pendingStartDifficulty=requestedDifficulty;
        gameplayStartGeneration++;
        loadOpeningSplash();
        if("ui/opening-splash".equals(loadedAsset)){
            pendingStartAfterSplashFrame=true;
        }else{
            Log.w("DH2Native","Could not prepare original loading image before game start | "+baseReport);
            startPendingMenuGame(gameplayStartGeneration);
        }
    }
    private void afterNativeFrame(){
        if(pendingStartAfterSplashFrame&&"ui/opening-splash".equals(loadedAsset)){
            pendingStartAfterSplashFrame=false;
            final int generation=gameplayStartGeneration;
            // PixelCopy and the overlay are UI-thread work. Waiting until the
            // next animation callback lets the just-drawn source splash reach
            // the SurfaceView before the blocking native world load begins.
            surface.postOnAnimation(()->captureLoadingSplashAndStart(generation));
        }
        if(awaitingFirstWorldFrame&&loadedAsset!=null&&loadedAsset.startsWith("worlds/")){
            awaitingFirstWorldFrame=false;
            postLoadingOverlayRemovalAfterPresentedFrame(gameplayStartGeneration);
        }
        if(awaitingRestoredMenuFrame&&"ui/original-main-menu".equals(loadedAsset)){
            awaitingRestoredMenuFrame=false;
            postLoadingOverlayRemovalAfterPresentedFrame(gameplayStartGeneration);
        }
    }
    private void captureLoadingSplashAndStart(int generation){
        if(generation!=gameplayStartGeneration||isFinishing()||isDestroyed()||surface==null)return;
        int width=surface.getWidth(),height=surface.getHeight();
        if(width<=0||height<=0){surface.queueEvent(()->startPendingMenuGame(generation));return;}
        final Bitmap snapshot;
        try{
            snapshot=Bitmap.createBitmap(width,height,Bitmap.Config.ARGB_8888);
        }catch(RuntimeException error){
            Log.w("DH2Native","Could not allocate loading splash snapshot",error);
            surface.queueEvent(()->startPendingMenuGame(generation));
            return;
        }
        try{
            PixelCopy.request(surface,snapshot,copyResult->{
                if(generation!=gameplayStartGeneration||isFinishing()||isDestroyed()){
                    snapshot.recycle();return;
                }
                if(copyResult!=PixelCopy.SUCCESS){
                    Log.w("DH2Native","Loading splash capture failed | PixelCopy="+copyResult);
                    snapshot.recycle();
                    surface.queueEvent(()->startPendingMenuGame(generation));
                    return;
                }
                ImageView overlay=new ImageView(MainActivity.this);
                overlay.setScaleType(ImageView.ScaleType.FIT_XY);
                overlay.setImageBitmap(snapshot);
                overlay.setContentDescription("Dungeon Hunter 2 loading screen");
                gameplayLoadingSnapshot=snapshot;
                gameplayLoadingOverlay=overlay;
                viewport.addView(overlay,new FrameLayout.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT));
                overlay.bringToFront();
                // One UI frame draws the full-surface captured source image before
                // the GL thread enters synchronous NativeStartGame.
                overlay.postOnAnimation(()->overlay.postOnAnimation(()->
                        surface.queueEvent(()->startPendingMenuGame(generation))));
            },new android.os.Handler(android.os.Looper.getMainLooper()));
        }catch(RuntimeException error){
            Log.w("DH2Native","Could not capture loading splash surface",error);
            snapshot.recycle();
            surface.queueEvent(()->startPendingMenuGame(generation));
        }
    }
    private void startPendingMenuGame(int generation){
        if(generation!=gameplayStartGeneration||pendingStartSlot<0)return;
        int slot=pendingStartSlot;
        boolean numeric=pendingStartHasNumericDifficulty;
        int difficulty=pendingStartDifficulty;
        pendingStartSlot=-1;
        String report=startMenuGame(slot,numeric,difficulty);
        acceptGameStart(slot,report,numeric,difficulty);
    }
    private void postLoadingOverlayRemovalAfterPresentedFrame(int generation){
        // afterNativeFrame() runs before GLSurfaceView swaps its buffer. Queue
        // a GL event so this UI callback cannot remove the captured splash
        // while the first world/menu frame is still being rendered.
        surface.queueEvent(()->surface.post(()->surface.postOnAnimation(()->{
            if(generation==gameplayStartGeneration)removeGameplayLoadingOverlay();
        })));
    }
    private void removeGameplayLoadingOverlay(){
        ImageView overlay=gameplayLoadingOverlay;
        gameplayLoadingOverlay=null;
        if(overlay!=null){
            if(overlay.getParent() instanceof ViewGroup)((ViewGroup)overlay.getParent()).removeView(overlay);
            overlay.setImageDrawable(null);
        }
        Bitmap snapshot=gameplayLoadingSnapshot;
        gameplayLoadingSnapshot=null;
        if(snapshot!=null&&!snapshot.isRecycled())snapshot.recycle();
    }
    private void loadAuthoredMenuAfterOpeningSplash(){
        for(int i=0;i<assets.length;i++)if(assets[i].equals("ui/original-main-menu")){selected=i;break;}
        loadSelected();
        if(openingSequence!=null)openingSequence.menuLoaded();
    }
    private void startTitleMusicIfReady(){
        if(OpeningFlowGate.mayStartTitleMusic(
                frontMenuReady,introCinematic!=null,titleMusicStarted)){
            titleMusicStarted=true;
            frontAudio.title();
            Log.i("DH2Native","Original title music started after opening flow");
        }
    }
    private String startMenuGame(int slot){return startMenuGame(slot,false,0);}
    private String startMenuGame(int slot,boolean hasNumericDifficulty,int requestedDifficulty){return NativeBridge.startMenuGame(slot,getAssets(),debugLevelRow,hasNumericDifficulty,requestedDifficulty);}
    @Override protected void onNewIntent(Intent intent){
        if(intent.hasExtra("profile_slot")&&intent.getIntExtra("profile_slot",-1)!=metadataSlot){
            Log.i("DH2Native","Campaign metadata slot change rejected | requested "+intent.getIntExtra("profile_slot",-1)+" | active "+metadataSlot+" | relaunch required");
            show("Relaunch the app to select another campaign import.");
            return;
        }
        super.onNewIntent(intent);setIntent(intent);
        if(intent.hasExtra("debug_level_row")&&(getApplicationInfo().flags&ApplicationInfo.FLAG_DEBUGGABLE)!=0){
            int requestedRow=intent.getIntExtra("debug_level_row",-1);
            if(requestedRow==-1||requestedRow==23||requestedRow==4){debugLevelRow=requestedRow;if(requestedRow!=-1)Log.i("DH2Native","Debug LevelList row override armed | row "+requestedRow+" | profile save unchanged");}
            else Log.w("DH2Native","Ignoring unsupported debug LevelList row "+requestedRow);
        }
        if(intent.hasExtra("enemy_ai"))enemyAi=intent.getBooleanExtra("enemy_ai",true);
        if(intent.hasExtra("time_ms"))inspectionTimeMs=intent.getIntExtra("time_ms",-1);
        pendingActorCommand=true;
        if(!ready||loadedAsset==null||!loadedAsset.startsWith("worlds/"))return;
        final int index=intent.getIntExtra("object_index",-1),time=inspectionTimeMs,target=intent.getIntExtra("combat_target_index",-1);
        final String state=intent.getStringExtra("object_state");
        surface.queueEvent(()->{
            NativeBridge.enemyAi(enemyAi);
            NativeBridge.animationTime(time);NativeBridge.focusObject(index);
            if(intent.getBooleanExtra("player_attack",false)){String report=NativeBridge.playerAttack(intent.getIntExtra("player_target_index",-1));Log.i("DH2Native","Player command applied | "+report);pendingActorCommand=false;show(baseReport+"\n"+report);return;}
            String targetReport=index>=0&&(state!=null||intent.hasExtra("combat_target_index"))?NativeBridge.combatTarget(index,target):"Combat target unchanged";
            boolean accepted=targetReport.equals("Combat target selected")||targetReport.equals("Combat target cleared")||targetReport.equals("Combat target unchanged");
            String report=accepted?(state==null?"Actor state unchanged":NativeBridge.objectState(index,state)):targetReport;
            Log.i("DH2Native","Actor command applied | index "+index+" | state "+state+" | time "+time+" | "+report);
            pendingActorCommand=false;
            show(baseReport+"\n"+report);surface.requestRender();
        });
    }
    private void loadSelected(){loadSelected(false);}
    private void loadSelected(boolean gameplayExit){
        String name=assets[selected];
        if(name.equals(loadedAsset))return;
        runOnUiThread(()->{boolean inspectionControls=inspectionMode&&name.startsWith("worlds/");movement.setVisibility(inspectionControls?View.VISIBLE:View.GONE);vitals.setVisibility(inspectionControls?View.VISIBLE:View.GONE);attack.setVisibility(inspectionControls?View.VISIBLE:View.GONE);});
        try{
            if(name.equals("ui/original-main-menu")){
                String report=gameplayExit?NativeBridge.returnToMainMenu(getFilesDir().getAbsolutePath(),getAssets()):NativeBridge.loadFrontScreen(getFilesDir().getAbsolutePath(),getAssets());
                if(gameplayExit&&!report.startsWith("Original menu ready")){
                    baseReport=report;Log.w("DH2Native",report);show(report);
                    if(report.startsWith("Menu return deferred: ")&&
                       !menuReturnRetryPending&&menuReturnRetryAttempts<5){
                        menuReturnRetryPending=true;
                        status.postDelayed(()->{
                            if(isFinishing()||surface==null){menuReturnRetryPending=false;return;}
                            surface.queueEvent(()->{
                                menuReturnRetryPending=false;menuReturnRetryAttempts++;
                                loadSelected(true);
                            });
                        },100);
                    }
                    return;
                }
                menuReturnRetryPending=false;menuReturnRetryAttempts=0;
                menuGameplaySlot=-1;
                appliedIntroLanguage=-1;
                loadedAsset=name;baseReport=report;Log.i("DH2Native",report);show(report);
                runOnUiThread(()->{frontMenuReady=false;if(!report.startsWith("Original menu ready")){titleMusicStarted=false;frontAudio.stop();}});return;
            }
            runOnUiThread(()->frontMenuReady=false);
            byte[] encoded=NativeBridge.readAsset(name,getAssets());
            if(encoded==null)throw new java.io.IOException("Asset read failed");
            String report=name.startsWith("worlds/")?NativeBridge.loadWorld(encoded,getAssets()):name.startsWith("models/")?NativeBridge.loadModel(encoded,getAssets()):NativeBridge.loadTexture(encoded);
            if(!report.contains("failed")&&!report.contains("error"))loadedAsset=name;
            baseReport=name+"\n"+report;
            if(name.startsWith("models/")||name.startsWith("worlds/"))NativeBridge.animationTime(inspectionTimeMs);
            if(name.startsWith("worlds/"))NativeBridge.focusObject(getIntent().getIntExtra("object_index",-1));
            if(name.startsWith("worlds/")&&pendingActorCommand&&getIntent().getBooleanExtra("player_attack",false)){
                String attackReport=NativeBridge.playerAttack(getIntent().getIntExtra("player_target_index",-1));report+="\n"+attackReport;
                Log.i("DH2Native","Player command applied | "+attackReport);pendingActorCommand=false;
            }
            if(name.startsWith("worlds/")&&(getIntent().getStringExtra("object_state")!=null||(pendingActorCommand&&getIntent().hasExtra("combat_target_index")))&&(!report.contains("Native combat resumed")||pendingActorCommand)){
                int index=getIntent().getIntExtra("object_index",-1);
                String targetReport=NativeBridge.combatTarget(index,getIntent().getIntExtra("combat_target_index",-1));
                String state=getIntent().getStringExtra("object_state");
                boolean accepted=targetReport.equals("Combat target selected")||targetReport.equals("Combat target cleared");
                report+="\n"+(accepted?(state==null?"Actor state unchanged":NativeBridge.objectState(index,state)):targetReport);
            }
            Log.i("DH2Native",name+": "+report);
            if(name.startsWith("worlds/")&&pendingActorCommand){
                Log.i("DH2Native","Actor command applied | index "+getIntent().getIntExtra("object_index",-1)+" | state "+getIntent().getStringExtra("object_state")+" | time "+getIntent().getIntExtra("time_ms",-1)+" | "+report);
                pendingActorCommand=false;
            }
            show(name+"\n"+report);surface.requestRender();
        }catch(Exception e){Log.e("DH2Native","Asset load failed: "+name,e);show(name+"\n"+e);}
    }
    private void acceptGameStart(int slot,String report,boolean hasNumericDifficulty,int requestedDifficulty){
        Log.i("DH2Native","Menu game start | slot "+slot+" | "+report);
        if(!isPlayableStart(report)){
            menuGameplaySlot=-1;
            for(int i=0;i<assets.length;i++)if("ui/original-main-menu".equals(assets[i])){selected=i;break;}
            loadSelected();
            awaitingRestoredMenuFrame=true;
            final String failure=report;
            baseReport=failure;
            runOnUiThread(()->{
                frontMenuReady=false;titleMusicStarted=false;frontAudio.stop();
                status.setVisibility(View.GONE);
                new android.app.AlertDialog.Builder(MainActivity.this)
                    .setTitle("Unable to start game")
                    .setMessage(failure+"\n\nProfile slot "+slot+
                        (hasNumericDifficulty?" · difficulty "+requestedDifficulty:""))
                    .setPositiveButton("Retry",(dialog,which)->surface.queueEvent(()->
                        beginMenuGameStart(slot,hasNumericDifficulty,requestedDifficulty)))
                    .setNegativeButton("Main menu",(dialog,which)->{})
                    .show();
            });
            Log.w("DH2Native","NativeStartGame failed; authored menu restored | slot "+slot+
                    " | numeric difficulty "+hasNumericDifficulty+"/"+requestedDifficulty+
                    " | "+failure);
            return;
        }
        menuGameplaySlot=slot;menuHasNumericDifficulty=hasNumericDifficulty;menuRequestedDifficulty=requestedDifficulty;
        loadedAsset=report.startsWith("SWAMP |")?"worlds/001_swamp.dwld":"worlds/crypt01.dwld";baseReport=report;
        for(int i=0;i<assets.length;i++)if(assets[i].equals(loadedAsset)){selected=i;break;}
        awaitingFirstWorldFrame=true;
        runOnUiThread(()->{frontMenuReady=false;titleMusicStarted=false;frontAudio.stop();movement.setVisibility(inspectionMode?View.VISIBLE:View.GONE);attack.setVisibility(inspectionMode?View.VISIBLE:View.GONE);vitals.setVisibility(inspectionMode?View.VISIBLE:View.GONE);status.setVisibility(inspectionMode?View.VISIBLE:View.GONE);});
    }
    private static boolean isPlayableStart(String report){
        return report!=null&&(report.startsWith("Crypt |")||report.startsWith("SWAMP |"));
    }
    private void handleBack(){
        if(!inspectionMode&&ready&&loadedAsset!=null&&loadedAsset.startsWith("worlds/")){
            movement.stop();surface.queueEvent(()->{
                String reply=NativeBridge.backToHud();
                if(reply!=null&&!reply.isEmpty()){
                    Log.w("DH2Native",reply);
                    runOnUiThread(()->android.widget.Toast.makeText(MainActivity.this,reply,android.widget.Toast.LENGTH_LONG).show());
                }
            });return;
        }
        if(!inspectionMode&&ready&&!"ui/original-main-menu".equals(loadedAsset)){
            movement.stop();selected=0;surface.queueEvent(this::loadSelected);return;
        }
        finish();
    }
    private void enqueueFrontMenuKey(int code,boolean down){
        GLSurfaceView current=surface;if(current==null)return;
        current.queueEvent(()->{
            String reply=NativeBridge.menuKey(code,down);
            if(reply!=null&&!reply.isEmpty())Log.w("DH2Native",reply);
        });
    }
    @Override public boolean dispatchKeyEvent(android.view.KeyEvent event){
        // Android Back keeps its established Activity/system behavior. The
        // source SWF already has its own on-screen keyboard; this path adds
        // optional hardware/IME key input only while EnterName owns the menu.
        boolean originalFrontMenu=!inspectionMode&&"ui/original-main-menu".equals(loadedAsset);
        boolean nameEntryActive=originalFrontMenu&&NativeBridge.menuTextInputActive();
        if(nameEntryActive){
            if(event.getAction()==android.view.KeyEvent.ACTION_MULTIPLE){
                String characters=event.getCharacters();
                if(characters!=null&&!characters.isEmpty()){
                    boolean handled=true;
                    for(int i=0;i<characters.length();i++){
                        char value=characters.charAt(i);
                        int code=FrontMenuKeyInputV1.gameSwfCode(value);
                        if(code<0){handled=false;break;}
                    }
                    if(handled){
                        surface.queueEvent(()->{
                            for(int i=0;i<characters.length();i++){
                                char value=characters.charAt(i);
                                int code=FrontMenuKeyInputV1.gameSwfCode(value);
                                boolean shift=FrontMenuKeyInputV1.requiresShift(value);
                                if(shift)NativeBridge.menuKey(16,true);
                                NativeBridge.menuKey(code,true);NativeBridge.menuKey(code,false);
                                if(shift)NativeBridge.menuKey(16,false);
                            }
                        });
                        return true;
                    }
                }
            }else{
                int code=FrontMenuKeyInputV1.gameSwfCode(event.getKeyCode());
                if(FrontMenuKeyInputV1.shouldForward(originalFrontMenu,nameEntryActive,
                        event.getAction(),code)){
                    enqueueFrontMenuKey(code,event.getAction()==android.view.KeyEvent.ACTION_DOWN);
                    return true;
                }
            }
        }
        return super.dispatchKeyEvent(event);
    }
    @Override public void onBackPressed(){handleBack();}
    private final class MovementControl extends View {
        private final Paint paint=new Paint(Paint.ANTI_ALIAS_FLAG);private final MovementControlPointerV1 pointer=new MovementControlPointerV1();private float axisX,axisY;
        MovementControl(){super(MainActivity.this);setContentDescription("Movement control");setFocusable(true);}
        @Override protected void onDraw(Canvas canvas){
            float radius=getWidth()*.44f,cx=getWidth()*.5f,cy=getHeight()*.5f;
            paint.setColor(Color.argb(140,55,65,75));canvas.drawCircle(cx,cy,radius,paint);
            paint.setStyle(Paint.Style.STROKE);paint.setStrokeWidth(3);paint.setColor(Color.argb(210,160,180,200));canvas.drawCircle(cx,cy,radius,paint);paint.setStyle(Paint.Style.FILL);
            paint.setColor(Color.argb(220,175,190,205));canvas.drawCircle(cx+axisX*radius*.65f,cy+axisY*radius*.65f,radius*.26f,paint);
        }
        @Override public boolean onTouchEvent(MotionEvent event){
            int action=event.getActionMasked(),actionIndex=event.getActionIndex(),pointerIndex=-1;
            MovementControlPointerV1.Decision decision;
            if(action==MotionEvent.ACTION_DOWN){pointerIndex=actionIndex;decision=pointer.down(event.getPointerId(pointerIndex),event.getX(pointerIndex),event.getY(pointerIndex));}
            else if(action==MotionEvent.ACTION_MOVE){pointerIndex=event.findPointerIndex(pointer.activePointerId());decision=pointer.move(pointerIndex>=0);}
            else if(action==MotionEvent.ACTION_POINTER_DOWN)decision=pointer.pointerDown();
            else if(action==MotionEvent.ACTION_POINTER_UP){pointerIndex=actionIndex;decision=pointer.pointerUp(event.getPointerId(pointerIndex));}
            else if(action==MotionEvent.ACTION_UP){pointerIndex=actionIndex;decision=pointer.up(event.getPointerId(pointerIndex));}
            else if(action==MotionEvent.ACTION_CANCEL)decision=pointer.cancel();
            else return true;
            if(decision==MovementControlPointerV1.Decision.IGNORE)return true;
            if(decision==MovementControlPointerV1.Decision.UPDATE){
                if(action==MotionEvent.ACTION_DOWN){axisX=axisY=0;}
                else{pointer.drag(event.getX(pointerIndex),event.getY(pointerIndex),getWidth()*.44f);axisX=pointer.axisX();axisY=pointer.axisY();}
            }else{axisX=axisY=0;if(action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_CANCEL)performClick();}
            if(decision==MovementControlPointerV1.Decision.RELEASE){surface.queueEvent(NativeBridge::stopMovement);}
            else if(action!=MotionEvent.ACTION_DOWN){float x=axisX,y=-axisY;surface.queueEvent(()->NativeBridge.moveAxis(x,y));}
            invalidate();return true;
        }
        @Override public boolean performClick(){super.performClick();return true;}
        void stop(){MovementControlPointerV1.Decision decision=pointer.cancel();axisX=axisY=0;invalidate();if(decision==MovementControlPointerV1.Decision.RELEASE)surface.queueEvent(NativeBridge::stopMovement);else surface.queueEvent(()->NativeBridge.moveAxis(0,0));}
    }
    @Override protected void onPause(){if(introCinematic!=null)introCinematic.pausePlayback();super.onPause();frontAudio.pause();movement.stop();surface.onPause();}
    @Override protected void onResume(){super.onResume();frontAudio.setCinematic(introCinematic);frontAudio.resume();if(introCinematic!=null)introCinematic.resumePlayback();surface.onResume();}
    @Override protected void onDestroy(){gameplayStartGeneration++;removeGameplayLoadingOverlay();if(debugAttackReceiver!=null)unregisterReceiver(debugAttackReceiver);if(introCinematic!=null)introCinematic.dispose();frontAudio.stop();super.onDestroy();}
    @Override protected void onSaveInstanceState(Bundle state){super.onSaveInstanceState(state);if(assets.length>0)state.putString("asset",assets[selected]);state.putBoolean("pendingPlayerAttack",pendingActorCommand&&getIntent().getBooleanExtra("player_attack",false));state.putBoolean("enemyAi",enemyAi);state.putInt("inspectionTimeMs",inspectionTimeMs);state.putInt("metadataSlot",metadataSlot);state.putInt("menuGameplaySlot",menuGameplaySlot);state.putBoolean("menuHasNumericDifficulty",menuHasNumericDifficulty);state.putInt("menuRequestedDifficulty",menuRequestedDifficulty);state.putBoolean("openingCinematicPending",introCinematic!=null&&introCinematic.isPending());state.putInt("openingCinematicPositionMs",introCinematic!=null?introCinematic.currentPositionMs():0);if(openingSequence!=null){state.putInt("openingSequenceStage",openingSequence.stage());state.putLong("openingSplashElapsedMs",openingSequence.splashElapsedMs(android.os.SystemClock.uptimeMillis()));}}
}
