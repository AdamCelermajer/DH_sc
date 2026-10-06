004b313c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004b3140 mov r7, r0
004b3144 sub sp, sp, #0x1c
004b3148 bl #0x4a3808
004b314c mov r0, r7
004b3150 bl #0x313a90
004b3154 ldr r6, [pc, #0x16c]
004b3158 mov r3, #1
004b315c cmp r3, #0
004b3160 add r6, pc, r6
004b3164 str r0, [sp, #0x14]
004b3168 str r3, [sp, #0xc]
004b316c bne #0x4b31bc
004b3170 add r3, sp, #0x14
004b3174 add r2, r3, #2
004b3178 add r3, r3, #1
004b317c ldrb r0, [r2, #1]
004b3180 ldrb r1, [r3, #-1]
004b3184 cmp r2, r3
004b3188 mov r4, r2
004b318c eor r1, r0, r1
004b3190 strb r1, [r3, #-1]
004b3194 ldrb r0, [r2, #1]
004b3198 eor r1, r1, r0
004b319c strb r1, [r2, #1]
004b31a0 ldrb r0, [r3, #-1]
004b31a4 sub r2, r2, #1
004b31a8 eor r1, r1, r0
004b31ac strb r1, [r3, #-1]
004b31b0 add r3, r3, #1
004b31b4 bhi #0x4b317c
004b31b8 ldr r0, [sp, #0x14]
004b31bc ldr r3, [pc, #0x108]
004b31c0 ldr r3, [r6, r3]
004b31c4 ldr r3, [r3]
004b31c8 cmp r3, r0
004b31cc beq #0x4b31d8
004b31d0 add sp, sp, #0x1c
004b31d4 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004b31d8 lsl r0, r0, #2
004b31dc mov r1, #1
004b31e0 bl #0x31056c
004b31e4 ldr sb, [pc, #0xe4]
004b31e8 ldr r2, [sp, #0x14]
004b31ec ldr r3, [r6, sb]
004b31f0 cmp r2, #0
004b31f4 str r0, [r3]
004b31f8 beq #0x4b31d0
004b31fc add sl, sp, #0x10
004b3200 mov r8, #1
004b3204 add r1, sl, r8
004b3208 add r3, sl, #2
004b320c mov r4, #0
004b3210 stm sp, {r1, r3}
004b3214 mov r0, r7
004b3218 mov r1, sl
004b321c bl #0x3df1a0
004b3220 cmp r8, #0
004b3224 str r8, [sp, #0xc]
004b3228 bne #0x4b326c
004b322c ldr r3, [sp]
004b3230 ldr r2, [sp, #4]
004b3234 ldrb r0, [r2, #1]
004b3238 ldrb r1, [r3, #-1]
004b323c cmp r2, r3
004b3240 eor r1, r0, r1
004b3244 strb r1, [r3, #-1]
004b3248 ldrb r0, [r2, #1]
004b324c eor r1, r1, r0
004b3250 strb r1, [r2, #1]
004b3254 ldrb r0, [r3, #-1]
004b3258 sub r2, r2, #1
004b325c eor r1, r1, r0
004b3260 strb r1, [r3, #-1]
004b3264 add r3, r3, #1
004b3268 bhi #0x4b3234
004b326c ldr r0, [sp, #0x10]
004b3270 ldr r5, [r6, sb]
004b3274 mov r1, #1
004b3278 add r0, r0, r1
004b327c ldr fp, [r5]
004b3280 bl #0x31056c
004b3284 str r0, [fp, r4, lsl #2]
004b3288 ldr r3, [r5]
004b328c ldr r2, [sp, #0x10]
004b3290 mov r0, r7
004b3294 ldr r1, [r3, r4, lsl #2]
004b3298 mov r3, #0
004b329c bl #0x317454
004b32a0 ldr r3, [r5]
004b32a4 mov r1, #0
004b32a8 ldr r2, [r3, r4, lsl #2]
004b32ac ldr r3, [sp, #0x10]
004b32b0 add r4, r4, #1
004b32b4 strb r1, [r2, r3]
004b32b8 ldr r3, [sp, #0x14]
004b32bc cmp r3, r4
004b32c0 bhi #0x4b3214
004b32c4 b #0x4b31d0
004b32c8 subeq r1, lr, r0, lsr sb
004b32cc andeq r3, r0, r8, lsr sp
004b32d0 andeq r3, r0, r8, lsr #19
