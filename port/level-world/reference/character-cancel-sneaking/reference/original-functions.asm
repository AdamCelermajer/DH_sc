
# _ZN14CharProperties13PROPS_DelBuffEiPN7Structs19CharacterPropertiesE
003e101c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e1020: ldr      r4, [r0, #0xe1c]
003e1024: ldr      r7, [pc, #0x208]
003e1028: add      r5, r0, #0xe10
003e102c: cmp      r4, #0
003e1030: add      r7, pc, r7
003e1034: sub      sp, sp, #0x54
003e1038: mov      sb, r0
003e103c: mov      r6, r2
003e1040: add      r5, r5, #8
003e1044: beq      #0x3e1128
003e1048: mov      r2, r5
003e104c: b        #0x3e1054
003e1050: mov      r4, r3
003e1054: ldr      r3, [r4, #0x10]
003e1058: cmp      r3, r1
003e105c: ldrlt    r3, [r4, #0xc]
003e1060: ldrge    r3, [r4, #8]
003e1064: movlt    r4, r2
003e1068: mov      r2, r4
003e106c: cmp      r3, #0
003e1070: bne      #0x3e1050
003e1074: cmp      r5, r4
003e1078: beq      #0x3e1134
003e107c: ldr      r3, [r4, #0x10]
003e1080: cmp      r3, r1
003e1084: bgt      #0x3e1128
003e1088: cmp      r5, r4
003e108c: beq      #0x3e1134
003e1090: add      r3, r4, #0x34
003e1094: add      ip, sp, #0x18
003e1098: str      r3, [sp, #4]
003e109c: ldm      r3, {r0, r1, r2, r3}
003e10a0: stm      ip, {r0, r1, r2, r3}
003e10a4: mov      r1, ip
003e10a8: add      r0, r4, #0x44
003e10ac: bl       #0x3de870
003e10b0: cmp      r0, #1
003e10b4: beq      #0x3e1194
003e10b8: cmp      r6, #0
003e10bc: beq      #0x3e1134
003e10c0: ldr      sl, [r4, #0x40]
003e10c4: ldr      r8, [r4, #0x38]
003e10c8: ldr      r5, [r4, #0x3c]
003e10cc: ldr      r7, [r4, #0x34]
003e10d0: ldr      r3, [r4, #0x44]
003e10d4: cmp      r3, r7
003e10d8: beq      #0x3e1134
003e10dc: ldr      r4, [r7]
003e10e0: cmp      r4, r6
003e10e4: mov      fp, r4
003e10e8: beq      #0x3e1140
003e10ec: add      r7, r7, #4
003e10f0: cmp      r7, r5
003e10f4: beq      #0x3e1118
003e10f8: cmp      r3, r7
003e10fc: beq      #0x3e1134
003e1100: ldr      r4, [r7]
003e1104: cmp      r4, r6
003e1108: beq      #0x3e113c
003e110c: add      r7, r7, #4
003e1110: cmp      r5, r7
003e1114: bne      #0x3e10f8
003e1118: ldr      r8, [sl, #4]!
003e111c: add      r5, r8, #0x80
003e1120: mov      r7, r8
003e1124: b        #0x3e10d4
003e1128: mov      r4, r5
003e112c: cmp      r5, r4
003e1130: bne      #0x3e1090
003e1134: add      sp, sp, #0x54
003e1138: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e113c: mov      fp, r6
003e1140: ldr      r0, [sb, #4]
003e1144: ldr      r1, [r4, #0x388]
003e1148: add      r0, r0, #0x3b4
003e114c: bl       #0x3db2d8
003e1150: mov      r0, fp
003e1154: bl       #0x4c5740
003e1158: mov      r0, r4
003e115c: bl       #0x310440
003e1160: ldr      r1, [sp, #4]
003e1164: add      r0, sp, #0x38
003e1168: add      r2, sp, #0x28
003e116c: add      r3, sp, #0x4c
003e1170: str      sl, [sp, #0x34]
003e1174: str      r5, [sp, #0x30]
003e1178: str      r8, [sp, #0x2c]
003e117c: str      r7, [sp, #0x28]
003e1180: bl       #0x3dfa9c
003e1184: mov      r0, sb
003e1188: mov      r1, #1
003e118c: bl       #0x3e0810
003e1190: b        #0x3e1134
003e1194: ldr      lr, [sp, #4]
003e1198: ldr      ip, [sb, #4]
003e119c: add      r6, sp, #8
003e11a0: ldm      lr, {r0, r1, r2, r3}
003e11a4: stm      r6, {r0, r1, r2, r3}
003e11a8: mov      r0, r6
003e11ac: mov      r1, #0
003e11b0: add      r8, ip, #0x3b4
003e11b4: bl       #0x3de8b4
003e11b8: ldr      r3, [sp, #8]
003e11bc: mov      r0, r8
003e11c0: ldr      r3, [r3]
003e11c4: ldr      r1, [r3, #0x388]
003e11c8: bl       #0x3db2d8
003e11cc: ldr      ip, [sp, #4]
003e11d0: ldm      ip, {r0, r1, r2, r3}
003e11d4: stm      r6, {r0, r1, r2, r3}
003e11d8: mov      r0, r6
003e11dc: mov      r1, #0
003e11e0: bl       #0x3de8b4
003e11e4: ldr      r3, [sp, #8]
003e11e8: ldr      r6, [r3]
003e11ec: cmp      r6, #0
003e11f0: beq      #0x3e1204
003e11f4: mov      r0, r6
003e11f8: bl       #0x4c5740
003e11fc: mov      r0, r6
003e1200: bl       #0x310440
003e1204: ldr      r3, [pc, #0x2c]
003e1208: add      r1, r4, #0x18
003e120c: ldr      r0, [r7, r3]
003e1210: bl       #0x494978
003e1214: add      r1, sp, #0x50
003e1218: str      r4, [r1, #-8]!
003e121c: mov      r0, r5
003e1220: bl       #0x3e0fd0
003e1224: mov      r0, sb
003e1228: mov      r1, #1
003e122c: bl       #0x3e0810
003e1230: b        #0x3e1134
003e1234: subseq   r3, fp, r0, ror #20
003e1238: andeq    r1, r0, r8, lsl #22

# _ZNK9Character16GetCharSkillListEv
003bc5fc: ldr      r3, [pc, #0x20]
003bc600: ldr      r2, [pc, #0x20]
003bc604: push     {r4, lr}
003bc608: add      r3, pc, r3
003bc60c: ldr      r2, [r3, r2]
003bc610: ldr      r4, [r2]
003bc614: bl       #0x3bc5c0
003bc618: mov      r3, #0xc
003bc61c: mla      r0, r3, r0, r4
003bc620: pop      {r4, pc}
003bc624: subseq   r8, sp, r8, lsl #9
003bc628: andeq    r1, r0, r8, asr #3

# _ZNK9Character18GetCharSkillListIdEv
003bc5c0: movw     r3, #0x1068
003bc5c4: ldr      r0, [r0, r3]
003bc5c8: ldr      r3, [pc, #0x24]
003bc5cc: cmp      r0, #0
003bc5d0: add      r3, pc, r3
003bc5d4: blt      #0x3bc5ec
003bc5d8: ldr      r2, [pc, #0x18]
003bc5dc: ldr      r3, [r3, r2]
003bc5e0: ldr      r3, [r3]
003bc5e4: cmp      r0, r3
003bc5e8: bxlt     lr
003bc5ec: mov      r0, #3
003bc5f0: bx       lr
003bc5f4: subseq   r8, sp, r0, asr #9
003bc5f8: andeq    r2, r0, r8, ror sp

# _ZN17CharAISkillScript10OnPreSkillEv
003da8b8: push     {r4, r5, r6, r7, lr}
003da8bc: ldr      r4, [pc, #0x108]
003da8c0: ldr      r7, [pc, #0x108]
003da8c4: sub      sp, sp, #0x34
003da8c8: add      r4, pc, r4
003da8cc: ldr      r3, [r4, r7]
003da8d0: add      r5, sp, #4
003da8d4: mov      r6, r0
003da8d8: ldr      r3, [r3]
003da8dc: mov      r0, r5
003da8e0: str      r3, [sp, #0x2c]
003da8e4: bl       #0x31b434
003da8e8: ldr      r3, [r6, #4]
003da8ec: ldr      r0, [r3, #0x3e4]
003da8f0: cmp      r0, #0
003da8f4: beq      #0x3da918
003da8f8: ldr      r1, [pc, #0xd4]
003da8fc: mov      r3, r5
003da900: add      r2, r6, #0xc
003da904: add      r1, pc, r1
003da908: bl       #0x37c390
003da90c: ldr      r3, [sp, #0xc]
003da910: cmp      r3, #0
003da914: beq      #0x3da944
003da918: mov      r6, #0
003da91c: mov      r0, r5
003da920: bl       #0x31b398
003da924: ldr      r3, [r4, r7]
003da928: ldr      r2, [sp, #0x2c]
003da92c: mov      r0, r6
003da930: ldr      r3, [r3]
003da934: cmp      r2, r3
003da938: bne      #0x3da9c8
003da93c: add      sp, sp, #0x34
003da940: pop      {r4, r5, r6, r7, pc}
003da944: ldr      r0, [sp, #0x28]
003da948: ldm      r0, {r1, r2}
003da94c: cmp      r1, r2
003da950: beq      #0x3da95c
003da954: mov      r3, sp
003da958: bl       #0x31c3cc
003da95c: ldr      r3, [r6, #4]
003da960: ldr      r1, [pc, #0x70]
003da964: mov      r2, r5
003da968: ldr      r0, [r3, #0x3e4]
003da96c: add      r1, pc, r1
003da970: bl       #0x37c494
003da974: ldr      r1, [sp, #0xc]
003da978: cmp      r1, #0
003da97c: bne      #0x3da918
003da980: ldr      r2, [sp, #0x28]
003da984: ldr      r3, [r2]
003da988: ldr      r2, [r2, #4]
003da98c: rsb      r3, r3, r2
003da990: asr      r3, r3, #4
003da994: add      r2, r3, r3, lsl #3
003da998: add      r2, r2, r2, lsl #6
003da99c: add      r2, r3, r2, lsl #3
003da9a0: add      r2, r2, r2, lsl #15
003da9a4: add      r3, r3, r2, lsl #3
003da9a8: cmp      r3, #0
003da9ac: moveq    r6, #1
003da9b0: beq      #0x3da91c
003da9b4: mov      r0, r5
003da9b8: bl       #0x3da43c
003da9bc: bl       #0x31bc80
003da9c0: mov      r6, r0
003da9c4: b        #0x3da91c
003da9c8: bl       #0x30e310
003da9cc: subseq   sl, fp, r8, asr #3
003da9d0: andeq    r4, r0, ip, lsr #1
003da9d4: subeq    sl, lr, ip, asr #30
003da9d8: subeq    sl, lr, ip, lsl #30

# _ZNK9Character12GetCharSkillEi
003bc784: push     {r4, r5, r6, lr}
003bc788: sub      sp, sp, #8
003bc78c: mov      r5, r1
003bc790: bl       #0x3bc5c0
003bc794: ldr      r4, [pc, #0xa4]
003bc798: ldr      r3, [pc, #0xa4]
003bc79c: mov      r6, #0xc
003bc7a0: add      r4, pc, r4
003bc7a4: ldr      r3, [r4, r3]
003bc7a8: cmp      r5, #0
003bc7ac: ldr      r3, [r3]
003bc7b0: mla      r6, r6, r0, r3
003bc7b4: blt      #0x3bc7c4
003bc7b8: ldr      r3, [r6, #4]
003bc7bc: cmp      r5, r3
003bc7c0: blt      #0x3bc7e8
003bc7c4: ldr      r3, [pc, #0x7c]
003bc7c8: ldr      r3, [r4, r3]
003bc7cc: ldr      r3, [r3]
003bc7d0: cmp      r3, #2
003bc7d4: moveq    r3, #0
003bc7d8: streq    r3, [r3]
003bc7dc: beq      #0x3bc7e8
003bc7e0: cmp      r3, #1
003bc7e4: beq      #0x3bc80c
003bc7e8: ldr      r3, [pc, #0x5c]
003bc7ec: ldr      r2, [r6, #8]
003bc7f0: mov      r0, #0x4c
003bc7f4: ldr      r3, [r4, r3]
003bc7f8: ldr      r2, [r2, r5, lsl #2]
003bc7fc: ldr      r3, [r3]
003bc800: mla      r0, r0, r2, r3
003bc804: add      sp, sp, #8
003bc808: pop      {r4, r5, r6, pc}
003bc80c: ldr      r0, [pc, #0x3c]
003bc810: ldr      r1, [pc, #0x3c]
003bc814: ldr      r2, [pc, #0x3c]
003bc818: ldr      r0, [r4, r0]
003bc81c: ldr      r3, [pc, #0x38]
003bc820: mov      ip, #0x3d
003bc824: add      r1, pc, r1
003bc828: add      r2, pc, r2
003bc82c: add      r3, pc, r3
003bc830: add      r0, r0, #0xa8
003bc834: str      ip, [sp]
003bc838: bl       #0x30e004
003bc83c: b        #0x3bc7e8
003bc840: ldrsheq  r8, [sp], #-0x20
003bc844: andeq    r1, r0, r8, asr #3
003bc848: andeq    r3, r0, r0, asr #19
003bc84c: andeq    r4, r0, ip, lsl r4
003bc850: andeq    r1, r0, r0, asr #19
003bc854: ldrheq   r1, [r0], #-0xb4
003bc858: subseq   r7, r0, r0, lsr #31
003bc85c: ldrsbeq  r7, [r0], #-0xf4

# _ZN9Character14CancelSneakingEv
003bc6b8: push     {r4, r5, r6, lr}
003bc6bc: ldr      r3, [r0]
003bc6c0: mov      r5, r0
003bc6c4: mov      lr, pc
003bc6c8: ldr      pc, [r3, #0x28]
003bc6cc: ldr      r4, [pc, #0xa8]
003bc6d0: cmp      r0, #0
003bc6d4: add      r4, pc, r4
003bc6d8: bne      #0x3bc760
003bc6dc: mov      r0, r5
003bc6e0: bl       #0x3bc690
003bc6e4: cmp      r0, #0
003bc6e8: bne      #0x3bc6f0
003bc6ec: pop      {r4, r5, r6, pc}
003bc6f0: mov      r0, r5
003bc6f4: bl       #0x3bc5fc
003bc6f8: ldr      r2, [r0, #4]
003bc6fc: cmp      r2, #0
003bc700: beq      #0x3bc6ec
003bc704: ldr      r1, [pc, #0x74]
003bc708: ldr      r0, [r0, #8]
003bc70c: mov      r3, #0x4c
003bc710: ldr      ip, [r4, r1]
003bc714: ldr      r1, [r0]
003bc718: ldr      ip, [ip]
003bc71c: mla      r1, r3, r1, ip
003bc720: ldr      r1, [r1, #0x1c]
003bc724: ands     r1, r1, #0x2000000
003bc728: movne    r1, #0
003bc72c: bne      #0x3bc754
003bc730: mov      r4, r3
003bc734: add      r1, r1, #1
003bc738: cmp      r1, r2
003bc73c: beq      #0x3bc6ec
003bc740: ldr      r3, [r0, r1, lsl #2]
003bc744: mla      r3, r4, r3, ip
003bc748: ldr      r3, [r3, #0x1c]
003bc74c: tst      r3, #0x2000000
003bc750: beq      #0x3bc734
003bc754: add      r0, r5, #0x3c8
003bc758: pop      {r4, r5, r6, lr}
003bc75c: b        #0x3d84e0
003bc760: add      r0, r5, #0x560
003bc764: mov      r1, #0x92
003bc768: mov      r2, #0
003bc76c: bl       #0x3e101c
003bc770: mov      r3, #1
003bc774: strb     r3, [r5, #0x415]
003bc778: b        #0x3bc6dc
003bc77c: ldrheq   r8, [sp], #-0x3c
003bc780: andeq    r4, r0, ip, lsl r4

# _ZN6CharAI14AI_CancelSkillEj
003d84e0: push     {r4, r5, lr}
003d84e4: mov      r4, r0
003d84e8: ldr      r2, [r0, #0xb4]
003d84ec: ldr      r0, [r0, #0xb8]
003d84f0: ldr      r3, [pc, #0xc4]
003d84f4: sub      sp, sp, #0xc
003d84f8: rsb      r0, r2, r0
003d84fc: cmp      r1, r0, asr #2
003d8500: mov      r5, r1
003d8504: add      r3, pc, r3
003d8508: blo      #0x3d8530
003d850c: ldr      r1, [pc, #0xac]
003d8510: ldr      r1, [r3, r1]
003d8514: ldr      r1, [r1]
003d8518: cmp      r1, #2
003d851c: moveq    r3, #0
003d8520: streq    r3, [r3]
003d8524: beq      #0x3d8530
003d8528: cmp      r1, #1
003d852c: beq      #0x3d8584
003d8530: ldr      r3, [r2, r5, lsl #2]
003d8534: cmp      r3, #0
003d8538: beq      #0x3d8554
003d853c: ldr      r0, [r4, #4]
003d8540: mov      r1, r5
003d8544: bl       #0x3bc784
003d8548: ldr      r3, [r0, #0x48]
003d854c: cmp      r3, #1
003d8550: beq      #0x3d855c
003d8554: add      sp, sp, #0xc
003d8558: pop      {r4, r5, pc}
003d855c: ldr      r3, [r4, #0xb4]
003d8560: ldr      r0, [r3, r5, lsl #2]
003d8564: bl       #0x3db16c
003d8568: cmp      r0, #0
003d856c: beq      #0x3d8554
003d8570: ldr      r3, [r4, #0xb4]
003d8574: ldr      r0, [r3, r5, lsl #2]
003d8578: add      sp, sp, #0xc
003d857c: pop      {r4, r5, lr}
003d8580: b        #0x3da8b8
003d8584: ldr      r0, [pc, #0x38]
003d8588: ldr      r1, [pc, #0x38]
003d858c: ldr      r2, [pc, #0x38]
003d8590: ldr      r0, [r3, r0]
003d8594: ldr      r3, [pc, #0x34]
003d8598: add      r2, pc, r2
003d859c: movw     ip, #0x122
003d85a0: add      r1, pc, r1
003d85a4: add      r0, r0, #0xa8
003d85a8: add      r3, pc, r3
003d85ac: str      ip, [sp]
003d85b0: bl       #0x30e004
003d85b4: ldr      r2, [r4, #0xb4]
003d85b8: b        #0x3d8530
003d85bc: subseq   ip, fp, ip, lsl #11
003d85c0: andeq    r3, r0, r0, asr #19
003d85c4: andeq    r1, r0, r0, asr #19
003d85c8: subeq    r5, lr, r8, lsr lr
003d85cc: subeq    sp, lr, r8, ror #3
003d85d0: subeq    sp, lr, r0, lsl #3

# _ZN17CharAISkillScript19OnSkillCheck_ActiveEv
003db16c: push     {r4, r5, r6, r7, lr}
003db170: ldr      r4, [pc, #0x100]
003db174: ldr      r7, [pc, #0x100]
003db178: sub      sp, sp, #0x34
003db17c: add      r4, pc, r4
003db180: ldr      r3, [r4, r7]
003db184: add      r5, sp, #4
003db188: mov      r6, r0
003db18c: ldr      r3, [r3]
003db190: mov      r0, r5
003db194: str      r3, [sp, #0x2c]
003db198: bl       #0x31b434
003db19c: ldr      r3, [r6, #4]
003db1a0: ldr      r0, [r3, #0x3e4]
003db1a4: cmp      r0, #0
003db1a8: beq      #0x3db1cc
003db1ac: ldr      r1, [pc, #0xcc]
003db1b0: mov      r3, r5
003db1b4: add      r2, r6, #0xc
003db1b8: add      r1, pc, r1
003db1bc: bl       #0x37c390
003db1c0: ldr      r3, [sp, #0xc]
003db1c4: cmp      r3, #0
003db1c8: beq      #0x3db1f8
003db1cc: mov      r6, #0
003db1d0: mov      r0, r5
003db1d4: bl       #0x31b398
003db1d8: ldr      r3, [r4, r7]
003db1dc: ldr      r2, [sp, #0x2c]
003db1e0: mov      r0, r6
003db1e4: ldr      r3, [r3]
003db1e8: cmp      r2, r3
003db1ec: bne      #0x3db274
003db1f0: add      sp, sp, #0x34
003db1f4: pop      {r4, r5, r6, r7, pc}
003db1f8: ldr      r0, [sp, #0x28]
003db1fc: ldm      r0, {r1, r2}
003db200: cmp      r1, r2
003db204: beq      #0x3db210
003db208: mov      r3, sp
003db20c: bl       #0x31c3cc
003db210: ldr      r3, [r6, #4]
003db214: ldr      r1, [pc, #0x68]
003db218: mov      r2, r5
003db21c: ldr      r0, [r3, #0x3e4]
003db220: add      r1, pc, r1
003db224: bl       #0x37c494
003db228: ldr      r3, [sp, #0xc]
003db22c: cmp      r3, #0
003db230: bne      #0x3db1cc
003db234: ldr      r2, [sp, #0x28]
003db238: ldm      r2, {r0, r3}
003db23c: rsb      r3, r0, r3
003db240: asr      r3, r3, #4
003db244: add      r2, r3, r3, lsl #3
003db248: add      r2, r2, r2, lsl #6
003db24c: add      r2, r3, r2, lsl #3
003db250: add      r2, r2, r2, lsl #15
003db254: add      r3, r3, r2, lsl #3
003db258: rsb      r3, r3, #0
003db25c: cmp      r3, #1
003db260: bls      #0x3db1cc
003db264: add      r0, r0, #0x70
003db268: bl       #0x31bc80
003db26c: mov      r6, r0
003db270: b        #0x3db1d0
003db274: bl       #0x30e310
003db278: subseq   sb, fp, r4, lsl sb
003db27c: andeq    r4, r0, ip, lsr #1
003db280: umaaleq  sl, lr, r8, r6
003db284: subeq    sl, lr, r8, ror #12

# _ZNK9Character10IsSneakingEv
003bc690: add      r1, r0, #0xff0
003bc694: push     {r4, lr}
003bc698: add      r1, r1, #4
003bc69c: mov      r2, #0xc6
003bc6a0: add      r0, r0, #0x560
003bc6a4: bl       #0x3dedb4
003bc6a8: cmp      r0, #0
003bc6ac: movle    r0, #0
003bc6b0: movgt    r0, #1
003bc6b4: pop      {r4, pc}

# _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003dedb4: str      lr, [sp, #-4]!
003dedb8: ldr      r3, [pc, #0xf0]
003dedbc: cmp      r2, #0
003dedc0: sub      sp, sp, #0xc
003dedc4: add      r3, pc, r3
003dedc8: blt      #0x3dedfc
003dedcc: cmp      r2, #0xdf
003dedd0: ble      #0x3dee20
003dedd4: ldr      r2, [pc, #0xd8]
003dedd8: ldr      r2, [r3, r2]
003deddc: ldr      r2, [r2]
003dede0: cmp      r2, #2
003dede4: beq      #0x3dee10
003dede8: cmp      r2, #1
003dedec: beq      #0x3dee78
003dedf0: mvn      r0, #0
003dedf4: add      sp, sp, #0xc
003dedf8: ldm      sp!, {pc}
003dedfc: ldr      r2, [pc, #0xb0]
003dee00: ldr      r2, [r3, r2]
003dee04: ldr      r2, [r2]
003dee08: cmp      r2, #2
003dee0c: bne      #0x3dee38
003dee10: mov      r3, #0
003dee14: str      r3, [r3]
003dee18: mvn      r0, #0
003dee1c: b        #0x3dedf4
003dee20: ldr      r0, [pc, #0x90]
003dee24: ldr      r3, [r3, r0]
003dee28: ldr      r3, [r3, r2, lsl #2]
003dee2c: add      r1, r1, r3
003dee30: ldr      r0, [r1, #4]
003dee34: b        #0x3dedf4
003dee38: cmp      r2, #1
003dee3c: bne      #0x3dedf0
003dee40: ldr      r0, [pc, #0x74]
003dee44: ldr      r1, [pc, #0x74]
003dee48: ldr      r2, [pc, #0x74]
003dee4c: ldr      r0, [r3, r0]
003dee50: ldr      r3, [pc, #0x70]
003dee54: movw     ip, #0x103
003dee58: add      r1, pc, r1
003dee5c: add      r0, r0, #0xa8
003dee60: add      r2, pc, r2
003dee64: add      r3, pc, r3
003dee68: str      ip, [sp]
003dee6c: bl       #0x30e004
003dee70: mvn      r0, #0
003dee74: b        #0x3dedf4
003dee78: ldr      r0, [pc, #0x3c]
003dee7c: ldr      r1, [pc, #0x48]
003dee80: ldr      r2, [pc, #0x48]
003dee84: ldr      r0, [r3, r0]
003dee88: ldr      r3, [pc, #0x44]
003dee8c: mov      ip, #0x104
003dee90: add      r1, pc, r1
003dee94: add      r0, r0, #0xa8
003dee98: add      r2, pc, r2
003dee9c: add      r3, pc, r3
003deea0: str      ip, [sp]
003deea4: bl       #0x30e004
003deea8: mvn      r0, #0
003deeac: b        #0x3dedf4
003deeb0: subseq   r5, fp, ip, asr #25
003deeb4: andeq    r3, r0, r0, asr #19
003deeb8: andeq    r2, r0, r8, lsr #5
003deebc: andeq    r1, r0, r0, asr #19
003deec0: subeq    pc, sp, r0, lsl #11
003deec4: strheq   r6, [lr], #-0xe0
003deec8: subeq    r6, lr, ip, asr #28
003deecc: subeq    pc, sp, r8, asr #10
003deed0: subeq    r6, lr, r8, lsl #29
003deed4: subeq    r6, lr, r4, lsl lr
