from pathlib import Path
r=Path(__file__).resolve().parent.parent
s=(r/'port/level-world/tools/probe_canonical_property_declarations_v1.py').read_text()
s=s.replace("root=Path(__file__).resolve().parents[3]","root=Path(__file__).resolve().parent.parent")
s=s.replace("ref=root/'port/level-world/reference/canonical-object-factory-v1'","ref=root/'port/level-world/reference/trigger-zone-v22'")
s=s.replace("[('ObjectBase',0x33f014),('GameObject',0x38cee8),('Character',0x3a9fe4),('OpenableContainer',0x3a1e8c),('AnimatedDecor',0x389dd4)]","[('Zone',0x397df8),('TriggerZone',0x39bfa4)]")
exec(compile(s,str(__file__),'exec'))
