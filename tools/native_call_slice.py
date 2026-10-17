#!/usr/bin/env python3
"""Generate a bounded direct-call slice from the recovered Ghidra pseudocode.

This reads the checked-in pseudocode index and shards. It does not run Ghidra,
execute game code, or claim a complete call graph. In particular, indirect,
virtual, callback, and Lua-dispatched calls need separate evidence.
"""

from __future__ import annotations

import argparse
import csv
import json
import re
import sys
from collections import defaultdict, deque
from pathlib import Path
from typing import Any


DIRECT_CALL = re.compile(r"\b(_Z[A-Za-z0-9_.$]+|FUN_[0-9A-Fa-f]+)\s*\(")
HEX_ADDRESS = re.compile(r"^(?:0x)?[0-9a-fA-F]+$")


def _direct_call_tokens(body: str, own_name: str) -> list[str]:
    """Return named call tokens, excluding only the function definition."""
    tokens = []
    skipped_definition = False
    for match in DIRECT_CALL.finditer(body):
        token = match.group(1)
        if token == own_name and not skipped_definition:
            skipped_definition = True
            continue
        tokens.append(token)
    return tokens


def _address(value: str) -> int:
    """Parse an ELF address. Bare addresses are interpreted as hexadecimal."""
    text = value.strip()
    if not HEX_ADDRESS.fullmatch(text):
        raise ValueError(f"not a function name or hexadecimal address: {value!r}")
    return int(text[2:] if text.lower().startswith("0x") else text, 16)


def _load_symbol_aliases(path: Path | None) -> dict[str, set[int]]:
    aliases: dict[str, set[int]] = defaultdict(set)
    if path is None or not path.is_file():
        return aliases
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        for row in csv.DictReader(stream):
            try:
                address = int(row["address"], 10)
                names = json.loads(row.get("aliases", "[]"))
            except (KeyError, TypeError, ValueError, json.JSONDecodeError):
                continue
            for item in names:
                if not isinstance(item, dict):
                    continue
                for key in ("name", "demangled"):
                    name = item.get(key)
                    if isinstance(name, str) and name:
                        aliases[name].add(address)
    return aliases


class PseudocodeIndex:
    def __init__(self, decomp_dir: Path, symbols_csv: Path | None = None):
        self.decomp_dir = decomp_dir
        index_path = decomp_dir / "function-index.jsonl"
        self.rows: dict[int, dict[str, Any]] = {}
        self.by_name: dict[str, set[int]] = defaultdict(set)
        self.shards: dict[str, bytes] = {}
        self.aliases = _load_symbol_aliases(symbols_csv)
        self.aliases_by_address: dict[int, set[str]] = defaultdict(set)
        for name, addresses in self.aliases.items():
            for address in addresses:
                self.aliases_by_address[address].add(name)
        self.callees: dict[int, list[int]] = {}
        self.unindexed: dict[int, list[str]] = {}
        self.callers: dict[int, set[int]] = defaultdict(set)

        with index_path.open("r", encoding="utf-8-sig") as stream:
            for line_number, line in enumerate(stream, 1):
                if not line.strip():
                    continue
                row = json.loads(line)
                address = _address(str(row["elf_address"]))
                # Keep one output record per ELF start. Prefer a successful
                # decompiler record if the export contains an alias duplicate.
                previous = self.rows.get(address)
                if previous is None or (not previous.get("success") and row.get("success")):
                    self.rows[address] = row
                self.by_name[row["name"]].add(address)

        for row in self.rows.values():
            address = _address(str(row["elf_address"]))
            body = self._body(row)
            tokens = _direct_call_tokens(body, str(row.get("name", ""))) if body else []
            targets: list[int] = []
            missing: list[str] = []
            for token in tokens:
                candidates = self.by_name.get(token, set())
                if len(candidates) == 1:
                    target = next(iter(candidates))
                    if target not in targets:
                        targets.append(target)
                        self.callers[target].add(address)
                elif token not in missing:
                    missing.append(token)
            self.callees[address] = targets
            self.unindexed[address] = missing

    def _body(self, row: dict[str, Any]) -> str:
        if not row.get("success"):
            return ""
        shard_name = row.get("file")
        if not isinstance(shard_name, str):
            return ""
        shard = self.shards.get(shard_name)
        if shard is None:
            try:
                shard = (self.decomp_dir / shard_name).read_bytes()
            except OSError:
                return ""
            self.shards[shard_name] = shard
        start = int(row.get("byte_offset", 0))
        end = start + int(row.get("byte_length", 0))
        return shard[start:end].decode("utf-8", "replace")

    def resolve(self, spec: str) -> int:
        try:
            address = _address(spec)
        except ValueError:
            candidates = self.by_name.get(spec, set())
            if not candidates:
                candidates = self.aliases.get(spec, set())
            if len(candidates) == 1:
                address = next(iter(candidates))
            elif not candidates:
                raise ValueError(f"root does not resolve to an indexed function: {spec!r}")
            else:
                choices = ", ".join(f"0x{item:x}" for item in sorted(candidates))
                raise ValueError(f"ambiguous function name {spec!r}; use an address ({choices})")
        if address not in self.rows:
            raise ValueError(f"root address is absent from the decompiler index: 0x{address:x}")
        return address

    def names_for(self, address: int) -> list[str]:
        row = self.rows[address]
        names = {str(row.get("name", ""))}
        names.update(self.aliases_by_address.get(address, set()))
        return sorted(name for name in names if name)

    def name(self, address: int) -> str:
        return str(self.rows[address].get("name", f"0x{address:x}"))

    def pseudo_snippet(self, address: int, limit: int) -> str:
        if limit <= 0:
            return ""
        row = self.rows[address]
        body = self._body(row)
        if not body:
            return ""
        own_name = self.name(address)
        lines = []
        definition_line = None
        for match in DIRECT_CALL.finditer(body):
            if match.group(1) == own_name:
                definition_line = body.count("\n", 0, match.start())
                break
        for line_number, line in enumerate(body.splitlines()):
            stripped = line.strip()
            if not stripped or stripped.startswith("/*") or stripped.startswith("//"):
                continue
            tokens = DIRECT_CALL.findall(line)
            if tokens and line_number != definition_line:
                lines.append(stripped)
            if len(lines) == 3:
                break
        if not lines:
            lines = [line.strip() for line in body.splitlines() if line.strip()][:2]
        snippet = "\n".join(lines)
        if len(snippet) > limit:
            snippet = snippet[: max(0, limit - 1)].rstrip() + "…"
        return snippet


def analyze_slice(
    decomp_dir: Path,
    roots: list[str],
    max_depth: int = 2,
    name_prefixes: list[str] | None = None,
    symbols_csv: Path | None = None,
    snippet_chars: int = 360,
    edge_limit: int = 16,
) -> dict[str, Any]:
    if max_depth < 0:
        raise ValueError("max_depth must be nonnegative")
    if edge_limit < 0:
        raise ValueError("edge_limit must be nonnegative")
    if not roots:
        raise ValueError("at least one root function is required")

    index = PseudocodeIndex(decomp_dir, symbols_csv)
    root_addresses: list[int] = []
    for spec in roots:
        address = index.resolve(spec)
        if address not in root_addresses:
            root_addresses.append(address)

    # Multi-source BFS gives a deterministic nearest-root shortest path.
    distances: dict[int, int] = {}
    parents: dict[int, int | None] = {}
    root_for: dict[int, int] = {}
    queue: deque[int] = deque()
    for address in root_addresses:
        distances[address] = 0
        parents[address] = None
        root_for[address] = address
        queue.append(address)
    while queue:
        current = queue.popleft()
        depth = distances[current]
        if depth >= max_depth:
            continue
        for target in index.callees.get(current, []):
            if target in distances:
                continue
            distances[target] = depth + 1
            parents[target] = current
            root_for[target] = root_for[current]
            queue.append(target)

    prefixes = name_prefixes or []
    emitted = []
    for address, depth in distances.items():
        names = index.names_for(address)
        matches = not prefixes or any(name.startswith(prefix)
                                      for name in names for prefix in prefixes)
        if not matches and address not in root_addresses:
            continue
        row = index.rows[address]
        path_addresses = []
        cursor: int | None = address
        while cursor is not None:
            path_addresses.append(cursor)
            cursor = parents[cursor]
        path_addresses.reverse()

        all_direct_callees = []
        for target in index.callees.get(address, []):
            all_direct_callees.append({"address": f"0x{target:x}", "name": index.name(target)})
        all_direct_callees.extend({"address": None, "name": name, "indexed": False}
                                   for name in index.unindexed.get(address, []))

        all_direct_callers = [
            {"address": f"0x{caller:x}", "name": index.name(caller)}
            for caller in sorted(index.callers.get(address, set()),
                                 key=lambda item: (index.name(item), item))
        ]
        direct_callees = (all_direct_callees if edge_limit == 0
                          else all_direct_callees[:edge_limit])
        direct_callers = (all_direct_callers if edge_limit == 0
                          else all_direct_callers[:edge_limit])
        function = {
            "address": f"0x{address:x}",
            "name": index.name(address),
            "aliases": [name for name in names if name != index.name(address)],
            "depth": depth,
            "shortest_path": {
                "root": {"address": f"0x{root_for[address]:x}",
                         "name": index.name(root_for[address])},
                "functions": [{"address": f"0x{item:x}", "name": index.name(item)}
                              for item in path_addresses],
            },
            "direct_callers": direct_callers,
            "direct_callers_total": len(all_direct_callers),
            "direct_callers_truncated": len(direct_callers) < len(all_direct_callers),
            "direct_callees": direct_callees,
            "direct_callees_total": len(all_direct_callees),
            "direct_callees_truncated": len(direct_callees) < len(all_direct_callees),
            "unresolved_named_callees": index.unindexed.get(address, []),
            "decompiler": {
                "success": bool(row.get("success")),
                "has_warning": bool(row.get("has_warning")),
                "pseudo_file": row.get("file"),
                "byte_offset": row.get("byte_offset"),
                "byte_length": row.get("byte_length"),
                "snippet": index.pseudo_snippet(address, snippet_chars),
            },
        }
        emitted.append(function)

    emitted.sort(key=lambda item: (item["depth"], item["name"], item["address"]))
    root_records = [{"address": f"0x{address:x}", "name": index.name(address)}
                    for address in root_addresses]
    unresolved_roots = [record for record in root_records
                        if not index.rows[int(record["address"], 16)].get("success")]
    return {
        "schema": "dh2-native-direct-call-slice/v1",
        "source": "Ghidra pseudocode function-index.jsonl and shard byte ranges",
        "roots": root_records,
        "max_depth": max_depth,
        "name_prefix_filter": prefixes,
        "reachable_unique_count": len(distances),
        "emitted_unique_count": len(emitted),
        "functions": emitted,
        "limits": [
            "Edges come from named direct calls found in generated pseudocode and resolved to indexed function starts.",
            "Indirect calls, vtable dispatch, function pointers, callbacks, Lua/dynamic dispatch, and runtime registrations are incomplete.",
            "External or unresolved call names, failed/omitted decompilations, and pseudocode errors can hide edges; shortest paths are hypotheses, not proof of runtime reachability.",
            "Function addresses and snippets are evidence references, not a claim that the original function is understood or reconstructed.",
        ],
        "root_decompiler_failures": unresolved_roots,
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--entry", action="append", required=True,
                        help="root ELF address (hex, with or without 0x), mangled Ghidra name, or exact demangled symbol; repeatable")
    parser.add_argument("--max-depth", type=int, default=2,
                        help="maximum number of direct indexed-call edges from any root (default: 2)")
    parser.add_argument("--name-prefix", action="append", default=[],
                        help="emit only functions whose indexed or demangled name starts with this prefix; traversal still crosses other names; repeatable")
    parser.add_argument("--snippet-chars", type=int, default=360,
                        help="maximum pseudo-callsite snippet chars per emitted function; use 0 to omit (default: 360)")
    parser.add_argument("--edge-limit", type=int, default=16,
                        help="maximum listed direct callers/callees per function; use 0 for all (default: 16; totals and truncation flags are always included)")
    parser.add_argument("--repo-root", type=Path, default=Path(__file__).resolve().parents[1],
                        help="repository root (default: parent of tools/)")
    parser.add_argument("--decomp-dir", type=Path,
                        help="override the libDungeonHunter2.so decompiler evidence directory")
    parser.add_argument("--symbols-csv", type=Path,
                        help="override the original ELF function-index.csv used for demangled root aliases")
    parser.add_argument("--output", "-o", type=Path,
                        help="write JSON to this path instead of stdout")
    parser.add_argument("--pretty", action="store_true",
                        help="indent JSON for review; default output is compact")
    args = parser.parse_args(argv)

    repo_root = args.repo_root.resolve()
    decomp_dir = (args.decomp_dir or
                  repo_root / "recovered" / "native" / "decompiled" / "libDungeonHunter2.so").resolve()
    symbols_csv = args.symbols_csv
    if symbols_csv is None:
        candidate = repo_root / "recovered" / "native" / "symbols" / "libDungeonHunter2.so" / "function-index.csv"
        symbols_csv = candidate if candidate.is_file() else None
    if not (decomp_dir / "function-index.jsonl").is_file():
        parser.error(f"missing decompiler index: {decomp_dir / 'function-index.jsonl'}")
    if args.snippet_chars < 0 or args.edge_limit < 0:
        parser.error("--snippet-chars and --edge-limit must be nonnegative")

    try:
        result = analyze_slice(decomp_dir, args.entry, args.max_depth,
                               args.name_prefix, symbols_csv, args.snippet_chars,
                               args.edge_limit)
    except (OSError, ValueError, KeyError, json.JSONDecodeError) as exc:
        print(f"native_call_slice: {exc}", file=sys.stderr)
        return 2

    rendered = json.dumps(result, ensure_ascii=False,
                          indent=2 if args.pretty else None,
                          separators=None if args.pretty else (",", ":"))
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(rendered + "\n", encoding="utf-8")
    else:
        print(rendered)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
