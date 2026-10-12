"""Actual original FSM Update and bounded virtual bodies versus optimized native
owned-metadata frame. The frozen state oracle's deeper services stay explicit;
stun/scare fixture deliveries deliberately make no state change as in original.
Transition/event/getter comparisons are unchanged regression kernels.
"""
from pathlib import Path
R=Path(__file__).resolve().parent
source=(R/'character_state_differential.py').read_text()
assert "new.invoke('dh2_character_state_update'" in source
source=source.replace("new.invoke('dh2_character_state_update'","new.invoke('dh2_state_owner_frame_update_fixture'")
source=source.replace("report={'original_sha256':", "report={'validation':'PASS','owner_frame_original_update_cases':new.invoke('dh2_state_owner_frame_update_count',[]),'original_sha256':")
source=source.replace("'scope':__doc__,", "'scope':"+repr(__doc__)+",")
paths=('character_state.cpp','character_state.hpp','character_native_fsm.cpp','character_native_fsm.hpp','character_state_owner.hpp','character_state_owner_data.inc','character_state_owner_frame.cpp','character_state_owner_frame.hpp','tests/character_state_owner_frame_oracle.cpp','tests/character_state_owner_frame_differential.py','tests/character_state_differential.py')
inject="report['source_sha256']={str((ROOT/x).relative_to(ROOT.parents[1])).replace('\\\\','/'):hashlib.sha256((ROOT/x).read_bytes()).hexdigest() for x in "+repr(paths)+"};report['packaged_APK']=False;report['full_AI']=False;"
source=source.replace("args.report.parent.mkdir(parents=True,exist_ok=True);",inject+"args.report.parent.mkdir(parents=True,exist_ok=True);")
exec(compile(source,str(R/'character_state_differential.py'),'exec'),{'__name__':'__main__','__file__':str(R/'character_state_differential.py')})
