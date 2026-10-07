_ZN12EventManager13DelayedDetachEiP14IEventReceiver 0x3382a4 208
003382a4: push {r4, r5, r6, lr}
003382a8: ldr r4, [r0, #0xc]
003382ac: sub sp, sp, #8
003382b0: mov r5, r0
003382b4: cmp r4, #0
003382b8: add ip, r0, #8
003382bc: beq #0x338338
003382c0: mov r0, ip
003382c4: b #0x3382cc
003382c8: mov r4, r3
003382cc: ldr r3, [r4, #0x10]
003382d0: cmp r3, r1
003382d4: ldrlt r3, [r4, #0xc]
003382d8: ldrge r3, [r4, #8]
003382dc: movlt r4, r0
003382e0: mov r0, r4
003382e4: cmp r3, #0
003382e8: bne #0x3382c8
003382ec: cmp ip, r4
003382f0: beq #0x33832c
003382f4: ldr r3, [r4, #0x10]
003382f8: cmp r3, r1
003382fc: bgt #0x338338
00338300: cmp ip, r4
00338304: beq #0x33832c
00338308: mov r1, r4
0033830c: ldr r6, [r1, #0x14]!
00338310: b #0x338324
00338314: ldr r3, [r6, #8]
00338318: cmp r3, r2
0033831c: beq #0x338340
00338320: ldr r6, [r6]
00338324: cmp r1, r6
00338328: bne #0x338314
0033832c: mov r0, #0
00338330: add sp, sp, #8
00338334: pop {r4, r5, r6, pc}
00338338: mov r4, ip
0033833c: b #0x338300
00338340: mov r3, #0x10
00338344: add r0, sp, #8
00338348: str r3, [r0, #-4]!
0033834c: bl #0x708ec0
00338350: str r4, [r0, #8]
00338354: str r6, [r0, #0xc]
00338358: ldr r3, [r5, #0x2c]
0033835c: add r2, r5, #0x28
00338360: stm r0, {r2, r3}
00338364: str r0, [r3]
00338368: str r0, [r5, #0x2c]
0033836c: mov r0, #1
00338370: b #0x338330

_ZN12EventManager6UpdateEd 0x33900c 132
0033900c: push {r4, r5, r6, lr}
00339010: mov r6, r0
00339014: ldr r3, [r6, #0x20]
00339018: add r4, r0, #0x20
0033901c: cmp r3, r4
00339020: beq #0x339084
00339024: mov r2, r3
00339028: ldr r2, [r2]
0033902c: cmp r4, r2
00339030: bne #0x339028
00339034: ldr r5, [r3, #8]
00339038: mov r0, r6
0033903c: mov r1, r5
00339040: bl #0x338ebc
00339044: cmp r5, #0
00339048: beq #0x33905c
0033904c: mov r0, r5
00339050: ldr r3, [r5]
00339054: mov lr, pc
00339058: ldr pc, [r3, #4]
0033905c: ldr r0, [r6, #0x20]
00339060: mov r1, #0xc
00339064: ldr r3, [r0]
00339068: ldr r2, [r0, #4]
0033906c: str r3, [r2]
00339070: str r2, [r3, #4]
00339074: bl #0x708f00
00339078: ldr r3, [r6, #0x20]
0033907c: cmp r3, r4
00339080: bne #0x339024
00339084: mov r0, r6
00339088: pop {r4, r5, r6, lr}
0033908c: b #0x3383f4

_ZN12EventManagerD2Ev 0x338590 124
00338590: ldr r3, [pc, #0x6c]
00338594: ldr r2, [pc, #0x6c]
00338598: push {r4, r5, r6, lr}
0033859c: add r3, pc, r3
003385a0: ldr r2, [r3, r2]
003385a4: mov r5, r0
003385a8: mov r4, r0
003385ac: add r2, r2, #8
003385b0: str r2, [r5], #0x20
003385b4: mov r0, r5
003385b8: bl #0x338374
003385bc: add r0, r4, #0x28
003385c0: bl #0x3383b4
003385c4: mov r0, r5
003385c8: bl #0x338374
003385cc: ldr r3, [r4, #0x18]
003385d0: cmp r3, #0
003385d4: beq #0x3385fc
003385d8: add r5, r4, #8
003385dc: mov r0, r5
003385e0: ldr r1, [r4, #0xc]
003385e4: bl #0x338438
003385e8: mov r3, #0
003385ec: str r5, [r4, #0x14]
003385f0: str r3, [r4, #0x18]
003385f4: str r5, [r4, #0x10]
003385f8: str r3, [r4, #0xc]
003385fc: mov r0, r4
00338600: pop {r4, r5, r6, pc}

_ZNK12EventManager10RaiseAsyncERK6IEvent 0x339090 4
00339090: b #0x338ebc

_ZN12EventManager6AttachEiP14IEventReceiveri 0x338da0 284
00338da0: push {r4, r5, r6, r7, lr}
00338da4: ldr r4, [r0, #0xc]
00338da8: sub sp, sp, #0xc
00338dac: str r1, [sp, #4]
00338db0: cmp r4, #0
00338db4: mov r5, r2
00338db8: mov r6, r3
00338dbc: add r0, r0, #8
00338dc0: beq #0x338e6c
00338dc4: mov r2, r0
00338dc8: b #0x338dd0
00338dcc: mov r4, r3
00338dd0: ldr r3, [r4, #0x10]
00338dd4: cmp r3, r1
00338dd8: ldrlt r3, [r4, #0xc]
00338ddc: ldrge r3, [r4, #8]
00338de0: movlt r4, r2
00338de4: mov r2, r4
00338de8: cmp r3, #0
00338dec: bne #0x338dcc
00338df0: cmp r0, r4
00338df4: beq #0x338e7c
00338df8: ldr r3, [r4, #0x10]
00338dfc: cmp r3, r1
00338e00: bgt #0x338e6c
00338e04: cmp r0, r4
00338e08: beq #0x338e7c
00338e0c: mov r2, r4
00338e10: ldr r7, [r2, #0x14]!
00338e14: b #0x338e28
00338e18: ldr r3, [r7, #8]
00338e1c: cmp r5, r3
00338e20: beq #0x338e74
00338e24: ldr r7, [r7]
00338e28: cmp r7, r2
00338e2c: bne #0x338e18
00338e30: mov r0, r7
00338e34: bl #0x33860c
00338e38: mov r2, #0
00338e3c: strb r2, [r0, #0x10]
00338e40: str r6, [r0, #0xc]
00338e44: str r5, [r0, #8]
00338e48: ldr r2, [r4, #0x18]
00338e4c: mov r3, r0
00338e50: str r7, [r0]
00338e54: str r2, [r3, #4]
00338e58: mov r0, #1
00338e5c: str r3, [r2]
00338e60: str r3, [r4, #0x18]
00338e64: add sp, sp, #0xc
00338e68: pop {r4, r5, r6, r7, pc}
00338e6c: mov r4, r0
00338e70: b #0x338e04
00338e74: mov r0, #0
00338e78: b #0x338e64
00338e7c: add r1, sp, #4
00338e80: bl #0x338c98
00338e84: mov r4, r0
00338e88: bl #0x33860c
00338e8c: mov r2, #0
00338e90: strb r2, [r0, #0x10]
00338e94: str r6, [r0, #0xc]
00338e98: str r5, [r0, #8]
00338e9c: ldr r2, [r4, #4]
00338ea0: mov r3, r0
00338ea4: str r4, [r0]
00338ea8: str r2, [r3, #4]
00338eac: mov r0, #1
00338eb0: str r3, [r2]
00338eb4: str r3, [r4, #4]
00338eb8: b #0x338e64

_ZN12EventManager6DetachEiP14IEventReceiver 0x33811c 172
0033811c: push {r4, lr}
00338120: ldr r3, [r0, #0xc]
00338124: add r0, r0, #8
00338128: cmp r3, #0
0033812c: beq #0x3381a0
00338130: mov r4, r0
00338134: b #0x33813c
00338138: mov r3, ip
0033813c: ldr ip, [r3, #0x10]
00338140: cmp ip, r1
00338144: ldrlt ip, [r3, #0xc]
00338148: ldrge ip, [r3, #8]
0033814c: movlt r3, r4
00338150: mov r4, r3
00338154: cmp ip, #0
00338158: bne #0x338138
0033815c: cmp r0, r3
00338160: beq #0x338198
00338164: ldr ip, [r3, #0x10]
00338168: cmp ip, r1
0033816c: bgt #0x3381a0
00338170: cmp r0, r3
00338174: ldrne r0, [r3, #0x14]!
00338178: bne #0x338190
0033817c: b #0x338198
00338180: ldr r1, [r0, #8]
00338184: cmp r1, r2
00338188: beq #0x3381a8
0033818c: ldr r0, [r0]
00338190: cmp r3, r0
00338194: bne #0x338180
00338198: mov r0, #0
0033819c: pop {r4, pc}
003381a0: mov r3, r0
003381a4: b #0x338170
003381a8: ldr r3, [r0]
003381ac: ldr r2, [r0, #4]
003381b0: mov r1, #0x14
003381b4: str r3, [r2]
003381b8: str r2, [r3, #4]
003381bc: bl #0x708f00
003381c0: mov r0, #1
003381c4: pop {r4, pc}

_ZN12EventManager5FlushEv 0x3384ac 76
003384ac: push {r4, r5, r6, lr}
003384b0: mov r4, r0
003384b4: add r0, r0, #0x20
003384b8: bl #0x338374
003384bc: ldr r3, [r4, #0x18]
003384c0: cmp r3, #0
003384c4: beq #0x3384ec
003384c8: add r5, r4, #8
003384cc: mov r0, r5
003384d0: ldr r1, [r4, #0xc]
003384d4: bl #0x338438
003384d8: mov r3, #0
003384dc: str r5, [r4, #0x14]
003384e0: str r3, [r4, #0x18]
003384e4: str r5, [r4, #0x10]
003384e8: str r3, [r4, #0xc]
003384ec: add r0, r4, #0x28
003384f0: pop {r4, r5, r6, lr}
003384f4: b #0x3383b4

_ZNK12EventManager5RaiseERK6IEvent 0x338ebc 336
00338ebc: push {r4, r5, r6, r7, r8, lr}
00338ec0: mov r6, r0
00338ec4: ldr r3, [r1]
00338ec8: mov r0, r1
00338ecc: sub sp, sp, #8
00338ed0: mov r7, r1
00338ed4: mov lr, pc
00338ed8: ldr pc, [r3, #8]
00338edc: ldr r8, [r6, #0xc]
00338ee0: add r1, r6, #8
00338ee4: cmp r8, #0
00338ee8: beq #0x339004
00338eec: mov r2, r1
00338ef0: b #0x338ef8
00338ef4: mov r8, r3
00338ef8: ldr r3, [r8, #0x10]
00338efc: cmp r0, r3
00338f00: ldrgt r3, [r8, #0xc]
00338f04: ldrle r3, [r8, #8]
00338f08: movgt r8, r2
00338f0c: mov r2, r8
00338f10: cmp r3, #0
00338f14: bne #0x338ef4
00338f18: cmp r1, r8
00338f1c: beq #0x338ffc
00338f20: ldr r3, [r8, #0x10]
00338f24: cmp r0, r3
00338f28: blt #0x339004
00338f2c: cmp r1, r8
00338f30: beq #0x338ffc
00338f34: str sp, [sp]
00338f38: str sp, [sp, #4]
00338f3c: ldr r5, [r8, #0x14]!
00338f40: mov r4, sp
00338f44: cmp r5, r8
00338f48: moveq r5, sp
00338f4c: beq #0x338fc4
00338f50: mov r0, r4
00338f54: bl #0x33860c
00338f58: ldr r3, [r5, #8]
00338f5c: str r3, [r0, #8]
00338f60: ldr r3, [r5, #0xc]
00338f64: str r3, [r0, #0xc]
00338f68: ldrb r3, [r5, #0x10]
00338f6c: strb r3, [r0, #0x10]
00338f70: ldr r3, [sp, #4]
00338f74: str r4, [r0]
00338f78: str r3, [r0, #4]
00338f7c: str r0, [r3]
00338f80: str r0, [sp, #4]
00338f84: ldr r5, [r5]
00338f88: cmp r8, r5
00338f8c: bne #0x338f50
00338f90: ldr r5, [sp]
00338f94: mov r1, r7
00338f98: mov r2, r6
00338f9c: cmp r5, r4
00338fa0: beq #0x338fd4
00338fa4: ldr r3, [r5, #8]
00338fa8: mov r0, r3
00338fac: ldr r3, [r3]
00338fb0: mov lr, pc
00338fb4: ldr pc, [r3, #8]
00338fb8: cmp r0, #1
00338fbc: beq #0x338fd4
00338fc0: ldr r5, [r5]
00338fc4: cmp r5, r4
00338fc8: mov r1, r7
00338fcc: mov r2, r6
00338fd0: bne #0x338fa4
00338fd4: ldr r0, [sp]
00338fd8: cmp r0, r4
00338fdc: bne #0x338fe8
00338fe0: b #0x338ffc
00338fe4: mov r0, r5
00338fe8: ldr r5, [r0]
00338fec: mov r1, #0x14
00338ff0: bl #0x708f00
00338ff4: cmp r5, r4
00338ff8: bne #0x338fe4
00338ffc: add sp, sp, #8
00339000: pop {r4, r5, r6, r7, r8, pc}
00339004: mov r8, r1
00339008: b #0x338f2c

_ZN12EventManagerD0Ev 0x338574 28
00338574: push {r4, lr}
00338578: mov r4, r0
0033857c: bl #0x3384f8
00338580: mov r0, r4
00338584: bl #0x310440
00338588: mov r0, r4
0033858c: pop {r4, pc}

_ZN12EventManagerC1Ev 0x3380bc 96
003380bc: ldr r1, [pc, #0x50]
003380c0: push {r4, r5}
003380c4: ldr r4, [pc, #0x4c]
003380c8: add r1, pc, r1
003380cc: mov ip, #0
003380d0: ldr r4, [r1, r4]
003380d4: mov r2, r0
003380d8: add r5, r0, #0x20
003380dc: add r4, r4, #8
003380e0: str r4, [r0]
003380e4: str ip, [r0, #0xc]
003380e8: add r4, r0, #0x28
003380ec: strb ip, [r2, #8]!
003380f0: str r2, [r0, #0x14]
003380f4: str r4, [r0, #0x2c]
003380f8: str ip, [r0, #0x18]
003380fc: str r5, [r0, #0x24]
00338100: str r2, [r0, #0x10]
00338104: str r5, [r0, #0x20]
00338108: str r4, [r0, #0x28]
0033810c: pop {r4, r5}
00338110: bx lr
00338114: rsbeq ip, r5, r8, asr #19
00338118: ldrdeq r4, r5, [r0], -r4

_ZN12EventManagerD1Ev 0x3384f8 124
003384f8: ldr r3, [pc, #0x6c]
003384fc: ldr r2, [pc, #0x6c]
00338500: push {r4, r5, r6, lr}
00338504: add r3, pc, r3
00338508: ldr r2, [r3, r2]
0033850c: mov r5, r0
00338510: mov r4, r0
00338514: add r2, r2, #8
00338518: str r2, [r5], #0x20
0033851c: mov r0, r5
00338520: bl #0x338374
00338524: add r0, r4, #0x28
00338528: bl #0x3383b4
0033852c: mov r0, r5
00338530: bl #0x338374
00338534: ldr r3, [r4, #0x18]
00338538: cmp r3, #0
0033853c: beq #0x338564
00338540: add r5, r4, #8
00338544: mov r0, r5
00338548: ldr r1, [r4, #0xc]
0033854c: bl #0x338438
00338550: mov r3, #0
00338554: str r5, [r4, #0x14]
00338558: str r3, [r4, #0x18]
0033855c: str r5, [r4, #0x10]
00338560: str r3, [r4, #0xc]
00338564: mov r0, r4
00338568: pop {r4, r5, r6, pc}
0033856c: rsbeq ip, r5, ip, lsl #11
00338570: ldrdeq r4, r5, [r0], -r4

_ZN12EventManagerC2Ev 0x33805c 96
0033805c: ldr r1, [pc, #0x50]
00338060: push {r4, r5}
00338064: ldr r4, [pc, #0x4c]
00338068: add r1, pc, r1
0033806c: mov ip, #0
00338070: ldr r4, [r1, r4]
00338074: mov r2, r0
00338078: add r5, r0, #0x20
0033807c: add r4, r4, #8
00338080: str r4, [r0]
00338084: str ip, [r0, #0xc]
00338088: add r4, r0, #0x28
0033808c: strb ip, [r2, #8]!
00338090: str r2, [r0, #0x14]
00338094: str r4, [r0, #0x2c]
00338098: str ip, [r0, #0x18]
0033809c: str r5, [r0, #0x24]
003380a0: str r2, [r0, #0x10]
003380a4: str r5, [r0, #0x20]
003380a8: str r4, [r0, #0x28]
003380ac: pop {r4, r5}
003380b0: bx lr
003380b4: rsbeq ip, r5, r8, lsr #20
003380b8: ldrdeq r4, r5, [r0], -r4

_ZN12EventManager17DropDelayedDetachEv 0x3383f4 68
003383f4: push {r4, r5, r6, lr}
003383f8: mov r5, r0
003383fc: ldr r4, [r5, #0x28]!
00338400: b #0x338424
00338404: ldr r0, [r4, #0xc]
00338408: mov r1, #0x14
0033840c: ldr r3, [r0]
00338410: ldr r2, [r0, #4]
00338414: str r3, [r2]
00338418: str r2, [r3, #4]
0033841c: bl #0x708f00
00338420: ldr r4, [r4]
00338424: cmp r5, r4
00338428: bne #0x338404
0033842c: mov r0, r5
00338430: pop {r4, r5, r6, lr}
00338434: b #0x3383b4