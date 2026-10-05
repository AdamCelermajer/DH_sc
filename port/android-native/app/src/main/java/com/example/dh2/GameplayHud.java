package com.example.dh2;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Bitmap;
import android.graphics.RectF;
import android.view.MotionEvent;
import android.view.View;
import java.util.Arrays;

/** A transient presentation of the native player. No inventory or skill state lives here. */
final class GameplayHud extends View {
    interface Actions { void invoke(int operation,int index); }
    private final Paint paint=new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Actions actions;
    private final FaeryPressLifecycleV1 faeryPress;
    private int[] state=new int[0];
    private volatile String[] iconNames=new String[0];
    private final Bitmap[] icons=new Bitmap[5];
    private String message="";
    private long messageUntil;
    private int pressed=-1;
    private int activePointer=-1;
    GameplayHud(Context context,Actions actions){super(context);this.actions=actions;
        faeryPress=new FaeryPressLifecycleV1(operation->actions.invoke(operation,0));
        setContentDescription("Three equipped skills, faery spell, and health potion");setFocusable(true);}
    void update(int[] snapshot){int[] next=snapshot==null?new int[0]:snapshot;
        if(!Arrays.equals(state,next)){state=next.clone();invalidate();}}
    // Decode happens on the GL owner thread; native cache only lends pixels.
    boolean needsIcons(String[] names){return !Arrays.equals(iconNames,names);}
    void icons(String[] names,Bitmap[] bitmaps){iconNames=names.clone();
        for(int i=0;i<5;i++)icons[i]=bitmaps[i];invalidate();}
    void result(String text){message=text==null?"":text;messageUntil=android.os.SystemClock.uptimeMillis()+3000;invalidate();postInvalidateDelayed(3000);}
    private boolean live(){return state.length>=25&&state[0]!=0;}
    private int value(int index,int fallback){return state.length>index?state[index]:fallback;}
    private boolean usable(int control){if(!live())return false;
        if(control<3)return value(17+control,-1)>=0&&value(9+control*2,0)!=0;
        if(control==3)return value(23,-1)>=0&&value(7,0)!=0;
        return value(14,0)>0;}
    private float cx(int control){return getWidth()*(control+.5f)/5f;}
    private float radius(){return Math.min(getWidth()/11f,getHeight()*.30f);}
    @Override protected void onDraw(Canvas canvas){
        float r=radius(),cy=getHeight()*.50f;
        for(int i=0;i<5;i++){
            boolean enabled=usable(i);float x=cx(i);
            paint.setStyle(Paint.Style.FILL);paint.setColor(Color.argb(210,18,22,27));canvas.drawCircle(x,cy,r,paint);
            paint.setStyle(Paint.Style.STROKE);paint.setStrokeWidth(getResources().getDisplayMetrics().density*2);
            paint.setColor(enabled?Color.rgb(198,172,108):Color.rgb(83,87,91));canvas.drawCircle(x,cy,r,paint);
            if(i<4){int pct=value(i==3?6:8+i*2,0);paint.setColor(Color.rgb(100,174,216));
                canvas.drawArc(x-r,cy-r,x+r,cy+r,-90,Math.max(0,Math.min(100,pct))*3.6f,false,paint);}
            paint.setStyle(Paint.Style.FILL);paint.setColor(enabled?Color.WHITE:Color.rgb(160,163,167));
            if(icons[i]!=null){paint.setAlpha(enabled?255:105);canvas.drawBitmap(icons[i],null,new RectF(x-r*.62f,cy-r*.65f,x+r*.62f,cy+r*.40f),paint);paint.setAlpha(255);}
            paint.setTextAlign(Paint.Align.CENTER);paint.setTextSize(r*.38f);
            String main=i<3?(value(17+i,-1)<0?"—":""+(i+1)):i==3?"Faery":"Potion";
            if(icons[i]==null)canvas.drawText(main,x,cy+r*.12f,paint);
            paint.setTextSize(r*.25f);
            String detail=i<3?(value(17+i,-1)<0?"Empty":"Lv "+value(20+i,0)):
                i==3?(value(23,-1)<0?"Locked":"Lv "+value(24,0)):"× "+Math.max(0,value(14,0));
            canvas.drawText(detail,x,cy+r*.55f,paint);
        }
        if(android.os.SystemClock.uptimeMillis()<messageUntil&&!message.isEmpty()){
            paint.setTextSize(radius()*.30f);paint.setTextAlign(Paint.Align.CENTER);paint.setColor(Color.WHITE);
            canvas.drawText(message.length()>85?message.substring(0,82)+"…":message,getWidth()/2f,getHeight()*.08f,paint);
        }else if(state.length>26&&state[25]!=0){
            paint.setTextSize(radius()*.26f);paint.setTextAlign(Paint.Align.CENTER);paint.setColor(Color.rgb(234,174,130));
            canvas.drawText("Skill status unavailable",getWidth()/2f,getHeight()*.08f,paint);
        }
    }
    private int hit(float x,float y){for(int i=0;i<5;i++)if(Math.hypot(x-cx(i),y-getHeight()*.50f)<=radius())return i;return -1;}
    void cancelPress(){if(faeryPress!=null)faeryPress.release();pressed=-1;activePointer=-1;}
    @Override public boolean onTouchEvent(MotionEvent e){
        if(e.getActionMasked()==MotionEvent.ACTION_DOWN){cancelPress();pressed=hit(e.getX(),e.getY());
            if(pressed<0)return false;
            activePointer=e.getPointerId(0);
            if(pressed==3&&usable(3))faeryPress.press();
            return true;}
        if(e.getActionMasked()==MotionEvent.ACTION_UP){int selected=pressed;pressed=-1;
            boolean casting=faeryPress.isPressed();faeryPress.release();activePointer=-1;
            if(selected>=0&&selected==hit(e.getX(),e.getY())){performClick();
                if(selected==3&&casting)return true;
                if(usable(selected)&&selected!=3)actions.invoke(selected<3?0:2,selected<3?selected:0);
                else result(selected<3?(value(17+selected,-1)<0?"No skill equipped":"Skill unavailable"):
                     selected==3?"Faery spell unavailable":"No health potions");}return selected>=0;}
        if(e.getActionMasked()==MotionEvent.ACTION_POINTER_UP&&e.getPointerId(e.getActionIndex())==activePointer){cancelPress();return true;}
        if(e.getActionMasked()==MotionEvent.ACTION_CANCEL){cancelPress();return true;}return pressed>=0;
    }
    @Override public void onWindowFocusChanged(boolean focused){super.onWindowFocusChanged(focused);if(!focused)cancelPress();}
    @Override protected void onVisibilityChanged(View changed,int visibility){super.onVisibilityChanged(changed,visibility);if(visibility!=VISIBLE)cancelPress();}
    @Override protected void onDetachedFromWindow(){cancelPress();super.onDetachedFromWindow();}
    @Override public boolean performClick(){super.performClick();return true;}
}
