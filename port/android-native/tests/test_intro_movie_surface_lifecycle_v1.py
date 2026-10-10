"""Regression for intro playback across SurfaceView and Activity lifecycle changes."""
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[3]
JAVA = (ROOT / "port/android-native/app/src/main/java/com/example/dh2/IntroMovieV119.java").read_text(encoding="utf-8")


class IntroMovieSurfaceLifecycleTests(unittest.TestCase):
    def test_prepared_and_surface_created_share_valid_surface_start_gate(self):
        prepared = JAVA.split("player.setOnPreparedListener(", 1)[1].split("player.setOnCompletionListener", 1)[0]
        surface_created = JAVA.split("public void surfaceCreated(", 1)[1].split("public void surfaceChanged", 1)[0]
        start = JAVA.split("private void startIfReady()", 1)[1].split("private static void close", 1)[0]

        self.assertIn("startIfReady();", prepared)
        self.assertIn("startIfReady();", surface_created)
        self.assertIn("||started", start)
        self.assertIn("!surface.getHolder().getSurface().isValid()", start)
        self.assertIn("player.start();started=true", start)

    def test_surface_and_activity_pause_only_pause_actual_playback(self):
        surface_destroyed = JAVA.split("public void surfaceDestroyed(", 1)[1].split("});", 1)[0]
        pause = JAVA.split("void pause()", 1)[1].split("void resume()", 1)[0]
        resume = JAVA.split("void resume()", 1)[1].split("void skip()", 1)[0]

        self.assertIn("if(started){player.pause();started=false;}", surface_destroyed)
        self.assertIn("if(started&&player!=null", pause)
        self.assertIn("startIfReady();", resume)


if __name__ == "__main__":
    unittest.main()
