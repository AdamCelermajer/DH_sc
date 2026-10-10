# Frontend presentation readiness

## Main menu and opening scenes are separate

The original MainMenu scene is `models/main_menu_charactere_swamp.bdae` (SHA-256
`8a3462863a3d6ca2255d96989d53a21cddfba821e42c8a13e676f10e51d1300b`). Original
`MenuMainMenu::SetupScene` at `0x42c510` loads
`data/3d/menu/main_menu_charactere_swamp.bdae`, constructs that Collada scene,
and adds the same scene node to SceneManager. The platform title-intro movie is
the separate `original-media/intro.mp4` asset (SHA-256
`859f2dde32401f996a5faeaf8c914965b5410d0200315c0bfb90d8c71aa59b22`). Its
Android lifecycle requests the video independently. The user's original main
menu reference at `.local-inputs/dh2-final-mainmenu.png` (SHA-256
`6596b570ea1e44ad4d0b354b036d8702f96de6b88cfe36a77c9a3f4db7ee700b`) shows
the MainMenu scene with an equipped Warrior and profile UI; it is not a frame
from either video. The Act 1 Swamp conversation cinematic is separately visible
at `.local-inputs/referenceframes/dh2-act1/at-0120s.png` (SHA-256
`8d2417a8ee0b7ed754bb1bc1de1d375b2d961d9f581c0af17bfd5d89fb3a5251`): Rene is
at a cage with the player character, torchlit stonework, dialogue and skip UI.
That is a different authored presentation from the outdoor Boglands MainMenu
stage with its seated statue and profile UI; the cinematic is not a valid
MainMenu visual target. The direct-Swamp launcher remains unchanged.

## Available preview and exact blockers

The callable Windows preview currently lives in `features/frontend/interactive.cpp`.
Its Main/Start branch submits the original swamp background plus SWF overlay;
it does not submit the saved-slot Character. Its Class branch uses the separate
`CLASS_SELECTION` scene and temporary `CreationPreview` CharacterVisual actors.
The class projection retains the base camera aspect `0x3faaaaab` (4:3). The
authored Collada aspect `1.5` is only used in the horizontal-to-vertical FOV
conversion. The earlier evidence text implying viewport-derived Class camera
aspect was stale and has been corrected.

The idle interface is an optional callback seam, not a bound native owner. At
the source OnSelect endpoint, the preview still needs the same class
Character/NativeFsm24/CharacterStateOwner path to deliver CSAnim14 event34 into
`SM_SetIdleState(false)`, with the same Gear/stance facts and ordinary-idle
animator. The temporary preview actor cannot stand in for that Character.

The light inputs expose the authored `Omni01` and original setter values, but
the source owner contracts are not connected to a live menu. The remaining
provider must borrow the same menu SceneManager's first `lght` node, spawn the
type-19 `LightPoint` through that ObjectManager, publish PlayerLight slot 0 and
the Application tweaker, append the same automatic light, then route the same
CLight to the renderer's material-uniform path. Asset-decoded light metadata
alone does not light the scene.

Main-menu actor acceptance requires `MainMenu.mSavegameSlot` to reach the same
actual `CreatePlayer`/canonical Character owner, finish the complete Character
InitPost and InitFinal lifecycle, then submit its source visual packets. A Gear
record or the bounded `31875` construction proof does not establish the full
InitPost continuation. Current native host callbacks do not provide a completed
actor lifecycle or saved-avatar render submission.

No presentation capture was made: none of these actual providers changed in
this lane, and another static-scene capture would not resolve an acceptance
boundary. The next capture should follow a real provider binding and use the
same retained source frame for update and render. No floor geometry, tint, or
camera offset was changed.
