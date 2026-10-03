# Source encounter checkpoint

The launcher now opens a bundled development encounter. It uses native ARM64
and x86_64 libraries built from the repository source. No original game engine
binary is loaded by this app.

## What you can try

Move with the left pad, approach the red sentries and hold **Attack**. Their
health decreases, defeated sentries disappear, and the selected original kill
objective advances to **2/2 COMPLETE**. Staying near a sentry allows its attacks
to reduce the player's health to **DEFEATED**. **Reset** starts a fresh encounter
and replaces its saved run. **Diagnostics** opens the earlier asset and script
testing screen.

In-progress and completed encounter state is saved to app-private storage and
restored after a cold process restart. These are authored development saves,
not files compatible with the original game. See
[persistence scope and verification](PERSISTENCE-INTEGRATION.md).

The room, warrior, textures and idle/walk/attack clips come from the supplied
cache. They are packaged during the build, so the encounter needs no file
imports. The room is one cross floor from the original `void_maze` module
catalogue; it is not a recovered complete level.

## Recovered and authored behavior

The original recovered combat formula runs in the source Lua runtime. Its
results use reconstructed property composition, health, damage, nonplayer death
and compiled kill-objective components. The encounter routes resulting death
events into the objective while it is active and incomplete.

Placement, movement, floor collision, camera, AI steering, attack targeting,
input and event orchestration are authored development integration. Two tuned
property rows are appended to an owned copy of the original property table;
all original rows remain intact. Death contexts retain the original objective's
property identifier. The player has 80 HP; each sentry has 18 HP. Sentries reuse
the warrior model with a red tint.

Horizontal movement in the original animations is removed from rendered poses
so the session controls the character position. This is not a claim that the
original animation controller has been reconstructed.

## Build

```powershell
python port/android-app/build.py --sdk <Android-SDK> --ndk <NDK> --cache <unpacked-cache-files>
```

The cache directory contains `data/3d` and `data/pydata`. The builder packages
only the listed encounter assets and records their hashes and constant-table
derivation in `build-validation.json` and the APK asset manifest.

## Remaining work

The original world/level lifecycle, full AI and animation controller, actual
enemy appearances, weapon attachments, inventory UI, rewards, campaign
progression and audio remain unfinished. The new save format covers only this
development room; it does not restore original campaign saves. This checkpoint
does not complete the source game rebuild.

The original-engine Test11 compatibility APKs are separate builds. Their
playable original game must not be confused with this source encounter.
