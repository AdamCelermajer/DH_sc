package com.gameloft.layout;

import android.content.Context;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.widget.TextView;
import com.samsung.zirconia.R;

/* JADX INFO: loaded from: classes.dex */
public class DataDownloaderTextView extends TextView {
    private DataDownloaderTextView(Context context) {
        super(context);
        a();
    }

    public DataDownloaderTextView(Context context, AttributeSet attributeSet) {
        super(context, attributeSet);
        a();
    }

    public DataDownloaderTextView(Context context, AttributeSet attributeSet, int i) {
        super(context, attributeSet, i);
        a();
    }

    private void a() {
        if (isInEditMode()) {
            return;
        }
        try {
            setTypeface(Typeface.createFromAsset(getContext().getAssets(), getContext().getString(com.samsung.zirconia.R.string.data_downloader_custom_font_name)));
        } catch (Exception e) {
        }
    }
}
