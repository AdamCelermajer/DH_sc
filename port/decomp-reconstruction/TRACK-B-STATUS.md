# Track B status

Updated 2026-10-13 by the integrator. Rules and decisions: `TRACK-B.md`. Totals: `py -3 tools/decomp_progress.py`.

## Totals (whole binary in scope: 25,218 functions)

| status | functions | share |
|---|---:|---:|
| reconstructed | 12,494 | 49.5% |
| upstream (Lua 5.1.4, Box2D 2.0.1, gameswf r1714, tinyxml, libjpeg, FreeType, Musepack) | 4,208 | 16.7% |
| partial | 5,018 | 19.9% |
| skipped | 3,148 | 12.5% |
| no ledger row yet (G1b in progress) | ~350 | 1.4% |

| category | queued | reconstructed | upstream | partial | skipped |
|---|---:|---:|---:|---:|---:|
| game-world | 1,527 | 983 | 0 | 449 | 92 |
| game-app | 1,409 | 784 | 2 | 360 | 65 |
| game-character | 1,143 | 866 | 0 | 230 | 46 |
| game-items-save | 831 | 618 | 0 | 103 | 66 |
| game-ui | 808 | 456 | 0 | 216 | 32 |
| game-anim | 365 | 211 | 0 | 124 | 30 |
| glitch-scene-collada-gui | 4,735 | 1,504 | 3 | 2,221 | 1,007 |
| glitch-video | 1,701 | 635 | 0 | 328 | 738 |
| glitch-core-io | 1,336 | 993 | 17 | 100 | 226 |
| net-session-online | 1,244 | 882 | 0 | 215 | 147 |
| net-transport | 1,048 | 807 | 0 | 128 | 113 |
| pydata-accessors | 2,273 | 2,197 | 0 | 57 | 19 |
| vox-audio | 1,140 | 914 | 45 | 60 | 121 |
| core-platform | 653 | 317 | 0 | 46 | 290 |
| tinyxml-and-c-libs | 1,846 | 12 | 1,755 | 3 | 76 |
| vendor-gameswf | 2,370 | 312 | 1,600 | 378 | 80 |
| vendor-box2d | 318 | 2 | 316 | 0 | 0 |
| vendor-lua | 471 | 1 | 470 | 0 | 0 |

At the start of Track B (`2482a90d`) the scope was 15,164 functions with 7,472 reconstructed.

## What "reconstructed" means today

The body follows the Hex-Rays control flow and side effects. It compiles in the unified tree. A Sonnet
verifier checked the three largest functions of each worker batch against Hex-Rays and the assembly, and a
batch that failed got one Haiku fix pass. Nothing has been executed against the original machine code yet.
`tools/trackb/armdiff` (in progress) will run original and reconstructed functions side by side under Unicorn.

## Waves done

| wave | what | result |
|---|---|---|
| setup | queue the whole game, one CMake program, tools | `6d4d454c` |
| L0, keystone | 44 foundation game headers (Sonnet) | all compile |
| E1, E2 | newly queued engine, net and pydata functions | 741 + 240 rows |
| L1, L2, L3 | 459 + 29 game class headers, header fixes | 482 of 488 compile alone |
| decisions | 12 engine decisions (binary wins), static C++ runtime | engine tests pass |
| vendor | Lua, Box2D and gameswf vendored, 2,130 rows upstream | `8124d46c` |
| R1, R1n | engine and net partial redo | 107 + 157 upgraded |
| G1a | 181 gameplay batches | 1,600 reconstructed |
| L3b | 867 layout requests applied (add and retype only) | `b70ac95e` |
| unify | net classes, game types (Point3D, streams, Structs): one definition each | `5fdf7cb3`, `3c1011ab` |
| V1 | Gameloft's gameswf, Box2D and Lua additions | 315 reconstructed |
| R2a | gameplay redo of unblocked partial rows | 464 upgraded |

## Running

- G1b: the last 200 gameplay batches.
- U1: canonical engine types (core-io, video, scene), one definition per class, with a map of every stand-in.
- The gameswf fork's core layouts (character, tu_string, allocator, scene_node).
- Net structure: CAndroidSocket base, CTransport slots, the GetTransportMgr singleton, DataPacket slots.
- armdiff: the ARM32 differential tester (NDK armv7a build plus Unicorn).

## Next

1. U2: replace every engine stand-in with its canonical type (from the U1 maps). Then redo the
   engine-blocked rows: scene 2,221 partial, video 328, and the game rows waiting on scene types.
2. Build repair until the whole tree builds and every test links (`build/tb-all`).
3. Platform layer: the JNI entry points (167 skipped rows) and the Android glue, required for an Android build.
4. Review the 866 skipped rows whose notes give no clear reason.
5. armdiff across every reconstructed function that makes no calls (~3,900), then the ones that make calls.
6. The rename and cleanup pass: the evidence-based member names collected by the waves, statics and signatures.
7. ARM64 Android build of the whole tree.
