#!/usr/bin/env python3
"""Guard the Android full-surface menu viewport and inset ownership contract."""

from pathlib import Path
import xml.etree.ElementTree as ET


ROOT = Path(__file__).resolve().parents[3]
ACTIVITY = ROOT / "port/android-native/app/src/main/java/com/example/dh2/MainActivity.java"
MANIFEST = ROOT / "port/android-native/app/src/main/AndroidManifest.xml"
NATIVE = ROOT / "port/android-native/app/src/main/cpp/native_app.cpp"
UI = ROOT / "port/android-native/app/src/main/cpp/original_ui_session.cpp"
VIEWPORT = ROOT / "port/engine-ui/original_menu_viewport_v1.hpp"
SMOKE = ROOT / "port/android-native/tools/menu_ui_runtime_smoke.py"
SPLASH = ROOT / "port/android-native/app/src/main/cpp/original_ui_session.cpp"
CINEMATIC = ROOT / "port/android-native/app/src/main/java/com/example/dh2/IntroCinematicView.java"
VIDEO_FIT = ROOT / "port/android-native/app/src/main/java/com/example/dh2/IntroVideoFit.java"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise SystemExit("FAIL " + message)


activity = ACTIVITY.read_text(encoding="utf-8")
native = NATIVE.read_text(encoding="utf-8")
ui = UI.read_text(encoding="utf-8")
viewport = VIEWPORT.read_text(encoding="utf-8")
smoke = SMOKE.read_text(encoding="utf-8")
splash = SPLASH.read_text(encoding="utf-8")
cinematic = CINEMATIC.read_text(encoding="utf-8")
video_fit = VIDEO_FIT.read_text(encoding="utf-8")
manifest = ET.parse(MANIFEST).getroot()
android = "{http://schemas.android.com/apk/res/android}"
main = next(node for node in manifest.iter("activity")
            if node.get(android + "name") == ".MainActivity")

require(main.get(android + "screenOrientation") == "sensorLandscape",
        "launcher activity must retain the original landscape presentation")
require("WindowCompat.setDecorFitsSystemWindows(getWindow(),false)" in activity,
        "normal game window must lay out edge-to-edge before surface creation")
require("LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES" in activity,
        "landscape rendering must be allowed to use the short-edge cutout area")
require("controller.hide(WindowInsets.Type.systemBars())" in activity
        and "BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE" in activity,
        "immersive game hides system bars while allowing transient swipe access")
require("setOnApplyWindowInsetsListener(viewport" in activity
        and "safeInsets[0]=safe.left" in activity
        and "return insets;" in activity,
        "insets must update gameplay overlay placement without padding/resizing the SWF stage")
require("NativeBridge.menuTouch(x,y,action)" in activity
        and "Keep input in GLSurfaceView-local pixels" in activity,
        "touch input must stay in the same local-pixel coordinate space as the GL surface")
require('original_menu_viewport_v1::full_surface(surface_width,surface_height)' in native
        and "glViewport(viewport[0],viewport[1],viewport[2],viewport[3])" in native
        and "original_ui.render(surface_width,surface_height,error)" in native,
        "native GL and menu renderer must share the same canonical full-surface frame")
require("viewport_for_screen_state_v1(" in ui
        and "return full_surface(surface_width, surface_height);" in viewport
        and "background_viewport" in viewport,
        "front SWF and 3D backdrop must use the original full-driver bounds")
require("stage_width_twips = 9600" in viewport
        and "stage_height_twips = 6400" in viewport,
        "logical SWF stage must remain authored at 480x320 pixels")
require("IntroVideoFit.fullSurface(" in cinematic
        and "static Result fullSurface(" in video_fit
        and "return new Result(surfaceWidth, surfaceHeight,\n                surfaceWidth, surfaceHeight);" in video_fit,
        "opening movie must stretch to both live surface axes without aspect bars")
require("PixelCopy.request(surface,snapshot" in activity
        and "overlay.setScaleType(ImageView.ScaleType.FIT_XY)" in activity
        and "awaitingFirstWorldFrame&&loadedAsset!=null&&loadedAsset.startsWith(\"worlds/\")" in activity
        and "surface.queueEvent(()->surface.post(()->surface.postOnAnimation" in activity,
        "synchronous game startup must preserve the source loading image until a world frame is drawn")
require("const auto splash_viewport=ui::original_menu_viewport_v1::background_viewport(width,height);" in splash
        and "background.viewport[2]=splash_viewport[2]; background.viewport[3]=splash_viewport[3];" in splash,
        "two-brothers startup splash must use the complete display rectangle")
require("MenuFlash2DCamera sends the complete live driver rectangle to the SWF." in smoke
        and "return (0, 0, width, height)" in smoke,
        "API 37 smoke taps must target the same full-driver bounds as production")
require("if(inspectionMode)layout.setOnApplyWindowInsetsListener" in activity,
        "inspection-shell safe-area padding must not change the production game stage")

print("PASS fullscreen viewport: landscape, edge-to-edge/cutout, immersive bars, and full GL backdrop")
print("PASS layout scope: front SWFs retain the 480x320 authored stage over full-driver bounds; HUD remains full-surface")
print("PASS startup scale: opening movie and two-brothers splash fill both live surface axes")
