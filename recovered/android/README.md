# Archived Android code exports

This directory makes the raw Java and smali exports from the historical
recovery handoff browsable as ordinary Git files. The files were copied
byte-for-byte from `Dungeon-Hunter-2-Source-Recovery.zip` (SHA-256
`b3ff974e2b74f50387465d5665f60d56ac79c29a449c6299745461998045c4d8`).
The [manifest](code-import-manifest.json) records every file's byte count and
SHA-256.

| Tree | Files | Bytes | Meaning |
| --- | ---: | ---: | --- |
| [`java/`](java/) | 364 | 1,335,043 | Raw JADX Java output, including generated inner-class/resource files |
| [`smali/`](smali/) | 357 | 4,095,069 | APKTool smali output for the recovered DEX classes |

The raw Java overlaps the repaired [`port/android-java/java/`](../../port/android-java/README.md)
tree: 287 relative paths occur in both, of which 143 have identical bytes and
144 differ. Another 77 raw paths are absent from the repaired tree, largely
generated inner-class and resource files; the repaired tree has one path absent
from the raw tree. These counts describe file names and bytes, not source
completeness. The repaired tree is the one that passed the recorded Java/DEX
build checks. The raw output has known decompiler damage and should not be
treated as a buildable replacement. Smali is an APKTool representation of DEX,
not the original studio Java source.

Verify the checked-out import with:

```sh
python tools/verify_android_raw_import.py
```

With the original ZIP available separately, also verify the archive hash,
member CRCs and exact source bytes:

```sh
python tools/verify_android_raw_import.py --archive /path/to/Dungeon-Hunter-2-Source-Recovery.zip
```

The original APK/DEX binaries, Android resources, cache assets and recovered
shader/XML exports are not part of this import. These generated code exports
retain the provenance and rights of the supplied game. No game-wide open-source
license is asserted; see [`RIGHTS.md`](../../RIGHTS.md).
