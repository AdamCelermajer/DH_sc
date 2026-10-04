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
        'tools/decode_title_music.py','tools/inspect_front_swf.py',
        'tools/front_media_smoke.py','tools/package_front_handoff.py',
        'tools/front_text_smoke.py','tools/front_loading_smoke.py','tools/audit_loading_bitmap.py',
        'tools/recover_menu_sounds.py','tools/front_sound_smoke.py',
        'tools/front_input_smoke.py',
        'tools/menu_frame_clock_audit.cpp',
        'tools/menu_options_audit.cpp',
        'tools/run_menu_options_audit.py',
        'tools/menu_renderer_scope_audit.cpp',
        'tools/run_menu_renderer_scope_audit.py',
        'tools/menu_manager_push_differential.py',
        'tools/audit_menu_stack.py',
        'app/src/main/cpp/original_menu_sound_data.hpp',
        'app/src/main/cpp/menu_frame_clock.hpp',
    ]
    paths.update(Path('port/android-native')/p for p in new)
    paths.update(Path('port/engine-ui')/p for p in [
        'gameswf_text_property_overlay_v1.cmake',
        'overlays/text-property-v1/gameswf_text.cpp',
        'tools/build_text_property_overlay.py',
        'swf_text_layout_connection.hpp','swf_text_layout_connection.cpp',
        'swf_menu_sound.hpp','swf_menu_sound.cpp',
        'swf_menu_options.hpp','swf_menu_options.cpp',
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
    (handoff/'contribution-review.patch').write_text(''.join(patch),encoding='utf-8',newline='\n')
    (handoff/'contribution-files.json').write_text(json.dumps(dict(status='WORK IN PROGRESS; media verified, menus incomplete',files=records),indent=2)+'\n',encoding='utf-8')
    print('Packaged review diff and',len(records),'file receipts')


if __name__=='__main__':main()
