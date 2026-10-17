# Borrowed dead-focus services

The selected adapter receives the existing Character identity, property view,
BuffOwner, source CharAI byte4d, PlayerCombat FX fields, Debug runtime and retained
skill preparation/VM calls. It creates no Character fields, inventory, property
store, Scene, timer store, frame or VM.

The original ELF hash and scoped function byte hashes are in
`original-functions.json`. Its ARM listing pins CSDead focus, CancelSneaking,
AI_CancelSkill, FX methods, RemoveAllBuffs, constructors and the relevant AI
update callers. PlayerManager's large method is hashed in full; only its changed
skill-member/level loop and final UpdateAllSkills branch are listed. Direct
`.text` ARM B/BL xrefs are captured; indirect callers are not claimed complete.

Dead focus first delivers the appended mandatory Debug prelude before flags or
LookAt. The real retained Debug owner executes both load/GetSwitch pairs.
CancelSneaking queries actual IsPlayer, removes source key146 with NULL instance,
then writes canonical byte415=1. Constructor evidence proves this is embedded
CharAI+4d, initialized1. It then reads resolved property198. Nonpositive values
skip skill services; positive values require the existing immutable SkillTables,
preparation and same VM Active/Pre calls. The selected slot is the list index.

Self/state/highlight FX fields1484/148c/14a0 are constructor-zero. Zero handles
execute authentic empty branches. Nonzero handles require a real synchronous
DropAnimatedFX manager; failure retains prior effects. Highlight additionally
clears its field after successful delivery. RemoveAllBuffs uses the same owner
and always performs its source recalc, including an empty map.

The focused host test uses actual cache classes, AI rows, tables and scripts,
one BuffOwner and Coordinator, and the real Debug file/map owner. Its remaining
controller/animation/Character-event dependencies and positive FX drop are
declared test callees. A positive cached Sneak read is explicitly injected only
to exercise provider boundaries; this does not supply a native Sneaking producer.
Existing state oracle replay counts the new mandatory prelude separately because
the historical oracle fixtures Debug calls. Its failure test proves that a Debug
throw leaves the reached incoming state and elapsed reset, without dead flags or
later services. The adapter catches provider failures; the native receiver must
throw when its status is not complete so later source services do not run.

Skill updates are caused by InitScriptProcess/InitFinal, IncSkill, ChangeFaery,
IncFaeryLevel, ReloadSkills/AI_ReloadSkills, Revive and PlayerManager's actual
changed skill-level loop. UpdateSkills additionally belongs to SG_SetSkillInSlot.
CharAI Update and AISExternal OnUpdate do not directly call UpdateAllSkills.
The renderer's former unconditional per-frame skill update must remain removed.
Real AI frame binding still requires target/master/aggro, virtual AIS/VM and
source controller/flags/zoning gates. Dead flags retain bit100; locked controller
is a real guard unless forced.

Native Player kill count/trophy/local/online continuation remains open. Current
combat application has already reached dead1/HP0; calling full Kill again would
skip its outer branch. A future continuation must enter after that actual prefix
without clearing dead or inventing rewards, trophies, loot or online success.

Run the focused selected-library gate with `run_character_dead_focus_services_v1_host.py`
and a short output directory on Windows. `--state-reference` optionally replays a
separately generated existing bounded state oracle against the selected DSO.
No positive native FX, full AI frame or full Kill body is claimed.
