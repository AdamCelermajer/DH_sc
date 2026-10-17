# Native PyData script table reader

This subfolder contains a bounded, data-only C ABI decoder for the recovered
common and `001_swamp` PyData script tables. The command whitelist and field
layouts match the Python reader in `../pydata_scripts.py`. The decoder does not
execute or schedule commands.

## Ownership and offsets

Initialize `dh2_script_table` with `{0}` and call
`dh2_script_table_decode(names_bytes, names_size, program_bytes, program_size,
&table, &error)`. A successful decode owns copies of both byte tables, script
and command arrays, and each `ExecScript` integer vector. Script names and
string values are views into those owned input copies. Keep the table alive
while reading any of those views and release all of it with
`dh2_script_table_destroy(&table)`. The destroy function also accepts the
zeroed result left behind after a decode failure.

On failure, the result is empty and `error` gives the input table, absolute
byte offset, and script/command indices when known. Table-size, count, string,
and vector limits are public constants in the header. The decoder rejects
unknown command IDs, malformed UTF-8 and bool bytes, mismatched script counts,
truncation, and trailing bytes. It validates complete table consumption before
returning success.

`dh2_script_name_lookup` searches one table from a supplied local index using
ASCII case folding, matching native `strcasecmp` behavior in the default C
locale. `dh2_script_resolve_id` mirrors the recovered common/level lookup and
returns level IDs after the common-table count.

## Host corpus and malformed-input checks

With the cache described in the project handoff available at `work/cache/files`,
run from this folder:

```sh
python tests/run_host.py
```

Or pass another cache directory with `--cache PATH`. The runner compiles the
C++ decoder with exceptions and RTTI disabled, then links the C test harness
with the C compiler. The harness decodes all 15 common and 55 SWAMP scripts
(81 and 789 commands), checks the `LizardMan_Intro` and
`enterLocation_StartRoom` byte offsets and representative payloads, verifies
case-insensitive native ID resolution, and runs malformed/truncated/unknown-ID
cases. It overwrites and frees the source buffers before inspecting the
decoded corpus to verify result ownership.

## Android integration boundary

An Android loader can read the common table pair once and the current level's
pair when that level is loaded, then retain each decoded `dh2_script_table`
for as long as scheduled script references may use its views. A runtime can
store a script ID plus its own program counter, activation state, and timer
outside this decoder. When the application later implements the verified
native scheduling semantics, it can look up the decoded command at that
program counter and advance its separate runtime state. On level unload, first
cancel or retire scheduled references to that level, then destroy its table.
The common table can live for the game session. None of that scheduling or
dispatch behavior is implemented here.
