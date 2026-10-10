"""Read-only original IDA camera/viewport evidence capture."""
import json
import struct
import urllib.request
from pathlib import Path

root = Path(__file__).resolve().parent
for address in ("0x583734", "0x582084", "0x6e5550", "0x428f38", "0x428b74", "0x58364c", "0x6e5238", "0x5b0a9c", "0x6dc5a8"):
    body = {"jsonrpc": "2.0", "id": 1, "method": "tools/call", "params": {
        "name": "decompile", "arguments": {"addr": address, "max_lines": 5000,
        "include_addresses": True, "database": "dh2-libdungeonhunter2"}}}
    request = urllib.request.Request("http://127.0.0.1:8746/mcp", json.dumps(body).encode(),
        headers={"Content-Type": "application/json", "Accept": "application/json,text/event-stream"})
    with urllib.request.urlopen(request, timeout=60) as response:
        raw = response.read().decode()
    if raw.startswith(("event:", "data:")):
        raw = next(line[6:] for line in raw.splitlines() if line.startswith("data: "))
    result = json.loads(raw)
    if result.get("error") or result.get("result", {}).get("isError"):
        raise RuntimeError(result)
    destination = root / ("live-ida-camera-" + address[2:] + ".json")
    destination.write_text(json.dumps(result, indent=2), encoding="utf-8")
    print(address, destination.name)
print("Ctor FOV bits", hex(1067506044), struct.unpack("<f", struct.pack("<I", 1067506044))[0])
print("Ctor aspect bits", hex(1068149419), struct.unpack("<f", struct.pack("<I", 1068149419))[0])
