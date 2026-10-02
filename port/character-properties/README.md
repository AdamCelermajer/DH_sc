# Character property records and sheet operations

Source C reads the owner's complete `character_properties_pyarray.bin` and
provides owned 224-integer sheets. Original record vtables and ARM32 pointers
are absent from the source representation. This is a standalone component;
entity ownership, Lua object callbacks,
Android APK integration and full source-built gameplay remain unfinished.
The later [stat composition module](../property-composition/README.md) provides
the original base/equipment/buff calculation with explicit borrowed sheets.

## Original reader evidence

The original three array readers, four nested record readers and three primitive
readers execute against the exact 401,608-byte cache file, SHA-256
`516ba82f631174d4c0a24708342549b5993c402f68c2c1dabd5ddc9eea138784`.
They consume the whole file in registered order:

| Table | Records | Byte range, end exclusive |
| --- | ---: | --- |
| CharacterTable | 448 | 0–401,412 |
| StatAutoAssignSchemeTable | 3 | 401,412–401,572 |
| StatListTable | 4 | 401,572–401,608 |

All 100,352 character field values match actual original reader destinations.
The original offset table is exactly 0,4,…,892; an original record is 900 bytes
including a four-byte vtable. The three counts also match their corresponding
ordered name tables. Defaults use character row zero; types use row one.
The stat assignment/list payloads are traced and validated by the source loader,
but their gameplay consumers are not implemented. See
[reader trace](../../reports/character-property-reader-trace.json).

Virtual stream reads and bounded zeroed allocation/disposal are explicit models.
This does not run entities or the original game. The file belongs to the owner's
cache and is supplied separately under the repository's [rights scope](../../RIGHTS.md).

## Property operations

Original get, set, default, type, is-set, add, reset and load bodies execute and
match source ARM64/host in **2,022 counted operation checks**. Every field is
covered. Source loading additionally compares every value from all 448 records
to actual original reader output. Whole sheets, vtable/output guards, input
preservation and stack restoration are checked. ARM64 executes in Unicorn;
hardware and Android app execution are not claimed here.

An unset property equals its serialized default. Adding to an unset property
replaces it with the delta; otherwise it adds with low-32-bit wrap. A serialized
type of -1 resolves to 16. Reset copies defaults, and load copies a chosen row.
See [comparison evidence](arm-differential-validation.json).

The source loader borrows immutable caller bytes, requires two character rows
for defaults/types, bounds input to 4 MiB, validates all three tables and requires
exact consumption. Rejection preserves output. Invalid original debug/log/fatal
paths are not reproduced. Strict ASan/UBSan checks pass **12,000** valid/damaged
iterations covering truncation, count damage, corrupt views, invalid indexes,
aliases, wrap and sentinel semantics. [Host build/safety](host-build-validation.json),
[16 KiB ARM64 build](android-build-validation.json).

## Reproduce

```sh
python3 tools/trace_character_properties.py --help
python3 port/character-properties/build.py --host --report /path/to/host.json
python3 port/character-properties/build.py --ndk /path/to/ndk --report /path/to/android.json
python3 port/character-properties/tests/differential.py --help
```

Tracing needs Unicorn/pyelftools and the pinned original library. Source builds
use only repository C sources. Differential checks require the original library,
owner cache, existing arithmetic oracle and the two compiled source modules.
