# Checked script overrides

The separate `overrides/data/scripts/ai/sandworm_small_core.luac` preserves all
original bytes except one added `)` at original line 159:

```diff
- PlayAnim(GetPyOID("AnimDict", "swampking_emerge");
+ PlayAnim(GetPyOID("AnimDict", "swampking_emerge"));
```

The original has 14,520 bytes; this override has 14,521. The exact original is
preserved in [the cache source tree](../../recovered/scripts/README.md).
Both hashes and the exact replacement are recorded in its manifest.
Lua 5.1.5 parse-only compilation passes for the override. This fixes syntax;
the sandworm's runtime behavior is untested and native engine APIs remain needed.
It is not yet applied to the game, cache or Android source preview.
