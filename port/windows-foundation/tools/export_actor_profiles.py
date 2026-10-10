#!/usr/bin/env python3
"""Export audited configuration records to generic actor profile XML.

Does not infer actor/class names, animation tables, decoded stats or equipment.
Plain clip lists retain occurrence order with unauthored neutral weight 1. Raw
property integers remain uninterpreted unless an encoding accompanies the source.
"""
from __future__ import annotations
import argparse
import json
import math
import sys
from pathlib import Path
import xml.etree.ElementTree as ET


def export(source: dict) -> ET.Element:
    configurations = source.get("configurations")
    if not isinstance(configurations, list) or not configurations:
        raise ValueError("Source must contain nonempty configurations array")
    root = ET.Element("actorProfiles", version="1")
    shared_encodings = source.get("property_encodings", {})
    if not isinstance(shared_encodings, dict) or any(
            not isinstance(name, str) or not isinstance(encoding, str) or not encoding
            for name, encoding in shared_encodings.items()):
        raise ValueError("property_encodings must map names to explicit nonempty encoding strings")
    seen = set()
    for configuration in configurations:
        identity = configuration.get("character")
        model = configuration.get("model")
        if not isinstance(identity, str) or not identity or identity in seen:
            raise ValueError("Every configuration needs a unique character name")
        if not isinstance(model, str) or not model:
            raise ValueError(f"Configuration {identity} has no model")
        seen.add(identity)
        states = configuration.get("states", {})
        if not isinstance(states, dict):
            raise ValueError(f"States must be an object: {identity}")
        attributes = {"id": identity, "model": model, "character": identity}
        for key, target in (("property_row", "propertyRow"), ("animation_table", "animationTable"),
                            ("animation_table_name", "animationTableName")):
            if key in configuration:
                attributes[target] = str(configuration[key])
        templates = states.get("Template", [])
        if templates:
            first = templates[0]
            attributes["template"] = first if isinstance(first, str) else first["uri"]
        actor = ET.SubElement(root, "actor", attributes)
        for name, clips in states.items():
            if not isinstance(name, str) or not name or not isinstance(clips, list):
                raise ValueError(f"Invalid state list for {identity}")
            state = ET.SubElement(actor, "state", name=name)
            for clip in clips:
                if isinstance(clip, str):
                    clip_attributes = {"uri": clip, "weight": "1", "weightAuthored": "false"}
                elif isinstance(clip, dict) and isinstance(clip.get("uri"), str):
                    weight = clip.get("weight", 1)
                    if isinstance(weight, bool) or not isinstance(weight, (int, float)) or not math.isfinite(weight) or weight < 0:
                        raise ValueError(f"Invalid clip weight for {identity}/{name}")
                    clip_attributes = {"uri": clip["uri"], "weight": str(weight),
                                       "weightAuthored": "true" if "weight" in clip else "false"}
                else:
                    raise ValueError(f"Invalid clip for {identity}/{name}")
                if not clip_attributes["uri"]:
                    raise ValueError(f"Empty clip URI for {identity}/{name}")
                ET.SubElement(state, "clip", clip_attributes)
        properties = configuration.get("properties", {})
        if not isinstance(properties, dict):
            raise ValueError(f"Properties must be an object for {identity}")
        encodings = dict(shared_encodings)
        configuration_encodings = configuration.get("property_encodings", {})
        if not isinstance(configuration_encodings, dict) or any(
                not isinstance(name, str) or not isinstance(encoding, str) or not encoding
                for name, encoding in configuration_encodings.items()):
            raise ValueError(f"Invalid property_encodings for {identity}")
        encodings.update(configuration_encodings)
        for name, property_value in properties.items():
            value = property_value.get("raw") if isinstance(property_value, dict) else property_value
            fallback_encoding = encodings.get(name, "original-table-raw-integer-uninterpreted")
            encoding = (property_value.get("encoding", fallback_encoding)
                        if isinstance(property_value, dict) else fallback_encoding)
            if isinstance(value, bool) or not isinstance(value, int) or not -(2**63) <= value < 2**63:
                raise ValueError(f"Invalid raw integer property {identity}/{name}")
            if not isinstance(encoding, str) or not encoding:
                raise ValueError(f"Invalid property encoding {identity}/{name}")
            ET.SubElement(actor, "property", name=name, raw=str(value), encoding=encoding)
    return root


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    root = export(json.loads(args.source.read_text(encoding="utf-8-sig")))
    ET.indent(root)
    payload = ET.tostring(root, encoding="utf-8", xml_declaration=True)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("xb") as target:
        target.write(payload)
    print(json.dumps({"output": str(args.output), "profiles": len(root)}))


if __name__ == "__main__":
    try:
        main()
    except (ValueError, OSError, KeyError, TypeError) as error:
        print(f"Actor profile export failed: {error}", file=sys.stderr)
        sys.exit(1)
