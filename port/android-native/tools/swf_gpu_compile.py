"""Record Android compile-only evidence for the native SWF GPU backend.

This does not build/install an APK or validate GPU drawing. Source snapshots
and both Android ABI compiler outputs are recorded without overwriting proofs.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    assert not args.output.exists(), 'Preserve earlier compiler evidence'
    cpp = ROOT/'port/android-native/app/src/main/cpp'
    files = [cpp/name for name in ('swf_gpu.hpp','swf_gpu.cpp','authored_shader_program.hpp',
                                   'authored_shader_program.cpp','original_ui_assets.hpp',
                                   'original_ui_assets.cpp','original_ui_asset_catalog.inc','CMakeLists.txt')]
    files += [ROOT/'port/engine-ui/swf_movie.hpp',ROOT/'port/scene-materials/shader_sources.hpp',
              ROOT/'port/scene-materials/swf_texture.hpp',ROOT/'port/asset-payloads/sha256.hpp',
              ROOT/'port/android-native/reports/original-ui-catalog.json',Path(__file__)]
    snapshot = {p.relative_to(ROOT).as_posix(): sha(p) for p in files}
    commands = []
    for abi, target in (('arm64-v8a','aarch64-linux-android26'),('x86_64','x86_64-linux-android26')):
        command = [str(args.compiler), '--target='+target, '-std=c++17','-Wall','-Wextra','-Werror',
                   '-fno-fast-math','-ffp-contract=off','-fsyntax-only',
                   '-I',str(ROOT/'port/scene-materials'),'-I',str(ROOT/'port/engine-ui'),
                   str(cpp/'authored_shader_program.cpp'),str(cpp/'swf_gpu.cpp'),
                   str(cpp/'original_ui_assets.cpp')]
        process = subprocess.run(command,cwd=ROOT,capture_output=True,text=True,timeout=60)
        commands.append(dict(abi=abi,arguments=command,returncode=process.returncode,
                             stdout=process.stdout,stderr=process.stderr))
        assert process.returncode == 0 and not process.stderr.strip(),commands[-1]
    assert snapshot == {p.relative_to(ROOT).as_posix(): sha(p) for p in files}, 'Compiler inputs changed during verification'
    result = dict(validation='PASS',source_sha256=snapshot,compiler_sha256=sha(args.compiler),commands=commands,
                  compile_only=True,linked=False,packaged_apk=False,live_gpu=False,
                  original_full_render_parity=False,
                  scope='Modern GLES2 native SWF draw sink and exact bundled UI resource reader; source shader/known immediate color and blend connections; retained textures and shared-stencil query target')
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(validation='PASS',abis=[row['abi'] for row in commands],live_gpu=False)))


if __name__ == '__main__':
    main()
