Combat Ctrl_Kill owner V1
========================

This successor replaces the bounded legacy application core's artificial
ds.dead=1 prefix with actual dh2_character_ctrl_kill at the lethal HitFor point.
Old APIs/modules/tests are unchanged. Add combat_ctrl_kill_owner_v1.cpp to
level-world; no shared renderer/build files were edited here.

Retain ONE CombatCtrlKillOwnerV1 per actual target lifetime, borrowing its
source KillActor56 projection, SAME CombatActorState and KillWorld16, and whole
KillServices16 provider. KillActor56.properties must equal the live actor's
PropertyView; its dead projection must equal canonical life.dead at entry.
Keep source OID/property_id/template_id/master/tracked-target/quest flag fields
from actual initialization/registry producers. Never invent registry identities,
set suppress_quest to skip providers, manufacture Level.loot_gate150, or reset
dead to make a previously applied legacy lethal hit eligible for this owner.

combat_apply_ctrl_kill_v1(result, actualRequest, attackerIdentity, owner,
playerDefender, actualSM_IsIdle, error) retains the old bounded direct-owner
offline/single-player/default-debug/no-gold/empty-critical-skill domain. It is
not a replacement for complete source HitFor player/trophy/online or whole
F_ApplyResult FX/audio/AI. Result.complete denotes ONLY this declared core.
Status requests are returned for actual source FSM/DoT providers, not accepted
as delivered. Source FX/text/audio/trophy/AI endpoints remain external.

Exact sequencing
----------------

Combo/push fields and health prefix run first. Lethal health calls whole
Ctrl_Kill immediately: outer IsDead, inner Kill IsDead, source dead store and
HP0, current Level / early DropLoot, contributor/master/credit and counter
updates, credited XP, quest events, then source RaiseEvent2. Only successful
whole Ctrl_Kill reaches lifecycle3, dead outcome masking, common HP/MP leech
and status continuation. A failure exposes KillResult.phase and health/kill
prefix output but leaves leech_reached=false and complete=false.

Wrapper publishes the source KillActor.dead byte store to SAME canonical life
before every backend callback (and after failure), without byte-punning uint32.
Reentry after that publication observes the literal dead gate; reentry before
publication is an explicit unresolved continuation. Kernel/backing identities
must survive calls. Actual callbacks may source-Revive life; that canonical
store remains authoritative when the projection was unchanged. Root services
must also publish any contributor tracked-target clear to its actual source
OOI owner before next callback, using existing retained KillActor projections.

Binding all existing KillServices is mandatory when reached. In particular
kill_current_level returns exact retained source Level+150 projection, drop
must run world loot owner, aggro enumeration returns actual contributors and
masters, handlecast uses sameWorld registry, distribution invokes source XP
pipeline only on the original credited branch, and quest async copies full
KillQuest48 into actual queue. Nonlocal/online/dead queries must be real facts.
The wrapper does not return successful no-ops for these methods.

Testing and limits
------------------

O1/O2 ASAN+UBSAN host passes88 checks each: actual bundled property rules and
same property/life owners, complete native Ctrl_Kill kernel, ordered callback
observations, published death during recursive callback, successful core and
required DropLoot failure, lifecycle/leech barrier, no duplicate retry. Host
identity/Level/loot/XP/quest callbacks are explicit observer fixtures; this is
not a production loot/XP/quest proof. Existing character-kill original
differential remains its kernel evidence; this new ownership composition's
proof is host sequencing and authority, not a new ARM whole-method oracle.
Current renderer cannot switch until genuine metadata/world Kill services are
ready. An after-pending_death reward hook remains inappropriate.
