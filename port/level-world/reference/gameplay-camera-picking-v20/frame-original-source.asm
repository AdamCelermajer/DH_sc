_ZN5Level6UpdateEb
003f82d8 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f82dc ldr      r4, [pc, #0x8e0]
003f82e0 ldr      r6, [pc, #0x8e0]
003f82e4 mov      r5, r0
003f82e8 add      r4, pc, r4
003f82ec ldr      r3, [r4, r6]
003f82f0 ldr      r0, [pc, #0x8d4]
003f82f4 sub      sp, sp, #0x124
003f82f8 ldr      r3, [r3]
003f82fc add      r0, pc, r0
003f8300 mov      r7, r1
003f8304 str      r3, [sp, #0x11c]
003f8308 bl       #0x3136b4 ; _Z20PushProfilingContextPKc
003f830c ldr      r3, [r5, #0x130]
003f8310 cmp      r3, #0x26
003f8314 beq      #0x3f8350
003f8318 cmp      r7, #0
003f831c bne      #0x3f8350
003f8320 mov      r0, r5
003f8324 bl       #0x3f6990 ; _ZN5Level12_LoadProcessEv
003f8328 ldr      r0, [pc, #0x8a0]
003f832c add      r0, pc, r0
003f8330 bl       #0x3136b8 ; _Z19PopProfilingContextPKc
003f8334 ldr      r3, [r4, r6]
003f8338 ldr      r2, [sp, #0x11c]
003f833c ldr      r3, [r3]
003f8340 cmp      r2, r3
003f8344 bne      #0x3f8bc0
003f8348 add      sp, sp, #0x124
003f834c pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003f8350 bl       #0x42a4fc ; _ZN12MenuDebugHUD11GetInstanceEv
003f8354 ldr      r3, [r0, #0xd8]
003f8358 mov      r7, r0
003f835c cmp      r3, #0
003f8360 bne      #0x3f83ac
003f8364 ldr      sl, [pc, #0x868]
003f8368 add      r7, sp, #0x104
003f836c ldr      r8, [r4, sl]
003f8370 mov      r0, r8
003f8374 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003f8378 ldr      r1, [pc, #0x858]
003f837c add      r2, sp, #0x70
003f8380 mov      r0, r7
003f8384 add      r1, pc, r1
003f8388 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f838c mov      r0, r8
003f8390 mov      r1, r7
003f8394 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003f8398 cmp      r0, #0
003f839c beq      #0x3f83d4
003f83a0 mov      r0, r7
003f83a4 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f83a8 b        #0x3f8328
003f83ac add      r8, r7, #0xc8
003f83b0 mov      r0, r8
003f83b4 ldr      r1, [r7, #0xcc]
003f83b8 bl       #0x3f1b40 ; _ZNSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
003f83bc mov      r3, #0
003f83c0 str      r3, [r7, #0xd8]
003f83c4 str      r8, [r7, #0xd4]
003f83c8 str      r8, [r7, #0xd0]
003f83cc str      r3, [r7, #0xcc]
003f83d0 b        #0x3f8364
003f83d4 mov      r0, r8
003f83d8 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003f83dc ldr      r1, [pc, #0x7f8]
003f83e0 add      sb, sp, #0xec
003f83e4 add      r2, sp, #0x6c
003f83e8 add      r1, pc, r1
003f83ec mov      r0, sb
003f83f0 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f83f4 mov      r1, sb
003f83f8 mov      r0, r8
003f83fc bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003f8400 mov      fp, r0
003f8404 mov      r0, sb
003f8408 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f840c mov      r0, r7
003f8410 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f8414 cmp      fp, #0
003f8418 bne      #0x3f8328
003f841c ldr      r3, [r5, #0x148]
003f8420 cmn      r3, #1
003f8424 bne      #0x3f883c
003f8428 ldr      r8, [pc, #0x7b0]
003f842c ldr      r3, [r4, r8]
003f8430 mov      r1, #0
003f8434 mov      r2, #1
003f8438 ldr      r0, [r3, #0x40]
003f843c bl       #0x36e478 ; _ZN13PlayerManager14GetLocalPlayerEib
003f8440 ldr      r0, [r0, #0x660]
003f8444 cmp      r0, #0
003f8448 beq      #0x3f8454
003f844c mov      r1, #0
003f8450 bl       #0x3bc480 ; _ZN9Character9SG_UpdateEb
003f8454 ldrb     r3, [r5, #0x144]
003f8458 cmp      r3, #0
003f845c beq      #0x3f899c
003f8460 ldr      r3, [r4, r8]
003f8464 ldr      r3, [r3, #0x40]
003f8468 ldr      r3, [r3, #0x714]
003f846c cmp      r3, #0
003f8470 beq      #0x3f898c
003f8474 ldr      r0, [r5, #0x194]
003f8478 bl       #0x4798c8 ; _ZN16GameEventManager6UpdateEv
003f847c ldr      r2, [r4, r8]
003f8480 ldr      r3, [r5, #0x38]
003f8484 ldr      r2, [r2, #0x10]
003f8488 cmp      r3, #0
003f848c ldr      r7, [r2, #0x1c]
003f8490 beq      #0x3f89c8
003f8494 ldr      ip, [r3, #0x1cc]
003f8498 ldr      r2, [r3, #0x1d0]
003f849c ldr      r3, [r3, #0x1d4]
003f84a0 add      r1, sp, #0x30
003f84a4 mov      r0, r7
003f84a8 mov      sb, #0x3f800000
003f84ac str      ip, [sp, #0x30]
003f84b0 str      r2, [sp, #0x34]
003f84b4 str      r3, [sp, #0x38]
003f84b8 str      sb, [sp, #0x3c]
003f84bc bl       #0x589508 ; _ZN6glitch5scene13CSceneManager15setAmbientLightERKNS_5video7SColorfE
003f84c0 ldr      r7, [r4, r8]
003f84c4 ldr      r0, [r7, #0x44]
003f84c8 bl       #0x34bd08 ; _ZN13PhysicalWorld6updateEv
003f84cc bl       #0x3ea730 ; _ZN17SpawnGroupManager11GetInstanceEv
003f84d0 mov      r2, #0
003f84d4 mov      r3, #0
003f84d8 bl       #0x3e9238 ; _ZN17SpawnGroupManager6updateEd
003f84dc bl       #0x3d24f8 ; _ZN6CharAI12HandleGroupsEv
003f84e0 mov      r1, sb
003f84e4 ldr      r0, [r7, #0x38]
003f84e8 bl       #0x34a620 ; _ZN13ObjectManager6UpdateEf
003f84ec bl       #0x3ce9b8 ; _ZN6CharAI14IncUpdateQueueEv
003f84f0 mov      r0, r5
003f84f4 bl       #0x3f9870 ; _ZN5Level16UpdateCameraZoomEv
003f84f8 ldr      r3, [pc, #0x6e4]
003f84fc ldr      r0, [r4, r3]
003f8500 bl       #0x496594 ; _ZN15VisualFXManager6UpdateEv
003f8504 ldr      r3, [r5, #0x12c]
003f8508 cmp      r3, #0
003f850c beq      #0x3f8524
003f8510 ldr      r2, [pc, #0x6d0]
003f8514 ldr      r2, [r4, r2]
003f8518 ldr      r2, [r2]
003f851c cmp      r3, r2
003f8520 beq      #0x3f8bac
003f8524 ldr      r3, [r5, #0x128]
003f8528 mov      r0, r3
003f852c ldr      r3, [r3]
003f8530 mov      lr, pc
003f8534 ldr      pc, [r3, #0x10]
003f8538 ldr      r3, [r5, #0x128]
003f853c mov      r1, #0
003f8540 ldr      r3, [r3, #8]
003f8544 mov      r0, r3
003f8548 ldr      r3, [r3]
003f854c mov      lr, pc
003f8550 ldr      pc, [r3, #0xb8]
003f8554 ldr      r3, [r5, #0x128]
003f8558 add      r0, sp, #0x4c
003f855c ldr      r1, [r3, #8]
003f8560 bl       #0x597180 ; _ZNK6glitch5scene10ISceneNode19getAbsolutePositionEv
003f8564 ldr      r3, [r5, #0x128]
003f8568 ldr      r0, [r3, #8]
003f856c bl       #0x597290 ; _ZNK6glitch5scene10ISceneNode9getParentEv
003f8570 mov      r1, r0
003f8574 add      r0, sp, #0x40
003f8578 bl       #0x597180 ; _ZNK6glitch5scene10ISceneNode19getAbsolutePositionEv
003f857c ldr      r1, [sp, #0x48]
003f8580 ldr      r0, [sp, #0x54]
003f8584 bl       #0x30e3ac
003f8588 ldr      r1, [r5, #0x1a4]
003f858c bl       #0x30e3ac
003f8590 ldr      r3, [r4, r8]
003f8594 mov      fp, r0
003f8598 ldr      r1, [sp, #0x40]
003f859c ldr      r3, [r3, #0x10]
003f85a0 ldr      r0, [sp, #0x4c]
003f85a4 ldr      r7, [r3, #0x1c]
003f85a8 bl       #0x30e3ac
003f85ac ldr      r1, [r5, #0x19c]
003f85b0 bl       #0x30e3ac
003f85b4 ldr      r1, [r7, #0x458]
003f85b8 bl       #0x30ed6c
003f85bc ldr      r1, [sp, #0x44]
003f85c0 mov      sb, r0
003f85c4 ldr      r0, [sp, #0x50]
003f85c8 bl       #0x30e3ac
003f85cc ldr      r1, [r5, #0x1a0]
003f85d0 bl       #0x30e3ac
003f85d4 ldr      r1, [r7, #0x45c]
003f85d8 bl       #0x30ed6c
003f85dc ldr      r1, [r7, #0x460]
003f85e0 mov      r3, r0
003f85e4 mov      r0, fp
003f85e8 str      r3, [sp, #0x1c]
003f85ec bl       #0x30ed6c
003f85f0 mov      r1, sb
003f85f4 mov      r7, r0
003f85f8 mov      r0, sb
003f85fc bl       #0x30ed6c
003f8600 ldr      r3, [sp, #0x1c]
003f8604 mov      sb, r0
003f8608 mov      r1, r3
003f860c mov      r0, r3
003f8610 bl       #0x30ed6c
003f8614 mov      r1, r0
003f8618 mov      r0, sb
003f861c bl       #0x30eba4
003f8620 mov      r1, r7
003f8624 mov      sb, r0
003f8628 mov      r0, r7
003f862c bl       #0x30ed6c
003f8630 mov      r1, r0
003f8634 mov      r0, sb
003f8638 bl       #0x30eba4
003f863c bl       #0x30e8a4
003f8640 bl       #0x30e1c0
003f8644 bl       #0x30e6a0
003f8648 mov      r1, #0
003f864c mov      sb, r0
003f8650 mov      r0, fp
003f8654 bl       #0x30e70c
003f8658 ldr      r3, [r4, r8]
003f865c ldr      r7, [r5, #0x38]
003f8660 cmp      r0, #0
003f8664 ldr      r3, [r3, #0x10]
003f8668 addne    sb, sb, #0x80000000
003f866c cmp      r7, #0
003f8670 ldr      r3, [r3, #0x1c]
003f8674 str      r3, [sp, #0x20]
003f8678 beq      #0x3f8a20
003f867c ldr      r0, [r7, #0x1d8]
003f8680 bl       #0x30e964
003f8684 mov      r1, sb
003f8688 bl       #0x30eba4
003f868c mov      fp, r0
003f8690 ldr      r0, [r7, #0x1dc]
003f8694 bl       #0x30e964
003f8698 mov      r1, sb
003f869c bl       #0x30eba4
003f86a0 add      r3, r7, #0x1e0
003f86a4 mov      r2, r0
003f86a8 mov      r1, fp
003f86ac ldr      r0, [sp, #0x20]
003f86b0 bl       #0x3522f8 ; _ZN12SceneManager9UpdateFogEffRK7Point3DIfE
003f86b4 ldr      r7, [pc, #0x530]
003f86b8 ldr      fp, [r4, sl]
003f86bc add      sb, sp, #0xd4
003f86c0 add      r7, pc, r7
003f86c4 mov      r0, fp
003f86c8 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003f86cc add      r2, sp, #0x68
003f86d0 mov      r0, sb
003f86d4 mov      r1, r7
003f86d8 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f86dc mov      r1, sb
003f86e0 mov      r0, fp
003f86e4 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003f86e8 ldr      fp, [r4, r8]
003f86ec mov      r3, r0
003f86f0 mov      r0, sb
003f86f4 ldr      r2, [fp, #0x10]
003f86f8 ldr      r2, [r2, #0x1c]
003f86fc ldrb     sb, [r2, #0x430]
003f8700 str      r3, [sp, #0x1c]
003f8704 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f8708 ldr      r3, [sp, #0x1c]
003f870c cmp      r3, sb
003f8710 beq      #0x3f8b60
003f8714 ldr      fp, [r4, sl]
003f8718 ldr      r7, [pc, #0x4d0]
003f871c add      sb, sp, #0xa4
003f8720 mov      r0, fp
003f8724 add      r7, pc, r7
003f8728 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003f872c add      r2, sp, #0x60
003f8730 mov      r0, sb
003f8734 mov      r1, r7
003f8738 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f873c mov      r1, sb
003f8740 mov      r0, fp
003f8744 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003f8748 ldr      fp, [r4, r8]
003f874c mov      r3, r0
003f8750 mov      r0, sb
003f8754 ldr      r2, [fp, #0x10]
003f8758 ldr      r2, [r2, #0x1c]
003f875c ldrb     sb, [r2, #0x431]
003f8760 str      r3, [sp, #0x1c]
003f8764 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f8768 ldr      r3, [sp, #0x1c]
003f876c cmp      r3, sb
003f8770 beq      #0x3f8b14
003f8774 ldrb     r1, [r5, #0x144]
003f8778 cmp      r1, #0
003f877c bne      #0x3f87c8
003f8780 ldr      r3, [pc, #0x46c]
003f8784 ldr      r3, [r4, r3]
003f8788 ldr      r7, [r3]
003f878c ldrb     r3, [r7, #0x32]
003f8790 cmp      r3, #0
003f8794 beq      #0x3f89ac
003f8798 mov      r0, r7
003f879c bl       #0x36c014 ; _ZN15VoxSoundManager18SetInSafeZoneMusicEb
003f87a0 mov      ip, #0
003f87a4 ldr      r1, [r5, #0x124]
003f87a8 mov      r3, ip
003f87ac mov      r2, ip
003f87b0 mov      r0, r7
003f87b4 str      ip, [sp]
003f87b8 str      ip, [sp, #4]
003f87bc bl       #0x36b80c ; _ZN15VoxSoundManager4PlayEibiib
003f87c0 mov      r3, #1
003f87c4 strb     r3, [r7, #0x31]
003f87c8 mov      r3, #1
003f87cc strb     r3, [r5, #0x144]
003f87d0 mov      r0, r5
003f87d4 bl       #0x3f0ae0 ; _ZN5Level14UpdateListenerEv
003f87d8 mov      r0, r5
003f87dc bl       #0x3f16c0 ; _ZN5Level16UpdateDynamicFogEv
003f87e0 bl       #0x42a4fc ; _ZN12MenuDebugHUD11GetInstanceEv
003f87e4 ldr      sl, [r4, sl]
003f87e8 str      r0, [sp, #0x20]
003f87ec add      r7, sp, #0x74
003f87f0 mov      r0, sl
003f87f4 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003f87f8 ldr      r1, [pc, #0x3f8]
003f87fc add      r2, sp, #0x58
003f8800 mov      r0, r7
003f8804 add      r1, pc, r1
003f8808 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f880c mov      r0, sl
003f8810 mov      r1, r7
003f8814 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003f8818 mov      sl, r0
003f881c mov      r0, r7
003f8820 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f8824 cmp      sl, #0
003f8828 bne      #0x3f886c
003f882c ldr      r0, [pc, #0x3c8]
003f8830 add      r0, pc, r0
003f8834 bl       #0x3136b8 ; _Z19PopProfilingContextPKc
003f8838 b        #0x3f8334
003f883c ldr      r8, [pc, #0x39c]
003f8840 ldr      r0, [r4, r8]
003f8844 bl       #0x31f684 ; _ZNK11Application21IsCurrentlyInGameViewEv
003f8848 cmp      r0, #0
003f884c beq      #0x3f842c
003f8850 ldr      r2, [pc, #0x3a8]
003f8854 mov      r3, fp
003f8858 ldr      r1, [r5, #0x148]
003f885c ldr      r0, [r4, r2]
003f8860 mvn      r2, #0
003f8864 bl       #0x4605c0 ; _ZN13ScriptManager11StartScriptEiib
003f8868 b        #0x3f842c
003f886c ldr      sl, [pc, #0x390]
003f8870 ldr      r0, [sp, #0x20]
003f8874 ldr      r7, [pc, #0x38c]
003f8878 add      sl, pc, sl
003f887c ldr      r1, [sl]
003f8880 bl       #0x42a7b0 ; _ZN12MenuDebugHUD17DisplayHugeNumberEi
003f8884 ldr      r1, [sl]
003f8888 ldr      r0, [sp, #0x20]
003f888c add      r7, pc, r7
003f8890 cmp      r1, #0xf
003f8894 movle    r1, #0
003f8898 movgt    r1, #1
003f889c bl       #0x42a8c0 ; _ZN12MenuDebugHUD25SetDisplaySealOfFreshnessEb
003f88a0 ldr      r0, [r4, r8]
003f88a4 ldr      sl, [r7]
003f88a8 bl       #0x31f66c ; _ZN11Application5GetDtEv
003f88ac rsb      r0, r0, sl
003f88b0 cmp      r0, #0
003f88b4 str      r0, [r7]
003f88b8 bgt      #0x3f882c
003f88bc ldr      r3, [pc, #0x348]
003f88c0 movw     r2, #0xdb17
003f88c4 movt     r2, #0x2b52
003f88c8 ldr      r3, [r4, r3]
003f88cc str      r2, [sp, #0x24]
003f88d0 ldr      r2, [pc, #0x338]
003f88d4 ldr      sl, [r3]
003f88d8 movw     ip, #0xf26b
003f88dc ldr      fp, [pc, #0x330]
003f88e0 ldr      sb, [pc, #0x330]
003f88e4 movt     ip, #0xda
003f88e8 str      ip, [sp, #0x28]
003f88ec mov      r7, #0
003f88f0 str      r2, [sp, #0x2c]
003f88f4 sub      sl, sl, #1
003f88f8 cmp      r7, #0
003f88fc beq      #0x3f8a78
003f8900 mov      r0, r7
003f8904 ldr      r1, [r5, #0x10c]
003f8908 bl       #0x30e31c
003f890c cmp      r0, #0
003f8910 beq      #0x3f8a78
003f8914 mov      r0, r7
003f8918 bl       #0x3ef9fc ; _Z25DBG_DisplayLoadLevelStatsPKc
003f891c ldr      r0, [sp, #0x20]
003f8920 bl       #0x42a738 ; _ZN12MenuDebugHUD14HideHugeNumberEv
003f8924 ldr      r0, [sp, #0x20]
003f8928 mov      r1, #0
003f892c bl       #0x42a8c0 ; _ZN12MenuDebugHUD25SetDisplaySealOfFreshnessEb
003f8930 ldr      r3, [pc, #0x2e4]
003f8934 ldr      r5, [r4, r8]
003f8938 mov      r1, #0
003f893c add      r3, pc, r3
003f8940 movw     r2, #0x1388
003f8944 str      r2, [r3]
003f8948 ldr      r0, [r5, #0x40]
003f894c mov      r2, r1
003f8950 bl       #0x36e744 ; _ZN13PlayerManager9GetPlayerEib
003f8954 ldr      r3, [r0, #0x664]
003f8958 mov      ip, #0
003f895c mov      lr, #1
003f8960 mov      r0, r5
003f8964 mov      r1, r7
003f8968 mov      r2, ip
003f896c bic      r3, r3, r3, asr #31
003f8970 stm      sp, {ip, lr}
003f8974 str      ip, [sp, #8]
003f8978 str      ip, [sp, #0xc]
003f897c str      ip, [sp, #0x10]
003f8980 str      ip, [sp, #0x14]
003f8984 bl       #0x32bdc8 ; _ZN11Application9LoadLevelEPKcijbbibjj
003f8988 b        #0x3f882c
003f898c ldr      r3, [pc, #0x26c]
003f8990 ldr      r0, [r4, r3]
003f8994 bl       #0x45c37c ; _ZN13ScriptManager17ExecuteAllScriptsEv
003f8998 b        #0x3f8474
003f899c ldr      r3, [r4, r8]
003f89a0 ldr      r0, [r3, #0x40]
003f89a4 bl       #0x378fb4 ; _ZN13PlayerManager6UpdateEv
003f89a8 b        #0x3f8460
003f89ac ldr      r1, [r5, #0x11c]
003f89b0 mov      ip, #0x7d0
003f89b4 mov      r0, r7
003f89b8 mov      r2, #1
003f89bc str      ip, [sp]
003f89c0 bl       #0x36bd78 ; _ZN15VoxSoundManager9PlayMusicEibbi
003f89c4 b        #0x3f87a0
003f89c8 ldr      r2, [pc, #0x250]
003f89cc ldr      r2, [r4, r2]
003f89d0 ldr      r2, [r2]
003f89d4 cmp      r2, #2
003f89d8 streq    r3, [r3]
003f89dc beq      #0x3f8494
003f89e0 cmp      r2, #1
003f89e4 bne      #0x3f8494
003f89e8 ldr      r0, [pc, #0x234]
003f89ec ldr      r1, [pc, #0x234]
003f89f0 ldr      r2, [pc, #0x234]
003f89f4 ldr      r0, [r4, r0]
003f89f8 ldr      r3, [pc, #0x230]
003f89fc mov      ip, #0x1dc
003f8a00 add      r1, pc, r1
003f8a04 add      r3, pc, r3
003f8a08 add      r0, r0, #0xa8
003f8a0c add      r2, pc, r2
003f8a10 str      ip, [sp]
003f8a14 bl       #0x30e004
003f8a18 ldr      r3, [r5, #0x38]
003f8a1c b        #0x3f8494
003f8a20 ldr      r3, [pc, #0x1f8]
003f8a24 ldr      r3, [r4, r3]
003f8a28 ldr      r3, [r3]
003f8a2c cmp      r3, #2
003f8a30 streq    r7, [r7]
003f8a34 beq      #0x3f867c
003f8a38 cmp      r3, #1
003f8a3c bne      #0x3f867c
003f8a40 ldr      r0, [pc, #0x1dc]
003f8a44 ldr      r1, [pc, #0x1e8]
003f8a48 ldr      r2, [pc, #0x1e8]
003f8a4c ldr      r0, [r4, r0]
003f8a50 ldr      r3, [pc, #0x1e4]
003f8a54 mov      ip, #0x1dc
003f8a58 add      r1, pc, r1
003f8a5c add      r0, r0, #0xa8
003f8a60 add      r2, pc, r2
003f8a64 add      r3, pc, r3
003f8a68 str      ip, [sp]
003f8a6c bl       #0x30e004
003f8a70 ldr      r7, [r5, #0x38]
003f8a74 b        #0x3f867c
003f8a78 cmp      sl, #0
003f8a7c moveq    r1, sl
003f8a80 beq      #0x3f8ae8
003f8a84 ldr      r3, [sp, #0x2c]
003f8a88 movw     ip, #0xe6ab
003f8a8c mov      r1, sl
003f8a90 ldr      r2, [r4, r3]
003f8a94 ldr      r0, [r2]
003f8a98 mul      r0, ip, r0
003f8a9c ldr      ip, [sp, #0x24]
003f8aa0 add      r0, r0, #0x2b000
003f8aa4 add      r0, r0, #0x3fc
003f8aa8 add      r0, r0, #1
003f8aac umull    ip, r3, ip, r0
003f8ab0 rsb      ip, r3, r0
003f8ab4 add      r3, r3, ip, lsr #1
003f8ab8 ldr      ip, [sp, #0x28]
003f8abc lsr      r3, r3, #0x17
003f8ac0 mls      r3, ip, r3, r0
003f8ac4 str      r3, [r2]
003f8ac8 mov      r0, r3
003f8acc bl       #0x30eb2c
003f8ad0 cmp      r1, #0
003f8ad4 movge    r2, #0x48
003f8ad8 rsblt    r1, r1, #0
003f8adc movlt    r3, #0x48
003f8ae0 mulge    r1, r2, r1
003f8ae4 mullt    r1, r3, r1
003f8ae8 ldr      r3, [r4, fp]
003f8aec ldr      r2, [r4, sb]
003f8af0 ldr      r0, [r3]
003f8af4 ldr      r2, [r2]
003f8af8 add      r0, r0, #1
003f8afc str      r0, [r3]
003f8b00 add      r2, r2, r1
003f8b04 ldrb     r3, [r2, #4]
003f8b08 cmp      r3, #0
003f8b0c ldrne    r7, [r2, #0x20]
003f8b10 b        #0x3f88f8
003f8b14 ldr      r3, [fp, #0x10]
003f8b18 add      sb, sp, #0x8c
003f8b1c ldr      r3, [r3, #0x1c]
003f8b20 str      r3, [sp, #0x1c]
003f8b24 bl       #0x330740 ; _ZN13DebugSwitches11GetInstanceEv
003f8b28 mov      r1, r7
003f8b2c mov      fp, r0
003f8b30 add      r2, sp, #0x5c
003f8b34 mov      r0, sb
003f8b38 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f8b3c mov      r0, fp
003f8b40 mov      r1, sb
003f8b44 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003f8b48 ldr      r3, [sp, #0x1c]
003f8b4c eor      r0, r0, #1
003f8b50 strb     r0, [r3, #0x431]
003f8b54 mov      r0, sb
003f8b58 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f8b5c b        #0x3f8774
003f8b60 ldr      r3, [fp, #0x10]
003f8b64 add      sb, sp, #0xbc
003f8b68 ldr      r3, [r3, #0x1c]
003f8b6c str      r3, [sp, #0x1c]
003f8b70 bl       #0x330740 ; _ZN13DebugSwitches11GetInstanceEv
003f8b74 mov      r1, r7
003f8b78 mov      fp, r0
003f8b7c add      r2, sp, #0x64
003f8b80 mov      r0, sb
003f8b84 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003f8b88 mov      r0, fp
003f8b8c mov      r1, sb
003f8b90 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003f8b94 ldr      r3, [sp, #0x1c]
003f8b98 eor      r0, r0, #1
003f8b9c strb     r0, [r3, #0x430]
003f8ba0 mov      r0, sb
003f8ba4 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f8ba8 b        #0x3f8714
003f8bac mov      r0, r3
003f8bb0 ldr      r3, [r3]
003f8bb4 mov      lr, pc
003f8bb8 ldr      pc, [r3, #0x10]
003f8bbc b        #0x3f8524
003f8bc0 bl       #0x30e310
003f8bc4 subseq   ip, sb, r8, lsr #15
003f8bc8 andeq    r4, r0, ip, lsr #1
003f8bcc subeq    lr, ip, ip, lsl #17
003f8bd0 subeq    lr, ip, ip, asr r8
003f8bd4 andeq    r0, r0, r4, lsl #17
003f8bd8 subeq    r6, ip, r4, asr #23
003f8bdc subeq    r7, ip, r0, ror #10
003f8be0 strdeq   r3, r4, [r0], -r4
003f8be4 andeq    r1, r0, r8, lsl #22
003f8be8 strheq   r4, [r0], -r0
003f8bec subeq    r7, ip, r0, lsr r3
003f8bf0 subeq    r7, ip, ip, ror #5
003f8bf4 andeq    r0, r0, r4, lsr #27
003f8bf8 umaaleq  lr, ip, r4, r3
003f8bfc subeq    lr, ip, r8, asr r3
003f8c00 andeq    r1, r0, r0, lsr #20
003f8c04 subseq   sl, sl, r8, ror #16
003f8c08 ldrsbeq  r0, [sl], #-0xe0
003f8c0c andeq    r1, r0, r0, asr #17
003f8c10 muleq    r0, r4, ip
003f8c14 andeq    r1, r0, r8, lsl #1
003f8c18 andeq    r0, r0, r4, ror r8
003f8c1c subseq   r0, sl, r0, lsr #28
003f8c20 andeq    r3, r0, r0, asr #19
003f8c24 andeq    r1, r0, r0, asr #19
003f8c28 ldrdeq   r5, r6, [ip], #-0x98
003f8c2c subeq    sp, ip, r4, ror #3
003f8c30 subeq    sb, ip, r4, asr r2
003f8c34 subeq    r5, ip, r0, lsl #19
003f8c38 umaaleq  sp, ip, r0, r1
003f8c3c strdeq   sb, sl, [ip], #-0x14
_ZN7GSLevel6UpdateEP12StateMachined
00386630 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00386634 ldr      r4, [pc, #0x1c0]
00386638 ldr      r3, [pc, #0x1c0]
0038663c ldr      r5, [pc, #0x1c0]
00386640 add      r4, pc, r4
00386644 ldr      r2, [r4, r3]
00386648 ldr      r3, [r4, r5]
0038664c sub      sp, sp, #0x44
00386650 ldrb     r2, [r2, #0xec]
00386654 ldr      r3, [r3]
00386658 mov      r6, r0
0038665c cmp      r2, #0
00386660 str      r3, [sp, #0x3c]
00386664 bne      #0x38674c
00386668 ldr      r3, [r0, #0x38]
0038666c sub      r3, r3, #1
00386670 cmp      r3, #3
00386674 addls    pc, pc, r3, lsl #2
00386678 b        #0x386710
0038667c b        #0x386768
00386680 b        #0x386778
00386684 b        #0x386790
00386688 b        #0x38668c
0038668c ldr      r3, [pc, #0x174]
00386690 ldr      r2, [pc, #0x174]
00386694 ldr      sb, [pc, #0x174]
00386698 ldr      r8, [r4, r3]
0038669c ldr      r3, [pc, #0x170]
003866a0 ldr      fp, [r4, r2]
003866a4 mov      sl, #0
003866a8 ldr      r3, [r4, r3]
003866ac add      sb, pc, sb
003866b0 add      r7, sp, #0x24
003866b4 str      sl, [r3]
003866b8 mov      r0, r8
003866bc strb     sl, [fp]
003866c0 add      sb, sb, #0xd
003866c4 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003866c8 mov      r0, r7
003866cc mov      r1, sb
003866d0 str      r7, [sp, #0x34]
003866d4 str      r7, [sp, #0x38]
003866d8 bl       #0x3865e0 ; _ZNSs19_M_range_initializeEPKcS0_.clone.2
003866dc mov      r1, r7
003866e0 mov      r0, r8
003866e4 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003866e8 mov      r3, r0
003866ec mov      r0, r7
003866f0 str      r3, [sp, #4]
003866f4 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003866f8 ldr      r3, [sp, #4]
003866fc cmp      r3, sl
00386700 bne      #0x3867b4
00386704 ldr      r0, [r6, #0x34]
00386708 mov      r1, #0
0038670c bl       #0x3f82d8 ; _ZN5Level6UpdateEb
00386710 ldr      r3, [r6, #0x34]
00386714 cmp      r3, #0
00386718 beq      #0x386740
0038671c ldrb     r2, [r3, #0x145]
00386720 cmp      r2, #0
00386724 bne      #0x38674c
00386728 ldr      r2, [r3, #0x130]
0038672c cmp      r2, #1
00386730 ble      #0x386740
00386734 ldr      r3, [r3, #0x130]
00386738 cmp      r3, #0x1a
0038673c ble      #0x38674c
00386740 bl       #0x42ca8c ; _ZN11MenuManager11GetInstanceEv
00386744 mov      r1, #1
00386748 bl       #0x42ea04 ; _ZN11MenuManager6UpdateEb
0038674c ldr      r3, [r4, r5]
00386750 ldr      r2, [sp, #0x3c]
00386754 ldr      r3, [r3]
00386758 cmp      r2, r3
0038675c bne      #0x3867f8
00386760 add      sp, sp, #0x44
00386764 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00386768 mov      r3, #2
0038676c str      r3, [r0, #0x38]
00386770 ldr      r3, [r0, #0x34]
00386774 b        #0x386714
00386778 ldr      r0, [r0, #0x34]
0038677c bl       #0x3ef218 ; _ZN5Level4LoadEv
00386780 mov      r3, #3
00386784 str      r3, [r6, #0x38]
00386788 ldr      r3, [r6, #0x34]
0038678c b        #0x386714
00386790 ldr      r0, [r0, #0x34]
00386794 mov      r1, #0
00386798 bl       #0x3f82d8 ; _ZN5Level6UpdateEb
0038679c ldr      r3, [r6, #0x34]
003867a0 ldr      r2, [r3, #0x130]
003867a4 cmp      r2, #0x26
003867a8 moveq    r2, #4
003867ac streq    r2, [r6, #0x38]
003867b0 b        #0x386714
003867b4 mov      r3, #1
003867b8 add      r7, sp, #0xc
003867bc strb     r3, [fp]
003867c0 mov      r0, r8
003867c4 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003867c8 mov      r1, sb
003867cc mov      r0, r7
003867d0 str      r7, [sp, #0x1c]
003867d4 str      r7, [sp, #0x20]
003867d8 bl       #0x3865e0 ; _ZNSs19_M_range_initializeEPKcS0_.clone.2
003867dc mov      r0, r8
003867e0 mov      r1, r7
003867e4 mov      r2, sl
003867e8 bl       #0x337ddc ; _ZN13DebugSwitches9SetSwitchERKSsb
003867ec mov      r0, r7
003867f0 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003867f4 b        #0x386704
003867f8 bl       #0x30e310
003867fc rsbeq    lr, r0, r0, asr r4
00386800 strdeq   r3, r4, [r0], -r4
00386804 andeq    r4, r0, ip, lsr #1
00386808 andeq    r0, r0, r4, lsl #17
0038680c ldrdeq   r2, r3, [r0], -r0
00386810 subseq   fp, r3, r4, asr #18
00386814 strdeq   r2, r3, [r0], -ip
_ZN11Application6UpdateEv
0032ccc4 push     {r4, r5, r6, r7, r8, sl, lr}
0032ccc8 ldr      r5, [pc, #0x2d4]
0032cccc ldr      r6, [pc, #0x2d4]
0032ccd0 ldrb     r2, [r0, #0xa4]
0032ccd4 add      r5, pc, r5
0032ccd8 ldr      r3, [r5, r6]
0032ccdc sub      sp, sp, #0x2c
0032cce0 cmp      r2, #0
0032cce4 ldr      r3, [r3]
0032cce8 mov      r4, r0
0032ccec str      r3, [sp, #0x24]
0032ccf0 beq      #0x32cd10
0032ccf4 ldr      r3, [r5, r6]
0032ccf8 ldr      r2, [sp, #0x24]
0032ccfc ldr      r3, [r3]
0032cd00 cmp      r2, r3
0032cd04 bne      #0x32cfa0
0032cd08 add      sp, sp, #0x2c
0032cd0c pop      {r4, r5, r6, r7, r8, sl, pc}
0032cd10 bl       #0x7fd794 ; _Z9GetOnlinev
0032cd14 ldrb     r3, [r0, #5]
0032cd18 cmp      r3, #0
0032cd1c bne      #0x32cea8
0032cd20 ldrb     r3, [r4, #0x7a]
0032cd24 cmp      r3, #0
0032cd28 movne    r3, #0
0032cd2c strbne   r3, [r4, #0x7a]
0032cd30 ldr      r3, [r4, #0x10]
0032cd34 ldr      r3, [r3, #0x20]
0032cd38 mov      r0, r3
0032cd3c ldr      r3, [r3]
0032cd40 mov      lr, pc
0032cd44 ldr      pc, [r3, #0xc]
0032cd48 ldr      r3, [r4, #0x70]
0032cd4c mov      r7, r0
0032cd50 rsb      r3, r3, r0
0032cd54 cmp      r3, #0x7d0
0032cd58 bhi      #0x32cf00
0032cd5c ldr      r0, [r4, #0x28]
0032cd60 cmp      r0, #0
0032cd64 beq      #0x32cd80
0032cd68 ldr      r3, [pc, #0x23c]
0032cd6c add      r3, pc, r3
0032cd70 ldr      r2, [r3, #4]
0032cd74 rsb      r2, r2, r7
0032cd78 cmp      r2, #0xfa0
0032cd7c bgt      #0x32ce9c
0032cd80 str      r7, [r4, #0x70]
0032cd84 bl       #0x7fd794 ; _Z9GetOnlinev
0032cd88 ldrb     r3, [r0, #5]
0032cd8c cmp      r3, #0
0032cd90 bne      #0x32cec0
0032cd94 ldr      r0, [r4, #0x20]
0032cd98 bl       #0x33c568 ; _ZN15TouchScreenBase13ProcessEventsEv
0032cd9c mov      r0, r4
0032cda0 bl       #0x320da4 ; _ZN11Application9ComputeDtEv
0032cda4 ldr      r0, [r4, #0x8c]
0032cda8 bl       #0x30e2e0
0032cdac mov      r7, r0
0032cdb0 mov      r1, r0
0032cdb4 mov      r0, #0x44000000
0032cdb8 add      r0, r0, #0x7a0000
0032cdbc bl       #0x30ec94
0032cdc0 ldr      r3, [pc, #0x1e8]
0032cdc4 mov      r1, r7
0032cdc8 mov      sl, r0
0032cdcc ldr      r8, [r5, r3]
0032cdd0 add      r7, sp, #0xc
0032cdd4 mov      r0, r8
0032cdd8 bl       #0x31177c ; _ZN12PerfCounters6UpdateEf
0032cddc ldr      r1, [r4, #0x8c]
0032cde0 mov      r0, r4
0032cde4 bl       #0x32c438 ; _ZN11Application7_UpdateEi
0032cde8 mov      r0, r4
0032cdec bl       #0x32ade8 ; _ZN11Application5_DrawEv
0032cdf0 ldr      r1, [pc, #0x1bc]
0032cdf4 add      r2, sp, #8
0032cdf8 mov      r0, r7
0032cdfc add      r1, pc, r1
0032ce00 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0032ce04 mov      ip, #0x42000000
0032ce08 mov      r3, #0
0032ce0c add      ip, ip, #0x700000
0032ce10 mov      r2, sl
0032ce14 mov      r1, r7
0032ce18 mov      r0, r8
0032ce1c str      ip, [sp]
0032ce20 bl       #0x312734 ; _ZN12PerfCounters15SetCounterValueERKSsfff
0032ce24 mov      r0, r7
0032ce28 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0032ce2c bl       #0x7fd794 ; _Z9GetOnlinev
0032ce30 ldrb     r3, [r0, #5]
0032ce34 cmp      r3, #0
0032ce38 bne      #0x32ceb4
0032ce3c bl       #0x38174c ; _ZN6Device17IsHighPerformanceEv
0032ce40 cmp      r0, #0
0032ce44 beq      #0x32ccf4
0032ce48 ldr      r3, [pc, #0x168]
0032ce4c add      r3, pc, r3
0032ce50 ldr      r2, [r3, #8]
0032ce54 ldr      r0, [r3, #0xc]
0032ce58 add      r2, r2, #1
0032ce5c str      r2, [r3, #8]
0032ce60 ldr      r1, [r4, #0x8c]
0032ce64 cmp      r2, #0xa
0032ce68 add      r2, r0, r1
0032ce6c str      r2, [r3, #0xc]
0032ce70 beq      #0x32cf10
0032ce74 ldr      r1, [r3, #0x10]
0032ce78 cmp      r1, #0
0032ce7c ble      #0x32ccf4
0032ce80 ldr      r3, [r4, #0x10]
0032ce84 mov      r2, #0
0032ce88 mov      r0, r3
0032ce8c ldr      r3, [r3]
0032ce90 mov      lr, pc
0032ce94 ldr      pc, [r3, #0x10]
0032ce98 b        #0x32ccf4
0032ce9c str      r7, [r3, #4]
0032cea0 bl       #0x339100 ; _ZNK19DataReloaderManager10checkFilesEv
0032cea4 b        #0x32cd80
0032cea8 bl       #0x7fd794 ; _Z9GetOnlinev
0032ceac bl       #0x7fd914 ; _ZN7COnline14ReceivePacketsEv
0032ceb0 b        #0x32cd20
0032ceb4 bl       #0x7fd794 ; _Z9GetOnlinev
0032ceb8 bl       #0x7fd634 ; _ZN7COnline11SendPacketsEv
0032cebc b        #0x32ce3c
0032cec0 ldr      r7, [pc, #0xf4]
0032cec4 add      r7, pc, r7
0032cec8 mov      r0, r7
0032cecc bl       #0x3136b4 ; _Z20PushProfilingContextPKc
0032ced0 bl       #0x7fd794 ; _Z9GetOnlinev
0032ced4 mov      r8, r0
0032ced8 ldr      r0, [r4, #0x8c]
0032cedc bl       #0x30e2e0
0032cee0 mov      r1, r0
0032cee4 mov      r0, r8
0032cee8 bl       #0x824f34 ; _ZN11COnlineImpl6UpdateEf
0032ceec bl       #0x320e98 ; _ZN15OnlineSingletonI15OnlineGameStateE11GetInstanceEv
0032cef0 bl       #0x4a039c ; _ZN15OnlineGameState6UpdateEv
0032cef4 mov      r0, r7
0032cef8 bl       #0x3136b8 ; _Z19PopProfilingContextPKc
0032cefc b        #0x32cd94
0032cf00 str      r0, [r4, #0x70]
0032cf04 mov      r0, r4
0032cf08 bl       #0x320da4 ; _ZN11Application9ComputeDtEv
0032cf0c b        #0x32ccf4
0032cf10 movw     r1, #0x6667
0032cf14 movt     r1, #0x6666
0032cf18 smull    r0, r1, r1, r2
0032cf1c ldr      r0, [r3, #0x10]
0032cf20 asr      r2, r2, #0x1f
0032cf24 rsb      r1, r2, r1, asr #2
0032cf28 rsb      r1, r0, r1
0032cf2c cmp      r1, #0xf
0032cf30 rsble    r1, r1, #0x10
0032cf34 strle    r1, [r3, #0x10]
0032cf38 ble      #0x32cf74
0032cf3c cmp      r1, #0x20
0032cf40 rsble    r1, r1, #0x21
0032cf44 strle    r1, [r3, #0x10]
0032cf48 ble      #0x32cf74
0032cf4c cmp      r1, #0x31
0032cf50 rsble    r1, r1, #0x32
0032cf54 strle    r1, [r3, #0x10]
0032cf58 ble      #0x32cf74
0032cf5c mov      r2, #0
0032cf60 str      r2, [r3, #0xc]
0032cf64 str      r2, [r3, #0x10]
0032cf68 str      r2, [r3, #8]
0032cf6c mov      r1, #5
0032cf70 b        #0x32cf90
0032cf74 ldr      r3, [pc, #0x44]
0032cf78 mov      r2, #0
0032cf7c cmp      r1, #4
0032cf80 add      r3, pc, r3
0032cf84 str      r2, [r3, #0xc]
0032cf88 str      r2, [r3, #8]
0032cf8c ble      #0x32cf6c
0032cf90 ldr      r3, [pc, #0x2c]
0032cf94 add      r3, pc, r3
0032cf98 str      r1, [r3, #0x10]
0032cf9c b        #0x32ce80
0032cfa0 bl       #0x30e310
0032cfa4 strhteq  r7, [r6], #-0xdc
0032cfa8 andeq    r4, r0, ip, lsr #1
0032cfac rsbeq    r2, r7, r8, lsl #25
0032cfb0 strdeq   r0, r1, [r0], -ip
0032cfb4 subseq   r2, sb, r4, lsl #10
0032cfb8 rsbeq    r2, r7, r8, lsr #23
0032cfbc subseq   r2, sb, ip, lsr #8
0032cfc0 rsbeq    r2, r7, r4, ror sl
0032cfc4 rsbeq    r2, r7, r0, ror #20
