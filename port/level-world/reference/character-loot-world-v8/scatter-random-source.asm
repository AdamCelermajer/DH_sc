003ec5d4: push {r4, lr}
003ec5d8: ldr r4, [pc, #0x7c]
003ec5dc: cmp r0, #0
003ec5e0: add r4, pc, r4
003ec5e4: beq #0x3ec644
003ec5e8: ldr r2, [pc, #0x70]
003ec5ec: mov r1, r0
003ec5f0: movw r0, #0xe6ab
003ec5f4: ldr r2, [r4, r2]
003ec5f8: movw r3, #0xdb17
003ec5fc: movt r3, #0x2b52
003ec600: ldr lr, [r2]
003ec604: movw ip, #0xf26b
003ec608: movt ip, #0xda
003ec60c: mul r0, r0, lr
003ec610: add r0, r0, #0x2b000
003ec614: add r0, r0, #0x3fc
003ec618: add r0, r0, #1
003ec61c: umull lr, r3, r3, r0
003ec620: rsb lr, r3, r0
003ec624: add r3, r3, lr, lsr #1
003ec628: lsr r3, r3, #0x17
003ec62c: mls r3, ip, r3, r0
003ec630: mov r0, r3
003ec634: str r3, [r2]
003ec638: bl #0x30eb2c
003ec63c: eor r0, r1, r1, asr #31
003ec640: sub r0, r0, r1, asr #31
003ec644: ldr r3, [pc, #0x18]
003ec648: ldr r3, [r4, r3]
003ec64c: ldr r2, [r3]
003ec650: add r2, r2, #1
003ec654: str r2, [r3]
003ec658: pop {r4, pc}
003ec65c: ldrheq r8, [sl], #-0x40
003ec660: muleq r0, r4, ip
003ec664: andeq r1, r0, r8, lsl #1
