package com.gameloft.android.GAND.GloftD2SS.installer;

import android.widget.ProgressBar;
import com.samsung.zirconia.R;

/* JADX INFO: loaded from: classes.dex */
final class a implements Runnable {

    /* JADX INFO: renamed from: a, reason: collision with root package name */
    final /* synthetic */ GameInstaller f116a;

    a(GameInstaller gameInstaller) {
        this.f116a = gameInstaller;
    }

    @Override // java.lang.Runnable
    public final void run() {
        try {
            ProgressBar progressBar = (ProgressBar) this.f116a.findViewById(com.samsung.zirconia.R.id.data_downloader_linear_progress_bar);
            if (progressBar != null) {
                progressBar.setProgress(this.f116a.ba);
            }
        } catch (Exception e) {
        }
    }
}
