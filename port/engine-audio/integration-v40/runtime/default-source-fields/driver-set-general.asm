00890198 sub sp, sp, #0x10
0089019c push {r4, r5, r6, lr}
008901a0 ldr r4, [pc, #0x98]
008901a4 add lr, sp, #0x10
008901a8 stm lr, {r0, r1, r2, r3}
008901ac ldr r3, [pc, #0x90]
008901b0 add r4, pc, r4
008901b4 ldr r6, [sp, #0x44]
008901b8 ldr r5, [r4, r3]
008901bc ldm lr!, {r0, r1, r2, r3}
008901c0 mov ip, r5
008901c4 stm ip!, {r0, r1, r2, r3}
008901c8 ldm lr!, {r0, r1, r2, r3}
008901cc stm ip!, {r0, r1, r2, r3}
008901d0 ldm lr, {r0, r1, r2, r3}
008901d4 ldr lr, [pc, #0x6c]
008901d8 stm ip, {r0, r1, r2, r3}
008901dc ldr lr, [r4, lr]
008901e0 ldr r3, [sp, #0x48]
008901e4 ldr r5, [sp, #0x40]
008901e8 mov r1, #0
008901ec str r3, [lr]
008901f0 ldr r3, [pc, #0x54]
008901f4 mov r0, r5
008901f8 ldr r3, [r4, r3]
008901fc str r5, [r3]
00890200 bl #0x30e2f8
00890204 cmp r0, #0
00890208 bne #0x890224
0089020c ldr r3, [pc, #0x3c]
00890210 ldr r3, [r4, r3]
00890214 str r6, [r3]
00890218 pop {r4, r5, r6, lr}
0089021c add sp, sp, #0x10
00890220 bx lr
00890224 ldr r3, [pc, #0x24]
00890228 mov r0, r6
0089022c mov r1, r5
00890230 ldr r4, [r4, r3]
00890234 bl #0x30ec94
00890238 str r0, [r4]
0089023c b #0x890218
00890240 andseq r4, r0, r0, ror #17
00890244 andeq r3, r0, r4, ror #2
00890248 andeq r2, r0, r4, lsr #11
0089024c andeq r1, r0, r4, asr sb
00890250 andeq r2, r0, ip, lsr #11
