package com.gameloft.android.GAND.GloftD2SS;

import com.samsung.zirconia.R;

/* JADX INFO: loaded from: classes.dex */
final class az implements Runnable {

    /* JADX INFO: renamed from: a, reason: collision with root package name */
    final /* synthetic */ ay f62a;

    az(ay ayVar) {
        this.f62a = ayVar;
    }

    @Override // java.lang.Runnable
    public final void run() {
        this.f62a.f61a.a(true);
        this.f62a.f61a.a(this.f62a.f61a.getString(com.samsung.zirconia.R.string.DRM_EZIRCONIA_SUCCESS_1));
    }
}
