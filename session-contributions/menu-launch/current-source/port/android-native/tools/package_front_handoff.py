"""Write reviewable diffs against preserved baselines without touching the main checkout."""
import argparse
import difflib
import hashlib
import json
from pathlib import Path


def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--snapshot',type=Path,required=True)
    parser.add_argument('--handoff',type=Path,required=True)
    args=parser.parse_args()
    snapshot=args.snapshot.resolve();handoff=args.handoff.resolve()
    baseline=handoff/'baseline'
    paths={p.relative_to(baseline) for p in baseline.rglob('*') if p.is_file()}
    new=[
        'app/src/main/java/com/example/dh2/CinematicActivity.java',
        'app/src/main/java/com/example/dh2/FrontAudio.java',
        'app/src/main/java/com/example/dh2/OriginalCatalogActivity.java',
        'app/src/main/java/com/example/dh2/OriginalCatalogUrl.java',
        'app/src/main/res/xml/original_catalog_network.xml',
        'tools/decode_title_music.py','tools/inspect_front_swf.py',
        'tools/front_media_smoke.py','tools/package_front_handoff.py',
        'tools/front_text_smoke.py','tools/front_loading_smoke.py','tools/audit_loading_bitmap.py',
        'tools/audit_loading_resources.py',
        'tools/recover_menu_sounds.py','tools/front_sound_smoke.py',
        'tools/front_input_smoke.py',
        'tools/front_navigation_smoke.py',
        'tools/front_shared_navigation_smoke.py',
        'tools/front_settings_smoke.py',
        'tools/front_name_smoke.py',
        'tools/repair_keyboard_atlas.py','tools/verify_keyboard_atlas.py','tools/repair_loading_atlas.py',
        'tools/front_keyboard_smoke.py',
        'tools/front_resize_transition_smoke.py',
        'tools/front_pop_above_smoke.py',
        'tools/front_class_smoke.py',
        'tools/front_class_scene_smoke.py',
        'tools/menu_frame_clock_audit.cpp','tools/menu_erase_slot_audit.cpp',
        'tools/menu_options_audit.cpp','tools/menu_save_slot_properties_audit.cpp','tools/menu_save_slot_services_audit.cpp',
        'tools/run_menu_options_audit.py',
        'tools/menu_renderer_scope_audit.cpp',
        'tools/run_menu_renderer_scope_audit.py',
        'tools/menu_manager_push_differential.py',
        'tools/audit_menu_stack.py',
        'app/src/main/cpp/original_menu_sound_data.hpp',
        'app/src/main/cpp/menu_frame_clock.hpp',
    ]
    paths.update(Path('port/android-native')/p for p in new)
    paths.update(Path('port/game-data')/p for p in [
        'class_preview_setup.hpp','class_preview_setup.cpp',
        'tests/class_preview_setup.cpp',
        'fresh_player_profile_v1.hpp','fresh_player_profile_v1.cpp',
        'menu_profile_metadata_v1.hpp','menu_profile_metadata_v1.cpp','tests/menu_profile_metadata_v1.cpp',
        'campaign_profile_files_v1.hpp','campaign_profile_files_v1.cpp','tests/campaign_profile_files_v1.cpp',
        'tests/fresh_player_profile_v1.cpp',
        'player_savegame_v1.hpp','player_savegame_v1.cpp','tests/player_location_v1.cpp','tests/player_entry_points_v1.cpp',
        'quest_savegame_v1.hpp','quest_savegame_v1.cpp','tests/quest_savegame_v1.cpp',
    ])
    paths.add(Path('port/level-world/tests/modular_resource.cpp'))
    for folder in ['fresh-player-profile-v1','player-location-v1','player-entry-points-v1','quest-savegame-v1','menu-profile-metadata-v1']:
        for current in sorted((snapshot/'port/game-data/reference'/folder).glob('*.json')):
            paths.add(current.relative_to(snapshot))
    paths.update(Path('port/engine-ui')/p for p in [
        'gameswf_text_property_overlay_v1.cmake',
        'overlays/text-property-v1/gameswf_text.cpp',
        'tools/build_text_property_overlay.py',
        'swf_text_layout_connection.hpp','swf_text_layout_connection.cpp',
        'swf_menu_sound.hpp','swf_menu_sound.cpp',
        'swf_menu_options.hpp','swf_menu_options.cpp',
        'swf_menu_save_slots.hpp','swf_menu_save_slots.cpp',
        'swf_menu_device_v1.hpp','swf_menu_device_v1.cpp','loading_menu_v1.hpp','loading_menu_v1.cpp','swf_loading_menu_v1.hpp','swf_loading_menu_v1.cpp',
        'menu_avatar_preview_v1.hpp','menu_avatar_preview_v1.cpp','swf_menu_avatar_preview_v1.cpp','tests/menu_avatar_preview_v1.cpp',
        'localization_parse_ex_v1.hpp','localization_parse_ex_v1.cpp','tests/localization_parse_ex_v1.cpp',
        'swf_menu_parsed_string_v1.hpp','swf_menu_parsed_string_v1.cpp',
        'save_slot_date_v1.hpp','save_slot_date_v1.cpp','tests/save_slot_date_v1.cpp',
        'menu_save_slot_projection_v1.hpp','menu_save_slot_projection_v1.cpp','tests/menu_save_slot_projection_v1.cpp',
        'swf_menu_navigation.hpp','swf_menu_navigation.cpp',
        'menu_manager_push_v1.hpp','menu_manager_push_v1.cpp',
        'menu_native_event_v1.hpp','menu_native_event_v1.cpp',
    ])
    patch=[];records=[]
    for relative in sorted(paths):
        current=snapshot/relative;old=baseline/relative
        before=old.read_text(encoding='utf-8').splitlines(keepends=True) if old.exists() else []
        after=current.read_text(encoding='utf-8').splitlines(keepends=True)
        if before==after:continue
        name=relative.as_posix()
        patch.extend(difflib.unified_diff(before,after,fromfile='a/'+name if old.exists() else '/dev/null',tofile='b/'+name))
        records.append(dict(path=name,kind='modified' if old.exists() else 'new',sha256=hashlib.sha256(current.read_bytes()).hexdigest()))
    for current in sorted((snapshot/'port/android-native/app/src/main/assets/original-media').glob('*')):
        records.append(dict(path=current.relative_to(snapshot).as_posix(),kind='media_asset',bytes=current.stat().st_size,sha256=hashlib.sha256(current.read_bytes()).hexdigest()))
    for suffix in ['pyarray','pyarraynames','pystructnames']:
        current=snapshot/'port/android-native/app/src/main/assets/data'/('help_pages_'+suffix+'.bin')
        records.append(dict(path=current.relative_to(snapshot).as_posix(),kind='original_loading_hint_asset',bytes=current.stat().st_size,sha256=hashlib.sha256(current.read_bytes()).hexdigest(),source='Original help_pages PyData cache'))
    for name in ['design_pyarray.bin','design_pyarraynames.bin','design_pystructnames.bin']:
        current=snapshot/'port/android-native/app/src/main/assets/original-cache/data/pydata'/name
        records.append(dict(path=current.relative_to(snapshot).as_posix(),kind='original_cache_asset',bytes=current.stat().st_size,sha256=hashlib.sha256(current.read_bytes()).hexdigest()))
    current=snapshot/'port/android-native/app/src/main/assets/models/class_selection.bdae'
    if current.exists():
        records.append(dict(path=current.relative_to(snapshot).as_posix(),kind='original_scene_asset',bytes=current.stat().st_size,sha256=hashlib.sha256(current.read_bytes()).hexdigest(),source='Dungeon-Hunter-2-HD-v1-0-2-cache.zip: com.gameloft.android.GAND.GloftD2SS/files/data/3D/OptimizedMaxFiles/CLASS_SELECTION.bdae'))
    for suffix in ['_pyarray.bin','_pyarraynames.bin','_pystructnames.bin']:
        current=snapshot/'port/android-native/app/src/main/assets/data'/('loot_table'+suffix)
        records.append(dict(path=current.relative_to(snapshot).as_posix(),kind='original_cache_asset',bytes=current.stat().st_size,sha256=hashlib.sha256(current.read_bytes()).hexdigest(),source='Dungeon-Hunter-2-HD-v1-0-2-cache.zip: com.gameloft.android.GAND.GloftD2SS/files/data/pydata/'+current.name))
    class_assets=['animations/prince_menu_idle_'+c+s+'.bdae' for c in ['knight','rogue','mage'] for s in ['','_02']]
    class_assets+=['animations/prince_template_anim.bdae','textures/atlas_modular_rogue.tga','textures/atlas_modular_mage.tga']
    for name in class_assets:
        current=snapshot/'port/android-native/app/src/main/assets'/name
        records.append(dict(path=current.relative_to(snapshot).as_posix(),kind='original_class_preview_asset',bytes=current.stat().st_size,sha256=hashlib.sha256(current.read_bytes()).hexdigest()))
    for name in ['models/mc_rweapon_longsword_01.bdae','models/mc_rweapon_dagger_01.bdae','models/mc_rweapon_quarterstaff_01.bdae','textures/atlas_weapons_dh2.tga','textures/envmap_swamp.tga']:
        current=snapshot/'port/android-native/app/src/main/assets'/name
        records.append(dict(path=current.relative_to(snapshot).as_posix(),kind='original_weapon_preview_asset',bytes=current.stat().st_size,sha256=hashlib.sha256(current.read_bytes()).hexdigest(),source='Dungeon-Hunter-2-HD-v1-0-2-cache.zip original weapon scene/texture'))
    for current in sorted((snapshot/'port/android-native/app/src/main/assets/front-compat').glob('*')):
        records.append(dict(path=current.relative_to(snapshot).as_posix(),kind='keyboard_compatibility_asset',bytes=current.stat().st_size,sha256=hashlib.sha256(current.read_bytes()).hexdigest(),source='Generated from original SWF/atlas; original Android mapping unresolved'))
    for folder in ['fresh-player-profile-v1','player-location-v1','player-entry-points-v1','quest-savegame-v1','menu-profile-metadata-v1']:
        for current in sorted((snapshot/'port/game-data/reference'/folder).glob('*.bin')):
            records.append(dict(path=current.relative_to(snapshot).as_posix(),kind='original_profile_fixture',bytes=current.stat().st_size,sha256=hashlib.sha256(current.read_bytes()).hexdigest(),source='Original ARM creation/date/metadata/file-buffer or location-reader instructions; filesystem/job processing remains unverified'))
    (handoff/'contribution-review.patch').write_text(''.join(patch),encoding='utf-8',newline='\n')
    (handoff/'contribution-files.json').write_text(json.dumps(dict(status='WORK IN PROGRESS; media verified, menus incomplete',files=records),indent=2)+'\n',encoding='utf-8')
    print('Packaged review diff and',len(records),'file receipts')


if __name__=='__main__':main()
