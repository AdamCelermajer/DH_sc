from pathlib import Path

repo = Path(__file__).resolve().parents[2]
scratch = repo / '.local-inputs/claude-v20.1/fix097b'
scratch.mkdir(parents=True, exist_ok=True)
for name in ['runtime_skill_activation_session_v1', 'runtime_skills_text_v1', 'runtime_skill_mana_v1', 'runtime_skill_session_training_v1']:
    source = repo / f'port/windows-foundation/features/generic_skills/run_{name}_tests.ps1'
    if not source.exists():
        continue
    script = source.read_text()
    script = script.replace('$PSScriptRoot', "'" + source.parent.as_posix() + "'")
    script = script.replace("$buildRoot = Join-Path $repoRoot '.local-inputs\\windows-foundation-build'", "$buildRoot = 'C:/Users/adamc/Desktop/workspace/DH_wt/build-fix097'")
    script = script.replace("$build = Join-Path $repoRoot '.local-inputs\\windows-foundation-build'", "$build = 'C:/Users/adamc/Desktop/workspace/DH_wt/build-fix097'")
    script = script.replace('$output = & $exe $repoRoot', "$output = & $exe 'C:/Users/adamc/Desktop/workspace/DH_sc'")
    if name == 'runtime_skill_activation_session_v1':
        script = script.replace("$output = & $exe 'C:/Users/adamc/Desktop/workspace/DH_sc'", "$output = & $exe 'C:/Users/adamc/Desktop/workspace/DH_sc' 1 'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-v20.1/fix097b/test-output'")
    script = script.replace("$exe = Join-Path '" + source.parent.as_posix() + "' 'runtime_skills_text_v1_tests.exe'", "$exe = '" + (scratch / 'runtime_skills_text_v1_tests.exe').as_posix() + "'")
    script = script.replace("$exe = Join-Path '" + source.parent.as_posix() + "' 'runtime_skill_mana_v1_tests.exe'", "$exe = '" + (scratch / 'runtime_skill_mana_v1_tests.exe').as_posix() + "'")
    # Preserve own compiler outputs outside shared test directories.
    for old in ['windows-skill-cast-session-test', 'windows-skills-text-test', 'windows-skill-mana-test', 'windows-skill-session-training-test']:
        script = script.replace(old, 'claude-v20.1/fix097b/test-output')
    (scratch / (name + '.ps1')).write_text(script)
