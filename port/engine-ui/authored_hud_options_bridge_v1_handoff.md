# Typed authored HUD options callback

Compile authored_hud_options_bridge_v1.cpp into level-world with the existing
typed CharacterMenuValueV1 and owned HUD query sources. Include its header in
the native CharacterMenuAsBridge dispatcher. Inside the active callback call:

```
authored_hud_options_bridge_v1(name,call,*same_world.saved_options,
                             HudInitServices16{context,remaining},error);
```

Names are NativeGetOptionParameters and NativeUseIpodPlayer. Whole frozen
dh2_ui_hud_initialization_v1 entries3/4 own guard/order/member/result behavior;
the adapter uses the actual CharacterMenuCall member/text/result hooks, never
SwfMovie.action_script reentry. Other HUD natives retain their existing owners.

Remaining providers return1 delivered,0 required-failure:
string_symbol: q.value is exact source integer text ID, response.text pinned
through all member-write reentry;
language_override: actual SAME isKOREAN_BUILD byte9f640b (original initial0);
publish_language: exact lang_kor int9a5c0c = q.value (reached only flagtrue and
keyLanguage);
platform_music_support: original signed nativeIsSupportMM result, not bool.
Use android_music_support_v1(actualBuild.MANUFACTURER,actualBuild.MODEL,value)
over source Android producer. Actual original APK normal result−1; whole native
entry4 returns boolean(value==1). Missing platform source stays required.

Strict Android compile both ABIs PASS. Current sanitized host attempt is
UNAVAILABLE because WSL g++ did not return within45seconds; receipt records
command/current hashes. No WSL restart performed. Actual original DEX proof
executes78 device combinations, all−1. Parent reports live wholeDisplayRightHud
now selects real CurrentHud and sword artwork without options/iPod warnings;
root owns that integration/live receipt.
