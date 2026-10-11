# Local Texture Lab

Background texture enhancement through Comfy MCP and Blender MCP. This tool captures the assembled model, runs local FLUX.2 Klein material edits, projects the edited/reference color ratios onto unchanged geometry, and bakes a separate atlas in the original UV layout. It supports character parts, armor, weapons and other UV-mapped models through JSON recipes. It never replaces game assets or uses mouse/keyboard automation.

## Run

Start Blender with its MCP server on port 9876 and ComfyUI on port 8188. The locally installed Comfy MCP environment must contain the Python MCP SDK. Required native models are `flux-2-klein-4b-fp8.safetensors`, `qwen_3_4b.safetensors`, and `flux2-vae.safetensors` in the normal Comfy model folders.

```powershell
tools/texture_lab/TextureLab.cmd all path/to/asset.recipe.json --new-run
```

Stages can also be run separately: `capture`, `enhance`, `project`, `bake`. `finish` resumes atlas compositing from an already saved raw bake and mask. Enhancement resumes the exact recorded MCP jobs instead of queuing duplicates. `--new-run` creates a unique name/output and saves `recipe.resolved.json` for later stage calls. Existing capture destinations are rejected. Blender must retain the captured scene while subsequent stages run.

The output contains reference renders, exact API workflows, MCP job receipts, enhanced views, coverage counts, an edit mask, a candidate atlas, baked-model renders, and `candidate.blend`. **Judge the baked-model renders**, because a good generated image can still map poorly.

## Recipe

Paths resolve relative to the recipe file. Two input routes are supported:

- `source_scene` plus `objects`: copy chosen meshes and their materials from the running Blender file.
- `source_obj`, `atlas`, and optional `obj_include`: import an isolated original OBJ using the supplied atlas. `obj_rotation_degrees` corrects the OBJ import coordinate convention; `[0,0,0]` is used for the recovered DH2 local Z-up geometry.

```json
{
  "name": "sword-detail-01",
  "source_obj": "sword.obj",
  "obj_rotation_degrees": [0, 0, 0],
  "atlas": "weapons.png",
  "output": "runs/sword-detail-01",
  "resolution": 1024,
  "atlas_size": [2048, 2048],
  "view_angles": [0, 90, 180, 270],
  "prompt": "Improve only existing painted metal and leather surface details. Preserve exact silhouette, colors, design, edge positions, framing and lighting. Classic hand-painted fantasy materials, fine scratches and leather grain, no redesign.",
  "seed": 424242
}
```

`view_angles` are degrees around world Z, with zero looking from negative Y. The camera frames mesh bounds automatically. `view_elevation` adds a vertical component. `camera_overrides` can supply exact `location`, `target`, and `ortho_scale` per view for repairs.

`protect_objects` excludes listed source objects from paint transfer; their baseline atlas pixels remain protected by the baked coverage mask. `view_regions` restricts a view to rectangles `[x0,y0,x1,y1]` in normalized image coordinates **with Y increasing upward**. `region_feather` softens borders. This is how the eye-only repair is kept separate from outfit work.

`uv_map` selects the source bake UV layer (default `UVMap`); `uv_scale` can normalize known preview UV transforms per source object. The source model must already display the correct original texture. Raw recovered OBJ files omit material transforms and rigs; inspect the reference before enhancing a new asset.

`ratio_min`, `ratio_max`, and `strength` limit how far colors may change. A lower strength preserves more source paint. The defaults are 0.2, 4.0 and 1.0. `canonical_bake_half` can select one local-space half for known mirrored UV regions, e.g. `{"Head": {"axis": 0, "keep": "positive"}}`. This is an explicit per-asset choice, not automatic overlap detection.

## Scope and verification

Current build processes one atlas per recipe; split models with multiple unrelated atlases into separate material/mesh runs. Shared or mirrored UVs cannot represent independent paint for both sides. The tool blends camera projections, rejects zero visible coverage, records source geometry/UV fingerprints, checks original inputs for changes, and composites only covered pixels over the baseline atlas. This preserves unpainted atlas regions; it does not certify seam quality automatically.

Local verification covers the basic player, its narrow-eye repair, an original 66-triangle longsword, and a separate 232-triangle armor torso. Generated edits can still change ornaments, introduce marks, or disagree across views. Inspect front/back/side baked results before accepting an art upgrade. No rig export, BDAE rebuild, animation change, or game-runtime replacement is performed.

Environment overrides: `TEXTURE_LAB_MCP_HOME`, `TEXTURE_LAB_COMFY_SHARED`, and `COMFY_LOCAL_URL`. The Windows launcher uses the same isolated environment as the installed Comfy MCP. No model download is performed by this tool.
