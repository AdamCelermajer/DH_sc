# NativeGetPlayerHUDInfos v1

Frozen new producer and protected retained-core callback adapter. This is a complete caller reconstruction with required backend services, not a complete player/skill/inventory owner.

## Authority and scope

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Complete NativeGetPlayerHUDInfos `0x44e5cc`, 3248 bytes, function SHA256 `69b6445174fe31555f5972f89483777293a8b57d21855fd5bf85a3dd783970e4`, and twelve helper captures are in original-functions.json and reference/original-functions.asm. literals-callees.json records source names/callees. manager/NOTES.md records the complete manager integration contract; that manager has not been implemented here.

The actual decompressed dqhud_droid SWF has no literal NativeGetPlayerHUDInfos (native-action-names.json). The existing real-core load plus advance(0) observation requested two localization calls. This is a bounded startup observation, not proof about every future ActionScript path. The authored HUD also references NativeHUDSkill, NativeHUDSpell, NativeUsePotion and equipment/detail callbacks. This producer must not be presented as proof that those endpoints are connected.

## Caller ABI and ordered effects

Only argument counts 2 and 3 enter the original body. Argument 0 requires original type 5 and an as_object cast; argument 1 is to_number then ARM d2iz truncation. Optional argument 2 is to_bool and selects remote versus local player. NativeGetPlayerChar 43c388 performs signed index checks, uses GetLocalPlayer 36e478 or GetRemotePlayer 36e2ac, and reads Character at Player+660. The returned actor is captured once.

A null actor, removed byte +81, or inactive byte +80 writes only PlayerActive=false and returns the supplied object. For an active actor the source queries all three skill slots first. For each slot other than -1 it then queries usable, level and skill info. Spell info follows. Output writes are strictly ordered:

| Key | Source |
| --- | --- |
| PlayerActive | true |
| LEVEL | PROPS_GetInt(19,false) |
| HP_PCT | signed wrap32(100*resolved[36])/resolved[38] |
| HP_LOWPCT | CharacterDesign/LowHealthPercentage |
| MP_PCT | signed wrap32(100*resolved[41])/resolved[43] |
| XP_PCT | signed wrap32(100*resolved[33])/resolved[34] |
| SPELL_PCT | f2iz(spell fraction*100) |
| SPELL_MP | low byte of AI_IsSpellUsable |
| SKILL1_PCT, SKILL1_MP | fraction*100 then captured usable low byte |
| SKILL2_PCT, SKILL2_MP | same for slot 1 |
| SKILL3_PCT, SKILL3_MP | same for slot 2 |
| NB_POTIONS | ItemInventory.GetNumPotions |
| AVAIL_POINTS | PROPS_GetInt(148,false)!=0 |
| TouchToMove | saved DPad option equals zero |

The resolved sheet is reread before each percentage; a preceding object setter may mutate later values. There are no percentage clamps here. Signed division truncates toward zero after the multiply wraps. Zero denominator uses an explicit required runtime-result service. Float conversion models ARM NaN=0 and positive/negative saturation. Source setters capture/reload virtual+1c, ignore its return and finally return the same object. Services must preserve actor and sheet lifetime throughout synchronous mutation.

## Required backend producers

SG_GetSkillInSlot 3bbe68 and SG_GetSkillLevel 3bbed0 read Character+14e8 and call 467488/4668dc; null produces -1. CharacterSkillOwner's configured script slots are not these getters and do not establish a skill level. AI_IsSkillUsable 3d8358 requires native state gates, owner flags bit 8000, AI skill-use gate, AI+b4/b8 storage and script OnSkillCheck_Usable. AI_IsSpellUsable 3d80b4 requires native state, CanUseSkills, selected spell and AI+c0/c4 storage. SkillInfo 3d7e88 and SpellInfo 3d7da8 use script GetInfo 3daca8; a missing script writes fraction zero. GetCooldown 3da3d0 uses stored timer identity and actual FindTimer, then 1-(remaining/total). These ownership/services are explicit.

ItemInventory.GetNumPotions 3fc690 reads inventory+24: null yields zero; otherwise a signed int16 at item+50. PROPS_GetInt 3df6e0 calls GetProperty then arithmetic right shift by eight. ApplicationGetSavedOption 320e44 and world PlayerManager ownership remain required services. No potion, skill, property, player or saved-option fixture is claimed as a genuine live producer by this stage.

## Native and core adapter

hud_player_infos.hpp/.cpp exports dh2_ui_hud_player_infos_v1 and ordered HudInfosServices16 requests. Return 0 means delivered/no-op, -1 malformed, -2 required failure; failed prefixes remain. The kernel requires a coherent borrowed HudInfosActor32 projection.

hud_player_infos_core.hpp/.cpp exports hud_player_infos_callback_v1(fn_call,services,error). The caller must use the retained movie's protected scope. This module adds no global registration or generic reentry. It strongly retains the exact supplied AS object, uses the actual core number/bool conversion, performs source-safe signed conversion, and applies exact source keys with actual set_member bool/number values. Setter/watch code executes; a rejected/read-only setter return is ignored as in the original. Failed provider delivery retains prior writes and produces no successful result. Primitive/null argument 0 is rejected explicitly instead of following the original invalid dereference. This is a safety boundary, not an original general-AS conversion proof.

## Evidence and reproduction

Original complete caller versus optimized ARM64: 520 cases, 13,905 ordered requests, 184 explicit zero-denominator runtime fixtures, zero mismatches. Gold info-gold.bin SHA256 `6038db6112028dc8083845fae77c573525decfb35599eb656305a9a48e2d2024`. Service hooks implement AS cast/coercion/string maintenance/member effects and deeper actor backends; the captured caller instructions, arithmetic and order execute directly.

Sanitized host replays the same 520/13,905 gold and 17 failure guards. The real-core AS adapter adds 8 sessions, 54 checks and 66 deliveries. CharacterDesign/LowHealthPercentage=15 comes from the genuine owned constants loader decoding the actual design_pycst.bin (152 assignments). Other backend services remain explicit fixtures. Actual AS sessions use a minimal core root/object environment, not a complete authored HUD startup. ASan/UBSan (upstream vptr excluded for the adapter)/LSan: zero findings.

Run from the repository on Windows:

    python port/engine-ui/tests/hud_player_infos_host.py --core-library /home/adampalace/dh2-gameswf-asan/libgameswf_core.a

Standalone source kernel test: tests/hud_player_infos.cpp takes info-gold.bin. Core test: tests/hud_player_infos_core.cpp takes the real design_pycst.bin, links the two new producer/adapter sources, frozen script_constants.cpp, and the frozen GameSWF core (z/dl; core feature definitions retained). Source and core-library hashes are checked before/after. Reports hud-player-infos-arm64-differential.json and hud-player-infos-host-audit.json bind sources, corpus, binaries and dependencies. No central CMake, core/facade, Android renderer, SDK or emulator changes were made.
