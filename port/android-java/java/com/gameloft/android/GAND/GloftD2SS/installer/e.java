package com.gameloft.android.GAND.GloftD2SS.installer;

import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;
import com.samsung.zirconia.R;

/* JADX INFO: loaded from: classes.dex */
final class e implements Runnable {

    /* JADX INFO: renamed from: a, reason: collision with root package name */
    final /* synthetic */ String f120a;
    final /* synthetic */ String b;
    final /* synthetic */ GameInstaller c;

    e(GameInstaller gameInstaller, String str, String str2) {
        this.c = gameInstaller;
        this.f120a = str;
        this.b = str2;
    }

    @Override // java.lang.Runnable
    public final void run() {
        View viewInflate = this.c.getLayoutInflater().inflate(com.samsung.zirconia.R.layout.gi_layout_download_toast_message, (ViewGroup) this.c.findViewById(com.samsung.zirconia.R.id.toast_layout));
        ((TextView) viewInflate.findViewById(com.samsung.zirconia.R.id.data_downloader_toast_message)).setText(this.f120a);
        if (this.b != null) {
            ((TextView) viewInflate.findViewById(com.samsung.zirconia.R.id.data_downloader_toast_title)).setText(this.b);
        }
        Toast toast = new Toast(this.c.getApplicationContext());
        toast.setGravity(17, 0, 0);
        toast.setDuration(1);
        toast.setView(viewInflate);
    }
}
