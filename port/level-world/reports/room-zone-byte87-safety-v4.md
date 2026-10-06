# RoomZone byte87 modern safety repair

RoomZone factory 0x340f74 uses allocation followed by C1 0x396558. C1 delegates Zone C2 0x397ca0, which delegates the canonical GameObject base. None supplies ObjectBase byte87. RoomZone DeclareProperties 0x396554 is bx lr, so inherited `isGlobal` defaults do not produce this field. ObjectManager room lookup nevertheless reads byte87. This is an original indeterminate-allocation field, not a missing XML override to manufacture.

The modern correction initializes byte87=false in **only** CanonicalRoomZoneV3 through the existing base.store_byte producer. It keeps type11, room−1, source constructor fields, canonical identity, publication order, property declaration behavior, same Runtime, and Module backlink unchanged. Subsequent writes remain visible through the same canonical callback. No global constructor policy or other unknown class is altered.

Native receipts in this package describe fresh nine-receiver checks and the existing actual-cache nine-Module graph composition. The latter constructs actual Module visuals/controller/room PF, source generated RoomZone Spawn, InitBounds, InitFinal and release. Network/device/local-player external transports are fixtures as explicitly stated in the test. It does not prove full SWAMP205 objects, gameplay campaign loading or the running app's compiled RoomZone implementation. The changed RoomZone CPP is compiled into the native executable against the listed current APK libraries, so loader must rebuild this source for adoption.

Original allocation/constructor instructions are included in canonical-object-factory-v1/original-source.asm. No poisoned-allocation ARM execution is claimed by this receipt; source audit identifies the absent write, while the modern behavior is tested natively.

Acceptance: adopt the one CPP correction, rebuild coherent loader sources, rerun unfiltered MGP. The expected key11/type11 failure disappears; any next class/provider boundary must remain explicit. Prior immutable packages remain unchanged.
