# Original frontend movie art adapter

The runtime uses the complete retained original `dqmenus.swf` graph, not Android first-frame placeholders or hybrid guessed atlas regions. Source roots are MainMenu497, EnterName95, SelectClass416 and StartGame459; native callback names and property paths remain compatible with the flow owner. `compose` traverses the actual source timeline graph, applying source labels, hidden paths and live text bindings from `flow::PresentationState`. `show` settles at the retained source tween endpoint. Default or selected Idle settles at the actual source STOP frame within its label interval. Name Accept69 stops at frame4, hiding its orange hover glow and revealing the original gold base button; selecting frame0 was the earlier color mismatch. Original icon labels select their exact source frames. `normalize_source_path` removes unnamed numeric placement segments when matching AS paths.

The original generic stage is1024×768. Source placement and field matrices preserve this, and composition normalizes once into the host's480×320 logical API. `project` maps that logical rectangle to the viewport; `source_point` applies the exact inverse for current button contours. General AVM-driven reflow remains a fidelity limit; the adapter is a projection of recovered source properties, not a full ActionScript interpreter.

## Exact embedded texture ownership

`source_texture_file(bitmap_id)` returns an owned original texture file under `art/assets/source_bitmap_ID.tga`. These files are extracted directly from the movie's DefineBitsLossless/DefineBitsLossless2 payloads. Bitmap0 uses a white1×1 texture for original solid geometry. Actual dimensions are retained: bitmap1 is2048×1024;8/52/62/431 are1024×1024;15/44/98 are512×512;448 is256×256. Color channels are unpremultiplied exactly when decoding source Lossless2 ARGB into conventional straight-alpha TGA.

The generic movie repeatedly exports different embedded bitmap definitions under the same `MenusGraphics` resource name. The retained external `MenusGraphics.tga` equals embeddedbitmap62, not bitmap8. Therefore an external filename alone cannot reproduce this source movie: it caused the earlier whole-atlas mosaic backdrop. Runtime texture routing uses each exact embedded definition, and normalizes UVs by its actual width/height.

The source bitmap8 contains the ornate name background visible in the original reference. Source bitmap1 contains real red keyboard circles, shift/delete/space artwork. Original solid shape contours contain keyboard lettering; the exporter uses Shapely constrained triangulation for actual holes in O/P/Q outlines, without generating replacement typed letters. `batch_colors` and `bitmap_ids` are parallel to current draw batches. Shapes, fields and hits traverse the same visible placement graph.

## Original text

English labels resolve exact entries from original menu, gameplaymenus and global symbol/English tables. Source word-wrap and multiline flags, font ID/height, field matrix, alignment, paragraph margins, color and bounds are retained. Font IDs5 and29 name `Fontin SmallCaps` and map to original cache `data/Fontin SmallCaps.ttf`. These source DefineFont3 tags lack HasLayout; stored source metrics remain zero and the host uses the loaded original font's actual metrics.

Dynamic profiles, slots, class selection and entered names remain owner supplied. Empty slots hide original profile branches according to the recovered flow. No placeholder saved character is invented.

## Provenance and visual limits

All three user-supplied original references were inspected. The Android variant's keyboard transforms and stone background failed that reference gate. The lead authorized selecting the complete original generic movie and its own embedded image definitions. `source_layout.json` records movie/text hashes, roots, extraction counts and exclusions; there are no hybrid shape rebindings in the current pipeline. Run `export_art.py` with Python and Shapely2.1+ to reproduce the source graph, legacy inspection data, textures and receipt.

General AVM callbacks, mask/filter behavior and viewport reflow are not fully reconstructed. The upper keyboard layer follows the supplied original reference and source flow policy; shift behavior is documented by flow. Original gold name Confirm comes from source Idle Stop4, with no asset substitution or tint. Original 3D scenery and avatars remain lead-owned.

Native full-viewport evidence is `MenuFlash2DCamera::Update` at0x42cd84 in `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0042/0042cd84.c`.

## Validation

Strict clang C++17 compilation passes for the adapter and generated graph. Runtime tests cover source compositions, labels, original background/keyboard/lettering presence, visibility, exact symbol resolution, complete triangle/color/bitmap routing, hits and projection. Link tests with original_art.cpp, original_art_data.cpp, compose.cpp, runtime_source_data.cpp and flow/presentation.cpp. Integration captures are owned by the lead and must still pass the original-reference gate.


## Rasterization evidence

Original `render_handler_glitch::set_antialiased(bool)` at0x7d3e00 is a no-op in the IDA export (`pseudocode/007d/007d3e00.c`). Original GameSWF fragment shaders sample texture, add diffuse, and multiply vertex color; they do not implement edge-coverage smoothing. The exporter curve tolerance is0.25twips (0.0125source pixels), so polygon approximation is already subpixel. Compare like-resolution captures before changing the keyboard's actual original vector geometry or inventing antialias state. Device TTF rasterization uses a separate native anti-alias flag and should not be confused with the source keyboard vector outlines.
