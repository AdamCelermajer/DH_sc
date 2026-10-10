"""Exercise actual source masks and triangulation UV interpolation."""
import runpy
from pathlib import Path
m=runpy.run_path(str(Path(__file__).with_name('export_art.py')))
# Source display state: active tab button animates its tab-icon alpha to 1 at
# frame32; deactivated stat training adds exact source shape315 while keeping
# the white plus shape311 in the display list.
assert m['sprites'][265][32][1]['color'][0][3]==1.0
assert m['sprites'][265][0][1]['color'][0][3]==0.0
assert any(p['character']==315 for p in m['sprites'][316][10].values())
assert any(p['character']==311 for p in m['sprites'][316][10].values())
assert not any(p['character']==315 for p in m['sprites'][316][0].values())
# Actual source training-button masks previously ignored by first-frame export.
assert 311 in m['hit_shapes'], 'actual original solid mask unavailable'
# Skills sprite496 has 16 original Grey solid-fill overlays. Their alpha is
# the original Placement color transform (102/256), with each overlay ordered
# after that exact bitmap role. No neighboring color/filter branch is admitted.
grey_overlays=[item for item in m['source_solids']
               if item[0].startswith('menu_SkillTreeSheetNew/buttons/skill') and '/Grey/' in item[0]]
assert len(grey_overlays)==16, f'expected 16 original Skills Grey solids, got {len(grey_overlays)}'
for role,ident,verts,rgba,after in grey_overlays:
    assert ident==486 and verts and abs(rgba[3]-102/256)<1e-8
    assert after.startswith(role.split('/Grey/')[0]+'/btimg/'), (role,after)
# Active source stats/inventory/skills frame0 contains no clipDepth masks.
# Exercise an actual separately authored map mask scope without claiming that
# mask support fixes visibility failures in these unrelated character pages.
mask_art=[];mask_text=[]
m['walk'](655,[1,0,0,1,0,0],'actual_map_mask_scope',mask_art,mask_text)
assert m['mask_applications'], 'source mask range never reached'
# Independent clipping area/UV property test: geometry outside mask is removed,
# while original linear UV across triangle remains continuous at new vertices.
original=[[-10,-10,0,0],[20,-10,1,0],[-10,20,0,1]]
mask=[[0,0,0,0],[8,0,0,0],[0,8,0,0]]
out=m['clipped'](original,mask)
assert out and len(out)%3==0
for x,y,u,v in out:
    assert x>=-1e-5 and y>=-1e-5 and x+y<=8+1e-5
    assert abs(u-(x+10)/30)<1e-5 and abs(v-(y+10)/30)<1e-5
# Actual source main background shape90 and placement cover full480x320 stage
# within authored antialias edge fraction, not a fabricated opaque rectangle.
art=[];text=[]
m['walk'](267,[1,0,0,1,-260,-60],'actual_menu',art,text)
background=[verts for _,ident,verts in art if ident==90]
assert len(background)==1
verts=background[0]
assert min(v[0] for v in verts)<=0 and max(v[0] for v in verts)>=480
assert min(v[1] for v in verts)<=0 and max(v[1] for v in verts)>=319.5
print('original menu masked art export tests PASS;',len(m['mask_applications']),'source mask applications')
