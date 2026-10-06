import pathlib, subprocess, sys
root = pathlib.Path(__file__).resolve().parents[3] if 'tools' == pathlib.Path(__file__).parent.name else pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
build_root = root.parent / 'build'
source = root / 'port/level-loader/tests/cmake-connected-owner'
target = 'dh2_loader_canonical_module_files_probe'
kind = sys.argv[1]
if kind in ('host', 'sanitizers'):
    linux = lambda p: '/mnt/c/' + str(p).replace('\\', '/')[3:]
    build = build_root / ('connected-owner-' + kind)
    options = ['-DCMAKE_BUILD_TYPE=Debug']
    if kind == 'sanitizers':
        options += ['-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer',
                    '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined']
    subprocess.run(['wsl.exe', '-d', 'Ubuntu', '--', 'cmake', '-S', linux(source), '-B', linux(build)] + options, check=True)
    subprocess.run(['wsl.exe', '-d', 'Ubuntu', '--', 'cmake', '--build', linux(build), '--target', target, '-j', '4'], check=True)
else:
    assert kind in ('x86_64', 'arm64-v8a')
    sdk = pathlib.Path(r'C:\Users\adamc\AppData\Local\Android\Sdk')
    build = build_root / ('connected-owner-android-' + kind)
    cmake = sdk / 'cmake/3.22.1/bin/cmake.exe'
    subprocess.run([str(cmake), '-S', str(source), '-B', str(build), '-G', 'Ninja',
        '-DCMAKE_MAKE_PROGRAM=' + str(sdk / 'cmake/3.22.1/bin/ninja.exe'),
        '-DCMAKE_TOOLCHAIN_FILE=' + str(sdk / 'ndk/29.0.14206865/build/cmake/android.toolchain.cmake'),
        '-DANDROID_ABI=' + kind, '-DANDROID_PLATFORM=android-24', '-DANDROID_STL=c++_static'], check=True)
    subprocess.run([str(cmake), '--build', str(build), '--target', target, '-j', '4'], check=True)
