00456994 10402de9 push {r4, lr}
00456998 0010a0e3 mov r1, #0
0045699c 1400a0e3 mov r0, #0x14
004569a0 f2e6faeb bl #0x310570
004569a4 1c409fe5 ldr r4, [pc, #0x1c]
004569a8 1c309fe5 ldr r3, [pc, #0x1c]
004569ac 0010a0e3 mov r1, #0
004569b0 04408fe0 add r4, pc, r4
004569b4 033094e7 ldr r3, [r4, r3]
004569b8 0c1080e5 str r1, [r0, #0xc]
004569bc 083083e2 add r3, r3, #8
004569c0 003080e5 str r3, [r0]
004569c4 1080bde8 pop {r4, pc}
004569c8 e0e05300 subseq lr, r3, r0, ror #1
004569cc ec400000 andeq r4, r0, ip, ror #1
