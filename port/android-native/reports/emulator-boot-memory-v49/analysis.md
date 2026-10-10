# Guarded emulator boot memory diagnosis, 2026-10-08

The repeated stop is a lack of physical RAM headroom at admission. It is not a failed Windows commit reservation, and the receipts do not establish a DH_sc application leak. No emulator or ADB process was launched by this investigation. No launcher, watchdog, AVD, pagefile, or running-process configuration was changed.

## Direct evidence

The latest manifest is `.local-inputs/emulator-guards-v36/20261008-212054-b2efd9d5.manifest.json`. Its fresh admission snapshot contains 7,565,479,936 available bytes (7.05 GiB), 31.83 GiB physical RAM total, and 45.44 GiB unused commit capacity. This differs from the earlier 10.67 GiB free-RAM observation supplied with the task. `--allow-physical-pressure` permitted admission despite the normal startup RAM requirement.

The command requested `-memory 3072`, but the emulator log says `Increasing RAM size to 4096MB`. The generated `C:/Users/adamc/.android/avd/Medium_Phone.avd/hardware-qemu.ini` confirms `hw.ramSize = 4096`, `hw.cpu.ncore = 2`, `hw.gpu.mode = swiftshader`, and `vm.heapSize = 576`. Thus the attempted guest-RAM reduction did not take effect. The AVD registration `Medium_Phone_API_37.0.ini` points to `Medium_Phone.avd`, not a directory named after the registration.

The latest log reports `Boot completed in 24364 ms`. This proves that the emulator logged Android boot completion before the guard stop; it does not prove the main chat's external ADB readiness check passed. The 20:38 attempt similarly logged boot completion at 21.083 seconds before its later RAM stop.

The latest telemetry has an owned-job private-memory increase from 6.01 GiB at 32.98 seconds to 9.74 GiB at 34.02 seconds. Total system commit increased from 54.14 to 58.23 GiB during that interval. Most of that commit increase is accounted for by the owned emulator job. Host available RAM was 2.04 GiB at the last full census, then 644,538,368 bytes (0.600 GiB) at the fast sample at 34.484 seconds, corresponding to 98.114% physical RAM used. The verified guard terminated its owned job successfully. The latest complete census still had 35.61 GiB commit headroom. Neither the 14 GiB soft private cap nor the 15 GiB OS process-tree cap was the trigger.

| Run | Backend | Fresh free RAM | Android boot log | Outcome | Whole-run peak private / working set |
| --- | --- | ---: | ---: | --- | ---: |
| 20261006-135603-dda32915 | host, Vulkan disabled | 18.61 GiB | 17.540 s | Earlier 5 GiB soft cap stopped job at 23.27 s | 5.51 / 3.84 GiB |
| 20261008-122645-af80cf61 | host, Vulkan enabled | 13.14 GiB | 19.889 s | Physical floor stopped job at 558.95 s | 9.91 / 8.84 GiB |
| 20261008-200728-d4878cd3 | SwiftShader | 12.61 GiB | 22.380 s | Job completed normally after 287.34 s | 10.32 / 9.65 GiB |
| 20261008-203817-88412c2f | SwiftShader | 8.32 GiB | 21.083 s | 98% stop at 62.39 s | 9.17 / 7.74 GiB |
| 20261008-212054-b2efd9d5 | SwiftShader, requested 3 GiB guest | 7.05 GiB | 24.364 s | 98% stop at 34.50 s | 9.74 / 5.51 GiB |

Peaks include the owned process tree, not solely one QEMU PID. The fast 98% samples do not include a new process census, so their resident-memory attribution cannot be derived exactly from the last full sample. The short October 6 GLES-only run confirms the configuration can reach boot but does not establish its steady-state memory use.

## Cause and limits

- Shared host load is materially consuming RAM before launch. During this investigation, process-group working sets were approximately Codex 4.06 GiB, ChatGPT 3.99 GiB, WSL 3.04 GiB, Node 1.69 GiB, and Java 1.18 GiB. These are current observations, not a complete per-worker history; shared pages can overlap. No unrelated workload was stopped.
- SwiftShader is doing GLES and Vulkan software rendering. The installed NVIDIA RTX 4070 SUPER already reached boot with `--gpu host`, including a prior Vulkan-disabled attempt. A renderer contribution is plausible, and a guarded GLES-only hardware attempt is supported by existing evidence, but the receipts do not identify the allocation responsible for every GiB or prove a graphics leak.
- Lowering only `--guest-memory-mib` is ineffective here because emulator 37.2.12 raises the guest to 4 GiB. The main chat should use 4096 explicitly and read back the generated hardware settings after launch. No low-RAM image-mode workaround is needed for the recommended attempt.
- The pagefile is already system managed, allocated at 63,488 MiB (62 GiB), with roughly 6 GiB used in the current observation. Commit is ample. A larger pagefile would not remove the mandatory physical-RAM stop. `--allow-guest-paging` changes admission policy in the launcher; it does not force Windows to keep guest storage out of physical RAM.

Official sources: [graphics backend choices](https://developer.android.com/studio/run/emulator-acceleration) and [Vulkan disable flag / Windows commit guidance](https://developer.android.com/studio/run/emulator-troubleshooting). These support `-gpu host` and `-feature -Vulkan`; memory reduction remains a measured hypothesis until the next guarded run.

## Recommended next owned attempt

Avoid another admission with only 7–8 GiB free. Wait for build/worker demand to settle; the main chat can coordinate its owned work without terminating unrelated applications. At least 12 GiB must be available in the launcher's own fresh preflight, with 16 GiB preferred.

Use existing launcher options: `--avd Medium_Phone_API_37.0 --port 5554 --adb-port 5037 --gpu host --disable-vulkan --guest-memory-mib 4096 --hard-memory-gib 15 --soft-memory-gib 14 --duration-seconds 3600 --allow-guest-paging --min-physical-available-gib 2`. Omit `--allow-physical-pressure`.

That combination requires 12 GiB available at admission (10 GiB observed startup allowance plus 2 GiB physical floor), keeps a runtime 2 GiB floor, retains the unchanged fast 98% emergency stop, and preserves all process-tree/commit/lifetime limits. With `--min-physical-available-gib 6` instead, admission requires the preferred 16 GiB and the runtime floor is stronger.

The first milestone is external ADB `device` status and `sys.boot_completed=1` before app install/launch. Record the new manifest, emulator log, and telemetry. If the host has insufficient fresh admission RAM, the safe result is refusal; do not repeat a pressure-mode attempt with the same low headroom. This investigation did not run this recommendation.
