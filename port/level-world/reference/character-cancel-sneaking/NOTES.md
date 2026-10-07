# Character::CancelSneaking native coordinator

This stage reconstructs the complete valid-projection control flow of
`Character::CancelSneaking` and `CharAI::AI_CancelSkill`. Player classification,
full buff deletion, and skill-script Active/Pre execution remain required genuine
services. Nothing in the native coordinator invents animation selection, clears
the Sneak property, or treats an unavailable backend as successful.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The ten complete routine captures are in `original-functions.json` and
`reference/original-functions.asm`; each manifest entry binds its instruction hash.

## Source order and live reads

| Routine | Address | Recovered role |
| --- | --- | --- |
| Character::CancelSneaking | 0x3bc6b8 | Complete 204-byte enclosing coordinator |
| Character::IsSneaking | 0x3bc690 | Resolved property 198, signed raw result >0 |
| CharProperties::_GetProperty | 0x3dedb4 | Genuine resolved raw word access |
| Character::GetCharSkillListId | 0x3bc5c0 | Cached list ID, literal fallback 3 |
| Character::GetCharSkillList | 0x3bc5fc | Ordered SkillList row projection |
| Character::GetCharSkill | 0x3bc784 | Re-fetch live owner list, indexed Skill row |
| CharAI::AI_CancelSkill | 0x3d84e0 | Script presence/type/Active/Pre tail |
| CharProperties::PROPS_DelBuff | 0x3e101c | Captured required buff backend |
| CharAISkillScript::OnSkillCheck_Active | 0x3db16c | Captured required skill VM backend |
| CharAISkillScript::OnPreSkill | 0x3da8b8 | Captured required skill VM backend |

1. Call Character virtual +0x28, IsPlayer. Any nonzero result takes the player
   branch. Character identity is captured by the enclosing call.
2. Player: call `PROPS_DelBuff` with CharProperties at Character+0x560,
   **buff map key 146 (0x92)**, and a null optional property pointer. Only after
   that call returns, write Character byte+0x415=1. This is not property ID146.
   In the property schema ID146 happens to be `Slow_Duration`; that coincidence
   does not determine the buff map key's semantics.
3. Read the live resolved `Special_Sneak` property, ID198 (0xc6), and test the
   raw signed word >0. No fixed-point conversion, bit test or float conversion.
   Buff deletion can change this word synchronously before the read.
4. If positive, obtain the ordered SkillList. The ID is the raw cached word at
   Character+0x1068, corresponding to resolved payload index28, `SkillTree`.
   Negative or ID>=source table count selects literal row3. It is not a guessed
   default or a name lookup. Count0 returns without reading the skill table.
5. Scan list order and select the first Skill runtime row whose flags+0x1c carry
   bit0x02000000. Call embedded CharAI+0x3c8 `AI_CancelSkill` with the **list slot
   index**, not the SkillTable ID. Stop scanning after that first match.
6. AI cancellation first checks its script vector at AI+0xb4/+0xb8. A null
   selected script returns immediately, without querying the owner skill row.
   Otherwise reload AI owner+4, re-fetch its skill list and indexed Skill row,
   and require the exact word+0x48 (authored `Type`) to equal1.
7. Reload the selected script, call its `OnSkillCheck_Active`, and if nonzero
   reload the script vector again and tailcall `OnPreSkill`. Thus Active may
   replace the script or change the owner; Pre uses the freshly reloaded script.
   Pre's result is ignored by this void source call. The native provider's
   delivery failure is a separate error contract.

No direct source writes to Character+0x520, heading, animation, target or
Special_Sneak occur here. Deeper delivered buff/script effects can change those
states. Do not add a blanket flag/property/animation reset to Spawn1.

`PROPS_DelBuff` itself searches the actual buff map and inspects the associated
deque; null optional property does not imply unconditional deletion of every
matching record. Its timer cancellation, property destruction, erase, and
recalculation paths are captured but not implemented by this coordinator. Its
genuine backend is required for every player branch, even when Special_Sneak
is currently nonpositive. The two captured skill callbacks invoke the actual
skill script owner/ReturnValues path; their whole VM registration is also outside
this module. Source debug assertion/fault continuation for invalid indices is
not emulated as valid native array access.

## Native API and ownership

New `character_cancel_sneaking.hpp/.cpp` expose
`dh2_character_cancel_sneaking(Character48*,Services16*)` and
`dh2_character_cancel_skill(AI24*,uint32_t slot,Services16*)`.
Return0 means complete, 1 malformed projection, and 2 provider delivery failure.
Failure may follow delivered source effects; there is no rollback.

Character48 borrows its real resolved PropertySheet16, ordered Tables32,
embedded AI24 projection, identity, and byte415. Tables32 borrows List16 rows
and complete 76-byte Skill row projections. Skill payload flags are word7 and
Type word18; other fields remain uninterpreted here. List metadata preserves
order and duplicate IDs. AI24 borrows its live owner and script vector.

Services16 delivers four operations: IsPlayer, full DelBuff(146,null), skill
Active, and skill Pre. Request24 carries captured receiver, list index and
argument. All storage must remain alive through callbacks; callbacks may mutate
live projections or reenter but must not destroy held backing or throw across
the C interface. The port does not provide generic Lua reentry permissions.

Native safety guards reject unaligned/null required projections, reserved fields,
out-of-range list/skill/script indices, and arrays beyond65,536 entries. These
are explicit native contracts, separately tested. Unused deeper storage is not
required: nonpositive Sneak returns before list access, and a null script returns
before owner/skill access. Genuine resolved data, not a supplied Sneak boolean,
drives the native branch.

## Actual cache and proof scope

The authorized cache SHA256 is
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
Its `skills_pyarray.bin` is11,862 bytes, SHA256
`e336986d5aee78fd1aed7fe7ec43cc8d58fa03d4fd27216779c0e96e47acc2d6`.
It contains36 ordered lists and127 Skill rows. **No supplied Skill flags have
0x02000000**, so the cancellation tail is unreachable with those actual rows.
This does not make player buff deletion optional or justify clearing Sneak.

`skill-layout/` captures the complete source SkillList/Skill row and table readers
to ground the test-only serialized traversal: integer words, one-byte booleans,
vector counts/payloads and string lengths/payloads. Tests traverse the complete
actual stream and consume its exact final byte. Unused runtime string/vector
pointers are projected null. This is not a new production owned SkillTable loader
or a proof that original table allocation/string ownership is reconstructed.

Original/O2 proof:976 source control cases,1,538 ordered service calls,541 cases
with callback mutation configurations (156 execute a mutation), plus152 actual-cache cases/228 calls
across all36 lists and invalid-ID fallback boundaries. Zero mismatches. The
original CancelSneaking, IsSneaking, actual resolved GetProperty, list-ID fallback,
list lookup, GetCharSkill and AI cancellation instructions execute. Player,
DelBuff, and skill VM bodies are explicit services whose arguments, transitional
state, effects/reloads, and order are compared. Synthetic cases cover signed
Sneak edges, first/middle/no flagged skills, duplicate list references, nullable
scripts, Type0/1/other, Active0/1/other, live buff/property and Active/script/owner
mutation, and high-word identities through an explicit32-to64 identity mapping.

ASan/UBSan/LeakSanitizer host proof:34,930 checks,976 gold cases,152 actual-cache
cases,1,791 total delivered callbacks including malformed/provider tests,15
guards, and zero findings. Gold binary CSK1 SHA256:
`60517c5c257ead1d7ce0af8ab80123b69ad938ce58a64bc666f78c71916acbe3`.
The host does not execute whole skill Lua callbacks, full buff deletion, a whole
Spawn1 frame, or a packaged APK. No central DSO is rebuilt by its runner.

## Reproduction and integration

From repository root, with the existing oracle Python/PYTHONPATH environment:

```powershell
& port/level-world/tools/build_character_cancel_sneaking_oracle.ps1
python port/level-world/tests/character_cancel_sneaking_differential.py
python port/level-world/tests/character_cancel_sneaking_host.py
```

The host runner binds exact compiler/run commands, source/proof/cache hashes,
executable bytes and verifies immutable inputs before/after. A future central
target uses `tests/character_cancel_sneaking.cpp` linked to world and takes two
arguments: `reference/character-cancel-sneaking/cancel-sneaking-fixtures.bin`
and `.local-inputs/character-cancel-sneaking/skills_pyarray.bin`.

For Spawn1, attach genuine IsPlayer and resolved property backing first. A
nonplayer nonpositive Sneak branch requires no buff/skill backend. For other
branches preserve the required delivery failures until full actual buff/skill
owners are available. This handoff adds no CMake, renderer, checkpoint, shared
runner, DesignSettings, or existing frozen-module changes.

Frozen CPP SHA256:
`f3bc02c718a29739d8afc71aa5b8040714de1c84b23b3b84a1d8c474bdcdcf54`.
Frozen HPP SHA256:
`d3b808ff7280d181c96e6ad60df3d650cfbf3dcba33e0286ad0c84b55beeffe0`.
