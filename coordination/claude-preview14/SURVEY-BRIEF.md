# Preview 14 feature survey brief (READ-ONLY phase)

User direction (2026-10-10): feature work resumes. Preview 14 goal = the FULL character menu working and faithful (Stats/Skills point attribution with real effect on the character, Faery page with unlock state tied to the character, Quest log + a coded quest system, Map page + HUD minimap, Equipment pages with auto-equip of everything), the main-menu character-slot metadata (current map/act/difficulty) connected to the character state, and ground drops that show in the world and can be picked up into the inventory (this must come before chests/pots work).

You are a SURVEY worker for ONE stream. Do NOT edit any file under port/, docs/ or tools/. You may create scratch under `.local-inputs/claude-preview14/<stream>/` and write your report to `coordination/claude-preview14/<STREAM>-survey.md`. The root reads all reports and then launches implementation workers, so be precise and honest.

Rules: COMMON-BRIEF.md (coordination/claude-preview12/COMMON-BRIEF.md) for evidence sources (IDA pseudocode, reference video via ffmpeg, existing reports). QUIET RULES (coordination/claude-preview13/QUIET-RULES.md) are mandatory for any EXE run: only `port/windows-foundation/tools/quiet_run.ps1` (hidden desktop, silent, parallel); never start the game directly. Use the latest EXE `.local-inputs/windows-source-clock-v19-preview-13-rc2/dh-foundation.exe` and the Preview 12 package assets/args patterns (`.local-inputs/claude-preview13/quiet-test/`, `verify-final/runs/*/run.args`, `.local-inputs/claude-preview13/p/b016/` for saves with custom items). Keep your context lean (small model): grep, limited reads.

Report structure (keep it under ~250 lines):
A. EXISTING: what already exists for this stream (files/modules with one-line purpose) and its status per item: source-only / component-tested / integrated in main.cpp / verified in the EXE (say how you verified; run it if cheap).
B. ORIGINAL BEHAVIOUR: what the original game does (IDA functions/addresses, SWF actions, data tables; reference video timestamps with what you actually saw - mark inference vs observation).
C. GAPS: concrete list of what is missing or wrong, each with evidence (log line, capture, code location).
D. DESIGN: the minimal reusable implementation: data model, which existing owners/state it reads/writes (CharacterState, GameSave, SaveStore, PropertyRules...), new files, and the exact integration hooks needed in main.cpp / CMakeLists (list each hunk location by nearby anchor). State persistence/save-format implications (versioning, never break existing saves).
E. WORK BREAKDOWN: 2-5 implementation tasks sized S/M/L (S<2h, M<half day, L>half day of a careful engineer), in dependency order, each with its focused test plan and its verifier script idea (quiet batch).
F. DEPENDENCIES/CONFLICTS with the other streams (list: skills/stats, faery, quests, map, equipment, main-menu metadata, drops) and shared files.
G. OPEN QUESTIONS for the user (only things you cannot decide from evidence).
