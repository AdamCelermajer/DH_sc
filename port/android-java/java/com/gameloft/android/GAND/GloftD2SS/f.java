package com.gameloft.android.GAND.GloftD2SS;

import android.media.MediaPlayer;

/* JADX INFO: loaded from: classes.dex */
final class f implements MediaPlayer.OnCompletionListener {
    f() {
    }

    @Override // android.media.MediaPlayer.OnCompletionListener
    public final void onCompletion(MediaPlayer mediaPlayer) {
        GLMediaPlayer.b[GLMediaPlayer.H] = 9;
    }
}
