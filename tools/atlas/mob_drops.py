"""Join authored actor loot roots to their source-table graph.

This is a data browser, not a loot simulator. Prob/weight fields are preserved
verbatim because their runtime interpretation depends on native gates and player
state that a static table join cannot evaluate.
"""
from collections import defaultdict
from copy import deepcopy


def _key(table, index):
    return (table, int(index))


def _identity(row):
    return {
        "table": row.get("table"),
        "index": row.get("index"),
        "name": row.get("name"),
        "id": row.get("id"),
    }


def build_index(tables, references):
    """Build reusable row/reference indexes for generating many mob dossiers."""
    rows = {}
    by_name = defaultdict(list)
    for row in tables or []:
        table = row.get("table")
        index = row.get("index")
        if table is None or index is None:
            continue
        key = _key(table, index)
        rows[key] = row
        if row.get("name") is not None:
            by_name[(table, row["name"])].append(row)

    refs_by_cell = defaultdict(list)
    for ref in references or []:
        context = ref.get("context") or {}
        table = context.get("table")
        index = context.get("index")
        field = context.get("field")
        if table is not None and index is not None and field is not None:
            refs_by_cell[(table, int(index), field)].append(ref)
    return {"rows": rows, "by_name": by_name, "refs_by_cell": refs_by_cell}


def _root(index, root_index):
    """Accept a group-5 index, a table-row mapping, or an actor row mapping."""
    if isinstance(root_index, dict):
        table = root_index.get("table")
        if table == "loot_table#group5":
            return int(root_index["index"]), root_index
        fields = root_index.get("fields") or {}
        if "Loot" in fields:
            return int(fields["Loot"]), root_index
        if "Loot" in root_index:
            return int(root_index["Loot"]), root_index
        if "loot_index" in root_index:
            return int(root_index["loot_index"]), root_index
        raise ValueError("root_index mapping must be a loot row or carry Loot")
    return int(root_index), None


def build(ctx, tables, references, root_index):
    """Return a JSON-ready, lossless reachable graph for one loot-table root.

    `ctx` may be the result of :func:`build_index` to amortize indexes across
    actor dossiers. Otherwise a fresh index is built from `tables` and
    `references`. `root_index` is the group-5 row index or an actor row.
    """
    idx = ctx if isinstance(ctx, dict) and "rows" in ctx and "refs_by_cell" in ctx else build_index(tables, references)
    rows = idx["rows"]
    loot_id, actor = _root(idx, root_index)
    root_key = _key("loot_table#group5", loot_id)
    root_row = rows.get(root_key)
    result = {
        "root": {"table": root_key[0], "index": loot_id,
                 "name": root_row.get("name") if root_row else None,
                 "row_id": root_row.get("id") if root_row else None},
        "actor": _identity(actor) if actor else None,
        "nodes": [],
        "terminal_items": [],
        "gaps": [],
        "cycles": [],
        "notes": ["Static table graph only; no runtime drop guarantee or probability is inferred."],
    }
    if root_row is None:
        result["gaps"].append({"kind": "dangling-root", "target": {"table": root_key[0], "index": loot_id}})
        return result

    visited = set()
    active = []
    terminals_seen = set()

    def ref_ids(source, field):
        return [r.get("id") for r in idx["refs_by_cell"].get((source["table"], int(source["index"]), field), [])]

    def edge(source, field, target_table, target_index, extra=None):
        target_index = int(target_index)
        target = rows.get(_key(target_table, target_index))
        edge_obj = {"field": field, "source_ref_ids": ref_ids(source, field),
                    "target": {"table": target_table, "index": target_index,
                               "name": target.get("name") if target else None,
                               "row_id": target.get("id") if target else None}}
        if extra:
            edge_obj.update(deepcopy(extra))
        if target is None:
            gap = {"kind": "dangling-edge", **deepcopy(edge_obj)}
            result["gaps"].append(gap)
            edge_obj["status"] = "DANGLING"
        else:
            edge_obj["status"] = "RESOLVED"
        return edge_obj, target

    def visit(table, row_index, via=None):
        key = _key(table, row_index)
        if key in active:
            result["cycles"].append({"path": [{"table": t, "index": i} for t, i in active] +
                                      [{"table": table, "index": int(row_index)}]})
            return
        if key in visited:
            return
        row = rows.get(key)
        if row is None:
            result["gaps"].append({"kind": "dangling-node", "target": {"table": table, "index": int(row_index)}})
            return
        visited.add(key)
        active.append(key)
        node = {"identity": _identity(row), "fields": deepcopy(row.get("fields") or {}), "via": via, "edges": []}
        result["nodes"].append(node)
        fields = row.get("fields") or {}

        if table == "loot_table#group5":
            for field in ("FixedLootEntries", "RandomLootEntries"):
                for position, entry in enumerate(fields.get(field) or []):
                    list_id = entry.get("ItemListID", -1)
                    if int(list_id) >= 0:
                        e, child = edge(row, f"{field}[{position}].ItemListID", "loot_table#group2", list_id,
                                        {"entry": deepcopy(entry), "entry_kind": "fixed" if field.startswith("Fixed") else "random"})
                        node["edges"].append(e)
                        if child:
                            visit("loot_table#group2", list_id, {"from": _identity(row), "field": e["field"]})
                    power_id = entry.get("ItemPowerList", -1)
                    if int(power_id) >= 0:
                        e, child = edge(row, f"{field}[{position}].ItemPowerList", "item_powers#group0", power_id,
                                        {"entry_kind": "item-power-list"})
                        node["edges"].append(e)
                        if child:
                            visit("item_powers#group0", power_id, {"from": _identity(row), "field": e["field"]})
            for position, child_id in enumerate(fields.get("SubLoots") or []):
                e, child = edge(row, f"SubLoots[{position}]", "loot_table#group5", child_id)
                node["edges"].append(e)
                if child:
                    visit("loot_table#group5", child_id, {"from": _identity(row), "field": e["field"]})
        elif table == "loot_table#group2":
            for position, item_ref in enumerate(fields.get("List") or []):
                item_id = item_ref.get("Item", -1)
                if int(item_id) < 0:
                    continue
                e, child = edge(row, f"List[{position}].Item", "loot_table#group3", item_id,
                                {"item_list_entry": deepcopy(item_ref)})
                node["edges"].append(e)
                if child:
                    item_key = _key("loot_table#group3", item_id)
                    sig = (key, position, item_key)
                    if sig not in terminals_seen:
                        terminals_seen.add(sig)
                        result["terminal_items"].append({
                            "item": _identity(child),
                            "item_fields": deepcopy(child.get("fields") or {}),
                            "item_list": _identity(row),
                            "item_list_entry": deepcopy(item_ref),
                            "edge": deepcopy(e),
                        })
                    visit("loot_table#group3", item_id, {"from": _identity(row), "field": e["field"]})
        elif table == "item_powers#group0":
            for position, power_ref in enumerate(fields.get("List") or []):
                power_id = power_ref.get("Power", -1)
                if int(power_id) < 0:
                    continue
                e, child = edge(row, f"List[{position}].Power", "item_powers#group1", power_id,
                                {"power_list_entry": deepcopy(power_ref)})
                node["edges"].append(e)
                if child:
                    visit("item_powers#group1", power_id, {"from": _identity(row), "field": e["field"]})
        elif table == "item_powers#group1":
            # AttrBonusList and SpecialEffect are kept as authored terminal
            # metadata; their independent native consumers determine effects.
            pass
        active.pop()

    visit(*root_key)
    result["counts"] = {"nodes": len(result["nodes"]), "terminal_items": len(result["terminal_items"]),
                         "gaps": len(result["gaps"]), "cycles": len(result["cycles"])}
    return result
