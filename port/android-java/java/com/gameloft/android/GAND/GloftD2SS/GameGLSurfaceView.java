package com.gameloft.android.GAND.GloftD2SS;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

/* JADX INFO: loaded from: classes.dex */
class GameGLSurfaceView extends GLSurfaceView {
    public static boolean c;
    public static int d;
    GameRenderer e;

    /* JADX INFO: renamed from: a, reason: collision with root package name */
    public static int f23a = -1;
    public static int b = -1;
    public static int f = 0;
    public static boolean g = false;

    public static class ConfigChooser implements GLSurfaceView.EGLConfigChooser {
        private static int g = 4;
        private static int[] h = {12324, 4, 12323, 4, 12322, 4, 12352, g, 12344};
        protected int e;
        protected int f;
        private int[] i = new int[1];

        /* JADX INFO: renamed from: a, reason: collision with root package name */
        protected int f24a = 5;
        protected int b = 6;
        protected int c = 5;
        protected int d = 0;

        public ConfigChooser(int i, int i2, int i3, int i4, int i5, int i6) {
            this.e = i5;
            this.f = i6;
        }

        private int a(EGL10 egl10, EGLDisplay eGLDisplay, EGLConfig eGLConfig, int i, int i2) {
            if (egl10.eglGetConfigAttrib(eGLDisplay, eGLConfig, i, this.i)) {
                return this.i[0];
            }
            return 0;
        }

        /* JADX WARN: Code duplicated, block: B:12:0x0081  */
        private EGLConfig a(EGL10 egl10, EGLDisplay eGLDisplay, EGLConfig[] eGLConfigArr) {
            int iAbs;
            EGLConfig eGLConfig = null;
            int i = 1000;
            int length = eGLConfigArr.length;
            int i2 = 0;
            while (i2 < length) {
                EGLConfig eGLConfig2 = eGLConfigArr[i2];
                int iA = a(egl10, eGLDisplay, eGLConfig2, 12325, 0);
                int iA2 = a(egl10, eGLDisplay, eGLConfig2, 12326, 0);
                if (iA < this.e || iA2 < this.f) {
                    iAbs = i;
                    eGLConfig2 = eGLConfig;
                } else {
                    int iA3 = a(egl10, eGLDisplay, eGLConfig2, 12324, 0);
                    int iA4 = a(egl10, eGLDisplay, eGLConfig2, 12323, 0);
                    int iA5 = a(egl10, eGLDisplay, eGLConfig2, 12322, 0);
                    iAbs = Math.abs(a(egl10, eGLDisplay, eGLConfig2, 12321, 0) - this.d) + Math.abs(iA3 - this.f24a) + Math.abs(iA4 - this.b) + Math.abs(iA5 - this.c);
                    if (iAbs >= i) {
                        iAbs = i;
                        eGLConfig2 = eGLConfig;
                    }
                }
                i2++;
                i = iAbs;
                eGLConfig = eGLConfig2;
            }
            return eGLConfig;
        }

        private void b(EGL10 egl10, EGLDisplay eGLDisplay, EGLConfig[] eGLConfigArr) {
            for (EGLConfig eGLConfig : eGLConfigArr) {
                String[] strArr = {"EGL_BUFFER_SIZE", "EGL_ALPHA_SIZE", "EGL_BLUE_SIZE", "EGL_GREEN_SIZE", "EGL_RED_SIZE", "EGL_DEPTH_SIZE", "EGL_STENCIL_SIZE", "EGL_CONFIG_CAVEAT", "EGL_CONFIG_ID", "EGL_LEVEL", "EGL_MAX_PBUFFER_HEIGHT", "EGL_MAX_PBUFFER_PIXELS", "EGL_MAX_PBUFFER_WIDTH", "EGL_NATIVE_RENDERABLE", "EGL_NATIVE_VISUAL_ID", "EGL_NATIVE_VISUAL_TYPE", "EGL_PRESERVED_RESOURCES", "EGL_SAMPLES", "EGL_SAMPLE_BUFFERS", "EGL_SURFACE_TYPE", "EGL_TRANSPARENT_TYPE", "EGL_TRANSPARENT_RED_VALUE", "EGL_TRANSPARENT_GREEN_VALUE", "EGL_TRANSPARENT_BLUE_VALUE", "EGL_BIND_TO_TEXTURE_RGB", "EGL_BIND_TO_TEXTURE_RGBA", "EGL_MIN_SWAP_INTERVAL", "EGL_MAX_SWAP_INTERVAL", "EGL_LUMINANCE_SIZE", "EGL_ALPHA_MASK_SIZE", "EGL_COLOR_BUFFER_TYPE", "EGL_RENDERABLE_TYPE", "EGL_CONFORMANT"};
                int[] iArr = new int[1];
                for (int i : new int[]{12320, 12321, 12322, 12323, 12324, 12325, 12326, 12327, 12328, 12329, 12330, 12331, 12332, 12333, 12334, 12335, 12336, 12337, 12338, 12339, 12340, 12343, 12342, 12341, 12345, 12346, 12347, 12348, 12349, 12350, 12351, 12352, 12354}) {
                    if (!egl10.eglGetConfigAttrib(eGLDisplay, eGLConfig, i, iArr)) {
                        while (egl10.eglGetError() != 12288) {
                        }
                    }
                }
            }
        }

        private static void printConfig(EGL10 egl10, EGLDisplay eGLDisplay, EGLConfig eGLConfig) {
            String[] strArr = {"EGL_BUFFER_SIZE", "EGL_ALPHA_SIZE", "EGL_BLUE_SIZE", "EGL_GREEN_SIZE", "EGL_RED_SIZE", "EGL_DEPTH_SIZE", "EGL_STENCIL_SIZE", "EGL_CONFIG_CAVEAT", "EGL_CONFIG_ID", "EGL_LEVEL", "EGL_MAX_PBUFFER_HEIGHT", "EGL_MAX_PBUFFER_PIXELS", "EGL_MAX_PBUFFER_WIDTH", "EGL_NATIVE_RENDERABLE", "EGL_NATIVE_VISUAL_ID", "EGL_NATIVE_VISUAL_TYPE", "EGL_PRESERVED_RESOURCES", "EGL_SAMPLES", "EGL_SAMPLE_BUFFERS", "EGL_SURFACE_TYPE", "EGL_TRANSPARENT_TYPE", "EGL_TRANSPARENT_RED_VALUE", "EGL_TRANSPARENT_GREEN_VALUE", "EGL_TRANSPARENT_BLUE_VALUE", "EGL_BIND_TO_TEXTURE_RGB", "EGL_BIND_TO_TEXTURE_RGBA", "EGL_MIN_SWAP_INTERVAL", "EGL_MAX_SWAP_INTERVAL", "EGL_LUMINANCE_SIZE", "EGL_ALPHA_MASK_SIZE", "EGL_COLOR_BUFFER_TYPE", "EGL_RENDERABLE_TYPE", "EGL_CONFORMANT"};
            int[] iArr = new int[1];
            for (int i : new int[]{12320, 12321, 12322, 12323, 12324, 12325, 12326, 12327, 12328, 12329, 12330, 12331, 12332, 12333, 12334, 12335, 12336, 12337, 12338, 12339, 12340, 12343, 12342, 12341, 12345, 12346, 12347, 12348, 12349, 12350, 12351, 12352, 12354}) {
                if (!egl10.eglGetConfigAttrib(eGLDisplay, eGLConfig, i, iArr)) {
                    while (egl10.eglGetError() != 12288) {
                    }
                }
            }
        }

        @Override // android.opengl.GLSurfaceView.EGLConfigChooser
        public EGLConfig chooseConfig(EGL10 egl10, EGLDisplay eGLDisplay) {
            int[] iArr = new int[1];
            egl10.eglChooseConfig(eGLDisplay, h, null, 0, iArr);
            int i = iArr[0];
            if (i <= 0) {
                throw new IllegalArgumentException("No configs match configSpec");
            }
            EGLConfig[] eGLConfigArr = new EGLConfig[i];
            egl10.eglChooseConfig(eGLDisplay, h, eGLConfigArr, i, iArr);
            b(egl10, eGLDisplay, eGLConfigArr);
            return a(egl10, eGLDisplay, eGLConfigArr);
        }
    }

    public static class ContextFactory implements GLSurfaceView.EGLContextFactory {

        /* JADX INFO: renamed from: a, reason: collision with root package name */
        public static int f25a = 12440;

        @Override // android.opengl.GLSurfaceView.EGLContextFactory
        public EGLContext createContext(EGL10 egl10, EGLDisplay eGLDisplay, EGLConfig eGLConfig) {
            GameGLSurfaceView.checkEglError("Before eglCreateContext", egl10);
            EGLContext eGLContextEglCreateContext = egl10.eglCreateContext(eGLDisplay, eGLConfig, EGL10.EGL_NO_CONTEXT, new int[]{f25a, 2, 12344});
            GameGLSurfaceView.checkEglError("After eglCreateContext", egl10);
            return eGLContextEglCreateContext;
        }

        @Override // android.opengl.GLSurfaceView.EGLContextFactory
        public void destroyContext(EGL10 egl10, EGLDisplay eGLDisplay, EGLContext eGLContext) {
            egl10.eglDestroyContext(eGLDisplay, eGLContext);
        }
    }

    public GameGLSurfaceView(Context context) {
        super(context);
        this.e = new GameRenderer(context);
        setEGLContextFactory(new ContextFactory());
        setEGLConfigChooser(new ConfigChooser(5, 6, 5, 0, 16, 8));
        setRenderer(this.e);
    }

    private void a(boolean z, int i, int i2) {
        setEGLContextFactory(new ContextFactory());
        setEGLConfigChooser(new ConfigChooser(5, 6, 5, 0, 16, 8));
        setRenderer(this.e);
    }

    public static void checkEglError(String str, EGL10 egl10) {
        while (egl10.eglGetError() != 12288) {
        }
    }

    public static native void nativeOnTouch(int i, int i2, int i3, long j, int i4, int i5);

    @Override // android.opengl.GLSurfaceView
    public void onPause() {
        super.onPause();
    }

    @Override // android.opengl.GLSurfaceView
    public void onResume() {
        super.onResume();
        String str = Build.MANUFACTURER + "_" + Build.MODEL;
    }

    @Override // android.view.View
    public void onSizeChanged(int i, int i2, int i3, int i4) {
        f23a = i;
        b = i2;
    }

    @Override // android.view.View
    public boolean onTouchEvent(MotionEvent motionEvent) {
        int action = motionEvent.getAction();
        int i = action & 255;
        int pointerCount = motionEvent.getPointerCount();
        int actionIndex = motionEvent.getActionIndex();
        int pointerId = motionEvent.getPointerId(actionIndex);
        System.out.println("------------------------------------Count = " + pointerCount);
        motionEvent.getEventTime();
        if (action == 0) {
            try {
                nativeOnTouch(1, (int) motionEvent.getX(0), (int) motionEvent.getY(0), 0L, 0, 0);
            } catch (Exception e) {
            }
        }
        if (i == 5) {
            try {
                nativeOnTouch(1, (int) motionEvent.getX(actionIndex), (int) motionEvent.getY(actionIndex), pointerId, 0, 0);
            } catch (Exception e2) {
            }
        }
        if (action == 2) {
            for (int i2 = 0; i2 < pointerCount; i2++) {
                try {
                    nativeOnTouch(2, (int) motionEvent.getX(i2), (int) motionEvent.getY(i2), motionEvent.getPointerId(i2), 0, 0);
                } catch (Exception e3) {
                }
            }
        }
        if (i == 6) {
            try {
                nativeOnTouch(0, (int) motionEvent.getX(actionIndex), (int) motionEvent.getY(actionIndex), pointerId, 0, 0);
            } catch (Exception e4) {
            }
        }
        if (action != 1) {
            return true;
        }
        for (int i3 = 0; i3 < pointerCount; i3++) {
            try {
                nativeOnTouch(0, (int) motionEvent.getX(i3), (int) motionEvent.getY(i3), motionEvent.getPointerId(i3), 0, 0);
            } catch (Exception e5) {
                e5.printStackTrace();
            }
        }
        return true;
    }

    @Override // android.view.View
    public void onWindowFocusChanged(boolean z) {
        System.out.println("**********************************FFFFFFFFFFocus : " + z);
        GameRenderer.e = z;
        if (!z) {
            GLMediaPlayer.stopAllSounds();
            DungeonHunter2.nativePause(1);
            return;
        }
        if (GLiveMain.cf) {
            System.out.println("Focus-----------------launchGLLive");
            DungeonHunter2.OpenGLive(DungeonHunter2.x);
        } else if (IGPActivity.f28a) {
            System.out.println("Focus-----------------launchIGP");
            DungeonHunter2.OpenIGP(DungeonHunter2.y);
        } else {
            if (GLMediaPlayer.M) {
                return;
            }
            DungeonHunter2.nativeResume(1);
        }
    }
}
