0045fefc f0412de9 push {r4, r5, r6, r7, r8, lr}
0045ff00 e8409fe5 ldr r4, [pc, #0xe8]
0045ff04 e8509fe5 ldr r5, [pc, #0xe8]
0045ff08 28d04de2 sub sp, sp, #0x28
0045ff0c 04408fe0 add r4, pc, r4
0045ff10 053094e7 ldr r3, [r4, r5]
0045ff14 000051e3 cmp r1, #0
0045ff18 003093e5 ldr r3, [r3]
0045ff1c 24308de5 str r3, [sp, #0x24]
0045ff20 0c6090e5 ldr r6, [r0, #0xc]
0045ff24 0200000a beq #0x45ff34
0045ff28 0c30d6e5 ldrb r3, [r6, #0xc]
0045ff2c 000053e3 cmp r3, #0
0045ff30 1a00000a beq #0x45ffa0
0045ff34 bc309fe5 ldr r3, [pc, #0xbc]
0045ff38 0c708de2 add r7, sp, #0xc
0045ff3c 038094e7 ldr r8, [r4, r3]
0045ff40 0800a0e1 mov r0, r8
0045ff44 4f5efbeb bl #0x337888
0045ff48 ac109fe5 ldr r1, [pc, #0xac]
0045ff4c 08208de2 add r2, sp, #8
0045ff50 0700a0e1 mov r0, r7
0045ff54 01108fe0 add r1, pc, r1
0045ff58 63d0faeb bl #0x3140ec
0045ff5c 0710a0e1 mov r1, r7
0045ff60 0800a0e1 mov r0, r8
0045ff64 c75efbeb bl #0x337a88
0045ff68 0700a0e1 mov r0, r7
0045ff6c b8e0faeb bl #0x318254
0045ff70 0cc0d6e5 ldrb ip, [r6, #0xc]
0045ff74 00005ce3 cmp ip, #0
0045ff78 0f00001a bne #0x45ffbc
0045ff7c 7c209fe5 ldr r2, [pc, #0x7c]
0045ff80 083096e5 ldr r3, [r6, #8]
0045ff84 101096e5 ldr r1, [r6, #0x10]
0045ff88 020094e7 ldr r0, [r4, r2]
0045ff8c 0d20d6e5 ldrb r2, [r6, #0xd]
0045ff90 000090e5 ldr r0, [r0]
0045ff94 04c08de5 str ip, [sp, #4]
0045ff98 00c08de5 str ip, [sp]
0045ff9c 1a2efceb bl #0x36b80c
0045ffa0 053094e7 ldr r3, [r4, r5]
0045ffa4 24209de5 ldr r2, [sp, #0x24]
0045ffa8 003093e5 ldr r3, [r3]
0045ffac 030052e1 cmp r2, r3
0045ffb0 0d00001a bne #0x45ffec
0045ffb4 28d08de2 add sp, sp, #0x28
0045ffb8 f081bde8 pop {r4, r5, r6, r7, r8, pc}
0045ffbc 3c209fe5 ldr r2, [pc, #0x3c]
0045ffc0 083096e5 ldr r3, [r6, #8]
0045ffc4 101096e5 ldr r1, [r6, #0x10]
0045ffc8 020094e7 ldr r0, [r4, r2]
0045ffcc 7dcea0e3 mov ip, #0x7d0
0045ffd0 0d20d6e5 ldrb r2, [r6, #0xd]
0045ffd4 000090e5 ldr r0, [r0]
0045ffd8 003053e2 subs r3, r3, #0
0045ffdc 0130a013 movne r3, #1
0045ffe0 00c08de5 str ip, [sp]
0045ffe4 632ffceb bl #0x36bd78
0045ffe8 ecffffea b #0x45ffa0
0045ffec c7b8faeb bl #0x30e310
0045fff0 844b5300 subseq r4, r3, r4, lsl #23
0045fff4 ac400000 andeq r4, r0, ip, lsr #1
0045fff8 84080000 andeq r0, r0, r4, lsl #17
0045fffc 2cd14600 subeq sp, r6, ip, lsr #2
00460000 a40d0000 andeq r0, r0, r4, lsr #27
