"""Bounded exit-notification yield; no live OS/process/JobObject operations."""
from pathlib import Path
import hashlib,importlib.util,json,sys,tempfile,unittest
from unittest.mock import patch
HERE=Path(__file__).resolve();TOOLS=HERE.parents[1];SOURCE=TOOLS/'emulator_watchdog_v36.py'
spec=importlib.util.spec_from_file_location('watchdog_exit_drain_v48',SOURCE)
wd=importlib.util.module_from_spec(spec);sys.modules[spec.name]=wd;spec.loader.exec_module(wd)
OUT=TOOLS.parent/'reports/emulator-watchdog-exit-drain-v48'
def record(pid,created=None):return {'pid':pid,'parent_pid':100,'image':'python.exe','create_time_filetime':created or pid*100,'private_bytes':1024,'handles':3,'alive':True}
class API:
 def __init__(self,clock,clear_after=None,missing_reason=None):
  self.clock=clock;self.clear_after=clear_after;self.queries=0;self.reads={};self.missing_reason=missing_reason or wd.ProcessGone(11)
 def system_snapshot(self):return {'commit_total_bytes':8*wd.GIB,'commit_limit_bytes':64*wd.GIB,'physical_available_bytes':16*wd.GIB}
 def enumerate_processes(self):return {100:record(100,1000),10:record(10)}
 def job_process_ids(self,handle):
  self.queries+=1;return [10] if self.clear_after is not None and self.clock[0]>=self.clear_after else [10,11]
 def process_snapshot(self,pid,metadata=None):
  self.reads[pid]=self.reads.get(pid,0)+1
  if pid==11:raise self.missing_reason
  return record(pid,1000 if pid==100 else None)
class ExitDrainTests(unittest.TestCase):
 def setUp(self):
  OUT.mkdir(parents=True,exist_ok=True);self.tmp=tempfile.TemporaryDirectory(dir=OUT);self.path=Path(self.tmp.name)/'manifest.json'
  self.manifest={'launcher_pid':100,'launcher_create_time':1000,'target_pid':10,'target_create_time':1000};self.path.write_text(json.dumps(self.manifest));self.digest=hashlib.sha256(self.path.read_bytes()).hexdigest()
  self.clock=[0.0];self.sleeps=[]
 def tearDown(self):self.tmp.cleanup()
 def sleep(self,amount):self.sleeps.append(amount);self.clock[0]+=amount
 def collect(self,api):
  with patch.object(wd.time,'monotonic',side_effect=lambda:self.clock[0]),patch.object(wd.time,'sleep',side_effect=self.sleep):
   return wd.collect_snapshot(api,self.manifest,'fake-job',0,self.path,self.digest)
 def test_notification_clears_after_first_bounded_yield(self):
  api=API(self.clock,.07);v=self.collect(api);self.assertTrue(v['complete'],v['errors']);self.assertEqual(api.queries,4)
  self.assertEqual(v['job_process_ids'],[10]);self.assertEqual(self.sleeps,[.075]);self.assertEqual(v['job_exit_drain_wait_seconds'],.075)
  self.assertEqual(v['job_membership_passes'][0]['missing_current_pids'],[11]);self.assertTrue(v['job_membership_passes'][1]['accepted'])
 def test_notification_clears_after_second_yield_without_extra_passes(self):
  api=API(self.clock,.14);v=self.collect(api);self.assertTrue(v['complete']);self.assertEqual(api.queries,5)
  self.assertEqual(len(self.sleeps),2);self.assertLessEqual(sum(self.sleeps),.15);self.assertEqual(len(v['job_membership_passes']),3)
 def test_permanent_listed_ghost_still_fails_closed_no_zero_memory(self):
  api=API(self.clock);v=self.collect(api);self.assertFalse(v['complete']);self.assertEqual(api.queries,5);self.assertLessEqual(sum(self.sleeps),.15)
  self.assertEqual(v['job_process_ids'],[10,11]);self.assertNotIn(11,[p['pid'] for p in v['job_members']]);self.assertIn('monitoring_unavailable',wd.evaluate(v,wd.Limits())['reasons'])
 def test_access_error_is_not_an_exit_yield(self):
  api=API(self.clock,.07,PermissionError('denied'));v=self.collect(api);self.assertFalse(v['complete']);self.assertEqual(self.sleeps,[]);self.assertIn('PermissionError',v['errors'][0])
 def test_new_unsampled_members_never_qualify_for_drain(self):
  api=API(self.clock);api.process_snapshot=lambda pid,metadata=None:record(pid,1000 if pid==100 else None)
  sets=iter([[10],[10],[10,11],[10,12],[10,13]])
  def queries(handle):api.queries+=1;return next(sets)
  api.job_process_ids=queries;v=self.collect(api);self.assertFalse(v['complete']);self.assertEqual(self.sleeps,[]);self.assertEqual(api.queries,5)
 def test_creation_reuse_in_exited_handle_diagnostic_still_fails(self):
  api=API(self.clock,.07,wd.ProcessGone(11,'GetExitCodeProcess_exited',create_time_filetime=9999,exit_code=0,exit_time_filetime=10000))
  original=api.process_snapshot
  def snapshot(pid,metadata=None):
   if pid==11 and api.reads.get(pid,0)==0:api.reads[pid]=1;return record(11,1100)
   return original(pid,metadata)
  api.process_snapshot=snapshot;api.enumerate_processes=lambda:{100:record(100,1000),10:record(10),11:record(11)}
  v=self.collect(api);self.assertFalse(v['complete']);self.assertEqual(self.sleeps,[]);self.assertIn('identity_error',v['job_membership_passes'][0])
 def test_exit_and_invalid_open_are_distinguished_neither_omits_listed_pid(self):
  for error in [wd.ProcessGone(11,'OpenProcess_ERROR_INVALID_PARAMETER'),wd.ProcessGone(11,'GetExitCodeProcess_exited',create_time_filetime=1100,exit_time_filetime=1200,exit_code=0)]:
   with self.subTest(reason=error.reason):
    self.clock[0]=0;self.sleeps.clear();v=self.collect(API(self.clock,None,error));self.assertFalse(v['complete'])
    diagnostic=v['job_membership_passes'][0]['process_gone_diagnostics'][0];self.assertEqual(diagnostic['reason'],error.reason)
    self.assertEqual(v['job_process_ids'],[10,11]);self.assertNotIn('private_bytes',diagnostic)
 def test_slow_first_wait_does_not_extend_wait_budget(self):
  api=API(self.clock)
  def oversleep(amount):self.sleeps.append(amount);self.clock[0]+=.21
  with patch.object(wd.time,'monotonic',side_effect=lambda:self.clock[0]),patch.object(wd.time,'sleep',side_effect=oversleep):
   v=wd.collect_snapshot(api,self.manifest,'fake-job',0,self.path,self.digest)
  self.assertFalse(v['complete']);self.assertEqual(len(self.sleeps),1);self.assertEqual(api.queries,5)
if __name__=='__main__':
 run=unittest.main(verbosity=2,exit=False);r=run.result
 receipt={'status':'PASS' if r.wasSuccessful() else 'FAIL','tests':r.testsRun,'failures':len(r.failures),'errors':len(r.errors),
  'watchdog_sha256':hashlib.sha256(SOURCE.read_bytes()).hexdigest(),'test_sha256':hashlib.sha256(HERE.read_bytes()).hexdigest(),
  'membership_metric_pass_bound':3,'membership_query_bound':5,'exit_drain_planned_wait_bound_seconds':.15,'injected_API_only':True}
 (OUT/'fixture-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');raise SystemExit(0 if r.wasSuccessful() else 1)
