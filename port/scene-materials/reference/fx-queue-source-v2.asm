
0063b620 _ZN6glitch7collada33CGlitchNewParticleSystemSceneNode19onRegisterSceneNodeEv 164
0063b620 push {r4, r5, r6, r7, lr}
0063b624 ldr r2, [r0, #0x178]
0063b628 movw r3, #0x6f97
0063b62c movt r3, #0x96f9
0063b630 ldr r1, [r2]
0063b634 sub sp, sp, #0x1c
0063b638 mov r6, r0
0063b63c ldr r1, [r1, #-0xc]
0063b640 add r2, r2, r1
0063b644 ldr r1, [r2, #0x24]
0063b648 ldr r2, [r2, #0x28]
0063b64c rsb r2, r1, r2
0063b650 asr r2, r2, #2
0063b654 mul r3, r3, r2
0063b658 cmp r3, #0
0063b65c beq #0x63b6b8
0063b660 ldr r7, [r0, #0x110]
0063b664 add r4, sp, #0x14
0063b668 mov r0, r4
0063b66c ldr ip, [r7]
0063b670 mov r1, r6
0063b674 mov r2, #0
0063b678 ldr r3, [r6]
0063b67c ldr r5, [ip, #0x24]
0063b680 mov lr, pc
0063b684 ldr pc, [r3, #0x84]
0063b688 mov r2, #8
0063b68c mov r3, #0
0063b690 str r2, [sp]
0063b694 mvn r2, #0x80000000
0063b698 str r2, [sp, #8]
0063b69c str r3, [sp, #4]
0063b6a0 mov r0, r7
0063b6a4 mov r1, r6
0063b6a8 mov r2, r4
0063b6ac blx r5
0063b6b0 mov r0, r4
0063b6b4 bl #0x310be8
0063b6b8 mov r0, #1
0063b6bc add sp, sp, #0x1c
0063b6c0 pop {r4, r5, r6, r7, pc}

0058b7f4 _ZN6glitch5scene13CSceneManager7drawAllEPNS0_10ISceneNodeE 152
0058b7f4 push {r4, r5, r6, lr}
0058b7f8 mov r5, r1
0058b7fc ldr r3, [r0]
0058b800 ldr r1, [r0, #0x18]
0058b804 mov r4, r0
0058b808 mov lr, pc
0058b80c ldr pc, [r3, #0x40]
0058b810 cmp r5, #0
0058b814 beq #0x58b874
0058b818 mov r0, r4
0058b81c ldr r3, [r4]
0058b820 mov lr, pc
0058b824 ldr pc, [r3, #0x44]
0058b828 mov r1, r5
0058b82c mov r0, r4
0058b830 ldr r3, [r4]
0058b834 mov lr, pc
0058b838 ldr pc, [r3, #0x28]
0058b83c mov r0, r4
0058b840 ldr r3, [r4]
0058b844 mov lr, pc
0058b848 ldr pc, [r3, #0x4c]
0058b84c mov r0, r4
0058b850 ldr r3, [r4]
0058b854 ldr r1, [r4, #0x18]
0058b858 mov lr, pc
0058b85c ldr pc, [r3, #0x48]
0058b860 mov r3, #9
0058b864 add r0, r4, #0x114
0058b868 str r3, [r4, #0x174]
0058b86c pop {r4, r5, r6, lr}
0058b870 b #0x58a75c
0058b874 ldrb r3, [r4, #0x288]
0058b878 cmp r3, #0
0058b87c beq #0x58b818
0058b880 mov r0, r4
0058b884 bl #0x58b7ac
0058b888 b #0x58b818

00359338 _ZN12SceneManager7drawAllEPN6glitch5scene10ISceneNodeE 364
00359338 push {r4, r5, r6, r7, r8, sl, lr}
0035933c ldr r4, [pc, #0x14c]
00359340 ldr r5, [pc, #0x14c]
00359344 sub sp, sp, #0x8c
00359348 add r4, pc, r4
0035934c ldr r3, [r4, r5]
00359350 mov r6, r0
00359354 mov r7, r1
00359358 ldr r3, [r3]
0035935c add r0, r0, #0x294
00359360 str r3, [sp, #0x84]
00359364 bl #0x40d5a0
00359368 mov r0, r6
0035936c mov r1, r7
00359370 bl #0x3592b0
00359374 ldrb r3, [r6, #0x290]
00359378 cmp r3, #0
0035937c bne #0x35939c
00359380 ldr r3, [r4, r5]
00359384 ldr r2, [sp, #0x84]
00359388 ldr r3, [r3]
0035938c cmp r2, r3
00359390 bne #0x35948c
00359394 add sp, sp, #0x8c
00359398 pop {r4, r5, r6, r7, r8, sl, pc}
0035939c ldr r3, [pc, #0xf4]
003593a0 mov r8, #0
003593a4 strb r8, [r6, #0x290]
003593a8 ldr sl, [r4, r3]
003593ac add r7, sp, #0x6c
003593b0 mov r0, sl
003593b4 bl #0x337888
003593b8 ldr r1, [pc, #0xdc]
003593bc add r2, sp, #4
003593c0 mov r0, r7
003593c4 add r1, pc, r1
003593c8 bl #0x3140ec
003593cc mov r0, sl
003593d0 mov r1, r7
003593d4 bl #0x337a88
003593d8 mov sl, r0
003593dc mov r0, r7
003593e0 bl #0x318254
003593e4 cmp sl, r8
003593e8 beq #0x359380
003593ec mov r0, r8
003593f0 bl #0x30e580
003593f4 ldr r1, [pc, #0xa4]
003593f8 add r7, sp, #8
003593fc mov r2, r0
00359400 add r1, pc, r1
00359404 mov r0, r7
00359408 bl #0x30eae4
0035940c ldr r3, [r6, #0x14]
00359410 mov r0, sp
00359414 mov sl, sp
00359418 mov r1, r3
0035941c ldr r3, [r3]
00359420 mov lr, pc
00359424 ldr pc, [r3, #0x98]
00359428 mov r0, r7
0035942c mov r1, r8
00359430 bl #0x570bf4
00359434 mov r1, r8
00359438 mov r7, r0
0035943c mov r0, #8
00359440 bl #0x5341ac
00359444 mov r6, r0
00359448 bl #0x60730c
0035944c mov r2, sp
00359450 mov r3, r8
00359454 mov r1, r7
00359458 ldr ip, [r6]
0035945c mov r0, r6
00359460 mov lr, pc
00359464 ldr pc, [ip, #0x10]
00359468 mov r0, r6
0035946c bl #0x31d584
00359470 mov r0, r7
00359474 bl #0x31d584
00359478 ldr r0, [sp]
0035947c cmp r0, r8
00359480 beq #0x359380
00359484 bl #0x31d584
00359488 b #0x359380
0035948c bl #0x30e310
00359490 rsbeq fp, r3, r8, asr #14
00359494 andeq r4, r0, ip, lsr #1
00359498 andeq r0, r0, r4, lsl #17
0035949c subseq r6, r6, ip, lsl #14
003594a0 subseq r7, r6, r0, asr #15

00356d80 _ZN6glitch4core8heapsinkINS_5scene13CSceneManager21STransparentNodeEntryEEEvPT_ii 392
00356d80 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00356d84 lsl r7, r1, #1
00356d88 cmp r2, r7
00356d8c sub sp, sp, #0x34
00356d90 mov r4, r1
00356d94 mov sb, r2
00356d98 mov r8, r0
00356d9c ble #0x356edc
00356da0 add r3, sp, #0x2c
00356da4 str r3, [sp, #4]
00356da8 add r3, sp, #0x28
00356dac str r3, [sp, #8]
00356db0 add r3, sp, #0x1c
00356db4 mov sl, #0x14
00356db8 str r3, [sp, #0xc]
00356dbc add r6, r7, #1
00356dc0 cmp sb, r6
00356dc4 mlale r5, sl, r7, r8
00356dc8 bgt #0x356ee4
00356dcc mov r6, r7
00356dd0 mul r4, sl, r4
00356dd4 mov r1, r5
00356dd8 add r7, r8, r4
00356ddc mov r0, r7
00356de0 bl #0x3538ac
00356de4 cmp r0, #0
00356de8 beq #0x356edc
00356dec ldm r5, {r1, r2, r3}
00356df0 cmp r3, #0
00356df4 str r2, [sp, #0x18]
00356df8 str r1, [sp, #0x14]
00356dfc str r3, [sp, #0x1c]
00356e00 ldrne r2, [r3]
00356e04 addne r2, r2, #1
00356e08 strne r2, [r3]
00356e0c ldr r3, [r8, r4]
00356e10 ldr r2, [r5, #0x10]
00356e14 ldr r1, [r5, #0xc]
00356e18 str r3, [r5]
00356e1c ldr r3, [r7, #4]
00356e20 str r2, [sp, #0x24]
00356e24 str r1, [sp, #0x20]
00356e28 str r3, [r5, #4]
00356e2c ldr r3, [r7, #8]
00356e30 cmp r3, #0
00356e34 str r3, [sp, #0x2c]
00356e38 ldrne r2, [r3]
00356e3c moveq r2, r3
00356e40 addne r2, r2, #1
00356e44 strne r2, [r3]
00356e48 ldrne r2, [sp, #0x2c]
00356e4c ldr r3, [r5, #8]
00356e50 str r2, [r5, #8]
00356e54 ldr r0, [sp, #4]
00356e58 str r3, [sp, #0x2c]
00356e5c bl #0x351e3c
00356e60 ldr r2, [r7, #0xc]
00356e64 ldr r3, [sp, #0x1c]
00356e68 ldr r1, [sp, #0x14]
00356e6c str r2, [r5, #0xc]
00356e70 ldr r2, [sp, #0x18]
00356e74 ldr r0, [r7, #0x10]
00356e78 cmp r3, #0
00356e7c str r0, [r5, #0x10]
00356e80 str r1, [r8, r4]
00356e84 str r2, [r7, #4]
00356e88 str r3, [sp, #0x28]
00356e8c ldrne r2, [r3]
00356e90 moveq r2, r3
00356e94 mov r4, r6
00356e98 addne r2, r2, #1
00356e9c strne r2, [r3]
00356ea0 ldrne r2, [sp, #0x28]
00356ea4 ldr r3, [r7, #8]
00356ea8 str r2, [r7, #8]
00356eac ldr r0, [sp, #8]
00356eb0 str r3, [sp, #0x28]
00356eb4 bl #0x351e3c
00356eb8 ldr r3, [sp, #0x24]
00356ebc ldr r2, [sp, #0x20]
00356ec0 ldr r0, [sp, #0xc]
00356ec4 str r3, [r7, #0x10]
00356ec8 str r2, [r7, #0xc]
00356ecc lsl r7, r6, #1
00356ed0 bl #0x351e3c
00356ed4 cmp r7, sb
00356ed8 blt #0x356dbc
00356edc add sp, sp, #0x34
00356ee0 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00356ee4 mla r5, sl, r7, r8
00356ee8 mla fp, sl, r6, r8
00356eec mov r0, r5
00356ef0 mov r1, fp
00356ef4 bl #0x3538ac
00356ef8 cmp r0, #0
00356efc beq #0x356dcc
00356f00 mov r5, fp
00356f04 b #0x356dd0
vtableword 0: 0 ('pthread_mutexattr_init', 0)
vtableword 4: 0 ('pthread_mutexattr_init', 0)
vtableword 8: 0 ('pthread_mutexattr_init', 0)
vtableword c: 0 ('pthread_mutexattr_init', 0)
vtableword 10: 190 None
vtableword 14: 0 ('pthread_mutexattr_init', 0)
vtableword 18: 0 ('pthread_mutexattr_init', 0)
vtableword 1c: 6677d0 ('_ZNK6glitch7collada24IParticleSystemSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE', 4)
vtableword 20: 6677d4 ('_ZN6glitch7collada24IParticleSystemSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE', 4)
vtableword 24: 63ca28 ('_ZN6glitch7collada33CGlitchNewParticleSystemSceneNodeD1Ev', 108)
vtableword 28: 63ca94 ('_ZN6glitch7collada33CGlitchNewParticleSystemSceneNodeD0Ev', 28)
vtableword 2c: 63b620 ('_ZN6glitch7collada33CGlitchNewParticleSystemSceneNode19onRegisterSceneNodeEv', 164)
vtableword 30: 63fed4 ('_ZN6glitch7collada33CGlitchNewParticleSystemSceneNode9onAnimateEj', 264)
vtableword 34: 596d10 ('_ZN6glitch5scene10ISceneNode12onUpdateTimeEj', 92)
vtableword 38: 643ea8 ('_ZN6glitch7collada33CGlitchNewParticleSystemSceneNode6renderEPv', 1308)
vtableword 3c: 596e24 ('_ZNK6glitch5scene10ISceneNode20getRenderVertexCountEPv', 8)
vtableword 40: 596e2c ('_ZNK6glitch5scene10ISceneNode7getNameEv', 8)
vtableword 44: 598a04 ('_ZN6glitch5scene10ISceneNode7setNameEPKc', 40)
vtableword 48: 597f50 ('_ZN6glitch5scene10ISceneNode7setNameERKSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEE', 24)
vtableword 4c: 63b1d0 ('_ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode14getBoundingBoxEv', 512)
vtableword 50: 637b84 ('_ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode25getTransformedBoundingBoxEv', 8)
vtableword 54: 35a8e0 ('_ZNK6glitch5scene10ISceneNode25getAbsoluteTransformationEv', 8)
vtableword 58: 599444 ('_ZN6glitch5scene10ISceneNode25setAbsoluteTransformationERNS_4core8CMatrix4IfEE', 740)
vtableword 5c: 598908 ('_ZNK6glitch5scene10ISceneNode25getRelativeTransformationEv', 252)
vtableword 60: 59901c ('_ZN6glitch5scene10ISceneNode25setRelativeTransformationERKNS_4core8CMatrix4IfEE', 164)
vtableword 64: 596ec4 ('_ZN6glitch5scene10ISceneNode10setVisibleEb', 152)
vtableword 68: 596f5c ('_ZNK6glitch5scene10ISceneNode5getIDEv', 8)
vtableword 6c: 596f64 ('_ZN6glitch5scene10ISceneNode5setIDEi', 8)
vtableword 70: 637b3c ('_ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode6getUIDEv', 12)
vtableword 74: 596ff4 ('_ZNK6glitch5scene10ISceneNode10getScopeIDEv', 16)
vtableword 78: 598864 ('_ZN6glitch5scene10ISceneNode8addChildEPS1_', 164)
vtableword 7c: 597004 ('_ZN6glitch5scene10ISceneNode11removeChildEPS1_', 104)
vtableword 80: 5987e8 ('_ZN6glitch5scene10ISceneNode9removeAllEv', 124)
vtableword 84: 59706c ('_ZN6glitch5scene10ISceneNode6removeEv', 40)
vtableword 88: 598770 ('_ZN6glitch5scene10ISceneNode11addAnimatorEPNS0_18ISceneNodeAnimatorE', 120)
vtableword 8c: 5986e8 ('_ZN6glitch5scene10ISceneNode14removeAnimatorEPNS0_18ISceneNodeAnimatorE', 136)
vtableword 90: 598658 ('_ZN6glitch5scene10ISceneNode15removeAnimatorsEv', 144)
vtableword 94: 597d58 ('_ZN6glitch5scene10ISceneNode17addBindedAnimatorEPNS0_18ISceneNodeAnimatorE', 116)
vtableword 98: 597fe4 ('_ZN6glitch5scene10ISceneNode20removeBindedAnimatorEPNS0_18ISceneNodeAnimatorE', 116)
vtableword 9c: 597f68 ('_ZN6glitch5scene10ISceneNode21removeBindedAnimatorsEv', 124)
vtableword a0: 66777c ('_ZNK6glitch7collada24IParticleSystemSceneNode11getMaterialEj', 32)
vtableword a4: 66779c ('_ZNK6glitch7collada24IParticleSystemSceneNode16getMaterialCountEv', 20)
vtableword a8: 5970b8 ('_ZN6glitch5scene10ISceneNode21notifyMaterialChangedEv', 4)
vtableword ac: 5970bc ('_ZNK6glitch5scene10ISceneNode8getScaleEv', 8)
vtableword b0: 5970c4 ('_ZN6glitch5scene10ISceneNode8setScaleERKNS_4core8vector3dIfEE', 40)
vtableword b4: 5970ec ('_ZNK6glitch5scene10ISceneNode11getRotationEv', 8)
vtableword b8: 5970f4 ('_ZN6glitch5scene10ISceneNode11setRotationERKNS_4core10quaternionE', 48)
vtableword bc: 597124 ('_ZNK6glitch5scene10ISceneNode11getPositionEv', 8)
vtableword c0: 59712c ('_ZN6glitch5scene10ISceneNode11setPositionERKNS_4core8vector3dIfEE', 40)
vtableword c4: 5971d0 ('_ZNK6glitch5scene10ISceneNode15getUserPropertyEv', 8)
vtableword c8: 5971d8 ('_ZNK6glitch5scene10ISceneNode18getUserPropertyStrEv', 8)
vtableword cc: 597254 ('_ZNK6glitch5scene10ISceneNode19getTriangleSelectorEv', 8)
vtableword d0: 59725c ('_ZN6glitch5scene10ISceneNode19setTriangleSelectorEPNS0_17ITriangleSelectorE', 52)
vtableword d4: 597c60 ('_ZN6glitch5scene10ISceneNode22updateAbsolutePositionEb', 248)
vtableword d8: 637b78 ('_ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode7getTypeEv', 12)
vtableword dc: 667774 ('_ZN6glitch7collada24IParticleSystemSceneNode5cloneEv', 8)
vtableword e0: 5974cc ('_ZN6glitch5scene10ISceneNode11setGameDataEPv', 8)
vtableword e4: 5974d4 ('_ZNK6glitch5scene10ISceneNode11getGameDataEv', 8)
vtableword e8: 5974dc ('_ZN6glitch5scene10ISceneNode15setCameraOffsetEf', 8)
vtableword ec: 5974e4 ('_ZNK6glitch5scene10ISceneNode15getCameraOffsetEv', 8)
vtableword f0: 5974ec ('_ZN6glitch5scene10ISceneNode17setRenderingLayerEi', 8)
vtableword f4: 5974f4 ('_ZNK6glitch5scene10ISceneNode17getRenderingLayerEv', 8)
vtableword f8: 5974fc ('_ZN6glitch5scene10ISceneNode17onMaterialChangedEv', 4)
vtableword fc: 597544 ('_ZN6glitch5scene10ISceneNode14resetTransformEb', 4)
vtableword 100: 597500 ('_ZN6glitch5scene10ISceneNode30setNodeHierarchyAsShadowCasterEb', 68)
vtableword 104: 596ce0 ('_ZN6glitch5scene10ISceneNode8onDeleteEv', 40)
vtableword 108: 596e34 ('_ZN6glitch5scene10ISceneNode23notifyVisibilityChangedEb', 140)
vtableword 10c: 596ec0 ('_ZN6glitch5scene10ISceneNode21onChangedSceneManagerEv', 4)
vtableword 110: 642e58 ('_ZN6glitch7collada33CGlitchNewParticleSystemSceneNode18initParticleSystemEPNS_5video12IVideoDriverEb', 4176)
vtableword 114: 637b48 ('_ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode16getParticleCountEv', 48)
vtableword 118: 63f07c ('_ZN6glitch7collada33CGlitchNewParticleSystemSceneNode26getParticleSystemParameterEPKc', 168)
vtableword 11c: 63f1c8 ('_ZN6glitch7collada33CGlitchNewParticleSystemSceneNode15setParticleMeshEN5boost13intrusive_ptrINS_5scene11CMeshBufferEEE', 76)
vtableword 120: 63f214 ('_ZN6glitch7collada33CGlitchNewParticleSystemSceneNode4initEv', 1668)
vtableword 124: 63db4c ('_ZN6glitch7collada33CGlitchNewParticleSystemSceneNode6attachEPNS_5scene10ISceneNodeE', 240)
vtableword 128: fffffe70 None
vtableword 12c: fffffe70 None
vtableword 130: fffffe70 None
vtableword 134: 0 ('pthread_mutexattr_init', 0)
vtableword 138: 6443f4 ('_ZTv0_n12_N6glitch7collada33CGlitchNewParticleSystemSceneNodeD1Ev', 16)
vtableword 13c: 6443d4 ('_ZTv0_n12_N6glitch7collada33CGlitchNewParticleSystemSceneNodeD0Ev', 16)
vtableword 140: 599788 ('_ZTv0_n16_N6glitch5scene10ISceneNode8onDeleteEv', 16)

actual particle vtable 20

0063ca94 _ZN6glitch7collada33CGlitchNewParticleSystemSceneNodeD0Ev 28
0063ca94 push {r4, lr}
0063ca98 mov r4, r0
0063ca9c bl #0x63ca28
0063caa0 mov r0, r4
0063caa4 bl #0x30e2b0
0063caa8 mov r0, r4
0063caac pop {r4, pc}

actual particle vtable 38

00596e2c _ZNK6glitch5scene10ISceneNode7getNameEv 8
00596e2c ldr r0, [r0, #0x20]
00596e30 bx lr

actual particle vtable d0

00637b78 _ZNK6glitch7collada33CGlitchNewParticleSystemSceneNode7getTypeEv 12
00637b78 movw r0, #0x6164
00637b7c movt r0, #0x6765
00637b80 bx lr

actual particle vtable d8

005974cc _ZN6glitch5scene10ISceneNode11setGameDataEPv 8
005974cc str r1, [r0, #0x124]
005974d0 bx lr

003510f0 _ZN6glitch4core8heapsortINS_5scene13CSceneManager24SRenderDataSortNodeEntryEEEvPT_i 136
003510f0 push {r4, r5, r6, r7, r8, sb, sl, lr}
003510f4 sub r4, r1, #1
003510f8 add r7, r4, r4, lsr #31
003510fc mov r6, r1
00351100 asrs r7, r7, #1
00351104 mov r5, r0
00351108 sub r8, r0, #8
0035110c bmi #0x351130
00351110 add r7, r7, #1
00351114 add sl, r1, #1
00351118 mov r1, r7
0035111c mov r0, r8
00351120 mov r2, sl
00351124 bl #0x35104c
00351128 subs r7, r7, #1
0035112c bne #0x351118
00351130 cmp r4, #0
00351134 blt #0x351174
00351138 add r4, r5, r4, lsl #3
0035113c ldr r2, [r4]
00351140 ldr r3, [r5]
00351144 ldr r1, [r5, #4]
00351148 str r2, [r5]
0035114c ldr ip, [r4, #4]
00351150 mov r2, r6
00351154 mov r0, r8
00351158 str ip, [r5, #4]
0035115c str r1, [r4, #4]
00351160 str r3, [r4], #-8
00351164 mov r1, #1
00351168 bl #0x35104c
0035116c subs r6, r6, #1
00351170 bne #0x35113c
00351174 pop {r4, r5, r6, r7, r8, sb, sl, pc}

00356f08 _ZN6glitch4core8heapsortINS_5scene13CSceneManager21STransparentNodeEntryEEEvPT_i 340
00356f08 push {r4, r5, r6, r7, r8, sb, sl, lr}
00356f0c sub r4, r1, #1
00356f10 add r8, r4, r4, lsr #31
00356f14 sub sp, sp, #0x20
00356f18 asrs r8, r8, #1
00356f1c mov r6, r1
00356f20 mov r5, r0
00356f24 sub r7, r0, #0x14
00356f28 bmi #0x356f4c
00356f2c add r8, r8, #1
00356f30 add sl, r1, #1
00356f34 mov r1, r8
00356f38 mov r0, r7
00356f3c mov r2, sl
00356f40 bl #0x356d80
00356f44 subs r8, r8, #1
00356f48 bne #0x356f34
00356f4c cmp r4, #0
00356f50 blt #0x357054
00356f54 mov r3, #0x14
00356f58 mla r4, r3, r4, r5
00356f5c add sl, sp, #0x1c
00356f60 add r4, r4, #0x10
00356f64 add r8, sp, #0x18
00356f68 add sb, sp, #0xc
00356f6c ldm r5, {r1, r2, r3}
00356f70 cmp r3, #0
00356f74 stmib sp, {r1, r2, r3}
00356f78 ldrne r2, [r3]
00356f7c mov r0, sl
00356f80 addne r2, r2, #1
00356f84 strne r2, [r3]
00356f88 ldr r2, [r5, #0xc]
00356f8c ldr r3, [r5, #0x10]
00356f90 str r2, [sp, #0x10]
00356f94 str r3, [sp, #0x14]
00356f98 ldr r3, [r4, #-0x10]
00356f9c str r3, [r5]
00356fa0 ldr r3, [r4, #-0xc]
00356fa4 str r3, [r5, #4]
00356fa8 ldr r2, [r4, #-8]
00356fac str r2, [sp, #0x1c]
00356fb0 cmp r2, #0
00356fb4 ldrne r3, [r2]
00356fb8 addne r3, r3, #1
00356fbc strne r3, [r2]
00356fc0 ldrne r2, [sp, #0x1c]
00356fc4 ldr r3, [r5, #8]
00356fc8 str r2, [r5, #8]
00356fcc str r3, [sp, #0x1c]
00356fd0 bl #0x351e3c
00356fd4 ldr r2, [r4, #-4]
00356fd8 ldr r3, [sp, #4]
00356fdc mov r0, r8
00356fe0 str r2, [r5, #0xc]
00356fe4 ldr r2, [r4]
00356fe8 str r2, [r5, #0x10]
00356fec str r3, [r4, #-0x10]
00356ff0 ldr r3, [sp, #8]
00356ff4 str r3, [r4, #-0xc]
00356ff8 ldr r2, [sp, #0xc]
00356ffc cmp r2, #0
00357000 str r2, [sp, #0x18]
00357004 ldrne r3, [r2]
00357008 addne r3, r3, #1
0035700c strne r3, [r2]
00357010 ldrne r2, [sp, #0x18]
00357014 ldr r3, [r4, #-8]
00357018 str r3, [sp, #0x18]
0035701c str r2, [r4, #-8]
00357020 bl #0x351e3c
00357024 ldr r3, [sp, #0x10]
00357028 mov r2, r6
0035702c mov r1, #1
00357030 str r3, [r4, #-4]
00357034 ldr r3, [sp, #0x14]
00357038 mov r0, r7
0035703c str r3, [r4], #-0x14
00357040 bl #0x356d80
00357044 mov r0, sb
00357048 bl #0x351e3c
0035704c subs r6, r6, #1
00357050 bne #0x356f6c
00357054 add sp, sp, #0x20
00357058 pop {r4, r5, r6, r7, r8, sb, sl, pc}
