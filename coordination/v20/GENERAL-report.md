# v20 GENERAL report (branch v20/general, from v20/integrate 09a1bb68)

Scope: the audit blockers H7 (Buff_Rene_Haste literal), H8 (hand-transcribed moth OnOpen), H1-H3 (forced launch args), and cheap Act-1 literals (item 4). Evidence labels: [src] code/data read, [test] isolated unit test, [run] quiet hidden run, [doc] historical report.

## 1. Investigation

**H8 (container OnOpen).** The authored OnOpen bodies are `data/scripts/objects/*.luac`. They are plain Lua source despite the extension [src]. `moth_spawn_container.luac` is `local prob_spawn = 25; calc_spawn = GetRand(0,100); Trace(...); if calc_spawn < prob_spawn then Summon(GetPyOID("CharacterTable","Swamp_Moth_Minions"), true, 0.0, 0.0, 0.0, true) end` [src]. The compiled `*_pyscripts.bin` banks do not contain these bodies. They only hold the object name (`game_objects_pyarray.bin` references `moth_spawn_container`) [src]. So the "generic interpreter" for this path is the original Lua runtime (`port/script-runtime`, dh2_script_vm). In the live EXE that runtime was not linked before this change (only tests used it) [src]. The earlier hand-transcribed contract was a C++ mirror of this file [doc: CONTAINERS2-report].

**H7 (haste).** `data/scripts/ai/rene.luac` line 25: `local OID_HASTE = GetPyOID("ClassTable","Buff_Rene_Haste")`. CatchUp (lines 97-100): `CreateBuff(OID_HASTE,0,false,0)`, `ApplyBuff(OID_HASTE, buff)`, `EnableCollisions(false)`. CaughtUp (lines 123-124): `RemoveBuff(OID_HASTE)`, `EnableCollisions(true)` [src]. The buff is therefore named by the authored AI script. The companion's AI row has `Script = "rene"`, which is the key to that file [src].

**H1-H3 (launch args).** `package_v20.py` copies the Preview 16 base package, adds `additional_files`, and applies `remove_arg_pairs` to `startup.args`/`swamp.args` [src]. The base `swamp.args` carries `--condition-active RENE_FOLLOW`, `--source-root-scopes original-cache/data/scene/001_swamp.mlx`, `--campaign-commands original-campaign.xml` and `--campaign-triggers` [src]. `startup.args` (menu start) carries the same three flags plus `--campaign-triggers` [src].

**Packaging gaps found while running** [run]. The Preview 16 base package is missing files the code reads: `data/pydata/scripts_pyscripts.bin` and `scripts_pyscriptnames.bin` (the common bank), `data/pydata/dialogs_pyarray.bin`, `dialogs_pyarraynames.bin`, `dialogs_pycst.bin`, `dialogs_pystructnames.bin`, six Swamp decor/prop `.bdae` files listed in the packager's fixed `added` list, and `data/scripts/ai/rene.luac` and `data/scripts/objects/*.luac`. All are in `assets-extra/android/data` [src]. Without the common bank the `--campaign-triggers` path exits 1 at startup [run].

## 2. Implementation (commits on v20/general)

- **H7, haste buff from the authored AI script.** `features/companions/catch_up_buff_v1.{hpp,cpp}` reads the `GetPyOID("ClassTable", row)` declaration of the local that the first `ApplyBuff(...)` call names. Lua `--` comments are ignored. `main.cpp` reads `data/scripts/ai/<AiProps.script>.luac` through the asset catalog and looks up the row. No literal remains. A script with no `ApplyBuff`, or a non-ClassTable declaration, gives no haste (fail closed).
- **H8, authored OnOpen run through the Lua runtime.** `features/containers/container_open_script_v1.{hpp,cpp}` now loads the authored Lua text and runs `OnOpen(nil)` in a fresh `dh2_script_vm`. It binds `GetRand`, `GetPyOID` (CharacterTable only), `Summon` (6 args), `Trace`, and `GetName`. Any other game call raises a Lua error, so the open fails closed and logs. The hand-transcribed contract tables were removed. The pool reservation scans literal `Summon(GetPyOID("CharacterTable","..."))` calls in the authored text. The new static target `foundation_container_open_lua` compiles the dh2 script-runtime recipe (Lua 5.1 plus bindings), minus `script_constants.cpp`, which `foundation_runtime_potion` already owns. `main.cpp` gains a `read_authored_script_text` helper and honours the relative offset of Summon. `spawn=false` is refused explicitly.
- **Build wiring.** `CMakeLists.txt` adds `foundation_container_open_lua`, moves the runner out of `foundation_container_drops`, and adds the `catch_up_buff_v1` unit test.

Not done (honest): item 4 Act-1 literals were only inventoried, not fixed. See section 6.

## 3. Isolated tests [test]

- `container_open_script_v1` (rewritten): verbatim moth and container.luac text plus a synthetic script. Covers draws 10/25/0, a refused summon, a container with no summon, the synthetic script's own rows and offsets, a random failure, an unknown row, an unbound call, and the reservation scan. PASS.
- `catch_up_buff_v1` (new): verbatim rene.luac excerpt plus negative forms (no ApplyBuff, commented ApplyBuff, non-ClassTable declaration, comparison, undeclared local, `--` inside a string). PASS.
- `follower_decision_v1`: PASS.

## 4. Build and ctest

- Full build (all targets) of `build-v20general`: exit 0. The baseline at 09a1bb68 also built with exit 0.
- ctest: 161 of 163 passed. Expected worktree failure `session_skill_binding` [brief]. `schema_v4` failed under `-j 4` load and then passed when run alone (`Passed 0.25 sec`), so it is a load-sensitive flake, not a regression I caused.

## 5. Integrated runtime

**Frozen EXE:** `DH_sc/.local-inputs/v20/general/frozen/dh-foundation-general-c3.exe`
**SHA256:** `899B28814D1CE7C7F2F183557A750D19E5733973C8F6C922FBC7E0110E0FDA7E`
**Before EXE (09a1bb68 build):** `DH_wt/build-v20generalbase/dh-foundation.exe`, SHA256 `1AC56E362DD4D5F4B0CBBD00C4670A4D2BA3BBB4E12A3B4D96157356110A803E` (worktree removed; build output kept).

### Scene ledger (the minimal opening was run several times while packaging gaps were fixed; the rows below are the runs that count)

| scene / check | EXE | args | result |
|---|---|---|---|
| Moth barrel `_prim_DestructibleContainer_05_321_364_22_41_21_187_21_46`, interact at frame 22, seeds 1-8, 120 frames, quiet | before 1AC5...; after 899B... | c7 `s1.args` with the package asset root, combat-seed varied | **PASS (parity):** rolls 71,53,0,62,33,56,70,33 identical before and after. Summon `Swamp_Moth_Minions spawned` on seed 3 in both. Loot `table=9 selected=1 delivered=1 status=ok` identical. Logs: `runs/parity`, `runs/before` |
| Minimal opening: `swamp.args` minus `--condition-active RENE_FOLLOW`, `--source-root-scopes`, `--campaign-commands`; keeps `--campaign-triggers`; `--fresh-player`; `--campaign-skip-frame 1400`; 4500 frames | 899B... | `runs/minimal/minimal.args` | Exit 0. Scripts run from compiled banks (`common=15 level=55`); 17/18 trigger zones fed; `enterLocation_StartRoom` finished; `Swamp_Intro` started (camera and actor clips); skip registered. `Swamp_Intro` did **not** finish and **no Rene follower line appeared**. **Rene-follow: NOT PROVEN.** |
| Same minimal args on the base EXE, before dialog/decor staging | 1AC5... | same | Identical stall at `Script_WaitDialog` (same 5906-line log). The stall predates this change |

The historical c7 rolls (86,90,9,1,26,16,70,47, from an older build) differ from both current runs and were not used as the comparison.

**Visual verification:** none. I did not inspect any frames. The moth result is a log-level behaviour parity check, not a visual check.

## 6. Launch-arg answer (item 3) and packaging change

Proven: the compiled-bank path works with `--campaign-triggers` alone (no `--campaign-commands`, no `--source-root-scopes`), with the common bank staged. The `--condition-active RENE_FOLLOW` flag is still unproven to be removable, because Rene did not appear in the minimal run.

Recommended `package_v20.py` metadata (read only; I did not change the packager):
- `remove_arg_pairs`: `{"startup.args": ["--source-root-scopes", "--campaign-commands"], "swamp.args": ["--source-root-scopes", "--campaign-commands"]}`. Keep `--condition-active RENE_FOLLOW` until a Rene follow run passes.
- `additional_files` (sources under `.local-inputs/assets-extra/android/data`): `pydata/scripts_pyscripts.bin`, `scripts_pyscriptnames.bin`, `dialogs_pyarray.bin`, `dialogs_pyarraynames.bin`, `dialogs_pycst.bin`, `dialogs_pystructnames.bin` (to `assets/data/pydata/`); `scripts/ai/rene.luac`; `scripts/objects/*.luac` (15 files, to `assets/data/scripts/objects/`); the six decor/prop `.bdae` already in the packager's fixed list.
- Note: `remove_arg_pairs` removes a flag and its value, so it works for these two flags.

## 7. Remaining gaps (explicit)

1. **Rene follow after the opening is not proven.** The minimal run stops at `Swamp_Intro` in a dialog/cutscene that did not finish after the skip. The haste code path (H7) is only unit-tested. It is not exercised in any runtime run.
2. **The opening does not finish headless.** `--space-key-interval` presses register as `scheduled=1 pressed=0`, so Space does not reach the UI here. `--campaign-skip-frame` does not finish `Swamp_Intro` either. Same on the base EXE. This needs a separate investigation.
3. OnOpen actor is passed as nil. The `container.luac` damage branch (`DealDamages`, `actor:GetName`, `actor:PlayFX`) is not executed. This matches the previous behaviour (no damage), but it is not the original behaviour. A player-opened container with the damage branch would fail closed.
4. `Summon` relative offsets follow the authored `relative` flag. The game-side semantics of `relative=false` are assumed, not verified. `spawn=false` is refused.
5. GetRand uses the combat world RNG stream. Whether the original Lua `GetRand` shares it is unverified.
6. The Lua runtime is now linked into `dh-foundation.exe`. The EXE size and startup cost are not measured.
7. Item 4 Act-1 literals (inventory, not fixed):
   - `main.cpp` default level `data/scene/001_swamp.mlx` in the frontend start (around line 995) and the `--position 1090.75,-212.202,258` Swamp start in the args.
   - Knight class branch `selected->class_id != "KnightPlayerBase"` (around line 977, audit H9).
   - Swamp combat argument rows and `--source-root-scopes`/`--campaign-commands` in the packager (audit H1-H5).
   - Latent Act-1 modules not in `main.cpp` (audit L1-L7).
8. `schema_v4` is load-sensitive under `-j 4`.

## 8. Artifacts

- Source: `port/windows-foundation/features/companions/catch_up_buff_v1.{hpp,cpp,_tests.cpp}`, `port/windows-foundation/features/containers/container_open_script_v1.{hpp,cpp,_tests.cpp}`, `port/windows-foundation/main.cpp`, `port/windows-foundation/CMakeLists.txt`.
- Runs and logs: `DH_sc/.local-inputs/v20/general/runs/{parity,before,minimal,minimal-base}/`.
- Run package: `DH_sc/.local-inputs/v20/general/pkg-parity/` (hardlinked base package plus the staged files above).
- Scratch scripts: `DH_sc/.local-inputs/v20/general/scratch/`.
