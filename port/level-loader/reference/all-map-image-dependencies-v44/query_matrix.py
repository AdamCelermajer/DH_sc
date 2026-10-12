from pathlib import Path
import argparse,json,collections,hashlib,zipfile
p=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');matrix_path=p/'reports/all-map-image-dependencies-v44.json'
j=json.loads(matrix_path.read_text());assets={tuple(a[k] for k in ['uri','module_selector']):a for a in j['asset_identity_records']}
def rows(dep):
 if not dep.get('asset_identity_key'):return
 a=assets[tuple(dep['asset_identity_key'])];mats={m['catalog']:m for m in a.get('materials',[])}
 if a.get('instances'):
  for inst in a['instances']:
   for slot in inst['material_slots']:
    m=mats[slot['material_catalog']]
    for sampler in m.get('samplers',[]):yield {'source_uri':dep.get('source_uri'),'authored_name':dep.get('authored_name'),'authored_bres_uri':dep['authored_bres_uri'],'asset':a['uri'],'module_selector':a['module_selector'],'scope':dep['scope'],'instance_index':inst['instance_index'],'node':inst['node'],'material_slot':slot['slot'],'material_catalog':m['catalog'],'material_id':m['id'],**sampler}
 else:
  for m in mats.values():
   for sampler in m.get('samplers',[]):yield {'source_uri':dep.get('source_uri'),'authored_name':dep.get('authored_name'),'authored_bres_uri':dep['authored_bres_uri'],'asset':a['uri'],'module_selector':a['module_selector'],'scope':dep['scope'],'instance_index':None,'material_catalog':m['catalog'],'material_id':m['id'],**sampler}
def verify():
 total=collections.Counter();counts=0
 for level in j['levels']:
  per=collections.Counter()
  for dep in level['dependencies']:
   n=0
   for row in rows(dep):
    n+=1;per[row['status']]+=1;total[row['status']]+=1;counts+=1
    if row['status']=='source_no_image':assert row['image_catalog']==0xffffffff and row['authored_uri']==''
    else:assert row['image_catalog']!=0xffffffff and row['authored_uri']
   assert n==dep['joined_image_row_count'],(level['identity'],dep['dependency_index'],n,dep['joined_image_row_count'])
  assert dict(per)==level['counts']['image_statuses']
 assert counts==j['summary']['image_rows'] and dict(total)==j['summary']['image_statuses']
 assert len(j['levels'])==51 and j['summary']['malformed_bres_count']==0
 report={'validation':'PASS','normalized_matrix_sha256':hashlib.sha256(matrix_path.read_bytes()).hexdigest(),'normalized_matrix_bytes':matrix_path.stat().st_size,'levels':51,'reconstructed_source_instance_material_sampler_rows':counts,'status_counts':dict(total),'source_no_image_and_authored_uri_distinguished':True,'actual_runtime_rendering_claim':False}
 (p/'reports/all-map-image-dependencies-v44-verified.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('--verify',action='store_true');ap.add_argument('--level');ap.add_argument('--problem-only',action='store_true');ap.add_argument('--limit',type=int,default=10);args=ap.parse_args()
 if args.verify:verify()
 if args.level:
  level=next(l for l in j['levels'] if l['identity']==args.level);examples=[]
  for dep in level['dependencies']:
   for row in rows(dep):
    if args.problem_only and row['status'] in ['source_no_image','compatibility_canonical_resolved']:continue
    if len(examples)<args.limit:examples.append(row)
  print(json.dumps({'level':args.level,'scope':level['scope'],'counts':level['counts'],'runtime_failure_inferred':False,'examples':examples},indent=2))
