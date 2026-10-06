import pathlib,subprocess,sys
root=pathlib.Path(__file__).resolve().parents[3];sdk=pathlib.Path(r'C:\Users\adamc\AppData\Local\Android\Sdk')
kind=sys.argv[1];assert kind in ('host','sanitizers','x86_64','arm64-v8a')
source=root/'port/level-loader/tests/cmake-receiver-transport';build=root.parent/'build'/('receiver-transport-'+kind)
target='dh2_loader_module_graph_source_probe';linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
if kind in ('host','sanitizers'):
    base=['wsl.exe','-d','Ubuntu','--','cmake'];args=['-S',linux(source),'-B',linux(build),'-G','Ninja','-DCMAKE_BUILD_TYPE=Debug']
    if kind=='sanitizers':args+=['-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer','-DCMAKE_C_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer','-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined','-DCMAKE_SHARED_LINKER_FLAGS=-fsanitize=address,undefined']
else:
    base=[str(sdk/'cmake/3.22.1/bin/cmake.exe')];args=['-S',str(source),'-B',str(build),'-G','Ninja','-DCMAKE_BUILD_TYPE=Debug',
        '-DCMAKE_MAKE_PROGRAM='+str(sdk/'cmake/3.22.1/bin/ninja.exe'),'-DCMAKE_TOOLCHAIN_FILE='+str(sdk/'ndk/29.0.14206865/build/cmake/android.toolchain.cmake'),
        '-DANDROID_ABI='+kind,'-DANDROID_PLATFORM=android-24','-DANDROID_STL=c++_static']
subprocess.run(base+args,check=True)
subprocess.run(base+['--build',linux(build) if kind in ('host','sanitizers') else str(build),'--target',target,'dh2_loader_config_across_regression','dh2_loader_room_zone_across_regression','dh2_loader_character_family_prefix_probe','dh2_loader_application_rng_probe','dh2_loader_chest_data_probe','dh2_loader_character_position_kernel_probe','dh2_loader_character_position_owner_probe','dh2_loader_character_position_prefix_probe','dh2_loader_dummy_source_probe','dh2_loader_same_level_ctor_probe','dh2_loader_swamp_families_source_probe','dh2_loader_destructible_source_probe','dh2_loader_native_ctor_bindings_probe','dh2_loader_container_callbacks_v21_probe','dh2_loader_trigger_zone_v22_probe','-j','2'],check=True)
