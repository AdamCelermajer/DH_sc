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
    private GLSurfaceView surface;
    private TextView status;
    private String[] assets=new String[0];
    private volatile int selected;
    private volatile boolean ready;
    private MovementControl movement;
    private Button attack;
    private TextView vitals;
    private volatile String loadedAsset;
    private volatile String baseReport;
    private volatile boolean pendingActorCommand;
    private volatile boolean enemyAi=true;
    private BroadcastReceiver debugAttackReceiver;
    @Override public void onCreate(Bundle state) {
        setTheme(android.R.style.Theme_Material_NoActionBar);super.onCreate(state);
        enemyAi=state!=null?state.getBoolean("enemyAi",true):getIntent().getBooleanExtra("enemy_ai",true);
        LinearLayout layout=new LinearLayout(this);layout.setOrientation(LinearLayout.VERTICAL);
        layout.setBackgroundColor(Color.rgb(24,27,32));
        layout.setOnApplyWindowInsetsListener((view,insets)->{
            if(android.os.Build.VERSION.SDK_INT>=30){
                android.graphics.Insets bars=insets.getInsets(android.view.WindowInsets.Type.systemBars());
                view.setPadding(bars.left,bars.top,bars.right,bars.bottom);
            }else view.setPadding(insets.getSystemWindowInsetLeft(),insets.getSystemWindowInsetTop(),insets.getSystemWindowInsetRight(),insets.getSystemWindowInsetBottom());
            return insets;
        });
        TextView title=new TextView(this);title.setText("Dungeon Hunter 2 — Native 3D source");
        title.setTextColor(Color.WHITE);title.setTextSize(18);title.setPadding(16,12,16,8);layout.addView(title);
        status=new TextView(this);status.setTextColor(Color.rgb(210,220,230));status.setPadding(16,8,16,8);
        status.setText(NativeBridge.buildInfo());
        try{
            String[] textures=getAssets().list("textures"),models=getAssets().list("models"),worlds=getAssets().list("worlds");
            Arrays.sort(textures);Arrays.sort(models);java.util.ArrayList<String> names=new java.util.ArrayList<>();
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
        surface=new GLSurfaceView(this);surface.setContentDescription("DH2 native texture viewport");surface.setEGLContextClientVersion(2);
        surface.setEGLConfigChooser(8,8,8,8,16,0);
        surface.setOnTouchListener(new View.OnTouchListener(){
            float x,y;
            @Override public boolean onTouch(View view,MotionEvent event){
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
            @Override public void onSurfaceCreated(GL10 gl,EGLConfig config){
                Log.i("DH2Native",NativeBridge.initialize());loadedAsset=null;ready=true;
                NativeBridge.enemyAi(enemyAi);
                if(assets.length>0)loadSelected();else show("No bundled asset fixtures");
            }
            @Override public void onSurfaceChanged(GL10 gl,int w,int h){NativeBridge.resize(w,h);}
            @Override public void onDrawFrame(GL10 gl){NativeBridge.draw();long now=android.os.SystemClock.uptimeMillis();if(now-lastVitals>=200&&loadedAsset!=null&&loadedAsset.startsWith("worlds/")){lastVitals=now;int[] values=NativeBridge.playerVitals();runOnUiThread(()->{vitals.setText(String.format(java.util.Locale.ROOT,"HP %.1f / %.1f   MP %.1f / %.1f%s",values[0]/256f,values[1]/256f,values[2]/256f,values[3]/256f,values[4]!=0?"   Defeated":""));vitals.setTextColor(values[4]!=0||values[5]==0?Color.rgb(255,150,150):Color.WHITE);});}}
        });
        surface.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        FrameLayout viewport=new FrameLayout(this);viewport.addView(surface,new FrameLayout.LayoutParams(-1,-1));
        vitals=new TextView(this);vitals.setContentDescription("Player health and mana");vitals.setTextSize(14);vitals.setTextColor(Color.WHITE);vitals.setPadding(12,8,12,8);vitals.setBackgroundColor(Color.argb(160,15,20,25));vitals.setVisibility(View.GONE);viewport.addView(vitals,new FrameLayout.LayoutParams(-2,-2,Gravity.TOP|Gravity.LEFT));
        movement=new MovementControl();movement.setVisibility(View.GONE);
        int padSize=(int)(160*getResources().getDisplayMetrics().density);
        FrameLayout.LayoutParams padLayout=new FrameLayout.LayoutParams(padSize,padSize,Gravity.BOTTOM|Gravity.LEFT);
        padLayout.leftMargin=padLayout.bottomMargin=(int)(16*getResources().getDisplayMetrics().density);viewport.addView(movement,padLayout);
        attack=new Button(this);attack.setText("Attack");attack.setContentDescription("Attack nearby enemy");attack.setVisibility(View.GONE);
        FrameLayout.LayoutParams attackLayout=new FrameLayout.LayoutParams((int)(112*getResources().getDisplayMetrics().density),(int)(64*getResources().getDisplayMetrics().density),Gravity.BOTTOM|Gravity.RIGHT);attackLayout.rightMargin=attackLayout.bottomMargin=(int)(24*getResources().getDisplayMetrics().density);viewport.addView(attack,attackLayout);
        attack.setOnClickListener(v->surface.queueEvent(()->{String report=NativeBridge.playerAttack(-1);Log.i("DH2Native","Player input | "+report);show(baseReport+"\n"+report);}));
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
                    if("com.example.dh2.DEBUG_ANIMATION_TIME".equals(intent.getAction())){
                        final int time=intent.getIntExtra("time_ms",-1);
                        surface.queueEvent(()->{NativeBridge.animationTime(time);Log.i("DH2Native","Animation time command applied | time "+time);surface.requestRender();});
                        return;
                    }
                    final int target=intent.getIntExtra("player_target_index",-1);
                    surface.queueEvent(()->{String report=NativeBridge.playerAttack(target);Log.i("DH2Native","Player command applied | "+report);show(baseReport+"\n"+report);});
                }
            };
            IntentFilter filter=new IntentFilter("com.example.dh2.DEBUG_PLAYER_ATTACK");
            filter.addAction("com.example.dh2.DEBUG_ANIMATION_TIME");
            if(Build.VERSION.SDK_INT>=33)registerReceiver(debugAttackReceiver,filter,"android.permission.DUMP",null,Context.RECEIVER_EXPORTED);
            else registerReceiver(debugAttackReceiver,filter,"android.permission.DUMP",null);
        }
    }
    private void show(String text){runOnUiThread(()->status.setText(text));}
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
            show(baseReport+"\n"+report);surface.requestRender();
        });
    }
    private void loadSelected(){
        String name=assets[selected];
        if(name.equals(loadedAsset))return;
        runOnUiThread(()->{movement.setVisibility(name.startsWith("worlds/")?View.VISIBLE:View.GONE);vitals.setVisibility(name.startsWith("worlds/")?View.VISIBLE:View.GONE);attack.setVisibility(name.startsWith("worlds/")?View.VISIBLE:View.GONE);});
        try(InputStream input=getAssets().open(name);ByteArrayOutputStream out=new ByteArrayOutputStream()){
            byte[] block=new byte[8192];int n;
            while((n=input.read(block))!=-1){if(out.size()+n>32*1024*1024)throw new java.io.IOException("Texture too large");out.write(block,0,n);}
            String report=name.startsWith("worlds/")?NativeBridge.loadWorld(out.toByteArray(),getAssets()):name.startsWith("models/")?NativeBridge.loadModel(out.toByteArray(),getAssets()):NativeBridge.loadTexture(out.toByteArray());
            if(!report.contains("failed")&&!report.contains("error"))loadedAsset=name;
            baseReport=name+"\n"+report;
            if(name.startsWith("models/")||name.startsWith("worlds/"))NativeBridge.animationTime(getIntent().getIntExtra("time_ms",-1));
            if(name.startsWith("worlds/"))NativeBridge.focusObject(getIntent().getIntExtra("object_index",-1));
            if(name.startsWith("worlds/")&&pendingActorCommand&&getIntent().getBooleanExtra("player_attack",false)){
                String attackReport=NativeBridge.playerAttack(getIntent().getIntExtra("player_target_index",-1));report+="\n"+attackReport;
                Log.i("DH2Native","Player command applied | "+attackReport);pendingActorCommand=false;
            }
            if(name.startsWith("worlds/")&&getIntent().getStringExtra("object_state")!=null&&(!report.contains("Native combat resumed")||pendingActorCommand)){
                int index=getIntent().getIntExtra("object_index",-1);
                String targetReport=NativeBridge.combatTarget(index,getIntent().getIntExtra("combat_target_index",-1));
                report+="\n"+(targetReport.equals("Combat target selected")||targetReport.equals("Combat target cleared")?NativeBridge.objectState(index,getIntent().getStringExtra("object_state")):targetReport);
            }
            Log.i("DH2Native",name+": "+report);
            if(name.startsWith("worlds/")&&pendingActorCommand){
                Log.i("DH2Native","Actor command applied | index "+getIntent().getIntExtra("object_index",-1)+" | state "+getIntent().getStringExtra("object_state")+" | time "+getIntent().getIntExtra("time_ms",-1)+" | "+report);
                pendingActorCommand=false;
            }
            show(name+"\n"+report);surface.requestRender();
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
    @Override protected void onPause(){super.onPause();ready=false;movement.stop();surface.onPause();}
    @Override protected void onResume(){super.onResume();surface.onResume();}
    @Override protected void onDestroy(){if(debugAttackReceiver!=null)unregisterReceiver(debugAttackReceiver);super.onDestroy();}
    @Override protected void onSaveInstanceState(Bundle state){super.onSaveInstanceState(state);if(assets.length>0)state.putString("asset",assets[selected]);state.putBoolean("pendingPlayerAttack",pendingActorCommand&&getIntent().getBooleanExtra("player_attack",false));state.putBoolean("enemyAi",enemyAi);}
}
