#!/usr/bin/env python3
"""Dump source visibility and user properties for SWAMP module-zero floors."""
from __future__ import annotations

import argparse
import ctypes as c
import importlib.util
import json
from pathlib import Path


def load_audit_module(path: Path):
    spec = importlib.util.spec_from_file_location("scene_cache_audit", path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"could not load scene audit helpers: {path}")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--repo", type=Path, required=True)
    parser.add_argument("--cache", type=Path, required=True)
    parser.add_argument("--library", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    helpers = load_audit_module(args.repo / "port/scene-payloads/tests/audit_cache.py")
    dll = helpers.bind(args.library)
    raw = (args.cache / "data/3d/modules/swamp/swamp.bdae").read_bytes()
    storage = c.create_string_buffer(raw)
    bres = helpers.Bres()
    assert dll.dh2_bres_open(c.byref(bres), storage, len(raw)) == 0
    scene = helpers.Scene()
    assert dll.dh2_scene_open(c.byref(scene), c.byref(bres)) == 0

    floor_nodes = []
    for visual_index in range(scene.visuals):
        visual = helpers.Visual()
        assert dll.dh2_scene_visual(c.byref(scene), visual_index, c.byref(visual)) == 0
        pending = []
        for root_index in range(visual.roots):
            node = helpers.Node()
            assert dll.dh2_scene_root_node(c.byref(visual), root_index, c.byref(node)) == 0
            pending.append((node, 0, []))
        while pending:
            node, depth, parents = pending.pop()
            name = c.string_at(node.name).decode("utf-8", errors="replace")
            node_id = c.string_at(node.id).decode("utf-8", errors="replace")
            if "floor" in name.lower():
                properties = c.c_void_p()
                byte_count = c.c_size_t()
                status = dll.dh2_scene_user_data_string(
                    c.byref(node), c.byref(properties), c.byref(byte_count))
                assert status == 0
                user_data = c.string_at(properties, byte_count.value - 1).decode(
                    "utf-8", errors="replace") if properties.value else ""
                instances = []
                for index in range(node.instances):
                    instance = helpers.Instance()
                    assert dll.dh2_scene_instance(c.byref(node), index,
                                                  c.byref(instance)) == 0
                    instances.append({
                        "type": instance.type,
                        "geometry_url": c.string_at(instance.geometry_url).decode(
                            "utf-8", errors="replace") if instance.geometry_url else None,
                    })
                floor_nodes.append({
                    "visual": c.string_at(visual.id).decode("utf-8", errors="replace"),
                    "node_id": node_id,
                    "node_name": name,
                    "source_record_offset": node.record,
                    "depth": depth,
                    "ancestor_names": parents,
                    "source_visible_word": node.visible,
                    "user_properties": user_data,
                    "instances": instances,
                })
            children = []
            for index in range(node.children):
                child = helpers.Node()
                assert dll.dh2_scene_child_node(c.byref(node), index,
                                                c.byref(child)) == 0
                children.append(child)
            for child in reversed(children):
                pending.append((child, depth + 1, parents + [name]))

    selected_root = "_module_obj_4of4_brdwalk_sw_00"
    module_zero = [row for row in floor_nodes
                   if selected_root in row["ancestor_names"]]
    document = {
        "scope": "raw source BRES scene-node visibility word and user-property string; no engine execution",
        "source": "data/3d/modules/swamp/swamp.bdae",
        "floor_named_node_count": len(floor_nodes),
        "floor_named_nodes_all_have_visible_word_one": all(
            row["source_visible_word"] == 1 for row in floor_nodes),
        "module_zero_parent": selected_root,
        "module_zero_floor_nodes": module_zero,
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(document, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(document, indent=2))


if __name__ == "__main__":
    main()
