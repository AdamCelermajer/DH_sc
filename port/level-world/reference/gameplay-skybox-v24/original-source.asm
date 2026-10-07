_ZN12SceneManager18AddSkyBoxSceneNodeEPKcS1_
00359a38 push     {r4, r5, r6, r7, lr}
00359a3c ldrb     r3, [r1]
00359a40 sub      sp, sp, #0x14
00359a44 mov      r4, r0
00359a48 cmp      r3, #0
00359a4c bne      #0x359a58
00359a50 add      sp, sp, #0x14
00359a54 pop      {r4, r5, r6, r7, pc}
00359a58 mov      ip, #1
00359a5c mov      r3, #0
00359a60 str      ip, [sp]
00359a64 bl       #0x3596f8 ; _ZN12SceneManager9LoadSceneEPKcS1_bb
00359a68 subs     r5, r0, #0
00359a6c beq      #0x359a50
00359a70 movw     r1, #0x6164
00359a74 movt     r1, #0x6d65
00359a78 ldr      r3, [r4]
00359a7c mov      r0, r4
00359a80 mov      r2, r5
00359a84 mov      lr, pc
00359a88 ldr      pc, [r3, #0x1c]
00359a8c subs     r7, r0, #0
00359a90 beq      #0x359a50
00359a94 ldr      r3, [r4, #0x438]
00359a98 cmp      r3, #0
00359a9c beq      #0x359acc
00359aa0 mov      r0, r3
00359aa4 ldr      r3, [r3]
00359aa8 mov      lr, pc
00359aac ldr      pc, [r3, #0x68]
00359ab0 ldr      r3, [r4, #0x438]
00359ab4 ldr      r2, [r3]
00359ab8 ldr      r0, [r2, #-0xc]
00359abc add      r0, r3, r0
00359ac0 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00359ac4 mov      r3, #0
00359ac8 str      r3, [r4, #0x438]
00359acc add      r0, sp, #8
00359ad0 ldr      r3, [r7]
00359ad4 mov      r1, r7
00359ad8 mov      lr, pc
00359adc ldr      pc, [r3, #0xf8]
00359ae0 ldr      r0, [sp, #8]
00359ae4 cmp      r0, #0
00359ae8 str      r0, [sp, #0xc]
00359aec beq      #0x359b0c
00359af0 ldr      r3, [r0, #4]
00359af4 add      r3, r3, #1
00359af8 str      r3, [r0, #4]
00359afc ldr      r0, [sp, #8]
00359b00 cmp      r0, #0
00359b04 beq      #0x359b0c
00359b08 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00359b0c mov      r1, #0
00359b10 mov      r0, #0x148
00359b14 bl       #0x5341ac ; _ZnwjN6glitch6memory13E_MEMORY_HINTE
00359b18 mov      r2, r7
00359b1c add      r1, sp, #0xc
00359b20 mov      r6, r0
00359b24 bl       #0x362810 ; _ZN19SkyBoxMeshSceneNodeC1ERKN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPNS2_5scene14IMeshSceneNodeE
00359b28 ldr      r3, [r4, #4]
00359b2c str      r6, [r4, #0x438]
00359b30 mov      r1, r6
00359b34 mov      r0, r3
00359b38 ldr      r3, [r3]
00359b3c mov      lr, pc
00359b40 ldr      pc, [r3, #0x5c]
00359b44 ldr      r3, [r5]
00359b48 ldr      r0, [r3, #-0xc]
00359b4c add      r0, r5, r0
00359b50 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00359b54 ldr      r0, [sp, #0xc]
00359b58 cmp      r0, #0
00359b5c beq      #0x359a50
00359b60 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00359b64 b        #0x359a50
_ZN19SkyBoxMeshSceneNodeD0Ev
0036278c push     {r4, lr}
00362790 mov      r4, r0
00362794 bl       #0x362718 ; _ZN19SkyBoxMeshSceneNodeD1Ev
00362798 mov      r0, r4
0036279c bl       #0x310440 ; _Z10CustomFreePv
003627a0 mov      r0, r4
003627a4 pop      {r4, pc}
_ZN19SkyBoxMeshSceneNodeC2ERKN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPNS2_5scene14IMeshSceneNodeE
0036299c push     {r4, r5, r6, lr}
003629a0 mvn      r4, #0
003629a4 sub      sp, sp, #0x40
003629a8 str      r4, [sp]
003629ac add      r4, sp, #0x2c
003629b0 str      r4, [sp, #4]
003629b4 add      r4, sp, #0x10
003629b8 mov      ip, #0
003629bc mov      lr, #0x3f800000
003629c0 mov      r5, r1
003629c4 mov      r6, r3
003629c8 add      r1, r1, #4
003629cc mov      r3, #0
003629d0 str      r4, [sp, #8]
003629d4 add      r4, sp, #0x20
003629d8 str      ip, [sp, #0x18]
003629dc str      lr, [sp, #0x28]
003629e0 str      r4, [sp, #0xc]
003629e4 str      ip, [sp, #0x2c]
003629e8 mov      r4, r0
003629ec str      ip, [sp, #0x30]
003629f0 str      ip, [sp, #0x34]
003629f4 str      ip, [sp, #0x10]
003629f8 str      ip, [sp, #0x14]
003629fc str      lr, [sp, #0x1c]
00362a00 str      lr, [sp, #0x20]
00362a04 str      lr, [sp, #0x24]
00362a08 bl       #0x646678 ; _ZN6glitch7collada14CMeshSceneNodeC2ERKN5boost13intrusive_ptrINS0_5IMeshEEEPNS0_5SNodeEiRKNS_4core8vector3dIfEERKNSA_10quaternionESE_
00362a0c ldr      r3, [r5]
00362a10 mov      r0, r4
00362a14 mov      r1, #0
00362a18 str      r3, [r4]
00362a1c ldr      r3, [r3, #-0x1c]
00362a20 ldr      r2, [r5, #0x28]
00362a24 str      r2, [r4, r3]
00362a28 ldr      r3, [r4]
00362a2c ldr      r2, [r5, #0x2c]
00362a30 ldr      r3, [r3, #-0xc]
00362a34 str      r2, [r4, r3]
00362a38 mov      r3, #1
00362a3c strb     r3, [r4, #0x138]
00362a40 str      r6, [r4, #0x13c]
00362a44 bl       #0x59719c ; _ZN6glitch5scene10ISceneNode19setAutomaticCullingENS0_14E_CULLING_TYPEE
00362a48 ldr      r3, [r4, #0x13c]
00362a4c cmp      r3, #0
00362a50 beq      #0x362a6c
00362a54 ldr      r2, [r3]
00362a58 ldr      r2, [r2, #-0xc]
00362a5c add      r3, r3, r2
00362a60 ldr      r2, [r3, #4]
00362a64 add      r2, r2, #1
00362a68 str      r2, [r3, #4]
00362a6c ldr      r3, [r4, #0x134]
00362a70 add      r5, sp, #0x3c
00362a74 mov      r2, #0
00362a78 mov      r1, r3
00362a7c mov      r0, r5
00362a80 ldr      r3, [r3]
00362a84 mov      lr, pc
00362a88 ldr      pc, [r3, #0x18]
00362a8c ldr      r3, [sp, #0x3c]
00362a90 mov      r0, r5
00362a94 ldr      r3, [r3, #4]
00362a98 cmp      r3, #0
00362a9c str      r3, [sp, #0x38]
00362aa0 ldrne    r2, [r3]
00362aa4 addne    r2, r2, #1
00362aa8 strne    r2, [r3]
00362aac bl       #0x310be8 ; _ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev
00362ab0 ldr      r3, [sp, #0x38]
00362ab4 add      r0, sp, #0x38
00362ab8 ldr      r3, [r3, #0x18]
00362abc ldr      r3, [r3, #8]
00362ac0 ldr      r2, [r3, #4]
00362ac4 tst      r2, #0x100000
00362ac8 bic      r2, r2, #0x100000
00362acc str      r2, [r3, #4]
00362ad0 movne    r2, #1
00362ad4 strbne   r2, [r3, #0x30]
00362ad8 bl       #0x3522b8 ; _ZN5boost13intrusive_ptrIN6glitch5video17CMaterialRendererEED1Ev
00362adc mov      r0, r4
00362ae0 add      sp, sp, #0x40
00362ae4 pop      {r4, r5, r6, pc}
_ZN19SkyBoxMeshSceneNodeD1Ev
00362718 push     {r4, r5, r6, lr}
0036271c ldr      r5, [pc, #0x5c]
00362720 ldr      r3, [pc, #0x5c]
00362724 ldr      r2, [r0, #0x13c]
00362728 add      r5, pc, r5
0036272c ldr      r3, [r5, r3]
00362730 cmp      r2, #0
00362734 mov      r4, r0
00362738 add      r1, r3, #0x128
0036273c add      r3, r3, #0x1c
00362740 str      r3, [r0]
00362744 str      r1, [r0, #0x140]
00362748 beq      #0x362764
0036274c ldr      r3, [r2]
00362750 ldr      r0, [r3, #-0xc]
00362754 add      r0, r2, r0
00362758 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
0036275c mov      r3, #0
00362760 str      r3, [r4, #0x13c]
00362764 ldr      r1, [pc, #0x1c]
00362768 mov      r0, r4
0036276c ldr      r1, [r5, r1]
00362770 add      r1, r1, #4
00362774 bl       #0x6462e0 ; _ZN6glitch7collada14CMeshSceneNodeD2Ev
00362778 mov      r0, r4
0036277c pop      {r4, r5, r6, pc}
00362780 rsbeq    r2, r3, r8, ror #6
00362784 ldrdeq   r2, r3, [r0], -ip
00362788 andeq    r0, r0, ip, lsl #21
_ZN19SkyBoxMeshSceneNodeD2Ev
003627a8 push     {r4, r5, r6, lr}
003627ac ldr      r3, [r1]
003627b0 mov      r5, r1
003627b4 mov      r4, r0
003627b8 str      r3, [r0]
003627bc ldr      r3, [r3, #-0x1c]
003627c0 ldr      r2, [r1, #0x28]
003627c4 str      r2, [r0, r3]
003627c8 ldr      r3, [r0]
003627cc ldr      r2, [r1, #0x2c]
003627d0 ldr      r3, [r3, #-0xc]
003627d4 str      r2, [r0, r3]
003627d8 ldr      r3, [r0, #0x13c]
003627dc cmp      r3, #0
003627e0 beq      #0x3627fc
003627e4 ldr      r2, [r3]
003627e8 ldr      r0, [r2, #-0xc]
003627ec add      r0, r3, r0
003627f0 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
003627f4 mov      r3, #0
003627f8 str      r3, [r4, #0x13c]
003627fc add      r1, r5, #4
00362800 mov      r0, r4
00362804 bl       #0x6462e0 ; _ZN6glitch7collada14CMeshSceneNodeD2Ev
00362808 mov      r0, r4
0036280c pop      {r4, r5, r6, pc}
_ZN19SkyBoxMeshSceneNode19onRegisterSceneNodeEv
003625d0 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003625d4 ldr      r3, [r0, #0x134]
003625d8 sub      sp, sp, #0x1c
003625dc mov      r5, r0
003625e0 cmp      r3, #0
003625e4 beq      #0x3626f4
003625e8 ldr      r2, [r0, #0x110]
003625ec ldr      sb, [r2, #0x14]
003625f0 cmp      sb, #0
003625f4 beq      #0x3626f4
003625f8 mov      r0, r3
003625fc ldr      r3, [r3]
00362600 mov      lr, pc
00362604 ldr      pc, [r3, #0x10]
00362608 subs     r7, r0, #0
0036260c beq      #0x3626f4
00362610 mov      r4, #1
00362614 add      sl, sp, #0x14
00362618 add      r6, sp, #0x10
0036261c mov      fp, #0
00362620 mov      r8, r7
00362624 b        #0x362644
00362628 cmp      r0, #5
0036262c beq      #0x362700
00362630 mov      r0, r6
00362634 bl       #0x310be8 ; _ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev
00362638 cmp      r8, r4
0036263c add      r4, r4, #1
00362640 bls      #0x3626f4
00362644 ldr      r3, [r5, #0x134]
00362648 sub      r7, r4, #1
0036264c mov      r0, sl
00362650 mov      r1, r3
00362654 mov      r2, r7
00362658 ldr      r3, [r3]
0036265c mov      lr, pc
00362660 ldr      pc, [r3, #0x14]
00362664 ldr      r3, [sp, #0x14]
00362668 subs     r0, r3, #0
0036266c beq      #0x362638
00362670 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00362674 ldr      r3, [r5, #0x134]
00362678 mov      r0, r6
0036267c mov      r2, r7
00362680 mov      r1, r3
00362684 ldr      r3, [r3]
00362688 mov      lr, pc
0036268c ldr      pc, [r3, #0x18]
00362690 ldr      ip, [r5, #0x134]
00362694 mov      r3, r7
00362698 mov      r2, sb
0036269c mov      r0, ip
003626a0 mov      r1, #0
003626a4 ldr      ip, [ip]
003626a8 mov      lr, pc
003626ac ldr      pc, [ip, #0x38]
003626b0 cmp      r0, #4
003626b4 cmpne    r0, #0x10
003626b8 bne      #0x362628
003626bc ldr      r3, [r5, #0x110]
003626c0 mov      r1, r5
003626c4 mov      r2, r6
003626c8 ldr      ip, [r3]
003626cc mov      r0, r3
003626d0 mov      r3, #2
003626d4 str      r3, [sp]
003626d8 mvn      r3, #0x80000000
003626dc str      r3, [sp, #8]
003626e0 str      fp, [sp, #4]
003626e4 mov      r3, r4
003626e8 mov      lr, pc
003626ec ldr      pc, [ip, #0x24]
003626f0 b        #0x362630
003626f4 mov      r0, #1
003626f8 add      sp, sp, #0x1c
003626fc pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00362700 ldr      r3, [r5, #0x134]
00362704 mov      r0, r3
00362708 ldr      r3, [r3]
0036270c mov      lr, pc
00362710 ldr      pc, [r3, #0x24]
00362714 b        #0x362630
_ZNK19SkyBoxMeshSceneNode7getTypeEv
0036233c movw     r0, #0x6b73
00362340 movt     r0, #0x5f79
00362344 bx       lr
_ZN19SkyBoxMeshSceneNode6renderEPv
00362398 push     {r4, r5, r6, r7, r8, lr}
0036239c ldrb     r3, [r0, #0x138]
003623a0 sub      sp, sp, #0x20
003623a4 mov      r4, r0
003623a8 cmp      r3, #0
003623ac mov      r6, r1
003623b0 beq      #0x362584
003623b4 ldr      r2, [r0, #0x134]
003623b8 ldr      r3, [r0, #0x110]
003623bc cmp      r2, #0
003623c0 ldr      r8, [r3, #0xe4]
003623c4 ldr      r5, [r3, #0x14]
003623c8 beq      #0x362584
003623cc cmp      r5, #0
003623d0 cmpne    r8, #0
003623d4 movne    r7, #0
003623d8 moveq    r7, #1
003623dc beq      #0x362584
003623e0 mov      r0, sp
003623e4 mov      r1, r8
003623e8 bl       #0x597180 ; _ZNK6glitch5scene10ISceneNode19getAbsolutePositionEv
003623ec ldr      r2, [sp, #4]
003623f0 ldr      r3, [sp, #8]
003623f4 ldr      r1, [sp]
003623f8 str      r2, [r4, #0x58]
003623fc str      r3, [r4, #0x5c]
00362400 str      r1, [r4, #0x54]
00362404 strb     r7, [r4, #0x64]
00362408 ldr      r3, [r8]
0036240c mov      r0, r8
00362410 mov      lr, pc
00362414 ldr      pc, [r3, #0x11c]
00362418 mov      r1, r0
0036241c bl       #0x30eba4
00362420 ldr      r3, [r4, #0x134]
00362424 str      r0, [r4, #0x4c]
00362428 str      r0, [r4, #0x24]
0036242c str      r0, [r4, #0x38]
00362430 strb     r7, [r4, #0x64]
00362434 mov      r0, r3
00362438 mov      r1, r5
0036243c ldr      r3, [r3]
00362440 add      r2, r4, #0x24
00362444 mov      lr, pc
00362448 ldr      pc, [r3, #0x44]
0036244c cmp      r6, #0
00362450 beq      #0x362584
00362454 ldr      r3, [r4, #0x134]
00362458 sub      r6, r6, #1
0036245c add      r0, sp, #0x1c
00362460 mov      r1, r3
00362464 mov      r2, r6
00362468 ldr      r3, [r3]
0036246c mov      lr, pc
00362470 ldr      pc, [r3, #0x14]
00362474 ldr      r3, [sp, #0x1c]
00362478 cmp      r3, #0
0036247c beq      #0x362584
00362480 ldr      r3, [r4, #0x134]
00362484 and      r0, r6, #0x1f
00362488 mov      r1, #1
0036248c ldr      r2, [r3, #0x14]
00362490 ands     r2, r2, r1, lsl r0
00362494 beq      #0x36258c
00362498 add      r8, sp, #0x18
0036249c mov      r1, r3
003624a0 mov      r0, r8
003624a4 mov      r2, r6
003624a8 ldr      r3, [r3]
003624ac mov      lr, pc
003624b0 ldr      pc, [r3, #0x18]
003624b4 ldr      r3, [r4, #0x134]
003624b8 add      r0, sp, #0x10
003624bc mov      r2, r6
003624c0 mov      r1, r3
003624c4 ldr      r3, [r3]
003624c8 mov      lr, pc
003624cc ldr      pc, [r3, #0x1c]
003624d0 ldr      r3, [sp, #0x10]
003624d4 cmp      r3, #0
003624d8 str      r3, [sp, #0x14]
003624dc beq      #0x3624fc
003624e0 ldr      r2, [r3]
003624e4 add      r2, r2, #1
003624e8 str      r2, [r3]
003624ec ldr      r0, [sp, #0x10]
003624f0 cmp      r0, #0
003624f4 beq      #0x3624fc
003624f8 bl       #0x362368 ; _ZN6glitch21intrusive_ptr_releaseEPKNS_24ISharedMemoryBlockHeaderINS_5video27CMaterialVertexAttributeMapEEE
003624fc add      r2, sp, #0x14
00362500 mov      r0, r5
00362504 mov      r1, r8
00362508 bl       #0x35eb10 ; _ZN6glitch5video12IVideoDriver11setMaterialERKN5boost13intrusive_ptrINS0_9CMaterialEEERKNS3_IKNS0_27CMaterialVertexAttributeMapEEE
0036250c ldr      r3, [r5]
00362510 mov      r0, r5
00362514 mov      r1, #0
00362518 mov      lr, pc
0036251c ldr      pc, [r3, #0x10c]
00362520 ldr      r3, [sp, #0x1c]
00362524 mov      r0, r5
00362528 add      r1, sp, #0xc
0036252c cmp      r3, #0
00362530 str      r3, [sp, #0xc]
00362534 ldrne    r2, [r3, #4]
00362538 addne    r2, r2, #1
0036253c strne    r2, [r3, #4]
00362540 bl       #0x35ebd0 ; _ZN6glitch5video12IVideoDriver14drawMeshBufferERKN5boost13intrusive_ptrIKNS_5scene11CMeshBufferEEE
00362544 ldr      r0, [sp, #0xc]
00362548 cmp      r0, #0
0036254c beq      #0x362554
00362550 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00362554 cmp      r7, #0
00362558 bne      #0x3625b0
0036255c ldr      r0, [sp, #0x14]
00362560 cmp      r0, #0
00362564 beq      #0x36256c
00362568 bl       #0x362368 ; _ZN6glitch21intrusive_ptr_releaseEPKNS_24ISharedMemoryBlockHeaderINS_5video27CMaterialVertexAttributeMapEEE
0036256c mov      r0, r8
00362570 bl       #0x310be8 ; _ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev
00362574 ldr      r0, [sp, #0x1c]
00362578 cmp      r0, #0
0036257c beq      #0x362584
00362580 bl       #0x31d584 ; _ZNK6glitch17IReferenceCounted4dropEv
00362584 add      sp, sp, #0x20
00362588 pop      {r4, r5, r6, r7, r8, pc}
0036258c mov      r0, r3
00362590 ldr      ip, [r3]
00362594 mov      r2, r5
00362598 mov      r3, r6
0036259c mov      lr, pc
003625a0 ldr      pc, [ip, #0x38]
003625a4 ldr      r3, [r4, #0x134]
003625a8 and      r7, r0, #4
003625ac b        #0x362498
003625b0 ldr      r3, [r4, #0x134]
003625b4 mov      r1, r5
003625b8 mov      r2, r6
003625bc mov      r0, r3
003625c0 ldr      r3, [r3]
003625c4 mov      lr, pc
003625c8 ldr      pc, [r3, #0x3c]
003625cc b        #0x36255c
_ZN19SkyBoxMeshSceneNodeC1ERKN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPNS2_5scene14IMeshSceneNodeE
00362810 push     {r4, r5, r6, r7, r8, sl, lr}
00362814 ldr      r5, [pc, #0x170]
00362818 ldr      r3, [pc, #0x170]
0036281c ldr      ip, [pc, #0x170]
00362820 add      r5, pc, r5
00362824 ldr      r3, [r5, r3]
00362828 ldr      ip, [r5, ip]
0036282c mov      r6, #1
00362830 ldr      lr, [r3, #0x30]
00362834 add      ip, ip, #8
00362838 str      ip, [r0, #0x140]
0036283c str      lr, [r0]
00362840 str      r6, [r0, #0x144]
00362844 ldr      r8, [lr, #-0xc]
00362848 ldr      sl, [r3, #0x34]
0036284c mov      r7, r1
00362850 sub      sp, sp, #0x44
00362854 str      sl, [r0, r8]
00362858 mov      r8, r2
0036285c mov      r2, r7
00362860 mvn      r7, #0
00362864 str      r7, [sp]
00362868 add      r7, sp, #0x2c
0036286c str      r7, [sp, #4]
00362870 add      r7, sp, #0x10
00362874 mov      ip, #0
00362878 mov      lr, #0x3f800000
0036287c add      r1, r3, #4
00362880 str      r7, [sp, #8]
00362884 mov      r3, #0
00362888 add      r7, sp, #0x20
0036288c mov      r4, r0
00362890 str      ip, [sp, #0x18]
00362894 str      lr, [sp, #0x28]
00362898 str      ip, [sp, #0x2c]
0036289c str      ip, [sp, #0x30]
003628a0 str      ip, [sp, #0x34]
003628a4 str      ip, [sp, #0x10]
003628a8 str      ip, [sp, #0x14]
003628ac str      lr, [sp, #0x1c]
003628b0 str      lr, [sp, #0x20]
003628b4 str      lr, [sp, #0x24]
003628b8 str      r7, [sp, #0xc]
003628bc bl       #0x646678 ; _ZN6glitch7collada14CMeshSceneNodeC2ERKN5boost13intrusive_ptrINS0_5IMeshEEEPNS0_5SNodeEiRKNS_4core8vector3dIfEERKNSA_10quaternionESE_
003628c0 ldr      r3, [pc, #0xd0]
003628c4 strb     r6, [r4, #0x138]
003628c8 str      r8, [r4, #0x13c]
003628cc ldr      r3, [r5, r3]
003628d0 mov      r0, r4
003628d4 mov      r1, #0
003628d8 add      r2, r3, #0x128
003628dc add      r3, r3, #0x1c
003628e0 str      r3, [r4]
003628e4 str      r2, [r4, #0x140]
003628e8 bl       #0x59719c ; _ZN6glitch5scene10ISceneNode19setAutomaticCullingENS0_14E_CULLING_TYPEE
003628ec ldr      r3, [r4, #0x13c]
003628f0 cmp      r3, #0
003628f4 beq      #0x362910
003628f8 ldr      r2, [r3]
003628fc ldr      r2, [r2, #-0xc]
00362900 add      r3, r3, r2
00362904 ldr      r2, [r3, #4]
00362908 add      r2, r2, r6
0036290c str      r2, [r3, #4]
00362910 ldr      r3, [r4, #0x134]
00362914 add      r5, sp, #0x3c
00362918 mov      r2, #0
0036291c mov      r1, r3
00362920 mov      r0, r5
00362924 ldr      r3, [r3]
00362928 mov      lr, pc
0036292c ldr      pc, [r3, #0x18]
00362930 ldr      r3, [sp, #0x3c]
00362934 mov      r0, r5
00362938 ldr      r3, [r3, #4]
0036293c cmp      r3, #0
00362940 str      r3, [sp, #0x38]
00362944 ldrne    r2, [r3]
00362948 addne    r2, r2, #1
0036294c strne    r2, [r3]
00362950 bl       #0x310be8 ; _ZN5boost13intrusive_ptrIN6glitch5video9CMaterialEED1Ev
00362954 ldr      r3, [sp, #0x38]
00362958 add      r0, sp, #0x38
0036295c ldr      r3, [r3, #0x18]
00362960 ldr      r3, [r3, #8]
00362964 ldr      r2, [r3, #4]
00362968 tst      r2, #0x100000
0036296c bic      r2, r2, #0x100000
00362970 str      r2, [r3, #4]
00362974 movne    r2, #1
00362978 strbne   r2, [r3, #0x30]
0036297c bl       #0x3522b8 ; _ZN5boost13intrusive_ptrIN6glitch5video17CMaterialRendererEED1Ev
00362980 mov      r0, r4
00362984 add      sp, sp, #0x44
00362988 pop      {r4, r5, r6, r7, r8, sl, pc}
0036298c rsbeq    r2, r3, r0, ror r2
00362990 andeq    r0, r0, ip, lsl #21
00362994 andeq    r2, r0, r4, asr #22
00362998 ldrdeq   r2, r3, [r0], -ip
