package com.example.dh2;
import android.app.Activity;
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
import android.content.pm.ActivityInfo;
import android.os.Build;
import android.util.Log;
import android.view.View;
import android.view.MotionEvent;
import android.view.Gravity;
import android.widget.*;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.util.Arrays;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class MainActivity extends Activity {
    private VsyncSurfaceViewV44 surface;
    private volatile FramePacingControllerV44 framePacer;
    private FrontAudio frontAudio;
    private SharedAudioFocusV40 sharedAudio;
    private volatile long nativeAudioOwnerV42;
    private TextView status;
    private String[] assets=new String[0];
    private volatile int selected;
    private volatile boolean ready;
    private MovementControl movement;
    private Button attack;
    private volatile Button toolsToggle;
    private int toolsPlacement=-1;
    private GameplayHud gameplayHud;
    private View authoredControls;
    private FrameLayout viewportOverlay;
    private CharacterPanel characterPanel;
    private final java.util.Map<String,android.graphics.Bitmap> characterIcons=new java.util.HashMap<>();
    private long nextHudUpdate;
    private volatile String loadedAsset;
    private volatile String baseReport;
    private volatile String lastNativeUiError;
    private volatile boolean pendingActorCommand;
    private volatile boolean enemyAi=true;
    private volatile boolean developerOpen;
    private LinearLayout developerPanel;
    private TextView loadError;
    private IntroMovieV119 sourceIntroMovie;
    private BroadcastReceiver debugAttackReceiver;
    @Override public void onCreate(Bundle state) {
        setTheme(android.R.style.Theme_Material_NoActionBar);super.onCreate(state);
        sharedAudio=new SharedAudioFocusV40(this,NativeBridge::audioActivityV40);
        frontAudio=new FrontAudio(this,sharedAudio);
        developerOpen=state!=null?state.getBoolean("developerOpen",false):getIntent().getBooleanExtra("developer",false);
        boolean inspection=getIntent().hasExtra("texture")||getIntent().hasExtra("model")||getIntent().getBooleanExtra("original_hud",false);
        if(!inspection&&!getIntent().getBooleanExtra("developer",false))setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
        getWindow().addFlags(android.view.WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN|View.SYSTEM_UI_FLAG_HIDE_NAVIGATION|View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY|View.SYSTEM_UI_FLAG_LAYOUT_STABLE|View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN|View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION);
        enemyAi=state!=null?state.getBoolean("enemyAi",true):getIntent().getBooleanExtra("enemy_ai",true);
        LinearLayout layout=new LinearLayout(this);layout.setOrientation(LinearLayout.VERTICAL);
        layout.setBackgroundColor(Color.rgb(24,27,32));
        layout.setOnApplyWindowInsetsListener((view,insets)->{
            if(android.os.Build.VERSION.SDK_INT>=30){
                android.graphics.Insets bars=insets.getInsets(android.view.WindowInsets.Type.systemBars()|android.view.WindowInsets.Type.displayCutout());
                view.setPadding(bars.left,bars.top,bars.right,bars.bottom);
            }else view.setPadding(insets.getSystemWindowInsetLeft(),insets.getSystemWindowInsetTop(),insets.getSystemWindowInsetRight(),insets.getSystemWindowInsetBottom());
            return insets;
        });
        developerPanel=new LinearLayout(this);developerPanel.setOrientation(LinearLayout.VERTICAL);
        developerPanel.setPadding(12,8,12,8);developerPanel.setBackgroundColor(Color.argb(235,24,27,32));
        TextView title=new TextView(this);title.setText("Development tools");
        title.setTextColor(Color.WHITE);title.setTextSize(16);title.setPadding(8,8,8,4);developerPanel.addView(title);
        status=new TextView(this);status.setTextColor(Color.rgb(210,220,230));status.setPadding(16,8,16,8);
        status.setText(NativeBridge.buildInfo());
        try{
            String[] textures=getAssets().list("textures"),models=getAssets().list("models"),worlds=getAssets().list("worlds");
            Arrays.sort(textures);Arrays.sort(models);Arrays.sort(worlds);java.util.ArrayList<String> names=new java.util.ArrayList<>();
            for(String name:worlds)if(name.endsWith(".dwld"))names.add("worlds/"+name);
            for(String name:models)names.add("models/"+name);
            for(String name:textures)names.add("textures/"+name);
            names.add("ui/original-health-panel");names.add("ui/original-main-menu");assets=names.toArray(new String[0]);
        }
        catch(Exception e){Log.e("DH2Native","Asset listing failed",e);status.setText(e.toString());}
        String requested=state!=null?state.getString("asset"):null;
        pendingActorCommand=state!=null?state.getBoolean("pendingActorCommand",false):getIntent().getBooleanExtra("player_attack",false)||getIntent().getStringExtra("object_state")!=null;
        if(requested==null&&getIntent().getStringExtra("model")!=null)requested="models/"+getIntent().getStringExtra("model");
        if(requested==null&&getIntent().getStringExtra("texture")!=null)requested="textures/"+getIntent().getStringExtra("texture");
        if(requested==null&&getIntent().getStringExtra("world")!=null)requested="worlds/"+getIntent().getStringExtra("world");
        if(requested==null&&getIntent().getBooleanExtra("original_hud",false))requested="ui/original-health-panel";
        if(requested==null)requested="ui/original-main-menu";
        if(requested!=null){for(int i=0;i<assets.length;i++)if(assets[i].equals(requested))selected=i;}
        Spinner picker=new Spinner(this);
        ArrayAdapter<String> adapter=new ArrayAdapter<>(this,android.R.layout.simple_spinner_item,assets);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);picker.setAdapter(adapter);
        picker.setSelection(selected);
        developerPanel.addView(picker);developerPanel.addView(status);
        surface=new VsyncSurfaceViewV44(this);surface.setContentDescription("Dungeon Hunter 2 world and player HUD");surface.setEGLContextClientVersion(2);
        surface.setEGLConfigChooser(8,8,8,8,16,0);
        surface.setOnTouchListener(new View.OnTouchListener(){
            float x,y;
            @Override public boolean onTouch(View view,MotionEvent event){
                if("ui/original-main-menu".equals(loadedAsset)){
                    final int action=event.getActionMasked();
                    if(action==MotionEvent.ACTION_CANCEL)queueOriginalMenuPointer(3,-1,0,0);
                    else if(action==MotionEvent.ACTION_MOVE){for(int i=0;i<event.getPointerCount();i++)queueOriginalMenuPointer(1,event.getPointerId(i),event.getX(i),event.getY(i));}
                    else if(action==MotionEvent.ACTION_DOWN||action==MotionEvent.ACTION_POINTER_DOWN){int i=event.getActionIndex();queueOriginalMenuPointer(0,event.getPointerId(i),event.getX(i),event.getY(i));}
                    else if(action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_POINTER_UP){int i=event.getActionIndex();queueOriginalMenuPointer(2,event.getPointerId(i),event.getX(i),event.getY(i));}
                    if(action==MotionEvent.ACTION_UP)view.performClick();return true;
                }
                if(event.getActionMasked()==MotionEvent.ACTION_DOWN){x=event.getX();y=event.getY();return true;}
                if(event.getActionMasked()==MotionEvent.ACTION_MOVE){
                    if(assets[selected].startsWith("worlds/")||assets[selected].startsWith("ui/"))return true;
                    float dx=(event.getX()-x)*.008f,dy=(event.getY()-y)*.008f;x=event.getX();y=event.getY();
                    surface.queueEvent(()->NativeBridge.orbit(dx,dy,1));return true;
                }
                if(event.getActionMasked()==MotionEvent.ACTION_UP){view.performClick();return true;}return true;
            }
        });
        surface.setRenderer(new GLSurfaceView.Renderer(){
            private boolean initialLoadPending;
            private boolean lastNativeAudioReady;
            @Override public void onSurfaceCreated(GL10 gl,EGLConfig config){
                if(framePacer!=null)framePacer.onRendererContextCreated();
                FrameBoundaryV42.configure((getApplicationInfo().flags&ApplicationInfo.FLAG_DEBUGGABLE)!=0&&getIntent().getBooleanExtra("frame_trace",false));
                nativeAudioOwnerV42=NativeBridge.audioApplicationReserveV42();
                final String initialization=NativeBridge.initialize(getAssets());
                Log.i("DH2Native",initialization);loadedAsset=null;ready=false;initialLoadPending=false;
                if(initialization==null||initialization.startsWith("Authored UI initialization failed:")){
                    //A cancelled/retiring audio incarnation did not complete
                    //native UI setup. Do not load assets against that partial
                    //generation on resize; the next actual surface initialization
                    //retries after the serial-guarded producer barrier.
                    baseReport=initialization==null?"Native initialization returned no result":initialization;
                    show(baseReport);return;
                }
                initialLoadPending=true;
                NativeBridge.debugCastProbe((getApplicationInfo().flags&ApplicationInfo.FLAG_DEBUGGABLE)!=0&&getIntent().getBooleanExtra("cast_trace",false));
                NativeBridge.configureFrontDevice(android.os.Build.MANUFACTURER,android.os.Build.MODEL);
                NativeBridge.characterPanelOpen(false);
                NativeBridge.enemyAi(enemyAi);
            }
            @Override public void onSurfaceChanged(GL10 gl,int w,int h){
                NativeBridge.resize(w,h);
                // Camera construction requires the real resized surface. This
                // also handles context recreation without replaying each resize.
                if(initialLoadPending&&w>1&&h>1){
                    initialLoadPending=false;ready=true;
                    if(assets.length>0)loadSelected();else show("No bundled asset fixtures");
                }
                if(framePacer!=null)framePacer.onRendererSurfaceReady(w,h);
            }
            @Override public void onDrawFrame(GL10 gl){
                final long pacingTicket=framePacer==null?0:framePacer.frameStarted();
                long frameTicket=0;
                try {
                frameTicket=FrameBoundaryV42.beginFrame();
                FrameBoundaryV42.beginNative(frameTicket);
                try { NativeBridge.draw(); }
                finally { FrameBoundaryV42.endNative(frameTicket); }
                final boolean nativeAudioReady=NativeBridge.audioSourceReadyV40();
                if(nativeAudioReady!=lastNativeAudioReady){lastNativeAudioReady=nativeAudioReady;runOnUiThread(()->sharedAudio.setNativeDemand(nativeAudioReady));}
                String audio;
                while((audio=NativeBridge.consumeOriginalMenuAudio())!=null){final String control=audio;runOnUiThread(()->frontAudio.control(control));}
                while((audio=NativeBridge.consumeOriginalMenuSound())!=null){final String sound=audio;runOnUiThread(()->frontAudio.effect(sound));}
                String launch=NativeBridge.consumeFrontLaunch();
                if(launch!=null){
                    final boolean active=NativeBridge.sourceCampaignSceneActive();
                    // Routing identifiers, not a claim about a loaded bundled map URI.
                    loadedAsset=active?"worlds/source-campaign":"ui/source-campaign-loading";
                    baseReport=launch;Log.i("DH2Native",launch);
                    lastNativeUiError=null;
                    runOnUiThread(()->{frontAudio.stop();authoredControls.setVisibility(active?View.VISIBLE:View.GONE);movement.setVisibility(View.GONE);attack.setVisibility(View.GONE);gameplayHud.setVisibility(View.GONE);show(launch);});
                }
                long now=android.os.SystemClock.uptimeMillis();
                int placement=NativeBridge.authoredCharacterMenuIsOpen()?2:
                    (loadedAsset!=null&&loadedAsset.startsWith("worlds/")?1:0);
                if(toolsToggle!=null&&placement!=toolsPlacement){
                    toolsPlacement=placement;
                    final int mode=placement;
                    runOnUiThread(()->{
                        toolsToggle.setVisibility(mode==2?View.GONE:View.VISIBLE);
                        FrameLayout.LayoutParams params=(FrameLayout.LayoutParams)toolsToggle.getLayoutParams();
                        params.gravity=Gravity.TOP|(mode==1?Gravity.CENTER_HORIZONTAL:Gravity.RIGHT);
                        toolsToggle.setLayoutParams(params);
                    });
                }
                if(gameplayHud!=null&&gameplayHud.getVisibility()==View.VISIBLE&&loadedAsset!=null&&loadedAsset.startsWith("worlds/")&&now>=nextHudUpdate){
                    nextHudUpdate=now+100;
                    int[] snapshot=NativeBridge.playerGameplayHud();
                    updateGameplayIcons();
                    runOnUiThread(()->gameplayHud.update(snapshot));
                }
                String error=NativeBridge.consumeOriginalUiError();
                if(error!=null&&!error.equals(lastNativeUiError)){lastNativeUiError=error;Log.e("DH2Native",error);showLoadError(error);}
                } finally {
                    try { FrameBoundaryV42.endFrame(frameTicket); }
                    finally { if(framePacer!=null)framePacer.frameFinished(pacingTicket); }
                }
            }
        });
        final boolean debugPacing=(getApplicationInfo().flags&ApplicationInfo.FLAG_DEBUGGABLE)!=0&&"vsync60".equals(getIntent().getStringExtra("frame_pacing"));
        if(debugPacing)framePacer=surface.enableVsync60();
        else {
            surface.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
            if(getIntent().hasExtra("frame_pacing"))Log.i("DH2Pacing","V44 opt-in refused or unknown; continuous default retained");
        }
        FrameLayout viewport=new FrameLayout(this);viewport.addView(surface,new FrameLayout.LayoutParams(-1,-1));
        viewportOverlay=viewport;
        sourceIntroMovie=new IntroMovieV119(this,viewport);
        String introBindingError=NativeBridge.bindIntroMovieV119(this,getAssets());
        if(introBindingError!=null)Log.e("DH2Native",introBindingError);
        loadError=new TextView(this);loadError.setTextColor(Color.WHITE);loadError.setBackgroundColor(Color.argb(230,85,25,25));loadError.setPadding(20,12,20,12);loadError.setVisibility(View.GONE);
        viewport.addView(loadError,new FrameLayout.LayoutParams(-2,-2,Gravity.TOP|Gravity.CENTER_HORIZONTAL));
        movement=new MovementControl();movement.setVisibility(View.GONE);
        int padSize=(int)(160*getResources().getDisplayMetrics().density);
        FrameLayout.LayoutParams padLayout=new FrameLayout.LayoutParams(padSize,padSize,Gravity.BOTTOM|Gravity.LEFT);
        padLayout.leftMargin=padLayout.bottomMargin=(int)(16*getResources().getDisplayMetrics().density);viewport.addView(movement,padLayout);
        attack=new Button(this);attack.setText("Attack");attack.setContentDescription("Attack nearby enemy");attack.setVisibility(View.GONE);
        FrameLayout.LayoutParams attackLayout=new FrameLayout.LayoutParams((int)(112*getResources().getDisplayMetrics().density),(int)(64*getResources().getDisplayMetrics().density),Gravity.BOTTOM|Gravity.RIGHT);attackLayout.rightMargin=attackLayout.bottomMargin=(int)(24*getResources().getDisplayMetrics().density);viewport.addView(attack,attackLayout);
        attack.setOnClickListener(v->surface.queueEvent(()->{String report=NativeBridge.playerAttack(-1);Log.i("DH2Native","Player input | "+report);show(baseReport+"\n"+report);}));
        gameplayHud=new GameplayHud(this,(operation,index)->surface.queueEvent(()->{
            String report=NativeBridge.playerGameplayAction(operation,index);
            Log.i("DH2Native","Gameplay HUD action | operation "+operation+" | index "+index+" | "+report);
            int[] snapshot=NativeBridge.playerGameplayHud();
            Log.i("DH2Native","Gameplay HUD snapshot | "+Arrays.toString(snapshot));
            runOnUiThread(()->{gameplayHud.update(snapshot);gameplayHud.result(report);});
        }));
        gameplayHud.setVisibility(View.GONE);
        FrameLayout.LayoutParams hudLayout=new FrameLayout.LayoutParams((int)(360*getResources().getDisplayMetrics().density),(int)(102*getResources().getDisplayMetrics().density),Gravity.BOTTOM|Gravity.CENTER_HORIZONTAL);
        hudLayout.bottomMargin=(int)(8*getResources().getDisplayMetrics().density);
        viewport.addView(gameplayHud,hudLayout);
        authoredControls=new View(this);authoredControls.setContentDescription("Original gameplay HUD touch controls");authoredControls.setVisibility(View.GONE);
        authoredControls.setOnTouchListener((view,event)->{
            int action=event.getActionMasked(),index=event.getActionIndex();
            if(action==MotionEvent.ACTION_CANCEL){queueHudPointer(3,-1,0,0);return true;}
            if(action==MotionEvent.ACTION_MOVE){for(int i=0;i<event.getPointerCount();i++)queueHudPointer(1,event.getPointerId(i),event.getX(i),event.getY(i));return true;}
            if(action==MotionEvent.ACTION_DOWN||action==MotionEvent.ACTION_POINTER_DOWN)queueHudPointer(0,event.getPointerId(index),event.getX(index),event.getY(index));
            else if(action==MotionEvent.ACTION_UP||action==MotionEvent.ACTION_POINTER_UP){queueHudPointer(2,event.getPointerId(index),event.getX(index),event.getY(index));view.performClick();}
            return true;
        });
        viewport.addView(authoredControls,new FrameLayout.LayoutParams(-1,-1));
        Button character=new Button(this);character.setText("Character development tools");character.setAllCaps(false);character.setContentDescription("Open character development fallback");
        developerPanel.addView(character);
        character.setOnClickListener(v->showCharacterPanel());
        ScrollView tools=new ScrollView(this);tools.addView(developerPanel);tools.setVisibility(developerOpen?View.VISIBLE:View.GONE);
        int toolsWidth=(int)(340*getResources().getDisplayMetrics().density);
        viewport.addView(tools,new FrameLayout.LayoutParams(toolsWidth,-1,Gravity.RIGHT));
        toolsToggle=new Button(this);toolsToggle.setText("Dev");toolsToggle.setContentDescription("Open development tools");
        FrameLayout.LayoutParams toolsButton=new FrameLayout.LayoutParams((int)(64*getResources().getDisplayMetrics().density),(int)(40*getResources().getDisplayMetrics().density),Gravity.TOP|Gravity.RIGHT);
        viewport.addView(toolsToggle,toolsButton);
        toolsToggle.setOnClickListener(v->{developerOpen=!developerOpen;tools.setVisibility(developerOpen?View.VISIBLE:View.GONE);toolsToggle.setText(developerOpen?"Close":"Dev");});
        layout.addView(viewport,new LinearLayout.LayoutParams(-1,0,1));
        picker.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener(){
            @Override public void onItemSelected(AdapterView<?> p,View v,int position,long id){selected=position;if(ready)surface.queueEvent(MainActivity.this::loadSelected);}
            @Override public void onNothingSelected(AdapterView<?> p){}
        });
        setContentView(layout);
        // Debug-only shell command invokes the same playerAttack entry point
        // as the button without am start pausing the Activity/held joystick.
        // DUMP is a system/shell permission, not granted to ordinary apps.
        if((getApplicationInfo().flags&ApplicationInfo.FLAG_DEBUGGABLE)!=0){
            debugAttackReceiver=new BroadcastReceiver(){
                @Override public void onReceive(Context context,Intent intent){
                    if(!ready||loadedAsset==null||!loadedAsset.startsWith("worlds/"))return;
                    if("com.example.dh2.DEBUG_FRAME_REPORT".equals(intent.getAction())){
                        final String reportRequest=intent.getStringExtra("request_id");
                        surface.queueEvent(()->{
                            try {
                                FrameBoundaryV42.finish();
                                org.json.JSONObject report=new org.json.JSONObject();
                                report.put("request_id",reportRequest==null?"":reportRequest);
                                report.put("pacing",new org.json.JSONObject(FramePacingReportV46.capture(framePacer)));
                                report.put("pid",android.os.Process.myPid());
                                report.put("captured_uptime_ms",android.os.SystemClock.uptimeMillis());
                                report.put("boundary",new org.json.JSONObject(FrameBoundaryV42.report()));
                                report.put("capabilities",new org.json.JSONObject(FrameBoundaryV42.discoverPresentCapabilities()));
                                report.put("guestThread",new org.json.JSONObject(FrameBoundaryV42.sampleGuestThreadCounters()));
                                report.put("resources",new org.json.JSONObject(NativeBridge.resourceBudgetReport()));
                                final byte[] bytes=report.toString().getBytes(java.nio.charset.StandardCharsets.UTF_8);
                                new Thread(()->{
                                    try(java.io.FileOutputStream output=openFileOutput("frame-boundary-v42.json",MODE_PRIVATE)){
                                        output.write(bytes);Log.i("DH2Perf","Bounded frame report saved: "+bytes.length+" bytes");
                                    }catch(Exception e){Log.e("DH2Perf","Frame report persistence failed",e);}
                                },"DH2FrameReport").start();
                            }catch(Exception e){Log.e("DH2Perf","Frame report failed",e);}
                        });
                        return;
                    }
                    if("com.example.dh2.DEBUG_EQUIPMENT".equals(intent.getAction())){
                        final int operation=intent.getIntExtra("operation",-1),index=intent.getIntExtra("index",-1),slot=intent.getIntExtra("slot",-1);
                        surface.queueEvent(()->{String report=NativeBridge.playerEquipmentAction(operation,index,slot);Log.i("DH2Native","Equipment command applied | "+report);show(baseReport+"\n"+report);requestFrameV44();});
                        return;
                    }
                    if("com.example.dh2.DEBUG_ANIMATION_TIME".equals(intent.getAction())){
                        final int time=intent.getIntExtra("time_ms",-1);
                        surface.queueEvent(()->{NativeBridge.animationTime(time);Log.i("DH2Native","Animation time command applied | time "+time);requestFrameV44();});
                        return;
                    }
                    final int target=intent.getIntExtra("player_target_index",-1);
                    surface.queueEvent(()->{String report=NativeBridge.playerAttack(target);Log.i("DH2Native","Player command applied | "+report);show(baseReport+"\n"+report);});
                }
            };
            IntentFilter filter=new IntentFilter("com.example.dh2.DEBUG_PLAYER_ATTACK");
            filter.addAction("com.example.dh2.DEBUG_ANIMATION_TIME");
            filter.addAction("com.example.dh2.DEBUG_EQUIPMENT");
            filter.addAction("com.example.dh2.DEBUG_FRAME_REPORT");
            if(Build.VERSION.SDK_INT>=33)registerReceiver(debugAttackReceiver,filter,"android.permission.DUMP",null,Context.RECEIVER_EXPORTED);
            else registerReceiver(debugAttackReceiver,filter,"android.permission.DUMP",null);
        }
    }
    private void updateGameplayIcons(){
        String[] names=NativeBridge.playerGameplayIcons();
        if(names==null||names.length!=5||!gameplayHud.needsIcons(names))return;
        android.graphics.Bitmap[] bitmaps=new android.graphics.Bitmap[5];
        for(int i=0;i<5;i++)if(names[i]!=null&&!names[i].isEmpty()){
            int[] pixels=NativeBridge.menuIcon(names[i]);
            if(pixels!=null&&pixels.length>=2&&pixels[0]>0&&pixels[1]>0&&pixels.length==(long)pixels[0]*pixels[1]+2)
                bitmaps[i]=android.graphics.Bitmap.createBitmap(pixels,2,pixels[0],pixels[0],pixels[1],android.graphics.Bitmap.Config.ARGB_8888);
        }
        runOnUiThread(()->gameplayHud.icons(names,bitmaps));
    }
    private void queueHudPointer(int phase,int pointer,float x,float y){
        if(!ready)return;
        surface.queueEvent(()->{
            String command=NativeBridge.authoredHudTouch(phase,pointer,x,y);
            if(command==null)return;
            Log.i("DH2Native","Authored HUD command | "+command);
            if(command.equals("pause"))show(baseReport+"\nOriginal in-game pause menu connection is pending.");
            else show(baseReport+"\n"+command);
        });
    }
    private void showCharacterPanel(){
        if(characterPanel!=null||!ready||loadedAsset==null||!loadedAsset.startsWith("worlds/"))return;
        movement.stop();gameplayHud.cancelPress();
        queueHudPointer(3,-1,0,0);authoredControls.setVisibility(View.GONE);
        surface.queueEvent(()->NativeBridge.characterPanelOpen(true));
        movement.setVisibility(View.GONE);attack.setVisibility(View.GONE);gameplayHud.setVisibility(View.GONE);
        characterPanel=new CharacterPanel(this,new CharacterPanel.Host(){
            @Override public void refresh(CharacterPanel panel){surface.queueEvent(()->publishCharacterPanel(panel,NativeBridge.playerCharacterSnapshot(),null));}
            @Override public void action(CharacterPanel panel,int operation,int index,int slot){surface.queueEvent(()->{String report=NativeBridge.playerCharacterAction(operation,index,slot);String snapshot=NativeBridge.playerCharacterSnapshot();Log.i("DH2Native","Character action | "+operation+" | "+index+" | "+slot+" | "+report);publishCharacterPanel(panel,snapshot,report);});}
            @Override public void close(){closeCharacterPanel();}
        });
        viewportOverlay.addView(characterPanel,new FrameLayout.LayoutParams(-1,-1));
        final CharacterPanel panel=characterPanel;
        surface.queueEvent(()->{String snapshot=NativeBridge.playerCharacterSnapshot();Log.i("DH2Native","Character snapshot | "+snapshot);publishCharacterPanel(panel,snapshot,null);});
    }
    // Invoked only on the GL thread, where the native texture decoder is owned.
    private void publishCharacterPanel(CharacterPanel panel,String snapshot,String report){
        try{
            org.json.JSONObject object=new org.json.JSONObject(snapshot);
            org.json.JSONObject evidence=new org.json.JSONObject();evidence.put("ready",object.optBoolean("ready"));evidence.put("stats",object.optJSONObject("stats"));evidence.put("skillPoints",object.optInt("skillPoints"));
            String[][] fields={{"Index","ItemID","Quantity","EquippedSlot"},{"Index","SkillLevel","EquippedSlot"},{"Index","State","Level","Selected"},{"Index","Name","ItemIndex"}};
            String[] collections={"items","skills","faeries","equipmentSlots"},keys={"ItemIcon","SkillIcon","Icon","Icon"};
            for(int group=0;group<collections.length;group++){
                org.json.JSONArray rows=object.optJSONArray(collections[group]);if(rows==null)continue;
                org.json.JSONArray compact=new org.json.JSONArray();
                for(int i=0;i<rows.length();i++){
                    org.json.JSONObject row=rows.optJSONObject(i);if(row==null)continue;String name=row.optString(keys[group]);
                    org.json.JSONObject values=new org.json.JSONObject();for(String key:fields[group])values.put(key,row.opt(key));compact.put(values);
                    if(name.isEmpty()||characterIcons.containsKey(name))continue;
                    int[] pixels=NativeBridge.menuIcon(name);android.graphics.Bitmap bitmap=null;
                    if(pixels!=null&&pixels.length>=2&&pixels[0]>0&&pixels[1]>0&&pixels.length==(long)pixels[0]*pixels[1]+2)
                        bitmap=android.graphics.Bitmap.createBitmap(pixels,2,pixels[0],pixels[0],pixels[1],android.graphics.Bitmap.Config.ARGB_8888);
                    characterIcons.put(name,bitmap);
                }
                evidence.put(collections[group],compact);
            }
            org.json.JSONArray inventory=object.optJSONArray("items");
            if(inventory!=null)for(int i=0;i<inventory.length();i++){
                org.json.JSONObject item=inventory.optJSONObject(i);if(item==null)continue;
                String name=item.optString("ItemCategoryIcon");if(name.isEmpty()||characterIcons.containsKey(name))continue;
                int[] pixels=NativeBridge.menuIcon(name);android.graphics.Bitmap bitmap=null;
                if(pixels!=null&&pixels.length>=2&&pixels[0]>0&&pixels[1]>0&&pixels.length==(long)pixels[0]*pixels[1]+2)
                    bitmap=android.graphics.Bitmap.createBitmap(pixels,2,pixels[0],pixels[0],pixels[1],android.graphics.Bitmap.Config.ARGB_8888);
                characterIcons.put(name,bitmap);
            }
            for(String name:new String[]{"CharacterSheet","Inventory","Skills","Faery","MenuBack","MenuCompare"}){
                if(characterIcons.containsKey(name))continue;
                int[] pixels=NativeBridge.menuIcon(name);android.graphics.Bitmap bitmap=null;
                if(pixels!=null&&pixels.length>=2&&pixels[0]>0&&pixels[1]>0&&pixels.length==(long)pixels[0]*pixels[1]+2)
                    bitmap=android.graphics.Bitmap.createBitmap(pixels,2,pixels[0],pixels[0],pixels[1],android.graphics.Bitmap.Config.ARGB_8888);
                characterIcons.put(name,bitmap);
            }
            Log.i("DH2Native","Character snapshot state | "+evidence);
        }catch(org.json.JSONException error){Log.w("DH2Native","Character icon metadata failed",error);}
        java.util.Map<String,android.graphics.Bitmap> images=new java.util.HashMap<>(characterIcons);
        runOnUiThread(()->{if(characterPanel==panel)panel.publish(snapshot,images,report);});
    }
    private void closeCharacterPanel(){
        if(characterPanel==null)return;viewportOverlay.removeView(characterPanel);characterPanel=null;
        surface.queueEvent(()->NativeBridge.characterPanelOpen(false));
        boolean world=loadedAsset!=null&&loadedAsset.startsWith("worlds/");movement.setVisibility(View.GONE);attack.setVisibility(View.GONE);gameplayHud.setVisibility(View.GONE);authoredControls.setVisibility(world?View.VISIBLE:View.GONE);
    }
    @Override public void onBackPressed(){
        if(sourceIntroMovie!=null&&sourceIntroMovie.visible()){sourceIntroMovie.skip();return;}
        if(characterPanel!=null){closeCharacterPanel();return;}
        if(!ready){super.onBackPressed();return;}
        surface.queueEvent(()->{if(!NativeBridge.authoredCharacterMenuBack())runOnUiThread(()->MainActivity.super.onBackPressed());});
    }
    private void show(String text){runOnUiThread(()->{
        status.setText(text);
        boolean failed=text.contains("failed")||text.contains("error")||text.contains("Exception");
        if(!failed)lastNativeUiError=null;
        loadError.setText(failed?text:"");loadError.setVisibility(failed?View.VISIBLE:View.GONE);
    });}
    private void showLoadError(String text){runOnUiThread(()->{
        status.setText(text);loadError.setText(text);loadError.setVisibility(View.VISIBLE);
    });}
    public void startSourceIntroV119(long generation,int language,String filename){
        runOnUiThread(()->{if(sourceIntroMovie==null||isFinishing()){NativeBridge.introMovieEventV119(generation,3,"Intro has no live Activity viewport");return;}frontAudio.stop();sourceIntroMovie.show(generation,filename);});
    }
    private void queueOriginalMenuPointer(int phase,int pointer,float x,float y){
        surface.queueEvent(()->{String error=NativeBridge.originalMenuPointer(phase,pointer,x,y);if(error!=null){Log.e("DH2Native",error);show(error);}});
    }
    public int platformLanguageV119(){
        String language=java.util.Locale.getDefault().getLanguage();
        if(language.equals("en"))return 0;if(language.equals("de"))return 1;if(language.equals("fr"))return 2;if(language.equals("es"))return 3;
        if(language.equals("it"))return 4;if(language.equals("ja"))return 5;if(language.equals("ko"))return 6;if(language.equals("ru"))return 7;
        return 8; //Unmapped platform code reaches original Settings.Load fallback.
    }
    @Override protected void onNewIntent(Intent intent){
        super.onNewIntent(intent);setIntent(intent);
        if(intent.hasExtra("enemy_ai"))enemyAi=intent.getBooleanExtra("enemy_ai",true);
        pendingActorCommand=true;
        if(!ready||loadedAsset==null||!loadedAsset.startsWith("worlds/"))return;
        final int index=intent.getIntExtra("object_index",-1),time=intent.getIntExtra("time_ms",-1),target=intent.getIntExtra("combat_target_index",-1);
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
            show(baseReport+"\n"+report);requestFrameV44();
        });
    }
    private void requestFrameV44(){
        final FramePacingControllerV44 pacing=framePacer;
        if(pacing!=null)pacing.requestUpdate();else surface.requestRender();
    }
    private void loadSelected(){
        String name=assets[selected];
        if(name.equals(loadedAsset))return;
        runOnUiThread(()->{closeCharacterPanel();movement.setVisibility(View.GONE);attack.setVisibility(View.GONE);gameplayHud.update(null);gameplayHud.setVisibility(View.GONE);authoredControls.setVisibility(name.startsWith("worlds/")?View.VISIBLE:View.GONE);});
        if(name.equals("ui/original-main-menu")){
            String report=NativeBridge.loadOriginalFrontScreen(getFilesDir().getAbsolutePath(),"main");
            if(!report.contains("failed"))loadedAsset=name;
            baseReport=report;Log.i("DH2Native",report);show(report);requestFrameV44();return;
        }
        runOnUiThread(()->frontAudio.stop());
        if(name.equals("ui/original-health-panel")){
            String report=NativeBridge.loadOriginalHealthPanel(getFilesDir().getAbsolutePath());
            if(!report.contains("failed"))loadedAsset=name;
            baseReport=name+"\n"+report;Log.i("DH2Native",baseReport);show(baseReport);requestFrameV44();return;
        }
        try(InputStream input=getAssets().open(name);ByteArrayOutputStream out=new ByteArrayOutputStream()){
            byte[] block=new byte[8192];int n;
            while((n=input.read(block))!=-1){if(out.size()+n>32*1024*1024)throw new java.io.IOException("Texture too large");out.write(block,0,n);}
            String selectedMlx="";
            if(name.startsWith("worlds/")){
                final String provenance=name.substring(0,name.length()-5)+"-provenance.json";
                try(InputStream metadata=getAssets().open(provenance);ByteArrayOutputStream metadataBytes=new ByteArrayOutputStream()){
                    byte[] metadataBlock=new byte[4096];int metadataCount;
                    while((metadataCount=metadata.read(metadataBlock))!=-1){if(metadataBytes.size()+metadataCount>1024*1024)throw new java.io.IOException("World provenance too large");metadataBytes.write(metadataBlock,0,metadataCount);}
                    selectedMlx="data/scene/"+new org.json.JSONObject(metadataBytes.toString("UTF-8")).getString("layout");
                }
            }
            String report=name.startsWith("worlds/")?NativeBridge.loadWorld(out.toByteArray(),getAssets(),getFilesDir().getAbsolutePath(),selectedMlx):name.startsWith("models/")?NativeBridge.loadModel(out.toByteArray(),getAssets()):NativeBridge.loadTexture(out.toByteArray());
            if(!report.contains("failed")&&!report.contains("error"))loadedAsset=name;
            baseReport=name+"\n"+report;
            if(name.startsWith("models/")||name.startsWith("worlds/"))NativeBridge.animationTime(getIntent().getIntExtra("time_ms",-1));
            if(name.startsWith("worlds/"))NativeBridge.focusObject(getIntent().getIntExtra("object_index",-1));
            if(name.startsWith("worlds/")&&pendingActorCommand&&getIntent().getBooleanExtra("player_attack",false)){
                String attackReport=NativeBridge.playerAttack(getIntent().getIntExtra("player_target_index",-1));report+="\n"+attackReport;
                Log.i("DH2Native","Player command applied | "+attackReport);pendingActorCommand=false;
            }
            // An intent is a command, not a desired state to replay every time
            // Android recreates the GL surface. The native actor owns its state.
            if(name.startsWith("worlds/")&&pendingActorCommand&&getIntent().getStringExtra("object_state")!=null){
                int index=getIntent().getIntExtra("object_index",-1);
                String targetReport=NativeBridge.combatTarget(index,getIntent().getIntExtra("combat_target_index",-1));
                report+="\n"+(targetReport.equals("Combat target selected")||targetReport.equals("Combat target cleared")?NativeBridge.objectState(index,getIntent().getStringExtra("object_state")):targetReport);
            }
            Log.i("DH2Native",name+": "+report);
            if(name.startsWith("worlds/")&&pendingActorCommand){
                Log.i("DH2Native","Actor command applied | index "+getIntent().getIntExtra("object_index",-1)+" | state "+getIntent().getStringExtra("object_state")+" | time "+getIntent().getIntExtra("time_ms",-1)+" | "+report);
                pendingActorCommand=false;
            }
            show(name+"\n"+report);requestFrameV44();
        }catch(Exception e){Log.e("DH2Native","Asset load failed: "+name,e);show(name+"\n"+e);}
    }
    private final class MovementControl extends View {
        private final Paint paint=new Paint(Paint.ANTI_ALIAS_FLAG);private float axisX,axisY;
        MovementControl(){super(MainActivity.this);setContentDescription("Movement control");setFocusable(true);}
        @Override protected void onDraw(Canvas canvas){
            float radius=getWidth()*.44f,cx=getWidth()*.5f,cy=getHeight()*.5f;
            paint.setColor(Color.argb(140,55,65,75));canvas.drawCircle(cx,cy,radius,paint);
            paint.setStyle(Paint.Style.STROKE);paint.setStrokeWidth(3);paint.setColor(Color.argb(210,160,180,200));canvas.drawCircle(cx,cy,radius,paint);paint.setStyle(Paint.Style.FILL);
            paint.setColor(Color.argb(220,175,190,205));canvas.drawCircle(cx+axisX*radius*.65f,cy+axisY*radius*.65f,radius*.26f,paint);
        }
        @Override public boolean onTouchEvent(MotionEvent event){
            if(event.getActionMasked()==MotionEvent.ACTION_DOWN||event.getActionMasked()==MotionEvent.ACTION_MOVE){
                axisX=(event.getX()-getWidth()*.5f)/(getWidth()*.44f);axisY=(event.getY()-getHeight()*.5f)/(getWidth()*.44f);
                float length=(float)Math.hypot(axisX,axisY);if(length>1){axisX/=length;axisY/=length;}
            }else if(event.getActionMasked()==MotionEvent.ACTION_UP||event.getActionMasked()==MotionEvent.ACTION_CANCEL){axisX=axisY=0;performClick();}
            float x=axisX,y=-axisY;surface.queueEvent(()->NativeBridge.moveAxis(x,y));invalidate();return true;
        }
        @Override public boolean performClick(){super.performClick();return true;}
        void stop(){axisX=axisY=0;invalidate();surface.queueEvent(()->NativeBridge.moveAxis(0,0));}
    }
    @Override protected void onPause(){if(sourceIntroMovie!=null)sourceIntroMovie.pause();sharedAudio.setResumed(false);if(isFinishing()||isChangingConfigurations())AudioApplicationLifecycleV42.closeBeforeProducerPause(surface,nativeAudioOwnerV42);if(framePacer!=null)framePacer.onPause();super.onPause();queueHudPointer(3,-1,0,0);if("ui/original-main-menu".equals(loadedAsset))surface.queueEvent(()->NativeBridge.originalMenuTouch(0,0,3));ready=false;frontAudio.pause();movement.stop();gameplayHud.cancelPress();surface.onPause();}
    @Override protected void onResume(){super.onResume();if(sourceIntroMovie!=null)sourceIntroMovie.resume();sharedAudio.setResumed(true);frontAudio.resume();surface.onResume();if(framePacer!=null)framePacer.onResume();surface.queueEvent(()->{if(loadedAsset!=null)ready=true;});}
    @Override public void onWindowFocusChanged(boolean focused){super.onWindowFocusChanged(focused);if(sharedAudio!=null)sharedAudio.setWindowFocused(focused);}
    @Override protected void onDestroy(){if(sourceIntroMovie!=null)sourceIntroMovie.destroy();NativeBridge.closeIntroMovieV119();sharedAudio.close();AudioApplicationLifecycleV42.destroyRequestOnly(nativeAudioOwnerV42);if(surface!=null)surface.destroyPacingV44();if(debugAttackReceiver!=null)unregisterReceiver(debugAttackReceiver);frontAudio.stop();super.onDestroy();}
    @Override protected void onSaveInstanceState(Bundle state){super.onSaveInstanceState(state);if(assets.length>0)state.putString("asset",assets[selected]);state.putBoolean("pendingActorCommand",pendingActorCommand);state.putBoolean("enemyAi",enemyAi);state.putBoolean("developerOpen",developerOpen);}
}
