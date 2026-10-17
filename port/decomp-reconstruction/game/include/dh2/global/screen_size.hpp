// Screen size globals (<global>), declarations only. Batch g1-game-ui-pack3-9e17e4-0 reads both: the MenuFlash2DCamera
// constructor halves them for the camera centre. Width_Screen is also declared in global/MenuManagerGlobals.hpp (same
// symbol, same type); the two declarations agree. The definitions belong to the category that owns the values and are
// not in the tree yet.
#pragma once

// Symbol Width_Screen, 4 bytes (.data, 0x99b114): the screen width that the menu code compares with 854, 960 and 800.
extern int Width_Screen;

// Symbol Height_Screen, 4 bytes (.data, 0x99b118): the screen height, the word after Width_Screen.
extern int Height_Screen;

// Port addition (UI2, 2026-10-17). DOCUMENTED DEVIATION from the original's stretch (TRACK-B.md decision 15): the original only knew 800/854/960 wide screens and otherwise 1280x720; the menu movies (4:3 stage with
// non-uniform authored scales) and the level camera (aspect 1.6677) are designed for a 16:9 screen and the binary stretches them to
// whatever the surface is. On a surface of another aspect (21:9 phone, square foldable) that stretches everything. These helpers give the
// 16:9 design area and the factor that keeps the 3D cameras undistorted (vertical field of view unchanged, more or less world at the sides).
#if defined(__ANDROID__) || defined(DH2_HOST_RUNNER)
inline bool ScreenUsesAspectFit() { return Width_Screen > 0 && Height_Screen > 0 && Width_Screen != 800 && Width_Screen != 854 && Width_Screen != 960; }
// real aspect / 16:9; 1.0 on a 16:9 surface and on the original's own screens
inline float ScreenAspectScale() {
    return ScreenUsesAspectFit() ? (static_cast<float>(Width_Screen) / static_cast<float>(Height_Screen)) * (9.0f / 16.0f) : 1.0f;
}
#else
inline bool ScreenUsesAspectFit() { return false; }
inline float ScreenAspectScale() { return 1.0f; }
#endif

// Offset (surface pixels) of the centred 16:9 Flash box that MenuFlash2DCamera::Update fits the movies into; 0 when the fit is off or the
// surface is 16:9. Code that converts between surface pixels and stage coordinates by hand (3D panes inside the movies, world-anchored Flash
// clips) adds / subtracts it: the original stretched the stage over the whole surface, where the offset is 0.
inline int ScreenFitOffsetX() {
    return (ScreenUsesAspectFit() && Width_Screen * 9 > Height_Screen * 16) ? (Width_Screen - Height_Screen * 16 / 9) / 2 : 0;
}
inline int ScreenFitOffsetY() {
    return (ScreenUsesAspectFit() && Width_Screen * 9 < Height_Screen * 16) ? (Height_Screen - Width_Screen * 9 / 16) / 2 : 0;
}
// Rescales the active 3D camera after a surface resize (game-world/src/g1-game-world-CameraBase-b32e3c-0.cpp).
void ScreenAspectRescaleActiveCamera(float oldScale);
