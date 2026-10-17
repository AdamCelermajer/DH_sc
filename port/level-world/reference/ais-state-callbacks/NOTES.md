# CharAIScript state callback leaves

The original ELF hash and four exact wrapper ranges are pinned in
`original-functions.json`. Adam's symbol/assembly corpus supplies the original
names and bytes; this continuation supplies maintained 64-bit source.

`CallStateUpdate`, `CallStateConditions`, `CallStateInit` and `CallStatePost`
each load the current state pointer at AIS `+0xb4`. Null returns immediately.
Otherwise the selected source string-data pointer at table `+0x14`, `+0x2c`,
`+0x44` or `+0x5c` is passed to `LuaScript::Call(char const*)` with the original
AIS receiver. There is no name validation in these wrappers. An absent or
empty name is therefore passed unchanged to the explicit Call provider.

The source projections borrow stable table and name storage, and use native
pointer widths. They do not overlay the old objects or reproduce the original
STL allocator. Each invocation rereads the live table; no table is cached
across update and conditions. Synchronous callback mutations survive failure.
The caller must provide valid, nonoverlapping objects and retain borrowed
projections through each invocation. State registration, alias resolution,
LuaScript::Call semantics and full AIS lifecycle are separate dependencies.

The host test covers actual update-to-conditions table removal/replacement and
name mutation, as well as missing-provider and failure boundaries. The original
comparison executes all four complete 20-byte wrappers and hooks only the Call
callee. It verifies selected arguments and post-callback AIS table facts.
These component checks are not Android pursuit or complete-gameplay evidence.
