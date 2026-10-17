# Borrowed cooldown fields

Adapted Adam c3ae797 `character_skill_cooldown_v3.cpp`: Skill3b97e0/496B (assert recovery excluded), Spell3b90e4/188B. Replace his owning skill class with borrowed selected-count/slot/live field18 providers; preserve repeated index reads, captured skill slot/count and fresh spell vector. String/object getNumber stays mandatory; identities remain native-width. Zero new complete-body credit. No VM/timer/property owner or native activation.

Run `python port/level-world/tests/run_character_skill_cooldown_services_host.py --compiler <g++> --original-elf <ELF> --output <short-host-directory>`. Real selected world/game-data/VM graph, original ARM and unchanged AI+skills commons through one Coordinator/event35. Registration/check fixtures execute no combat/pre/use provider; failures retain effects. Source timers do not cancel earlier cooldowns when a later one is set; StopTimerCB does not clear the skill field.
