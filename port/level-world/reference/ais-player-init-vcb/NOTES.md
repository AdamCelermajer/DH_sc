# AISPlayer callback flags

Adapted AdamCelermajer/DH_sc c3ae797332a82a30a586b9156cddc25445e36a4c `character_script_player_vcb_v2.cpp`; original AISPlayer3dd884/60B. Reuses our Default3dc7d8/92B, then captures b8 before fresh OnKill query and writes bit400. Player/IPhone vtables both slotcc select this caller. IsInVFTable37c2a0/116B executes in ARM proof; only hashString is modeled. Typed control guards/provider errors are port boundaries; no whole VM/actor/skill/native claim and zero newly reconstructed bodies.

Run `python port/level-world/tests/run_ais_player_init_vcb_host.py --compiler <g++> --original-elf <ELF> --output <short-host-build-directory> --report <ignored-validation.json>`. Wrapper builds actual selected CMake world library and sole shared VM, sequentially; short output avoids Windows MAX_PATH. Unchanged commons plus explicit registration fixtures verify Lua globals do not imply VFTable membership. No OnInit replay, second actor store, timer store or VM.
