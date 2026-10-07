"""Fully sampled final membership, departures and fail-closed churn. No OS API."""
from pathlib import Path
import hashlib,importlib.util,json,sys,tempfile,time,unittest
HERE=Path(__file__).resolve();TOOLS=HERE.parents[1];SOURCE=TOOLS/'emulator_watchdog_v36.py'
spec=importlib.util.spec_from_file_location('watchdog_membership_v45',SOURCE)
wd=importlib.util.module_from_spec(spec);sys.modules[spec.name]=wd;spec.loader.exec_module(wd)
OUT=TOOLS.parent/'reports/emulator-watchdog-membership-v45'

def record(pid,created=None,private=1024,image='qemu-system-x86_64.exe'):
 return {'pid':pid,'parent_pid':100,'image':image,'create_time_filetime':created or pid*100,
         'private_bytes':private,'working_set_bytes':128,'handles':4,'alive':True}
class API:
 def __init__(self,queries,sequences=None,census=None):
  self.queries=queries;self.query_count=0;self.sequences=sequences or {};self.reads={}
  self.records={p:record(p) for p in range(10,200)};self.records[100]=record(100,1000,image='python.exe')
  self.census=census or [100,10]
 def system_snapshot(self):return {'commit_total_bytes':8*wd.GIB,'commit_limit_bytes':64*wd.GIB,'physical_available_bytes':16*wd.GIB}
 def enumerate_processes(self):return {p:self.records[p] for p in self.census}
 def job_process_ids(self,handle):
  value=self.queries[min(self.query_count,len(self.queries)-1)];self.query_count+=1;return list(value)
 def process_snapshot(self,pid,metadata=None):
  index=self.reads.get(pid,0);self.reads[pid]=index+1;sequence=self.sequences.get(pid)
  value=sequence[min(index,len(sequence)-1)] if sequence else self.records[pid]
  if isinstance(value,Exception):raise value
  return dict(value)

class MembershipTests(unittest.TestCase):
 def setUp(self):
  OUT.mkdir(parents=True,exist_ok=True);self.temporary=tempfile.TemporaryDirectory(dir=OUT)
  self.path=Path(self.temporary.name)/'manifest.json';self.manifest={'launcher_pid':100,'launcher_create_time':1000,'target_pid':10,'target_create_time':1000}
  self.path.write_text(json.dumps(self.manifest));self.digest=hashlib.sha256(self.path.read_bytes()).hexdigest()
 def tearDown(self):self.temporary.cleanup()
 def collect(self,api):return wd.collect_snapshot(api,self.manifest,'fake-owned-job',time.monotonic(),self.path,self.digest)
 def closed(self,value):
  self.assertFalse(value['complete']);self.assertIn('monitoring_unavailable',wd.evaluate(value,wd.Limits())['reasons']);self.assertIn('owner',value)
 def test_live_sample_departed_before_refresh_is_safe_subset(self):
  api=API([[10],[10,11],[10]]);value=self.collect(api)
  self.assertTrue(value['complete'],value['errors']);self.assertEqual(api.query_count,3)
  self.assertEqual(value['job_process_ids'],[10]);self.assertEqual(value['job_reconciled_departed_pids'],[11]);self.assertEqual(value['job_reconciled_exited_pids'],[])
  self.assertEqual([p['pid'] for p in value['qemu']],[10])
  self.assertEqual(value['job_membership_passes'][0]['departed_pids'],[11]);self.assertTrue(value['job_membership_passes'][0]['accepted'])
  self.assertEqual(wd.evaluate(value,wd.Limits())['action'],'continue')
 def test_member_gone_while_sampled_then_excluded_accepts_first_pass(self):
  api=API([[10],[10,11],[10]],{11:[wd.ProcessGone(11)]});value=self.collect(api)
  self.assertTrue(value['complete']);self.assertEqual(api.query_count,3);self.assertEqual(value['job_membership_passes'][0]['process_gone_pids'],[11])
 def test_empty_final_job_after_all_sampled_members_depart_is_complete(self):
  value=self.collect(API([[10],[10],[]]));self.assertTrue(value['complete']);self.assertEqual(value['job_members'],[])
  self.assertEqual(wd.evaluate(value,wd.Limits())['action'],'complete')
 def test_departing_member_with_arrival_requires_fresh_arrival_metrics(self):
  api=API([[10],[10,11],[10,12],[10,12]]);value=self.collect(api)
  self.assertTrue(value['complete']);self.assertEqual(api.query_count,4);self.assertEqual(api.reads[12],1)
  first,last=value['job_membership_passes'];self.assertFalse(first['accepted']);self.assertEqual(first['missing_current_pids'],[12]);self.assertEqual(first['departed_pids'],[11]);self.assertTrue(last['accepted'])
 def test_old_census_metrics_cannot_admit_new_final_member(self):
  api=API([[10],[10],[10,12],[10,12]],{12:[record(12,1200,1024),record(12,1200,17*wd.GIB)]},[100,10,12]);value=self.collect(api)
  self.assertTrue(value['complete']);self.assertEqual(api.reads[12],2)
  self.assertEqual(value['job_members'][1]['private_bytes'],17*wd.GIB)
  self.assertIn('owned_job_private_limit',wd.evaluate(value,wd.Limits())['reasons']);self.assertIn('aggregate_qemu_private_limit',wd.evaluate(value,wd.Limits())['reasons'])
 def test_every_pass_new_unsampled_pid_remains_bounded_and_closed(self):
  api=API([[10],[10,11],[10,12],[10,13],[10,14]]);value=self.collect(api);self.closed(value)
  self.assertEqual(api.query_count,5);self.assertEqual(len(value['job_membership_passes']),3)
  self.assertEqual(value['job_membership_passes'][-1]['missing_current_pids'],[14]);self.assertNotIn(14,api.reads)
 def test_reused_pid_identity_is_not_relaxed_by_departures(self):
  api=API([[10],[10,11],[10]],{10:[record(10,1000),record(10,1001)]});value=self.collect(api);self.closed(value)
  self.assertEqual(value['job_membership_passes'][0]['identity_error']['pid'],10)
 def test_access_error_on_departing_member_still_closed(self):
  api=API([[10],[10,11],[10]],{11:[PermissionError('fixture denied')]});value=self.collect(api);self.closed(value)
  self.assertEqual(api.query_count,2);self.assertEqual(value['job_membership_passes'][0]['metric_error']['pid'],11)
 def test_dead_or_wrong_identity_record_still_closed(self):
  for bad in [dict(record(10),alive=False),record(12),dict(record(10),create_time_filetime=True),dict(record(10),create_time_filetime=0)]:
   with self.subTest(bad=bad):
    value=self.collect(API([[10]],{10:[record(10),bad]}));self.closed(value)
    self.assertIn('identity_error',value['job_membership_passes'][0])
 def test_processgone_that_remains_final_never_uses_old_metrics(self):
  api=API([[10]],{10:[record(10),wd.ProcessGone(10)]});value=self.collect(api);self.closed(value)
  self.assertEqual(api.query_count,5);self.assertEqual(value['job_membership_passes'][-1]['missing_current_pids'],[10])
 def test_telemetry_pid_lists_bounded_but_every_member_accounted(self):
  members=list(range(10,90));value=self.collect(API([members]));self.assertTrue(value['complete'],value['errors'])
  trace=value['job_membership_passes'][0]
  self.assertEqual(trace['queried_count'],80);self.assertEqual(trace['sampled_count'],80);self.assertEqual(trace['refreshed_count'],80)
  for key in ['queried_pids','sampled_identities','refreshed_pids']:self.assertEqual(len(trace[key]),64)
  self.assertEqual(len(value['job_members']),80);self.assertEqual(wd.evaluate(value,wd.Limits())['observations']['owned_private_bytes'],80*1024)
 def test_departed_qemu_not_double_counted_but_unrelated_qemu_limit_retained(self):
  api=API([[10],[10,11],[10]],{11:[record(11,1100,30*wd.GIB)],20:[record(20,2000,22*wd.GIB)]},[100,10,20]);value=self.collect(api)
  self.assertTrue(value['complete']);self.assertEqual(set(p['pid'] for p in value['qemu']),{10,20})
  reasons=wd.evaluate(value,wd.Limits())['reasons'];self.assertNotIn('owned_job_private_limit',reasons);self.assertIn('aggregate_qemu_private_limit',reasons)

def captured_receipts():
 root=TOOLS.parents[2];result=[]
 for name in ['20261006-170248-9cb4586f','20261006-175037-ad586867']:
  path=root/'.local-inputs/emulator-guards-v36'/f'{name}.watchdog.json'
  receipt=json.loads(path.read_text());value=receipt['last_snapshot'];members={p['pid'] for p in value['job_members']};final=set(value['job_process_ids'])
  result.append({'receipt':str(path),'sha256':hashlib.sha256(path.read_bytes()).hexdigest(),'elapsed_seconds':value['elapsed_seconds'],
    'reason':value['decision']['reasons'],'final_pids':sorted(final),'metric_pids':sorted(members),'missing_final_metrics':sorted(final-members),
    'owner_identity_valid':(value['owner']['pid'],value['owner']['create_time_filetime'])==(value['expected_owner']['pid'],value['expected_owner']['create_time_filetime']),
    'captured_per_pass_membership':False,'historical_cause_exact_sequence_unknown':True})
 (OUT/'captured-receipt-analysis.json').write_text(json.dumps(result,indent=2)+'\n')

if __name__=='__main__':
 runner=unittest.main(verbosity=2,exit=False);captured_receipts()
 result=runner.result;receipt={'status':'PASS' if result.wasSuccessful() else 'FAIL','tests':result.testsRun,'errors':len(result.errors),'failures':len(result.failures),
  'watchdog_sha256':hashlib.sha256(SOURCE.read_bytes()).hexdigest(),'test_sha256':hashlib.sha256(HERE.read_bytes()).hexdigest(),
  'injected_API_only':True,'live_process_device_or_emulator_operations':False,'membership_metric_pass_bound':3,'membership_query_bound':5,'diagnostic_pid_list_bound':64}
 (OUT/'fixture-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');raise SystemExit(0 if result.wasSuccessful() else 1)
