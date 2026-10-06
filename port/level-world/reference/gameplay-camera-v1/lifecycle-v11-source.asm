_ZN11CameraLevelD0Ev
0040fee8 push     {r4, lr}
0040feec mov      r4, r0
0040fef0 bl       #0x40fe7c ; _ZN11CameraLevelD1Ev
0040fef4 mov      r0, r4
0040fef8 bl       #0x310440 ; _Z10CustomFreePv
0040fefc mov      r0, r4
0040ff00 pop      {r4, pc}

_ZN10CameraBaseD2Ev
0040e780 ldr      r3, [pc, #0x80]
0040e784 ldr      r2, [pc, #0x80]
0040e788 ldr      r1, [pc, #0x80]
0040e78c add      r3, pc, r3
0040e790 push     {r4, lr}
0040e794 ldr      r2, [r3, r2]
0040e798 ldr      r1, [r3, r1]
0040e79c mov      r4, r0
0040e7a0 add      r2, r2, #8
0040e7a4 str      r2, [r0]
0040e7a8 ldr      r2, [r1]
0040e7ac cmp      r2, r0
0040e7b0 moveq    r3, #0
0040e7b4 streq    r3, [r1]
0040e7b8 ldr      r3, [r0, #4]
0040e7bc cmp      r3, #0
0040e7c0 beq      #0x40e7dc
0040e7c4 ldr      r2, [r3]
0040e7c8 ldr      r0, [r2, #-0xc]
0040e7cc add      r0, r3, r0
0040e7d0 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0040e7d4 mov      r3, #0
0040e7d8 str      r3, [r4, #4]
0040e7dc ldr      r3, [r4, #8]
0040e7e0 cmp      r3, #0
0040e7e4 beq      #0x40e800
0040e7e8 ldr      r2, [r3]
0040e7ec ldr      r0, [r2, #-0xc]
0040e7f0 add      r0, r3, r0
0040e7f4 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0040e7f8 mov      r3, #0
0040e7fc str      r3, [r4, #8]
0040e800 mov      r0, r4
0040e804 pop      {r4, pc}
0040e808 subseq   r6, r8, r4, lsl #6
0040e80c andeq    r2, r0, ip, ror #3
0040e810 strheq   r4, [r0], -r0

_ZN11CameraLevelD2Ev
0040ff04 push     {r4, lr}
0040ff08 ldr      r3, [pc, #0x58]
0040ff0c ldr      r2, [pc, #0x58]
0040ff10 ldr      r1, [r0, #0x44]
0040ff14 add      r3, pc, r3
0040ff18 ldr      r2, [r3, r2]
0040ff1c cmp      r1, #0
0040ff20 mov      r4, r0
0040ff24 add      r2, r2, #8
0040ff28 str      r2, [r0]
0040ff2c beq      #0x40ff48
0040ff30 ldr      r3, [r1]
0040ff34 mov      r0, r1
0040ff38 mov      lr, pc
0040ff3c ldr      pc, [r3, #4]
0040ff40 mov      r3, #0
0040ff44 str      r3, [r4, #0x44]
0040ff48 add      r0, r4, #0x64
0040ff4c bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0040ff50 add      r0, r4, #0x4c
0040ff54 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0040ff58 mov      r0, r4
0040ff5c bl       #0x411c0c ; _ZN12CameraTargetD2Ev
0040ff60 mov      r0, r4
0040ff64 pop      {r4, pc}
0040ff68 subseq   r4, r8, ip, ror fp
0040ff6c andeq    r4, r0, r8, lsr #4

_ZN11CameraLevelD1Ev
0040fe7c push     {r4, lr}
0040fe80 ldr      r3, [pc, #0x58]
0040fe84 ldr      r2, [pc, #0x58]
0040fe88 ldr      r1, [r0, #0x44]
0040fe8c add      r3, pc, r3
0040fe90 ldr      r2, [r3, r2]
0040fe94 cmp      r1, #0
0040fe98 mov      r4, r0
0040fe9c add      r2, r2, #8
0040fea0 str      r2, [r0]
0040fea4 beq      #0x40fec0
0040fea8 ldr      r3, [r1]
0040feac mov      r0, r1
0040feb0 mov      lr, pc
0040feb4 ldr      pc, [r3, #4]
0040feb8 mov      r3, #0
0040febc str      r3, [r4, #0x44]
0040fec0 add      r0, r4, #0x64
0040fec4 bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0040fec8 add      r0, r4, #0x4c
0040fecc bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0040fed0 mov      r0, r4
0040fed4 bl       #0x411c0c ; _ZN12CameraTargetD2Ev
0040fed8 mov      r0, r4
0040fedc pop      {r4, pc}
0040fee0 subseq   r4, r8, r4, lsl #24
0040fee4 andeq    r4, r0, r8, lsr #4

_ZN10CameraBaseD1Ev
0040e814 ldr      r3, [pc, #0x80]
0040e818 ldr      r2, [pc, #0x80]
0040e81c ldr      r1, [pc, #0x80]
0040e820 add      r3, pc, r3
0040e824 push     {r4, lr}
0040e828 ldr      r2, [r3, r2]
0040e82c ldr      r1, [r3, r1]
0040e830 mov      r4, r0
0040e834 add      r2, r2, #8
0040e838 str      r2, [r0]
0040e83c ldr      r2, [r1]
0040e840 cmp      r2, r0
0040e844 moveq    r3, #0
0040e848 streq    r3, [r1]
0040e84c ldr      r3, [r0, #4]
0040e850 cmp      r3, #0
0040e854 beq      #0x40e870
0040e858 ldr      r2, [r3]
0040e85c ldr      r0, [r2, #-0xc]
0040e860 add      r0, r3, r0
0040e864 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0040e868 mov      r3, #0
0040e86c str      r3, [r4, #4]
0040e870 ldr      r3, [r4, #8]
0040e874 cmp      r3, #0
0040e878 beq      #0x40e894
0040e87c ldr      r2, [r3]
0040e880 ldr      r0, [r2, #-0xc]
0040e884 add      r0, r3, r0
0040e888 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0040e88c mov      r3, #0
0040e890 str      r3, [r4, #8]
0040e894 mov      r0, r4
0040e898 pop      {r4, pc}
0040e89c subseq   r6, r8, r0, ror r2
0040e8a0 andeq    r2, r0, ip, ror #3
0040e8a4 strheq   r4, [r0], -r0

_ZN10CameraBaseD0Ev
0040f6f8 push     {r4, lr}
0040f6fc mov      r4, r0
0040f700 bl       #0x40e814 ; _ZN10CameraBaseD1Ev
0040f704 mov      r0, r4
0040f708 bl       #0x310440 ; _Z10CustomFreePv
0040f70c mov      r0, r4
0040f710 pop      {r4, pc}

_ZN13RootSceneNode7NewAnimEb
0035d624 push     {r4, lr}
0035d628 mov      r4, r0
0035d62c bl       #0x35d4cc ; _ZN13RootSceneNode19_EnableDisplacementEb
0035d630 ldrb     r3, [r4, #0x1ec]
0035d634 cmp      r3, #0
0035d638 beq      #0x35d654
0035d63c ldr      r3, [r4, #0x1f0]
0035d640 cmp      r3, #0
0035d644 beq      #0x35d654
0035d648 ldr      r1, [r4, #0x1fc]
0035d64c cmp      r1, #0
0035d650 bne      #0x35d658
0035d654 pop      {r4, pc}
0035d658 mov      r0, r4
0035d65c add      r1, r1, #1
0035d660 bl       #0x35ce6c ; _ZN13RootSceneNode11_ResetDeltaEj
0035d664 mov      r0, r4
0035d668 ldr      r3, [r4]
0035d66c ldr      r1, [r4, #0x1fc]
0035d670 mov      lr, pc
0035d674 ldr      pc, [r3, #0x14]
0035d678 pop      {r4, pc}
_ZN13RootSceneNode19_EnableDisplacementEb
0035d4cc push     {r4, r5, r6, r7, r8, lr}
0035d4d0 subs     r5, r1, #0
0035d4d4 mov      r4, r0
0035d4d8 beq      #0x35d4e8
0035d4dc ldr      r6, [r0, #0x1f0]
0035d4e0 cmp      r6, #0
0035d4e4 beq      #0x35d4f0
0035d4e8 strb     r5, [r4, #0x1ec]
0035d4ec pop      {r4, r5, r6, r7, r8, pc}
0035d4f0 mov      r1, r6
0035d4f4 bl       #0x35ccdc ; _ZN13RootSceneNode11GetAnimRootEb
0035d4f8 mov      r1, #1
0035d4fc str      r0, [r4, #0x1f0]
0035d500 mov      r0, r4
0035d504 bl       #0x35ccdc ; _ZN13RootSceneNode11GetAnimRootEb
0035d508 ldr      r3, [r4, #0x1f0]
0035d50c str      r0, [r4, #0x1f4]
0035d510 cmp      r3, #0
0035d514 beq      #0x35d61c
0035d518 cmp      r0, r3
0035d51c streq    r6, [r4, #0x1f4]
0035d520 beq      #0x35d548
0035d524 cmp      r0, #0
0035d528 beq      #0x35d548
0035d52c ldr      r3, [r0]
0035d530 ldr      r3, [r3, #-0xc]
0035d534 add      r0, r0, r3
0035d538 ldr      r3, [r0, #4]
0035d53c add      r3, r3, #1
0035d540 str      r3, [r0, #4]
0035d544 ldr      r3, [r4, #0x1f0]
0035d548 ldr      r2, [r3]
0035d54c mov      r1, #0
0035d550 mov      r0, #0x150
0035d554 ldr      r2, [r2, #-0xc]
0035d558 mov      r7, r4
0035d55c add      r3, r3, r2
0035d560 ldr      r2, [r3, #4]
0035d564 add      r2, r2, #1
0035d568 str      r2, [r3, #4]
0035d56c bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
0035d570 mvn      r1, #0
0035d574 mov      r6, r0
0035d578 bl       #0x5839d8 ; _ZN6glitch5scene15CEmptySceneNodeC1Ei
0035d57c str      r6, [r4, #0x1f8]
0035d580 mov      r3, r6
0035d584 ldr      r6, [r7, #0xf4]!
0035d588 cmp      r6, r7
0035d58c bne      #0x35d598
0035d590 b        #0x35d5c4
0035d594 ldr      r3, [r4, #0x1f8]
0035d598 cmp      r6, #0
0035d59c moveq    r1, r6
0035d5a0 subne    r1, r6, #4
0035d5a4 ldr      r6, [r6]
0035d5a8 mov      r0, r3
0035d5ac ldr      r3, [r3]
0035d5b0 mov      lr, pc
0035d5b4 ldr      pc, [r3, #0x5c]
0035d5b8 cmp      r7, r6
0035d5bc bne      #0x35d594
0035d5c0 ldr      r3, [r4, #0x1f8]
0035d5c4 mov      r1, r3
0035d5c8 mov      r0, r4
0035d5cc ldr      r3, [r4]
0035d5d0 mov      r7, r4
0035d5d4 mov      lr, pc
0035d5d8 ldr      pc, [r3, #0x5c]
0035d5dc ldr      r6, [r7, #0xfc]!
0035d5e0 b        #0x35d608
0035d5e4 ldr      r0, [r6, #8]
0035d5e8 bl       #0x369160 ; _Z13GetApplicatorPN6glitch5scene18ISceneNodeAnimatorE
0035d5ec subs     r3, r0, #0
0035d5f0 beq      #0x35d614
0035d5f4 ldr      r3, [r3]
0035d5f8 ldr      r1, [r4, #0x1f0]
0035d5fc mov      lr, pc
0035d600 ldr      pc, [r3, #8]
0035d604 ldr      r6, [r6]
0035d608 cmp      r7, r6
0035d60c bne      #0x35d5e4
0035d610 b        #0x35d4e8
0035d614 mov      r5, r3
0035d618 b        #0x35d4e8
0035d61c str      r3, [r4, #0x1f4]
0035d620 pop      {r4, r5, r6, r7, r8, pc}
