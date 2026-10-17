# Player skill preparation adapter

Adam c3ae797's stable nullable-instance/vector/Arguments/path ownership adapted to our immutable decoded Skill/Faery tables. Existing SetSkillsAndSpells1916B, GetCharFaery540B and skill constructor336B reused; zero new complete-body credit. Real Base/class rows resolve lists14/21/27 and1/2/3. Each has16skill slots+5faery slots. Mandatory Debug/AIS/Lua services remain external; tests read unchanged scripts but do not execute Lua or wire Android.

Original getters: SkillListId3bc5c0/60, SkillList3bc5fc/48, Skill3bc784/220; FaeryListId3ae5a0/60, FaeryList3ae5dc/48. Signed fallbacks Skill3/Faery0. Immutable tables pin rows/strings; unsafe row references fail as port guards. Provider errors retain completed effects; owner/property/output lifetimes stay external.

Run `python port/level-world/tests/run_character_player_skills_preparation_v3_host.py --compiler <g++> --cache <files> --original-elf <ELF>`. Source callback bodies and player VM/FSM/cooldown/buff integration are pending.

Adam's full player closure requires missing current-VM APIs: `create_empty`, `open_source_library` (retained stack results), `stack_size`, `load_source_file`, `required_failure_epoch`, `call_source_status_objects`, `call_first_source_v1`, `call_indexed_source_v3`, source-object/scoped/Include APIs and `dh2_script_callback_scope`. Current `call_discard_source` can discard genuine returns but cannot replace required-failure distinction or typed return observers. These are future narrow changes to the same VM, not a second runtime.
