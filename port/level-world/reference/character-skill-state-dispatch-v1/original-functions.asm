_ZN7CSSkill7OnEventEiP9CharacterP16CharStateMachineiPv 0x3c0ac8
003c0ac8: push {r4, lr}
003c0acc: ldr r3, [sp, #8]
003c0ad0: mov r4, r2
003c0ad4: cmp r3, #0x28
003c0ad8: bne #0x3c0afc
003c0adc: ldr r1, [pc, #0x1c]
003c0ae0: ldr r0, [sp, #0xc]
003c0ae4: add r1, pc, r1
003c0ae8: bl #0x30e31c
003c0aec: cmp r0, #0
003c0af0: ldreq r3, [r4, #0x520]
003c0af4: orreq r3, r3, #0x8000
003c0af8: streq r3, [r4, #0x520]
003c0afc: pop {r4, pc}
003c0b00: subseq r4, r0, r4, ror r0

_ZN7CSSkill8OnUpdateEiP9CharacterP16CharStateMachine 0x3c0018
003c0018: bx lr

_ZN7CSSkill6OnInitEiP9CharacterP16CharStateMachine 0x3c8438
003c8438: push {r4, r5, r6, r7, r8, lr}
003c843c: add r5, r2, #0x4f0
003c8440: add r5, r5, #0xc
003c8444: sub sp, sp, #0x50
003c8448: mov r4, #0
003c844c: mov r6, r1
003c8450: mov r0, r5
003c8454: mov r2, #0x22
003c8458: mov r3, #3
003c845c: str r4, [sp, #0x48]
003c8460: str r4, [sp, #0x4c]
003c8464: str r4, [sp]
003c8468: str r4, [sp, #4]
003c846c: ldr r8, [pc, #0x138]
003c8470: bl #0x3c7b18
003c8474: mov r0, r5
003c8478: mov r1, r6
003c847c: movw r2, #0xc358
003c8480: mov r3, #0xc
003c8484: str r4, [sp, #0x40]
003c8488: str r4, [sp, #0x44]
003c848c: str r4, [sp]
003c8490: str r4, [sp, #4]
003c8494: bl #0x3c7b18
003c8498: ldr r3, [pc, #0x110]
003c849c: add r8, pc, r8
003c84a0: mov r0, r5
003c84a4: ldr r7, [r8, r3]
003c84a8: mov r1, r6
003c84ac: movw r2, #0xc351
003c84b0: mov r3, #4
003c84b4: str r7, [sp, #0x38]
003c84b8: str r7, [sp]
003c84bc: str r4, [sp, #0x3c]
003c84c0: str r4, [sp, #4]
003c84c4: bl #0x3c7b18
003c84c8: mov r0, r5
003c84cc: mov r1, r6
003c84d0: movw r2, #0xc354
003c84d4: mov r3, #5
003c84d8: str r7, [sp, #0x30]
003c84dc: str r7, [sp]
003c84e0: str r4, [sp, #0x34]
003c84e4: str r4, [sp, #4]
003c84e8: bl #0x3c7b18
003c84ec: mov r0, r5
003c84f0: mov r1, r6
003c84f4: movw r2, #0xc355
003c84f8: mov r3, #6
003c84fc: str r7, [sp]
003c8500: str r7, [sp, #0x28]
003c8504: str r4, [sp, #0x2c]
003c8508: str r4, [sp, #4]
003c850c: bl #0x3c7b18
003c8510: ldr r3, [pc, #0x9c]
003c8514: mov r0, r5
003c8518: mov r1, r6
003c851c: ldr r7, [r8, r3]
003c8520: movw r2, #0xc35a
003c8524: mov r3, #0xb
003c8528: str r7, [sp, #0x20]
003c852c: str r4, [sp, #0x24]
003c8530: str r7, [sp]
003c8534: str r4, [sp, #4]
003c8538: bl #0x3c7b18
003c853c: mov r0, r5
003c8540: mov r1, r6
003c8544: movw r2, #0xc35c
003c8548: mov r3, #9
003c854c: str r7, [sp, #0x18]
003c8550: str r4, [sp, #0x1c]
003c8554: str r7, [sp]
003c8558: str r4, [sp, #4]
003c855c: bl #0x3c7b18
003c8560: mov r0, r5
003c8564: mov r1, r6
003c8568: movw r2, #0xc35d
003c856c: mov r3, #8
003c8570: str r7, [sp, #0x10]
003c8574: str r4, [sp, #0x14]
003c8578: str r7, [sp]
003c857c: str r4, [sp, #4]
003c8580: bl #0x3c7b18
003c8584: mov r0, r5
003c8588: mov r1, r6
003c858c: movw r2, #0xc35b
003c8590: mov r3, #0xa
003c8594: str r7, [sp]
003c8598: stmib sp, {r4, r7}
003c859c: str r4, [sp, #0xc]
003c85a0: bl #0x3c7b18
003c85a4: add sp, sp, #0x50
003c85a8: pop {r4, r5, r6, r7, r8, pc}
003c85ac: ldrsheq ip, [ip], #-0x54
003c85b0: strheq r4, [r0], -ip
003c85b4: andeq r3, r0, ip, asr #9

_ZN9Character13CSM_StopSkillEiPviRi 0x3ad290
003ad290: ldr r0, [r0, #0x520]
003ad294: ubfx r0, r0, #0xf, #1
003ad298: bx lr

_ZN9Character15CSM_InterruptedEiPviRi 0x3ad244
003ad244: cmp r3, #6
003ad248: beq #0x3ad264
003ad24c: cmp r3, #0xa
003ad250: beq #0x3ad270
003ad254: cmp r3, #5
003ad258: movne r0, #1
003ad25c: ldrbeq r0, [r0, #0x441]
003ad260: bx lr
003ad264: ldr r0, [r0, #0x520]
003ad268: ubfx r0, r0, #0x10, #1
003ad26c: bx lr
003ad270: ldr r0, [r0, #0x528]
003ad274: eor r0, r0, #0x20
003ad278: ubfx r0, r0, #5, #1
003ad27c: bx lr

_ZN6CSIdle6OnInitEiP9CharacterP16CharStateMachine 0x3c7e60
003c7e60: push {r4, r5, r6, r7, r8, sl, lr}
003c7e64: add r5, r2, #0x4f0
003c7e68: add r5, r5, #0xc
003c7e6c: sub sp, sp, #0x7c
003c7e70: mov r4, #0
003c7e74: mov r6, r1
003c7e78: mov sl, r2
003c7e7c: mov r0, r5
003c7e80: mov r2, #0x23
003c7e84: mov r3, #3
003c7e88: str r4, [sp, #0x70]
003c7e8c: str r4, [sp, #0x74]
003c7e90: str r4, [sp]
003c7e94: str r4, [sp, #4]
003c7e98: bl #0x3c7b18
003c7e9c: mov r0, r5
003c7ea0: mov r1, r6
003c7ea4: mov r2, #0x22
003c7ea8: mov r3, #3
003c7eac: str r4, [sp, #0x68]
003c7eb0: str r4, [sp, #0x6c]
003c7eb4: str r4, [sp]
003c7eb8: str r4, [sp, #4]
003c7ebc: ldr r8, [pc, #0x1d8]
003c7ec0: bl #0x3c7b18
003c7ec4: mov r0, r5
003c7ec8: mov r1, r6
003c7ecc: movw r2, #0xc358
003c7ed0: mov r3, #0xc
003c7ed4: str r4, [sp, #0x60]
003c7ed8: str r4, [sp, #0x64]
003c7edc: str r4, [sp]
003c7ee0: str r4, [sp, #4]
003c7ee4: bl #0x3c7b18
003c7ee8: ldr r3, [pc, #0x1b0]
003c7eec: add r8, pc, r8
003c7ef0: mov r0, r5
003c7ef4: ldr ip, [r8, r3]
003c7ef8: mov r1, r6
003c7efc: movw r2, #0xc35a
003c7f00: mov r3, #0xb
003c7f04: str ip, [sp]
003c7f08: str ip, [sp, #0x58]
003c7f0c: str r4, [sp, #0x5c]
003c7f10: str r4, [sp, #4]
003c7f14: bl #0x3c7b18
003c7f18: ldr r3, [pc, #0x184]
003c7f1c: mov r0, r5
003c7f20: mov r1, r6
003c7f24: ldr r7, [r8, r3]
003c7f28: movw r2, #0xc35b
003c7f2c: mov r3, #0xa
003c7f30: str r7, [sp, #0x50]
003c7f34: str r4, [sp, #0x54]
003c7f38: str r7, [sp]
003c7f3c: str r4, [sp, #4]
003c7f40: bl #0x3c7b18
003c7f44: mov r0, r5
003c7f48: mov r1, r6
003c7f4c: movw r2, #0xc35c
003c7f50: mov r3, #9
003c7f54: str r7, [sp, #0x48]
003c7f58: str r4, [sp, #0x4c]
003c7f5c: str r7, [sp]
003c7f60: str r4, [sp, #4]
003c7f64: bl #0x3c7b18
003c7f68: mov r0, r5
003c7f6c: mov r1, r6
003c7f70: movw r2, #0xc35d
003c7f74: mov r3, #8
003c7f78: str r7, [sp]
003c7f7c: str r7, [sp, #0x40]
003c7f80: str r4, [sp, #0x44]
003c7f84: str r4, [sp, #4]
003c7f88: bl #0x3c7b18
003c7f8c: mov r0, r5
003c7f90: mov r1, r6
003c7f94: movw r2, #0xc356
003c7f98: mov r3, #7
003c7f9c: str r4, [sp, #0x38]
003c7fa0: str r4, [sp, #0x3c]
003c7fa4: str r4, [sp]
003c7fa8: str r4, [sp, #4]
003c7fac: bl #0x3c7b18
003c7fb0: mov r0, r5
003c7fb4: mov r1, r6
003c7fb8: movw r2, #0xc355
003c7fbc: mov r3, #6
003c7fc0: str r4, [sp, #0x30]
003c7fc4: str r4, [sp, #0x34]
003c7fc8: str r4, [sp]
003c7fcc: str r4, [sp, #4]
003c7fd0: bl #0x3c7b18
003c7fd4: ldr r3, [pc, #0xcc]
003c7fd8: mov r0, r5
003c7fdc: mov r1, r6
003c7fe0: ldr ip, [r8, r3]
003c7fe4: movw r2, #0xc354
003c7fe8: mov r3, #5
003c7fec: str ip, [sp]
003c7ff0: str ip, [sp, #0x28]
003c7ff4: str r4, [sp, #0x2c]
003c7ff8: str r4, [sp, #4]
003c7ffc: bl #0x3c7b18
003c8000: mov r0, r5
003c8004: mov r1, r6
003c8008: movw r2, #0xc351
003c800c: mov r3, #4
003c8010: str r4, [sp, #0x20]
003c8014: str r4, [sp, #0x24]
003c8018: str r4, [sp]
003c801c: str r4, [sp, #4]
003c8020: bl #0x3c7b18
003c8024: mov r0, r5
003c8028: mov r1, r6
003c802c: movw r2, #0xc352
003c8030: mov r3, #3
003c8034: str r4, [sp, #0x18]
003c8038: str r4, [sp, #0x1c]
003c803c: str r4, [sp]
003c8040: str r4, [sp, #4]
003c8044: bl #0x3c7b18
003c8048: mov r0, r5
003c804c: mov r1, r6
003c8050: movw r2, #0xc353
003c8054: mov r3, #0xd
003c8058: str r4, [sp, #0x10]
003c805c: str r4, [sp, #0x14]
003c8060: str r4, [sp]
003c8064: str r4, [sp, #4]
003c8068: bl #0x3c7b18
003c806c: mov r0, r5
003c8070: mov r1, r6
003c8074: movw r2, #0xc357
003c8078: mov r3, #0xf
003c807c: str r4, [sp, #8]
003c8080: str r4, [sp, #0xc]
003c8084: str r4, [sp]
003c8088: str r4, [sp, #4]
003c808c: bl #0x3c7b18
003c8090: strb r4, [sl, #0x538]
003c8094: add sp, sp, #0x7c
003c8098: pop {r4, r5, r6, r7, r8, sl, pc}
003c809c: subseq ip, ip, r4, lsr #23
003c80a0: andeq r2, r0, r4, lsl #29
003c80a4: andeq r3, r0, ip, asr #9
003c80a8: ldrdeq r0, r1, [r0], -r4

_ZN6CSMove6OnInitEiP9CharacterP16CharStateMachine 0x3c80ac
003c80ac: push {r4, r5, r6, r7, r8, lr}
003c80b0: add r5, r2, #0x4f0
003c80b4: add r5, r5, #0xc
003c80b8: sub sp, sp, #0x60
003c80bc: mov r4, #0
003c80c0: mov r6, r1
003c80c4: mov r0, r5
003c80c8: mov r2, #0x3f
003c80cc: mov r3, #3
003c80d0: str r4, [sp, #0x58]
003c80d4: str r4, [sp, #0x5c]
003c80d8: str r4, [sp]
003c80dc: str r4, [sp, #4]
003c80e0: ldr r8, [pc, #0x18c]
003c80e4: bl #0x3c7b18
003c80e8: mov r0, r5
003c80ec: mov r1, r6
003c80f0: movw r2, #0xc358
003c80f4: mov r3, #0xc
003c80f8: str r4, [sp, #0x50]
003c80fc: str r4, [sp, #0x54]
003c8100: str r4, [sp]
003c8104: str r4, [sp, #4]
003c8108: bl #0x3c7b18
003c810c: ldr r3, [pc, #0x164]
003c8110: add r8, pc, r8
003c8114: mov r0, r5
003c8118: ldr ip, [r8, r3]
003c811c: mov r1, r6
003c8120: movw r2, #0xc35a
003c8124: mov r3, #0xb
003c8128: str ip, [sp]
003c812c: str ip, [sp, #0x48]
003c8130: str r4, [sp, #0x4c]
003c8134: str r4, [sp, #4]
003c8138: bl #0x3c7b18
003c813c: ldr r3, [pc, #0x138]
003c8140: mov r0, r5
003c8144: mov r1, r6
003c8148: ldr r7, [r8, r3]
003c814c: movw r2, #0xc35b
003c8150: mov r3, #0xa
003c8154: str r7, [sp, #0x40]
003c8158: str r4, [sp, #0x44]
003c815c: str r7, [sp]
003c8160: str r4, [sp, #4]
003c8164: bl #0x3c7b18
003c8168: mov r0, r5
003c816c: mov r1, r6
003c8170: movw r2, #0xc35c
003c8174: mov r3, #9
003c8178: str r7, [sp, #0x38]
003c817c: str r4, [sp, #0x3c]
003c8180: str r7, [sp]
003c8184: str r4, [sp, #4]
003c8188: bl #0x3c7b18
003c818c: mov r0, r5
003c8190: mov r1, r6
003c8194: movw r2, #0xc35d
003c8198: mov r3, #8
003c819c: str r7, [sp]
003c81a0: str r7, [sp, #0x30]
003c81a4: str r4, [sp, #0x34]
003c81a8: str r4, [sp, #4]
003c81ac: bl #0x3c7b18
003c81b0: mov r0, r5
003c81b4: mov r1, r6
003c81b8: movw r2, #0xc356
003c81bc: mov r3, #7
003c81c0: str r4, [sp, #0x28]
003c81c4: str r4, [sp, #0x2c]
003c81c8: str r4, [sp]
003c81cc: str r4, [sp, #4]
003c81d0: bl #0x3c7b18
003c81d4: mov r0, r5
003c81d8: mov r1, r6
003c81dc: movw r2, #0xc355
003c81e0: mov r3, #6
003c81e4: str r4, [sp, #0x20]
003c81e8: str r4, [sp, #0x24]
003c81ec: str r4, [sp]
003c81f0: str r4, [sp, #4]
003c81f4: bl #0x3c7b18
003c81f8: ldr r3, [pc, #0x80]
003c81fc: mov r0, r5
003c8200: mov r1, r6
003c8204: ldr ip, [r8, r3]
003c8208: movw r2, #0xc354
003c820c: mov r3, #5
003c8210: str ip, [sp]
003c8214: str ip, [sp, #0x18]
003c8218: str r4, [sp, #0x1c]
003c821c: str r4, [sp, #4]
003c8220: bl #0x3c7b18
003c8224: mov r0, r5
003c8228: mov r1, r6
003c822c: movw r2, #0xc353
003c8230: mov r3, #0xd
003c8234: str r4, [sp, #0x10]
003c8238: str r4, [sp, #0x14]
003c823c: str r4, [sp]
003c8240: str r4, [sp, #4]
003c8244: bl #0x3c7b18
003c8248: mov r0, r5
003c824c: mov r1, r6
003c8250: movw r2, #0xc357
003c8254: mov r3, #0xf
003c8258: str r4, [sp, #4]
003c825c: str r4, [sp, #8]
003c8260: str r4, [sp, #0xc]
003c8264: str r4, [sp]
003c8268: bl #0x3c7b18
003c826c: add sp, sp, #0x60
003c8270: pop {r4, r5, r6, r7, r8, pc}
003c8274: subseq ip, ip, r0, lsl #19
003c8278: andeq r2, r0, r4, lsl #29
003c827c: andeq r3, r0, ip, asr #9
003c8280: ldrdeq r0, r1, [r0], -r4

_ZN8CSAttack6OnInitEiP9CharacterP16CharStateMachine 0x3c8284
003c8284: push {r4, r5, r6, r7, r8, lr}
003c8288: add r5, r2, #0x4f0
003c828c: add r5, r5, #0xc
003c8290: sub sp, sp, #0x58
003c8294: mov r4, #0
003c8298: mov r6, r1
003c829c: mov r0, r5
003c82a0: mov r2, #0x22
003c82a4: mov r3, #3
003c82a8: str r4, [sp, #0x50]
003c82ac: str r4, [sp, #0x54]
003c82b0: str r4, [sp]
003c82b4: str r4, [sp, #4]
003c82b8: bl #0x3c7b18
003c82bc: mov r0, r5
003c82c0: mov r1, r6
003c82c4: movw r2, #0xc358
003c82c8: mov r3, #0xc
003c82cc: str r4, [sp, #0x48]
003c82d0: str r4, [sp, #0x4c]
003c82d4: str r4, [sp]
003c82d8: str r4, [sp, #4]
003c82dc: bl #0x3c7b18
003c82e0: mov r0, r5
003c82e4: mov r1, r6
003c82e8: movw r2, #0xc355
003c82ec: mov r3, #6
003c82f0: str r4, [sp, #0x40]
003c82f4: str r4, [sp, #0x44]
003c82f8: str r4, [sp]
003c82fc: str r4, [sp, #4]
003c8300: ldr r8, [pc, #0x120]
003c8304: bl #0x3c7b18
003c8308: mov r0, r5
003c830c: mov r1, r6
003c8310: movw r2, #0xc356
003c8314: mov r3, #7
003c8318: str r4, [sp, #0x38]
003c831c: str r4, [sp, #0x3c]
003c8320: str r4, [sp]
003c8324: str r4, [sp, #4]
003c8328: bl #0x3c7b18
003c832c: ldr r3, [pc, #0xf8]
003c8330: add r8, pc, r8
003c8334: mov r0, r5
003c8338: ldr ip, [r8, r3]
003c833c: mov r1, r6
003c8340: movw r2, #0xc35a
003c8344: mov r3, #0xb
003c8348: str ip, [sp]
003c834c: str ip, [sp, #0x30]
003c8350: str r4, [sp, #0x34]
003c8354: str r4, [sp, #4]
003c8358: bl #0x3c7b18
003c835c: ldr r3, [pc, #0xcc]
003c8360: mov r0, r5
003c8364: mov r1, r6
003c8368: ldr r7, [r8, r3]
003c836c: movw r2, #0xc35b
003c8370: mov r3, #0xa
003c8374: str r7, [sp, #0x28]
003c8378: str r4, [sp, #0x2c]
003c837c: str r7, [sp]
003c8380: str r4, [sp, #4]
003c8384: bl #0x3c7b18
003c8388: mov r0, r5
003c838c: mov r1, r6
003c8390: movw r2, #0xc35c
003c8394: mov r3, #9
003c8398: str r7, [sp, #0x20]
003c839c: str r4, [sp, #0x24]
003c83a0: str r7, [sp]
003c83a4: str r4, [sp, #4]
003c83a8: bl #0x3c7b18
003c83ac: mov r0, r5
003c83b0: mov r1, r6
003c83b4: movw r2, #0xc35d
003c83b8: mov r3, #8
003c83bc: str r7, [sp]
003c83c0: str r7, [sp, #0x18]
003c83c4: str r4, [sp, #0x1c]
003c83c8: str r4, [sp, #4]
003c83cc: bl #0x3c7b18
003c83d0: ldr r3, [pc, #0x5c]
003c83d4: mov r0, r5
003c83d8: mov r1, r6
003c83dc: ldr ip, [r8, r3]
003c83e0: movw r2, #0xc351
003c83e4: mov r3, #4
003c83e8: str ip, [sp]
003c83ec: str ip, [sp, #0x10]
003c83f0: str r4, [sp, #0x14]
003c83f4: str r4, [sp, #4]
003c83f8: bl #0x3c7b18
003c83fc: mov r0, r5
003c8400: mov r1, r6
003c8404: movw r2, #0xc357
003c8408: mov r3, #0xf
003c840c: str r4, [sp, #4]
003c8410: str r4, [sp, #8]
003c8414: str r4, [sp, #0xc]
003c8418: str r4, [sp]
003c841c: bl #0x3c7b18
003c8420: add sp, sp, #0x58
003c8424: pop {r4, r5, r6, r7, r8, pc}
003c8428: subseq ip, ip, r0, ror #14
003c842c: andeq r2, r0, r4, lsl #29
003c8430: andeq r3, r0, ip, asr #9
003c8434: andeq r3, r0, r4, lsl #7

_ZN7CSSkill6OnBlurEiP9CharacterP16CharStateMachinei 0x3c434c
003c434c: push {r4, r5, r6, r7, r8, lr}
003c4350: ldr r5, [pc, #0x118]
003c4354: ldr r6, [pc, #0x118]
003c4358: ldr r1, [pc, #0x118]
003c435c: add r5, pc, r5
003c4360: ldr r3, [r5, r6]
003c4364: ldr r8, [r5, r1]
003c4368: sub sp, sp, #0x28
003c436c: ldr r3, [r3]
003c4370: mov r0, r8
003c4374: mov r4, r2
003c4378: str r3, [sp, #0x24]
003c437c: bl #0x337888
003c4380: ldr r1, [pc, #0xf4]
003c4384: add r7, sp, #0xc
003c4388: add r2, sp, #8
003c438c: add r1, pc, r1
003c4390: mov r0, r7
003c4394: bl #0x3140ec
003c4398: mov r1, r7
003c439c: mov r0, r8
003c43a0: bl #0x337a88
003c43a4: mov r0, r7
003c43a8: bl #0x318254
003c43ac: add r0, r4, #0x3c8
003c43b0: bl #0x3d49c4
003c43b4: mov r0, r4
003c43b8: bl #0x3938f8
003c43bc: mov r0, r4
003c43c0: mov r1, #0x1f
003c43c4: mov r2, #0
003c43c8: bl #0x3a4d5c
003c43cc: ldr r3, [r4, #0x528]
003c43d0: tst r3, #0x100
003c43d4: bne #0x3c4414
003c43d8: ldr r0, [r4, #0x2dc]
003c43dc: cmp r0, #0
003c43e0: beq #0x3c43e8
003c43e4: bl #0x46eb20
003c43e8: mov r0, r4
003c43ec: bl #0x3a3064
003c43f0: cmp r0, #0
003c43f4: bne #0x3c4440
003c43f8: ldr r3, [r5, r6]
003c43fc: ldr r2, [sp, #0x24]
003c4400: ldr r3, [r3]
003c4404: cmp r2, r3
003c4408: bne #0x3c446c
003c440c: add sp, sp, #0x28
003c4410: pop {r4, r5, r6, r7, r8, pc}
003c4414: mov ip, #0
003c4418: mov r2, ip
003c441c: mov r1, #0xa
003c4420: mov r3, #0x30
003c4424: add r0, r4, #0x3b4
003c4428: str ip, [sp]
003c442c: bl #0x3dbe24
003c4430: mov r0, r4
003c4434: bl #0x3a3064
003c4438: cmp r0, #0
003c443c: beq #0x3c43f8
003c4440: mov r0, r4
003c4444: bl #0x3a3144
003c4448: cmp r0, #0
003c444c: bne #0x3c43f8
003c4450: mov r0, r4
003c4454: bl #0x3a3158
003c4458: cmp r0, #0
003c445c: ldreq r3, [r4, #0x520]
003c4460: biceq r3, r3, #0x10000
003c4464: streq r3, [r4, #0x520]
003c4468: b #0x3c43f8
003c446c: bl #0x30e310
003c4470: subseq r0, sp, r4, lsr r7
003c4474: andeq r4, r0, ip, lsr #1
003c4478: andeq r0, r0, r4, lsl #17
003c447c: subseq r0, r0, r4, asr #21
