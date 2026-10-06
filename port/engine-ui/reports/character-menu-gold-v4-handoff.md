# NativeInvGetPlayerGold V4

Item-list AS calls the native function with [playerIndex0, actual object], stores the returned object as player_gold, then reads GoldString. The old bridge accepted only one numeric argument and left that result undefined. InventoryMain's one-argument call happened to display the old numeric result.

The complete original44a5c4 supports these forms: argc2 uses arg1 iff it is an object; argc3 reads remote fromarg2 and does not use an object; other nonempty forms use remote=false/no object. Arg0 always goes through source numeric conversion to signed32. A real absent player leaves result/members unchanged. Original literals are format8c9040=`^d`, members8cc7b8=`Gold` and8cc7c0=`GoldString`.

The new typed helper parses actual same inventory gold through the retained HudText integer StringManager508ef4 implementation. Object form rereads gold after parsing, writes Gold then the previously formatted GoldString, and returns that SAME object. Non-object form returns the actual formatted STRING. No literal label or new gold authority is introduced. Existing query dispatch wires same player/Gear/text graph directly; no NativeApp/session hook is needed.

Original ARM wrapper oracle PASS8 (argc1/2/3/4, present/absent player) includes a formatter mutation proving fresh Gold reread. AS/CString/player/parse are explicitly declared fixture boundaries. Native typed regression PASS checks string/object forms, remote, null, watcher ordering and failure prefix. Receipt `port/level-world/reports/android-native-owner-tests/character-menu-gold-v4/receipt.json`, binary SHA bafb5a022b289b8423179c7a67a6e244f3b571cbd8445f2a5be58d1566309c2b. Live item-list text remains root's build/runtime check.

Root subsequently verified live PID9645/capture92-character-system-final-gold.png: original item list displays0, with Equip/Unequip/Back continuing successfully. This observation is root-delivered evidence; this lane did not perform app input/install.
