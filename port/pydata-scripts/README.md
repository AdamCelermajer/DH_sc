# Bounded PyData script-table reader

This component decodes original PyData script names and command records into
owned host-language values. It does not run commands, create game objects,
evaluate trigger conditions, or connect to the Android app.

## Binary layout recovered from the original reader

Both table formats are little-endian and packed without alignment between
fields.

```text
name table:
  u32 name_count
  repeat name_count times:
    u32 utf8_byte_count
    byte[utf8_byte_count] name       # no serialized NUL terminator

program table:
  u32 script_count
  repeat script_count times:
    u32 command_count
    repeat command_count times:
      u32 command_id
      command-specific packed fields
```

`ExecScript` has a variable-length integer vector: one byte bool, i32 script
index, u32 vector count, that many i32 values, then one byte bool. A command's
size is derived from its command reader; there is no per-command byte-size
prefix. Bool fields are one byte and strings are a u32 byte length followed by
exactly those bytes.

The native `readStringEx` copies bytes and does not itself validate a text
encoding; this host reader requires strings to be valid UTF-8. All names and
command strings in the verified common/SWAMP corpus satisfy that rule.

The decoder imposes limits on table bytes, names/scripts, commands, string
bytes, and variable arrays before building result values. It verifies complete
input consumption and equal name/program counts. An unknown command ID or
malformed field is an error; it is never skipped by guessing a size.

## Recovered flow examples

For the supplied cache, the common name table has 15 scripts and the SWAMP
tables have 55 names and 55 programs. The SWAMP program file contains 789
commands. `LizardMan_Intro` is local script index 17, with command-count word
at byte 2426 and command stream `[2430, 2656)`. Its decoded timeline is:

| Byte offset | Command | Payload |
| ---: | --- | --- |
| 2430 | ExecScript | `(true, 1, [], true)` |
| 2444 | LockCharacter | `("All")` |
| 2455 | SetCameraTarget | `(1000, "_prim_Waypoint_NewCamSpot", false)` |
| 2493 | Wait | `(500)` |
| 2501 | SpawnCharacter | `("_prim_Monster_LizManIntro1")` |
| 2535 | Wait | `(1500)` |
| 2543 | SpawnCharacter | `("_prim_Monster_LizManIntro2")` |
| 2577 | Wait | `(2000)` |
| 2585 | UnlockCharacter | `("All")` |
| 2596 | SetCameraTarget | `(1000, "LocalPlayer", false)` |
| 2620 | ExecScript | `(true, 3, [], true)` |
| 2634 | DoTutorial | `("CombatTuto", 7)` |

The paired common table identifies index 1 as `BeginScriptedCutScene` and
index 3 as `EndScriptedCutScene`. Native `Script_ExecScript::Execute` adds the
level script base only when the first bool is false, so these true-flag calls
refer to those common absolute IDs.

`enterLocation_StartRoom` is local script index 45. Its one command is
`StartDialog(-1, 3, 1703960)` at byte offset 17428, size 16. The recovered
`DialogStyles.EnterLocationDialog` constant is 3. The SWAMP MLX associates this
name with module 0's `_prim_Zone_EnterLoc_Prison`; that trigger has unlimited
trigger count. Separately, `_prim_EntryPoint` has `entrypointID=0` at
`(1090.75, -212.202, 258)`. The native `Level::_LoadPlayer` chooses an active
SpawnPoint whose ID matches the selected level entrypoint. Thus the entry
dialog trigger is a later overlap event, distinct from initial player spawn.

## Scope and evidence

The command schema covers every command observed in the supplied common and
SWAMP script files, plus source-traced `StartDialogID` (one signed dialog
index). Other native commands remain unsupported here, including
`SetCamera`, `WaitCamera`, `PlayAnimById`, AI commands,
container/loot operations, level changes, trophy operations, and tutorial
message queue commands. Add a schema only after tracing its native reader;
until then the parser rejects it. Parsing fields also does not prove that
referenced actor, waypoint, dialog, or script names exist at runtime.

Useful source evidence in `recovered/native/decompiled/libDungeonHunter2.so/`:

- `functions-004.pseudo.c`: `Level::_LoadScripts` (`0x00403a58`) appends common
  script tables then loads the level config's `*.pyscript`; the path loader
  derives `_pyscripts.bin` and `_pyscriptnames.bin` (`0x004038a0`).
- `functions-006.pseudo.c`: `LoadScriptFileNames` (`0x0046ad40`) reads
  length-prefixed names; `LoadScriptFile` (`0x0046b264`) reads command counts,
  peeks each ID and dispatches to the registered command reader. The shared
  `ScriptCmd::read` consumes the command ID; several leaf readers consume it
  directly instead.
- `functions-011.pseudo.c`: `ScriptCmd::read` (`0x0050f828`),
  `ExecScript::read` (`0x0050fc24`), `SetCameraTarget::read` (`0x005126c8`),
  `SpawnCharacter::read` (`0x00511d80`), `PlayEffect::read` (`0x005122e4`),
  `Wait::read` (`0x0051327c`), `DoTutorial::read` (`0x0050fe38`), and
  `StartDialog::read` (`0x0051345c`).
- `Structs::StartDialogID::read` (`0x005033e4`) reads one signed dialog ID;
  `Script_StartDialogID::Execute` (`0x00461114`) expands that row through
  `GetDialogMsgListById` (`0x00433f04`) and appends every step to the shared
  `MenuMessageManager<DialogMsg,1>` queue.
- `functions-006.pseudo.c`: `GetIDFromName` (`0x004691f0`),
  `Script_ExecScript::Execute` (`0x004707f4`), and
  `Script_StartDialog::Execute` (`0x00470fe8`).
- `functions-002.pseudo.c`: `TriggerZone::InitPost` (`0x003ac0a8`) resolves
  trigger script names; `TriggerZone::Update` (`0x003ab458`) starts a trigger
  script on player overlap; `SafeStartScriptOnlyOnce` is at `0x003ab2a0`.
- `functions-004.pseudo.c`: `Level::_LoadPlayer` (`0x00400018`) places the
  player at the matching entrypoint SpawnPoint.

## Host regression tests

Run standard-library-only tests with:

```sh
python -m unittest discover -s port/pydata-scripts/tests -v
```

The malformed-input tests always run. The cache corpus goldens run when the
original cache is discoverable next to the checkout or when `DH2_CACHE_ROOT`
points to its `files` directory. Corpus assertions include full consumption,
all command counts, the `LizardMan_Intro` byte timeline, the StartRoom dialog,
and common/level name lookup offsets.
