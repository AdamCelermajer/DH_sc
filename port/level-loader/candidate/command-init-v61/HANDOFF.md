# V61 five original Init order adapters

SOURCE_ONLY_UNCOMPILED. No compiler, WSL, sanitizer, oracle or device launch. V61 implements the source order around required Main leaves; it does not implement Main engine behavior or source Execute.

`script_command_init_v61.hpp` is a header-only template. Main supplies a Types trait naming its existing concrete Application, Constants, MenuManager, Menu, FXManager, ObjectManager, ObjectHandle, Object, Character and Animator types. Supply `ScriptInitLeavesV61<Types>` with weak references to the authoritative App and existing FX singleton, and real typed callbacks only. Each callback bool means its required body completed/transport succeeded; source Init itself is void. No boolean return register is fabricated. Missing reached callbacks and invalid/expired/foreign actual borrows fail.

Each typed borrow must point to the SAME actual native object as its alias shared owner (.get()==actual). Use existing owner leases or aliasing leases to actual embedded members; the loader allocates no Main manager/object/registry. Callback closures must capture App weakly. Main owns the actual temporary ObjectHandle returned from GetObjectByName and supplies a lease valid for the remainder of this Init; never return a dangling callback-local stack handle or an opcode DTO.

Bind `script_init_behavior_v61(leaves)` into the V59 factories only if Main has no existing Init implementation to retain. V59 executes its proven base BX LR directly for41 classes and calls this adapter only for the five nontrivial bodies. Adapter dispatch rechecks SAME canonical receiver/header/Data identity and concrete class. Leave resulting `.execute` absent until Main binds genuine functional engine execution. Do not install a second App/global/VM/registry.

|Kind|Reconstructed order|Required Main engine bodies|
|---:|---|---|
|10|actual App.constants2c -> SAME Data+c -> getConstant(DialogStyles,EnterLocationDialog) -> SAME skip4 exactly0/1|Actual PyDataConstants lookup|
|22/23|actual MenuManager.GetInstance -> SAME Data CString10 -> strstr(name,HUD); substring present -> SAME menu10 NULL; absent -> GetMenuByName(original name) -> SAME native-width menu10 alias|Actual instance and actual menu lookup; genuine NULL is allowed|
|20|SAME Data+8 signed32 -> existing FX singleton -> RegisterFXSetToLoad|Actual FX registration mutation; REQUIRED Main leaf|
|45|actual App.ObjectManager38 -> SAME Data CString18 -> GetObjectByName(name,-1,false,NULL) -> handle.GetObject(false) -> actual Character conversion -> SAME embedded Animator49c -> AddAnimDict(Data+8) -> AddAnimDict(Data+c)|All actual handle/actor/animator hooks; both dictionary registrations mutate Main and remain REQUIRED leaves|

The Actor path returns normally on genuine NULL GetObject or genuine NULL Character conversion, preserving the source early return and avoiding later unreached leaves. It requires an actual temporary handle even for an empty result. Embedded Animator transport validates the SAME parent Character pointer and SAME owner control block. It never computes a native Character+ARM49c address or fills opaque command+10/+14/+18. PlayEffect/PlayActorAnim functional registration leaves are intentionally explicit; loader reconstructs their call order only.

Menu ownership uses V59's two source-proven actual native uintptr_t menu10 cells and weak Menu alias leases, preserving64-bit addresses. Source substring matching is case-sensitive `strstr`, not equality. Other unproven extras remain opaque. Successful init is subject to V59 SAME-Data post-check; failed body prefixes are retained and Init is not replayed.

`script_command_init_v61_test.cpp` contains explicit synthetic Data/order/engine-leaf fixtures, not real engine effects. Source-only cases cover skip equality/inequality, HUD embedded substring and lowercase branch, real lookup NULL, foreign menu alias, required stops/replay refusal, signed FX ID, genuine actor/Character NULL early returns, exact two-dictionary order/failed second-prefix, foreign embedded animator, weak App expiry and foreign copied borrow. The Execute hook remains absent. Test JSON labels Main engine leaves as fixtures and FX/animation/whole-level verification false. None of these tests has run.

Private serial reproduction after parent resource admission:

    g++ -std=c++17 -O0 -g -Wall -Wextra -I. -Icandidate/command-c1-v59 -Icandidate/command-init-v61 candidate/command-c1-v59/script_command_receivers_v59.cpp script_manager_owner_v52.cpp candidate/command-init-v61/script_command_init_v61_test.cpp -o candidate/command-init-v61/native-test
    candidate/command-init-v61/native-test

Run from private port/level-loader using the admitted WSLUbuntu GCC15. No CMake edits required. Separate sanitizer compilation/run needs renewed parent admission after normal check.

Source provenance: exact frozen reference/scriptmanager-v52/command-init-459034.asm,4597e8.asm,45983c.asm,4598b4.asm,45a2ec.asm, schema offsets, and V59 MAIN-INIT-HOOKS.md. V61 does not expand the46 observed class coverage into unknown generic pair kinds; V60's37 unsampled local pairs remain unknown. Existing root/vendor/selected/V52/V59 files are unchanged.
