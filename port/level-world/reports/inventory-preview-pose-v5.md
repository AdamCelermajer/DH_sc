# Inventory preview initial pose publication

The preview now calls the existing `prince_visual.update_world(current_scene,error)` immediately before same-Gear draw extraction. This derives graph and instance world matrices from the sole live player root and currently retained pose. No clip sample, timeline advance, gameplay update, animation callback, RNG call or actor clone is introduced.

The initialization path binds the scene, compiles playback, and assigns the root position/scale/rotation. BlendedPlayback::start selects playback but the matrix publication is in its later animate/scene_phase path. The preview previously fetched parts with their earlier scene matrices and then multiplied by the inverse of the already initialized root. The resulting placements could be outside the source avatar camera on the first menu opening. A gameplay frame subsequently publishes the matrices, explaining the working reopen path.

Recovered RenderCharacterPane452578 camera aspect, temporary root origin, Euler preview rotation and rendering/restore sequence remain unchanged. The modern adapter keeps the actual root unchanged and rebases submitted placements into that source camera. It must first use placements containing the same root it removes. The new call makes that precondition explicit and repeats safely after equipment changes, restoring a GL context or opening the pane before the first world frame.

Validation: complete ARM64 renderer syntax PASS; isolated source-owner matrix regression PASS69 checks, including fresh unpublished root, repeated publication, retained root bytes unchanged, current node-pose update and malformed binding rejection without matrix writes. Receipt: `android-native-owner-tests/inventory-preview-pose-v5/receipt.json`. The native fixture isolates the matrix owner; it does not claim actual BRES/Gear/GPU visual acceptance.

Live verification remains with root: load world then immediately open Stats→Skills→Inventory before movement, compare avatar against post-frame reopen, equip/unequip and reopen, and repeat after context recreation. No package build, installation or emulator input was performed in this lane.
