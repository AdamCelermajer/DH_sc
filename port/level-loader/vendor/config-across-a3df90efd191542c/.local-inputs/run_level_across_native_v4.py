from pathlib import Path
import os,sys,runpy
root=Path(__file__).resolve().parents[1]
temp=root/'.local-inputs/clang-temp-level-v4';temp.mkdir(exist_ok=True)
os.environ['TMP']=str(temp);os.environ['TEMP']=str(temp)
sys.argv=['root_android_native_test.py','canonical-level-config-across-v4','port/level-world/tests/canonical_level_config_v1.cpp','port/level-world/canonical_level_config_module_v1.cpp','port/level-world/canonical_property_map_v1.cpp','port/level-world/canonical_gameobject_base_owner_v1.cpp','port/level-world/game_object_initialization_owner_v1.cpp','port/level-world/canonical_object_manager_v1.cpp','port/level-world/module_xml_selection_v1.cpp']
sys.argv.append('port/level-world/navigation_objects.cpp')
runner=root/'.local-inputs/root_android_native_test.py'
source=runner.read_text().replace("'-static-libstdc++',","'-static-libstdc++','-ffunction-sections','-fdata-sections','-Wl,--gc-sections',")
exec(compile(source,str(runner),'exec'),{'__name__':'__main__','__file__':str(runner)})
