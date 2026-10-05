v53: retain the native surface across size changes

The rapid Name-menu Back/resize path previously produced a blank Android presentation. resize-back-v53 captures this: native SWF visibility, frame progression and viewport were valid, and GLES readback contained the menu pixels, while Android screenshots were blank. Returning to the physical size could restore presentation. This is evidence of a surface-presentation/lifecycle problem; the specific Android compositor fault is not established. SurfaceSyncGroup timeouts were logged.

MainActivity now handles orientation, screenSize, screenLayout and smallestScreenSize configuration changes. The existing GLSurfaceView and native retained graph resize through onSurfaceChanged. No Android density or locale handling was overridden. onConfigurationChanged records the retained activity identity. A read-only inspect-front probe exposes graph visibility/positions/frame numbers and GPU pixels/binding/viewport for diagnosis.

resize-back-v53-fixed repeats the real failing path: actual name entry, Back, size change after 0.1 seconds, 2184x1968 presentation, then physical 2400x1080 restoration. Visible center colors >3200 at every captured stage, one GL initialization and retained-configuration callbacks. Earlier blank captures and GPU evidence remain preserved. front_resize_transition_smoke.py is the reproducible test in the source handoff.

class-scene-tests-v53-retained verifies the exact APK at three surfaces, all seven original camera clips, 27 animation tracks, class boundaries, transition input locking, Back, retained Rogue selection on resize, and Home/resume in the same process. This does not certify arbitrary resolutions or complete characters/lighting/game creation. Keyboard actions were tested on v52; its assets are unchanged. Current v53 screenshots also show the repaired keyboard on Back. The full menu objective remains active. Main checkout, main session and emulator5554 were untouched.

APK SHA256: 101f4bb03da60a4a1848d62b9329f17fb3622559009dce376eabcbe057251ae4
