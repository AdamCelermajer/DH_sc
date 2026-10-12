00457094 10402de9 push {r4, lr}
00457098 0010a0e3 mov r1, #0
0045709c 1000a0e3 mov r0, #0x10
004570a0 32e5faeb bl #0x310570
004570a4 1c409fe5 ldr r4, [pc, #0x1c]
004570a8 1c309fe5 ldr r3, [pc, #0x1c]
004570ac 0010a0e3 mov r1, #0
004570b0 04408fe0 add r4, pc, r4
004570b4 033094e7 ldr r3, [r4, r3]
004570b8 0c1080e5 str r1, [r0, #0xc]
004570bc 083083e2 add r3, r3, #8
004570c0 003080e5 str r3, [r0]
004570c4 1080bde8 pop {r4, pc}
004570c8 e0d95300 subseq sp, r3, r0, ror #19
004570cc d4430000 ldrdeq r4, r5, [r0], -r4
