"""Build/run the authored Swamp quest and GameEvent route on the host only."""
from pathlib import Path
import os
import subprocess
import zipfile

root = Path(__file__).resolve().parents[3]
output = root / ".local-inputs" / "swamp-quest-event-route-v1-host"
output.mkdir(parents=True, exist_ok=True)

def host_path(path):
    path = path.resolve()
    if os.name == "nt":
        return "/mnt/" + path.drive[0].lower() + path.as_posix()[2:]
    return str(path)

original = root / "port/android-native/app/build/generated/dh2-original-cache/assets/dh2-original-cache.zip"
event_prefix = "com.gameloft.android.GAND.GloftD2SS/files/data/pydata/"
with zipfile.ZipFile(original) as archive:
    for entry, name in (("v2eventmanager_pyarray.bin", "event-records.bin"),
                        ("v2eventmanager_pyarraynames.bin", "event-names.bin")):
        (output / name).write_bytes(archive.read(event_prefix + entry))

quest_cache = root / "port/level-world/reference/character-menu-profile-v51/cache"
quest_records = quest_cache / "v2quests_pyarray.bin"
quest_names = quest_cache / "v2quests_pyarraynames.bin"
sources = [
    root / "port/level-loader/tests/swamp_quest_event_route_v1.cpp",
    root / "port/level-loader/game_event_runtime_v75.cpp",
    root / "port/level-loader/game_event_manager_v50.cpp",
    root / "port/level-world/event_manager_owner_v12.cpp",
    root / "port/game-data/quest_persistence_v51.cpp",
    root / "port/game-data/quest_savegame_v1.cpp",
]
executable = output / "swamp-quest-event-route-v1"
command = ["g++", "-std=c++17", "-O1", "-g", "-Wall", "-Wextra", "-Werror",
           "-Wno-misleading-indentation", "-Wno-missing-field-initializers",
           "-ffunction-sections", "-fdata-sections", "-fsanitize=address,undefined",
           "-fno-omit-frame-pointer"]
command += ["-I" + host_path(p) for p in (root / "port/level-loader",
           root / "port/level-world", root / "port/game-data")]
command += [host_path(p) for p in sources]
command += ["-Wl,--gc-sections", "-o", host_path(executable)]
prefix = ["wsl.exe", "--exec"] if os.name == "nt" else []
build = subprocess.run(prefix + command, text=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, timeout=120)
(output / "compile.log").write_text(build.stdout)
if build.returncode:
    print(build.stdout, end="")
    raise SystemExit(build.returncode)
args = [host_path(executable), host_path(output / "event-records.bin"),
        host_path(output / "event-names.bin"), host_path(quest_records), host_path(quest_names)]
run = subprocess.run(prefix + args, text=True, stdout=subprocess.PIPE,
                     stderr=subprocess.STDOUT, timeout=30)
(output / "run.log").write_text(run.stdout)
print(run.stdout, end="")
raise SystemExit(run.returncode)
