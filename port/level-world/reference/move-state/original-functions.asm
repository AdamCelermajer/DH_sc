
# _ZNK9Character29IsUpdatingPositionFromPhysicsEv
003a2e44: ldr      r0, [r0, #0x520]
003a2e48: ubfx     r0, r0, #1, #1
003a2e4c: bx       lr

# _ZN12CharAnimator13ANIM_SetSpeedEf
003c93fc: push     {r4, lr}
003c9400: ldr      r2, [r0, #4]
003c9404: str      r1, [r0, #0x40]
003c9408: mov      r3, r0
003c940c: ldr      r2, [r2, #0x2d8]
003c9410: cmp      r2, #0
003c9414: beq      #0x3c9440
003c9418: mov      r0, r1
003c941c: ldr      r1, [r3, #0x34]
003c9420: ldr      r4, [r2, #0x38]
003c9424: bl       #0x30ed6c
003c9428: ldr      r3, [r4]
003c942c: mov      r1, r0
003c9430: mov      r2, #0
003c9434: mov      r0, r4
003c9438: mov      lr, pc
003c943c: ldr      pc, [r3, #0x28]
003c9440: pop      {r4, pc}

# _ZNK14CharProperties18PROPS_GetWalkSpeedEv
003de6c4: push     {r4, lr}
003de6c8: ldr      r0, [r0, #0xb50]
003de6cc: bl       #0x30e964
003de6d0: mov      r1, #0x3b800000
003de6d4: bl       #0x30ed6c
003de6d8: movw     r1, #0xd70a
003de6dc: movt     r1, #0x3c23
003de6e0: bl       #0x30ed6c
003de6e4: mov      r1, #0x3f800000
003de6e8: bl       #0x30eba4
003de6ec: mov      r1, #0
003de6f0: mov      r4, r0
003de6f4: bl       #0x30e2f8
003de6f8: cmp      r0, #0
003de6fc: moveq    r4, #0
003de700: mov      r0, r4
003de704: pop      {r4, pc}

# _ZN12CharAnimator8ANIM_SetEi
003cacb0: ldrb     r2, [r0, #0x49]
003cacb4: cmp      r2, #0
003cacb8: strne    r1, [r0, #0x50]
003cacbc: bxne     lr
003cacc0: mov      ip, #0x3f800000
003cacc4: str      ip, [r0, #0x40]
003cacc8: b        #0x3cab38

# _ZNK9Character28IsUpdatingRotationFromVisualEv
003a2e50: ldr      r0, [r0, #0x520]
003a2e54: ubfx     r0, r0, #2, #1
003a2e58: bx       lr

# _ZN6CSMove10UpdateTypeEP9CharacterP16CharStateMachine
003c0f18: push     {r4, r5, r6, r7, r8, lr}
003c0f1c: mov      r0, r1
003c0f20: ldr      r3, [r1]
003c0f24: mov      r4, r1
003c0f28: mov      lr, pc
003c0f2c: ldr      pc, [r3, #0x28]
003c0f30: ldr      r6, [pc, #0x224]
003c0f34: cmp      r0, #0
003c0f38: add      r6, pc, r6
003c0f3c: beq      #0x3c1074
003c0f40: ldr      r0, [r4, #0x1b8]
003c0f44: ldr      r8, [r4, #0x1bc]
003c0f48: ldr      r7, [r4, #0x1c0]
003c0f4c: mov      r1, r0
003c0f50: bl       #0x30ed6c
003c0f54: mov      r1, r8
003c0f58: mov      r5, r0
003c0f5c: mov      r0, r8
003c0f60: bl       #0x30ed6c
003c0f64: mov      r1, r0
003c0f68: mov      r0, r5
003c0f6c: bl       #0x30eba4
003c0f70: mov      r1, r7
003c0f74: mov      r5, r0
003c0f78: mov      r0, r7
003c0f7c: bl       #0x30ed6c
003c0f80: mov      r1, r0
003c0f84: mov      r0, r5
003c0f88: bl       #0x30eba4
003c0f8c: ldr      r3, [pc, #0x1cc]
003c0f90: ldr      r5, [r4, #0x53c]
003c0f94: mov      r7, r0
003c0f98: ldr      r3, [r6, r3]
003c0f9c: cmp      r5, #2
003c0fa0: ldr      r3, [r3]
003c0fa4: ldr      r8, [r3, #0x5c]
003c0fa8: ldr      r0, [r3, #0x60]
003c0fac: beq      #0x3c0fd8
003c0fb0: mov      r1, r0
003c0fb4: bl       #0x30ed6c
003c0fb8: mov      r1, r7
003c0fbc: bl       #0x30e70c
003c0fc0: cmp      r0, #0
003c0fc4: bne      #0x3c10f4
003c0fc8: cmp      r5, #0
003c0fcc: beq      #0x3c0ff4
003c0fd0: cmp      r5, #1
003c0fd4: beq      #0x3c1080
003c0fd8: mov      r1, r8
003c0fdc: mov      r0, r8
003c0fe0: bl       #0x30ed6c
003c0fe4: mov      r1, r7
003c0fe8: bl       #0x30e2f8
003c0fec: cmp      r0, #0
003c0ff0: beq      #0x3c1080
003c0ff4: mov      r3, #1
003c0ff8: str      r3, [r4, #0x53c]
003c0ffc: add      r0, r4, #0x560
003c1000: bl       #0x3de6c4
003c1004: ldr      r3, [pc, #0x158]
003c1008: str      r0, [r4, #0x52c]
003c100c: mov      r0, r4
003c1010: ldr      r3, [r6, r3]
003c1014: add      r5, r4, #0x490
003c1018: add      r5, r5, #0xc
003c101c: ldr      r7, [r3]
003c1020: bl       #0x3a3228
003c1024: ldr      r3, [pc, #0x13c]
003c1028: ldr      r1, [pc, #0x13c]
003c102c: ldr      r2, [r6, r3]
003c1030: mov      r3, #0xa0
003c1034: mla      r3, r3, r0, r7
003c1038: ldr      r0, [r2, #0x2c]
003c103c: ldr      r2, [pc, #0x12c]
003c1040: add      r1, pc, r1
003c1044: ldr      r6, [r3, #0x94]
003c1048: add      r2, pc, r2
003c104c: bl       #0x4c4bdc
003c1050: ands     r0, r0, #0x10
003c1054: bne      #0x3c10e8
003c1058: add      r1, r0, r6
003c105c: mov      r0, r5
003c1060: bl       #0x3cacb0
003c1064: ldr      r1, [r4, #0x52c]
003c1068: mov      r0, r5
003c106c: pop      {r4, r5, r6, r7, r8, lr}
003c1070: b        #0x3c93fc
003c1074: ldr      r3, [r4, #0x53c]
003c1078: cmp      r3, #0
003c107c: beq      #0x3c1084
003c1080: pop      {r4, r5, r6, r7, r8, pc}
003c1084: mov      r3, #1
003c1088: str      r3, [r4, #0x53c]
003c108c: add      r0, r4, #0x560
003c1090: bl       #0x3de6c4
003c1094: ldr      r3, [pc, #0xc8]
003c1098: str      r0, [r4, #0x52c]
003c109c: mov      r0, r4
003c10a0: ldr      r3, [r6, r3]
003c10a4: add      r5, r4, #0x490
003c10a8: add      r5, r5, #0xc
003c10ac: ldr      r7, [r3]
003c10b0: bl       #0x3a3228
003c10b4: ldr      r3, [pc, #0xac]
003c10b8: ldr      r1, [pc, #0xb4]
003c10bc: ldr      r2, [r6, r3]
003c10c0: mov      r3, #0xa0
003c10c4: mla      r3, r3, r0, r7
003c10c8: ldr      r0, [r2, #0x2c]
003c10cc: ldr      r2, [pc, #0xa4]
003c10d0: add      r1, pc, r1
003c10d4: ldr      r6, [r3, #0x94]
003c10d8: add      r2, pc, r2
003c10dc: bl       #0x4c4bdc
003c10e0: ands     r0, r0, #0x10
003c10e4: beq      #0x3c1058
003c10e8: mov      r0, r4
003c10ec: bl       #0x3a53e0
003c10f0: b        #0x3c1058
003c10f4: mov      r3, #2
003c10f8: str      r3, [r4, #0x53c]
003c10fc: add      r0, r4, #0x560
003c1100: bl       #0x3de6c4
003c1104: ldr      r3, [pc, #0x58]
003c1108: str      r0, [r4, #0x52c]
003c110c: mov      r0, r4
003c1110: ldr      r3, [r6, r3]
003c1114: add      r5, r4, #0x490
003c1118: add      r5, r5, #0xc
003c111c: ldr      r7, [r3]
003c1120: bl       #0x3a3228
003c1124: ldr      r3, [pc, #0x3c]
003c1128: ldr      r1, [pc, #0x4c]
003c112c: ldr      r2, [r6, r3]
003c1130: mov      r3, #0xa0
003c1134: mla      r3, r3, r0, r7
003c1138: ldr      r0, [r2, #0x2c]
003c113c: ldr      r2, [pc, #0x3c]
003c1140: add      r1, pc, r1
003c1144: ldr      r6, [r3, #0x70]
003c1148: add      r2, pc, r2
003c114c: bl       #0x4c4bdc
003c1150: ands     r0, r0, #0x20
003c1154: beq      #0x3c1058
003c1158: b        #0x3c10e8
003c115c: subseq   r3, sp, r8, asr fp
003c1160: andeq    r3, r0, r8, asr #5
003c1164: andeq    r4, r0, r4, asr #16
003c1168: strdeq   r3, r4, [r0], -r4
003c116c: subseq   r3, r0, r8, ror fp
003c1170: subseq   r3, r0, r0, lsl #23
003c1174: subseq   r3, r0, r8, ror #21
003c1178: ldrsheq  r3, [r0], #-0xa0
003c117c: subseq   r3, r0, r8, ror sl
003c1180: subseq   r3, r0, r0, lsl #21

# _ZN6CSMove8OnUpdateEiP9CharacterP16CharStateMachine
003c1184: push     {r4, r5, lr}
003c1188: ldrb     r1, [r2, #0x1b5]
003c118c: sub      sp, sp, #0xc
003c1190: mov      r4, r2
003c1194: cmp      r1, #0
003c1198: mov      r5, r0
003c119c: bne      #0x3c11b8
003c11a0: mov      r0, r4
003c11a4: mov      r1, #0x3f
003c11a8: mov      r2, #0
003c11ac: add      sp, sp, #0xc
003c11b0: pop      {r4, r5, lr}
003c11b4: b        #0x3a4d5c
003c11b8: ldr      r2, [r2, #0x408]
003c11bc: cmp      r2, #0
003c11c0: beq      #0x3c1224
003c11c4: mov      r2, r3
003c11c8: mov      r0, r5
003c11cc: mov      r1, r4
003c11d0: bl       #0x3c0f18
003c11d4: add      r0, r4, #0x560
003c11d8: bl       #0x3de6c4
003c11dc: ldr      r1, [r4, #0x52c]
003c11e0: mov      r5, r0
003c11e4: bl       #0x30e3ac
003c11e8: movw     r1, #0xb717
003c11ec: bic      r0, r0, #0x80000000
003c11f0: movt     r1, #0x38d1
003c11f4: bl       #0x30e70c
003c11f8: cmp      r0, #0
003c11fc: beq      #0x3c1208
003c1200: add      sp, sp, #0xc
003c1204: pop      {r4, r5, pc}
003c1208: add      r0, r4, #0x490
003c120c: add      r0, r0, #0xc
003c1210: mov      r1, r5
003c1214: str      r5, [r4, #0x52c]
003c1218: add      sp, sp, #0xc
003c121c: pop      {r4, r5, lr}
003c1220: b        #0x3c93fc
003c1224: ldr      r2, [r4]
003c1228: mov      r0, r4
003c122c: str      r3, [sp, #4]
003c1230: mov      lr, pc
003c1234: ldr      pc, [r2, #0x28]
003c1238: cmp      r0, #0
003c123c: ldr      r3, [sp, #4]
003c1240: beq      #0x3c1254
003c1244: ldrb     r2, [r4, #0x1b5]
003c1248: cmp      r2, #0
003c124c: beq      #0x3c1200
003c1250: b        #0x3c11c4
003c1254: mov      r0, r4
003c1258: bl       #0x39361c
003c125c: cmp      r0, #0
003c1260: ldr      r3, [sp, #4]
003c1264: beq      #0x3c1244
003c1268: ldr      r2, [r4]
003c126c: mov      r0, r4
003c1270: mov      lr, pc
003c1274: ldr      pc, [r2, #0x54]
003c1278: cmp      r0, #0
003c127c: ldr      r3, [sp, #4]
003c1280: bne      #0x3c1244
003c1284: b        #0x3c11a0

# _ZNK9Character14GetFocusObjectEv
003a2e24: ldr      r0, [r0, #0x408]
003a2e28: bx       lr

# _ZNK9Character29IsUpdatingRotationFromPhysicsEv
003a2e5c: ldr      r0, [r0, #0x520]
003a2e60: ubfx     r0, r0, #3, #1
003a2e64: bx       lr

# _ZNK9Character28IsUpdatingVisualWithRotationEv
003a2e68: ldr      r0, [r0, #0x520]
003a2e6c: eor      r0, r0, #0x10
003a2e70: ubfx     r0, r0, #4, #1
003a2e74: bx       lr

# _ZNK9Character13GetAnimStanceEv
003a53e0: push     {r4, r5, r6, lr}
003a53e4: ldr      r3, [r0]
003a53e8: mov      r4, r0
003a53ec: mov      lr, pc
003a53f0: ldr      pc, [r3, #0x28]
003a53f4: ldr      r5, [pc, #0xa8]
003a53f8: cmp      r0, #0
003a53fc: add      r5, pc, r5
003a5400: bne      #0x3a5438
003a5404: mov      r4, #0
003a5408: ldr      r3, [pc, #0x98]
003a540c: ldr      r1, [pc, #0x98]
003a5410: ldr      r2, [pc, #0x98]
003a5414: ldr      r3, [r5, r3]
003a5418: add      r1, pc, r1
003a541c: add      r2, pc, r2
003a5420: ldr      r0, [r3, #0x2c]
003a5424: bl       #0x4c4bdc
003a5428: cmp      r4, r0
003a542c: movlt    r0, r4
003a5430: movge    r0, #0
003a5434: pop      {r4, r5, r6, pc}
003a5438: add      r4, r4, #0x37c
003a543c: mov      r0, r4
003a5440: bl       #0x4000c8
003a5444: cmp      r0, #0
003a5448: movne    r4, #3
003a544c: bne      #0x3a5408
003a5450: mov      r0, r4
003a5454: bl       #0x400080
003a5458: cmp      r0, #0
003a545c: movne    r4, #4
003a5460: bne      #0x3a5408
003a5464: mov      r0, r4
003a5468: bl       #0x40019c
003a546c: subs     r1, r0, #0
003a5470: movne    r4, #2
003a5474: bne      #0x3a5408
003a5478: mov      r0, r4
003a547c: bl       #0x4001a0
003a5480: cmp      r0, #0
003a5484: movne    r4, #1
003a5488: bne      #0x3a5408
003a548c: mov      r0, r4
003a5490: bl       #0x3ffe8c
003a5494: cmp      r0, #0
003a5498: moveq    r4, #5
003a549c: beq      #0x3a5408
003a54a0: b        #0x3a5404

# _ZNK14CharProperties22PROPS_GetRotationSpeedEv
003de708: push     {r4, lr}
003de70c: ldr      r0, [r0, #0xb54]
003de710: bl       #0x30e964
003de714: mov      r1, #0x3b800000
003de718: bl       #0x30ed6c
003de71c: movw     r1, #0xd70a
003de720: movt     r1, #0x3c23
003de724: bl       #0x30ed6c
003de728: mov      r1, #0x3f800000
003de72c: bl       #0x30eba4
003de730: mov      r1, #0
003de734: mov      r4, r0
003de738: bl       #0x30e2f8
003de73c: cmp      r0, #0
003de740: moveq    r4, #0
003de744: mov      r0, r4
003de748: pop      {r4, pc}

# _ZNK9Character18GetCharAnimTableIdEv
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

# _ZN6CSMove6OnInitEiP9CharacterP16CharStateMachine
003c80ac: push     {r4, r5, r6, r7, r8, lr}
003c80b0: add      r5, r2, #0x4f0
003c80b4: add      r5, r5, #0xc
003c80b8: sub      sp, sp, #0x60
003c80bc: mov      r4, #0
003c80c0: mov      r6, r1
003c80c4: mov      r0, r5
003c80c8: mov      r2, #0x3f
003c80cc: mov      r3, #3
003c80d0: str      r4, [sp, #0x58]
003c80d4: str      r4, [sp, #0x5c]
003c80d8: str      r4, [sp]
003c80dc: str      r4, [sp, #4]
003c80e0: ldr      r8, [pc, #0x18c]
003c80e4: bl       #0x3c7b18
003c80e8: mov      r0, r5
003c80ec: mov      r1, r6
003c80f0: movw     r2, #0xc358
003c80f4: mov      r3, #0xc
003c80f8: str      r4, [sp, #0x50]
003c80fc: str      r4, [sp, #0x54]
003c8100: str      r4, [sp]
003c8104: str      r4, [sp, #4]
003c8108: bl       #0x3c7b18
003c810c: ldr      r3, [pc, #0x164]
003c8110: add      r8, pc, r8
003c8114: mov      r0, r5
003c8118: ldr      ip, [r8, r3]
003c811c: mov      r1, r6
003c8120: movw     r2, #0xc35a
003c8124: mov      r3, #0xb
003c8128: str      ip, [sp]
003c812c: str      ip, [sp, #0x48]
003c8130: str      r4, [sp, #0x4c]
003c8134: str      r4, [sp, #4]
003c8138: bl       #0x3c7b18
003c813c: ldr      r3, [pc, #0x138]
003c8140: mov      r0, r5
003c8144: mov      r1, r6
003c8148: ldr      r7, [r8, r3]
003c814c: movw     r2, #0xc35b
003c8150: mov      r3, #0xa
003c8154: str      r7, [sp, #0x40]
003c8158: str      r4, [sp, #0x44]
003c815c: str      r7, [sp]
003c8160: str      r4, [sp, #4]
003c8164: bl       #0x3c7b18
003c8168: mov      r0, r5
003c816c: mov      r1, r6
003c8170: movw     r2, #0xc35c
003c8174: mov      r3, #9
003c8178: str      r7, [sp, #0x38]
003c817c: str      r4, [sp, #0x3c]
003c8180: str      r7, [sp]
003c8184: str      r4, [sp, #4]
003c8188: bl       #0x3c7b18
003c818c: mov      r0, r5
003c8190: mov      r1, r6
003c8194: movw     r2, #0xc35d
003c8198: mov      r3, #8
003c819c: str      r7, [sp]
003c81a0: str      r7, [sp, #0x30]
003c81a4: str      r4, [sp, #0x34]
003c81a8: str      r4, [sp, #4]
003c81ac: bl       #0x3c7b18
003c81b0: mov      r0, r5
003c81b4: mov      r1, r6
003c81b8: movw     r2, #0xc356
003c81bc: mov      r3, #7
003c81c0: str      r4, [sp, #0x28]
003c81c4: str      r4, [sp, #0x2c]
003c81c8: str      r4, [sp]
003c81cc: str      r4, [sp, #4]
003c81d0: bl       #0x3c7b18
003c81d4: mov      r0, r5
003c81d8: mov      r1, r6
003c81dc: movw     r2, #0xc355
003c81e0: mov      r3, #6
003c81e4: str      r4, [sp, #0x20]
003c81e8: str      r4, [sp, #0x24]
003c81ec: str      r4, [sp]
003c81f0: str      r4, [sp, #4]
003c81f4: bl       #0x3c7b18
003c81f8: ldr      r3, [pc, #0x80]
003c81fc: mov      r0, r5
003c8200: mov      r1, r6
003c8204: ldr      ip, [r8, r3]
003c8208: movw     r2, #0xc354
003c820c: mov      r3, #5
003c8210: str      ip, [sp]
003c8214: str      ip, [sp, #0x18]
003c8218: str      r4, [sp, #0x1c]
003c821c: str      r4, [sp, #4]
003c8220: bl       #0x3c7b18
003c8224: mov      r0, r5
003c8228: mov      r1, r6
003c822c: movw     r2, #0xc353
003c8230: mov      r3, #0xd
003c8234: str      r4, [sp, #0x10]
003c8238: str      r4, [sp, #0x14]
003c823c: str      r4, [sp]
003c8240: str      r4, [sp, #4]
003c8244: bl       #0x3c7b18
003c8248: mov      r0, r5
003c824c: mov      r1, r6
003c8250: movw     r2, #0xc357
003c8254: mov      r3, #0xf
003c8258: str      r4, [sp, #4]
003c825c: str      r4, [sp, #8]
003c8260: str      r4, [sp, #0xc]
003c8264: str      r4, [sp]
003c8268: bl       #0x3c7b18
003c826c: add      sp, sp, #0x60
003c8270: pop      {r4, r5, r6, r7, r8, pc}
003c8274: subseq   ip, ip, r0, lsl #19
003c8278: andeq    r2, r0, r4, lsl #29
003c827c: andeq    r3, r0, ip, asr #9
003c8280: ldrdeq   r0, r1, [r0], -r4

# _ZN6CSMove6OnBlurEiP9CharacterP16CharStateMachinei
003c3aa4: push     {r4, r5, r6, r7, r8, lr}
003c3aa8: ldr      r4, [pc, #0x8c]
003c3aac: ldr      r6, [pc, #0x8c]
003c3ab0: ldr      r1, [pc, #0x8c]
003c3ab4: add      r4, pc, r4
003c3ab8: ldr      r3, [r4, r6]
003c3abc: ldr      r8, [r4, r1]
003c3ac0: sub      sp, sp, #0x20
003c3ac4: ldr      r3, [r3]
003c3ac8: mov      r0, r8
003c3acc: mov      r7, r2
003c3ad0: str      r3, [sp, #0x1c]
003c3ad4: bl       #0x337888
003c3ad8: ldr      r1, [pc, #0x68]
003c3adc: add      r5, sp, #4
003c3ae0: mov      r2, sp
003c3ae4: add      r1, pc, r1
003c3ae8: mov      r0, r5
003c3aec: bl       #0x3140ec
003c3af0: mov      r1, r5
003c3af4: mov      r0, r8
003c3af8: bl       #0x337a88
003c3afc: mov      r0, r5
003c3b00: bl       #0x318254
003c3b04: mov      r0, r7
003c3b08: bl       #0x3938f8
003c3b0c: ldr      r0, [r7, #0x2dc]
003c3b10: cmp      r0, #0
003c3b14: beq      #0x3c3b1c
003c3b18: bl       #0x46eb20
003c3b1c: ldr      r3, [r4, r6]
003c3b20: ldr      r2, [sp, #0x1c]
003c3b24: ldr      r3, [r3]
003c3b28: cmp      r2, r3
003c3b2c: bne      #0x3c3b38
003c3b30: add      sp, sp, #0x20
003c3b34: pop      {r4, r5, r6, r7, r8, pc}
003c3b38: bl       #0x30e310
003c3b3c: ldrsbeq  r0, [sp], #-0xfc
003c3b40: andeq    r4, r0, ip, lsr #1
003c3b44: andeq    r0, r0, r4, lsl #17
003c3b48: subseq   r1, r0, ip, ror #6

# _ZN6CSMove7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3bf8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c3bfc: ldr      r4, [pc, #0xac]
003c3c00: ldr      r6, [pc, #0xac]
003c3c04: ldr      ip, [pc, #0xac]
003c3c08: add      r4, pc, r4
003c3c0c: ldr      r1, [r4, r6]
003c3c10: ldr      r7, [r4, ip]
003c3c14: sub      sp, sp, #0x20
003c3c18: ldr      r1, [r1]
003c3c1c: mov      sb, r0
003c3c20: mov      r0, r7
003c3c24: mov      r5, r2
003c3c28: mov      sl, r3
003c3c2c: str      r1, [sp, #0x1c]
003c3c30: bl       #0x337888
003c3c34: ldr      r1, [pc, #0x80]
003c3c38: add      r8, sp, #4
003c3c3c: mov      r2, sp
003c3c40: add      r1, pc, r1
003c3c44: mov      r0, r8
003c3c48: bl       #0x3140ec
003c3c4c: mov      r1, r8
003c3c50: mov      r0, r7
003c3c54: bl       #0x337a88
003c3c58: mov      r0, r8
003c3c5c: bl       #0x318254
003c3c60: movw     r3, #0x23c1
003c3c64: str      r3, [r5, #0x520]
003c3c68: mov      r3, #0
003c3c6c: mov      r0, sb
003c3c70: str      r3, [r5, #0x53c]
003c3c74: mov      r2, sl
003c3c78: mov      r1, r5
003c3c7c: bl       #0x3c0f18
003c3c80: ldr      r0, [r5, #0x2dc]
003c3c84: cmp      r0, #0
003c3c88: beq      #0x3c3c90
003c3c8c: bl       #0x46eae0
003c3c90: ldr      r3, [r4, r6]
003c3c94: ldr      r2, [sp, #0x1c]
003c3c98: ldr      r3, [r3]
003c3c9c: cmp      r2, r3
003c3ca0: bne      #0x3c3cac
003c3ca4: add      sp, sp, #0x20
003c3ca8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c3cac: bl       #0x30e310
003c3cb0: subseq   r0, sp, r8, lsl #29
003c3cb4: andeq    r4, r0, ip, lsr #1
003c3cb8: andeq    r0, r0, r4, lsl #17
003c3cbc: subseq   r1, r0, r0, lsl r2

# _ZNK9Character16GetRotationSpeedEv
003a372c: ldr      r3, [r0, #0x520]
003a3730: tst      r3, #0x20
003a3734: beq      #0x3a3744
003a3738: mov      r0, #0xbf000000
003a373c: add      r0, r0, #0x800000
003a3740: bx       lr
003a3744: add      r0, r0, #0x560
003a3748: b        #0x3de708


# _ZNK9Character25IsValidatingFloorPositionEv
003a2e78: ldr      r0, [r0, #0x520]
003a2e7c: eor      r0, r0, #0x40
003a2e80: ubfx     r0, r0, #6, #1
003a2e84: bx       lr

# _ZN12CharAnimator8_SetAnimEij
003cab38: push     {r4, r5, r6, r7, r8, sl, lr}
003cab3c: ldr      r4, [pc, #0x150]
003cab40: ldr      r5, [pc, #0x150]
003cab44: sub      sp, sp, #0x44
003cab48: add      r4, pc, r4
003cab4c: ldr      r3, [r4, r5]
003cab50: cmp      r1, #0
003cab54: mov      r6, r0
003cab58: ldr      r3, [r3]
003cab5c: str      r3, [sp, #0x3c]
003cab60: blt      #0x3cab80
003cab64: ldr      r3, [pc, #0x130]
003cab68: ldr      r3, [r4, r3]
003cab6c: ldr      r3, [r3]
003cab70: cmp      r1, r3
003cab74: bge      #0x3cab80
003cab78: cmp      r2, #2
003cab7c: bls      #0x3cab9c
003cab80: ldr      r3, [r4, r5]
003cab84: ldr      r2, [sp, #0x3c]
003cab88: ldr      r3, [r3]
003cab8c: cmp      r2, r3
003cab90: bne      #0x3cac90
003cab94: add      sp, sp, #0x44
003cab98: pop      {r4, r5, r6, r7, r8, sl, pc}
003cab9c: ldr      r3, [pc, #0xfc]
003caba0: str      r1, [r0, #0x4c]
003caba4: mov      r8, #0x14
003caba8: ldr      r0, [r4, r3]
003cabac: mov      r3, #0xc
003cabb0: mla      r3, r3, r2, r6
003cabb4: ldr      r0, [r0]
003cabb8: str      r2, [r6, #0x2c]
003cabbc: ldr      r2, [pc, #0xe0]
003cabc0: mla      r8, r8, r1, r0
003cabc4: str      r1, [r3, #8]
003cabc8: ldr      sl, [r4, r2]
003cabcc: ldr      r2, [r8, #4]
003cabd0: add      r7, sp, #0x24
003cabd4: mov      r0, sl
003cabd8: str      r2, [r3, #0xc]
003cabdc: bl       #0x337888
003cabe0: ldr      r1, [pc, #0xc0]
003cabe4: add      r2, sp, #8
003cabe8: mov      r0, r7
003cabec: add      r1, pc, r1
003cabf0: bl       #0x3140ec
003cabf4: mov      r1, r7
003cabf8: mov      r0, sl
003cabfc: bl       #0x337a88
003cac00: mov      r0, r7
003cac04: bl       #0x3139ac
003cac08: ldr      r0, [r6, #4]
003cac0c: mov      r1, #0x24
003cac10: mov      r2, #0
003cac14: bl       #0x3a4d5c
003cac18: ldr      r3, [r8, #0x10]
003cac1c: cmp      r3, #2
003cac20: beq      #0x3cac34
003cac24: mov      r0, r6
003cac28: mov      r1, #0
003cac2c: bl       #0x3ca79c
003cac30: b        #0x3cab80
003cac34: mov      r0, sl
003cac38: bl       #0x337888
003cac3c: ldr      r1, [pc, #0x68]
003cac40: add      r7, sp, #0xc
003cac44: add      r2, sp, #4
003cac48: add      r1, pc, r1
003cac4c: mov      r0, r7
003cac50: bl       #0x3140ec
003cac54: mov      r0, sl
003cac58: mov      r1, r7
003cac5c: bl       #0x337a88
003cac60: mov      sl, r0
003cac64: eor      sl, sl, #1
003cac68: mov      r0, r7
003cac6c: bl       #0x3139ac
003cac70: tst      sl, #0xff
003cac74: beq      #0x3cac24
003cac78: ldr      r0, [r8, #8]
003cac7c: bl       #0x3ca708
003cac80: mov      r1, r0
003cac84: mov      r0, r6
003cac88: bl       #0x3ca79c
003cac8c: b        #0x3cab80
003cac90: bl       #0x30e310
003cac94: subseq   sb, ip, r8, asr #30
003cac98: andeq    r4, r0, ip, lsr #1
003cac9c: andeq    r2, r0, r8, asr #20
003caca0: andeq    r3, r0, ip, ror ip
003caca4: andeq    r0, r0, r4, lsl #17
003caca8: subeq    sl, pc, r4, ror #8
003cacac: umaaleq  r8, pc, r8, pc

# _ZNK9Character28IsUpdatingPositionFromVisualEv
003a2e38: ldr      r0, [r0, #0x520]
003a2e3c: and      r0, r0, #1
003a2e40: bx       lr

# _ZNK9Character24IsValidatingCameraLimitsEv
003a2e88: push     {r4, lr}
003a2e8c: ldr      r3, [r0]
003a2e90: mov      lr, pc
003a2e94: ldr      pc, [r3, #0x28]
003a2e98: pop      {r4, pc}

# _ZNK9Character14IsUpdatingPathEv
003a2e2c: ldr      r0, [r0, #0x520]
003a2e30: ubfx     r0, r0, #7, #1
003a2e34: bx       lr


# _ZN6Random9GetRandomEib.clone.1
003ca708: push     {r4, lr}
003ca70c: ldr      r4, [pc, #0x7c]
003ca710: cmp      r0, #0
003ca714: add      r4, pc, r4
003ca718: beq      #0x3ca778
003ca71c: ldr      r2, [pc, #0x70]
003ca720: mov      r1, r0
003ca724: movw     r0, #0xe6ab
003ca728: ldr      r2, [r4, r2]
003ca72c: movw     r3, #0xdb17
003ca730: movt     r3, #0x2b52
003ca734: ldr      lr, [r2]
003ca738: movw     ip, #0xf26b
003ca73c: movt     ip, #0xda
003ca740: mul      r0, r0, lr
003ca744: add      r0, r0, #0x2b000
003ca748: add      r0, r0, #0x3fc
003ca74c: add      r0, r0, #1
003ca750: umull    lr, r3, r3, r0
003ca754: rsb      lr, r3, r0
003ca758: add      r3, r3, lr, lsr #1
003ca75c: lsr      r3, r3, #0x17
003ca760: mls      r3, ip, r3, r0
003ca764: mov      r0, r3
003ca768: str      r3, [r2]
003ca76c: bl       #0x30eb2c
003ca770: eor      r0, r1, r1, asr #31
003ca774: sub      r0, r0, r1, asr #31
003ca778: ldr      r3, [pc, #0x18]
003ca77c: ldr      r3, [r4, r3]
003ca780: ldr      r2, [r3]
003ca784: add      r2, r2, #1
003ca788: str      r2, [r3]
003ca78c: pop      {r4, pc}
003ca790: subseq   sl, ip, ip, ror r3
003ca794: muleq    r0, r4, ip
003ca798: andeq    r1, r0, r8, lsl #1

# _ZN9Character10RaiseEventEiPv
003a4d5c: cmp      r1, #0x36
003a4d60: beq      #0x3a4d6c
003a4d64: add      r0, r0, #0x3c8
003a4d68: b        #0x3cbb34
003a4d6c: add      r0, r0, #0x560
003a4d70: mov      r1, r2
003a4d74: b        #0x3e123c

# _ZN12CharAnimator12_SetAnimStepEj
003ca79c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ca7a0: ldr      r3, [r0, #0x2c]
003ca7a4: mov      r2, #0xc
003ca7a8: ldr      r5, [pc, #0x374]
003ca7ac: mla      r3, r2, r3, r0
003ca7b0: ldr      r2, [pc, #0x370]
003ca7b4: add      r5, pc, r5
003ca7b8: mov      r4, r0
003ca7bc: ldr      r2, [r5, r2]
003ca7c0: ldr      r0, [r3, #8]
003ca7c4: mov      ip, #0x14
003ca7c8: ldr      r2, [r2]
003ca7cc: sub      sp, sp, #0x24
003ca7d0: mla      r2, ip, r0, r2
003ca7d4: ldr      r0, [r2, #8]
003ca7d8: cmp      r0, r1
003ca7dc: bhi      #0x3ca7e8
003ca7e0: add      sp, sp, #0x24
003ca7e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ca7e8: ldr      r2, [r2, #0xc]
003ca7ec: mov      r6, #0x38
003ca7f0: str      r1, [r3, #0x10]
003ca7f4: mla      r6, r6, r1, r2
003ca7f8: ldr      r0, [r4, #4]
003ca7fc: mov      r1, #0x26
003ca800: mov      r2, #0
003ca804: bl       #0x3a4d5c
003ca808: ldr      r3, [r6, #0x28]
003ca80c: cmp      r3, #1
003ca810: beq      #0x3caad8
003ca814: ldr      r3, [r6, #0x10]
003ca818: cmn      r3, #1
003ca81c: beq      #0x3ca9d8
003ca820: ldr      r3, [pc, #0x304]
003ca824: ldr      r0, [r5, r3]
003ca828: bl       #0x31f594
003ca82c: cmp      r0, #0
003ca830: beq      #0x3ca870
003ca834: ldr      r7, [r0, #0x128]
003ca838: cmp      r7, #0
003ca83c: beq      #0x3ca870
003ca840: mov      r0, r7
003ca844: ldr      r1, [r4, #4]
003ca848: bl       #0x40f980
003ca84c: cmp      r0, #0
003ca850: beq      #0x3ca870
003ca854: ldr      r1, [r6, #0x10]
003ca858: cmn      r1, #1
003ca85c: beq      #0x3ca9f4
003ca860: mov      r0, r7
003ca864: mov      r2, #0
003ca868: mov      r3, #1
003ca86c: bl       #0x40f904
003ca870: ldrb     r3, [r6, #0x34]
003ca874: strb     r3, [r4, #0x30]
003ca878: ldrb     r3, [r6, #0x34]
003ca87c: cmp      r3, #0
003ca880: moveq    r7, #1
003ca884: bne      #0x3caa10
003ca888: ldr      r3, [pc, #0x2a0]
003ca88c: ldr      r0, [r4, #4]
003ca890: ldr      sb, [r6, #0x2c]
003ca894: ldr      r3, [r5, r3]
003ca898: ldr      fp, [r3]
003ca89c: bl       #0x3935dc
003ca8a0: ldr      lr, [r0]
003ca8a4: ldr      r8, [r0, #4]
003ca8a8: ldr      sl, [r0, #8]
003ca8ac: mov      ip, #0xbf000000
003ca8b0: add      ip, ip, #0x800000
003ca8b4: str      lr, [sp, #0x14]
003ca8b8: mov      r0, fp
003ca8bc: mov      lr, #1
003ca8c0: mov      r1, sb
003ca8c4: add      r2, sp, #0x14
003ca8c8: mov      r3, #0
003ca8cc: str      r8, [sp, #0x18]
003ca8d0: str      sl, [sp, #0x1c]
003ca8d4: str      lr, [sp]
003ca8d8: str      ip, [sp, #8]
003ca8dc: str      ip, [sp, #4]
003ca8e0: bl       #0x36b5d8
003ca8e4: cmp      r7, #0
003ca8e8: beq      #0x3ca91c
003ca8ec: ldr      r8, [r6, #0x18]
003ca8f0: cmn      r8, #1
003ca8f4: beq      #0x3ca91c
003ca8f8: ldrb     r7, [r6, #4]
003ca8fc: cmp      r7, #0
003ca900: beq      #0x3caa94
003ca904: ldr      r3, [pc, #0x228]
003ca908: mov      r1, r8
003ca90c: ldr      r2, [r4, #4]
003ca910: ldr      r0, [r5, r3]
003ca914: mov      r3, #0
003ca918: bl       #0x495f04
003ca91c: ldr      r2, [r6, #0x30]
003ca920: ldr      r3, [r4, #4]
003ca924: str      r2, [r4, #0x34]
003ca928: ldr      r5, [r3, #0x2d8]
003ca92c: cmp      r5, #0
003ca930: beq      #0x3ca9e8
003ca934: ldr      r3, [r6, #8]
003ca938: cmn      r3, #1
003ca93c: beq      #0x3ca9e8
003ca940: mov      r3, #0
003ca944: strb     r3, [r4, #0x48]
003ca948: mov      r0, r4
003ca94c: bl       #0x3c9b7c
003ca950: ldrb     r3, [r4, #0x54]
003ca954: cmp      r3, #0
003ca958: beq      #0x3caa80
003ca95c: ldrb     r1, [r4, #0x49]
003ca960: ldr      r3, [r5, #0x38]
003ca964: ldrb     r2, [r6, #0x1c]
003ca968: cmp      r1, #0
003ca96c: beq      #0x3caac4
003ca970: mov      r1, #0
003ca974: str      r1, [r3, #0x14]
003ca978: mov      r1, #0
003ca97c: str      r1, [r3, #0xc]
003ca980: strb     r2, [r3, #0x10]
003ca984: ldr      r2, [r5, #0x38]
003ca988: ldr      r1, [r6, #8]
003ca98c: mov      r6, #0
003ca990: ldr      r3, [r4, #0x44]
003ca994: ldr      ip, [r2]
003ca998: mov      r0, r2
003ca99c: str      r6, [sp]
003ca9a0: mov      r2, r6
003ca9a4: mov      lr, pc
003ca9a8: ldr      pc, [ip, #0x1c]
003ca9ac: ldr      r1, [r4, #0x34]
003ca9b0: ldr      r0, [r4, #0x40]
003ca9b4: bl       #0x30ed6c
003ca9b8: ldr      r5, [r5, #0x38]
003ca9bc: mov      r1, r0
003ca9c0: mov      r2, r6
003ca9c4: mov      r0, r5
003ca9c8: ldr      r3, [r5]
003ca9cc: mov      lr, pc
003ca9d0: ldr      pc, [r3, #0x28]
003ca9d4: b        #0x3ca7e0
003ca9d8: ldr      r3, [r6, #0x20]
003ca9dc: cmp      r3, #0
003ca9e0: beq      #0x3ca870
003ca9e4: b        #0x3ca820
003ca9e8: mov      r0, r4
003ca9ec: bl       #0x3c9924
003ca9f0: b        #0x3ca7e0
003ca9f4: ldr      r0, [r6, #0x20]
003ca9f8: cmp      r0, #0
003ca9fc: beq      #0x3ca870
003caa00: ldr      r8, [r6, #0x24]
003caa04: bl       #0x3ca708
003caa08: ldr      r1, [r8, r0, lsl #2]
003caa0c: b        #0x3ca860
003caa10: ldr      r0, [r4, #4]
003caa14: mov      r1, #1
003caa18: add      r0, r0, #0x37c
003caa1c: bl       #0x3ffe3c
003caa20: mov      r7, r0
003caa24: ldr      r0, [r4, #4]
003caa28: mov      r1, #2
003caa2c: add      r0, r0, #0x37c
003caa30: bl       #0x3ffe3c
003caa34: mov      r1, r7
003caa38: mov      sl, r0
003caa3c: mov      r0, r4
003caa40: bl       #0x3c94b8
003caa44: mov      r1, r7
003caa48: eor      r8, r0, #1
003caa4c: ldrb     r2, [r6, #4]
003caa50: mov      r0, r4
003caa54: bl       #0x3c955c
003caa58: uxtb     r8, r8
003caa5c: eor      r0, r0, #1
003caa60: cmp      r8, #0
003caa64: uxtb     r7, r0
003caa68: bne      #0x3caaf0
003caa6c: cmp      r7, #0
003caa70: bne      #0x3cab08
003caa74: cmp      r8, #0
003caa78: beq      #0x3ca8e4
003caa7c: b        #0x3ca888
003caa80: ldr      r2, [r5, #0x38]
003caa84: ldrb     r1, [r6, #0x1c]
003caa88: str      r3, [r2, #0xc]
003caa8c: strb     r1, [r2, #0x10]
003caa90: b        #0x3ca984
003caa94: ldr      r0, [r4, #4]
003caa98: bl       #0x3935dc
003caa9c: ldr      r3, [r4, #4]
003caaa0: mov      r2, r0
003caaa4: ldr      r0, [pc, #0x88]
003caaa8: mov      r1, r8
003caaac: add      r3, r3, #0x16c
003caab0: ldr      r0, [r5, r0]
003caab4: str      r7, [sp, #4]
003caab8: str      r7, [sp]
003caabc: bl       #0x495888
003caac0: b        #0x3ca91c
003caac4: ldrb     r1, [r4, #0x4a]
003caac8: cmp      r1, #0
003caacc: ldreq    r1, [r6, #0xc]
003caad0: beq      #0x3ca974
003caad4: b        #0x3ca970
003caad8: ldr      r2, [r4, #0x2c]
003caadc: mov      r0, r4
003caae0: ldr      r1, [r6, #8]
003caae4: add      r2, r2, #1
003caae8: bl       #0x3cab38
003caaec: b        #0x3ca7e0
003caaf0: mov      r0, r4
003caaf4: mov      r1, sl
003caaf8: bl       #0x3c94b8
003caafc: eor      r0, r0, #1
003cab00: uxtb     r8, r0
003cab04: b        #0x3caa6c
003cab08: mov      r1, sl
003cab0c: mov      r0, r4
003cab10: ldrb     r2, [r6, #4]
003cab14: bl       #0x3c955c
003cab18: eor      r0, r0, #1
003cab1c: uxtb     r8, r0
003cab20: b        #0x3caa74
003cab24: ldrsbeq  sl, [ip], #-0x2c
003cab28: andeq    r3, r0, ip, ror ip
003cab2c: strdeq   r3, r4, [r0], -r4
003cab30: andeq    r0, r0, r4, lsr #27
003cab34: andeq    r1, r0, r8, lsl #22
