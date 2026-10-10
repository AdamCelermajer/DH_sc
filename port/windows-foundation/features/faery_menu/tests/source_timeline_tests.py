import json
from pathlib import Path

root=Path(__file__).resolve().parents[5]
actions=json.loads((root/'port/engine-ui/reference/character-menu-flow-v1/authored-actions.json').read_text(encoding='utf-8'))
layout=json.loads((Path(__file__).resolve().parents[1]/'source_layout.json').read_text(encoding='utf-8'))
sprite=next(block for block in actions['blocks'] if block['path']=='root/sprite547')

def values(row):
    result=[]
    for item in row.get('values',[]):
        if isinstance(item,dict):result.append(item.get('text',item.get('register',item.get('constant'))))
        else:result.append(item)
    return result

def descendants(rows):
    for row in rows:
        yield row
        yield from descendants(row.get('body',[]))

rows=list(descendants(sprite['rows']))
functions={row['offset']:row for row in rows if row.get('op')=='function2'}
faery_unlocked=functions[146676]
queries=[]
for row in descendants(faery_unlocked['body']):
    row_values=values(row)
    if row['op']=='push_data' and 'NativeHUDGetIsFaeryUnlocked' in row_values:
        queries.append(row_values)
assert [int(q[1]) for q in queries]==[0,1,2,3,4],queries
assert all(q[0]==0.0 and q[2]==2 for q in queries),queries

# Page initialization clears both source placeholder clips. GetActiveFaery is
# then authoritative for the selected slot and upgrades; it focuses the active
# button after unlock visuals have been computed.
on_show=functions[146409]
show_values=[values(row) for row in descendants(on_show['body']) if row['op']=='push_data']
assert any('Blank' in v and 'btimg' in v for v in show_values),show_values
assert any('Blank' in v and 'btimg2' in v for v in show_values),show_values
get_active=functions[147082]
active_values=[values(row) for row in descendants(get_active['body']) if row['op']=='push_data']
assert any('NativeHUDGetActiveFaery' in v for v in active_values),active_values
assert any('FaeryUpgraded' in v for v in active_values),active_values
assert sum(1 for v in active_values if 'Focused' in v)==5,active_values

layout_buttons=layout['button_states']
assert layout_buttons['state_frames']=={'idle':0,'focused':9,'locked_gotoAndStop_17_frame':16}
assert layout_buttons['icon_names']==['Lightning','Nature','Ice','Wind','Fire']
assert len(layout_buttons['variants'])==5
for slot,variant in enumerate(layout_buttons['variants']):
    assert variant['slot']==slot
    assert [state['index'] for state in variant['frames']]==[0,9,16]
    assert variant['frames'][2]['solid_overlays'][0]['shape_id']==535
    assert variant['frames'][2]['solid_overlays'][0]['after_bitmap_role'].endswith('FaeryElementImage/1')
    assert variant['frames'][2]['solid_overlays'][0]['rgba'][3]>0

# Every original onRelease closure sends the slot to SetActiveFaery directly.
# The AS click body has no unlock-query guard: frame17 is the source visual for
# locked rows, but the callback still runs and owns its native source result.
release_offsets=[148268,148501,148730,148959,149188]
for slot,offset in enumerate(release_offsets):
    body=functions[offset]['body'];body_rows=list(descendants(body))
    click_values=[values(row) for row in body_rows if row['op']=='push_data' and 'NativeHUDSetActiveFaery' in values(row)]
    assert any(len(v)>=4 and v[1]==float(slot) and v[2]==2 for v in click_values),(slot,click_values)
    assert not any('NativeHUDGetIsFaeryUnlocked' in values(row) for row in body_rows),slot

# NativePopAllAbove targets the CharacterMenu route root, then source page 547
# is pushed as a sibling overlay; CharacterSheetNew is not retained below it.
route_constants=[]
for row in descendants(next(block for block in actions['blocks'] if block['path']=='root/sprite267')['rows']):
    vals=values(row)
    if row['op']=='push_data' and any(v=='menu_FaerySheet' for v in vals):route_constants.append(vals)
assert route_constants, 'faery page push missing from source route'
assert layout['common_composition']=={'CharacterMenu_background_shape':90,'page_shape':497,'side_trim_shape':546,'CharacterSheetNew_retained':False}
print('PASS Faery source timeline: five unlock-driven frame17 states; all five locked-looking buttons still release to NativeHUDSetActiveFaery; focused/idle variants and CharacterMenu+Faery composition preserved')
