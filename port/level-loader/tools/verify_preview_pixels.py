"""Check saved fixed-level screenshots contain map pixels outside native UI."""
import hashlib,json,pathlib
from PIL import Image
ROOT=pathlib.Path(__file__).resolve().parents[3];REPORTS=ROOT/'port/level-loader/reports'
coverage_path=REPORTS/'fixed-map-preview-coverage.json'
coverage=json.loads(coverage_path.read_text())
swamp=json.loads((REPORTS/'swamp-map-preview.json').read_text())
rect=next(row['compared_rect'] for row in swamp['steps'] if row['case']=='missing_definition')
cases=[]
for row in coverage['levels']:
    if row['preview']!='frame_submitted':continue
    path=REPORTS/row['screenshot'];digest=hashlib.sha256(path.read_bytes()).hexdigest()
    assert digest==row['screenshot_sha256'],'Changed screenshot '+row['name']
    assert any(text.startswith(row['name']+' |') for text in row['visible_text']),'Wrong visible identity '+row['name']
    image=Image.open(path).convert('RGB');assert image.width==rect[2] and image.height>rect[3]
    cropped=image.crop(rect);colors=cropped.getcolors(cropped.width*cropped.height)
    assert colors and len(colors)>100,'Insufficient map color variation '+row['name']
    background=max(colors,key=lambda x:x[0]);changed=cropped.width*cropped.height-background[0]
    assert changed>1000,'Insufficient non-background map pixels '+row['name']
    cases.append({'level':row['name'],'screenshot_sha256':digest,'unique_map_colors':len(colors),'non_background_pixels':changed})
report={'validation':'PASS','scope':'Nonblank map viewport and visible identity checks over retained screenshots. This is not original image parity or gameplay verification.',
        'coverage_sha256':hashlib.sha256(coverage_path.read_bytes()).hexdigest(),
        'source_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        'compared_rect':rect,'cases':cases,'full_loader_verified':False}
(REPORTS/'fixed-map-preview-pixels.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','nonblank_previews':len(cases),'full_loader_verified':False}))
