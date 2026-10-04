"""Freeze the completed original display/native font-owner batch after main DSO proof."""
import hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ui=ROOT/'port/engine-ui';ref=ui/'reference/text-display-v2';output=ref/'freeze-manifest.json'
 assert not output.exists(),'Preserve immutable source receipt'
 host_path=ROOT/'port/level-world/reports/native-text-render-fresh-player-main-linked-host-audit-v4.json'
 host=json.loads(host_path.read_text());assert host['validation']=='PASS' and host['sanitizer_findings']==0 and len(host['host_audits'])==125
 assert host['host_audits']['Text_display_v2']=={'validation':'PASS','whole_original_cases':768,'ownership_and_required_guards':6}
 owner=host['host_audits']['Text_render_owner_v2'];assert owner['validation']=='PASS' and owner['actual_font_metrics']==4 and owner['actual_raster_owner_cases']==42 and owner['whole_layout_display_compositions']==4 and owner['ownership_and_source_guards']==9
 production=[ui/(name+suffix) for name in ('text_display_v2','text_render_owner_v2','hud_freetype_font_v2') for suffix in ('.hpp','.cpp')]
 for path in production:assert host['source_sha256'][path.relative_to(ROOT).as_posix()]==sha(path)
 evidence=[ref/name for name in ('capture.py','original-functions.json','reference/original-functions.asm','whole-gold-v3.bin','NOTES.md')]
 evidence += [ui/'tests'/name for name in ('text_display_v2.cpp','text_display_v2_original.py','text_render_owner_v2.cpp')]
 evidence += [Path(__file__)]
 source=json.loads((ref/'original-functions.json').read_text());assert sha(ROOT/'.local-inputs/libDungeonHunter2.so')==source['original_sha256']
 assert sha(ref/'whole-gold-v3.bin')=='df792d2bb9940c463561b87c4245b7436778830746399c68af07a6e44ee189ce'
 # Only the versioned class name/include and added legitimate metric getter
 # differ from the frozen matching-version raster producer implementation.
 cpp=(ui/'hud_freetype_font_v2.cpp').read_text().replace('HudFreetypeFontV2','HudFreetypeFont').replace('"hud_freetype_font_v2.hpp"','"hud_freetype_font.hpp"')
 begin=cpp.index('bool HudFreetypeFont::metrics(');end=cpp.index('bool HudFreetypeFont::raster(',begin)
 assert cpp[:begin]+cpp[end:]==(ui/'hud_freetype_font.cpp').read_text()
 dependencies=[ui/name for name in ('hud_freetype_font.hpp','hud_freetype_font.cpp','freetype_glyph_kernel.hpp','freetype_glyph_kernel.cpp','freetype_bitmap_alpha.hpp','freetype_bitmap_alpha.cpp','swf_movie.hpp','text_layout_v1.hpp','text_layout_v1.cpp')]
 inputs=[ROOT/'.local-inputs/font-text-discovery'/name for name in ('Fontin SmallCaps.ttf','wqy-zenhei.ttf')]
 report=dict(validation='PASS',version=2,original_sha256=source['original_sha256'],
  source_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in production},
  source_and_evidence_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in production+evidence},
  dependency_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in dependencies},
  actual_font_input_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in inputs},
  proof_sha256={host_path.relative_to(ROOT).as_posix():sha(host_path)},
  results=dict(main_DSO_suites=125,original_whole_display_cases=768,display_guards=6,font_owner=owner,sanitizer_findings=0),
  limits=dict(whole_glyph_display_native=True,real_matching_FT_face_and_draw_command_ownership=True,
   caller_bitmap_font_and_atlas_producers_required=True,retained_edit_text_class_migrated=False,
   private_font_copy_metadata_outside_layout_projection=True,draw_upload_sinks_in_host_are_fixtures=True,
   Android_GPU_execution=False,APK_promoted=False,physical_device_verified=False))
 output.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(dict(validation='PASS',manifest=output.relative_to(ROOT).as_posix(),sha256=sha(output),production_files=len(production))))
if __name__=='__main__':main()
