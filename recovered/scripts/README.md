# Exact cache Lua source

The owner's complete cache contains **219 readable source scripts, 900,493
bytes**, under `data/scripts`. Their `.luac` extension does not identify their
actual encoding: none has a Lua bytecode signature or NUL bytes. All are text;
217 decode as UTF-8, and two contain single-byte non-UTF-8 characters. Original
bytes, filenames, comments and line endings are retained under `original/`.

The tree has 73 AI scripts, 128 skill scripts, 15 object scripts, two level
scripts and one test script. These are actual cache source, distinct from the
generated native pseudocode. They were not decompiled or rewritten. Each was
compared byte-for-byte with its member in the complete cache ZIP, SHA-256
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
[The manifest](manifest.json) pins paths, members, sizes, encodings and hashes.

## Syntax and scope

Lua 5.1.5's [parse-only compiler option](https://www.lua.org/source/5.1/luac.c.html)
checks syntax without executing script code. **218 originals pass; one fails.**
The original `ai/sandworm_small_core.luac` line 159 lacks the outer closing
parenthesis in a `PlayAnim(GetPyOID(...))` call. A separate
[one-character source override](../../port/lua-scripts/README.md) passes syntax.
The original remains unchanged. [The validation report](../../reports/lua-source-validation.json)
records the compiler identity, per-file results and exact source/test hashes.

Syntax checks do not validate engine APIs, native object lifetimes, include
resolution, skill behavior, enemy behavior or gameplay. The original library
exports Lua 5.1-era APIs including `lua_getfenv` and `lua_setfenv`; that evidence
supports this syntax target, without establishing exact interpreter equivalence.
The native scripting bridge and a source-built game remain unfinished. These
files are not packaged into the current source preview APK.

The scripts retain the game's original rights status. This import does not
grant a game-wide open-source license; see [RIGHTS.md](../../RIGHTS.md).

```sh
python3 tools/verify_lua_source.py --cache /path/to/files --archive /path/to/cache.zip --compiler /usr/bin/luac5.1 --git-index --report /path/to/report.json
```

The archive/cache/compiler/index checks are optional. Hash verification of all
tracked original and override files always runs. The full-cache ZIP and generated
bytecode are not committed.
