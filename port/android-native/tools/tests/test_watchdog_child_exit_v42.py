"""Injected JobObject races only: no WindowsAPI/device/process operations."""
from pathlib import Path
import hashlib,importlib.util,json,sys,tempfile,time,unittest
SOURCE=Path(__file__).resolve().parents[1]/'emulator_watchdog_v36.py'
spec=importlib.util.spec_from_file_location('watchdog_child_exit_fixture',SOURCE)
wd=importlib.util.module_from_spec(spec);sys.modules[spec.name]=wd;spec.loader.exec_module(wd)
OUT=SOURCE.parents[1]/'reports/emulator-watchdog-child-exit-v42'
def record(pid,created=None,private=1024,image='qemu-system-x86_64.exe'):
 return {'pid':pid,'parent_pid':100,'image':image,'create_time_filetime':created or pid*100,'private_bytes':private,'handles':4,'alive':True}
class API:
 def __init__(self,queries,sequences=None,census=None):
  self.queries=queries;self.query_count=0;self.sequences=sequences or {};self.reads={}
  self.records={100:record(100,1000,image='python.exe'),10:record(10,2000),11:record(11,3000),12:record(12,4000),20:record(20,5000)}
  self.census=census or [100,10]
 def system_snapshot(self):return {'commit_total_bytes':8*wd.GIB,'commit_limit_bytes':64*wd.GIB,'physical_available_bytes':16*wd.GIB}
 def enumerate_processes(self):return {p:self.records[p]for p in self.census}
 def job_process_ids(self,handle):
  v=self.queries[min(self.query_count,len(self.queries)-1)];self.query_count+=1;return list(v)
 def process_snapshot(self,pid,metadata=None):
  n=self.reads.get(pid,0);self.reads[pid]=n+1;seq=self.sequences.get(pid)
  v=seq[min(n,len(seq)-1)]if seq else self.records[pid]
  if isinstance(v,Exception):raise v
  return dict(v)
class RaceTests(unittest.TestCase):
 def setUp(self):
  OUT.mkdir(parents=True,exist_ok=True);self.temp=tempfile.TemporaryDirectory(dir=OUT);self.path=Path(self.temp.name)/'manifest.json'
  self.manifest={'launcher_pid':100,'launcher_create_time':1000,'target_pid':10,'target_create_time':2000}
  self.path.write_text(json.dumps(self.manifest));self.digest=hashlib.sha256(self.path.read_bytes()).hexdigest()
 def tearDown(self):self.temp.cleanup()
 def collect(self,api):return wd.collect_snapshot(api,self.manifest,'fake-owned-job',time.monotonic(),self.path,self.digest)
 def require_closed(self,v):
  d=wd.evaluate(v,wd.Limits());self.assertEqual(d['action'],'terminate');self.assertIn('monitoring_unavailable',d['reasons']);self.assertNotIn('launcher_missing_or_exited',d['reasons']);self.assertEqual(v['owner']['pid'],100)
 def test_exact_new_final_member_exits_before_metric_read(self):
  api=API([[10],[10,11],[10],[10]],{11:[wd.ProcessGone(11)]});v=self.collect(api)
  self.assertTrue(v['complete'],v['errors']);self.assertEqual(v['job_process_ids'],[10]);self.assertEqual(v['job_reconciled_exited_pids'],[11]);self.assertEqual(v['job_membership_requeries'],2);self.assertEqual(wd.evaluate(v,wd.Limits())['action'],'continue')
 def test_initial_child_gone_then_fresh_membership_excludes(self):
  api=API([[10,11],[10,11],[10],[10]],{11:[wd.ProcessGone(11)]},[100,10,11]);v=self.collect(api)
  self.assertTrue(v['complete'],v['errors']);self.assertEqual(v['job_reconciled_exited_pids'],[11]);self.assertNotIn(11,[p['pid']for p in v['qemu']])
 def test_gone_pid_still_listed_is_not_omitted(self):
  api=API([[10],[10,11]],{11:[wd.ProcessGone(11)]});v=self.collect(api);self.require_closed(v);self.assertEqual(api.query_count,5);self.assertEqual(v['job_process_ids'],[10,11])
 def test_gone_pid_returning_live_fails_closed(self):
  api=API([[10],[10,11]],{11:[wd.ProcessGone(11),record(11,9999)]});self.require_closed(self.collect(api));self.assertEqual(api.reads[11],2)
 def test_creation_identity_changes_fail_closed(self):
  api=API([[10]],{10:[record(10,2000),record(10,9999)]});self.require_closed(self.collect(api))
 def test_access_error_not_tolerated_even_if_next_membership_excludes(self):
  api=API([[10],[10,11],[10]],{11:[PermissionError('fixture access denied')]});v=self.collect(api);self.require_closed(v);self.assertEqual(api.query_count,2);self.assertIn('PermissionError',v['errors'][0])
 def test_live_metric_error_not_tolerated(self):
  api=API([[10]],{10:[record(10,2000),RuntimeError('live metric unavailable')]});self.require_closed(self.collect(api))
 def test_new_live_member_is_measured_before_acceptance(self):
  api=API([[10],[10],[10,12],[10,12]]);v=self.collect(api)
  self.assertTrue(v['complete'],v['errors']);self.assertEqual(v['job_process_ids'],[10,12]);self.assertEqual([p['pid']for p in v['job_members']],[10,12]);self.assertEqual(api.reads[12],1)
  self.assertIn(12,[p['pid']for p in v['qemu']])
 def test_new_qemu_joined_after_census_keeps_aggregate_limit(self):
  api=API([[10],[10],[10,12],[10,12]],{12:[record(12,4000,13*wd.GIB)]});v=self.collect(api)
  self.assertTrue(v['complete']);self.assertIn('aggregate_qemu_private_limit',wd.evaluate(v,wd.Limits(soft_job_bytes=20*wd.GIB))['reasons'])
 def test_unstable_membership_is_bounded(self):
  api=API([[10],[10],[10,11],[10],[10,12]]);v=self.collect(api);self.require_closed(v);self.assertEqual(api.query_count,5)
 def test_job_limit_still_stops_after_reconciliation(self):
  api=API([[10],[10,11],[10],[10]],{10:[record(10,2000,58*wd.GIB)],11:[wd.ProcessGone(11)]});v=self.collect(api)
  self.assertTrue(v['complete']);self.assertIn('owned_job_private_limit',wd.evaluate(v,wd.Limits())['reasons'])
 def test_unrelated_qemu_aggregate_still_stops(self):
  api=API([[10]],{20:[record(20,5000,22*wd.GIB)]},[100,10,20]);v=self.collect(api)
  self.assertTrue(v['complete']);self.assertIn('aggregate_qemu_private_limit',wd.evaluate(v,wd.Limits())['reasons'])
 def test_manifest_change_still_stops(self):
  self.path.write_text('{}');v=self.collect(API([[10]]));self.assertIn('manifest_changed_or_unavailable',wd.evaluate(v,wd.Limits())['reasons'])
if __name__=='__main__':
 p=unittest.main(verbosity=2,exit=False)
 receipt={'status':'PASS'if p.result.wasSuccessful()else'FAIL','tests':p.result.testsRun,'errors':len(p.result.errors),'failures':len(p.result.failures),'watchdog_sha256':hashlib.sha256(SOURCE.read_bytes()).hexdigest(),'test_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'injected_API_only':True,'live_process_device_or_emulator_operations':False,'membership_metric_pass_bound':3,'membership_query_bound':5}
 (OUT/'race-test-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');raise SystemExit(0 if p.result.wasSuccessful() else 1)
