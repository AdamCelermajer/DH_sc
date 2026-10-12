"""Move the accepted stable bug IDs to history without deleting their evidence."""
from pathlib import Path

root = Path('C:/Users/adamc/Desktop/workspace/DH_sc')
fixes = {
119: 'Authentic admitted sale PickupGold callback, including native refusal/duplicate ordering.',
120: 'Original purchase confirmation and post-attempt PickupGold, maximum-loop and refusal behavior.',
121: 'Original point-allocation MenuSpending callback and admitted mutation ordering.',
122: 'Confirmed reachable merchant/page/back/equipment/skill/Faery cue omissions wired with authentic samples.',
123: 'Distinct append-generated merchant instances retain source values, identified state and ordered powers through trades and Continue.',
124: 'PC wheel, key and vertical drag navigation with correct selection/details and outside/modal/release suppression.',
125: 'Canonical powered Ring1/2 details, real gear effects and persistence; bare Rusted Ring base stats remain empty.',
126: 'Authenticated ordinary old-v1 admission and captured v2 admission restore; native bodies/targetability return, disabled actors stay disabled; narrow dead-timer save exception.',
127: 'Exact brown Champion source combat admission and per-placement legacy roster migration; foreign actors still reject.',
128: 'Checkpoint native-body restoration makes source Conscript Armor walkover pickup work once after death/Continue.',
129: 'Current Equipment consumer rebind and overview entry reset survive restore and close/reopen.',
130: 'Current source potion vital/stock admission, typed PotionDrink output and postfight/postrevive consumption/regeneration.',
131: 'Source CanRespawn/Limbus facts, atomic restore/rebind and native physical initialization restore checkpoint movement/combat/consumers.',
132: 'Exact campaign lifecycle entry completes the typed skill registry; postfight BashDown Use/Post and duplicates remain strict.',
133: 'Original Drop/Transmute Yes/No dialog, exact shade, full current-item lease, one-shot actions/cues and restore cancellation.',
134: 'Authentic power-count FontPalette colors and existing markup parser; Equipment/Sell names render without literal tags.',
135: 'Authenticated canonical powers project before initial selected equipment binding, preserving strict Save/Continue signatures.',
136: 'Current source camera providers bind before any active original-camera consumer, including ordinary Continue without scripted commands.'
}
tracker = root/'docs/BUGS-AND-IMPLEMENTATION.md'
lines = tracker.read_text(encoding='utf-8').splitlines()
removed = []
kept = []
for line in lines:
    matches = [number for number in fixes if line.startswith(f'| B{number:03d} |')]
    if matches:
        removed += matches
    else:
        kept.append(line)
assert sorted(removed) == list(fixes), 'Each accepted row must exist exactly once'
banner = ('Current accepted Windows release: **v21.3**, EXE `276C83146351F41E879F296143F8F59F74A0D3552F224D9015AEF87C3037779A`, '
          'source51c4ac5c, 177/177 tests; B119–B136 moved to RESOLVED-BUGS.md. '
          'Accepted source/runtime/package scope: `coordination/v21.3/FINAL-REVIEW.md`. '
          'Previous packages and saves remain preserved; older sections below are historical.\n')
kept.insert(2, banner)
kept = [line.replace('## Current release — V19 Preview 13', '## Historical release — V19 Preview 13') for line in kept]
tracker.write_text('\n'.join(kept)+'\n', encoding='utf-8')
history = root/'docs/RESOLVED-BUGS.md'
text = history.read_text(encoding='utf-8')
assert '### v21.3 — 2026-10-12' not in text
text += ('\n### v21.3 — 2026-10-12\n\n'
         'Root accepted source51c4ac5c and shipping EXE SHA256 `276C83146351F41E879F296143F8F59F74A0D3552F224D9015AEF87C3037779A`; '
         '177/177 complete tests and independent audio/menu/gameplay/package reviews pass. '
         'See `coordination/v21.3/FINAL-REVIEW.md`, which distinguishes frozen production proofs, reused unchanged-source evidence and diagnostic camera-only B126 rendering evidence. '
         'Original saves and accepted packages were preserved; seven progress files copied separately with hashes.\n\n'
         '| ID | Fix | Evidence | Build |\n|---|---|---|---|\n')
for number, fix in fixes.items():
    evidence = 'B122-report.md' if number == 122 else f'B{number:03d}-report.md'
    text += f'| B{number:03d} | {fix} | `coordination/v21.3/{evidence}` and final domain review. | v21.3 / 276C8314 |\n'
text += ('\nQualifications remain in each report: no acoustic parity or all-Act1 completion claim; conditional/unreached audio and TypeN focus-in remain unproved; '
         'merchant post-exhaustion UI soak is future coverage; screenshot ring powers are unknown; '
         'B126 direct renderer observer did not kill its moth before restore and is combined with separate real production lethal/restore proofs. '
         'Earlier fire, lighting/water and F9 targeting caveats remain open.\n')
history.write_text(text, encoding='utf-8')
print('Moved B119–B136 to resolved history after accepted v21.3 checks')
