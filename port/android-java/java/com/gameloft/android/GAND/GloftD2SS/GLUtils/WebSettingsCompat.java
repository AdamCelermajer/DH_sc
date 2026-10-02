package com.gameloft.android.GAND.GloftD2SS.GLUtils;

import android.webkit.WebSettings;
import java.lang.reflect.InvocationTargetException;

/** Preserves the old AppCache setter where present; modern WebView removed AppCache. */
public final class WebSettingsCompat {
    private WebSettingsCompat() {}

    public static void setAppCacheEnabled(WebSettings settings, boolean enabled) {
        try {
            WebSettings.class.getMethod("setAppCacheEnabled", boolean.class).invoke(settings, enabled);
        } catch (NoSuchMethodException removedInModernWebView) {
            // AppCache itself was removed, so there is no cache feature left to disable.
        } catch (InvocationTargetException error) {
            WebSettingsCompat.<RuntimeException>rethrow(error.getCause());
        } catch (IllegalAccessException error) {
            throw new IllegalStateException(error);
        }
    }

    @SuppressWarnings("unchecked")
    private static <T extends Throwable> void rethrow(Throwable error) throws T {
        throw (T) error;
    }
}
