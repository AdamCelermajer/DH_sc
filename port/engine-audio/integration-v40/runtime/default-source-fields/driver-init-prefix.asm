00893108 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0089310c add r1, r0, #8
00893110 mov r4, r0
00893114 sub sp, sp, #0x3c
00893118 mov r0, r1
0089311c ldr r5, [pc, #0x338]
00893120 str r1, [sp, #4]
00893124 bl #0x89347c
00893128 ldr r3, [pc, #0x330]
0089312c add r5, pc, r5
00893130 ldr r6, [r4, #0x14]
00893134 ldr r3, [r5, r3]
00893138 mov r7, #0x4000
0089313c str r7, [r4, #0x28]
00893140 str r7, [r4, #0x34]
00893144 ldr r1, [r3]
00893148 lsl r0, r6, #0xe
0089314c bl #0x30e2a4
00893150 ldr r1, [r4, #0x10]
00893154 mov r3, #0x96
00893158 ldr r5, [r4, #0x18]
0089315c mul r6, r6, r1
00893160 asr r5, r5, #3
00893164 mul r6, r3, r6
00893168 movw r3, #0x4dd3
0089316c mul r6, r5, r6
00893170 movt r3, #0x1062
00893174 smull r2, sl, r3, r6
00893178 mvn lr, #0x80000000
0089317c asr r6, r6, #0x1f
00893180 mov r2, #0x43000000
00893184 mov r3, #0
00893188 sub lr, lr, #0x800000
0089318c mov ip, #0x3f800000
00893190 add r2, r2, #0xb40000
00893194 rsb sl, r6, sl, asr #6
00893198 mov r6, #0
0089319c str r0, [r4, #0xc]
008931a0 str r3, [r4, #0xa8]
008931a4 str r3, [r4, #0x6c]
008931a8 str r3, [r4, #0x70]
008931ac str r3, [r4, #0x74]
008931b0 str r3, [r4, #0x78]
008931b4 str r3, [r4, #0x7c]
008931b8 str r3, [r4, #0x80]
008931bc str r3, [r4, #0x84]
008931c0 str r3, [r4, #0x88]
008931c4 str r3, [r4, #0x8c]
008931c8 mov r0, sl
008931cc str ip, [r4, #0x9c]
008931d0 str r2, [r4, #0xa4]
008931d4 str lr, [r4, #0xac]
008931d8 str r7, [r4, #0xb0]
008931dc mul r1, r1, r5
008931e0 str r6, [r4, #0x90]
008931e4 str lr, [r4, #0x94]
008931e8 str ip, [r4, #0x98]
008931ec str r2, [r4, #0xa0]
008931f0 str r6, [r4, #0x48]
008931f4 str r6, [r4, #0x4c]
