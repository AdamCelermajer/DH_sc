# Native gameplay camera integration

The visible emulator now runs the recovered gameplay camera in the Crypt
development world. Candidate APK: `dh2-native-camera-v20-6d1b7f73.apk`, SHA256
`6d1b7f73a4361a59a20083e808909ae1dd55b5075384ecd1c8fef0272e72c934`.
This is camera/UI acceptance, not an accepted complete combat checkpoint.
The prior full regression checkpoint remains `b3708a7e`.

## Actual changes

- The selected authored `x07_crypt_backup.mlx` is parsed through the loader's
  actual TinyXML service. Real LevelConfig constructor, declarations, defaults,
  overrides and InitPost select CameraTests/PlayerCamera_Default/Default with
  authored near/far 900/5000. Development Level transport retains the same
  initialized config; complete canonical Level/GS startup remains pending.
- One retained Application integration context owns SceneManager camera root
  membership, its first camera factory, ZoomHandler and animation directory.
  World sessions borrow these owners. GL/world replacement follows explicit
  zoom/root/active-camera, physical, animation-set and native-owner teardown.
- Camera follow reads the same player position and genuinely constructed NULL
  anchor field; physical2dc publishes the same native body. Actual PlayerManager
  records/counts are read, including the currently incomplete AddCharacter
  count producer. A missing positive anchor or other required service refuses.
- Source autozoom, target update, animator scene phase, view/projection and GPU
  depth conversion replace the former development orbit camera in gameplay.
  FX, geometry, enemy bars and combat text receive the same submitted view.
  World touch rays use the original engine frustum instead of inverting the
  GPU-adjusted matrix. The development generic click/movement tails remain
  required when reached.
- Modern display policy changes the SAME camera's aspect to the actual surface
  ratio after original Level.SetData, on camera publication/viewport changes.
  This intentionally overrides historical fixed aspect 1.66775239; 2400x1080
  uses 2.22222233. Authored vertical FOV, target, follow and zoom are preserved.
  This is a declared port adaptation, not original automatic resize parity.

## Verification and limits

Both ARM64 and x86_64 application builds succeeded. Only modern native ABIs are
packaged; the original 433,189,197-byte cache is inside this single APK.
Camera source successor differential proof covers 896 picking/projection and
160 offset cases. Actual-cache native composition verifies two World loads
through the same Application services and ownership-cycle checks.

Live PID28967 verifies normal main-menu Play into Crypt, source camera view
with the actual display aspect, and portrait-opened original Stats, Skills and
Inventory artwork. The inventory renders the same equipped avatar. Reviewed
screens/logs: `native-gameplay-camera-v20/04-character-stats`,
`05-character-skills`, `06-character-items`, `16-modern-camera-world`.
The transitional `03-world.png` still shows Single Player; it is deliberately
not used as world visual evidence. Initial install screenshots can precede UI
asset readiness; `15-main-menu-ready.png` is the settled main-menu capture.
Normal joystick movement was subsequently observed with actual body position
changing from (-22.2777,12.2093) to (-25.0939,14.5238), with the camera following.
That later process log also records required-provider errors from additional
menu interactions: ChangeFaery, NativeIsMultiplayerGame, NativeShowMinimapLegend
and NativeResetMapZoom remain unconnected. These extra tabs/actions are not
covered by the three core Stats/Skills/Inventory acceptance above. They are
preserved in `17-camera-follow-movement.log`, not hidden or called completed.

An earlier test PID28112 exposed the existing lethal-skill gap: Headsplitter
reduced enemy HP2827 to0, then required Hit service8 (Cmd_Kill) failed. Root's
development Hit adapter has no complete original kill/loot/XP/quest backend;
the frame exception currently tears down rendering and produces a black
screen. This candidate does not fix or accept lethal skills. Whole kill must
use real Level.word150=0 and genuine DropLoot/XP/event services, not a fabricated
loot gate, forced dead byte, skipped callback or successful empty service.

First Swamp gameplay, positive skybox, minimap/Overview input, complete scene
render registration, original Level/GS startup and physical-device acceptance
remain outstanding. The source-change ZIP records these camera integration
files only; it is not a complete standalone source build or a whole compiled
dependency capture. Later generic animation callback edits are not included
in the installed candidate and remain pending their coherent integration.
