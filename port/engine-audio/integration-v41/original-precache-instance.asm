003699fc push {r4, r5, r6, r7, r8, sl, lr}
00369a00 ldr r4, [pc, #0x11c]
00369a04 ldr r3, [pc, #0x11c]
00369a08 ldr r5, [pc, #0x11c]
00369a0c add r4, pc, r4
00369a10 ldr r2, [r4, r3]
00369a14 ldr r3, [r4, r5]
00369a18 sub sp, sp, #0x22c
00369a1c ldrb r2, [r2]
00369a20 ldr r3, [r3]
00369a24 mov r7, r0
00369a28 cmp r2, #0
00369a2c mov r6, r1
00369a30 str r3, [sp, #0x224]
00369a34 bne #0x369a4c
00369a38 cmp r1, #0
00369a3c blt #0x369a4c
00369a40 ldr r3, [r0, #0x1c]
00369a44 cmp r1, r3
00369a48 ble #0x369a68
00369a4c ldr r3, [r4, r5]
00369a50 ldr r2, [sp, #0x224]
00369a54 ldr r3, [r3]
00369a58 cmp r2, r3
00369a5c bne #0x369b20
00369a60 add sp, sp, #0x22c
00369a64 pop {r4, r5, r6, r7, r8, sl, pc}
00369a68 add ip, sp, #0x20
00369a6c str ip, [sp]
00369a70 add ip, sp, #0x1c
00369a74 add r3, sp, #0x18
00369a78 str ip, [sp, #4]
00369a7c add r0, r0, #0x64
00369a80 add ip, sp, #0x14
00369a84 add r2, sp, #0x10
00369a88 str ip, [sp, #8]
00369a8c bl #0x8896f4
00369a90 ldr r3, [r7, #0x1c]
00369a94 cmp r6, r3
00369a98 bgt #0x369a4c
00369a9c ldr r3, [r7, #8]
00369aa0 ldr r3, [r3, r6, lsl #2]
00369aa4 cmp r3, #0
00369aa8 bne #0x369a4c
00369aac ldr r3, [pc, #0x7c]
00369ab0 add sl, sp, #0x24
00369ab4 mov r0, sl
00369ab8 ldr r3, [r4, r3]
00369abc ldr r1, [r3]
00369ac0 bl #0x30e520
00369ac4 mov r0, sl
00369ac8 bl #0x30de54
00369acc ldr r1, [pc, #0x60]
00369ad0 mov r2, #0xd
00369ad4 add r0, sl, r0
00369ad8 add r1, pc, r1
00369adc bl #0x30e868
00369ae0 ldr r1, [sp, #0x10]
00369ae4 mov r0, sl
00369ae8 bl #0x30ed90
00369aec mov r1, #4
00369af0 mov r0, #0x28
00369af4 bl #0x310570
00369af8 ldr ip, [sp, #0x20]
00369afc ldr r3, [sp, #0x14]
00369b00 mov r1, sl
00369b04 ldr r2, [sp, #0x18]
00369b08 mov r8, r0
00369b0c str ip, [sp]
00369b10 bl #0x86f5fc
00369b14 ldr r3, [r7, #8]
00369b18 str r8, [r3, r6, lsl #2]
00369b1c b #0x369a4c
00369b20 bl #0x30e310
0036ca88 ldr r3, [pc, #0x38]
0036ca8c ldr r2, [pc, #0x38]
0036ca90 push {r4, r5, r6, lr}
0036ca94 add r3, pc, r3
0036ca98 ldr r4, [r3, r2]
0036ca9c ldr r3, [r4]
0036caa0 cmp r3, #0
0036caa4 beq #0x36caac
0036caa8 pop {r4, r5, r6, pc}
0036caac mov r1, #4
0036cab0 mov r0, #0xc4
0036cab4 bl #0x310570
0036cab8 mov r5, r0
0036cabc bl #0x36c7b0
0036cac0 str r5, [r4]
0036cac4 pop {r4, r5, r6, pc}
0036b048 ldr r3, [pc, #0x30]
0036b04c ldr r2, [pc, #0x30]
0036b050 push {r4, lr}
0036b054 add r3, pc, r3
0036b058 ldr r2, [r3, r2]
0036b05c ldr r4, [r2]
0036b060 cmp r4, #0
0036b064 beq #0x36b07c
0036b068 mov r0, r4
0036b06c bl #0x36afb0
0036b070 mov r0, r4
0036b074 pop {r4, lr}
0036b078 b #0x310440
0036b07c pop {r4, pc}
