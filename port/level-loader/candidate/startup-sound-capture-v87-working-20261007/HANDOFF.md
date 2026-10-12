# Actual nullable startup SoundManager capture

Source-only: no Root writes or validation jobs. New world header plus narrow
paired Door/Trigger headers/bodies and existing catalog-member forwarding.
Apply startup-sound-capture-v87.patch to current source and add
source_startup_sound_v87.hpp to existing world include owner; no new TU.

Door now retains one actual nullable manager snapshot through BOTH rawUID sound
loads, including after first callback rereads its current table/index. Trigger
uses the same captured manager for sound10 and still reaches external Lua when
actual managerNULL. Bool presence + independent load callbacks could not
express this source owner with the dynamic catalog lender. The typed receipt
fixes that lifetime directly, without a cache/registry/new manager.

renderer_startup_sound_capture_v87.hpp binds existing process
borrow_application_audio_v42 and actual precache_raw_uid. Include with current
audio application include directory, then invoke
bind_startup_sound_capture_v87(weak,output.startup) for Door/Trigger incoming
bags. Genuine supplied providers are preserved. Strong snapshot stays local to
the original method; callback captures weak campaign and manager only.

Root function/signature source remains authoritative; do not overwrite newer
class/catalog changes with whole proposal copies. V86 idle signature changes
are separate paired dependency. Main functional audio/playback/body behavior
remains existing engine ownership. Integrated runtime acceptance stays open.
