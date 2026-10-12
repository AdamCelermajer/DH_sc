import pathlib,subprocess,sys
root=pathlib.Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc')
sdk=pathlib.Path(r'C:\Users\adamc\AppData\Local\Android\Sdk')
abi=sys.argv[1];assert abi in ('x86_64','arm64-v8a')
build=root.parent/'build'/('connected-owner-android-'+abi)
cmake=sdk/'cmake/3.22.1/bin/cmake.exe'
subprocess.run([str(cmake),'-S',str(root/'port/level-loader/tests/cmake-connected-owner'),'-B',str(build),'-G','Ninja',
 '-DCMAKE_MAKE_PROGRAM='+str(sdk/'cmake/3.22.1/bin/ninja.exe'),
 '-DCMAKE_TOOLCHAIN_FILE='+str(sdk/'ndk/29.0.14206865/build/cmake/android.toolchain.cmake'),
 '-DANDROID_ABI='+abi,'-DANDROID_PLATFORM=android-24','-DANDROID_STL=c++_static'],check=True)
subprocess.run([str(cmake),'--build',str(build),'--target','dh2_loader_canonical_level_context_probe','-j','4'],check=True)
