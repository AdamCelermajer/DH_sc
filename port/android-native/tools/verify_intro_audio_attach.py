from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
audio = (ROOT / "port/android-native/app/src/main/java/com/example/dh2/FrontAudio.java").read_text()
activity = (ROOT / "port/android-native/app/src/main/java/com/example/dh2/MainActivity.java").read_text()
cinematic = (ROOT / "port/android-native/app/src/main/java/com/example/dh2/IntroCinematicView.java").read_text()
gradle = (ROOT / "port/android-native/app/build.gradle.kts").read_text()
smoke = (ROOT / "port/android-native/tools/menu_ui_runtime_smoke.py").read_text()

start = audio.index("void setCinematic(IntroCinematicView view)")
end = audio.index("private MediaPlayer create(", start)
attach = audio[start:end]
assert attach.index("cinematic=view;") < attach.index("view.setMusicVolume(musicVolume);")
assert attach.index("view.setMusicVolume(musicVolume);") < attach.index("view.setAudioFocusGained(focused&&resumed);")
assert "introCinematic.startPlayback(openingCinematicPositionMs)" in activity
assert activity.index("frontAudio.setCinematic(introCinematic);") < activity.index(
    "introCinematic.startPlayback(openingCinematicPositionMs)")
assert "onPause(){if(introCinematic!=null)introCinematic.pausePlayback()" in activity
assert "if(introCinematic!=null)introCinematic.resumePlayback()" in activity
finish_start = activity.index("private void finishOpeningCinematic()")
finish_end = activity.index("private void startTitleMusicIfReady()", finish_start)
finish = activity[finish_start:finish_end]
assert finish.index("mayApplyCinematicCompletion(isFinishing(),isDestroyed())") < finish.index("startTitleMusicIfReady();")
assert 'original-media/intro.mp4' in cinematic
assert "skipButton.setOnClickListener(v -> finish(true));" in cinematic
assert "skipPolicy.onMovieTap(currentPositionMs())" in cinematic
assert 'Opening cinematic skipped by user' in cinematic
assert "'intro_skip': (240, 160)" in smoke and "'--skip-intro'" in smoke
assert "opening cinematic skip-button callback" in smoke
assert 'noCompress += listOf("wav", "mp4")' in gradle
print("PASS: intro attaches current audio state, stays seekable, and exposes the source-timed skip control")
