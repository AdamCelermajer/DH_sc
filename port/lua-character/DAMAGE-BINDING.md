# Damage binding on owned property actors

`ApplyNonplayerHit(rawDamage, policy, deathReason)` is an authored development
method. Its name and arguments are not claims about the original Lua registration.
It applies the original-matched non-player damage projection using the actor's
retained property dataset generation and empty buffs.

Returns four values: processed boolean, death-requested boolean, signed whole
damage integer, and resulting death reason. It does not commit a dead flag or
run the original kill command target. The caller supplies a new death reason
input on each call; the property actor stores no original death-state owner.

`rawDamage` must be a finite float32 number in [0, 2^32), truncated to an unsigned
raw fixed amount. `deathReason` must be an exact signed 32-bit integer. Required
policy fields are read without invoking table metamethods:

| Field | Type |
| --- | --- |
| target_dead | boolean |
| target_monster | boolean |
| local_player_alive | boolean |
| online | boolean |
| manager_present | boolean |
| manager_mode | exact signed 32-bit integer |
| monster_invincible | boolean |
| force_kill_config | boolean |
| force_kill_switch | boolean |
| target_network | boolean |

Bad arity, missing fields, wrong types, unsafe casts and malformed property data
raise a controlled error before mutation. These are explicit boundary inputs,
not a recovered multiplayer, configuration, AI or Character implementation.

Runtime selftests cover ordinary and lethal hits, already-dead targets, local
player suppression, monster invincibility, network suppression/mode 5, forced
death reason retention, argument rollback and recovery. The standalone module
has 2,835 original ARM32/source ARM64/host comparisons; see
[damage evidence](../character-damage/README.md).

The recovered combat script can feed its raw damage into this projection, but
the original full attack result flags, buffs, DoT, leech, events, world, enemy
behavior, progression and savegame loop remain pending.
