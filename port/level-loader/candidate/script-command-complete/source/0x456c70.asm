00456c70 10402de9 push {r4, lr}
00456c74 0010a0e3 mov r1, #0
00456c78 1000a0e3 mov r0, #0x10
00456c7c 3be6faeb bl #0x310570
00456c80 1c409fe5 ldr r4, [pc, #0x1c]
00456c84 1c309fe5 ldr r3, [pc, #0x1c]
00456c88 0010a0e3 mov r1, #0
00456c8c 04408fe0 add r4, pc, r4
00456c90 033094e7 ldr r3, [r4, r3]
00456c94 0c1080e5 str r1, [r0, #0xc]
00456c98 083083e2 add r3, r3, #8
00456c9c 003080e5 str r3, [r0]
00456ca0 1080bde8 pop {r4, pc}
00456ca4 04de5300 subseq sp, r3, r4, lsl #28
00456ca8 f4350000 strdeq r3, r4, [r0], -r4
