#!/usr/bin/env python3
"""Static lifecycle contract for the intro movie and first authored menu."""

from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
ACTIVITY = ROOT / "port/android-native/app/src/main/java/com/example/dh2/MainActivity.java"
VIEW = ROOT / "port/android-native/app/src/main/java/com/example/dh2/IntroCinematicView.java"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise SystemExit("FAIL " + message)


activity = ACTIVITY.read_text(encoding="utf-8")
view = VIEW.read_text(encoding="utf-8")

require("boolean playOpeningCinematic=!inspectionMode&&(state==null||resumeOpeningCinematic)&&!getIntent().getBooleanExtra(\"skip_intro\",false)" in activity,
        "fresh normal install must play; explicit skip and inspection launches must bypass")
require("introCinematic=new IntroCinematicView(this,()->runOnUiThread(this::finishOpeningCinematic))" in activity
        and "introCinematic.startPlayback(openingCinematicPositionMs)" in activity,
        "created movie view must receive the saved playback position and completion callback")
require("candidate.setOnCompletionListener(mp -> finish())" in view
        and "candidate.setOnErrorListener" in view
        and "skipButton.setOnClickListener(v -> finish(true))" in view
        and "skipPolicy.onMovieTap(currentPositionMs())" in view,
        "movie completion and decode failure leave intro; source-gated skip button exits it")
require("selectedAsset = IntroMovieAssets.pathForSourceLanguage(languageIndex)" in view
        and "!sourceLanguageReady" in view
        and "IntroMovieAssets.pathForSourceLanguage(languageIndex)" in view,
        "persisted source language must select the matching movie before playback starts")
require("state.putBoolean(\"openingCinematicPending\",introCinematic!=null&&introCinematic.isPending())" in activity
        and "state.putInt(\"openingCinematicPositionMs\",introCinematic!=null?introCinematic.currentPositionMs():0)" in activity,
        "Activity recreation must preserve whether the intro remains and its current position")
require("@Override protected void onPause(){if(introCinematic!=null)introCinematic.pausePlayback()" in activity
        and "@Override protected void onResume(){super.onResume();frontAudio.setCinematic(introCinematic);frontAudio.resume();if(introCinematic!=null)introCinematic.resumePlayback();" in activity,
        "pause/resume must hold and resume the same movie view and synchronized audio owner")
require("@Override public void onConfigurationChanged" in activity
        and "android:configChanges=\"orientation|screenSize|smallestScreenSize|screenLayout\"" in
        (ROOT / "port/android-native/app/src/main/AndroidManifest.xml").read_text(encoding="utf-8"),
        "landscape rotation must resize the retained Activity rather than restart its intro state")
require("onSurfaceTextureDestroyed(SurfaceTexture texture)" in view
        and "releasePlayer();\n        return true;" in view
        and "startIfReady();" in view,
        "TextureView surface recreation must release and prepare playback again")
require("setBackgroundColor(Color.BLACK)" in view and "View matte = new View(context)" in view
        and "matte.setBackgroundColor(Color.BLACK)" in view and "video.setOpaque(true)" in view,
        "20:9 cinematic side mattes must cover the native scene behind the centered video")
require("openingSequence.usesSplashStage())loadOpeningSplash()" in activity
        and "openingSequence.mayLoadMenu(android.os.SystemClock.uptimeMillis())" in activity
        and 'NativeBridge.loadOpeningSplash(getFilesDir().getAbsolutePath(),getAssets())' in activity,
        "intro uses the original loading stage and cannot initialize the main menu until splash completes")
require("int openingStage=inspectionMode?OpeningSequenceV1.MENU:" in activity
        and "getIntent().getBooleanExtra(\"skip_intro\",false)?OpeningSequenceV1.SPLASH:" in activity
        and 'getIntent().getBooleanExtra("skip_intro",false)?0L:' in activity,
        "explicit intro skip bypasses video while preserving the authored splash-before-menu stage")
require('language==4?"menus/splash_final_jp.tga"' in
        (ROOT / "port/android-native/app/src/main/cpp/original_ui_session.cpp").read_text(encoding="utf-8")
        and 'language==5?"menus/splash_final_kor.tga"' in
        (ROOT / "port/android-native/app/src/main/cpp/original_ui_session.cpp").read_text(encoding="utf-8"),
        "source language selects the recovered Japanese and Korean splash variants")
require("if(\"ui/original-main-menu\".equals(loadedAsset)&&!frontMenuReady)" in activity,
        "authored main menu becomes ready only after its source movie has been loaded and drawn")
require("mayApplyCinematicCompletion(isFinishing(),isDestroyed())" in activity
        and "mayStartTitleMusic(" in activity,
        "late completion cannot resurrect a destroyed Activity; title music waits for both gates")

print("PASS fresh launch: intro stays over opaque splash; source-languaged splash then authored main menu")
print("PASS recreation: pending flag/position retained; activity resume and TextureView surface reprepare")
