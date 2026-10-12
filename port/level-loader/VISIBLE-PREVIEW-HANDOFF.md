# Visible loader preview

The private `DH2_Loader_API37` emulator uses `emulator-5590` and the app
`local.dh2.loader`. The launcher now shows its window by default; `--headless`
is an explicit option. `--restart` checks the running AVD identity before
stopping it and preserves its data. Other sessions' emulators are untouched.

The map picker contains the 16 fixed maps whose native assembly passed the
fixed-map coverage report. `build_preview.py` generates its catalog from that
report, preserving the original level identity and MLX definition. Pick a map
and press **Load map**. Drag to orbit, use **Zoom + / Zoom −**, **Next module**,
and **Whole map** to inspect it. **Reload** reloads the current successfully
loaded map; the failed-load check retains it.

Swamp, Red Desert Hub, and Icy Hub were switched through the picker in the same
running app and produced native successful frame logs and nonblank screenshots.
Module focus, zoom, failure retention, and reloading the selected map were
checked. Swamp is left open. Evidence is in `reports/visible-preview-checkpoint.json`.

This preview renders map geometry and materials. Mobs, chests, procedural
layouts, campaign transitions, and gameplay saves are still pending. The
procedural native readers are separate milestones; they are not exposed as
finished procedural map rendering by this picker.

Build: `python port/level-loader/tools/build_preview.py`.
Launch/restart the loader's own visible emulator:
`python port/level-loader/tools/start_preview_emulator.py --restart`.
After boot, install and launch with `preview_device.py install` and
`preview_device.py restore_swamp`. All device actions verify its AVD identity.
