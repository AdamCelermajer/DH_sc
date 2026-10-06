_ZN16CharStateMachineC1Ev
003c1ac4 ldr      ip, [pc, #0x84]
003c1ac8 str      r4, [sp, #-4]!
003c1acc ldr      r4, [pc, #0x80]
003c1ad0 add      ip, pc, ip
003c1ad4 mov      r2, #0
003c1ad8 ldr      r4, [ip, r4]
003c1adc mov      r1, r0
003c1ae0 str      r2, [r0, #4]
003c1ae4 add      r4, r4, #8
003c1ae8 str      r4, [r0]
003c1aec str      r2, [r0, #0xc]
003c1af0 mvn      r4, #0
003c1af4 strb     r2, [r1, #8]!
003c1af8 str      r1, [r0, #0x14]
003c1afc str      r4, [r0, #0x28]
003c1b00 str      r2, [r0, #0x5c]
003c1b04 str      r1, [r0, #0x10]
003c1b08 str      r2, [r0, #0x18]
003c1b0c str      r2, [r0, #0x20]
003c1b10 str      r2, [r0, #0x24]
003c1b14 str      r2, [r0, #0x60]
003c1b18 str      r2, [r0, #0x2c]
003c1b1c str      r2, [r0, #0x30]
003c1b20 str      r2, [r0, #0x34]
003c1b24 str      r2, [r0, #0x38]
003c1b28 str      r2, [r0, #0x3c]
003c1b2c str      r2, [r0, #0x40]
003c1b30 str      r2, [r0, #0x44]
003c1b34 str      r2, [r0, #0x48]
003c1b38 str      r2, [r0, #0x4c]
003c1b3c str      r2, [r0, #0x50]
003c1b40 str      r2, [r0, #0x54]
003c1b44 str      r2, [r0, #0x58]
003c1b48 ldm      sp!, {r4}
003c1b4c bx       lr
003c1b50 subseq   r2, sp, r0, asr #31
003c1b54 strdeq   r2, r3, [r0], -r8
_ZN14CharProperties13PROPS_AddBuffEijijiPKc
003e232c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e2330 mov      r8, r0
003e2334 ldr      ip, [r0, #0xe1c]
003e2338 ldr      r0, [pc, #0x3d4]
003e233c sub      sp, sp, #0x94
003e2340 add      fp, r8, #0xe10
003e2344 add      r0, pc, r0
003e2348 cmp      ip, #0
003e234c str      r0, [sp, #0xc]
003e2350 str      r1, [sp, #0x24]
003e2354 str      r2, [sp, #0x10]
003e2358 mov      r7, r3
003e235c add      fp, fp, #8
003e2360 ldr      sl, [sp, #0xb8]
003e2364 beq      #0x3e24d0
003e2368 mov      r2, fp
003e236c b        #0x3e2374
003e2370 mov      ip, r3
003e2374 ldr      r3, [ip, #0x10]
003e2378 cmp      r1, r3
003e237c ldrgt    r3, [ip, #0xc]
003e2380 ldrle    r3, [ip, #8]
003e2384 movgt    ip, r2
003e2388 mov      r2, ip
003e238c cmp      r3, #0
003e2390 bne      #0x3e2370
003e2394 cmp      fp, ip
003e2398 beq      #0x3e23a8
003e239c ldr      r3, [ip, #0x10]
003e23a0 cmp      r1, r3
003e23a4 blt      #0x3e24d0
003e23a8 cmp      r7, #0
003e23ac movle    r7, #0x80
003e23b0 mov      r5, #0
003e23b4 cmp      fp, ip
003e23b8 str      r5, [sp, #0x8c]
003e23bc beq      #0x3e24ec
003e23c0 add      lr, sp, #0x6c
003e23c4 add      sb, ip, #0x34
003e23c8 ldm      sb, {r0, r1, r2, r3}
003e23cc stm      lr, {r0, r1, r2, r3}
003e23d0 add      r0, ip, #0x44
003e23d4 mov      r1, lr
003e23d8 bl       #0x3de870 ; _ZNKSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE11_M_subtractERKS4_
003e23dc cmp      r7, r0
003e23e0 beq      #0x3e261c
003e23e4 ldr      r3, [sp, #0x8c]
003e23e8 cmp      r3, #0
003e23ec beq      #0x3e24ec
003e23f0 ldr      r0, [r8, #4]
003e23f4 ldr      r1, [r3, #0x388]
003e23f8 add      r0, r0, #0x3b4
003e23fc bl       #0x3db2d8 ; _ZN10CharTimers8TMR_StopEj
003e2400 ldr      r3, [sp, #0x8c]
003e2404 mvn      r2, #0
003e2408 str      r2, [r3, #0x388]
003e240c ldr      r2, [sp, #0x10]
003e2410 cmp      r2, #0
003e2414 ldreq    r1, [sp, #0x8c]
003e2418 bne      #0x3e25d0
003e241c ldr      lr, [r1, #0x390]
003e2420 ldr      r3, [lr, #4]
003e2424 cmp      r3, #0
003e2428 beq      #0x3e24ac
003e242c add      ip, sp, #0x3c
003e2430 add      r3, lr, #0x20
003e2434 ldm      r3, {r0, r1, r2, r3}
003e2438 stm      ip, {r0, r1, r2, r3}
003e243c add      r0, lr, #0x30
003e2440 mov      r1, ip
003e2444 bl       #0x3de870 ; _ZNKSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE11_M_subtractERKS4_
003e2448 cmp      r0, #1
003e244c bls      #0x3e24a8
003e2450 ldr      r3, [sp, #0x8c]
003e2454 ldr      r3, [r3, #0x390]
003e2458 ldr      r0, [r3, #4]
003e245c bl       #0x492550 ; _ZN10AnimatedFX17GetAnimControllerEv
003e2460 ldr      r2, [sp, #0x8c]
003e2464 ldr      r3, [r0]
003e2468 add      ip, sp, #0x2c
003e246c ldr      lr, [r2, #0x390]
003e2470 ldr      r5, [r3, #0x1c]
003e2474 mov      r4, r0
003e2478 add      r3, lr, #0x20
003e247c ldm      r3, {r0, r1, r2, r3}
003e2480 stm      ip, {r0, r1, r2, r3}
003e2484 mov      r1, ip
003e2488 add      r0, lr, #0x30
003e248c bl       #0x3de870 ; _ZNKSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE11_M_subtractERKS4_
003e2490 mov      r3, #0
003e2494 sub      r1, r0, #1
003e2498 str      r3, [sp]
003e249c mov      r0, r4
003e24a0 mov      r2, #1
003e24a4 blx      r5
003e24a8 ldr      r1, [sp, #0x8c]
003e24ac mov      r0, r8
003e24b0 bl       #0x3def84 ; _ZN14CharProperties16PROPS_ResetSheetEPN7Structs19CharacterPropertiesE
003e24b4 ldr      r3, [sp, #0x8c]
003e24b8 lsl      sl, sl, #8
003e24bc str      sl, [r3, #0x2b4]
003e24c0 ldr      r0, [sp, #0x8c]
003e24c4 str      sl, [r8, #0xd48]
003e24c8 add      sp, sp, #0x94
003e24cc pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e24d0 cmp      r7, #0
003e24d4 mov      ip, fp
003e24d8 movle    r7, #0x80
003e24dc mov      r5, #0
003e24e0 cmp      fp, ip
003e24e4 str      r5, [sp, #0x8c]
003e24e8 bne      #0x3e23c0
003e24ec add      r1, sp, #0x24
003e24f0 mov      r0, fp
003e24f4 bl       #0x3e209c ; _ZNSt3mapIiN14CharProperties8BuffDeclESt4lessIiESaISt4pairIKiS1_EEEixIiEERS1_RKT_
003e24f8 ldr      r3, [sp, #0x24]
003e24fc mov      r5, r0
003e2500 mov      r4, r0
003e2504 ldr      r0, [sp, #0xc0]
003e2508 str      r3, [r5], #8
003e250c bl       #0x30de54
003e2510 ldr      r1, [sp, #0xc0]
003e2514 add      r2, r1, r0
003e2518 mov      r0, r5
003e251c bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
003e2520 ldr      r2, [r4, #4]
003e2524 ldr      ip, [sp, #0xbc]
003e2528 rsbs     r3, r2, #1
003e252c movlo    r3, #0
003e2530 cmn      ip, #1
003e2534 moveq    r3, #0
003e2538 cmp      r3, #0
003e253c bne      #0x3e26e4
003e2540 add      ip, sp, #0x4c
003e2544 add      r5, r4, #0x20
003e2548 ldm      r5, {r0, r1, r2, r3}
003e254c stm      ip, {r0, r1, r2, r3}
003e2550 mov      r1, ip
003e2554 add      r0, r4, #0x30
003e2558 bl       #0x3de870 ; _ZNKSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE11_M_subtractERKS4_
003e255c cmp      r7, r0
003e2560 bls      #0x3e2614
003e2564 mov      r1, #0
003e2568 mov      r0, #0x394
003e256c bl       #0x310570 ; _Znwj15MemoryHintState
003e2570 str      r4, [r0, #0x390]
003e2574 ldr      r1, [sp, #0xc]
003e2578 ldr      r3, [pc, #0x198]
003e257c mvn      r2, #0
003e2580 ldr      r3, [r1, r3]
003e2584 add      r3, r3, #8
003e2588 str      r3, [r0]
003e258c str      r0, [sp, #0x8c]
003e2590 str      r8, [r0, #0x38c]
003e2594 ldr      r3, [sp, #0x8c]
003e2598 str      sl, [r3, #0x384]
003e259c ldr      r3, [sp, #0x8c]
003e25a0 str      r2, [r3, #0x388]
003e25a4 ldr      r2, [r4, #0x38]
003e25a8 ldr      r3, [r4, #0x30]
003e25ac sub      r2, r2, #4
003e25b0 cmp      r3, r2
003e25b4 beq      #0x3e2704
003e25b8 ldr      r2, [sp, #0x8c]
003e25bc str      r2, [r3]
003e25c0 ldr      r3, [r4, #0x30]
003e25c4 add      r3, r3, #4
003e25c8 str      r3, [r4, #0x30]
003e25cc b        #0x3e240c
003e25d0 ldr      r0, [r8, #4]
003e25d4 ldr      r4, [sp, #0x8c]
003e25d8 mov      r1, r2
003e25dc mov      r3, #0x36
003e25e0 add      r0, r0, #0x3b4
003e25e4 mov      r2, #0
003e25e8 str      r4, [sp]
003e25ec bl       #0x3dbe24 ; _ZN10CharTimers9TMR_StartEjiiPv
003e25f0 str      r0, [r4, #0x388]
003e25f4 ldr      r1, [sp, #0x8c]
003e25f8 ldr      r3, [r1, #0x388]
003e25fc cmn      r3, #1
003e2600 bne      #0x3e241c
003e2604 mov      r2, r1
003e2608 mov      r0, r8
003e260c ldr      r1, [sp, #0x24]
003e2610 bl       #0x3e101c ; _ZN14CharProperties13PROPS_DelBuffEiPN7Structs19CharacterPropertiesE
003e2614 mov      r0, #0
003e2618 b        #0x3e24c8
003e261c add      r1, sp, #0x88
003e2620 add      r2, sp, #0x84
003e2624 add      r3, sp, #0x80
003e2628 add      ip, sp, #0x7c
003e262c add      r4, sp, #0x5c
003e2630 str      r1, [sp, #0x14]
003e2634 str      r2, [sp, #0x18]
003e2638 str      r3, [sp, #0x1c]
003e263c str      ip, [sp, #0x20]
003e2640 b        #0x3e2650
003e2644 add      r5, r5, #1
003e2648 cmp      r7, r5
003e264c ble      #0x3e23e4
003e2650 ldm      sb, {r0, r1, r2, r3}
003e2654 stm      r4, {r0, r1, r2, r3}
003e2658 mov      r1, r5
003e265c mov      r0, r4
003e2660 bl       #0x3de8b4 ; _ZNSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE10_M_advanceEi
003e2664 ldr      r3, [sp, #0x5c]
003e2668 ldr      r6, [r3]
003e266c ldr      r3, [r6, #0x384]
003e2670 cmp      r3, sl
003e2674 strlo    r6, [sp, #0x8c]
003e2678 strlo    sl, [r6, #0x384]
003e267c blo      #0x3e2644
003e2680 bne      #0x3e2644
003e2684 ldr      r1, [r6, #0x388]
003e2688 ldr      r2, [sp, #0x14]
003e268c ldr      r3, [sp, #0x18]
003e2690 cmn      r1, #1
003e2694 beq      #0x3e26dc
003e2698 ldr      r1, [sp, #0x8c]
003e269c cmp      r1, #0
003e26a0 beq      #0x3e26dc
003e26a4 ldr      r0, [r8, #4]
003e26a8 ldr      r1, [r1, #0x388]
003e26ac add      r0, r0, #0x3b4
003e26b0 bl       #0x3db344 ; _ZNK10CharTimers12TMR_TimeLeftEjRjS0_
003e26b4 ldr      r0, [r8, #4]
003e26b8 ldr      r2, [sp, #0x1c]
003e26bc ldr      r3, [sp, #0x20]
003e26c0 ldr      r1, [r6, #0x388]
003e26c4 add      r0, r0, #0x3b4
003e26c8 bl       #0x3db344 ; _ZNK10CharTimers12TMR_TimeLeftEjRjS0_
003e26cc ldr      r3, [sp, #0x80]
003e26d0 ldr      r2, [sp, #0x88]
003e26d4 cmp      r2, r3
003e26d8 bhs      #0x3e2644
003e26dc str      r6, [sp, #0x8c]
003e26e0 b        #0x3e2644
003e26e4 mov      r1, ip
003e26e8 ldr      r3, [pc, #0x2c]
003e26ec ldr      ip, [sp, #0xc]
003e26f0 ldr      r2, [r8, #4]
003e26f4 ldr      r0, [ip, r3]
003e26f8 bl       #0x495430 ; _ZN15VisualFXManager10GrabAnimFXEiP10GameObject
003e26fc str      r0, [r4, #4]
003e2700 b        #0x3e2540
003e2704 mov      r0, r5
003e2708 add      r1, sp, #0x8c
003e270c bl       #0x3e21a8 ; _ZNSt5dequeIPN14CharProperties8BuffInstESaIS2_EE18_M_push_back_aux_vERKS2_
003e2710 b        #0x3e240c
003e2714 subseq   r2, fp, ip, asr #14
003e2718 andeq    r2, r0, ip, lsl #19
003e271c andeq    r1, r0, r8, lsl #22
_ZNK6CharAI11AI_HasAggroEv
003d49f0 ldr      r0, [r0, #0x8c]
003d49f4 subs     r0, r0, #0
003d49f8 movne    r0, #1
003d49fc bx       lr
_ZNK6CharAI12AI_IsAggroedEv
003d4a00 ldr      r0, [r0, #0xa4]
003d4a04 subs     r0, r0, #0
003d4a08 movne    r0, #1
003d4a0c bx       lr
