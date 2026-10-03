# Source character animation stance

`character_stance.hpp/.cpp` implements original GetAnimStance 0x3a53e0.
StanceFacts16 holds explicit source current-equip-set predicate results and
signed authored count. Its six bits are IsPlayer=1, HasStaff=2, HasBow=4,
IsDualWielding=8, HasTwoHander(false)=16, HasMainHandWeapon=32. Invalid high
bits/nonzero reserved fields reject before output mutation.

Player priority is staff3, bow4, dual2, twohand1, absent mainhand5, otherwise0.
Nonplayers choose0. Original signed candidate<count returns the candidate;
otherwise the result is0. The actual Android constant query is
AnimStances/COUNT_IPHONE with authored value5, so unarmed candidate5 returns0.
Current source inputs do not require a base+5 authored asset. Full inventory
and properties are explicit caller producers, not inferred visual equipment.

Source function manifest, instruction capture, literal relocation resolution
and original proof are in `../character-timer-discovery`. All576 original
player/predicate/signed-count combinations replay exactly through host native
source under g++ -O1 ASan/UBSan. Five malformed atomic guards and count5/6
bare boundary checks also pass. The current source/reference/host hashes are
in `../../reports/character-stance-host-sanitizers.json`. No new ARM64 or APK
proof is claimed until the parent executes those actual artifacts.
