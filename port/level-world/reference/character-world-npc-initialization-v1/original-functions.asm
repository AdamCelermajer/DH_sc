# _ZN5Level15_LoadCharStatesEv 0x3eff98
003eff98: ldr      r3, [pc, #0x70]
003eff9c: ldr      r2, [pc, #0x70]
003effa0: push     {r4, r5, r6, lr}
003effa4: add      r3, pc, r3
003effa8: ldr      r2, [r3, r2]
003effac: ldr      r6, [r2, #0x38]
003effb0: ldr      r4, [r6, #0x60]!
003effb4: cmp      r6, r4
003effb8: beq      #0x3efff8
003effbc: ldr      r5, [r4, #8]
003effc0: subs     r0, r5, #0
003effc4: beq      #0x3effec
003effc8: bl       #0x3a5784 // _ZNK9Character16GetPreSetAIStateEv
003effcc: subs     r3, r0, #0
003effd0: beq      #0x3efffc
003effd4: add      r0, r5, #0x4f0
003effd8: cmp      r3, #0x11
003effdc: add      r0, r0, #0xc
003effe0: mov      r1, #0
003effe4: beq      #0x3efffc
003effe8: bl       #0x3c1a00 // _ZN16CharStateMachine15SM_SetIdleStateEb
003effec: ldr      r4, [r4]
003efff0: cmp      r6, r4
003efff4: bne      #0x3effbc
003efff8: pop      {r4, r5, r6, pc}
003efffc: add      r0, r5, #0x4f0
003f0000: add      r0, r0, #0xc
003f0004: bl       #0x3c1a64 // _ZN16CharStateMachine19SM_SetPreSpawnStateEv
003f0008: ldr      r4, [r4]
003f000c: b        #0x3efff0 // 
003f0010: subseq   r4, sl, ip, ror #21
003f0014: strdeq   r3, r4, [r0], -r4
# _ZN9Character8InitPostEv 0x3b4d60
003b4d60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b4d64: ldr      r5, [pc, #0x828]
003b4d68: ldr      r6, [pc, #0x828]
003b4d6c: movw     r3, #0x1394
003b4d70: add      r5, pc, r5
003b4d74: ldr      r2, [r5, r6]
003b4d78: ldrb     r7, [r0, r3]
003b4d7c: sub      sp, sp, #0xfc
003b4d80: ldr      r2, [r2]
003b4d84: cmp      r7, #0
003b4d88: mov      r4, r0
003b4d8c: str      r2, [sp, #0xf4]
003b4d90: beq      #0x3b4db0
003b4d94: ldr      r3, [r5, r6]
003b4d98: ldr      r2, [sp, #0xf4]
003b4d9c: ldr      r3, [r3]
003b4da0: cmp      r2, r3
003b4da4: bne      #0x3b5590
003b4da8: add      sp, sp, #0xfc
003b4dac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b4db0: mov      r2, #1
003b4db4: strb     r2, [r0, r3]
003b4db8: bl       #0x38bd64 // _ZN10GameObject21CheckSpawnProbabilityEv
003b4dbc: ldr      r3, [r4, #0x274]
003b4dc0: cmp      r0, r3
003b4dc4: bge      #0x3b4d94
003b4dc8: ldr      fp, [pc, #0x7cc]
003b4dcc: add      r8, sp, #0xdc
003b4dd0: ldr      sl, [r5, fp]
003b4dd4: mov      r0, sl
003b4dd8: bl       #0x337888 // _ZN13DebugSwitches4loadEv
003b4ddc: ldr      r1, [pc, #0x7bc]
003b4de0: add      r2, sp, #0x48
003b4de4: mov      r0, r8
003b4de8: add      r1, pc, r1
003b4dec: bl       #0x3140ec // _ZNSsC1EPKcRKSaIcE
003b4df0: mov      r1, r8
003b4df4: mov      r0, sl
003b4df8: bl       #0x337a88 // _ZN13DebugSwitches9GetSwitchERKSs
003b4dfc: mov      r0, r8
003b4e00: bl       #0x318254 // _ZNSsD1Ev
003b4e04: movw     r3, #0x13fc
003b4e08: ldr      r2, [r4, r3]
003b4e0c: movw     r3, #0x13f8
003b4e10: ldr      r3, [r4, r3]
003b4e14: cmp      r3, r2
003b4e18: beq      #0x3b4e6c
003b4e1c: ldr      r1, [pc, #0x780]
003b4e20: add      r8, sp, #0x24
003b4e24: ldr      r3, [r4, #0x64]
003b4e28: ldr      r1, [r5, r1]
003b4e2c: mov      r0, r8
003b4e30: ldr      r1, [r1, #0x38]
003b4e34: str      r7, [sp]
003b4e38: str      r7, [sp, #4]
003b4e3c: bl       #0x34aca0 // _ZN13ObjectManager15GetObjectByNameEPKcibS1_
003b4e40: mov      r0, r8
003b4e44: mov      r1, r7
003b4e48: bl       #0x33fdc0 // _ZN12ObjectHandle9GetObjectEb
003b4e4c: cmp      r0, #0
003b4e50: beq      #0x3b4e6c
003b4e54: mov      r0, r8
003b4e58: bl       #0x33fee4 // _ZN12ObjectHandlecvP10GameObjectEv
003b4e5c: subs     r1, r0, #0
003b4e60: beq      #0x3b4e6c
003b4e64: ldr      r0, [r4, #0x378]
003b4e68: bl       #0x405540 // _ZN12v2Controller10Cmd_MoveToEP10GameObject
003b4e6c: mov      r0, r4
003b4e70: movw     r7, #0x13c8
003b4e74: bl       #0x3b3d38 // _ZN9Character18SafeGetCharPropsIdEv
003b4e78: ldrsh    r3, [r4, r7]
003b4e7c: cmn      r3, #1
003b4e80: movweq   r1, #0xffff
003b4e84: beq      #0x3b4ec4
003b4e88: ldr      sl, [r5, fp]
003b4e8c: add      r8, sp, #0xc4
003b4e90: mov      r0, sl
003b4e94: bl       #0x337888 // _ZN13DebugSwitches4loadEv
003b4e98: ldr      r1, [pc, #0x708]
003b4e9c: add      r2, sp, #0x44
003b4ea0: mov      r0, r8
003b4ea4: add      r1, pc, r1
003b4ea8: bl       #0x3140ec // _ZNSsC1EPKcRKSaIcE
003b4eac: mov      r1, r8
003b4eb0: mov      r0, sl
003b4eb4: bl       #0x337a88 // _ZN13DebugSwitches9GetSwitchERKSs
003b4eb8: mov      r0, r8
003b4ebc: bl       #0x318254 // _ZNSsD1Ev
003b4ec0: ldrh     r1, [r4, r7]
003b4ec4: add      r7, r4, #0x560
003b4ec8: sxth     r1, r1
003b4ecc: mov      r0, r7
003b4ed0: bl       #0x3df2a4 // _ZN14CharProperties18LoadBasePropertiesEi
003b4ed4: mov      r0, r7
003b4ed8: mov      r1, #1
003b4edc: bl       #0x3e0810 // _ZN14CharProperties16RecalcPropertiesEb
003b4ee0: mov      r0, r4
003b4ee4: bl       #0x3a54d4 // _ZNK9Character16GetCharModelNameEv
003b4ee8: subs     r8, r0, #0
003b4eec: beq      #0x3b4f04
003b4ef0: bl       #0x30de54 // 
003b4ef4: mov      r1, r8
003b4ef8: add      r2, r8, r0
003b4efc: add      r0, r4, #0x290
003b4f00: bl       #0x3109e0 // _ZNSs9_M_assignEPKcS0_
003b4f04: ldr      sl, [r5, fp]
003b4f08: add      r8, sp, #0xac
003b4f0c: mov      r0, sl
003b4f10: bl       #0x337888 // _ZN13DebugSwitches4loadEv
003b4f14: ldr      r1, [pc, #0x690]
003b4f18: add      r2, sp, #0x40
003b4f1c: mov      r0, r8
003b4f20: add      r1, pc, r1
003b4f24: bl       #0x3140ec // _ZNSsC1EPKcRKSaIcE
003b4f28: mov      r1, r8
003b4f2c: mov      r0, sl
003b4f30: bl       #0x337a88 // _ZN13DebugSwitches9GetSwitchERKSs
003b4f34: mov      r0, r8
003b4f38: bl       #0x318254 // _ZNSsD1Ev
003b4f3c: ldr      r0, [r4, #0x59c]
003b4f40: bl       #0x30e964 // 
003b4f44: movw     r1, #0x74bc
003b4f48: movt     r1, #0x3c13
003b4f4c: bl       #0x30ed6c // 
003b4f50: str      r0, [r4, #0x120]
003b4f54: ldr      r0, [r4, #0x5a0]
003b4f58: bl       #0x30e964 // 
003b4f5c: movw     r1, #0x74bc
003b4f60: movt     r1, #0x3c13
003b4f64: bl       #0x30ed6c // 
003b4f68: str      r0, [r4, #0x124]
003b4f6c: ldr      r0, [r4, #0x5a4]
003b4f70: bl       #0x30e964 // 
003b4f74: movw     r1, #0xd70a
003b4f78: movt     r1, #0x3c23
003b4f7c: bl       #0x30ed6c // 
003b4f80: str      r0, [r4, #0x128]
003b4f84: mov      r0, r4
003b4f88: bl       #0x38be5c // _ZN10GameObject8InitPostEv
003b4f8c: mov      r0, r4
003b4f90: bl       #0x38ab60 // _ZNK10GameObject13MeetConditionEv
003b4f94: subs     r1, r0, #0
003b4f98: beq      #0x3b5488
003b4f9c: mov      r1, #2
003b4fa0: mov      r0, r4
003b4fa4: bl       #0x3bc4d0 // _ZN9Character7SG_LoadEi
003b4fa8: ldr      r3, [pc, #0x600]
003b4fac: mov      r0, r4
003b4fb0: ldr      r3, [r5, r3]
003b4fb4: ldr      r8, [r3]
003b4fb8: bl       #0x3a2fec // _ZNK9Character11GetCharAIIdEv
003b4fbc: mov      r3, #0x44
003b4fc0: mla      r8, r3, r0, r8
003b4fc4: ldr      r3, [r4]
003b4fc8: mov      r0, r4
003b4fcc: mov      lr, pc
003b4fd0: ldr      pc, [r3, #0x28]
003b4fd4: cmp      r0, #0
003b4fd8: bne      #0x3b4ff4
003b4fdc: ldrb     r3, [r8, #0x10]
003b4fe0: cmp      r3, #0
003b4fe4: beq      #0x3b4ff4
003b4fe8: mov      r3, #1
003b4fec: strb     r3, [r4, #0x3ec]
003b4ff0: b        #0x3b5014 // 
003b4ff4: ldr      r3, [r4]
003b4ff8: mov      r0, r4
003b4ffc: mov      lr, pc
003b5000: ldr      pc, [r3, #0x28]
003b5004: cmp      r0, #0
003b5008: bne      #0x3b54a4
003b500c: add      r0, r4, #0x3c8
003b5010: bl       #0x3cf1f0 // _ZN6CharAI17LoadScriptProcessEv
003b5014: movw     r3, #0x1488
003b5018: ldr      r2, [pc, #0x594]
003b501c: ldr      r1, [r4, r3]
003b5020: ldr      r3, [r8, #0x30]
003b5024: str      r2, [sp, #0xc]
003b5028: mov      r2, r4
003b502c: add      r1, r1, r3
003b5030: ldr      r3, [sp, #0xc]
003b5034: ldr      sb, [pc, #0x57c]
003b5038: add      sl, sp, #0x94
003b503c: ldr      r0, [r5, r3]
003b5040: bl       #0x495430 // _ZN15VisualFXManager10GrabAnimFXEiP10GameObject
003b5044: movw     r3, #0x1484
003b5048: str      r0, [r4, r3]
003b504c: mov      r0, r4
003b5050: bl       #0x3b4738 // _ZN9Character24RegisterCharacterFXTableEv
003b5054: ldr      r8, [r5, fp]
003b5058: add      sb, pc, sb
003b505c: mov      r0, r8
003b5060: bl       #0x337888 // _ZN13DebugSwitches4loadEv
003b5064: add      r2, sp, #0x3c
003b5068: mov      r0, sl
003b506c: mov      r1, sb
003b5070: bl       #0x3140ec // _ZNSsC1EPKcRKSaIcE
003b5074: mov      r1, sl
003b5078: mov      r0, r8
003b507c: bl       #0x337a88 // _ZN13DebugSwitches9GetSwitchERKSs
003b5080: mov      r0, sl
003b5084: bl       #0x318254 // _ZNSsD1Ev
003b5088: add      r0, r4, #0x490
003b508c: add      r0, r0, #0xc
003b5090: bl       #0x3c9f4c // _ZN12CharAnimator15SetAnimationSetEv
003b5094: add      sl, sp, #0x7c
003b5098: mov      r0, r8
003b509c: bl       #0x337888 // _ZN13DebugSwitches4loadEv
003b50a0: add      r2, sp, #0x38
003b50a4: mov      r0, sl
003b50a8: mov      r1, sb
003b50ac: bl       #0x3140ec // _ZNSsC1EPKcRKSaIcE
003b50b0: mov      r1, sl
003b50b4: mov      r0, r8
003b50b8: bl       #0x337a88 // _ZN13DebugSwitches9GetSwitchERKSs
003b50bc: mov      r0, sl
003b50c0: bl       #0x318254 // _ZNSsD1Ev
003b50c4: mov      r0, r4
003b50c8: bl       #0x3b3b00 // _ZN9Character11_InitSoundsEv
003b50cc: add      sl, sp, #0x64
003b50d0: mov      r0, r8
003b50d4: bl       #0x337888 // _ZN13DebugSwitches4loadEv
003b50d8: add      r2, sp, #0x34
003b50dc: mov      r1, sb
003b50e0: mov      r0, sl
003b50e4: bl       #0x3140ec // _ZNSsC1EPKcRKSaIcE
003b50e8: mov      r1, sl
003b50ec: mov      r0, r8
003b50f0: bl       #0x337a88 // _ZN13DebugSwitches9GetSwitchERKSs
003b50f4: mov      r0, sl
003b50f8: bl       #0x318254 // _ZNSsD1Ev
003b50fc: ldr      r3, [r4]
003b5100: mov      r0, r4
003b5104: mov      lr, pc
003b5108: ldr      pc, [r3, #0x28]
003b510c: cmp      r0, #0
003b5110: beq      #0x3b538c
003b5114: ldr      r3, [r4, #0x2d8]
003b5118: cmp      r3, #0
003b511c: beq      #0x3b512c
003b5120: ldr      r0, [r3, #8]
003b5124: mov      r1, #0
003b5128: bl       #0x59719c // _ZN6glitch5scene10ISceneNode19setAutomaticCullingENS0_14E_CULLING_TYPEE
003b512c: ldr      r2, [pc, #0x470]
003b5130: mov      r0, r4
003b5134: mov      r1, #4
003b5138: str      r2, [sp, #0x10]
003b513c: bl       #0x3bc4d0 // _ZN9Character7SG_LoadEi
003b5140: ldr      r3, [sp, #0x10]
003b5144: ldr      r0, [r5, r3]
003b5148: bl       #0x31f594 // _ZNK11Application15GetCurrentLevelEv
003b514c: cmp      r0, #0
003b5150: beq      #0x3b5160
003b5154: ldr      r1, [r0, #0x118]
003b5158: mov      r0, r4
003b515c: bl       #0x3bb950 // _ZN9Character20SG_SetGameDifficultyEi
003b5160: ldr      r2, [sp, #0x10]
003b5164: mov      r1, r4
003b5168: ldr      r3, [r5, r2]
003b516c: ldr      r0, [r3, #0x40]
003b5170: bl       #0x36effc // _ZN13PlayerManager13IsLocalPlayerEPK9Character
003b5174: cmp      r0, #0
003b5178: bne      #0x3b54dc
003b517c: movw     r3, #0x13c8
003b5180: ldrsh    r1, [r4, r3]
003b5184: mov      r0, r7
003b5188: bl       #0x3df2a4 // _ZN14CharProperties18LoadBasePropertiesEi
003b518c: mov      r0, r7
003b5190: bl       #0x3df480 // _ZN14CharProperties19LoadGearsPropertiesEv
003b5194: mov      r0, r7
003b5198: mov      r1, #1
003b519c: bl       #0x3e0810 // _ZN14CharProperties16RecalcPropertiesEb
003b51a0: ldr      r2, [sp, #0x10]
003b51a4: mov      r1, r4
003b51a8: ldr      r3, [r5, r2]
003b51ac: ldr      r0, [r3, #0x40]
003b51b0: bl       #0x36effc // _ZN13PlayerManager13IsLocalPlayerEPK9Character
003b51b4: cmp      r0, #0
003b51b8: bne      #0x3b54d0
003b51bc: movw     r3, #0xc9ff
003b51c0: movt     r3, #0x3b9a
003b51c4: str      r3, [r4, #0x3a4]
003b51c8: mov      r2, #0
003b51cc: mov      r0, r7
003b51d0: mov      r1, #0xc2
003b51d4: bl       #0x3df6e0 // _ZNK14CharProperties12PROPS_GetIntEib
003b51d8: ldr      r2, [sp, #0xc]
003b51dc: bic      r0, r0, r0, asr #31
003b51e0: strb     r0, [r4, #0x3a8]
003b51e4: ldr      r3, [r5, r2]
003b51e8: ldr      r2, [r3, #0x1c]
003b51ec: ldr      r3, [r3, #0x20]
003b51f0: rsb      r3, r2, r3
003b51f4: asr      r3, r3, #3
003b51f8: add      r2, r3, r3, lsl #2
003b51fc: add      r2, r2, r2, lsl #4
003b5200: add      r2, r2, r2, lsl #8
003b5204: add      r2, r2, r2, lsl #16
003b5208: add      r3, r3, r2, lsl #1
003b520c: cmp      r3, #0
003b5210: beq      #0x3b538c
003b5214: mov      r0, #0x24
003b5218: mov      r1, #0
003b521c: bl       #0x31056c // _Znaj15MemoryHintState
003b5220: movw     r3, #0x1494
003b5224: str      r0, [r4, r3]
003b5228: ldr      r3, [pc, #0x38c]
003b522c: mov      sb, r0
003b5230: ldr      r3, [r5, r3]
003b5234: ldr      sl, [r3]
003b5238: cmp      sl, #0
003b523c: beq      #0x3b550c
003b5240: ldr      r3, [pc, #0x378]
003b5244: ldr      r2, [pc, #0x378]
003b5248: str      r7, [sp, #0x18]
003b524c: ldr      r3, [r5, r3]
003b5250: add      r2, pc, r2
003b5254: mov      r8, #0
003b5258: ldr      r3, [r3]
003b525c: str      r0, [sp, #0x14]
003b5260: mov      sb, r2
003b5264: mov      r7, r3
003b5268: b        #0x3b5278 // 
003b526c: add      r8, r8, #1
003b5270: cmp      r8, sl
003b5274: beq      #0x3b5504
003b5278: mov      r0, sb
003b527c: ldr      r1, [r7, r8, lsl #2]
003b5280: bl       #0x30e31c // 
003b5284: cmp      r0, #0
003b5288: bne      #0x3b526c
003b528c: ldr      sb, [sp, #0x14]
003b5290: ldr      r7, [sp, #0x18]
003b5294: mov      r1, r8
003b5298: ldr      r3, [sp, #0xc]
003b529c: str      r7, [sp, #0x14]
003b52a0: str      fp, [sp, #0x18]
003b52a4: ldr      r2, [r5, r3]
003b52a8: ldr      r3, [pc, #0x318]
003b52ac: str      r6, [sp, #0x1c]
003b52b0: mov      r8, #0
003b52b4: movw     sl, #0x1494
003b52b8: mov      r7, r1
003b52bc: mov      r6, r2
003b52c0: mov      fp, r3
003b52c4: b        #0x3b52cc // 
003b52c8: ldr      sb, [r4, sl]
003b52cc: mov      r0, r6
003b52d0: add      r1, r7, r8
003b52d4: mov      r2, #0
003b52d8: bl       #0x495430 // _ZN15VisualFXManager10GrabAnimFXEiP10GameObject
003b52dc: str      r0, [sb, r8, lsl #2]
003b52e0: ldr      r3, [r4, sl]
003b52e4: ldr      r3, [r3, r8, lsl #2]
003b52e8: cmp      r3, #0
003b52ec: beq      #0x3b5350
003b52f0: ldr      r2, [r5, fp]
003b52f4: mov      r0, r3
003b52f8: mov      r1, #0
003b52fc: ldr      lr, [r2]
003b5300: ldr      ip, [r2, #4]
003b5304: ldr      r2, [r2, #8]
003b5308: str      lr, [r3, #0x34]
003b530c: str      ip, [r3, #0x38]
003b5310: str      r2, [r3, #0x3c]
003b5314: bl       #0x492aa0 // _ZN10AnimatedFX11SyncIrrDataEb
003b5318: ldr      r3, [r4, sl]
003b531c: mov      r1, #0
003b5320: ldr      r0, [r3, r8, lsl #2]
003b5324: bl       #0x492ef0 // _ZN10AnimatedFX10SetVisibleEb
003b5328: ldr      r3, [r4, sl]
003b532c: ldr      r0, [r3, r8, lsl #2]
003b5330: bl       #0x49267c // _ZN10AnimatedFX11GetAnimatorEv
003b5334: ldr      r3, [r0]
003b5338: mov      lr, pc
003b533c: ldr      pc, [r3, #0x44]
003b5340: mov      r1, #1
003b5344: ldr      r3, [r0]
003b5348: mov      lr, pc
003b534c: ldr      pc, [r3, #0x40]
003b5350: add      r8, r8, #1
003b5354: cmp      r8, #9
003b5358: bne      #0x3b52c8
003b535c: ldr      r2, [sp, #0x10]
003b5360: mov      r1, r4
003b5364: ldr      r7, [sp, #0x14]
003b5368: ldr      r3, [r5, r2]
003b536c: ldr      fp, [sp, #0x18]
003b5370: ldr      r6, [sp, #0x1c]
003b5374: ldr      r0, [r3, #0x40]
003b5378: bl       #0x36effc // _ZN13PlayerManager13IsLocalPlayerEPK9Character
003b537c: cmp      r0, #0
003b5380: bne      #0x3b5514
003b5384: mov      r0, r4
003b5388: bl       #0x3a41a0 // _ZN9Character23AddMultiplayerHighlightEv
003b538c: add      r8, r4, #0xff0
003b5390: add      r8, r8, #4
003b5394: mov      r1, r8
003b5398: mov      r2, #0xd2
003b539c: mov      r0, r7
003b53a0: bl       #0x3dedb4 // _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003b53a4: bl       #0x30e964 // 
003b53a8: mov      r3, #0x1440
003b53ac: str      r0, [r4, r3]
003b53b0: mov      r2, #0xd3
003b53b4: mov      r1, r8
003b53b8: mov      r0, r7
003b53bc: bl       #0x3dedb4 // _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003b53c0: bl       #0x30e964 // 
003b53c4: movw     r3, #0x1444
003b53c8: str      r0, [r4, r3]
003b53cc: add      r1, r4, #0x160
003b53d0: mov      r0, r4
003b53d4: bl       #0x3a58f4 // _ZN9Character18SetInitialPositionERK7Point3DIfE
003b53d8: add      r1, r4, #0x1440
003b53dc: mov      r0, r4
003b53e0: add      r1, r1, #0x10
003b53e4: mov      r2, #1
003b53e8: bl       #0x393db4 // _ZN10GameObject11SetPositionERK7Point3DIfEb
003b53ec: ldr      ip, [r4, #0x16c]
003b53f0: ldr      r0, [r4, #0x2d8]
003b53f4: ldr      r1, [r4, #0x170]
003b53f8: ldr      r2, [r4, #0x174]
003b53fc: movw     r3, #0x145c
003b5400: str      ip, [r4, r3]
003b5404: movw     r3, #0x1460
003b5408: str      r1, [r4, r3]
003b540c: cmp      r0, #0
003b5410: movw     r3, #0x1464
003b5414: str      r2, [r4, r3]
003b5418: beq      #0x3b5420
003b541c: bl       #0x470a54 // _ZN12VisualObject12ApplyMeshBoxEv
003b5420: mov      r1, #0
003b5424: mov      r2, #1
003b5428: mov      r0, r4
003b542c: bl       #0x3a59ac // _ZN9Character6ReviveEP10GameObjectb
003b5430: mov      r0, r4
003b5434: bl       #0x3b3a70 // _ZN9Character9_InitHpMpEv
003b5438: ldrb     r1, [r4, #0x3ec]
003b543c: cmp      r1, #0
003b5440: beq      #0x3b54c4
003b5444: mov      r0, r4
003b5448: bl       #0x3d37d0 // _ZN6CharAI10AddToGroupEP9Character
003b544c: ldr      r7, [r5, fp]
003b5450: add      r4, sp, #0x4c
003b5454: mov      r0, r7
003b5458: bl       #0x337888 // _ZN13DebugSwitches4loadEv
003b545c: ldr      r1, [pc, #0x168]
003b5460: add      r2, sp, #0x30
003b5464: mov      r0, r4
003b5468: add      r1, pc, r1
003b546c: bl       #0x3140ec // _ZNSsC1EPKcRKSaIcE
003b5470: mov      r0, r7
003b5474: mov      r1, r4
003b5478: bl       #0x337a88 // _ZN13DebugSwitches9GetSwitchERKSs
003b547c: mov      r0, r4
003b5480: bl       #0x318254 // _ZNSsD1Ev
003b5484: b        #0x3b4d94 // 
003b5488: mov      r0, r4
003b548c: ldr      r3, [r4]
003b5490: mov      lr, pc
003b5494: ldr      pc, [r3, #0x40]
003b5498: mov      r0, r4
003b549c: bl       #0x33ddb4 // _ZN10ObjectBase6DeleteEv
003b54a0: b        #0x3b4d94 // 
003b54a4: ldr      r3, [pc, #0xf8]
003b54a8: mov      r1, r4
003b54ac: ldr      r3, [r5, r3]
003b54b0: ldr      r0, [r3, #0x40]
003b54b4: bl       #0x36effc // _ZN13PlayerManager13IsLocalPlayerEPK9Character
003b54b8: cmp      r0, #0
003b54bc: bne      #0x3b500c
003b54c0: b        #0x3b4fe8 // 
003b54c4: add      r0, r4, #0x3c8
003b54c8: bl       #0x3ce7c0 // _ZN6CharAI17InitScriptProcessEb
003b54cc: b        #0x3b5444 // 
003b54d0: mov      r0, r4
003b54d4: bl       #0x3b3a90 // _ZN9Character16_InitSkillsSlotsEv
003b54d8: b        #0x3b51bc // 
003b54dc: mov      r0, r4
003b54e0: bl       #0x3b395c // _ZN9Character14_InitEquipmentEv
003b54e4: mov      r0, r7
003b54e8: bl       #0x3defac // _ZN14CharProperties20ResetGearsPropertiesEv
003b54ec: movw     r3, #0x14e8
003b54f0: ldr      r0, [r4, r3]
003b54f4: cmp      r0, #0
003b54f8: beq      #0x3b517c
003b54fc: bl       #0x4679e8 // _ZN14PlayerSavegame15SG_TryQuestSyncEv
003b5500: b        #0x3b517c // 
003b5504: ldr      sb, [sp, #0x14]
003b5508: ldr      r7, [sp, #0x18]
003b550c: mvn      r1, #0
003b5510: b        #0x3b5298 // 
003b5514: ldr      r3, [sp, #0xc]
003b5518: mov      r2, #0
003b551c: movw     r8, #0x149c
003b5520: ldr      r0, [r5, r3]
003b5524: ldr      r3, [pc, #0xa4]
003b5528: ldr      r3, [r5, r3]
003b552c: ldr      r3, [r3]
003b5530: ldr      r1, [r3, #0x88]
003b5534: bl       #0x495430 // _ZN15VisualFXManager10GrabAnimFXEiP10GameObject
003b5538: cmp      r0, #0
003b553c: str      r0, [r4, r8]
003b5540: beq      #0x3b5384
003b5544: mov      r2, #0
003b5548: str      r2, [r0, #0x3c]
003b554c: str      r2, [r0, #0x34]
003b5550: str      r2, [r0, #0x38]
003b5554: mov      r1, #0
003b5558: bl       #0x492aa0 // _ZN10AnimatedFX11SyncIrrDataEb
003b555c: mov      r1, #0
003b5560: ldr      r0, [r4, r8]
003b5564: bl       #0x492ef0 // _ZN10AnimatedFX10SetVisibleEb
003b5568: ldr      r0, [r4, r8]
003b556c: bl       #0x49267c // _ZN10AnimatedFX11GetAnimatorEv
003b5570: ldr      r3, [r0]
003b5574: mov      lr, pc
003b5578: ldr      pc, [r3, #0x44]
003b557c: mov      r1, #1
003b5580: ldr      r3, [r0]
003b5584: mov      lr, pc
003b5588: ldr      pc, [r3, #0x40]
003b558c: b        #0x3b5384 // 
003b5590: bl       #0x30e310 // 
003b5594: subseq   pc, sp, r0, lsr #26
003b5598: andeq    r4, r0, ip, lsr #1
003b559c: andeq    r0, r0, r4, lsl #17
003b55a0: subseq   pc, r0, r8, lsr #32
003b55a4: strdeq   r3, r4, [r0], -r4
003b55a8: subseq   lr, r0, ip, ror #30
003b55ac: ldrsheq  lr, [r0], #-0xe0
003b55b0: andeq    r0, r0, r8, asr r7
003b55b4: andeq    r1, r0, r8, lsl #22
003b55b8: ldrheq   lr, [r0], #-0xd8
003b55bc: andeq    r0, r0, r4, asr #13
003b55c0: muleq    r0, r4, r2
003b55c4: subseq   lr, r0, r0, ror #24
003b55c8: andeq    r3, r0, ip, lsr #30
003b55cc: subseq   lr, r0, r8, lsr #19
003b55d0: andeq    r3, r0, r8, asr #5
# _ZN9Character6ReviveEP10GameObjectb 0x3a59ac
003a59ac: push     {r4, r5, r6, r7, lr}
003a59b0: movw     r3, #0x1449
003a59b4: ldrb     r3, [r0, r3]
003a59b8: ldr      r5, [pc, #0x11c]
003a59bc: sub      sp, sp, #0x24
003a59c0: cmp      r3, #0
003a59c4: mov      r4, r0
003a59c8: mov      r6, r2
003a59cc: add      r5, pc, r5
003a59d0: bne      #0x3a5a44
003a59d4: mov      r2, #1
003a59d8: movw     r3, #0x1448
003a59dc: strb     r2, [r4, r3]
003a59e0: mov      r7, #0
003a59e4: movw     r3, #0x1449
003a59e8: strb     r7, [r4, r3]
003a59ec: mov      r0, r4
003a59f0: bl       #0x3b3a70 // _ZN9Character9_InitHpMpEv
003a59f4: strb     r7, [r4, #0x118]
003a59f8: bl       #0x7fd794 // _Z9GetOnlinev
003a59fc: ldrb     r3, [r0, #5]
003a5a00: cmp      r3, r7
003a5a04: mvnne    r3, #0
003a5a08: strne    r3, [r4, #0x110]
003a5a0c: movne    r3, #0
003a5a10: strne    r3, [r4, #0x114]
003a5a14: cmp      r6, #0
003a5a18: bne      #0x3a5ad0
003a5a1c: ldr      r3, [r4]
003a5a20: mov      r0, r4
003a5a24: mov      lr, pc
003a5a28: ldr      pc, [r3, #0x28]
003a5a2c: cmp      r0, #0
003a5a30: bne      #0x3a5a54
003a5a34: add      r0, r4, #0x3c8
003a5a38: bl       #0x3d8894 // _ZN6CharAI15UpdateAllSkillsEv
003a5a3c: add      sp, sp, #0x24
003a5a40: pop      {r4, r5, r6, r7, pc}
003a5a44: mov      r1, #3
003a5a48: mov      r2, #0
003a5a4c: bl       #0x3a4d5c // _ZN9Character10RaiseEventEiPv
003a5a50: b        #0x3a59d4 // 
003a5a54: bl       #0x7fd794 // _Z9GetOnlinev
003a5a58: ldrb     ip, [r0, #5]
003a5a5c: cmp      ip, #0
003a5a60: bne      #0x3a5a34
003a5a64: ldr      r3, [pc, #0x74]
003a5a68: add      r2, sp, #0x1c
003a5a6c: ldr      r0, [r5, r3]
003a5a70: movw     r3, #0x147c
003a5a74: ldr      lr, [r4, r3]
003a5a78: movw     r3, #0x1474
003a5a7c: ldr      r7, [r4, r3]
003a5a80: movw     r3, #0x1478
003a5a84: ldr      r6, [r4, r3]
003a5a88: add      r5, sp, #0x10
003a5a8c: mov      r3, ip
003a5a90: mov      r1, r5
003a5a94: str      r7, [sp, #0x10]
003a5a98: str      r6, [sp, #0x14]
003a5a9c: str      lr, [sp, #0x1c]
003a5aa0: str      lr, [sp, #0x18]
003a5aa4: str      ip, [sp]
003a5aa8: str      ip, [sp, #4]
003a5aac: str      ip, [sp, #8]
003a5ab0: bl       #0x525508 // _ZN7PFWorld16GetFloorHeightAtERK7Point3DIfEPfPS1_PP6PFRoomPP7PFFloorb
003a5ab4: ldr      r3, [sp, #0x1c]
003a5ab8: mov      r1, r5
003a5abc: mov      r0, r4
003a5ac0: mov      r2, #1
003a5ac4: str      r3, [sp, #0x18]
003a5ac8: bl       #0x393db4 // _ZN10GameObject11SetPositionERK7Point3DIfEb
003a5acc: b        #0x3a5a34 // 
003a5ad0: mov      r0, r4
003a5ad4: bl       #0x3b4088 // _ZN9Character18InitPhysicalObjectEv
003a5ad8: b        #0x3a5a1c // 
003a5adc: subseq   pc, lr, r4, asr #1
003a5ae0: andeq    r1, r0, r4, lsl #4
# _ZN9Character10RaiseEventEiPv 0x3a4d5c
003a4d5c: cmp      r1, #0x36
003a4d60: beq      #0x3a4d6c
003a4d64: add      r0, r0, #0x3c8
003a4d68: b        #0x3cbb34 // _ZN6CharAI12RaiseAIEventEiPv
003a4d6c: add      r0, r0, #0x560
003a4d70: mov      r1, r2
003a4d74: b        #0x3e123c // _ZN14CharProperties11BuffExpiredEPN10CharTimers5TimerE
# _ZN6CharAI10OnInitPostEv 0x3d0b80
003d0b80: push     {r4, lr}
003d0b84: ldr      r3, [r0, #0x1c]
003d0b88: cmp      r3, #0
003d0b8c: beq      #0x3d0ba0
003d0b90: mov      r0, r3
003d0b94: ldr      r3, [r3]
003d0b98: mov      lr, pc
003d0b9c: ldr      pc, [r3, #0xc]
003d0ba0: pop      {r4, pc}
# _ZN6CharAI14OnStateChangedEii 0x3d0bec
003d0bec: push     {r4, lr}
003d0bf0: ldr      r3, [r0, #0x1c]
003d0bf4: cmp      r3, #0
003d0bf8: beq      #0x3d0c0c
003d0bfc: mov      r0, r3
003d0c00: ldr      r3, [r3]
003d0c04: mov      lr, pc
003d0c08: ldr      pc, [r3, #0x20]
003d0c0c: pop      {r4, pc}
# _ZNK9Character16GetPreSetAIStateEv 0x3a5784
003a5784: push     {r4, lr}
003a5788: movw     r3, #0x13dc
003a578c: ldr      r2, [r0, r3]
003a5790: movw     r3, #0x13e0
003a5794: ldr      r3, [r0, r3]
003a5798: cmp      r2, r3
003a579c: beq      #0x3a57e4
003a57a0: ldr      r1, [pc, #0x44]
003a57a4: add      r4, r0, #0x13c0
003a57a8: add      r4, r4, #0xc
003a57ac: add      r1, pc, r1
003a57b0: mov      r0, r4
003a57b4: bl       #0x3a5720 // _ZNKSs7compareEPKc
003a57b8: cmp      r0, #0
003a57bc: bne      #0x3a57c4
003a57c0: pop      {r4, pc}
003a57c4: ldr      r1, [pc, #0x24]
003a57c8: mov      r0, r4
003a57cc: add      r1, pc, r1
003a57d0: bl       #0x3a5720 // _ZNKSs7compareEPKc
003a57d4: cmp      r0, #0
003a57d8: bne      #0x3a57e4
003a57dc: mov      r0, #0x11
003a57e0: pop      {r4, pc}
003a57e4: mov      r0, #3
003a57e8: pop      {r4, pc}
003a57ec: subseq   sp, r1, r4, lsl #22
003a57f0: subseq   sp, r1, ip, ror #21
# _ZN9Character9InitFinalEv 0x3b4978
003b4978: push     {r4, r5, r6, r7, r8, lr}
003b497c: ldr      r4, [pc, #0x22c]
003b4980: ldr      r6, [pc, #0x22c]
003b4984: movw     r3, #0x1395
003b4988: add      r4, pc, r4
003b498c: ldr      r2, [r4, r6]
003b4990: ldrb     r1, [r0, r3]
003b4994: sub      sp, sp, #0x48
003b4998: ldr      r2, [r2]
003b499c: cmp      r1, #0
003b49a0: mov      r5, r0
003b49a4: str      r2, [sp, #0x44]
003b49a8: beq      #0x3b49c8
003b49ac: ldr      r3, [r4, r6]
003b49b0: ldr      r2, [sp, #0x44]
003b49b4: ldr      r3, [r3]
003b49b8: cmp      r2, r3
003b49bc: bne      #0x3b4bac
003b49c0: add      sp, sp, #0x48
003b49c4: pop      {r4, r5, r6, r7, r8, pc}
003b49c8: mov      r2, #1
003b49cc: strb     r2, [r0, r3]
003b49d0: bl       #0x38bd64 // _ZN10GameObject21CheckSpawnProbabilityEv
003b49d4: ldr      r3, [r5, #0x274]
003b49d8: cmp      r0, r3
003b49dc: bge      #0x3b49ac
003b49e0: mov      r0, r5
003b49e4: bl       #0x38cd48 // _ZN10GameObject9InitFinalEv
003b49e8: mov      r0, r5
003b49ec: bl       #0x3a3094 // _ZNK9Character8IsFaerieEv
003b49f0: cmp      r0, #0
003b49f4: beq      #0x3b4b98
003b49f8: ldr      r3, [pc, #0x1b8]
003b49fc: mov      r1, #0
003b4a00: mov      r2, #1
003b4a04: ldr      r3, [r4, r3]
003b4a08: ldr      r0, [r3, #0x40]
003b4a0c: bl       #0x36e478 // _ZN13PlayerManager14GetLocalPlayerEib
003b4a10: ldr      r7, [r0, #0x660]
003b4a14: mov      r3, #0
003b4a18: str      r3, [sp, #8]
003b4a1c: cmp      r7, #0
003b4a20: str      r3, [sp]
003b4a24: str      r3, [sp, #4]
003b4a28: beq      #0x3b4a88
003b4a2c: mov      r1, sp
003b4a30: mov      r0, r7
003b4a34: bl       #0x393ae4 // _ZNK10GameObject12GetLookAtVecER7Point3DIfE
003b4a38: mov      r0, r7
003b4a3c: bl       #0x3935dc // _ZNK10GameObject17GetTargetPositionEv
003b4a40: ldr      r1, [r0]
003b4a44: mov      r7, r0
003b4a48: ldr      r0, [sp]
003b4a4c: bl       #0x30eba4 // 
003b4a50: str      r0, [sp]
003b4a54: ldr      r1, [r7, #4]
003b4a58: ldr      r0, [sp, #4]
003b4a5c: bl       #0x30eba4 // 
003b4a60: str      r0, [sp, #4]
003b4a64: ldr      r1, [r7, #8]
003b4a68: ldr      r0, [sp, #8]
003b4a6c: bl       #0x30eba4 // 
003b4a70: mov      r1, sp
003b4a74: str      r0, [sp, #8]
003b4a78: mov      r2, #1
003b4a7c: mov      r0, r5
003b4a80: mov      r8, sp
003b4a84: bl       #0x393db4 // _ZN10GameObject11SetPositionERK7Point3DIfEb
003b4a88: ldr      r3, [r5, #0x2d8]
003b4a8c: cmp      r3, #0
003b4a90: beq      #0x3b4af8
003b4a94: ldr      r3, [r5]
003b4a98: mov      r0, r5
003b4a9c: mov      lr, pc
003b4aa0: ldr      pc, [r3, #0x28]
003b4aa4: cmp      r0, #0
003b4aa8: beq      #0x3b4b7c
003b4aac: ldr      r3, [pc, #0x104]
003b4ab0: ldr      r1, [pc, #0x104]
003b4ab4: add      r7, sp, #0x2c
003b4ab8: ldr      r3, [r4, r3]
003b4abc: add      r1, pc, r1
003b4ac0: add      r2, sp, #0x10
003b4ac4: ldr      r3, [r3, #0x10]
003b4ac8: mov      r0, r7
003b4acc: ldr      r8, [r3, #0x1c]
003b4ad0: bl       #0x3140ec // _ZNSsC1EPKcRKSaIcE
003b4ad4: add      r8, r8, #0x294
003b4ad8: mov      r0, r8
003b4adc: mov      r1, r7
003b4ae0: bl       #0x40c3cc // _ZN15LightSetManager21GetLightSetIdFromNameESs
003b4ae4: mov      r8, r0
003b4ae8: mov      r0, r7
003b4aec: bl       #0x318254 // _ZNSsD1Ev
003b4af0: ldr      r3, [r5, #0x2d8]
003b4af4: str      r8, [r3, #0x40]
003b4af8: add      r7, r5, #0x3c8
003b4afc: mov      r0, r7
003b4b00: ldr      r3, [r5, #0x3c8]
003b4b04: mov      lr, pc
003b4b08: ldr      pc, [r3, #0x10]
003b4b0c: ldr      r3, [r5]
003b4b10: mov      r0, r5
003b4b14: mov      lr, pc
003b4b18: ldr      pc, [r3, #0x28]
003b4b1c: cmp      r0, #0
003b4b20: beq      #0x3b49ac
003b4b24: ldr      r3, [pc, #0x8c]
003b4b28: mov      r1, r5
003b4b2c: ldr      r3, [r4, r3]
003b4b30: ldr      r0, [r3, #0x40]
003b4b34: bl       #0x36effc // _ZN13PlayerManager13IsLocalPlayerEPK9Character
003b4b38: cmp      r0, #0
003b4b3c: beq      #0x3b49ac
003b4b40: mov      r0, r7
003b4b44: add      r7, r5, #0x560
003b4b48: bl       #0x3d8894 // _ZN6CharAI15UpdateAllSkillsEv
003b4b4c: mov      r0, r7
003b4b50: mov      r1, #1
003b4b54: bl       #0x3e0810 // _ZN14CharProperties16RecalcPropertiesEb
003b4b58: mov      r0, r7
003b4b5c: mov      r1, #0xc2
003b4b60: mov      r2, #0
003b4b64: bl       #0x3df6e0 // _ZNK14CharProperties12PROPS_GetIntEib
003b4b68: bic      r0, r0, r0, asr #31
003b4b6c: strb     r0, [r5, #0x3a8]
003b4b70: mov      r0, r5
003b4b74: bl       #0x3bc4a8 // _ZN9Character7SG_SaveEv
003b4b78: b        #0x3b49ac // 
003b4b7c: ldr      r3, [pc, #0x34]
003b4b80: ldr      r1, [pc, #0x38]
003b4b84: add      r7, sp, #0x14
003b4b88: ldr      r3, [r4, r3]
003b4b8c: add      r1, pc, r1
003b4b90: add      r2, sp, #0xc
003b4b94: b        #0x3b4ac4 // 
003b4b98: mov      r0, r5
003b4b9c: bl       #0x3a307c // _ZNK9Character10IsFollowerEv
003b4ba0: cmp      r0, #0
003b4ba4: beq      #0x3b4a88
003b4ba8: b        #0x3b49f8 // 
003b4bac: bl       #0x30e310 // 
003b4bb0: subseq   r0, lr, r8, lsl #2
003b4bb4: andeq    r4, r0, ip, lsr #1
003b4bb8: strdeq   r3, r4, [r0], -r4
003b4bbc: subseq   pc, r0, ip, ror r3
# _ZN6CharAI17InitScriptProcessEb 0x3ce7c0
003ce7c0: push     {r4, r5, r6, lr}
003ce7c4: mov      r4, r0
003ce7c8: ldr      r0, [r0, #4]
003ce7cc: mov      r5, r1
003ce7d0: bl       #0x3b3a70 // _ZN9Character9_InitHpMpEv
003ce7d4: mov      r0, r4
003ce7d8: bl       #0x3ce044 // _ZN6CharAI18SetSkillsAndSpellsEv
003ce7dc: mov      r0, r4
003ce7e0: bl       #0x3d8894 // _ZN6CharAI15UpdateAllSkillsEv
003ce7e4: ldr      r3, [r4]
003ce7e8: mov      r0, r4
003ce7ec: mov      lr, pc
003ce7f0: ldr      pc, [r3, #0xc]
003ce7f4: cmp      r5, #0
003ce7f8: beq      #0x3ce80c
003ce7fc: mov      r0, r4
003ce800: ldr      r3, [r4]
003ce804: mov      lr, pc
003ce808: ldr      pc, [r3, #0x10]
003ce80c: pop      {r4, r5, r6, pc}
# _ZN10AISDefault14OnStateChangedEii 0x3dbe8c
003dbe8c: bx       lr
# _ZN9Character7InitAllEv 0x3b35f0
003b35f0: push     {r4, lr}
003b35f4: mov      r4, r0
003b35f8: ldr      r3, [r0]
003b35fc: mov      lr, pc
003b3600: ldr      pc, [r3, #0x1c]
003b3604: mov      r0, r4
003b3608: ldr      r3, [r4]
003b360c: mov      lr, pc
003b3610: ldr      pc, [r3, #0x58]
003b3614: pop      {r4, pc}
# _ZN9Character18InitPhysicalObjectEv 0x3b4088
003b4088: push     {r4, r5, r6, r7, r8, sl, lr}
003b408c: sub      sp, sp, #0x24
003b4090: mov      r5, r0
003b4094: bl       #0x3a30dc // _ZNK9Character14IsInvisibleManEv
003b4098: ldr      r4, [pc, #0x320]
003b409c: subs     r6, r0, #0
003b40a0: add      r4, pc, r4
003b40a4: bne      #0x3b41a8
003b40a8: ldrb     r7, [r5, #0x84]
003b40ac: cmp      r7, #0
003b40b0: bne      #0x3b412c
003b40b4: mov      r0, r5
003b40b8: bl       #0x3a310c // _ZNK9Character5IsNPCEv
003b40bc: subs     r6, r0, #0
003b40c0: beq      #0x3b41f0
003b40c4: ldr      r3, [pc, #0x2f8]
003b40c8: mov      r1, r7
003b40cc: mov      r0, #0x28
003b40d0: ldr      r3, [r4, r3]
003b40d4: ldr      r8, [r3, #0x44]
003b40d8: bl       #0x310570 // _Znwj15MemoryHintState
003b40dc: mov      ip, #0x400
003b40e0: mvn      r3, #3
003b40e4: str      ip, [sp]
003b40e8: mov      r1, r8
003b40ec: movw     ip, #0xd1f
003b40f0: mov      r2, r5
003b40f4: mov      r6, r0
003b40f8: str      ip, [sp, #4]
003b40fc: str      r7, [sp, #8]
003b4100: bl       #0x3b4010 // 
003b4104: ldr      r3, [pc, #0x2bc]
003b4108: ldr      r3, [r4, r3]
003b410c: mov      r0, r5
003b4110: mov      r1, r6
003b4114: add      r3, r3, #8
003b4118: mov      r2, #1
003b411c: str      r3, [r6]
003b4120: add      sp, sp, #0x24
003b4124: pop      {r4, r5, r6, r7, r8, sl, lr}
003b4128: b        #0x394bf8 // _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
003b412c: ldr      r3, [pc, #0x290]
003b4130: mov      r1, r6
003b4134: mov      r0, #0x28
003b4138: ldr      r3, [r4, r3]
003b413c: mov      r7, #1
003b4140: ldr      sl, [r3, #0x44]
003b4144: bl       #0x310570 // _Znwj15MemoryHintState
003b4148: mov      ip, #2
003b414c: mov      r1, sl
003b4150: mov      r2, r5
003b4154: mov      r3, r7
003b4158: str      ip, [sp, #0x10]
003b415c: movw     ip, #0xffff
003b4160: mov      r8, r0
003b4164: str      r6, [sp, #0xc]
003b4168: str      ip, [sp, #0x14]
003b416c: str      r6, [sp]
003b4170: str      r6, [sp, #4]
003b4174: str      r6, [sp, #8]
003b4178: str      r7, [sp, #0x18]
003b417c: bl       #0x46f2f0 // _ZN14PhysicalObjectC2EP13PhysicalWorldP10GameObjectbbbbstti
003b4180: ldr      r3, [pc, #0x244]
003b4184: mov      r0, r5
003b4188: mov      r1, r8
003b418c: ldr      r3, [r4, r3]
003b4190: mov      r2, r7
003b4194: add      r3, r3, #8
003b4198: str      r3, [r8]
003b419c: add      sp, sp, #0x24
003b41a0: pop      {r4, r5, r6, r7, r8, sl, lr}
003b41a4: b        #0x394bf8 // _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
003b41a8: ldr      r3, [pc, #0x214]
003b41ac: mov      r1, #0
003b41b0: mov      r0, #0x28
003b41b4: ldr      r3, [r4, r3]
003b41b8: ldr      r7, [r3, #0x44]
003b41bc: bl       #0x310570 // _Znwj15MemoryHintState
003b41c0: mov      ip, #0
003b41c4: mov      r3, ip
003b41c8: mov      lr, #0x100
003b41cc: mov      r1, r7
003b41d0: mov      r2, r5
003b41d4: mov      r6, r0
003b41d8: str      lr, [sp]
003b41dc: str      ip, [sp, #4]
003b41e0: str      ip, [sp, #8]
003b41e4: bl       #0x3b4010 // 
003b41e8: ldr      r3, [pc, #0x1e0]
003b41ec: b        #0x3b4108 // 
003b41f0: ldr      r3, [r5]
003b41f4: mov      r0, r5
003b41f8: mov      lr, pc
003b41fc: ldr      pc, [r3, #0x28]
003b4200: cmp      r0, #0
003b4204: bne      #0x3b4264
003b4208: mov      r0, r5
003b420c: bl       #0x3a307c // _ZNK9Character10IsFollowerEv
003b4210: cmp      r0, #0
003b4214: beq      #0x3b42d0
003b4218: ldr      r3, [pc, #0x1a4]
003b421c: mov      r1, #0
003b4220: mov      r0, #0x28
003b4224: ldr      r3, [r4, r3]
003b4228: ldr      r7, [r3, #0x44]
003b422c: bl       #0x310570 // _Znwj15MemoryHintState
003b4230: mov      ip, #8
003b4234: str      ip, [sp]
003b4238: movw     ip, #0xd3b
003b423c: mvn      r3, #1
003b4240: str      ip, [sp, #4]
003b4244: mov      r1, r7
003b4248: mov      ip, #0
003b424c: mov      r2, r5
003b4250: mov      r6, r0
003b4254: str      ip, [sp, #8]
003b4258: bl       #0x3b4010 // 
003b425c: ldr      r3, [pc, #0x170]
003b4260: b        #0x3b4108 // 
003b4264: ldr      r3, [pc, #0x158]
003b4268: mov      r1, r6
003b426c: mov      r0, #0x28
003b4270: ldr      r3, [r4, r3]
003b4274: mov      r7, #1
003b4278: ldr      r8, [r3, #0x44]
003b427c: bl       #0x310570 // _Znwj15MemoryHintState
003b4280: mov      ip, #4
003b4284: mov      r1, r8
003b4288: mov      r2, r5
003b428c: str      ip, [sp]
003b4290: mvn      r3, #0
003b4294: movw     ip, #0xd7f
003b4298: mov      r6, r0
003b429c: str      ip, [sp, #4]
003b42a0: str      r7, [sp, #8]
003b42a4: bl       #0x3b4010 // 
003b42a8: ldr      r3, [pc, #0x128]
003b42ac: mov      r0, r5
003b42b0: mov      r1, r6
003b42b4: ldr      r3, [r4, r3]
003b42b8: mov      r2, r7
003b42bc: add      r3, r3, #8
003b42c0: str      r3, [r6]
003b42c4: add      sp, sp, #0x24
003b42c8: pop      {r4, r5, r6, r7, r8, sl, lr}
003b42cc: b        #0x394bf8 // _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
003b42d0: mov      r0, r5
003b42d4: bl       #0x3a30ac // _ZNK9Character10IsSummonedEv
003b42d8: subs     r6, r0, #0
003b42dc: bne      #0x3b4218
003b42e0: mov      r0, r5
003b42e4: bl       #0x3a3094 // _ZNK9Character8IsFaerieEv
003b42e8: subs     r7, r0, #0
003b42ec: beq      #0x3b4354
003b42f0: ldr      r3, [pc, #0xcc]
003b42f4: mov      r1, r6
003b42f8: mov      r0, #0x28
003b42fc: ldr      r3, [r4, r3]
003b4300: ldr      r8, [r3, #0x44]
003b4304: bl       #0x310570 // _Znwj15MemoryHintState
003b4308: mov      r1, r8
003b430c: mov      r3, r6
003b4310: mov      r2, r5
003b4314: mov      ip, #0x100
003b4318: mov      r7, r0
003b431c: str      ip, [sp]
003b4320: str      r6, [sp, #4]
003b4324: str      r6, [sp, #8]
003b4328: bl       #0x3b4010 // 
003b432c: ldr      r3, [pc, #0x9c]
003b4330: mov      r0, r5
003b4334: mov      r1, r7
003b4338: ldr      r3, [r4, r3]
003b433c: mov      r2, #1
003b4340: add      r3, r3, #8
003b4344: str      r3, [r7]
003b4348: add      sp, sp, #0x24
003b434c: pop      {r4, r5, r6, r7, r8, sl, lr}
003b4350: b        #0x394bf8 // _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
003b4354: mov      r0, r5
003b4358: bl       #0x3a3064 // _ZNK9Character9IsMonsterEv
003b435c: subs     r1, r0, #0
003b4360: beq      #0x3b43ac
003b4364: ldr      r3, [pc, #0x58]
003b4368: mov      r1, r7
003b436c: mov      r0, #0x28
003b4370: ldr      r3, [r4, r3]
003b4374: ldr      r8, [r3, #0x44]
003b4378: bl       #0x310570 // _Znwj15MemoryHintState
003b437c: mov      ip, #0x10
003b4380: mov      r3, #2
003b4384: str      ip, [sp]
003b4388: mov      r1, r8
003b438c: movw     ip, #0xd3f
003b4390: mov      r2, r5
003b4394: mov      r6, r0
003b4398: str      ip, [sp, #4]
003b439c: str      r7, [sp, #8]
003b43a0: bl       #0x3b4010 // 
003b43a4: ldr      r3, [pc, #0x30]
003b43a8: b        #0x3b4108 // 
003b43ac: mov      r0, r5
003b43b0: mov      r2, r1
003b43b4: add      sp, sp, #0x24
003b43b8: pop      {r4, r5, r6, r7, r8, sl, lr}
003b43bc: b        #0x394bf8 // _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
003b43c0: ldrsheq  r0, [lr], #-0x90
003b43c4: strdeq   r3, r4, [r0], -r4
003b43c8: andeq    r3, r0, ip, asr pc
003b43cc: andeq    r2, r0, r8, lsl r4
003b43d0: andeq    r0, r0, ip, asr #26
003b43d4: andeq    r3, r0, r4, lsr #4
003b43d8: andeq    r1, r0, ip, asr sp
003b43dc: andeq    r1, r0, r0, lsr #5
# _ZNK9Character18GetCharAnimTableIdEv 0x3a3228
003a3228: mov      r3, #0x1000
003a322c: ldr      r0, [r0, r3]
003a3230: ldr      r3, [pc, #0x24]
003a3234: cmp      r0, #0
003a3238: add      r3, pc, r3
003a323c: blt      #0x3a3254
003a3240: ldr      r2, [pc, #0x18]
003a3244: ldr      r3, [r3, r2]
003a3248: ldr      r3, [r3]
003a324c: cmp      r0, r3
003a3250: bxlt     lr
003a3254: mov      r0, #0x11
003a3258: bx       lr
003a325c: subseq   r1, pc, r8, asr r8
003a3260: andeq    r2, r0, r0, asr #17
