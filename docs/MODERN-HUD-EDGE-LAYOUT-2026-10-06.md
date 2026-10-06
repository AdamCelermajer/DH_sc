# Gameplay HUD uses the whole display

The original HUD was rendered inside its centred 480x320 stage. On the
2400x1080 emulator that left a 390-pixel gap at each side, pulling the
portrait, movement and action controls inward. The modern layout now uses
the source viewport's expanded rectangle to anchor those groups to the actual
display edges, preserving the original artwork, scale and touch geometry.

Player bars, portrait and pause move together toward the left edge. Potion
moves toward the right edge. Movement and actions retain their original side,
including the mirrored HUD styles. Duplicate deadzone instances follow their
actual group; enemy and ally bars retain their world projection. Music and
status captions retain their own alignment. No shared parent containing both
left and right controls is moved.

Each draw and shape hit-test temporarily translates the actual source nodes,
then restores their current matrices. Source animation, repeated onPush,
joystick displacement and resize therefore do not accumulate offsets. The
development button no longer covers the potion and is hidden on character
screens.

Actual-cache tests pass all four HUD styles at 480x320, 2400x1080 and1024x768,
including genuine moved shape hits, old-position rejection and exact matrix
restoration on success, failure and exception. Platform/font/texture fixture
boundaries are documented in the host receipt. Both Android ABI builds pass.
Live5554 verifies portrait opening, real joystick movement and normal Attack
at the new positions. The attack test also exposed a legacy mesh-only effect
route; it now uses the existing general composite factory and submits the
actual gradual material without a black frame.

Checkpoint:
`port/android-native/build/checkpoints/dh2-native-hud-edges-29d2d5b5.apk`
SHA25629d2d5b504589cbe37a7d79650b7963474573b89b05903b4ce3c7bbe32e1f098.
Receipt: `port/android-native/reports/hud-edge-layout-v6/live-receipt.json`.
The visible emulator is left in gameplay, screenshot167, PID16514.

This changes HUD placement, not the development camera. The reference video's
player framing moves off-centre during combat, whereas the current camera
immediately centres the actor's X/Y. Original camera constructor, focus and
update recovery is a separate active investigation; no arbitrary horizontal
offset was added. Complete Swamp gameplay and physical-device tests remain
unfinished.
