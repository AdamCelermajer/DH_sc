package com.gameloft.android.GAND.GloftD2SS.installer;

import android.content.Context;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import com.samsung.zirconia.R;
import java.util.Iterator;

/* JADX INFO: loaded from: classes.dex */
final class b implements Runnable {

    /* JADX INFO: renamed from: a, reason: collision with root package name */
    final /* synthetic */ int f117a;
    final /* synthetic */ int b;
    final /* synthetic */ Context c;
    final /* synthetic */ GameInstaller d;

    b(GameInstaller gameInstaller, int i, int i2, Context context) {
        this.d = gameInstaller;
        this.f117a = i;
        this.b = i2;
        this.c = context;
    }

    /* JADX DEBUG: TODO: convert one arg to string using `String.valueOf()`, args: (wrap int:CAST) */
    /* JADX DEBUG: TODO: convert one arg to string using `String.valueOf()`, args: (wrap long:TERNARY) */
    @Override // java.lang.Runnable
    public final void run() {
        try {
            GameInstaller.access$402(this.d, this.f117a);
            this.d.setContentView(this.f117a);
            if (this.b != GameInstaller.access$500(this.d)) {
                GameInstaller.access$602(this.d, GameInstaller.access$500(this.d));
                GameInstaller.access$502(this.d, this.b);
            }
            switch (this.b) {
                case 0:
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_yes, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_no, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.CHECKING_REQUIRED_FILES, new Object[]{this.c}));
                    break;
                case 1:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    if (GameInstaller.access$900(this.d) != 2) {
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_FILES_THROUGH_CARRIER_QUESTION, new Object[]{this.c}) + "\n" + this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_FILES_THROUGH_CARRIER_EXTRA, new Object[]{this.c}));
                    } else {
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_FILES_THROUGH_CARRIER_QUESTION_3G_ORANGE_IL, new Object[]{this.c}));
                    }
                    break;
                case 2:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.STATE_CONFIRM_UPDATE, new Object[]{this.c}));
                    break;
                case 3:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes)).setText(this.d.getString(com.samsung.zirconia.R.string.RETRY_WIFI, new Object[]{this.c}));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no));
                    ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no)).setText(this.d.getString(com.samsung.zirconia.R.string.CARRIER, new Object[]{this.c}));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_cancel));
                    ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_cancel)).setText(this.d.getString(com.samsung.zirconia.R.string.CANCEL, new Object[]{this.c}));
                    if (GameInstaller.access$900(this.d) != 2) {
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.ERROR_NO_WIFI_DETECTED, new Object[]{this.c}));
                    } else {
                        ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no)).setText(this.d.getString(com.samsung.zirconia.R.string.USE_3G_ORANGE_IL, new Object[]{this.c}));
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.ERROR_NO_WIFI_DETECTED_3G_ORANGE_IL, new Object[]{this.c}));
                    }
                    break;
                case 4:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes)).setText(this.d.getString(com.samsung.zirconia.R.string.OK, new Object[]{this.c}));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_no, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    if (GameInstaller.access$900(this.d) == 1 || GameInstaller.access$600(this.d) == 28) {
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_ANYTIME_MESSAGE_MINIMAL, new Object[]{this.c}));
                    } else if (GameInstaller.access$900(this.d) == 2) {
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_ANYTIME_MESSAGE_3G_ORANGE_IL, new Object[]{this.c}));
                    } else {
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_ANYTIME_MESSAGE, new Object[]{this.c}));
                    }
                    if (this.b != GameInstaller.access$500(this.d)) {
                        this.d.b();
                    }
                    break;
                case 5:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_cancel));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_yes, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_no, false);
                    ((ProgressBar) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_linear_progress_bar)).setMax((int) ((this.d.j / 1024) + 1));
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DONT_TURN_OFF_YOUR_PHONE, new Object[]{this.c}));
                    TextView textView = (TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_progress_text);
                    textView.setVisibility(4);
                    if (GameInstaller.access$900(this.d) == 1) {
                        textView.setText(this.d.getString(com.samsung.zirconia.R.string.DONT_TURN_OFF_YOUR_PHONE_WIFI, new Object[]{this.c}));
                    } else if (GameInstaller.access$1100(this.d) != 1) {
                        this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text);
                    } else {
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.a(com.samsung.zirconia.R.string.DONT_TURN_OFF_YOUR_PHONE_WIFI, null, new StringBuilder().append((int) ((GameInstaller.m_iRealRequiredSize >> 20) + 1)).toString()));
                    }
                    break;
                case 7:
                case GameInstaller.LAYOUT_UNZIP_FILES_CANCEL_QUESTION /* 28 */:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_FILE_CANCEL_QUESTION, new Object[]{this.c}));
                    break;
                case 8:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_FAIL, new Object[]{this.c}));
                    break;
                case 9:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_FILES_NO_WIFI_QUESTION, new Object[]{this.c}));
                    GameInstaller.access$800(this.d, com.samsung.zirconia.R.id.data_downloader_progress_bar, false);
                    if (GameInstaller.access$900(this.d) == 1) {
                        ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no)).setText(this.d.getString(com.samsung.zirconia.R.string.LATER, new Object[]{this.c}));
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.ERROR_NO_WIFI_DETECTED_MINIMAL, new Object[]{this.c}));
                    } else if (GameInstaller.access$900(this.d) == 0) {
                        ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no)).setText(this.d.getString(com.samsung.zirconia.R.string.USE_3G, new Object[]{this.c}));
                        ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes)).setText(this.d.getString(com.samsung.zirconia.R.string.USE_WIFI, new Object[]{this.c}));
                    } else if (GameInstaller.access$900(this.d) == 2) {
                        ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no)).setText(this.d.getString(com.samsung.zirconia.R.string.USE_3G_ORANGE_IL, new Object[]{this.c}));
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_FILES_NO_WIFI_QUESTION_3G_ORANGE_IL, new Object[]{this.c}));
                        ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes)).setText(this.d.getString(com.samsung.zirconia.R.string.USE_WIFI, new Object[]{this.c}));
                    }
                    break;
                case GameInstaller.LAYOUT_DOWNLOAD_FILES_QUESTION /* 10 */:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.a(com.samsung.zirconia.R.string.DOWNLOAD_FILES_QUESTION, null, new StringBuilder().append((int) ((GameInstaller.m_iRealRequiredSize >> 20) + 1)).toString()));
                    break;
                case 13:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes)).setText(this.d.getString(com.samsung.zirconia.R.string.OK, new Object[]{this.c}));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_no, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.a(GameInstaller.access$1000(this.d), "{GAME_NAME}", this.d.getString(com.samsung.zirconia.R.string.app_name)));
                    break;
                case 16:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.ERROR_NO_CARRIER_DATA_DETECTED, new Object[]{this.c}));
                    break;
                case 17:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.STATE_CONNECT_FAIL, new Object[]{this.c}));
                    break;
                case 18:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes)).setText(this.d.getString(com.samsung.zirconia.R.string.OK, new Object[]{this.c}));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_no, false);
                    long j = this.d.h;
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.a(this.d.a(0) ? com.samsung.zirconia.R.string.NO_EXTERNAL_STORAGE_FOUND : com.samsung.zirconia.R.string.NO_ENOUGH_SPACE_AVAILABLE, null, new StringBuilder().append(this.d.a(0) ? j : j - this.d.g).toString()));
                    break;
                case 19:
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_yes, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_no, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.STATE_SEND_REQUEST, new Object[]{this.c}));
                    break;
                case 20:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_cancel));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_yes, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_no, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.WAIT_WHILE_ACTIVATING_WIFI, new Object[]{this.c}));
                    break;
                case 21:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes)).setText(this.d.getString(com.samsung.zirconia.R.string.OK, new Object[]{this.c}));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_no, false);
                    GameInstaller.access$800(this.d, com.samsung.zirconia.R.id.data_downloader_progress_bar, false);
                    if (this.d.aq != 29) {
                        if (this.d.aX) {
                            GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_yes, false);
                            GameInstaller.access$800(this.d, com.samsung.zirconia.R.id.data_downloader_progress_bar, true);
                        }
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.DOWNLOAD_SUCCESSFULLY, new Object[]{this.c}));
                    } else {
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.STATE_NO_NEW_VERSION, new Object[]{this.c}));
                    }
                    break;
                case 22:
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_yes, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_no, false);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.VERIFYING, new Object[]{this.c}));
                    break;
                case 23:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes));
                    ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_yes)).setText(this.d.getString(com.samsung.zirconia.R.string.OK, new Object[]{this.c}));
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no));
                    ((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_no)).setText(this.d.getString(com.samsung.zirconia.R.string.CANCEL, new Object[]{this.c}));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_cancel, false);
                    if (GameInstaller.access$900(this.d) != 2) {
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.ERROR_NO_WIFI_DETECTED_2, new Object[]{this.c}));
                    } else {
                        ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(this.d.getString(com.samsung.zirconia.R.string.ERROR_NO_WIFI_DETECTED_3G_ORANGE_IL, new Object[]{this.c}));
                    }
                    break;
                case GameInstaller.LAYOUT_UNZIP_FILES /* 27 */:
                    this.d.a().add((Button) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_cancel));
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_yes, false);
                    GameInstaller.access$700(this.d, com.samsung.zirconia.R.id.data_downloader_no, false);
                    ((ProgressBar) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_linear_progress_bar)).setMax((int) ((this.d.j / 1024) + 1));
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_main_text)).setText(com.samsung.zirconia.R.string.EXTRACTING);
                    ((TextView) this.d.findViewById(com.samsung.zirconia.R.id.data_downloader_progress_text)).setVisibility(4);
                    break;
            }
            try {
                Iterator it = this.d.a().iterator();
                while (it.hasNext()) {
                    ((Button) it.next()).setOnClickListener(this.d.bj);
                }
            } catch (Exception e) {
            }
        } catch (Exception e2) {
            GameInstaller.access$200(this.d, 21);
        }
    }
}
