# Host renderer diagnostic

This isolated check draws the first primitive of a real BRES candle mesh in Chrome WebGL1. It calls the checked C++ BRES/mesh views, compiles original GLSL from `shaders.pak`, draws an indexed triangle list, reads pixels back and saves a PNG and JSON report. Two modes are available:

- `flat` uses original `UnlitMaterialColorVP.glsl` and `UnlitMaterialColorFP.glsl` with a diagnostic orange color.
- `textured` reads the primitive's UVs and its `Material__12` diffuse-sampler link to BRES image index 2, resolves the exact `env_crypt.tga` cache filename, decodes its PVRTC pixels through `port/texture-assets`, and uploads those RGBA8 pixels to a WebGL texture. It uses original `UnlitOneTextureAndVertexColorVP.glsl` and `UnlitTexturedFP.glsl`.

From the repository root on Windows, with Python 3, a C++17 compiler and Chrome or Edge:

```powershell
python port/renderer-preview/run.py --fixture ../cache/files/data/3d/animateddecors/candle_flame.bdae --shaders ../cache/files/shaders.pak
python port/renderer-preview/run.py --fixture ../cache/files/data/3d/animateddecors/candle_flame.bdae --shaders ../cache/files/shaders.pak --mode textured
```

Outputs go to the ignored `port/renderer-preview/build/` directory. Flat mode writes `preview.png`/`report.json`; textured mode writes `preview-textured.png`/`report-textured.json`. Each PNG is the actual WebGL canvas. Both draws produced 53,185 non-background pixels; textured readback had 6,460 distinct RGB colors. The generated HTML files can be opened for inspection.

These are local-space diagnostics, not a faithful game renderer. The textured mode follows a checked diffuse image-index link, but its unlit shader choice, white vertex color, linear filtering, clamped texture edges and lack of blending or lighting are diagnostic choices. It does not reproduce the original effect program, scene transforms, animation, texture orientation rules, Android lifecycle or gameplay. The original archive, BRES and texture files are separate owner-supplied inputs; no game artwork is embedded in these source files.
