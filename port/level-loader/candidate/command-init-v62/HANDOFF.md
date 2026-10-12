# V62 exact live-read Init successor

SOURCE_ONLY_UNCOMPILED. V62 template/test code has not been compiled or run. This worker launched no compiler, WSL, sanitizer, oracle, or device. Parent's separate V59 production CPP arm64 syntax check is independent: it does not instantiate V62 or prove engine effects. V59 and V61 files/old manifests are unchanged.

Two concrete V61 API defects are corrected:

1. Show/Hide source calls MenuManager::GetInstance at459844/4597f0 BEFORE loading command.data_c at459848/4597f4. V62 reborrows current checked actual receiver/header/Data immediately after this required Main leaf, then reads and caches its CString10 for strstr(name,"HUD") and any GetMenuByName call. A valid header change inside GetInstance affects the source name/branch. A foreign pointer without SAME retained native lease fails at this explicit safety guard. V59's post-body identity check still rejects changed assignments AFTER the reached source prefix; V62 does not force success.
2. Effect source reads the current VisualFXManager singleton at4598c8 for each Init. V61 retained a weak bind-time snapshot. V62 requires current_fx_singleton(ActualInitBorrowV62<FX>&,error), a typed Main projection over its authoritative current singleton, after Data+8 is read. A missing/foreign/NULL provider fails. It never constructs a manager or uses a stale still-live one.

Exact original live/cached load order from retained ASM:

|Body|Original loads and calls|V62 rule|
|---|---|---|
|StartDialog|459048 cache command.data_c inr12;459054 actual App.constants2c;459060 cache Data+c scalar inr5;459068 getConstant;45906c compare cached scalar;459078 write SAME skip4|Cache original Data pointer and pre-lookup scalar. Lookup mutation must not change the comparison value. No fresh header/Data/scalar reload after lookup.|
|Show/Hide|GetInstance -> current command.data_c -> Data CString10 -> strstr -> source branch -> optional GetMenuByName -> SAME receiver menu10 write|Fresh checked Data header read only after GetInstance, then cache that CString through lookup. Native-width weak Menu alias transport remains unchanged.|
|PlayEffect|4598b4 cache command.data_c inr2;4598bc cache Data+8 inr1;4598c8 current FXsingleton inr0;4598cc tail RegisterFXSetToLoad|Read/cache signed32 ID before current-singleton projection. Projection mutation of Data+8 must not change the argument. Actual FX registration remains required Main body.|
|PlayActorAnim|45a2f8 cache command.data_c inr5;45a30c App.ObjectManager38;45a310 Data CString18; actual handle lookup/GetObject(false)/Character conversion;45a358/35c cache SAME Character.Animator49c inr4;45a360 load cachedData+8;45a368 first Add;45a36c reuse SAMEr4;45a370 freshly load cachedData+c;45a374 second Add|Keep original Data owner and SAME Character/Animator pin across Main leaves. Freshly read only Data+c from that cached owner after first Add. Do not re-resolve Character/Animator or replace cachedData after header mutation. Both registrations remain required Main bodies.|

V59 native safety semantics remain explicit: scoped pins keep cached modern objects alive across callbacks; source failed prefixes remain retained; SAME-kind/Data/header post-check occurs after completed Init body; failed Init is not replayed. A changed Actor header inside firstAdd therefore still reaches secondAdd on old cachedData+c, then fails the V59 post-body guard. A changed Menu header inside GetInstance reaches the branch from newly borrowedData, then fails that same post-body guard. These guards are native additions, not claims that original ARM returned bool or enforced pointer ownership.

Main integration: use existing concrete Types and real leaves from V61 contract, but replace actual_fx_singleton weak snapshot with REQUIRED current_fx_singleton typed projection. Keep App authority weak and callbacks weakly captured; returned alias owners must address SAME actual objects. Bind script_init_behavior_v62 into canonical V59 factories only where no genuine existing Init implementation must be retained. Execute remains absent. The source order reconstructs Main leaf calls only; dialogue/FX/animation/quest execution bodies are Main-owned.

Source-only tests retain V61's explicit synthetic transport fixtures and add callback mutations: cached StartDialog scalar, switched still-live FX singleton and ID mutation, missing current singleton provider, Show/Hide header replacement changing HUD/name branch, fresh second dictionary scalar without actor/animator relookup, and Actor header replacement retaining both leaf prefixes before V59 failure. Test-only mutation writes the actual nonconst synthetic Data scalar cell, not a copied record. None of these tests is real Main functional verification and none has run.

Private reproduction for a future admitted parent lane, serial only:

    g++ -std=c++17 -O0 -g -Wall -Wextra -I. -Icandidate/command-c1-v59 -Icandidate/command-init-v62 candidate/command-c1-v59/script_command_receivers_v59.cpp script_manager_owner_v52.cpp candidate/command-init-v62/script_command_init_v62_test.cpp -o candidate/command-init-v62/native-test
    candidate/command-init-v62/native-test

Use private port/level-loader. No compiler/WSL launch is authorized by this document. Any actual lane still requires parent's current Main admission.
