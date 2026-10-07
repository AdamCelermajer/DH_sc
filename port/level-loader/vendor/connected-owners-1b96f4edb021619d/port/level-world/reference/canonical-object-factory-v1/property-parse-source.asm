# 0x30f020
0030f020: push {r4, lr}
0030f024: mov r4, r1
0030f028: bl #0x30e094
0030f02c: subs r0, r0, #0
0030f030: movne r0, #1
0030f034: strb r0, [r4]
0030f038: pop {r4, pc}

# 0x30f03c
0030f03c: push {r4, lr}
0030f040: mov r4, r1
0030f044: bl #0x30e094
0030f048: str r0, [r4]
0030f04c: pop {r4, pc}
0030f050: push {r4, r5, r6, r7, lr}
0030f054: sub sp, sp, #0xc

# 0x30f288
0030f288: push {r4, lr}
0030f28c: mov r4, r1
0030f290: mov r1, #0
0030f294: bl #0x30e6ac
0030f298: bl #0x30e6a0
0030f29c: str r0, [r4]
0030f2a0: pop {r4, pc}
0030f2a4: push {r4, r5, r6, r7, r8, sl, lr}
0030f2a8: sub sp, sp, #0xc
0030f2ac: mov r3, #0x2c
0030f2b0: mov r7, r0
0030f2b4: add r4, sp, #8
0030f2b8: mov r0, #0x100
0030f2bc: strh r3, [r4, #-4]!
0030f2c0: mov r5, r1
0030f2c4: bl #0x310454
0030f2c8: mov r1, r7
0030f2cc: mov r6, r0
0030f2d0: bl #0x30e520
0030f2d4: mov r0, r6
0030f2d8: mov r1, r4
0030f2dc: bl #0x30e028
0030f2e0: cmp r0, #0
0030f2e4: beq #0x30f2f8
0030f2e8: mov r1, #0
0030f2ec: bl #0x30e6ac
0030f2f0: bl #0x30e6a0
0030f2f4: mov sl, r0
0030f2f8: mov r1, r4
0030f2fc: mov r0, #0
0030f300: bl #0x30e028
0030f304: cmp r0, #0
