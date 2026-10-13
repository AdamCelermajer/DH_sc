// Screen size globals (<global>), declarations only. Batch g1-game-ui-pack3-9e17e4-0 reads both: the MenuFlash2DCamera
// constructor halves them for the camera centre. Width_Screen is also declared in global/MenuManagerGlobals.hpp (same
// symbol, same type); the two declarations agree. The definitions belong to the category that owns the values and are
// not in the tree yet.
#pragma once

// Symbol Width_Screen, 4 bytes (.data, 0x99b114): the screen width that the menu code compares with 854, 960 and 800.
extern int Width_Screen;

// Symbol Height_Screen, 4 bytes (.data, 0x99b118): the screen height, the word after Width_Screen.
extern int Height_Screen;
