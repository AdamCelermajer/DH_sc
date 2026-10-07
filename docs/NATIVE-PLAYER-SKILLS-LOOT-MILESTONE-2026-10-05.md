# Player skills and powered loot: native library integration

The main native build now selects the retained player skill V3 owner and powered-loot V7 owner. The new indexed script return implementation replaces the earlier return implementation; it is compiled once, with no second script runtime. Existing inventory, item effects, localization and script owners remain the authorities used by these additions.

## Skills

The player skill owner retains one script VM, property store, saved skill state, faery state, buffs and timer store. Its resource loader uses all 219 original cache scripts. Initialization was exercised for all three starting classes, with 19 source skill instances. Source callback ordering, passive buff application/removal, cooldown creation/expiry, skill AI operations and faery selection have native implementations.

The isolated original-code comparisons covered 8,800 cases. The central host run reproduced the frozen player, callback, AI and indexed-return results against the actual selected libraries. This does not establish full active skill animation, target search, combat or nonempty effects. Those operations require their real providers. The visible Android player still needs to adopt this retained owner and its single timer store.

## Powered loot

The native owner loads 121 actual power lists, 39 quantity lists and the same 937 power definitions used by item effects. It implements weighted power/quantity selection, monopoly conflicts, source retry/fallback ordering, difficulty variants, item valuation, gold valuation and name updating. Original-code comparisons covered 4,781 cases.

The host composition created, localized, valued and stored 363 powered items through the actual inventory owner. The storage stress test explicitly projects the source unlimited-inventory field; it also verifies the ordinary full-inventory failure prefix. This is not a complete random/subloot/drop/pickup/merchant implementation of AddLoot, and does not add a second inventory or RNG authority.

## Verification

- `port/level-world/reports/native-player-skills-loot-main-linked-host-audit-v1.json`: 151 suites, PASS, zero address/undefined/leak sanitizer findings.
- `port/android-native/reports/native-player-skills-loot-library-build-v1.json`: ARM64 and x86_64 selected-source/ELF inspection, PASS. Native libraries are ELF64 with at least 16 KiB load alignment; the receipt records actual library hashes, compiler records and the central host receipt hash.
- Both Android `dh2_native` build closures completed successfully with the new owners selected.

These receipts verify library integration. No new APK was packaged or installed for this milestone; the installed visible source-player checkpoint remains `dh2-native-source-player-ui-136e924a.apk`.

## Work ownership and remaining connections

This chat owns the in-game character stats, inventory, equipment and skills menu, equipped-item rendering, targeting and skill/item animation. The separate “Inspect app launch and menus” chat owns the main menu and character selection. Menu graphics are available, but a complete functional in-game character menu is still pending.

The next connection is the retained visual skin resource owner to equipped armor/weapons and the live animated player scene. The character menu must then query and act on that same player inventory, skill and property state. Whole active skill state/animation and editable text rendering are being completed in parallel. Full campaign/gameplay, audio, saves and physical-device verification remain unfinished; this milestone does not close the reconstruction goal.
