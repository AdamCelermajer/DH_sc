"""Exercise the bundled authored Crypt, actual touch control and lifecycle.

The new movement policy is checked against exported original floor geometry.
This is not original PF implementation or GPU equivalence verification.
"""
import argparse,hashlib,json,math,re,subprocess,time
from pathlib import Path
import xml.etree.ElementTree as ET
from PIL import Image,ImageChops
from emulator_smoke import inspect,launch_fresh

POSITION=r'Player position ([\d.-]+) ([\d.-]+) ([\d.-]+) \| moved (\d+) \| blocked (\d+)'
READY=r'World ready .*?position ([\d.-]+) ([\d.-]+) ([\d.-]+)'
def main():
 p=argparse.ArgumentParser();p.add_argument('--adb',required=True);p.add_argument('--serial',required=True)
 p.add_argument('--apk',type=Path,required=True);p.add_argument('--floor',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--native-floors',action='store_true');p.add_argument('--sewn-floors',action='store_true');p.add_argument('--graph-search',action='store_true');p.add_argument('--world-route',action='store_true')
 p.add_argument('--findpath',action='store_true');p.add_argument('--floor-motion',action='store_true');p.add_argument('--native-objects',action='store_true');p.add_argument('--avoidance',action='store_true');p.add_argument('--actor-producers',action='store_true')
 p.add_argument('--heading-control',action='store_true');a=p.parse_args()
 if a.heading_control:a.actor_producers=True
 if a.actor_producers:a.avoidance=True
 if a.avoidance:a.native_objects=True
 if a.native_objects:a.floor_motion=True
 if a.floor_motion:a.findpath=True
 if a.findpath:a.world_route=True
 if a.world_route:a.graph_search=True
 if a.graph_search:a.sewn_floors=True
 if a.sewn_floors:a.native_floors=True
 graph_nodes,graph_edges=(335,838) if a.sewn_floors else (321,778)
 assert a.serial.startswith('emulator-');a.output.mkdir(parents=True,exist_ok=True)
 geometry=json.loads(a.floor.read_text(encoding='utf-8-sig'));libraries=inspect(a.apk);rows=[];movement=[]
 def adb(*args,missing=False):
  r=subprocess.run([a.adb,'-s',a.serial,*args],capture_output=True,text=True,timeout=45)
  if missing and r.returncode==1 and not (r.stdout+r.stderr).strip():return ''
  if r.returncode:raise RuntimeError(r.stdout+r.stderr)
  return r.stdout.strip()
 def logs():
  pid=adb('shell','pidof','com.example.dh2',missing=True);assert pid,'App process exited'
  text=adb('logcat','-d','--pid='+pid,'-v','brief')
  assert not re.search(r'FATAL EXCEPTION|GL error|Shader failed|Link failed|World load failed|Model load failed|Animation sample failed|Skin sample failed|Object sample failed',text),text
  return text
 def wait(predicate):
  deadline=time.monotonic()+30
  while True:
   text=logs()
   if predicate(text):return text
   assert time.monotonic()<deadline,text;time.sleep(.1)
 def launch(cursor=None):
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0')
  args=['--es','world','crypt01.dwld']
  if cursor is not None:args+=['--ei','time_ms',str(cursor)]
  launch_fresh(adb,*args)
  return wait(lambda text:'World ready | rooms 8 | visual draws 101 | navigation triangles 314 | idle tracks 23 | walk tracks 27' in text and 'Model frame submitted at' in text and (not a.native_floors or f'Native floors ready | records 8 | graph nodes {graph_nodes} | graph edges {graph_edges} | selector collision controls height' in text) and (not a.sewn_floors or 'Native floor links ready | neighbour relations 14 | validation references 998' in text) and (not a.graph_search or 'Native graph route probe | floor pairs 64 | successful 64 | segments 1598 | state 5454073390c7d916 | endpoints and movement pending' in text) and (cursor is None or f'Animation frame rendered at {cursor} ms' in text))
 def route_marker(text):
  if a.world_route:assert 'Native world route probe | floor pairs 64 | successful 64 | direct 0 | graph 64 | segments 2086 | state 3a3ab2a724cc383b | smoothing and movement pending' in text,text
  if a.findpath:assert 'Native FindPath probe | floor pairs 64 | successful 64 | owned 64 | segments 2086 | state 9356b419cae2bc57 | position controller pending' in text,text
  if a.floor_motion:assert 'Native floor motion probe | floor pairs 64 | position valid 64 | accepted 64 | direction valid 64 | state d60ad48039cc85c6 | controller and dynamic obstacles pending' in text,text
  if a.native_objects:assert 'Native obstacle registry probe | floor pairs 64 | accepted 64 | registered 64 | relocated 56 | state 22a98dfc4102db51 | forces and controller pending' in text,text
  if a.avoidance:assert 'Native avoidance probe | floors 8 | force contributions 16 | adjusted 8 | turn limited 8 | state 09d1d79d2612c945 | actor producers and controller pending' in text,text
  if a.actor_producers:assert 'Native actor producer probe | floors 8 | registered 8 | physical radius updates 8 | state 30c1f3ac1bf81145 | physical construction and controller pending' in text,text
  if a.heading_control:assert 'Native heading control | player movement and melee facing use recovered source | full UpdatePath and physics pending' in text,text
 def hierarchy():
  # A native draw log can precede Android's status-panel relayout and buffer
  # presentation. Require the loaded UI and a draw at its final viewport size;
  # do not retry/relax missing geometry, shader failures or exited processes.
  deadline=time.monotonic()+30
  while True:
   adb('shell','uiautomator','dump','/sdcard/dh2-world-window.xml')
   root=ET.fromstring(adb('shell','cat','/sdcard/dh2-world-window.xml'))
   views={n.get('content-desc'):tuple(map(int,re.findall(r'\d+',n.get('bounds')))) for n in root.iter('node') if n.get('content-desc')}
   ready=any('Crypt | 8 rooms | 11 monsters | 84 scenery objects' in n.get('text','') for n in root.iter('node'))
   bounds=views.get('DH2 native texture viewport');frames=re.findall(r'Model frame submitted at (\d+) x (\d+)',logs())
   if ready and bounds and frames and tuple(map(int,frames[-1]))==(bounds[2]-bounds[0],bounds[3]-bounds[1]):return views
   assert time.monotonic()<deadline,('World UI/frame did not settle',ready,bounds,frames[-1:] );time.sleep(.1)
 def capture(stem):
  views=hierarchy();bounds=views['DH2 native texture viewport'];pad=views['Movement control']
  assert bounds[0]<=pad[0]<pad[2]<=bounds[2] and bounds[1]<=pad[1]<pad[3]<=bounds[3],(bounds,pad)
  path=a.output/(stem+'.png');adb('shell','screencap','-p','/sdcard/dh2-world-test.png');adb('pull','/sdcard/dh2-world-test.png',str(path))
  picture=Image.open(path).convert('RGB').crop(bounds)
  mask=ImageChops.difference(picture,Image.new('RGB',picture.size,(20,23,28))).convert('L').point(lambda v:255 if v>8 else 0)
  pixels=mask.histogram()[255];assert pixels>picture.width*picture.height*.2,'Missing level geometry'
  rows.append({'screenshot':path.name,'viewport':picture.size,'movement_control_bounds':pad,'rendered_pixels':pixels})
  return picture
 def control(dx=0,dy=0):
  left,top,right,bottom=hierarchy()['Movement control'];return round((left+right)/2+dx*(right-left)*.4),round((top+bottom)/2+dy*(bottom-top)*.4)
 def position(text):
  values=re.findall(POSITION,text);assert values,text;last=values[-1]
  return [float(v) for v in last[:3]],int(last[3]),int(last[4])
 def ground(point):
  heights=[]
  for triangle in geometry['floor']:
   aa,b,c=triangle['corners'];bx,by=b[0]-aa[0],b[1]-aa[1];cx,cy=c[0]-aa[0],c[1]-aa[1]
   dx,dy=point[0]-aa[0],point[1]-aa[1];den=bx*cy-by*cx
   if abs(den)<1e-4:continue
   u,v=(dx*cy-dy*cx)/den,(bx*dy-by*dx)/den
   if u>=-1e-6 and v>=-1e-6 and u+v<=1.000001:heights.append(aa[2]+u*(b[2]-aa[2])+v*(c[2]-aa[2]))
  assert heights,point;h=min(heights,key=lambda v:abs(v-point[2]));assert abs(h-point[2])<.05,(point,h)
  return h
 def facing(text,expected):
  if not a.heading_control:return {}
  values=re.findall(r'Player facing \| angle ([\d.eE+-]+) \| native updates (\d+)',text);assert values,text
  angle,count=float(values[-1][0]),int(values[-1][1]);assert count>0 and abs(angle-expected)<1e-6,(angle,expected,count)
  return {'heading_angle':angle,'native_heading_updates':count}
 def release(x,y):
  adb('shell','input','touchscreen','motionevent','UP',str(x),str(y))
  return wait(lambda text:bool(re.findall(POSITION,text)) and text.rfind('Locomotion clip: idle')>text.rfind('Locomotion clip: walk'))
 prior=adb('shell','cmd','window','user-rotation').split()
 try:
  assert 'Success' in adb('install','-r',str(a.apk))
  package_paths=adb('shell','pm','path','com.example.dh2').splitlines();assert len(package_paths)==1 and package_paths[0].startswith('package:'),package_paths
  installed_sha=adb('shell','sha256sum',package_paths[0].removeprefix('package:')).split()[0];assert installed_sha==hashlib.sha256(a.apk.read_bytes()).hexdigest(),'Installed APK differs from tested artifact'
  adb('shell','cmd','window','user-rotation','lock','0')
  launch(100);frozen=capture('idle-frozen-100');repeat=capture('idle-frozen-repeat')
  assert frozen.size==repeat.size and ImageChops.difference(frozen,repeat).getbbox() is None,'Frozen world changed'
  launch(700);other=capture('idle-frozen-700');assert ImageChops.difference(frozen,other).getbbox(),'Idle clip did not change pixels'
  text=launch();start=[float(v) for v in re.findall(READY,text)[-1]];ground(start)
  route_marker(text)
  live=capture('idle-live');live2=capture('idle-live-repeat');assert ImageChops.difference(live,live2).getbbox(),'Live world animation froze'
  x,y=control(0,1);adb('shell','input','touchscreen','motionevent','DOWN',str(x),str(y));time.sleep(1.8)
  walking=capture('walk-stairs');text=release(x,y);end,steps,blocked=position(text);ground(end)
  assert end[1]<start[1]-500 and end[2]<start[2]-100 and steps>0,(start,end,steps)
  movement.append({'case':'stairs','start':start,'end':end,'ground_height':ground(end),'moved_frames':steps,'blocked_frames':blocked,**facing(text,0.)})
  renders=text.count('Renderer: ');ready_count=len(re.findall(READY,text));adb('shell','cmd','window','user-rotation','lock','1')
  text=wait(lambda t:t.count('Renderer: ')>renders and len(re.findall(READY,t))>ready_count and bool(re.findall(r'Model frame submitted at (\d+) x (\d+)',t)) and int(re.findall(r'Model frame submitted at (\d+) x (\d+)',t)[-1][0])>int(re.findall(r'Model frame submitted at (\d+) x (\d+)',t)[-1][1]))
  restored=[float(v) for v in re.findall(READY,text)[-1]];assert math.dist(end,restored)<.05,(end,restored);ground(restored);capture('landscape-restored')
  ready_count=len(re.findall(READY,text));adb('shell','cmd','window','user-rotation','lock','0');text=wait(lambda t:t.count('Renderer: ')>renders+1 and len(re.findall(READY,t))>ready_count and bool(re.findall(r'Model frame submitted at (\d+) x (\d+)',t)) and int(re.findall(r'Model frame submitted at (\d+) x (\d+)',t)[-1][0])<int(re.findall(r'Model frame submitted at (\d+) x (\d+)',t)[-1][1]));capture('portrait-restored')
  # Pause with a finger held: onPause must cancel movement even without UP.
  x,y=control(0,-1);adb('shell','input','touchscreen','motionevent','DOWN',str(x),str(y));time.sleep(.5)
  # ADB keyevent returns before Activity.onPause necessarily runs. Wait for
  # its new native stop-position marker instead of reusing the last release.
  position_count=len(re.findall(POSITION,logs()));adb('shell','input','keyevent','KEYCODE_HOME')
  paused_text=wait(lambda t:len(re.findall(POSITION,t))>position_count);paused=position(paused_text)[0];ready_count=len(re.findall(READY,paused_text));adb('shell','input','touchscreen','motionevent','CANCEL','0','0');time.sleep(.5)
  adb('shell','am','start','-W','-n','com.example.dh2/.MainActivity')
  text=wait(lambda t:t.count('Renderer: ')>renders+2 and len(re.findall(READY,t))>ready_count and t.rfind('Model frame submitted at')>t.rfind('World ready'))
  resumed=[float(v) for v in re.findall(READY,text)[-1]];assert math.dist(paused,resumed)<.05,(paused,resumed)
  x,y=control();adb('shell','input','tap',str(x),str(y));stationary=position(logs())[0];assert math.dist(resumed,stationary)<.05,(resumed,stationary);ground(stationary);capture('resume-stopped')
  movement.append({'case':'pause_cancels_held_touch','paused':paused,'resumed':resumed,'stationary_after_resume':stationary})
  (a.output/'world-lifecycle.log').write_text('\n'.join(line for line in logs().splitlines() if 'DH2Native' in line or 'AndroidRuntime' in line)+'\n')
  # Known rubble boundary near spawn: hold east long enough to reach it.
  text=launch();start=[float(v) for v in re.findall(READY,text)[-1]];x,y=control(1,0)
  route_marker(text)
  adb('shell','input','touchscreen','motionevent','DOWN',str(x),str(y));time.sleep(2.5);text=release(x,y);end,steps,blocked=position(text)
  assert steps>0 and blocked>0 and 0<end[0]-start[0]<300,(start,end,steps,blocked);ground(end);capture('rubble-boundary')
  movement.append({'case':'rubble_boundary','start':start,'end':end,'moved_frames':steps,'blocked_frames':blocked,'ground_height':ground(end),**facing(text,math.pi/2)})
  (a.output/'world-boundary.log').write_text('\n'.join(line for line in logs().splitlines() if 'DH2Native' in line or 'AndroidRuntime' in line)+'\n')
 finally:
  adb('shell','input','touchscreen','motionevent','CANCEL','0','0')
  if prior and prior[0] in ('free','lock'):adb('shell','cmd','window','user-rotation',*prior)
 report={'apk_sha256':hashlib.sha256(a.apk.read_bytes()).hexdigest(),'installed_apk_sha256':installed_sha,'serial':a.serial,'api':adb('shell','getprop','ro.build.version.sdk'),'abi':adb('shell','getprop','ro.product.cpu.abi'),'libraries':libraries,'cases':rows,'movement':movement,'frozen_idle_stable':True,'live_idle_changed_pixels':True,'rotation_position_preserved':True,'pause_cancels_movement':True,'floor_geometry_sha256':hashlib.sha256(a.floor.read_bytes()).hexdigest(),'physical_arm64_tested':False,'original_pf_parity_verified':False,'original_gpu_parity_verified':False,'full_game_playable':False}
 if a.native_floors:report.update(native_floor_records=8,native_graph_nodes=graph_nodes,native_graph_edges=graph_edges,native_selector_collision_used_by_height=True,original_movement_controller_reconstructed=False)
 if a.sewn_floors:report.update(native_neighbour_floor_relations=14,native_validation_references=998,native_floor_sewing_used_by_level_load=True,native_route_search_used_by_gameplay=False)
 if a.graph_search:report.update(native_graph_node_search_used_by_startup_probe=True,crypt_route_probe={'floor_pairs':64,'successful':64,'segments':1598,'state_fnv1a64':'5454073390c7d916'},world_endpoint_selection_reconstructed=False)
 if a.world_route:report.update(native_world_coordinate_search_used_by_startup_probe=True,world_endpoint_selection_reconstructed=True,crypt_world_route_probe={'floor_pairs':64,'successful':64,'direct':0,'graph':64,'segments':2086,'state_fnv1a64':'3a3ab2a724cc383b'},original_smoothing_reconstructed=False,original_findpath_reconstructed=False)
 if a.findpath:report.update(native_findpath_used_by_startup_probe=True,original_smoothing_reconstructed=True,original_findpath_reconstructed=True,original_waypoint_path_step_reconstructed=True,native_findpath_used_by_actor_movement=False,crypt_findpath_probe={'floor_pairs':64,'successful':64,'owned':64,'segments':2086,'state_fnv1a64':'9356b419cae2bc57'})
 if a.floor_motion:report.update(native_floor_motion_used_by_startup_probe=True,original_position_validation_reconstructed=True,original_floor_direction_validation_reconstructed=True,original_obstacle_parent_backend_reconstructed=False,native_floor_motion_used_by_actor_movement=False,crypt_motion_probe={'floor_pairs':64,'position_valid':64,'accepted':64,'direction_valid':64,'state_fnv1a64':'d60ad48039cc85c6'})
 if a.native_objects:report.update(native_object_initialization_used_by_startup_probe=True,original_motion_obstacle_defaults_reconstructed=True,original_init_object_reconstructed=True,original_init_obstacle_reconstructed=True,original_obstacle_parent_backend_reconstructed=True,native_obstacle_registry_used_by_actor_movement=False,original_obstacle_force_reconstructed=False,crypt_objects_probe={'floor_pairs':64,'accepted':64,'registered':64,'relocated':56,'state_fnv1a64':'22a98dfc4102db51'})
 if a.avoidance:report.update(original_obstacle_force_reconstructed=True,original_obstacle_avoidance_reconstructed=True,original_concrete_physical_collision_filter_reconstructed=True,native_avoidance_used_by_startup_probe=True,native_avoidance_used_by_actor_movement=False,original_actor_navigation_producers_reconstructed=False,crypt_avoidance_probe={'floors':8,'force_contributions':16,'adjusted':8,'turn_limited':8,'state_fnv1a64':'09d1d79d2612c945'})
 if a.actor_producers:report.update(original_update_pfobject_reconstructed=True,original_concrete_obstacle_producers_reconstructed=True,original_physical_radius_conversion_reconstructed=True,native_actor_producers_used_by_startup_probe=True,native_actor_producers_used_by_actor_movement=False,original_physical_body_construction_reconstructed=False,original_actor_navigation_producers_reconstructed=False,crypt_producers_probe={'floors':8,'registered':8,'physical_radius_updates':8,'state_fnv1a64':'30c1f3ac1bf81145'})
 if a.heading_control:report.update(original_heading_reconstructed=True,native_heading_used_by_player_movement=True,original_movement_controller_reconstructed=False)
 (a.output/'world-smoke.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k not in ('libraries','cases')}))
if __name__=='__main__':main()
