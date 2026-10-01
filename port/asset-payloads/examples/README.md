# Real small exports

These files were produced by the new C++ decoder through `tools/export_assets.py`, using the recovered `data/3d/animateddecors/candle_flame.bdae`.

- Input SHA-256: `7ece047e6cb253036d50260018fd7299ad9835e743ca0fcd4e2d3004ea0269a6`.
- [candle-flame.obj](candle-flame.obj): two local-space meshes, four triangle faces, normals, UVs and original material names. Textures, material files and scene transforms are not included.
- [candle-flame.animations.json](candle-flame.animations.json): both animation tracks, exact getter time values in milliseconds and raw stored float components. Track application/interpolation and scene binding are not implied.

Reproduce from this module's directory after building:

```sh
python tools/export_assets.py /path/to/cache/files/data/3d/animateddecors/candle_flame.bdae \
  --obj examples/candle-flame.obj \
  --animation-json examples/candle-flame.animations.json
```

These are derived game data, not newly licensed artwork or a standalone game. Original provenance and repository rights notes apply.
