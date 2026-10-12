0045eec0 f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045eec4 c4419fe5 ldr r4, [pc, #0x1c4]
0045eec8 c4519fe5 ldr r5, [pc, #0x1c4]
0045eecc c4119fe5 ldr r1, [pc, #0x1c4]
0045eed0 04408fe0 add r4, pc, r4
0045eed4 053094e7 ldr r3, [r4, r5]
0045eed8 017094e7 ldr r7, [r4, r1]
0045eedc 44d04de2 sub sp, sp, #0x44
0045eee0 003093e5 ldr r3, [r3]
0045eee4 0280a0e1 mov r8, r2
0045eee8 24608de2 add r6, sp, #0x24
0045eeec 3c308de5 str r3, [sp, #0x3c]
0045eef0 0ca090e5 ldr sl, [r0, #0xc]
0045eef4 0700a0e1 mov r0, r7
0045eef8 6262fbeb bl #0x337888
0045eefc 98119fe5 ldr r1, [pc, #0x198]
0045ef00 20208de2 add r2, sp, #0x20
0045ef04 0600a0e1 mov r0, r6
0045ef08 01108fe0 add r1, pc, r1
0045ef0c 76d4faeb bl #0x3140ec
0045ef10 0610a0e1 mov r1, r6
0045ef14 0700a0e1 mov r0, r7
0045ef18 da62fbeb bl #0x337a88
0045ef1c 0600a0e1 mov r0, r6
0045ef20 cbe4faeb bl #0x318254
0045ef24 0c609ae5 ldr r6, [sl, #0xc]
0045ef28 70119fe5 ldr r1, [pc, #0x170]
0045ef2c 0600a0e1 mov r0, r6
0045ef30 01108fe0 add r1, pc, r1
0045ef34 ebbdfaeb bl #0x30e6e8
0045ef38 009050e2 subs sb, r0, #0
0045ef3c 3600001a bne #0x45f01c
0045ef40 5c319fe5 ldr r3, [pc, #0x15c]
0045ef44 14708de2 add r7, sp, #0x14
0045ef48 01a0a0e3 mov sl, #1
0045ef4c 033094e7 ldr r3, [r4, r3]
0045ef50 383093e5 ldr r3, [r3, #0x38]
0045ef54 146093e5 ldr r6, [r3, #0x14]
0045ef58 0c8083e2 add r8, r3, #0xc
0045ef5c 060058e1 cmp r8, r6
0045ef60 1900000a beq #0x45efcc
0045ef64 2c1096e5 ldr r1, [r6, #0x2c]
0045ef68 000051e3 cmp r1, #0
0045ef6c 0b00000a beq #0x45efa0
0045ef70 0700a0e1 mov r0, r7
0045ef74 6c7bfbeb bl #0x33dd2c
0045ef78 0700a0e1 mov r0, r7
0045ef7c f483fbeb bl #0x33ff54
0045ef80 00b050e2 subs fp, r0, #0
0045ef84 0500000a beq #0x45efa0
0045ef88 78339be5 ldr r3, [fp, #0x378]
0045ef8c 09a0c3e5 strb sl, [r3, #9]
0045ef90 78039be5 ldr r0, [fp, #0x378]
0045ef94 8099feeb bl #0x40559c
0045ef98 78339be5 ldr r3, [fp, #0x378]
0045ef9c 0990c3e5 strb sb, [r3, #9]
0045efa0 0c2096e5 ldr r2, [r6, #0xc]
0045efa4 000052e3 cmp r2, #0
0045efa8 0100001a bne #0x45efb4
0045efac 0d0000ea b #0x45efe8
0045efb0 0320a0e1 mov r2, r3
0045efb4 083092e5 ldr r3, [r2, #8]
0045efb8 000053e3 cmp r3, #0
0045efbc fbffff1a bne #0x45efb0
0045efc0 0260a0e1 mov r6, r2
0045efc4 060058e1 cmp r8, r6
0045efc8 e5ffff1a bne #0x45ef64
0045efcc 053094e7 ldr r3, [r4, r5]
0045efd0 3c209de5 ldr r2, [sp, #0x3c]
0045efd4 003093e5 ldr r3, [r3]
0045efd8 030052e1 cmp r2, r3
0045efdc 2a00001a bne #0x45f08c
0045efe0 44d08de2 add sp, sp, #0x44
0045efe4 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045efe8 043096e5 ldr r3, [r6, #4]
0045efec 0c1093e5 ldr r1, [r3, #0xc]
0045eff0 010056e1 cmp r6, r1
0045eff4 0500001a bne #0x45f010
0045eff8 0360a0e1 mov r6, r3
0045effc 043093e5 ldr r3, [r3, #4]
0045f000 0c2093e5 ldr r2, [r3, #0xc]
0045f004 060052e1 cmp r2, r6
0045f008 faffff0a beq #0x45eff8
0045f00c 0c2096e5 ldr r2, [r6, #0xc]
0045f010 030052e1 cmp r2, r3
0045f014 0360a011 movne r6, r3
0045f018 cfffffea b #0x45ef5c
0045f01c 80309fe5 ldr r3, [pc, #0x80]
0045f020 08708de2 add r7, sp, #8
0045f024 0620a0e1 mov r2, r6
0045f028 031094e7 ldr r1, [r4, r3]
0045f02c 0060a0e3 mov r6, #0
0045f030 0830a0e1 mov r3, r8
0045f034 381091e5 ldr r1, [r1, #0x38]
0045f038 0700a0e1 mov r0, r7
0045f03c 00608de5 str r6, [sp]
0045f040 04608de5 str r6, [sp, #4]
0045f044 15affbeb bl #0x34aca0
0045f048 0700a0e1 mov r0, r7
0045f04c 0610a0e1 mov r1, r6
0045f050 5a83fbeb bl #0x33fdc0
0045f054 060050e1 cmp r0, r6
0045f058 dbffff0a beq #0x45efcc
0045f05c 0700a0e1 mov r0, r7
0045f060 bb83fbeb bl #0x33ff54
0045f064 007050e2 subs r7, r0, #0
0045f068 d7ffff0a beq #0x45efcc
0045f06c 783397e5 ldr r3, [r7, #0x378]
0045f070 0120a0e3 mov r2, #1
0045f074 0920c3e5 strb r2, [r3, #9]
0045f078 780397e5 ldr r0, [r7, #0x378]
0045f07c 4699feeb bl #0x40559c
0045f080 783397e5 ldr r3, [r7, #0x378]
0045f084 0960c3e5 strb r6, [r3, #9]
0045f088 cfffffea b #0x45efcc
0045f08c 9fbcfaeb bl #0x30e310
0045f090 c05b5300 subseq r5, r3, r0, asr #23
0045f094 ac400000 andeq r4, r0, ip, lsr #1
0045f098 84080000 andeq r0, r0, r4, lsl #17
0045f09c 78e14600 subeq lr, r6, r8, ror r1
0045f0a0 18a04600 subeq sl, r6, r8, lsl r0
0045f0a4 f4370000 strdeq r3, r4, [r0], -r4
