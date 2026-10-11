# B072 Map page not as intended (fix/b072, build DH_wt/build-fix072)

## User symptom
Screenshot b072-map-page.png: black rotated parallelogram with blobs, saturated warm parchment showing the wrong region.

## Evidence
Video (Part 2, cIAW38IfLxY.mp4, 640x360 source; frames at t=58, 90, 155-170 as 2 fps sequences, scratch .local-inputs/claude-preview161/fix072/ref):
- Visited areas are flat dark slate irregular silhouettes (sampled RGB ~ (38,36,38)) of the walkable floors over the parchment (t=90, 163). Not module visuals, no black room fill.
- Sequence t=160-172: the user drags the map (pan, constant scale); markers (red enemies, cyan player, red "!" NPC, arches) clipped at the rectangle edge.
- Parchment region/scale: landmarks (crosses, tower, arches, lake) are matched by texel region x 291-981, y 329-674 of the 1024 px map_bottom.tga (ours showed a 2x smaller region). Parchment is dimmer/desaturated: mean RGB (91,81,63) vs texture region (183,142,74).
IDA:
- SceneManager::AddNodeToMap 0x3524a0; PFWorld floor loader (pseudocode-all.c 377589-377625): for every `_floor` node, AddNodeToMap(last `_minimap` node of the module if one exists, else the floor node). Scene +652 = the map node (shown by MenuCharMenu_Map::Show, hidden by Hide 0x4537c0). Swamp modules have no `_minimap` nodes (checked swamp.bdae), so the floors are the map geometry.
- MenuCharMenu_Map::Show 0x455138: per scene child visible only if its bounds centre is inside a visited RoomZone (IsInsideRooms 0x453be8). RenderMap 0x454108 renders that scene through the map camera; UpdateMapCamera 0x45310c clamps pan; ZoomHandler::onEvent 0x382040 (touch pan x50, zoom event 7); ResetZoom 0x38201c.
- SWF sprite 655: layer 602 at depth 5 has clip_depth 10 -> icons are clipped to the map rectangle.

## Root cause and change
1. The page drew every visited module's visual ranges with vertex colours (baked dark) -> black module-sized fill. Now OriginalScene::mapMesh (original_scene.cpp) collects `_minimap` else `_floor` helper geometry per module; assembled_level keeps per-zone map ranges; main.cpp draws those flat slate (0.15,0.14,0.15).
2. Parchment UV used the 1024 px texture size; the SWF fill matrix addresses a 512 px bitmap -> region now matches the video landmarks.
3. Parchment tint multiply (0.50,0.57,0.85), measured from video; cause in the original is unknown (labelled in code).
4. Icons clipped to the rectangle (Sutherland-Hodgman with UV) and kept up to 80 px outside it so edge markers show partially, as in the video.

## Verification
- Unit: level_module_zones (every Swamp and Darkwood zone has map ranges, contiguous, centroid inside room box), map_visit (projection margin). ctest: all pass except the known session_skill_binding; `save`/`schema_v4` fail only when run in parallel and pass alone.
- EXE (quiet run, rc3 Swamp assets, `--map-page-frame 150`): before (Preview 16 exe) out/before-f9.png vs after out/after-f9.png, out/after-fresh.png. After shows floor silhouettes on a dim parchment like the video t=58/90.

## Verified vs not
Verified against video frames: slate silhouette style, parchment region and tint, icon clipping, marker styles, pan behaviour. Verified in IDA: floor/_minimap map node source, room visibility, clamp, clip layer.
NOT verified: orientation (up (-1,1,0), IDA constant; no original Swamp map frame exists, only Darkwood which cannot load in game); exact scale/zoom limits (PC range 1..4 adaptation); wheel/drag/key input glue not driven in the EXE (quiet runs inject no input) although the math is unit tested; why the original is dimmer; the slate shade comes from one video sample.
Other marker rules (NPC, exits, objectives) were done by the earlier MAPMARKERS stream and not re-audited here.

## Package files required
data/3D/textures/map_bottom.tga, data/3D/camera/minimapcameras.bdae (as before). No new files.
