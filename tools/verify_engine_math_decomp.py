#!/usr/bin/env python3
"""Run a focused Ghidra decompilation check for the 20 reconstructed math functions."""
import argparse
import hashlib
import json
import os
import subprocess
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile


def write_modes(elf_path: Path, path: Path) -> None:
    with elf_path.open('rb') as stream:
        elf = ELFFile(stream)
        table = elf.get_section_by_name('.symtab') or elf.get_section_by_name('.dynsym')
        rows = sorted({(symbol['st_value'], symbol['st_size'])
                       for symbol in table.iter_symbols()
                       if symbol['st_info']['type'] == 'STT_FUNC'
                       and symbol['st_shndx'] != 'SHN_UNDEF'
                       and symbol['st_size'] > 0})
    path.write_text(''.join(f'{address:x}\t{size}\n' for address, size in rows),
                    encoding='utf-8')


def headless_command(launcher: Path, project_dir: Path, scripts: Path,
                     elf_path: Path, modes_path: Path, targets_path: Path,
                     report_path: Path, timeout_seconds: int, workers: int,
                     reuse_project: bool) -> list[str]:
    command = [str(launcher), str(project_dir), 'dh2-engine-math-pilot',
               '-scriptPath', str(scripts)]
    if reuse_project:
        # The fresh import already repaired ARM/Thumb ranges and helper types.
        # Reopen the saved program without auto-analysis, then reapply only the
        # small idempotent helper signature fix and decompile the 20 targets.
        command += ['-process', 'libDungeonHunter2.so', '-noanalysis',
                    '-postScript', 'FixArmEabiFloatHelpers.java',
                    '-postScript', 'VerifyEngineMathDecomp.java', str(targets_path),
                    str(report_path), str(timeout_seconds)]
        return command

    command += [
        '-import', str(elf_path), '-overwrite',
        '-preScript', 'ConfigureRecovery.java', str(modes_path),
        '-preScript', 'FixArmEabiFloatHelpers.java',
        '-postScript', 'FixThumbRanges.java', str(modes_path),
        '-postScript', 'EnsureNamedFunctions.java', str(modes_path),
        '-postScript', 'FixArmEabiFloatHelpers.java',
        '-postScript', 'VerifyEngineMathDecomp.java', str(targets_path),
        str(report_path), str(timeout_seconds),
        '-analysisTimeoutPerFile', '900', '-max-cpu', str(workers),
    ]
    return command


def write_project_manifest(path: Path, data: dict) -> None:
    temporary = path.with_suffix(path.suffix + '.tmp')
    temporary.write_text(json.dumps(data, indent=2) + '\n', encoding='utf-8')
    temporary.replace(path)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--elf', type=Path, required=True,
                        help='original libDungeonHunter2.so (not a rebuilt library)')
    parser.add_argument('--ghidra-home', type=Path, required=True,
                        help='Ghidra 11.0.3 installation directory')
    parser.add_argument('--jdk-home', type=Path,
                        help='optional JDK home; otherwise Ghidra launch.properties applies')
    parser.add_argument('--work-dir', type=Path, required=True,
                        help='private output/work directory outside the repository')
    parser.add_argument('--reuse-project', action='store_true',
                        help='reuse the project from a successful fresh run; decompile only the 20 targets')
    parser.add_argument('--repo-root', type=Path,
                        default=Path(__file__).resolve().parents[1])
    parser.add_argument('--workers', type=int, default=4)
    parser.add_argument('--timeout-seconds', type=int, default=60)
    args = parser.parse_args()

    elf_path = args.elf.resolve()
    ghidra_home = args.ghidra_home.resolve()
    repo_root = args.repo_root.resolve()
    work_dir = args.work_dir.resolve()
    try:
        work_dir.relative_to(repo_root)
    except ValueError:
        pass
    else:
        parser.error('--work-dir must be outside the repository')
    if not elf_path.is_file():
        parser.error(f'ELF input does not exist: {elf_path}')
    if not (ghidra_home / 'support' / 'analyzeHeadless.bat').is_file() and not (
            ghidra_home / 'support' / 'analyzeHeadless').is_file():
        parser.error(f'No analyzeHeadless launcher under {ghidra_home / "support"}')
    if args.workers < 1 or args.timeout_seconds < 1:
        parser.error('workers and timeout-seconds must be positive')

    metadata = json.loads((repo_root / 'port/engine-math/original-functions.json')
                          .read_text(encoding='utf-8'))
    actual_hash = hashlib.sha256(elf_path.read_bytes()).hexdigest()
    expected_hash = metadata['original_engine_sha256']
    if actual_hash != expected_hash:
        parser.error(f'original ELF SHA-256 mismatch: expected {expected_hash}, got {actual_hash}')

    ghidra_scripts = repo_root / 'tools' / 'ghidra'
    project_dir = work_dir / 'projects'
    project_dir.mkdir(parents=True, exist_ok=True)
    modes_path = work_dir / 'libDungeonHunter2.modes.tsv'
    targets_path = work_dir / 'engine-math-targets.tsv'
    report_path = work_dir / 'engine-math-decomp-validation.json'
    manifest_path = work_dir / 'dh2-engine-math-pilot.json'
    if args.reuse_project:
        project_file = project_dir / 'dh2-engine-math-pilot.gpr'
        if not project_file.is_file() or not manifest_path.is_file():
            parser.error('--reuse-project requires a successful prior fresh run in this --work-dir')
        manifest = json.loads(manifest_path.read_text(encoding='utf-8'))
        if (manifest.get('schema') != 'dh2-ghidra-math-pilot/v1' or
                manifest.get('validation') != 'PASS' or
                manifest.get('original_engine_sha256') != actual_hash or
                manifest.get('ghidra_home') != str(ghidra_home) or
                manifest.get('project_name') != 'dh2-engine-math-pilot'):
            parser.error('--reuse-project manifest does not match this ELF/Ghidra installation; run a fresh import')
    else:
        # Do not let a failed overwrite leave a stale manifest that authorizes reuse.
        manifest_path.unlink(missing_ok=True)
        write_modes(elf_path, modes_path)
    targets_path.write_text(''.join(
        f"{row['elf_address']}\t{row['original_symbol']}\n"
        for row in metadata['functions']), encoding='utf-8')

    launcher = ghidra_home / 'support' / (
        'analyzeHeadless.bat' if os.name == 'nt' else 'analyzeHeadless')
    command = headless_command(launcher, project_dir, ghidra_scripts, elf_path,
                               modes_path, targets_path, report_path,
                               args.timeout_seconds, args.workers,
                               args.reuse_project)
    env = os.environ.copy()
    if args.jdk_home:
        jdk_home = args.jdk_home.resolve()
        if not (jdk_home / 'bin' / ('java.exe' if os.name == 'nt' else 'java')).is_file():
            parser.error(f'No Java executable under {jdk_home / "bin"}')
        env['JAVA_HOME'] = str(jdk_home)
        env['JAVA_HOME_OVERRIDE'] = str(jdk_home)

    report_path.unlink(missing_ok=True)
    print('+ ' + subprocess.list2cmdline(command), flush=True)
    completed = subprocess.run(command, cwd=work_dir, env=env, check=False)
    if not report_path.is_file():
        print(f'Ghidra did not write expected report: {report_path}', file=sys.stderr)
        return completed.returncode or 1
    report = json.loads(report_path.read_text(encoding='utf-8'))
    print(json.dumps({
        'validation': report.get('validation'),
        'decompiled': report.get('decompiled'),
        'target_count': report.get('target_count'),
        'no_return_warnings': report.get('no_return_warnings'),
        'report': str(report_path),
    }, indent=2))
    passed = completed.returncode == 0 and report.get('validation') == 'PASS'
    if passed and not args.reuse_project:
        write_project_manifest(manifest_path, {
            'schema': 'dh2-ghidra-math-pilot/v1',
            'validation': 'PASS',
            'original_engine_sha256': actual_hash,
            'ghidra_home': str(ghidra_home),
            'project_name': 'dh2-engine-math-pilot',
            'program_name': 'libDungeonHunter2.so',
            'ghidra_version': report.get('ghidra_version'),
            'target_count': report.get('target_count'),
        })
    return 0 if passed else 1


if __name__ == '__main__':
    raise SystemExit(main())
