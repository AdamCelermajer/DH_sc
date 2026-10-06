_ZN5LevelD1Ev
003f9458 mov      r3, #0
003f945c str      r3, [r4, #0xec]
003f9460 ldr      r5, [r5, r7]
003f9464 mov      r0, r5
003f9468 bl       #0x31f55c ; _ZN11Application11CleanGlitchEv
003f946c ldr      r0, [r5, #0x3c]
003f9470 bl       #0x379fe8 ; _ZN10LuaManager18FlushBufferedFilesEv
003f9474 add      r0, r4, #0xf8
003f9478 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f947c add      r0, r4, #0x44
003f9480 bl       #0x37c004 ; _ZN9LuaScriptD1Ev
003f9484 mov      r0, r4
003f9488 bl       #0x338590 ; _ZN12EventManagerD2Ev
003f948c mov      r0, r4
003f9490 pop      {r4, r5, r6, r7, r8, pc}
003f9494 ldr      r0, [r6, #4]
003f9498 bl       #0x3f18d8 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKijENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE.clone.18
003f949c mov      r3, #0
003f94a0 str      r3, [r6, #0x10]
003f94a4 stmib    r6, {r3, r6}
003f94a8 str      r6, [r6, #0xc]
003f94ac b        #0x3f930c
003f94b0 ldrheq   fp, [sb], #-0x7c
003f94b4 andeq    r0, r0, r4, ror r7
003f94b8 ldrdeq   r4, r5, [r0], -r8
003f94bc andeq    r1, r0, r0, lsr #20
003f94c0 andeq    r0, r0, ip, lsr #28

_ZN5LevelD2Ev
003f9244 mov      r3, #0
003f9248 str      r3, [r4, #0xec]
003f924c ldr      r5, [r5, r7]
003f9250 mov      r0, r5
003f9254 bl       #0x31f55c ; _ZN11Application11CleanGlitchEv
003f9258 ldr      r0, [r5, #0x3c]
003f925c bl       #0x379fe8 ; _ZN10LuaManager18FlushBufferedFilesEv
003f9260 add      r0, r4, #0xf8
003f9264 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003f9268 add      r0, r4, #0x44
003f926c bl       #0x37c004 ; _ZN9LuaScriptD1Ev
003f9270 mov      r0, r4
003f9274 bl       #0x338590 ; _ZN12EventManagerD2Ev
003f9278 mov      r0, r4
003f927c pop      {r4, r5, r6, r7, r8, pc}
003f9280 ldr      r0, [r6, #4]
003f9284 bl       #0x3f18d8 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKijENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE.clone.18
003f9288 mov      r3, #0
003f928c str      r3, [r6, #0x10]
003f9290 stmib    r6, {r3, r6}
003f9294 str      r6, [r6, #0xc]
003f9298 b        #0x3f90f8
003f929c ldrsbeq  fp, [sb], #-0x90
003f92a0 andeq    r0, r0, r4, ror r7
003f92a4 ldrdeq   r4, r5, [r0], -r8
003f92a8 andeq    r1, r0, r0, lsr #20
003f92ac andeq    r0, r0, ip, lsr #28

_ZN11Application8PostInitEv
0032f7f0 ldr      r2, [pc, #0x258]
0032f7f4 sub      sp, sp, #0x44
0032f7f8 add      r8, pc, r8
0032f7fc ldr      r3, [r8, r2]
0032f800 mov      r5, r0
0032f804 mov      r1, #0
0032f808 ldr      r3, [r3]
0032f80c mov      r0, #0x30
0032f810 str      r2, [sp, #4]
0032f814 str      r3, [sp, #0x3c]
0032f818 bl       #0x310570 ; _Znwj15MemoryHintState
0032f81c mov      r6, r0
0032f820 bl       #0x3380bc ; _ZN12EventManagerC1Ev
0032f824 mov      r1, #0
0032f828 str      r6, [r5, #0x14]
0032f82c mov      r0, #0x20
0032f830 bl       #0x310570 ; _Znwj15MemoryHintState
0032f834 mov      r6, r0
0032f838 bl       #0x339ec8 ; _ZN12StateMachineC1Ev
0032f83c str      r6, [r5, #0x18]
0032f840 mov      r1, #0
0032f844 mov      r0, #0x24
0032f848 bl       #0x310570 ; _Znwj15MemoryHintState
0032f84c ldr      r1, [r5, #0x28]
0032f850 mov      r6, r0
0032f854 bl       #0x4c36b8 ; _ZN15PyDataConstantsC1EP19DataReloaderManager
0032f858 str      r6, [r5, #0x2c]

_ZN5LevelC2EPKcijjjbbii
003f34dc ldr      r3, [sp, #0x1c]
003f34e0 add      sb, pc, sb
003f34e4 mov      r4, r0
003f34e8 ldr      lr, [sb, r3]
003f34ec mov      r3, r2
003f34f0 mov      r8, r1
003f34f4 ldr      r2, [lr]
003f34f8 ldrb     fp, [sp, #0x458]
003f34fc str      r3, [sp, #0x18]
003f3500 str      ip, [sp, #0x14]
003f3504 str      r2, [sp, #0x424]
003f3508 ldrb     sl, [sp, #0x45c]
003f350c bl       #0x33805c ; _ZN12EventManagerC2Ev
003f3510 ldr      r2, [pc, #0x31c]
003f3514 mov      r6, #0
003f3518 mvn      r5, #0
003f351c ldr      r2, [sb, r2]
003f3520 add      r7, r4, #0x44
003f3524 mov      r1, r6
003f3528 add      r2, r2, #8
003f352c str      r2, [r4]
003f3530 str      r6, [r4, #0x30]
003f3534 str      r6, [r4, #0x38]
003f3538 str      r5, [r4, #0x3c]
003f353c str      r5, [r4, #0x40]
003f3540 mov      r0, r7
003f3544 bl       #0x37c584 ; _ZN9LuaScriptC1Eb

_ZN12EventManagerD0Ev
00338574 push     {r4, lr}
00338578 mov      r4, r0
0033857c bl       #0x3384f8 ; _ZN12EventManagerD1Ev
00338580 mov      r0, r4
00338584 bl       #0x310440 ; _Z10CustomFreePv
00338588 mov      r0, r4
0033858c pop      {r4, pc}

_ZN5LevelC1EPKcijjjbbii
003f3144 ldr      r3, [sp, #0x1c]
003f3148 add      sb, pc, sb
003f314c mov      r4, r0
003f3150 ldr      lr, [sb, r3]
003f3154 mov      r3, r2
003f3158 mov      r8, r1
003f315c ldr      r2, [lr]
003f3160 ldrb     fp, [sp, #0x458]
003f3164 str      r3, [sp, #0x18]
003f3168 str      ip, [sp, #0x14]
003f316c str      r2, [sp, #0x424]
003f3170 ldrb     sl, [sp, #0x45c]
003f3174 bl       #0x33805c ; _ZN12EventManagerC2Ev
003f3178 ldr      r2, [pc, #0x31c]
003f317c mov      r6, #0
003f3180 mvn      r5, #0
003f3184 ldr      r2, [sb, r2]
003f3188 add      r7, r4, #0x44
003f318c mov      r1, r6
003f3190 add      r2, r2, #8
003f3194 str      r2, [r4]
003f3198 str      r6, [r4, #0x30]
003f319c str      r6, [r4, #0x38]
003f31a0 str      r5, [r4, #0x3c]
003f31a4 str      r5, [r4, #0x40]
003f31a8 mov      r0, r7
003f31ac bl       #0x37c584 ; _ZN9LuaScriptC1Eb