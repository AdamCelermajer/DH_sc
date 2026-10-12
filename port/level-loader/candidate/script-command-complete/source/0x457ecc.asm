00457ecc 10402de9 push {r4, lr}
00457ed0 0010a0e3 mov r1, #0
00457ed4 1000a0e3 mov r0, #0x10
00457ed8 a4e1faeb bl #0x310570
00457edc 28409fe5 ldr r4, [pc, #0x28]
00457ee0 28209fe5 ldr r2, [pc, #0x28]
00457ee4 0010a0e3 mov r1, #0
00457ee8 04408fe0 add r4, pc, r4
00457eec 022094e7 ldr r2, [r4, r2]
00457ef0 0c1080e5 str r1, [r0, #0xc]
00457ef4 0410c0e5 strb r1, [r0, #4]
00457ef8 082082e2 add r2, r2, #8
00457efc 002080e5 str r2, [r0]
00457f00 0020e0e3 mvn r2, #0
00457f04 082080e5 str r2, [r0, #8]
00457f08 1080bde8 pop {r4, pc}
00457f0c a8cb5300 subseq ip, r3, r8, lsr #23
00457f10 04190000 andeq r1, r0, r4, lsl #18
