"""Small synthetic wire/slice/loss/scheduler tests. No device/server calls."""
import unittest,tempfile,json,hashlib
from pathlib import Path
from parse_saved_perfetto_v42 import fields,decode,merge_ranges
from collect_present_trace_v42 import validate_config
OUT=Path(__file__).resolve().parents[1]/'reports/present-attribution-v42/runtime-analysis'
def v(n):
 out=bytearray()
 while n>=128:out.append((n&127)|128);n>>=7
 out.append(n);return bytes(out)
def u(k,n):return v(k*8)+v(n)
def b(k,raw):return v(k*8+2)+v(len(raw))+raw
def event(ts,key,payload,tid=3338):return b(2,u(1,ts)+u(2,tid)+b(key,payload))
def mark(ts,text):return event(ts,3,b(2,text.encode()))
def bundle(events,lost=False):return b(1,u(1,0)+u(3,int(lost))+events)
def packet(payload):return b(1,payload)
class Tests(unittest.TestCase):
 def run_decode(self,data):
  with tempfile.TemporaryDirectory(dir=OUT)as tmp:
   path=Path(tmp)/'fixture.pftrace';path.write_bytes(data);return decode(path,3338,3319)
 def test_varint_and_wire(self):
  self.assertEqual([(k,w,bytes(x)if isinstance(x,memoryview)else x)for k,w,x in fields(memoryview(u(9,123456)+b(2,b'hello')))],[ (9,0,123456),(2,2,b'hello')])
 def test_truncation(self):
  for raw in [b'\x80',b'\x0a\x05abc',b'\x09abc']:
   with self.assertRaises(ValueError):list(fields(memoryview(raw)))
 def test_visible_slice(self):
  r,d=self.run_decode(packet(bundle(mark(1000,'B|3319|eglSwapBuffers')+mark(2000,'E'))));self.assertEqual(r['synchronous_GL_TID_slice_stats']['eglSwapBuffers']['total_ms'],.001)
 def test_loss_drops_crossing_stack_and_keeps_complete_islands(self):
  raw=packet(bundle(mark(1000,'B|3319|known')+mark(2000,'E')+mark(3000,'B|3319|invalid')))+packet(bundle(mark(6000,'E')+mark(7000,'B|3319|known')+mark(8000,'E'),True))
  r,d=self.run_decode(raw);self.assertNotIn('invalid',r['synchronous_GL_TID_slice_stats']);self.assertEqual(r['synchronous_GL_TID_slice_stats']['known']['samples'],2);self.assertEqual(r['excluded_loss_ranges'],[(3000,6000)])
 def test_scheduler_known_running_sleep_runnable(self):
  def switch(ts,prev,nxt,state):return event(ts,4,u(2,prev)+u(6,nxt)+u(4,state))
  data=switch(1000,42,3338,0)+switch(2000,3338,42,1)+event(5000,20,u(2,3338))+switch(6000,42,3338,0)+switch(7000,3338,42,0)
  r,d=self.run_decode(packet(bundle(data)));self.assertEqual(r['scheduler_running']['total_ms'],.002);self.assertEqual(r['scheduler_sleep_before_observed_wake']['total_ms'],.003);self.assertEqual(r['scheduler_runnable_wait']['total_ms'],.001)
 def test_unsupported_compression_clock(self):
  for data in [packet(b(50,b'deflate')),packet(b(1,u(1,0)+u(5,4)+mark(1000,'E')))]:
   with self.assertRaises(ValueError):self.run_decode(data)
 def test_loss_union(self):self.assertEqual(merge_ranges([(3,6),(1,4),(9,10)]),[(1,6),(9,10)])
 def test_calibration_bounds(self):
  p=OUT.parent/'capture-calibrated-v43.textproto';text=p.read_text();self.assertTrue(validate_config(text));self.assertIn('buffer_size_kb: 1024',text);self.assertIn('drain_period_ms: 100',text)
if __name__=='__main__':
 OUT.mkdir(exist_ok=True);p=unittest.main(verbosity=2,exit=False)
 receipt={'status':'PASS'if p.result.wasSuccessful()else'FAIL','tests':p.result.testsRun,'failures':len(p.result.failures),'errors':len(p.result.errors),'synthetic_wire_only':True,'no_device_or_server_calls':True,'parser_sha256':hashlib.sha256((Path(__file__).parent/'parse_saved_perfetto_v42.py').read_bytes()).hexdigest()};(OUT/'offline-parser-test-receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');raise SystemExit(0 if p.result.wasSuccessful() else 1)
