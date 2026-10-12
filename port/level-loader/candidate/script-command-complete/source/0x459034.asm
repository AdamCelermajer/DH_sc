00459034 44309fe5 ldr r3, [pc, #0x44]
00459038 44209fe5 ldr r2, [pc, #0x44]
0045903c 70402de9 push {r4, r5, r6, lr}
00459040 03308fe0 add r3, pc, r3
00459044 022093e7 ldr r2, [r3, r2]
00459048 0cc090e5 ldr ip, [r0, #0xc]
0045904c 0040a0e1 mov r4, r0
00459050 30109fe5 ldr r1, [pc, #0x30]
00459054 2c0092e5 ldr r0, [r2, #0x2c]
00459058 2c209fe5 ldr r2, [pc, #0x2c]
0045905c 01108fe0 add r1, pc, r1
00459060 0c509ce5 ldr r5, [ip, #0xc]
00459064 02208fe0 add r2, pc, r2
00459068 dbae01eb bl #0x4c4bdc
0045906c 000055e1 cmp r5, r0
00459070 0000a013 movne r0, #0
00459074 0100a003 moveq r0, #1
00459078 0400c4e5 strb r0, [r4, #4]
0045907c 7080bde8 pop {r4, r5, r6, pc}
00459080 50ba5300 subseq fp, r3, r0, asr sl
00459084 f4370000 strdeq r3, r4, [r0], -r4
00459088 cc3b4700 subeq r3, r7, ip, asr #23
0045908c 043e4700 subeq r3, r7, r4, lsl #28
