00455c08 0c2090e5 ldr r2, [r0, #0xc]
00455c0c 24309fe5 ldr r3, [pc, #0x24]
00455c10 1820d2e5 ldrb r2, [r2, #0x18]
00455c14 03308fe0 add r3, pc, r3
00455c18 000052e3 cmp r2, #0
00455c1c 0100001a bne #0x455c28
00455c20 0200a0e1 mov r0, r2
00455c24 1eff2fe1 bx lr
00455c28 0c209fe5 ldr r2, [pc, #0xc]
00455c2c 101090e5 ldr r1, [r0, #0x10]
00455c30 020093e7 ldr r0, [r3, r2]
00455c34 ecffffea b #0x455bec
00455c38 7cee5300 subseq lr, r3, ip, ror lr
00455c3c 201a0000 andeq r1, r0, r0, lsr #20
