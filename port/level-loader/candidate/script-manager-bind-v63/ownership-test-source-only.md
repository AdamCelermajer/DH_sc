# V63 ownership companion test (source only)

The unique script_manager_bind_v63_ownership_test.cpp accompanies the existing frozen V63 binder. No new binding version or production change was made, and no genuine binder defect was found by source inspection. No compiler/runtime/build/WSL/oracle/device job has run for this test.

The future native test checks rejection of an existing command factory without invoking it; rejection of direct App ownership and an alias into App's control block as services.owner; unchanged output services across those failures; required Execute/expired App rejection; successful independent cache wiring without cache or engine calls during binding; typed Execute argument/result forwarding through an explicitly failing fixture; and App destruction despite retained composed services and an actual V59 command. It also checks that the independent cache remains alive only until its retained services owner is released.

All Main/stream owners and Execute behavior here are explicit transport fixtures. The genuine native V59 receiver and V52 generated-data storage operate on a tiny synthetic kind1 record, with the source-proven base455628 BX LR Init. The test does not provide successful Main engine effects or evidence of level readiness. It has not run; its future PASS output is not a current receipt.

Exact dependency includes for ONE future parent-admitted NDK syntax leaf:

- Direct: script_manager_bind_v63.hpp (same candidate directory).
- Transitive: script_command_init_v62.hpp, then script_command_receivers_v59.hpp, then script_manager_owner_v52.hpp.
- Header search roots: port/level-loader, candidate/script-manager-bind-v63, candidate/command-init-v62, candidate/command-c1-v59.
- Syntax input only: candidate/script-manager-bind-v63/script_manager_bind_v63_ownership_test.cpp, with C++17 and -fsyntax-only under the parent's admitted arm64 NDK setup. No production CPP or archived libraries need to be compiled/linked for this syntax leaf.
- A future actual native runtime check would additionally link script_manager_owner_v52.cpp and candidate/command-c1-v59/script_command_receivers_v59.cpp; that requires separate permission/admission and is not requested or launched here.

Existing V63 header/archive hashes are checked unchanged in the companion JSON receipt. The original frozen V63 manifest/archive remain unchanged; this companion source is recorded separately.
