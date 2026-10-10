"""Decode unmodified original SWF navigation with the existing repo decoder."""
from pathlib import Path
import hashlib
import importlib.util
import json
ROOT = Path(__file__).resolve().parents[5]
spec = importlib.util.spec_from_file_location("creation_decoder", ROOT / "port/android-native/tests/test_creation_continuation_live_v1.py")
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
swf = ROOT / "port/android-native/app/src/main/assets/original-cache/data/menus/dqmenus_droid.swf"
raw, actions = module.movie_actions(swf)
selected = []
for key, (_, rows) in actions.items():
    if key in [("root/sprite93",0,0),("root/sprite428",0,0),("root/sprite428",29,0),("root/sprite510",0,0),("root/sprite471",0,0)]:
        selected.append({"sprite":key[0],"frame":key[1],"action_index":key[2],"actions":rows})
output = {"source":str(swf.relative_to(ROOT)),"sha256":hashlib.sha256(raw).hexdigest(),"selected_actions":selected}
Path(__file__).with_name("authored_navigation_evidence.json").write_text(json.dumps(output,indent=2)+"\n",encoding="utf-8")
print("Recovered",len(selected),"original navigation action tags")
