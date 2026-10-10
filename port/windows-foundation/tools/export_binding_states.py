#!/usr/bin/env python3
"""Add exact original CharAnim states without regenerating existing bindings.

Requires a native binding_clip_metadata probe, compiled against the recovered
loaders. Existing states, AI, sequence policies, factions, and clip metadata are
preserved. Unsupported table schemas/redirect cycles/conflicting assets reject.
"""
from __future__ import annotations
import argparse
import copy
import hashlib
import json
import math
from pathlib import Path, PurePosixPath
import struct
import subprocess
import xml.etree.ElementTree as ET
import zipfile


class Reader:
    def __init__(self, data):
        self.data, self.offset = data, 0

    def value(self, kind="i"):
        size = struct.calcsize("<" + kind)
        if self.offset + size > len(self.data):
            raise ValueError("Truncated original table")
        result = struct.unpack_from("<" + kind, self.data, self.offset)[0]
        self.offset += size
        return result

    def count(self):
        result = self.value("I")
        if result > 100000:
            raise ValueError("Original count exceeds budget")
        return result

    def integers(self):
        return [self.value() for _ in range(self.count())]

    def strings(self):
        result = []
        for _ in range(self.count()):
            size = self.count()
            if self.offset + size > len(self.data):
                raise ValueError("Truncated original string")
            result.append(self.data[self.offset:self.offset + size].decode("ascii"))
            self.offset += size
        return result


def tables(read):
    names = Reader(read("data/pydata/animations_pyarraynames.bin"))
    sequence_names, camera_names, character_names = names.strings(), names.strings(), names.strings()
    schema = Reader(read("data/pydata/animations_pystructnames.bin"))
    steps, sequences, cameras, states = [schema.strings() for _ in range(4)]
    if steps != ["AnchorFX", "Anim", "BlendOut", "Cam", "CamDir", "FX", "MoveGO", "RandomCam", "Redir", "Sound", "Speed", "Swoosh"] or sequences != ["Loop", "Steps", "Type"] or cameras != ["CamAnims", "Crit", "Idle", "Shake", "Template"]:
        raise ValueError("Unsupported original animation schema")
    clips = Reader(read("data/pydata/animations_dictionary_pyarray.bin")).strings()
    data = Reader(read("data/pydata/animations_pyarray.bin"))
    rows = []
    for _ in range(data.count()):
        loop, sequence_steps = data.value(), []
        for _ in range(data.count()):
            step = {}
            for field in steps:
                step[field] = (data.integers() if field == "RandomCam" else data.value("B" if field in ["AnchorFX", "CamDir", "MoveGO", "Swoosh"] else "f" if field == "Speed" else "i"))
            if step["Redir"] not in [0, 1] or not math.isfinite(step["Speed"]) or step["Speed"] <= 0:
                raise ValueError("Invalid original animation step")
            sequence_steps.append(step)
        rows.append(dict(loop=loop, type=data.value(), steps=sequence_steps))
    for _ in range(data.count()):
        data.integers()
        for _ in range(4):
            data.value()
    characters = []
    for _ in range(data.count()):
        characters.append({state: data.integers() if state in ["Interact", "Spells"] else [data.value()] for state in states})
    if len(rows) != len(sequence_names) or len(characters) != len(character_names) or data.offset != len(data.data):
        raise ValueError("Original animation dimensions/suffix differ")
    return rows, sequence_names, characters, clips


def signature(node):
    return [node.tag, sorted(node.attrib.items()), (node.text or "").strip(), [signature(child) for child in node]]


def state_node(state, ids, rows, names, clips, needed, model):
    result = ET.Element("state", name=state)

    def emit(sequence_id, parent, path=()):
        if sequence_id in path or len(path) >= 32 or not 0 <= sequence_id < len(rows):
            raise ValueError("Invalid original sequence redirect")
        for order, step in enumerate(rows[sequence_id]["steps"]):
            child = ET.SubElement(parent, "step", index=str(order), animationId=str(step["Anim"]), redirect=str(step["Redir"]), speed=str(step["Speed"]), blendOut=str(step["BlendOut"]), moveGO=str(step["MoveGO"]))
            if step["Redir"]:
                emit(step["Anim"], child, path + (sequence_id,))
            elif step["Anim"] >= 0:
                uri = clips[step["Anim"]]
                child.set("uri", uri)
                needed.setdefault(uri, model)

    for sequence_id in ids:
        if sequence_id == -1:
            continue
        if not 0 <= sequence_id < len(rows):
            raise ValueError("Original state sequence outside table")
        row = rows[sequence_id]
        child = ET.SubElement(result, "sequence", id=str(sequence_id), name=names[sequence_id], loop=str(row["loop"]), type=str(row["type"]))
        emit(sequence_id, child)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True)
    parser.add_argument("--bindings", type=Path, required=True)
    parser.add_argument("--profiles", type=Path, required=True)
    parser.add_argument("--asset-root", type=Path, required=True)
    parser.add_argument("--metadata-probe", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--audit", type=Path, required=True)
    parser.add_argument("--state", action="append", required=True)
    args = parser.parse_args()
    original = args.bindings.read_bytes()
    root = ET.fromstring(original)
    if root.tag != "meleeBindings":
        raise ValueError("Expected authoritative meleeBindings root")
    profiles = {actor.attrib["id"]: actor for actor in ET.parse(args.profiles).getroot().findall("actor")}
    source_inputs, staged, additions, needed = {}, [], [], {}
    with zipfile.ZipFile(args.cache) as cache:
        index = {}
        for member in cache.namelist():
            if "/files/" in member:
                key = member.split("/files/", 1)[1].lower()
                if key in index:
                    raise ValueError("Ambiguous original cache resource: " + key)
                index[key] = member

        def read(uri):
            safe = PurePosixPath(uri.replace("\\", "/"))
            if safe.is_absolute() or ".." in safe.parts or ":" in str(safe):
                raise ValueError("Unsafe original cache URI")
            member = index[safe.as_posix().lower()]
            raw = cache.read(member)
            source_inputs[member] = dict(bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())
            return raw

        rows, names, characters, clips = tables(read)
        for actor in root.findall("actor"):
            identity = actor.attrib["id"]
            if identity not in profiles:
                raise ValueError("Explicit actor profile unavailable: " + identity)
            profile = profiles[identity]
            if "propertyRow" in profile.attrib and actor.attrib.get("propertyRow") != profile.attrib["propertyRow"]:
                raise ValueError("Actor profile property-row identity differs: " + identity)
            animation_table = int(profile.attrib["animationTable"])
            if not 0 <= animation_table < len(characters):
                raise ValueError("Character animation row outside table")
            for state in args.state:
                if state not in characters[animation_table]:
                    raise ValueError("Unknown authored state: " + state)
                node = state_node(state, characters[animation_table][state], rows, names, clips, needed, actor.attrib["model"])
                existing = actor.find("state[@name='" + state + "']")
                if existing is not None:
                    if signature(existing) != signature(node):
                        raise ValueError("Existing state differs from exact source: " + identity + "/" + state)
                    continue
                actor.append(node)
                additions.append(dict(actor=identity, animationTable=animation_table, state=state, sequences=[int(child.attrib["id"]) for child in node]))

        def stage(uri):
            raw = read(uri)
            target = args.asset_root / "original-cache" / uri.lower()
            if target.exists():
                if target.read_bytes() != raw:
                    raise ValueError("Conflicting staged original asset: " + str(target))
            else:
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(raw)
                staged.append(str(target))
            return target

        metadata_root = root.find("clips")
        if metadata_root is None:
            metadata_root = ET.SubElement(root, "clips")
        existing_clips = {node.attrib["uri"].lower(): node for node in metadata_root.findall("clip")}
        if len(existing_clips) != len(metadata_root.findall("clip")):
            raise ValueError("Duplicate case-folded original clip metadata URI")
        new_clips = []
        for uri, model in needed.items():
            clip_path, model_path = stage(uri), stage(model)
            result = subprocess.run([str(args.metadata_probe.resolve()), str(model_path.resolve()), str(clip_path.resolve())], capture_output=True, text=True, check=True, timeout=60)
            metadata = json.loads(result.stdout)
            node = ET.Element("clip", uri=uri, startMs=str(metadata["startMs"]), endMs=str(metadata["endMs"]))
            for marker in metadata["markers"]:
                ET.SubElement(node, "marker", **{key: str(value) for key, value in marker.items()})
            if uri.lower() in existing_clips:
                existing = existing_clips[uri.lower()]
                # Asset URI lookup is case-insensitive; retain existing spelling.
                node.set("uri", existing.attrib["uri"])
                if signature(existing) != signature(node):
                    raise ValueError("Existing clip metadata differs from native source: " + uri)
            else:
                metadata_root.append(node)
                new_clips.append(uri)
        # Removing only additions must reproduce every previous node/attribute.
        preserved = copy.deepcopy(root)
        for entry in additions:
            actor = preserved.find("actor[@id='" + entry["actor"] + "']")
            actor.remove(actor.find("state[@name='" + entry["state"] + "']"))
        for uri in new_clips:
            parent = preserved.find("clips")
            parent.remove(parent.find("clip[@uri='" + uri + "']"))
        if signature(preserved) != signature(ET.fromstring(original)):
            raise ValueError("Additive export changed preserved binding metadata")
    ET.indent(root)
    payload = ET.tostring(root, encoding="utf-8", xml_declaration=True)
    if args.bindings.read_bytes() != original:
        raise ValueError("Bindings changed during export")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(payload)
    report = dict(status="PASS", additions=additions, newClipMetadata=new_clips, staged=staged, inputs=source_inputs, preserved_base_metadata=True, output_sha256=hashlib.sha256(payload).hexdigest(), previous_sha256=hashlib.sha256(original).hexdigest(), exporter_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(), metadata_probe_sha256=hashlib.sha256(args.metadata_probe.read_bytes()).hexdigest(), profiles_sha256=hashlib.sha256(args.profiles.read_bytes()).hexdigest())
    args.audit.parent.mkdir(parents=True, exist_ok=True)
    args.audit.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(dict(status="PASS", added_states=len(additions), new_clips=len(new_clips), staged=len(staged))))


if __name__ == "__main__":
    main()
