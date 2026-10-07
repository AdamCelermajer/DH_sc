"""Small no-device parser/bounds acceptance tests."""
import unittest,subprocess,sys,json
from collect_present_trace_v42 import validate_config,parse_stat,CONFIG
from analyze_frame_boundary_v42 import analyze
class Acceptance(unittest.TestCase):
 def test_config(self):self.assertTrue(validate_config(CONFIG.read_text()))
 def test_bounds(self):
  text=CONFIG.read_text()
  for field in ['duration_ms: 20000','size_kb: 8192','write_into_file: true','max_file_size_bytes: 8388608']:
   with self.assertRaises(ValueError):validate_config(text.replace(field,''))
 def test_global_controls(self):
  for field in ['exclusive_prio: 1','deferred_start: true','trigger_config {}','output_path: "x"']:
   with self.assertRaises(ValueError):validate_config(CONFIG.read_text()+field)
 def test_stat(self):
  r=parse_stat('123 (GL thread (test)) S '+' '.join(str(i)for i in range(4,53)))
  self.assertEqual((r['pid'],r['comm'],r['minor_faults'],r['major_faults'],r['user_ticks'],r['kernel_ticks'],r['threads'],r['start_ticks']),(123,'GL thread (test)',10,12,14,15,20,22))
 def test_stat_unavailable(self):
  for r in ['', '1 (thread) S','garbage']:self.assertFalse(parse_stat(r)['available'])
 def test_markers(self):
  r=analyze({'schema':'dh2-frame-boundary-v42','tid':12,'frames':2,'rows':[[100,110,120,130,10,20],[200,210,220,240,30,45]]})
  self.assertEqual(r['native_wall']['samples'],2);self.assertEqual(r['intercallback_unattributed_gap']['max_ms'],.00007)
  self.assertIsNone(r['actual_presented_fps']);self.assertIsNone(r['actual_egl_swap_ms'])
 def test_invalid_markers(self):
  for row in [[100,0,0,130,0,0],[100,120,110,130,0,0],[100,110,120,130.0,0,0]]:
   with self.assertRaises(ValueError):analyze({'schema':'dh2-frame-boundary-v42','tid':1,'frames':1,'rows':[row]})
 def test_cpu_unavailable(self):
  r=analyze({'schema':'dh2-frame-boundary-v42','tid':12,'frames':1,'rows':[[100,110,120,130,-1,-1]]})
  self.assertFalse(r['guest_thread_cpu']['available'])
 def test_dry_run(self):
  p=subprocess.run([sys.executable,str(CONFIG.parents[2]/'tools/collect_present_trace_v42.py')],capture_output=True,text=True,timeout=5)
  self.assertEqual(p.returncode,0,p.stderr);self.assertTrue(json.loads(p.stdout)['no_ADB_or_device_calls'])
 def test_wrong_serial(self):
  p=subprocess.run([sys.executable,str(CONFIG.parents[2]/'tools/collect_present_trace_v42.py'),'--serial','physical-device'],capture_output=True,text=True,timeout=5)
  self.assertNotEqual(p.returncode,0)
if __name__=='__main__':unittest.main()
