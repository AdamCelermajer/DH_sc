# Full InfoHUDManager source integration contract

Read-only complete captures; no full-manager native implementation or execution proof is claimed. See original-functions.json for exact function hashes and literal-bindings.json for source-relative strings and cached-clip offsets. These observations augment the frozen five-clip FastUpdate region without changing its proof scope.

## Update 41ec00

1. Application.GetCurrentLevel 31f594 is required. A nonnull Level with byte+198==0 rejects; null Level alone does not reject. The byte producer remains unresolved and is not named as an invented pause/loading flag.
2. Reload RenderFX at manager+57c; null returns.
3. If initialized byte+4 is false, call initCachedChars 41d880, then applyOneTimeValues 41d728; only afterwards clear manager cached target+0.
4. Read signed timer+8. Negative stores 500, calls SlowUpdate then FastUpdate, with no GetDt subtraction.
5. Nonnegative captures the old timer, calls Application.GetDt 31f66c, subtracts with 32-bit wrap, stores, then FastUpdate. Crossing negative triggers SlowUpdate on the next call, not this call.

This manager timing is not another actor elapsed-time increment. Application/UI frame ownership and required service failure policy still need an enclosing caller implementation.

## Cache and one-time setup

initCachedChars requires FX, reads actual saved HUDStyle, resolves `_root.menu_HUD_%d`, and retains its base. Cache creation 427ca0 receives the exact FX/base/path. HurtCorners and DeathTimer are absolute/null-base paths. HUDStyle>1 uses btn_0/btn_pre0/btn_post0 lists; other styles use btn_skill1/2/3. Initialized is written only at the end. Borrowed caches cannot outlive their graph.

| Manager offset | Authored target |
| --- | --- |
| +c/+3c/+6c/+9c/+cc | HP, Distress, Hurt, MP, XP frozen status clips |
| +fc | HUDelements.HealthBars.btn_potion.cnt.value |
| +12c/+15c | HUDelements.controls.controls.btn_spell.CoolDown / Grey |
| +18c/+1bc/+1ec | three list button collections |
| +21c/+24c/+27c | formatted btn_skill1/2/3.CoolDown |
| +2ac/+2dc/+30c | corresponding Grey |
| +33c | HUDelements.btn_charactermenu.anim_levelup |
| +36c | HUDelements.controls.controls.Joystick |
| +39c | enemy |
| +3cc/+3fc/+42c | enemy.enemy_name.text / enemy.enemy_level.value / enemy.HpBar.bar_hp |
| +45c | HUDelements.btn_charactermenu.btimg.HudChar |
| +48c/+4bc | _root.DeathTimer / _root.DeathTimer.DeathNumberTimeMc.TimerNumber_txt |
| +4ec/+51c/+54c | HUDelements.HealthBars.Ally0/1/2 |

applyOneTimeValues gets cached Joystick, then applies visible byte+9b from saved DPad!=0. Local player 0 yields Character+660; null Character returns. SG_GetPlayerClass 3bb7fc IDs 290/291/292 select HudChar frame 2; 325/326/327 frame 1; all others frame 0 through actual RenderFX goto(false). No inferred class names.

## Complete FastUpdate 41e064

1. Local player 0, Character+660; null Character returns. Original assumes Player exists.
2. GetNumPotions 3fc690; signed `%d`; cached potion text; SetText 7a92e0 with parse=false.
3. The frozen status region 41e0f4..41e208 drives HP/Distress/Hurt/MP/XP.
4. GetChar(levelup cache) before PROPS_GetInt(148,false); visible byte is nonzero result.
5. Query slots 0..2. Slot -1 means cooldown zero; otherwise actual script vector at Character+47c[index] and GetCooldown, or zero if that script is null.
6. Read saved HUDStyle. Style>1 reads each list button's actual AS member SlotId through virtual+20, to_number/d2iz; unsigned values <=2 choose a slot, other values (including negative) choose zero. Other styles use direct index. Each cooldown goto is max(wrap(trunc(fraction*100)-1),0), false, with no upper clamp.
7. The first pointer at Character+47c gates a spell pointer read at +488 and spell cooldown. A null spell under this source-coherent nonnull skill assumption is not given a fabricated success fallback.
8. GetOnline 7fd794. Online death/allies work runs before enemy work; offline bypasses it.
9. Enemy branch calls owner IsDead virtual34, repeats CharAI.GetTargetAsCharacter 3d5450, and examines raw target+14a4 using IsCharacter virtual24 and IsMonster 3a3064. Preserve the captured branch/reload sequence instead of blanket dead-target hiding.
10. New eligible target stores manager+0, obtains StringManager text at target+1040, IsBoss/GetLevel (missing level text `??`), shows enemy and invokes label `Show`. Actual Debug load/GetShowMonstersNamesAndNetID affects net-id/name formatting. HP percent uses float*100, truncation, min100, subtract1 with no lower clamp.
11. Lost prior cached target clears manager+0, hides enemy, sets enemy HP frame0, invokes label `Hide`. Already-null cache avoids redundant effects.

Online branch: Player deathTimer+3a8 nonnegative is divided by 1000 for DeathTimer. Up to three allies are selected from actual player count/remote virtual50, Character pointer and Player+4e5 guards. It uses cached Player level+330, HP frame clamp0..99, StringManager multi-player name formatting, death timer or -1, real Character XYZ with Z+350, GetScreenPosFromWorldPos 50e830 and inverse-pixel scales 416538/416578. SetPosition 7aa3f0 and root InvokeASCallback `AlliesBarDisplay` receive six actual AS arguments. Required camera/network/player/string/AS services are not replaced by overlay guesses.

## SlowUpdate 41de54

Local Character gate. Slot lookup and usable queries are interleaved per slot (unlike NativeGetPlayerHUDInfos, which queries all slots first). Missing slot remains unusable. GetChar(spell Grey) happens before IsSpellUsable, then visible byte is the raw source result XOR1. Saved HUDStyle chooses actual AS SlotId mapping or direct index. Each skill Grey cache is GetChar-null-checked and then GetChar is called again before the visible-byte write. Preserve synchronous reload order; do not normalize these effects to unconditional availability.

## Integration boundary

Five-clip status and sprite/advance owners are separately frozen. Full potions, skill/spell getters, private cooldown timers, saved options, dead/target/network/string providers, AS text/member setters and retained cache manager lifetime remain mandatory for the full manager. Literal/capture evidence is source discovery, not full manager original/native parity. NativeGetPlayerHUDInfos is a separate complete caller with explicit services; it does not substitute the native manager above.
