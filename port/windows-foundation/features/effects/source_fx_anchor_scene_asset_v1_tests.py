import xml.etree.ElementTree as ET
from pathlib import Path

root = Path(__file__).resolve().parents[4]
mgp = root / ".local-inputs/v20/asset-stage/assets/data/3d/modules/swamp/mgp/bossroom_witch_.mgp"
tree = ET.parse(mgp)
matches = [node for node in tree.getroot().findall("GameObject")
           if node.get("name") == "_prim_Waypoint_WitchSpotter"]
assert len(matches) == 1, "expected exactly one WitchSpotter source object"
source = matches[0]
assert source.get("type") == "Block"
assert source.get("gametype") == "Dummy"
assert source.get("_templateName") == "Waypoint"
assert source.get("position") == "-503.271,1505.86,128.244"
assert source.get("scale") == "1.0,1.0,1.0"
assert source.get("rotation") == "0.0,0.0,0.0"
print("PASS | WitchSpotter anchor geometry is read from the exact staged MGP row")
