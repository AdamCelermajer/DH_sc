"""Compact saved receipts; false rendering/full-loader flags remain visible."""
import json,pathlib
reports=pathlib.Path(__file__).resolve().parents[1]/'reports'
for name in ('xml-original-comparison.json','resource-path-differential.json','swamp-fixed-sources-host.json','fixed-sources-level-coverage.json','swamp-fixed-map-host.json','fixed-map-level-coverage.json','visual-transform-differential.json','user-properties-differential.json'):
    row=json.loads((reports/name).read_text())
    fields={k:row[k] for k in ('validation','scope','summary','documents','module_links','map_render_verified','gameplay_verified','full_loader_verified') if k in row}
    if name=='xml-original-comparison.json':
        cases=row['cases'];fields.update(cases=len(cases),tree_equal=sum(r.get('tree_equal',False) for r in cases),
            error_equal=sum(r.get('error_equal',False) for r in cases),
            original_execution_failures=sum('original_execution_failure' in r for r in cases),
            all_cached_xml_compared=row['all_cached_xml_compared'])
    if name=='swamp-fixed-map-host.json':
        native=row['native']
        fields.update({k:native[k] for k in ('asset_count','module_count','geometry_kinds','navigation','ownership_checks','map_render_verified','gameplay_verified')})
    if name in ('visual-transform-differential.json','user-properties-differential.json'):
        fields.update({k:row[k] for k in ('case_count','cases_checked','max_quaternion_error') if k in row})
    print(json.dumps({'receipt':name,**fields}))
