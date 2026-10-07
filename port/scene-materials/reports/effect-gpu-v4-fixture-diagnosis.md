# Effect GPU fixture zero-pixel diagnosis

The source shaders and authored material state are correct in the reached
fixture. The test supplied clockwise triangles to an authored BACK/CCW pass
and never initialized its depth attachment, despite the source pass enabling
LEQUAL depth testing. Both failures can reject all fragments without GL errors.

Isolated diagnostic ran original material/shaders across both fixture windings
and controlled cull/depth/blend modes. Source state plus `glClearDepthf(1)` /
depth clear and CCW geometry produces FIRE RGBA204,102,26,255 over all1024
pixels, confirmed by root's Android receipt:
`port/level-world/reports/android-native-owner-tests/effect-gpu-diagnostic-v4/receipt.json`.

The main `tests/effect_gpu_v4.cpp` fixture now requests EGL_DEPTH_SIZE24, uses
CCW indices0,1,2/0,2,3, and clears COLOR|DEPTH with depth1 before the original
pass. No production renderer, shader, blend, cull or depth state was changed.

Parent-authorized child main runner invocation failed at its first adb push
before compile/runtime. Root must execute `.local-inputs/root_effect_gpu_test_v4.py`
with its available emulator access to obtain the corrected main-test receipt.
Diagnostic completion is not a claim of whole live particle rendering or texture
owner acceptance; that test uses a declared white-texture fullscreen fixture.
