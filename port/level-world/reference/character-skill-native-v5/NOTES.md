# Character skill native bindings V5

This isolated owner borrows the retained V3 session, inventory shared PropertyState, current State56, and supplied live world registry. It owns only target result storage and named backups. It does not replace frozen V3/V4 or claim a complete campaign.

## Source correction to V4

The V4 NOTES statement that BashDown Pre executes global PlayFX is incorrect. In the actual `prince_warrior_bashdown.luac`, that line is commented out. The effect is authored in animation metadata. V4 files and freeze remain byte-identical; this correction governs integration. ColdRay/JumpKick populated-target Use branches still require their actual reachable object FX and SkillCombatRoll providers.

## Integration

Construct `CharacterSkillNativeBindingsV5` with the genuine `FreshInventoryOwnedV4`, current `NativeFsm24`, mana services, and `SkillNativeWorldV5`. Supply its binding factory to the retained V3 session; attach that session before initialization. After VCB registration, call `install_object_binding()` on the same VM. Keep the existing scoped V3 ClearTarget binding: it performs the source native setter/last-target synchronization. V4 supplies skill-state and animation-context callbacks against the same State56 and AnimationInstance.

HasMana/UseMana execute the recovered native property41 and ordered Application/player/DebugSwitches paths. Debug configuration and owned CString lifetimes are required synchronous services; failure retains reached side effects. CalcManaCost remains the actual Lua `_commons.luac` SetTempProps/GetProp implementation on V3, not a substitute native formula.

HasShield and stance read the selected live inventory set and actual decoded item type/weapon fields. Stance uses the source COUNT_IPHONE/AnimStances properties and genuine two-hander query. No equipped item is invented to satisfy a skill check.

Target filters, search, named backup, empty/size/top/pop use the genuine search kernel. The versioned kernel preserves the caller-captured origin and resolves saved identities lazily at the source iterator Get point. Object top returns through the same VM object bridge. Registry/object backing and identity resolution must remain valid during synchronous calls; provider failure preserves the accepted prefix. LookAt requires actual controller delivery. Unsupported search origin/flags and non-Enemy/non-AttackableOnly domains fail explicitly when reached.

## Evidence and limits

`mana-gold-v5.json` and the ARM64 O2 report record 4096 original ARM32/port cases, 11320 ordered services, zero mismatches. Nonnegative costs, bypasses, Debug switches, zero cost and property mutation are covered; negative assertion continuation is explicitly outside the recovered domain.

The ASan/UBSan/leak host receipt records three actual source scripts, 15 callbacks in retained V3 players, three same-owner mana spends, and three deliberate required Debug failures preserving the debit prefix (523 checks). Check/Pre/Use/Post and insufficient-mana Check run without alias scripts. These are explicit empty-world fixtures, so the populated-target combat/FX branches are not claimed. ClearTarget uses the actual scoped native setter and synchronization.

The versioned search replay passes the existing original SearchEff corpus: 412 comparisons, 26580 ordered callbacks, 1480 accepted records, nine synchronous reentries, 2884 atomic rejection checks and 1236 provider failure checks. Focused sanitizer assertions also cover captured origin despite live position mutation, ordered lazy backup resolution, and retained accepted prefix on a later resolver failure. This replay is kernel evidence; it is not whole-wrapper ARM32 parity for populated source scripts.

The host receipt hashes the actual historical central DSO closure copied to a private snapshot, verifies its private ldd resolution and stable hashes, and records new V5 source hashes. It deliberately makes no attribution of those historical DSOs to current central source. Central rebuilds after the snapshot do not affect this proof.

Required integration work remains: supply live room/object resolution, native query service producers, controller LookAt, same-VM object methods, actual reachable SkillCombatRoll/FX, and application/debug policies. Retain failure semantics rather than no-op success. V4 animation metadata delivery is a separate existing owner. These files are private and are not added to shared CMake by this handoff.
