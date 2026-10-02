package com.gameloft.android.GAND.GloftD2SS;

import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageButton;
import com.samsung.zirconia.R;

/* JADX INFO: loaded from: classes.dex */
final class j implements View.OnTouchListener {

    /* JADX INFO: renamed from: a, reason: collision with root package name */
    final /* synthetic */ GLiveMain f160a;

    j(GLiveMain gLiveMain) {
        this.f160a = gLiveMain;
    }

    @Override // android.view.View.OnTouchListener
    public final boolean onTouch(View view, MotionEvent motionEvent) {
        float x = motionEvent.getX();
        float y = motionEvent.getY();
        switch (motionEvent.getAction()) {
            case 0:
                ((ImageButton) view).setBackgroundResource(com.samsung.zirconia.R.drawable.info_on);
                return true;
            case 1:
                if (x < 0.0f || x > view.getWidth() || y < 0.0f || y > view.getHeight()) {
                    return true;
                }
                ((ImageButton) view).setBackgroundResource(com.samsung.zirconia.R.drawable.info);
                GLiveMain.f20a.loadUrl("http://livewebapp.gameloft.com/glive/info/");
                return true;
            case 2:
                if (x < 0.0f || x > view.getWidth() || y < 0.0f || y > view.getHeight()) {
                    ((ImageButton) view).setBackgroundResource(com.samsung.zirconia.R.drawable.info);
                    return true;
                }
                ((ImageButton) view).setBackgroundResource(com.samsung.zirconia.R.drawable.info_on);
                return true;
            default:
                return false;
        }
    }
}
