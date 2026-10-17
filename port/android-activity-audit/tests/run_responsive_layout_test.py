from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest


REPO = Path(__file__).resolve().parents[3]
ANDROID_SRC = REPO / "port/android-app/src/local/dh2/sourceviewer"
ACTIVITY = ANDROID_SRC / "MainActivity.java"
POLICY = ANDROID_SRC / "ResponsiveLayoutPolicy.java"
JAVA_TEST = Path(__file__).with_name("ResponsiveLayoutPolicyTest.java")


class ResponsiveLayoutSourceTest(unittest.TestCase):
    def test_controls_are_scrollable_and_layout_tracks_window_size(self):
        source = ACTIVITY.read_text(encoding="utf-8")
        self.assertIn("ScrollView controlScroll = new ScrollView(this)", source)
        self.assertIn("controlScroll.addView(controls", source)
        self.assertGreaterEqual(source.count("controls.addView("), 15)
        self.assertIn("panes.addOnLayoutChangeListener", source)
        self.assertIn("updateResponsiveLayout(right - left, bottom - top)", source)
        self.assertIn("ResponsiveLayoutPolicy.choose(widthPx, heightPx, density)", source)
        self.assertIn("layout.addView(panes", source)

    def test_padding_is_density_independent(self):
        source = ACTIVITY.read_text(encoding="utf-8")
        self.assertIn("layout.setPadding(dp(14), dp(14), dp(14), dp(14))", source)
        self.assertNotIn("layout.setPadding(14, 14, 14, 14)", source)


def run_java_policy_test():
    java_home = os.environ.get("JAVA_HOME")
    if java_home:
        javac = Path(java_home) / "bin" / ("javac.exe" if os.name == "nt" else "javac")
        java = Path(java_home) / "bin" / ("java.exe" if os.name == "nt" else "java")
    else:
        javac = Path(shutil.which("javac") or "")
        java = Path(shutil.which("java") or "")
    if not javac.is_file() or not java.is_file():
        raise RuntimeError("JDK javac and java are required to run the layout policy test")
    with tempfile.TemporaryDirectory(prefix="dh2-responsive-layout-") as temp:
        subprocess.run(
            [str(javac), "-d", temp, str(POLICY), str(JAVA_TEST)],
            check=True,
        )
        subprocess.run(
            [str(java), "-cp", temp, "local.dh2.sourceviewer.ResponsiveLayoutPolicyTest"],
            check=True,
        )


if __name__ == "__main__":
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(ResponsiveLayoutSourceTest)
    result = unittest.TextTestRunner(verbosity=2).run(suite)
    if not result.wasSuccessful():
        raise SystemExit(1)
    run_java_policy_test()
    print("Responsive layout policy passed host-JVM scenario checks.")
