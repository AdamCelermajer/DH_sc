# RAM incident, 7 October 2026

Cause identified in lane01's runtime record: two ad hoc Python inspections of
skills_pyarray.bin started at13:23:11Z and13:23:26Z (16:23 Jerusalem). They used
the incorrect fixed SkillList descriptor, read the following nested count as
1946157056 at0x12d, and materialized `[self.row(sub) for _ in range(n)]` without
count or remaining-byte validation. Their byte reader returned0 beyond EOF.
The tool returned running handles, but the worker printed only output and
discarded those handles; both jobs continued. A later bounded inspection
reported the same invalid count immediately. The user closed Python.

The emulator watchdog recorded only4173824 physical bytes free at13:27:42Z.
At13:27:43Z it terminated its own emulator job for system_commit_headroom.
The emulator's private allocation was about10.0GiB at that point; its resident
set had already been paged down. It was not the sole source of host pressure.
The reused launcher allowed physical pressure, so its15GiB emulator limit did
not protect against independent runaway Python helpers.

Containment: lane01 interrupted; no analysis Python/emulator/compiler processes
remain in the current process census. Unrelated existing Python servers were
left running. Current physical free memory recovered to21.73GiB of31.83GiB.
All source changes and existing saves remain intact. Latest installed V124 is
startup-blocked; later schema corrections are source-only, pending build.

Execution rules: no worker runs ad hoc binary-decoder Python. Workers provide
source/IDA evidence; any required input decoder is integrator-owned, validates
all counts/lengths against remaining bytes before iteration, streams/discards
records, uses an aggregate work/allocation bound and an OS memory cap. Running
tool sessions must be retained and waited/terminated; never discard a handle.
Builds and emulator runs are serialized, and physical-pressure override is
disabled in the canonical root launcher. No automatic emulator relaunch for
this incident response.
