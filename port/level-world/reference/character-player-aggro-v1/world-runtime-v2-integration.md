# Registered-world OnAggro continuation

`renderer_player_aggro_v2.inc` lists the retained runtime members and supplies
bind/callback implementations. Bind actual selected player events plus source
AISPlayer supplemental fields, actual registered NPC events, same Debug/files,
selected XML LevelConfig projection and Vox constructor-backed music fields.
Set WorldSkillCombatBackendsV6.aggro_context=&t and
aggro_event=player_aggro_callback_v2 before constructing execution.

Set/AddAggro notification invokes TARGET.OnAggro(OWNER). The complete inherited
AISDefault receiver is a proven bx-lr. Player OnAggro performs actual Debug,
counter, online/local, current level, signed weight, music and final Debug order.
Unsupported other notifications retain required failures.

Weight correction: original3ddc44 calls Character::GetCharAI3a3024, a STATIC
table getter; it does not return embedded runtime CharAI atCharacter+3c8.
GetCharAIId3a2fec reads Character+ffc, same resolved[2], fallback8. GetCharAI
selects source row stride44; OnAggro loads its +14 (decoded AiProps.flags).
Use those signed word bits, as the include does. Runtime CharAI ctor+14=-1 is
a distinct field and is not the aggro weight producer.

Source threshold is first DesignSettings row+14, decoded field4. Source config
combat_music_enabled is actual LevelConfig+1c8. Crypt's checked projection asset
worlds/crypt01-level-config-music-v1.bin SHA256
b037b66f6d1459ebd9688676dc2093e19f1bc558ba648ff3020eae842c950760
comes from already-bundled x07_crypt_backup.mlx SHA256
706b1e23ea7ca9c3c1695d6cf8679c177bdfb70e71d202e351a2b0c8564023c7.
The native projection owner does not claim the full original XML object factory.
Caller must bind the selected level's actual source projection, not reuse this
Crypt record for another level.

VoxMusicFieldsV1 is only its source music-field projection. Constructor current
music=-1 makes SetMusicState return before sound-bank/audio calls. Nonnegative
music must use real audio services; no-track is never forced after a live write.
Music-ID field is unavailable in this include and remains null: OnAggro does not
read it; the later OnDeAggro PlayMusic branch still requires its actual producer.
Trace/PlayMusic services remain required when reached.

Host17checks O1/O2 ASAN/UBSAN cover actual Crypt projection, target receiver,
null/invalid owners, no-track source branch, signed word policy, and positive
track mandatory-failure prefix. Fixtures supply online/weight/threshold query
values; this is not proof of audio delivery or the renderer integration.
