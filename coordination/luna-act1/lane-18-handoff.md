# Lane 18 handoff: quests, conditions, events

Implemented a source correction in `port/level-world/native_quest_runtime_v76.cpp`: the `Quest::SetState(Closed)` path removes talk markers for the quest's objective list only. The previous native path also removed the end objective marker, while IDA's `Quest::SetState` at `0x480c78` calls `ObjectiveList::RemoveObjectiveMarkers` on the list at `this+0x2c`; the end objective is a separate object at `this+0x1c`. This keeps the authored end objective's marker lifecycle out of that list operation.

Reused the existing saved-state owners and runtime wiring; no parallel quest or event state was added:

- The canonical `CharacterMenuQuestsV51` is obtained from the character profile bootstrap and retains its actual `PlayerSavegame`. `QuestConditionCompile` selects the current regular/volatile saved `QuestSavegame` collection and resolves each actual quest identity before compiling/updating it.
- `NativeQuestRuntimeV76` projects immutable authored accept/objective/end/reward/prerequisite rows while retaining the actual saved quest and objective persistence receivers. It handles quest state transitions, authored scripts, objective registration, rewards, condition evaluation and talk marker install/removal.
- `GameEventRuntimeV75` is the shared objective/event dispatcher. Level event dispatch borrows the actual completed Level's inherited `EventManager` after its source Load, and preserves the captured Level receiver across event construction/raise.
- The marker transport resolves the first authored NPC match from the current ordered Character list and uses the current design settings and fresh campaign FX owner. Condition/objective/event providers remain required dependencies; absent providers fail with the existing explicit error path.

Root wiring to preserve: call `source_campaign_condition_dependencies_v76` when binding condition dependencies; bind actual event dependencies through `bind_source_campaign_event_dependencies_v75`; bind marker FX/visual/animator providers with `bind_source_campaign_quest_markers_v76`; keep `borrow_source_campaign_events_v75` behind the actual level manager's completed source Load. Character save/update enters through `source_campaign_character_sg_update_v108`.

IDA cross-checks used: `Quest::Compile` `0x480178`, `Quest::SetState` `0x480c78`, `Quest::Update` `0x481818`, `Quest::IsVolatileState` `0x47f70c`, `ConditionList::Eval/AssignPyData` `0x478704/0x478914`, and `QuestMoveInZone::OnCollisionBegins` `0x396120`. IDA pseudocode was treated as inferred and checked against the exported ARM assembly; no Ghidra output was used.

Remaining explicit limitation: online activation/current-act quest messages still require their original networking providers. Talk marker runtime also requires the root's actual fresh-instance FX/visual/animator bindings. No build, test, emulator, or gameplay verification was performed; this is a narrow source correction and wiring handoff, not proof that Act 1 plays.
