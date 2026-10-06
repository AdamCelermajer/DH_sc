_ZN9CSStunned7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3cc0 push     {r4, r5, r6, r7, r8, lr}
003c3cc4 ldr      r4, [pc, #0x150]
003c3cc8 ldr      r8, [pc, #0x150]
003c3ccc ldr      r1, [pc, #0x150]
003c3cd0 add      r4, pc, r4
003c3cd4 ldr      r3, [r4, r8]
003c3cd8 ldr      r6, [r4, r1]
003c3cdc sub      sp, sp, #0x40
003c3ce0 ldr      r3, [r3]
003c3ce4 mov      r0, r6
003c3ce8 mov      r5, r2
003c3cec str      r3, [sp, #0x3c]
003c3cf0 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003c3cf4 ldr      r1, [pc, #0x12c]
003c3cf8 add      r7, sp, #0x24
003c3cfc add      r2, sp, #8
003c3d00 mov      r0, r7
003c3d04 add      r1, pc, r1
003c3d08 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003c3d0c mov      r1, r7
003c3d10 mov      r0, r6
003c3d14 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003c3d18 mov      r0, r7
003c3d1c bl       #0x318254 ; _ZNSsD1Ev
003c3d20 mov      r0, r6
003c3d24 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003c3d28 ldr      r1, [pc, #0xfc]
003c3d2c add      r7, sp, #0xc
003c3d30 add      r2, sp, #4
003c3d34 add      r1, pc, r1
003c3d38 mov      r0, r7
003c3d3c bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003c3d40 mov      r1, r7
003c3d44 mov      r0, r6
003c3d48 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003c3d4c mov      r0, r7
003c3d50 bl       #0x318254 ; _ZNSsD1Ev
003c3d54 movw     r3, #0x2202
003c3d58 str      r3, [r5, #0x520]
003c3d5c ldr      r3, [pc, #0xcc]
003c3d60 mov      r0, r5
003c3d64 add      r6, r5, #0x490
003c3d68 ldr      r3, [r4, r3]
003c3d6c add      r6, r6, #0xc
003c3d70 ldr      r7, [r3]
003c3d74 bl       #0x3a3228 ; _ZNK9Character18GetCharAnimTableIdEv
003c3d78 ldr      r3, [pc, #0xb4]
003c3d7c ldr      r1, [pc, #0xb4]
003c3d80 ldr      r2, [r4, r3]
003c3d84 mov      r3, #0xa0
003c3d88 mla      r3, r3, r0, r7
003c3d8c ldr      r0, [r2, #0x2c]
003c3d90 ldr      r2, [pc, #0xa4]
003c3d94 add      r1, pc, r1
003c3d98 ldr      r7, [r3, #0x8c]
003c3d9c add      r2, pc, r2
003c3da0 bl       #0x4c4bdc ; _ZNK15PyDataConstants11getConstantEPKcS1_
003c3da4 ands     r0, r0, #0x200
003c3da8 bne      #0x3c3e0c
003c3dac add      r1, r0, r7
003c3db0 mov      r0, r6
003c3db4 bl       #0x3cacb0 ; _ZN12CharAnimator8ANIM_SetEi
003c3db8 ldr      r3, [r5]
003c3dbc mov      r0, r5
003c3dc0 mov      lr, pc
003c3dc4 ldr      pc, [r3, #0x28]
003c3dc8 cmp      r0, #0
003c3dcc ldrne    r3, [r5, #0x378]
003c3dd0 movne    r2, #1
003c3dd4 mov      r0, r5
003c3dd8 strbne   r2, [r3, #8]
003c3ddc bl       #0x3bc6b8 ; _ZN9Character14CancelSneakingEv
003c3de0 ldr      r0, [r5, #0x2dc]
003c3de4 cmp      r0, #0
003c3de8 beq      #0x3c3df0
003c3dec bl       #0x46eae0 ; _ZN14PhysicalObject5unpinEv
003c3df0 ldr      r3, [r4, r8]
003c3df4 ldr      r2, [sp, #0x3c]
003c3df8 ldr      r3, [r3]
003c3dfc cmp      r2, r3
003c3e00 bne      #0x3c3e18
003c3e04 add      sp, sp, #0x40
003c3e08 pop      {r4, r5, r6, r7, r8, pc}
003c3e0c mov      r0, r5
003c3e10 bl       #0x3a53e0 ; _ZNK9Character13GetAnimStanceEv
003c3e14 b        #0x3c3dac
003c3e18 bl       #0x30e310
003c3e1c subseq   r0, sp, r0, asr #27
003c3e20 andeq    r4, r0, ip, lsr #1
003c3e24 andeq    r0, r0, r4, lsl #17
003c3e28 subseq   r1, r0, ip, asr #2
003c3e2c subseq   r1, r0, ip, ror r1
003c3e30 andeq    r4, r0, r4, asr #16
003c3e34 strdeq   r3, r4, [r0], -r4
003c3e38 subseq   r0, r0, r4, lsr #28
003c3e3c subseq   r0, r0, ip, lsr #28
_ZN8CSScared7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c45c4 push     {r4, r5, r6, r7, r8, lr}
003c45c8 ldr      r4, [pc, #0x198]
003c45cc ldr      r7, [pc, #0x198]
003c45d0 ldr      r1, [pc, #0x198]
003c45d4 add      r4, pc, r4
003c45d8 ldr      r3, [r4, r7]
003c45dc ldr      r8, [r4, r1]
003c45e0 sub      sp, sp, #0x30
003c45e4 ldr      r3, [r3]
003c45e8 mov      r0, r8
003c45ec mov      r5, r2
003c45f0 str      r3, [sp, #0x2c]
003c45f4 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003c45f8 ldr      r1, [pc, #0x174]
003c45fc add      r6, sp, #0x14
003c4600 add      r2, sp, #0x10
003c4604 mov      r0, r6
003c4608 add      r1, pc, r1
003c460c bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003c4610 mov      r1, r6
003c4614 mov      r0, r8
003c4618 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003c461c mov      r0, r6
003c4620 bl       #0x318254 ; _ZNSsD1Ev
003c4624 mov      r3, #0x2240
003c4628 str      r3, [r5, #0x520]
003c462c ldr      r3, [pc, #0x144]
003c4630 mov      r0, r5
003c4634 add      r6, r5, #0x490
003c4638 ldr      r3, [r4, r3]
003c463c add      r6, r6, #0xc
003c4640 ldr      r8, [r3]
003c4644 bl       #0x3a3228 ; _ZNK9Character18GetCharAnimTableIdEv
003c4648 ldr      r3, [pc, #0x12c]
003c464c ldr      r1, [pc, #0x12c]
003c4650 ldr      r2, [r4, r3]
003c4654 mov      r3, #0xa0
003c4658 mla      r3, r3, r0, r8
003c465c ldr      r0, [r2, #0x2c]
003c4660 ldr      r2, [pc, #0x11c]
003c4664 add      r1, pc, r1
003c4668 ldr      r8, [r3, #0x7c]
003c466c add      r2, pc, r2
003c4670 bl       #0x4c4bdc ; _ZNK15PyDataConstants11getConstantEPKcS1_
003c4674 ands     r0, r0, #0x100
003c4678 bne      #0x3c4758
003c467c add      r1, r0, r8
003c4680 mov      r0, r6
003c4684 bl       #0x3cacb0 ; _ZN12CharAnimator8ANIM_SetEi
003c4688 mov      r3, #0
003c468c movw     r0, #0x270e
003c4690 str      r3, [sp, #0xc]
003c4694 str      r3, [sp, #4]
003c4698 str      r3, [sp, #8]
003c469c bl       #0x3c26a0 ; _ZN6Random9GetRandomEib.clone.1
003c46a0 bl       #0x30e964
003c46a4 movw     r1, #0xb717
003c46a8 movt     r1, #0x38d1
003c46ac bl       #0x30ed6c
003c46b0 movw     r1, #0xb717
003c46b4 movt     r1, #0x3951
003c46b8 bl       #0x30eba4
003c46bc str      r0, [sp, #4]
003c46c0 movw     r0, #0x270e
003c46c4 bl       #0x3c26a0 ; _ZN6Random9GetRandomEib.clone.1
003c46c8 bl       #0x30e964
003c46cc movw     r1, #0xb717
003c46d0 movt     r1, #0x38d1
003c46d4 bl       #0x30ed6c
003c46d8 movw     r1, #0xb717
003c46dc movt     r1, #0x3951
003c46e0 bl       #0x30eba4
003c46e4 str      r0, [sp, #8]
003c46e8 mov      r0, #0x64
003c46ec bl       #0x3c26a0 ; _ZN6Random9GetRandomEib.clone.1
003c46f0 cmp      r0, #0x31
003c46f4 ldrle    r3, [sp, #4]
003c46f8 mov      r0, #0x64
003c46fc addle    r3, r3, #0x80000000
003c4700 strle    r3, [sp, #4]
003c4704 bl       #0x3c26a0 ; _ZN6Random9GetRandomEib.clone.1
003c4708 cmp      r0, #0x31
003c470c ldrle    r3, [sp, #8]
003c4710 add      r1, sp, #4
003c4714 addle    r3, r3, #0x80000000
003c4718 strle    r3, [sp, #8]
003c471c ldr      r0, [r5, #0x378]
003c4720 bl       #0x405374 ; _ZN12v2Controller15Cmd_HeadTowardsERK7Point3DIfE
003c4724 mov      r0, r5
003c4728 bl       #0x3bc6b8 ; _ZN9Character14CancelSneakingEv
003c472c ldr      r0, [r5, #0x2dc]
003c4730 cmp      r0, #0
003c4734 beq      #0x3c473c
003c4738 bl       #0x46eae0 ; _ZN14PhysicalObject5unpinEv
003c473c ldr      r3, [r4, r7]
003c4740 ldr      r2, [sp, #0x2c]
003c4744 ldr      r3, [r3]
003c4748 cmp      r2, r3
003c474c bne      #0x3c4764
003c4750 add      sp, sp, #0x30
003c4754 pop      {r4, r5, r6, r7, r8, pc}
003c4758 mov      r0, r5
003c475c bl       #0x3a53e0 ; _ZNK9Character13GetAnimStanceEv
003c4760 b        #0x3c467c
003c4764 bl       #0x30e310
003c4768 ldrheq   r0, [sp], #-0x4c
003c476c andeq    r4, r0, ip, lsr #1
003c4770 andeq    r0, r0, r4, lsl #17
003c4774 subseq   r0, r0, r8, asr #16
003c4778 andeq    r4, r0, r4, asr #16
003c477c strdeq   r3, r4, [r0], -r4
003c4780 subseq   r0, r0, r4, asr r5
003c4784 subseq   r0, r0, ip, asr r5
_ZN9CSStunned6OnBlurEiP9CharacterP16CharStateMachinei
003c3b4c push     {r4, r5, r6, r7, r8, lr}
003c3b50 ldr      r4, [pc, #0x90]
003c3b54 ldr      r6, [pc, #0x90]
003c3b58 ldr      r1, [pc, #0x90]
003c3b5c add      r4, pc, r4
003c3b60 ldr      r3, [r4, r6]
003c3b64 ldr      r8, [r4, r1]
003c3b68 sub      sp, sp, #0x20
003c3b6c ldr      r3, [r3]
003c3b70 mov      r0, r8
003c3b74 mov      r7, r2
003c3b78 str      r3, [sp, #0x1c]
003c3b7c bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003c3b80 ldr      r1, [pc, #0x6c]
003c3b84 add      r5, sp, #4
003c3b88 mov      r2, sp
003c3b8c add      r1, pc, r1
003c3b90 mov      r0, r5
003c3b94 bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003c3b98 mov      r1, r5
003c3b9c mov      r0, r8
003c3ba0 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003c3ba4 mov      r0, r5
003c3ba8 bl       #0x318254 ; _ZNSsD1Ev
003c3bac ldr      r3, [r7, #0x378]
003c3bb0 mov      r2, #0
003c3bb4 strb     r2, [r3, #8]
003c3bb8 ldr      r0, [r7, #0x2dc]
003c3bbc cmp      r0, r2
003c3bc0 beq      #0x3c3bc8
003c3bc4 bl       #0x46eb20 ; _ZN14PhysicalObject3pinEv
003c3bc8 ldr      r3, [r4, r6]
003c3bcc ldr      r2, [sp, #0x1c]
003c3bd0 ldr      r3, [r3]
003c3bd4 cmp      r2, r3
003c3bd8 bne      #0x3c3be4
003c3bdc add      sp, sp, #0x20
003c3be0 pop      {r4, r5, r6, r7, r8, pc}
003c3be4 bl       #0x30e310
003c3be8 subseq   r0, sp, r4, lsr pc
003c3bec andeq    r4, r0, ip, lsr #1
003c3bf0 andeq    r0, r0, r4, lsl #17
003c3bf4 subseq   r1, r0, r4, asr #5
_ZN8CSScared6OnBlurEiP9CharacterP16CharStateMachinei
003c4834 push     {r4, r5, r6, r7, r8, lr}
003c4838 ldr      r4, [pc, #0x90]
003c483c ldr      r6, [pc, #0x90]
003c4840 ldr      r1, [pc, #0x90]
003c4844 add      r4, pc, r4
003c4848 ldr      r3, [r4, r6]
003c484c ldr      r8, [r4, r1]
003c4850 sub      sp, sp, #0x20
003c4854 ldr      r3, [r3]
003c4858 mov      r0, r8
003c485c mov      r7, r2
003c4860 str      r3, [sp, #0x1c]
003c4864 bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003c4868 ldr      r1, [pc, #0x6c]
003c486c add      r5, sp, #4
003c4870 mov      r2, sp
003c4874 add      r1, pc, r1
003c4878 mov      r0, r5
003c487c bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003c4880 mov      r1, r5
003c4884 mov      r0, r8
003c4888 bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003c488c mov      r0, r5
003c4890 bl       #0x318254 ; _ZNSsD1Ev
003c4894 ldr      r0, [r7, #0x378]
003c4898 mov      r1, #0
003c489c bl       #0x4053d0 ; _ZN12v2Controller15Cmd_HeadTowardsEP10GameObject
003c48a0 ldr      r0, [r7, #0x2dc]
003c48a4 cmp      r0, #0
003c48a8 beq      #0x3c48b0
003c48ac bl       #0x46eb20 ; _ZN14PhysicalObject3pinEv
003c48b0 ldr      r3, [r4, r6]
003c48b4 ldr      r2, [sp, #0x1c]
003c48b8 ldr      r3, [r3]
003c48bc cmp      r2, r3
003c48c0 bne      #0x3c48cc
003c48c4 add      sp, sp, #0x20
003c48c8 pop      {r4, r5, r6, r7, r8, pc}
003c48cc bl       #0x30e310
003c48d0 subseq   r0, sp, ip, asr #4
003c48d4 andeq    r4, r0, ip, lsr #1
003c48d8 andeq    r0, r0, r4, lsl #17
003c48dc ldrsbeq  r0, [r0], #-0x5c
_ZN16CharStateMachine20SM_SetKnockBackStateEbPvb
003c5ea0 push     {r4, r5, r6, r7, r8, lr}
003c5ea4 mov      r4, r0
003c5ea8 ldr      r0, [r0, #4]
003c5eac mov      r7, r1
003c5eb0 mov      r5, r2
003c5eb4 mov      r6, r3
003c5eb8 bl       #0x3a3158 ; _ZNK9Character6IsBossEv
003c5ebc ldr      r8, [pc, #0x118]
003c5ec0 cmp      r0, #0
003c5ec4 add      r8, pc, r8
003c5ec8 beq      #0x3c5ed0
003c5ecc pop      {r4, r5, r6, r7, r8, pc}
003c5ed0 ldr      r0, [r4, #4]
003c5ed4 bl       #0x3a3228 ; _ZNK9Character18GetCharAnimTableIdEv
003c5ed8 cmp      r0, #0
003c5edc blt      #0x3c5ecc
003c5ee0 ldr      r3, [pc, #0xf8]
003c5ee4 ldr      r3, [r8, r3]
003c5ee8 ldr      r3, [r3]
003c5eec cmp      r0, r3
003c5ef0 bge      #0x3c5ecc
003c5ef4 ldr      r3, [pc, #0xe8]
003c5ef8 mov      r2, #0xa0
003c5efc cmp      r7, #0
003c5f00 ldr      r3, [r8, r3]
003c5f04 ldr      r3, [r3]
003c5f08 mla      r3, r2, r0, r3
003c5f0c beq      #0x3c5f68
003c5f10 ldr      r2, [pc, #0xd0]
003c5f14 ldr      r1, [pc, #0xd0]
003c5f18 ldr      r7, [r3, #0x24]
003c5f1c ldr      r0, [r8, r2]
003c5f20 ldr      r2, [pc, #0xc8]
003c5f24 add      r1, pc, r1
003c5f28 ldr      r0, [r0, #0x2c]
003c5f2c add      r2, pc, r2
003c5f30 bl       #0x4c4bdc ; _ZNK15PyDataConstants11getConstantEPKcS1_
003c5f34 ands     r0, r0, #0x800
003c5f38 bne      #0x3c5fc4
003c5f3c add      r7, r0, r7
003c5f40 mov      r3, #0x18
003c5f44 str      r7, [r4, #0x28]
003c5f48 str      r3, [r4, #0x2c]
003c5f4c cmp      r6, #0
003c5f50 bne      #0x3c5fac
003c5f54 mov      r0, r4
003c5f58 mov      r2, r5
003c5f5c movw     r1, #0xc35b
003c5f60 pop      {r4, r5, r6, r7, r8, lr}
003c5f64 b        #0x3c5684
003c5f68 ldr      r2, [pc, #0x78]
003c5f6c ldr      r1, [pc, #0x80]
003c5f70 ldr      r7, [r3, #0x48]
003c5f74 ldr      r0, [r8, r2]
003c5f78 ldr      r2, [pc, #0x78]
003c5f7c add      r1, pc, r1
003c5f80 ldr      r0, [r0, #0x2c]
003c5f84 add      r2, pc, r2
003c5f88 bl       #0x4c4bdc ; _ZNK15PyDataConstants11getConstantEPKcS1_
003c5f8c ands     r0, r0, #0x400
003c5f90 bne      #0x3c5fd0
003c5f94 ldr      r3, [r4, #0x2c]
003c5f98 add      r7, r0, r7
003c5f9c str      r7, [r4, #0x28]
003c5fa0 bic      r3, r3, #0x18
003c5fa4 str      r3, [r4, #0x2c]
003c5fa8 b        #0x3c5f4c
003c5fac mov      r0, r4
003c5fb0 mov      r3, r5
003c5fb4 mov      r1, #0xa
003c5fb8 movw     r2, #0xc35b
003c5fbc pop      {r4, r5, r6, r7, r8, lr}
003c5fc0 b        #0x3c1938
003c5fc4 ldr      r0, [r4, #4]
003c5fc8 bl       #0x3a53e0 ; _ZNK9Character13GetAnimStanceEv
003c5fcc b        #0x3c5f3c
003c5fd0 ldr      r0, [r4, #4]
003c5fd4 bl       #0x3a53e0 ; _ZNK9Character13GetAnimStanceEv
003c5fd8 b        #0x3c5f94
003c5fdc subseq   lr, ip, ip, asr #23
003c5fe0 andeq    r2, r0, r0, asr #17
003c5fe4 andeq    r4, r0, r4, asr #16
003c5fe8 strdeq   r3, r4, [r0], -r4
003c5fec umaaleq  lr, pc, r4, ip
003c5ff0 umaaleq  lr, pc, ip, ip
003c5ff4 subeq    lr, pc, ip, lsr ip
003c5ff8 subeq    lr, pc, r4, asr #24
_ZN14CharProperties16PROPS_DebuffSlowEj
003e2a5c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e2a60 mov      r5, r0
003e2a64 sub      sp, sp, #0x14
003e2a68 ldr      r0, [r0, #4]
003e2a6c mov      sl, r1
003e2a70 bl       #0x3a3158 ; _ZNK9Character6IsBossEv
003e2a74 ldr      r7, [pc, #0x130]
003e2a78 subs     r4, r0, #0
003e2a7c add      r7, pc, r7
003e2a80 bne      #0x3e2b9c
003e2a84 cmp      sl, #0
003e2a88 beq      #0x3e2b9c
003e2a8c ldr      r3, [pc, #0x11c]
003e2a90 ldr      r3, [r7, r3]
003e2a94 ldr      r8, [r3]
003e2a98 cmp      r8, #0
003e2a9c beq      #0x3e2b9c
003e2aa0 ldr      r3, [pc, #0x10c]
003e2aa4 ldr      fp, [pc, #0x10c]
003e2aa8 ldr      r3, [r7, r3]
003e2aac add      fp, pc, fp
003e2ab0 ldr      sb, [r3]
003e2ab4 b        #0x3e2ac4
003e2ab8 add      r4, r4, #1
003e2abc cmp      r4, r8
003e2ac0 beq      #0x3e2b9c
003e2ac4 ldr      r1, [sb, r4, lsl #2]
003e2ac8 mov      r0, fp
003e2acc bl       #0x30e31c
003e2ad0 cmp      r0, #0
003e2ad4 bne      #0x3e2ab8
003e2ad8 cmn      r4, #1
003e2adc beq      #0x3e2b9c
003e2ae0 ldr      r3, [pc, #0xd4]
003e2ae4 ldr      r3, [r7, r3]
003e2ae8 ldr      r8, [r3]
003e2aec cmp      r8, #0
003e2af0 beq      #0x3e2ba4
003e2af4 ldr      r3, [pc, #0xc4]
003e2af8 ldr      sb, [pc, #0xc4]
003e2afc mov      r6, r0
003e2b00 ldr      r3, [r7, r3]
003e2b04 add      sb, pc, sb
003e2b08 ldr      r7, [r3]
003e2b0c b        #0x3e2b1c
003e2b10 add      r6, r6, #1
003e2b14 cmp      r6, r8
003e2b18 beq      #0x3e2ba4
003e2b1c ldr      r1, [r7, r6, lsl #2]
003e2b20 mov      r0, sb
003e2b24 bl       #0x30e31c
003e2b28 cmp      r0, #0
003e2b2c bne      #0x3e2b10
003e2b30 mov      lr, r6
003e2b34 ldr      ip, [pc, #0x8c]
003e2b38 mov      r2, sl
003e2b3c mov      r0, r5
003e2b40 add      ip, pc, ip
003e2b44 mov      r1, r4
003e2b48 mov      r3, #1
003e2b4c mov      r6, #0
003e2b50 stm      sp, {r6, lr}
003e2b54 str      ip, [sp, #8]
003e2b58 bl       #0x3e232c ; _ZN14CharProperties13PROPS_AddBuffEijijiPKc
003e2b5c subs     r2, r0, #0
003e2b60 beq      #0x3e2b9c
003e2b64 mov      r1, r4
003e2b68 mov      r0, r5
003e2b6c bl       #0x3df314 ; _ZN14CharProperties23PROPS_ApplyClassToSheetEiPN7Structs19CharacterPropertiesE
003e2b70 mov      r0, r5
003e2b74 mov      r1, #0x30
003e2b78 bl       #0x3dfe60 ; _ZN14CharProperties14RecalcPropertyEi
003e2b7c mov      r0, r5
003e2b80 mov      r1, #0x2f
003e2b84 bl       #0x3dfe60 ; _ZN14CharProperties14RecalcPropertyEi
003e2b88 mov      r0, r5
003e2b8c mov      r1, #0x2e
003e2b90 add      sp, sp, #0x14
003e2b94 pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e2b98 b        #0x3dfe60
003e2b9c add      sp, sp, #0x14
003e2ba0 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e2ba4 mvn      lr, #0
003e2ba8 b        #0x3e2b34
003e2bac subseq   r2, fp, r4, lsl r0
003e2bb0 andeq    r3, r0, r8, ror #10
003e2bb4 muleq    r0, r0, sl
003e2bb8 subeq    r3, lr, r4, asr #7
003e2bbc andeq    r0, r0, r4, asr #13
003e2bc0 muleq    r0, r4, r2
003e2bc4 subeq    r3, lr, ip, ror r3
003e2bc8 subeq    r3, lr, r8, asr r3
_ZN12v2Controller15Cmd_HeadTowardsEP10GameObject
004053d0 push     {r4, lr}
004053d4 ldrb     r2, [r0, #9]
004053d8 ldr      r3, [pc, #0x44]
004053dc cmp      r2, #0
004053e0 add      r3, pc, r3
004053e4 bne      #0x40540c
004053e8 ldr      r2, [pc, #0x38]
004053ec ldr      r3, [r3, r2]
004053f0 ldrb     r3, [r3]
004053f4 cmp      r3, #0
004053f8 beq      #0x405400
004053fc pop      {r4, pc}
00405400 ldrb     r3, [r0, #8]
00405404 cmp      r3, #0
00405408 bne      #0x4053fc
0040540c ldr      r3, [r0, #4]
00405410 mov      r0, r3
00405414 ldr      r3, [r3]
00405418 mov      lr, pc
0040541c ldr      pc, [r3, #0x20]
00405420 pop      {r4, pc}
00405424 ldrheq   pc, [r8], #-0x60
00405428 andeq    r3, r0, r0, asr r6
_ZN9Character16Ctrl_HeadTowardsEP10GameObject
003ad8d4 push     {r4, r5, r6, r7, r8, sl, lr}
003ad8d8 sub      sp, sp, #0x14
003ad8dc ldr      r3, [r0]
003ad8e0 mov      r4, r0
003ad8e4 mov      r5, r1
003ad8e8 mov      lr, pc
003ad8ec ldr      pc, [r3, #0x54]
003ad8f0 ldr      r2, [pc, #0xa4]
003ad8f4 cmp      r0, #0
003ad8f8 add      r2, pc, r2
003ad8fc bne      #0x3ad96c
003ad900 cmp      r5, #0
003ad904 beq      #0x3ad974
003ad908 ldr      r3, [r4]
003ad90c mov      r0, r5
003ad910 ldr      r7, [r3, #0xdc]
003ad914 bl       #0x3935dc ; _ZNK10GameObject17GetTargetPositionEv
003ad918 mov      r5, r0
003ad91c mov      r0, r4
003ad920 bl       #0x3935dc ; _ZNK10GameObject17GetTargetPositionEv
003ad924 ldr      r1, [r0, #4]
003ad928 mov      r6, r0
003ad92c ldr      r0, [r5, #4]
003ad930 bl       #0x30e3ac
003ad934 ldr      r1, [r6, #8]
003ad938 mov      sl, r0
003ad93c ldr      r0, [r5, #8]
003ad940 bl       #0x30e3ac
003ad944 ldr      r1, [r6]
003ad948 mov      r8, r0
003ad94c ldr      r0, [r5]
003ad950 bl       #0x30e3ac
003ad954 str      sl, [sp, #8]
003ad958 str      r0, [sp, #4]
003ad95c str      r8, [sp, #0xc]
003ad960 mov      r0, r4
003ad964 add      r1, sp, #4
003ad968 blx      r7
003ad96c add      sp, sp, #0x14
003ad970 pop      {r4, r5, r6, r7, r8, sl, pc}
003ad974 ldrb     r3, [r4, #0x1b5]
003ad978 cmp      r3, #0
003ad97c beq      #0x3ad96c
003ad980 ldr      r1, [pc, #0x18]
003ad984 mov      r0, r4
003ad988 ldr      r3, [r4]
003ad98c ldr      r1, [r2, r1]
003ad990 mov      lr, pc
003ad994 ldr      pc, [r3, #0xdc]
003ad998 b        #0x3ad96c
_ZNK10ObjectBase17IsRemotelyUpdatedEv
0033dd10 ldr      r3, [r0, #0x110]
0033dd14 cmn      r3, #1
0033dd18 movne    r0, #1
0033dd1c ldrbeq   r0, [r0, #0x118]
0033dd20 bx       lr
_ZN12CharAnimator9ANIM_StopEv
003c9924 push     {r4, lr}
003c9928 ldr      r3, [r0, #4]
003c992c mov      r2, #0
003c9930 str      r2, [r0, #0x2c]
003c9934 ldr      r3, [r3, #0x2d8]
003c9938 mov      r4, r0
003c993c cmp      r3, r2
003c9940 beq      #0x3c9968
003c9944 ldr      r3, [r3, #0x38]
003c9948 mov      r1, #1
003c994c mov      r0, r3
003c9950 ldr      r3, [r3]
003c9954 mov      lr, pc
003c9958 ldr      pc, [r3, #0x24]
003c995c ldrb     r2, [r4, #0x48]
003c9960 cmp      r2, #0
003c9964 beq      #0x3c996c
003c9968 pop      {r4, pc}
003c996c ldr      r0, [r4, #4]
003c9970 mov      r3, #1
003c9974 mov      r1, #0x22
003c9978 strb     r3, [r4, #0x48]
003c997c pop      {r4, lr}
003c9980 b        #0x3a4d5c
_ZN12CharAnimator13ANIM_StopLoopEb
003c948c ldrb     r3, [r0, #0x48]
003c9490 cmp      r3, #0
003c9494 bxne     lr
003c9498 ldr      r2, [r0, #0x2c]
003c949c cmp      r1, #0
003c94a0 mov      r1, #0xc
003c94a4 mla      r2, r1, r2, r0
003c94a8 str      r3, [r2, #0xc]
003c94ac movne    r3, #1
003c94b0 strbne   r3, [r0, #0x4a]
003c94b4 bx       lr
_ZN6CharAI13OnStunExpiredEv
003d0c34 push     {r4, lr}
003d0c38 ldr      r3, [r0, #0x1c]
003d0c3c cmp      r3, #0
003d0c40 beq      #0x3d0c54
003d0c44 mov      r0, r3
003d0c48 ldr      r3, [r3]
003d0c4c mov      lr, pc
003d0c50 ldr      pc, [r3, #0x84]
003d0c54 pop      {r4, pc}
_ZN6CharAI13OnFearExpiredEv
003d0c58 push     {r4, lr}
003d0c5c ldr      r3, [r0, #0x1c]
003d0c60 cmp      r3, #0
003d0c64 beq      #0x3d0c78
003d0c68 mov      r0, r3
003d0c6c ldr      r3, [r3]
003d0c70 mov      lr, pc
003d0c74 ldr      pc, [r3, #0x88]
003d0c78 pop      {r4, pc}
_ZN10AISDefault13OnStunExpiredEv
003dbee0 bx       lr
_ZN10AISDefault13OnFearExpiredEv
003dbee4 bx       lr
_ZN14PhysicalObject9setFilterEsttb
0046ece8 push     {r4, r5, r6, r7, r8, lr}
0046ecec mov      r4, r0
0046ecf0 ldr      r0, [r0, #0x18]
0046ecf4 mov      r5, r1
0046ecf8 mov      r6, r2
0046ecfc cmp      r0, #0
0046ed00 mov      r7, r3
0046ed04 ldrb     r8, [sp, #0x18]
0046ed08 beq      #0x46ed28
0046ed0c strh     r1, [r0, #0x26]
0046ed10 strh     r3, [r0, #0x24]
0046ed14 strh     r2, [r0, #0x22]
0046ed18 ldr      r3, [r4, #4]
0046ed1c ldr      r1, [r4, #0x18]
0046ed20 ldr      r0, [r3, #0x10]
0046ed24 bl       #0x7e7afc ; _ZN7b2World8RefilterEP7b2Shape
0046ed28 ldr      r3, [r4, #0x1c]
0046ed2c cmp      r3, #0
0046ed30 beq      #0x46ed58
0046ed34 cmp      r8, #0
0046ed38 beq      #0x46ed58
0046ed3c strh     r5, [r3, #0x26]
0046ed40 strh     r7, [r3, #0x24]
0046ed44 strh     r6, [r3, #0x22]
0046ed48 ldr      r3, [r4, #4]
0046ed4c ldr      r1, [r4, #0x1c]
0046ed50 ldr      r0, [r3, #0x10]
0046ed54 bl       #0x7e7afc ; _ZN7b2World8RefilterEP7b2Shape
0046ed58 mov      r3, #0
0046ed5c strb     r3, [r4, #0x26]
0046ed60 pop      {r4, r5, r6, r7, r8, pc}
_ZN14PhysicalObject11resetFilterEv
0046ec6c push     {r4, lr}
0046ec70 ldr      r3, [r0, #0x18]
0046ec74 mov      r4, r0
0046ec78 cmp      r3, #0
0046ec7c beq      #0x46eca8
0046ec80 ldrh     r2, [r0, #0x20]
0046ec84 strh     r2, [r3, #0x22]
0046ec88 ldrh     r2, [r0, #0x22]
0046ec8c strh     r2, [r3, #0x24]
0046ec90 ldrh     r2, [r0, #0x24]
0046ec94 strh     r2, [r3, #0x26]
0046ec98 ldr      r3, [r0, #4]
0046ec9c ldr      r1, [r0, #0x18]
0046eca0 ldr      r0, [r3, #0x10]
0046eca4 bl       #0x7e7afc ; _ZN7b2World8RefilterEP7b2Shape
0046eca8 ldr      r3, [r4, #0x1c]
0046ecac cmp      r3, #0
0046ecb0 beq      #0x46ecdc
0046ecb4 ldrh     r2, [r4, #0x20]
0046ecb8 strh     r2, [r3, #0x22]
0046ecbc ldrh     r2, [r4, #0x22]
0046ecc0 strh     r2, [r3, #0x24]
0046ecc4 ldrh     r2, [r4, #0x24]
0046ecc8 strh     r2, [r3, #0x26]
0046eccc ldr      r3, [r4, #4]
0046ecd0 ldr      r1, [r4, #0x1c]
0046ecd4 ldr      r0, [r3, #0x10]
0046ecd8 bl       #0x7e7afc ; _ZN7b2World8RefilterEP7b2Shape
0046ecdc mov      r3, #0
0046ece0 strb     r3, [r4, #0x26]
0046ece4 pop      {r4, pc}
_ZN6Random9GetRandomEib.clone.1
003c26a0 push     {r4, lr}
003c26a4 ldr      r4, [pc, #0x7c]
003c26a8 cmp      r0, #0
003c26ac add      r4, pc, r4
003c26b0 beq      #0x3c2710
003c26b4 ldr      r2, [pc, #0x70]
003c26b8 mov      r1, r0
003c26bc movw     r0, #0xe6ab
003c26c0 ldr      r2, [r4, r2]
003c26c4 movw     r3, #0xdb17
003c26c8 movt     r3, #0x2b52
003c26cc ldr      lr, [r2]
003c26d0 movw     ip, #0xf26b
003c26d4 movt     ip, #0xda
003c26d8 mul      r0, r0, lr
003c26dc add      r0, r0, #0x2b000
003c26e0 add      r0, r0, #0x3fc
003c26e4 add      r0, r0, #1
003c26e8 umull    lr, r3, r3, r0
003c26ec rsb      lr, r3, r0
003c26f0 add      r3, r3, lr, lsr #1
003c26f4 lsr      r3, r3, #0x17
003c26f8 mls      r3, ip, r3, r0
003c26fc mov      r0, r3
003c2700 str      r3, [r2]
003c2704 bl       #0x30eb2c
003c2708 eor      r0, r1, r1, asr #31
003c270c sub      r0, r0, r1, asr #31
003c2710 ldr      r3, [pc, #0x18]
003c2714 ldr      r3, [r4, r3]
003c2718 ldr      r2, [r3]
003c271c add      r2, r2, #1
003c2720 str      r2, [r3]
003c2724 pop      {r4, pc}
003c2728 subseq   r2, sp, r4, ror #7
003c272c muleq    r0, r4, ip
003c2730 andeq    r1, r0, r8, lsl #1
