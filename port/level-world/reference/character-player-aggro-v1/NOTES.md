# Original AISPlayer aggro lifecycle

Source ELF SHA256 is
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Captured OnAggro3ddb70, OnDeAggro3dde48, selected script constructors3cd11c and
3ccfe4, CharAI dispatcher3d20c8 and world constructors retain the original
instructions. Original bodies call the inherited empty AISDefault body, query
`isTracingAggroCount`, update source counters+d0/+d4, follow actual online/local
player gates, query current level and other CharAI weight, and conditionally
change music. OnDeAggro's pending+c4/c8/cc storage is a vector, not a timer.
Constructor3ccf9c initializes it; clearing sets end to begin without releasing
the source capacity.

SetAggro3d7c34 invokes targetCharacter.CharAI.OnAggro(ownerCharacter). The skill
defender NPC therefore notifies attacker Player, requiring the actual selected
AISPlayer body. It must not substitute the NPC inherited empty method.

Level+38 is LevelConfig, not collada Scene. SetLevelConfig3f1518 writes it;
DeclareProperties3f4cd8 registers combat_music_enabled byte+1c8 with default1.
The actual Crypt backup MLX contains this authored LevelConfig attribute.
Level ctor3f319c initializes its config pointer null, but a loaded level needs
the source SetLevelConfig continuation. Missing config on the reached source
path is an assertion, not a disabled-music default.

VoxSoundManager ctor36c818 initializes ambient31=1 and levelMusic32=0. Its full
constructor/audio file loading is not replaced by these two fields. Actual
music delivery remains a mandatory backend whenever SetMusicState/PlayMusic is
reached. The combat threshold is DesignSettingsTable first-row word+14,
available from the existing DesignSettingsOwner as field4; it is not generated.
CharAI ctor3cede8 initializes its weight+14=-1; source Init must produce its
subsequent live value.

The original-instruction oracle executes 376 valid world/audio fixture cases.
The native host compares source count, weight, music flags, vector length and
ordered original calls for all cases, plus unavailable-service prefix behavior,
with ASAN/UBSAN at O1 and O2. World, Debug absent-file and audio callbacks are
explicit fixtures; production audio completion is not claimed.

This new helper does not alter the trophy/execution cpp/hpp captured by parent
checkpoint4156b785. Their retained source snapshot remains separately valid.
