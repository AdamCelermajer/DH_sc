local matrix leaf 0x631c14
00631c14 mov r1, #0
00631c18 push {r4, lr}
00631c1c mov r2, #0x40
00631c20 strb r1, [r0, #0x40]
00631c24 mov r4, r0
00631c28 bl #0x30e460
00631c2c mov r3, #0x3f800000
00631c30 mov r2, #1
00631c34 strb r2, [r4, #0x40]
00631c38 str r3, [r4, #0x3c]
00631c3c str r3, [r4]
00631c40 str r3, [r4, #0x14]
00631c44 str r3, [r4, #0x28]
00631c48 mov r0, r4
00631c4c pop {r4, pc}
_ZNK6glitch4core8CMatrix4IfE10isIdentityEv 0x5ba19c
005ba19c push {r4, r5, r6, r7, r8, lr}
005ba1a0 ldrb r5, [r0, #0x40]
005ba1a4 mov r7, r0
005ba1a8 cmp r5, #0
005ba1ac bne #0x5ba31c
005ba1b0 ldr r4, [r0]
005ba1b4 movw r1, #0x37bd
005ba1b8 movt r1, #0x3586
005ba1bc mov r0, r4
005ba1c0 bl #0x30eba4
005ba1c4 mov r1, #0x3f800000
005ba1c8 bl #0x30e4b4
005ba1cc cmp r0, #0
005ba1d0 beq #0x5ba314
005ba1d4 movw r1, #0x37bd
005ba1d8 movt r1, #0x3586
005ba1dc mov r0, r4
005ba1e0 bl #0x30e3ac
005ba1e4 mov r1, #0x3f800000
005ba1e8 bl #0x30e9ac
005ba1ec cmp r0, #0
005ba1f0 beq #0x5ba314
005ba1f4 ldr r4, [r7, #0x14]
005ba1f8 movw r1, #0x37bd
005ba1fc movt r1, #0x3586
005ba200 mov r0, r4
005ba204 bl #0x30eba4
005ba208 mov r1, #0x3f800000
005ba20c bl #0x30e4b4
005ba210 cmp r0, #0
005ba214 beq #0x5ba314
005ba218 movw r1, #0x37bd
005ba21c movt r1, #0x3586
005ba220 mov r0, r4
005ba224 bl #0x30e3ac
005ba228 mov r1, #0x3f800000
005ba22c bl #0x30e9ac
005ba230 cmp r0, #0
005ba234 beq #0x5ba314
005ba238 ldr r4, [r7, #0x28]
005ba23c movw r1, #0x37bd
005ba240 movt r1, #0x3586
005ba244 mov r0, r4
005ba248 bl #0x30eba4
005ba24c mov r1, #0x3f800000
005ba250 bl #0x30e4b4
005ba254 cmp r0, #0
005ba258 beq #0x5ba314
005ba25c movw r1, #0x37bd
005ba260 movt r1, #0x3586
005ba264 mov r0, r4
005ba268 bl #0x30e3ac
005ba26c mov r1, #0x3f800000
005ba270 bl #0x30e9ac
005ba274 cmp r0, #0
005ba278 beq #0x5ba314
005ba27c ldr r4, [r7, #0x3c]
005ba280 movw r1, #0x37bd
005ba284 movt r1, #0x3586
005ba288 mov r0, r4
005ba28c bl #0x30eba4
005ba290 mov r1, #0x3f800000
005ba294 bl #0x30e4b4
005ba298 cmp r0, #0
005ba29c beq #0x5ba314
005ba2a0 movw r1, #0x37bd
005ba2a4 movt r1, #0x3586
005ba2a8 mov r0, r4
005ba2ac bl #0x30e3ac
005ba2b0 mov r1, #0x3f800000
005ba2b4 bl #0x30e9ac
005ba2b8 cmp r0, #0
005ba2bc movne r6, r7
005ba2c0 beq #0x5ba314
005ba2c4 mov r4, #0
005ba2c8 movw r1, #0x37bd
005ba2cc cmp r4, r5
005ba2d0 movt r1, #0x3586
005ba2d4 beq #0x5ba2ec
005ba2d8 ldr r0, [r6, r4, lsl #2]
005ba2dc bic r0, r0, #0x80000000
005ba2e0 bl #0x30e9ac
005ba2e4 cmp r0, #0
005ba2e8 beq #0x5ba314
005ba2ec add r4, r4, #1
005ba2f0 cmp r4, #4
005ba2f4 bne #0x5ba2c8
005ba2f8 add r5, r5, #1
005ba2fc cmp r5, #4
005ba300 add r6, r6, #0x10
005ba304 bne #0x5ba2c4
005ba308 mov r0, #1
005ba30c strb r0, [r7, #0x40]
005ba310 pop {r4, r5, r6, r7, r8, pc}
005ba314 mov r0, #0
005ba318 pop {r4, r5, r6, r7, r8, pc}
005ba31c mov r0, #1
005ba320 pop {r4, r5, r6, r7, r8, pc}
local matrix leaf 0x5baa94
005baa94 mov r3, #0
005baa98 push {r4, lr}
005baa9c mov r2, #0x41
005baaa0 mov r4, r0
005baaa4 strb r3, [r0, #0x40]
005baaa8 bl #0x30e868
005baaac mov r0, r4
005baab0 pop {r4, pc}
_ZN6glitch5video6detail18setMatrixParameterEPPNS_4core8CMatrix4IfEERKS4_NS2_10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEE 0x5badb8
005badb8 push {r4, r5, r6, lr}
005badbc mov r4, r0
005badc0 ldr r0, [r0]
005badc4 ldr r3, [pc, #0x90]
005badc8 mov r5, r1
005badcc cmp r0, #0
005badd0 add r3, pc, r3
005badd4 beq #0x5bae10
005badd8 ldrb r2, [r1, #0x40]
005baddc cmp r2, #0
005bade0 beq #0x5bae04
005bade4 ldr r2, [pc, #0x74]
005bade8 ldr r3, [r3, r2]
005badec ldr r2, [r3]
005badf0 str r2, [r0]
005badf4 str r0, [r3]
005badf8 mov r3, #0
005badfc str r3, [r4]
005bae00 pop {r4, r5, r6, pc}
005bae04 mov r2, #0x41
005bae08 pop {r4, r5, r6, lr}
005bae0c b #0x30e868
005bae10 ldrb r2, [r1, #0x40]
005bae14 cmp r2, #0
005bae18 bne #0x5bae4c
005bae1c ldr r2, [pc, #0x3c]
005bae20 ldr r3, [r3, r2]
005bae24 ldr r6, [r3]
005bae28 cmp r6, #0
005bae2c beq #0x5bae50
005bae30 ldr r2, [r6]
005bae34 str r2, [r3]
005bae38 mov r1, r5
005bae3c mov r0, r6
005bae40 bl #0x5baa94
005bae44 str r6, [r4]
005bae48 pop {r4, r5, r6, pc}
005bae4c pop {r4, r5, r6, pc}
005bae50 bl #0x5bac80
005bae54 mov r6, r0
005bae58 b #0x5bae38
005bae5c eorseq sb, sp, r0, asr #25
005bae60 andeq r3, r0, r0, asr #25
