# Original save-file inspection prototype

`inspect_original_savegame.py` is a read-only audit tool for the recovered
`.savegame` fixtures. It does not use `DH2S`, invoke original-game callbacks,
change the cache, or claim conversion compatibility.

## Run against extracted cache files

From the repository root:

```powershell
python port/persistence/inspect_original_savegame.py `
  --cache-files ../cache/files `
  --manifest recovered/assets/cache-manifest.json
```

The command prints JSON to standard output. It first requires each selected
manifest entry to be marked `verified`, checks the extracted file's size and
SHA-256 against that manifest entry, and only then inspects it. A missing or
mismatched fixture makes the run fail. No output file is created unless the
caller redirects stdout.

## What the prototype decodes

Only a level-checkpoint directory prefix is parsed. The one
manifest-verified checkpoint fixture exhibits this prefix:

1. Little-endian `uint32` section count.
2. For each section, a little-endian `uint32` name length, that many printable
   ASCII name bytes, then a four-byte little-endian value of unknown meaning.
3. All remaining bytes are emitted as one opaque body hash. The inspector does
   not split that body into sections.

The available verified checkpoint backup has two labels, `INFO` and `OBJS`,
followed by the observed values 41 and 100. They are **not** interpreted as
byte lengths. The sample's remaining body begins at byte 28. Recognized labels
and any unknown labels are reported separately; each four-byte descriptor
value and the remaining body have exact offsets, lengths and hashes. Raw body
bytes are never emitted. Counts, names, file size and descriptor reads are
bounded before slicing.

Player profiles, settings, and debug-switch saves are reported as whole-file
opaque blobs with byte length and SHA-256. The profile prefix value seen in the
sample is intentionally not called a section count: its full framing and the
profile section names/layout have not been established. Backup comparisons
report byte equality, size, differing byte count and at most 32 differing
offsets only when both files have verified manifest entries. For the level
checkpoint, the local primary file is absent from the recovered manifest, so
it is not read or compared; the report says
`primary-not-in-verified-manifest`. The profile pair has both verified entries
and is compared. The inspector never labels a copy valid or chooses one for
restoration.

## Tests and fixture handling

```powershell
python -m unittest discover -s port/persistence/tests -p "test_*.py" -v
```

Tests require the external recovered `../cache/files` directory and the
checked-in `recovered/assets/cache-manifest.json`. They authenticate all
manifest-listed save fixtures, copy them into an operating-system temporary
directory, and inspect those copies only. The current incomplete archive
manifest does not authenticate the local level-checkpoint primary file, and
tests do not read it. A few tests derive temporary byte
mutations from an authenticated copy to exercise bounds, hash rejection, and
unknown-name reporting; neither those mutations nor copied original saves are
placed in the repository or written back to the cache. In a checkout without
the external cache, the fixture suite skips with a clear reason.

## Limits

- This is not a complete parser for any player-profile, settings, or debug
  `.savegame` file.
- Only the count/name/four-byte-value prefix of the verified level checkpoint
  backup is decoded. The four-byte values do not have established semantics.
- `INFO`/`OBJS` callback payload boundaries and contents are not decoded into
  game objects. No object IDs,
  transforms, script state, quest state, inventory, class, or spawn semantics
  are inferred by this tool.
- The tool does not read the original APK, run the original save/load code, or
  make a save usable by the rebuilt app.
- Matching the recovered manifest proves byte identity with that recovered
  cache entry. It does not establish save-version compatibility across game
  builds or prove that the primary/backup pair is semantically healthy.
- Passing these tests is evidence for bounded, read-only fixture inspection
  only. Original-save import/export remains unimplemented.
