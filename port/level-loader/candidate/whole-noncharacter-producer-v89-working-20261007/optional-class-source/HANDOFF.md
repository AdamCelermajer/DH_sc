# Actual optional class source closure

This is separate from the immediate V89 SWAMP input producer. It fixes discarded
genuine Generic/ColBox/Floor input packets and supplies their real implementation
dependencies. Apply the complete source closure together, not only the CPP call
hunk: Root currently has none of these optional catalog/ColBox/Floor implementation
units, so adopting only those calls creates unresolved source dependencies.

The existing native owner CMake helper now adds the actual catalog units,
ColBox/Floor units and existing Root RoomZone/RoomZoneFactory units, using its
existing absolute-path deduplication. No duplicate owner or new build target.
Canonical source fields/destruction helper are included in the same patch.

These classes still require genuine incoming class/physics/resource/release
packets. A provided Generic packet's independent owner must match the exact
scope authority passed to this catalog. optional-generic-owner-v89.patch supplies
that renderer authority adapter with weak current-scope guards; genuine callbacks
and immutable/native owners stay unchanged. No unsupported provider is advertised
through fabricated empty behavior. Runtime coverage is not established.

No tests/compiler/build/native execution. Shared Root unchanged.
