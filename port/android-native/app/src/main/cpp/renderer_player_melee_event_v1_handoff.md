Retain these members in PlayerSkillsRuntime:

```cpp
std::unique_ptr<dh2::character::skills::CharacterWorldMeleeAttackV1> melee_event;
std::uint32_t melee_on_attack{};
static void bind_melee_event(PlayerSkillsRuntime&);
static int melee_event_service(void*,const dh2::character::skills::MeleeAnimationRequestV1*,dh2::character::skills::MeleeAnimationResponseV1*);
int authored_melee_event(const char*);
```

Include `character_melee_animation_event_v1.hpp` and the new `.inc` after existing attack helpers. Compile `character_melee_animation_event_v1.cpp`. Call `bind_melee_event(t)` after attack/registered execution construction. The normal authored event delivery calls `authored_melee_event(actualEventName)` exactly once; do not additionally call the development `player_authored_event` damage path or the old skill-state event dispatcher on that event. Animation step begin/end still use the dedicated original step owner.

The dispatcher preserves step index/count before every event, ev_/an_/fx_/sfx_ prefix order, state-gated ranged/melee/skill/spell/interaction dispatch, and original Debug before actions. Normal melee enters original AI_CanAttack, selected AIS OnAttack, actual target Character conversion, F_MeleeAttack with mask022aab5 and exact weapon category, then the registered-world Apply owner. Character408 is embedded CharAI3c8+40, so its look-target borrow is the same canonical target field.

This handoff deliberately fails reached object-animation, explicit fx_/sfx_, projectile, spell and interaction providers until their original backends are composed. Registered combat application likewise preserves its actual HP prefix and missing FX/status/text/audio/AI tail errors. Do not describe this intermediate adapter as complete melee execution until all reached application providers are connected. No alternate targets, HP stores or random generators are created.
