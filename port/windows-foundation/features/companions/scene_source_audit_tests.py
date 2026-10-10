"""Pin Act 1 companion placement, visibility, and idle claims to source."""
from pathlib import Path
import hashlib
import json
import re
import xml.etree.ElementTree as ET


ROOT = Path(__file__).resolve().parents[4]
MGP = ROOT / ".local-inputs/windows-source-clock-v19-preview-2/assets/original-cache/data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp"
CAMPAIGN = ROOT / ".local-inputs/windows-encounter-source/original-campaign.xml"
DECLARATIONS = ROOT / "port/level-world/canonical_property_declarations_v1.inc"
LIFECYCLE_REPORT = ROOT / "port/windows-foundation/reports/live-lifecycle-integration.json"
VIDEO_MANIFEST = ROOT / ".local-inputs/referenceframes/dh2-act1/manifest.json"
SCENE_MANIFEST = ROOT / ".local-inputs/referenceframes/dh2-act1/first-spawn/manifest.json"


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


assert sha256(MGP) == "651eac91518c0a22362c22cb3ffe15972649e259a6af59d04ba028a90a7baf31"
assert sha256(CAMPAIGN) == "3ab2055920a8b648b16d77aa07b0435cbd899c624f5be3bcc145e2133c5cdd14"

root = ET.parse(MGP).getroot()
objects = {node.attrib.get("name"): node.attrib for node in root.iter("GameObject")}
priest = objects["_prim_NPC_PriestGood"]
faery = objects["_prim_Faery"]
assert priest["position"] == "793.9,-617.658,272.064"
assert priest["gametype"] == "Character" and priest["charpropsname"] == "WanderingPriest"
assert priest["_templateName"] == "NPC" and priest["activate_cond"] == "RENE_FOLLOW"
assert priest["isGlobal"] == "1"
assert faery["position"] == "544.191,414.689,-108.175"
assert faery["gametype"] == "Character" and faery["charpropsname"] == "DefaultFairy"
assert faery["_templateName"] == "Faery" and faery["isGlobal"] == "1"
for actor in (priest, faery):
    assert "ai_state" not in actor and "ai_state_visible" not in actor and "startanim" not in actor

# The same authored file has explicit visibility/state overrides for unrelated
# intro enemies. They are deliberately not evidence for companions.
troll = objects["_prim_ActorTroll"]
assert troll["ai_state_visible"] == "0" and troll["ai_state"] == "Limbus"

declarations = DECLARATIONS.read_text()
assert 'add("ai_state",0x13cc,std::string{""})' in declarations
assert 'add("ai_state_visible",0x13e4,std::uint8_t{1})' in declarations
life = json.loads(LIFECYCLE_REPORT.read_text())
assert life["authored_defaults"]["ai_state_visible"].startswith("Original Character declaration13e4 defaults to1")

campaign = ET.parse(CAMPAIGN).getroot()
scripts = {node.attrib.get("name"): node for node in campaign.iter("script")}
swamp = scripts["Swamp_Intro"]
commands = {int(node.attrib["index"]): node for node in swamp.findall("command")}
def command(index: int) -> tuple[str, dict[str, str]]:
    node = commands[index]
    return node.attrib["className"], {child.attrib.get("value", ""): child.attrib.get("value", "")
                                       for child in node if child.tag == "string"}

assert command(12) == ("Script_HideActor", {"_prim_NPC_PriestGood": "_prim_NPC_PriestGood"})
assert command(13) == ("Script_HideActor", {"_prim_Faery": "_prim_Faery"})
assert command(14)[0] == "Script_PlayAnimByName"
assert command(14)[1] == {"idle": "idle", "_anim_cage_001": "_anim_cage_001"}
assert command(41) == ("Script_ShowActor", {"_prim_NPC_PriestGood": "_prim_NPC_PriestGood"})
assert command(42) == ("Script_ShowActor", {"_prim_Faery": "_prim_Faery"})
assert command(43)[0] == "Script_SetActorPosition"
assert command(45)[0] == "Script_PlayActorAnim" and "_prim_NPC_PriestGood" in command(45)[1]
assert command(46)[0] == "Script_PlayActorAnim" and "_prim_Faery" in command(46)[1]
assert command(112)[0] == "Script_PlayAnimByName"
assert command(112)[1] == {"idle_opened": "idle_opened", "_anim_cage_001": "_anim_cage_001"}

# The separate Camp_Intro assigns explicit actor positions from named waypoints;
# no companion-specific static offset or follower distance is authored here.
camp = scripts["Camp_Intro"]
camp_commands = {int(node.attrib["index"]): node for node in camp.findall("command")}
assert camp_commands[6].attrib["className"] == "Script_SetActorPosition"
assert camp_commands[7].attrib["className"] == "Script_SetActorPosition"
assert any(x.attrib.get("value") == "_prim_Waypoint_CampIntro_RENE"
           for x in camp_commands[6].findall("string"))
assert any(x.attrib.get("value") == "_prim_Waypoint_CampIntro_FAERY"
           for x in camp_commands[7].findall("string"))

video = json.loads(VIDEO_MANIFEST.read_text())
scene = json.loads(SCENE_MANIFEST.read_text())
assert video["source_version"] == "1.0.3" and video["recovered_version"] == "1.0.2"
assert scene["version"] == "1.0.3"
for timestamp, expected in ((210, "b6c22b5f9cb7eb7282d9cc15092c14cb2a87abd8ca0362d955a76fd4614144f6"),
                            (212, "2a37ee5e1577919d96413b200812af3b666c73c859de401968f67100d246e163"),
                            (230, "26e644b55f8f3a7e7f96503a2bc8f70d9bf22fa3ed2bc2af43ee36faf7b5de02")):
    frame = next(x for x in scene["frames"] if x["timestamp_seconds"] == timestamp)
    path = ROOT / Path(frame["path"]).relative_to(ROOT)
    assert sha256(path) == expected

print("companion source placement, script-owned visibility/idle, and reference-frame evidence passed")
