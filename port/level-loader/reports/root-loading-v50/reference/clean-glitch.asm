# 0x31f55c _ZN11Application11CleanGlitchEv
0031f55c: ldr r3, [pc, #0x28]
0031f560: ldr r2, [pc, #0x28]
0031f564: push {r4, lr}
0031f568: add r3, pc, r3
0031f56c: ldr r2, [r3, r2]
0031f570: ldr r3, [r2, #0x10]
0031f574: ldr r3, [r3, #0x10]
0031f578: mov r0, r3
0031f57c: ldr r3, [r3]
0031f580: mov lr, pc
0031f584: ldr pc, [r3, #0xac]
0031f588: pop {r4, pc}
0031f58c: rsbeq r5, r7, r8, lsr #10
0031f590: strdeq r3, r4, [r0], -r4

# 0x59f300 _ZN6glitch5video9C2DDriver12freeTexturesEv
0059f300: push {r4, lr}
0059f304: ldr r3, [r0, #8]
0059f308: mov r4, r0
0059f30c: sub sp, sp, #8
0059f310: mov r0, r3
0059f314: ldr r3, [r3]
0059f318: mov lr, pc
0059f31c: ldr pc, [r3, #0x1fc]
0059f320: ldr r0, [r4, #0x10]
0059f324: cmp r0, #0
0059f328: beq #0x59f350
0059f32c: mov r2, #0
0059f330: add r3, sp, #8
0059f334: ldrh r1, [r4, #0x14]
0059f338: str r2, [r3, #-4]!
0059f33c: bl #0x5cd324
0059f340: ldr r0, [sp, #4]
0059f344: cmp r0, #0
0059f348: beq #0x59f350
0059f34c: bl #0x31d584
0059f350: ldr r0, [r4, #0x18]
0059f354: cmp r0, #0
0059f358: beq #0x59f384
0059f35c: mov r2, #0
0059f360: add r3, sp, #8
0059f364: ldrh r1, [r4, #0x1c]
0059f368: str r2, [r3, #-8]!
0059f36c: mov r3, sp
0059f370: bl #0x5cd324
0059f374: ldr r0, [sp]
0059f378: cmp r0, #0
0059f37c: beq #0x59f384
0059f380: bl #0x31d584
0059f384: add sp, sp, #8
0059f388: pop {r4, pc}

# 0x5aa290 _ZN6glitch5video12IVideoDriver12removeUnusedEv
005aa290: push {r4, r5, r6, lr}
005aa294: ldr r3, [r0, #0xd4]
005aa298: mov r4, r0
005aa29c: ldr r0, [r3, #0x14]
005aa2a0: bl #0x59f300
005aa2a4: ldr r0, [r4, #0xdc]
005aa2a8: bl #0x5dad20
005aa2ac: ldr r5, [r4, #0xdc]
005aa2b0: mov r0, r5
005aa2b4: bl #0x5d9e94
005aa2b8: mov r1, #0
005aa2bc: mov r0, r5
005aa2c0: bl #0x5da080
005aa2c4: ldr r4, [r4, #0xe0]
005aa2c8: mov r0, r4
005aa2cc: bl #0x5e828c
005aa2d0: mov r0, r4
005aa2d4: mov r1, #0
005aa2d8: pop {r4, r5, r6, lr}
005aa2dc: b #0x5e9f78

# 0x5d9e94 _ZN6glitch5video24CMaterialRendererManager20clearUnusedInstancesEv
005d9e94: push {r4, r5, r6, lr}
005d9e98: ldr r4, [r0, #8]
005d9e9c: mov r5, r0
005d9ea0: cmp r4, r0
005d9ea4: beq #0x5d9ee0
005d9ea8: mov r0, r5
005d9eac: ldrh r1, [r4, #0x22]
005d9eb0: bl #0x5d9db8
005d9eb4: ldr r2, [r4, #0xc]
005d9eb8: cmp r2, #0
005d9ebc: bne #0x5d9ec8
005d9ec0: b #0x5d9ee4
005d9ec4: mov r2, r3
005d9ec8: ldr r3, [r2, #8]
005d9ecc: cmp r3, #0
005d9ed0: bne #0x5d9ec4
005d9ed4: mov r4, r2
005d9ed8: cmp r5, r4
005d9edc: bne #0x5d9ea8
005d9ee0: pop {r4, r5, r6, pc}
005d9ee4: ldr r3, [r4, #4]
005d9ee8: ldr r1, [r3, #0xc]
005d9eec: cmp r1, r4
005d9ef0: bne #0x5d9f0c
005d9ef4: mov r4, r3
005d9ef8: ldr r3, [r3, #4]
005d9efc: ldr r2, [r3, #0xc]
005d9f00: cmp r2, r4
005d9f04: beq #0x5d9ef4
005d9f08: ldr r2, [r4, #0xc]
005d9f0c: cmp r2, r3
005d9f10: movne r4, r3
005d9f14: b #0x5d9ed8

# 0x5da080 _ZN6glitch4core6detail15SIDedCollectionIN5boost13intrusive_ptrINS_5video17CMaterialRendererEEEtLb0ENS5_6detail23materialrenderermanager11SPropertiesENS1_15sidedcollection12SValueTraitsEE9removeAllEb
005da080: push {r4, r5, r6, r7, r8, lr}
005da084: ldr r3, [r0, #8]
005da088: mov r5, r0
005da08c: mov r7, r1
005da090: cmp r5, r3
005da094: mov r6, #0
005da098: beq #0x5da0e4
005da09c: ldr r4, [r3, #0xc]
005da0a0: cmp r4, #0
005da0a4: bne #0x5da0b0
005da0a8: b #0x5da0ec
005da0ac: mov r4, r2
005da0b0: ldr r2, [r4, #8]
005da0b4: cmp r2, #0
005da0b8: bne #0x5da0ac
005da0bc: ldrh r1, [r3, #0x22]
005da0c0: mov r0, r5
005da0c4: mov r2, r7
005da0c8: bl #0x5d9f74
005da0cc: cmp r0, #0
005da0d0: addne r6, r6, #1
005da0d4: uxthne r6, r6
005da0d8: mov r3, r4
005da0dc: cmp r5, r3
005da0e0: bne #0x5da09c
005da0e4: mov r0, r6
005da0e8: pop {r4, r5, r6, r7, r8, pc}
005da0ec: ldr r2, [r3, #4]
005da0f0: ldr r1, [r2, #0xc]
005da0f4: cmp r3, r1
005da0f8: movne r4, r3
005da0fc: movne r1, #0
005da100: bne #0x5da11c
005da104: mov r4, r2
005da108: ldr r2, [r2, #4]
005da10c: ldr r1, [r2, #0xc]
005da110: cmp r1, r4
005da114: beq #0x5da104
005da118: ldr r1, [r4, #0xc]
005da11c: cmp r1, r2
005da120: movne r4, r2
005da124: ldrh r1, [r3, #0x22]
005da128: mov r0, r5
005da12c: mov r2, r7
005da130: bl #0x5d9f74
005da134: cmp r0, #0
005da138: addne r6, r6, #1
005da13c: uxthne r6, r6
005da140: mov r3, r4
005da144: b #0x5da0dc

# 0x5dad20 _ZN6glitch5video24CMaterialRendererManager19removeAllBatchBakerEv
005dad20: ldr r1, [pc, #0x260]
005dad24: ldr r3, [pc, #0x260]
005dad28: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005dad2c: add r1, pc, r1
005dad30: ldr r3, [r1, r3]
005dad34: sub sp, sp, #0x1c
005dad38: str r0, [sp]
005dad3c: ldr r4, [r3, #8]
005dad40: mov r5, r3
005dad44: str r1, [sp, #8]
005dad48: cmp r4, r5
005dad4c: beq #0x5dad88
005dad50: ldr r0, [r4, #0x14]
005dad54: bl #0x31d584
005dad58: ldr r1, [r4, #0xc]
005dad5c: cmp r1, #0
005dad60: mov r2, r1
005dad64: bne #0x5dad70
005dad68: b #0x5daf08
005dad6c: mov r2, r3
005dad70: ldr r3, [r2, #8]
005dad74: cmp r3, #0
005dad78: bne #0x5dad6c
005dad7c: mov r4, r2
005dad80: cmp r4, r5
005dad84: bne #0x5dad50
005dad88: ldr r3, [r4, #0x10]
005dad8c: cmp r3, #0
005dad90: beq #0x5dadac
005dad94: ldr r0, [r4, #4]
005dad98: bl #0x5d9734
005dad9c: mov r3, #0
005dada0: str r3, [r4, #0x10]
005dada4: stmib r4, {r3, r4}
005dada8: str r4, [r4, #0xc]
005dadac: ldr r2, [sp]
005dadb0: ldr r3, [pc, #0x1d8]
005dadb4: add r1, sp, #0x14
005dadb8: ldr sl, [r2, #8]
005dadbc: mov fp, #0xc
005dadc0: str r3, [sp, #0xc]
005dadc4: mov sb, #0x34
005dadc8: str r1, [sp, #4]
005dadcc: ldr r3, [sp]
005dadd0: cmp r3, sl
005dadd4: beq #0x5daef8
005dadd8: ldr r2, [sp]
005daddc: ldr r3, [r2, #0x18]
005dade0: ldr r1, [r2, #0x1c]
005dade4: ldrh r2, [sl, #0x22]
005dade8: rsb r1, r3, r1
005dadec: cmp r2, r1, asr #3
005dadf0: ldrhs r2, [sp, #8]
005dadf4: ldrhs r1, [sp, #0xc]
005dadf8: addlo r3, r3, r2, lsl #3
005dadfc: ldrhs r3, [r2, r1]
005dae00: ldr r3, [r3]
005dae04: cmp r3, #0
005dae08: str r3, [sp, #0x14]
005dae0c: ldrne r2, [r3]
005dae10: addne r2, r2, #1
005dae14: strne r2, [r3]
005dae18: ldrne r3, [sp, #0x14]
005dae1c: ldrb r2, [r3, #0x10]
005dae20: cmp r2, #0
005dae24: beq #0x5daec0
005dae28: mov r8, #0
005dae2c: ldr r2, [r3, #0x18]
005dae30: mul r3, fp, r8
005dae34: ldr r7, [r2, r3]
005dae38: add r3, r2, r3
005dae3c: cmp r7, #0
005dae40: ldrne r2, [r7]
005dae44: addne r2, r2, #1
005dae48: strne r2, [r7]
005dae4c: ldrb r6, [r3, #4]
005dae50: ldr r5, [r3, #8]
005dae54: cmp r6, #0
005dae58: beq #0x5dae84
005dae5c: sub r6, r6, #1
005dae60: uxtb r6, r6
005dae64: mla r6, r6, sb, sb
005dae68: mov r4, #0
005dae6c: add r3, r5, r4
005dae70: ldr r0, [r3, #0x20]
005dae74: add r4, r4, #0x34
005dae78: bl #0x5e49b8
005dae7c: cmp r4, r6
005dae80: bne #0x5dae6c
005dae84: cmp r7, #0
005dae88: beq #0x5daea8
005dae8c: ldr r3, [r7]
005dae90: sub r3, r3, #1
005dae94: cmp r3, #0
005dae98: str r3, [r7]
005dae9c: bne #0x5daea8
005daea0: mov r0, r7
005daea4: bl #0x6a4d9c
005daea8: ldr r3, [sp, #0x14]
005daeac: add r8, r8, #1
005daeb0: uxtb r8, r8
005daeb4: ldrb r2, [r3, #0x10]
005daeb8: cmp r2, r8
005daebc: bhi #0x5dae2c
005daec0: ldr r0, [sp, #4]
005daec4: bl #0x3522b8
005daec8: ldr r1, [sl, #0xc]
005daecc: cmp r1, #0
005daed0: beq #0x5daf48
005daed4: mov sl, r1
005daed8: b #0x5daee0
005daedc: mov sl, r3
005daee0: ldr r3, [sl, #8]
005daee4: cmp r3, #0
005daee8: bne #0x5daedc
005daeec: ldr r3, [sp]
005daef0: cmp r3, sl
005daef4: bne #0x5dadd8
005daef8: ldr r0, [r3, #0x28]
005daefc: bl #0x5d8624
005daf00: add sp, sp, #0x1c
005daf04: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005daf08: ldr r3, [r4, #4]
005daf0c: ldr r2, [r3, #0xc]
005daf10: cmp r4, r2
005daf14: beq #0x5daf20
005daf18: b #0x5daf3c
005daf1c: mov r3, r2
005daf20: ldr r2, [r3, #4]
005daf24: ldr r1, [r2, #0xc]
005daf28: cmp r1, r3
005daf2c: beq #0x5daf1c
005daf30: mov r4, r3
005daf34: ldr r1, [r3, #0xc]
005daf38: mov r3, r2
005daf3c: cmp r3, r1
005daf40: movne r4, r3
005daf44: b #0x5dad48
005daf48: ldr r3, [sl, #4]
005daf4c: ldr r2, [r3, #0xc]
005daf50: cmp sl, r2
005daf54: beq #0x5daf60
005daf58: b #0x5daf7c
005daf5c: mov r3, r2
005daf60: ldr r2, [r3, #4]
005daf64: ldr r1, [r2, #0xc]
005daf68: cmp r1, r3
005daf6c: beq #0x5daf5c
005daf70: mov sl, r3
005daf74: ldr r1, [r3, #0xc]
005daf78: mov r3, r2
005daf7c: cmp r3, r1
005daf80: movne sl, r3
005daf84: b #0x5dadcc
005daf88: eorseq sb, fp, r4, ror #26
005daf8c: andeq r2, r0, r4, ror #24
005daf90: ldrdeq r3, r4, [r0], -ip

# 0x5e828c _ZN6glitch5video15CTextureManager17clearPlaceHoldersEv
005e828c: mov r3, #0
005e8290: add r0, r0, #0x48
005e8294: mov r1, r3
005e8298: ldr r2, [r0, r3]
005e829c: cmp r2, #0
005e82a0: beq #0x5e82b0
005e82a4: ldr r2, [r2, #4]
005e82a8: cmp r2, #1
005e82ac: streq r1, [r0, r3]
005e82b0: add r3, r3, #4
005e82b4: cmp r3, #0x20
005e82b8: bne #0x5e8298
005e82bc: bx lr

# 0x5e9f78 _ZN6glitch4core6detail15SIDedCollectionIN5boost13intrusive_ptrINS_5video8ITextureEEEtLb0ENS5_6detail14texturemanager18STexturePropertiesENS1_15sidedcollection12SValueTraitsEE9removeAllEb
005e9f78: push {r4, r5, r6, r7, r8, lr}
005e9f7c: ldr r3, [r0, #8]
005e9f80: mov r5, r0
005e9f84: mov r7, r1
005e9f88: cmp r5, r3
005e9f8c: mov r6, #0
005e9f90: beq #0x5e9fdc
005e9f94: ldr r4, [r3, #0xc]
005e9f98: cmp r4, #0
005e9f9c: bne #0x5e9fa8
005e9fa0: b #0x5e9fe4
005e9fa4: mov r4, r2
005e9fa8: ldr r2, [r4, #8]
005e9fac: cmp r2, #0
005e9fb0: bne #0x5e9fa4
005e9fb4: ldrh r1, [r3, #0x34]
005e9fb8: mov r0, r5
005e9fbc: mov r2, r7
005e9fc0: bl #0x5e9e7c
005e9fc4: cmp r0, #0
005e9fc8: addne r6, r6, #1
005e9fcc: uxthne r6, r6
005e9fd0: mov r3, r4
005e9fd4: cmp r5, r3
005e9fd8: bne #0x5e9f94
005e9fdc: mov r0, r6
005e9fe0: pop {r4, r5, r6, r7, r8, pc}
005e9fe4: ldr r2, [r3, #4]
005e9fe8: ldr r1, [r2, #0xc]
005e9fec: cmp r3, r1
005e9ff0: movne r4, r3
005e9ff4: movne r1, #0
005e9ff8: bne #0x5ea014
005e9ffc: mov r4, r2
005ea000: ldr r2, [r2, #4]
005ea004: ldr r1, [r2, #0xc]
005ea008: cmp r1, r4
005ea00c: beq #0x5e9ffc
005ea010: ldr r1, [r4, #0xc]
005ea014: cmp r1, r2
005ea018: movne r4, r2
005ea01c: ldrh r1, [r3, #0x34]
005ea020: mov r0, r5
005ea024: mov r2, r7
005ea028: bl #0x5e9e7c
005ea02c: cmp r0, #0
005ea030: addne r6, r6, #1
005ea034: uxthne r6, r6
005ea038: mov r3, r4
005ea03c: b #0x5e9fd4
