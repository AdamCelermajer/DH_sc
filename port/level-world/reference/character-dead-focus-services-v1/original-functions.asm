
_ZN6CSDead7OnFocusEiP9CharacterP16CharStateMachineiiPv @003c4d50, bytes572, sha256=7a16b14c31d94ef4aa492583a503198b61fa4035e839d352ea75729bd56fa226
003c4d50  push     {r4, r5, r6, r7, r8, sl, lr}
003c4d54  ldr      r5, [pc, #0x210]
003c4d58  ldr      r6, [pc, #0x210]
003c4d5c  ldr      r1, [pc, #0x210]
003c4d60  add      r5, pc, r5
003c4d64  ldr      r3, [r5, r6]
003c4d68  ldr      r7, [r5, r1]
003c4d6c  sub      sp, sp, #0x4c
003c4d70  ldr      r3, [r3]
003c4d74  mov      r0, r7
003c4d78  mov      r4, r2
003c4d7c  str      r3, [sp, #0x44]
003c4d80  ldr      sl, [sp, #0x70]
003c4d84  bl       #0x337888
003c4d88  ldr      r1, [pc, #0x1e8]
003c4d8c  add      r8, sp, #0x2c
003c4d90  add      r2, sp, #0x10
003c4d94  add      r1, pc, r1
003c4d98  mov      r0, r8
003c4d9c  bl       #0x3140ec
003c4da0  mov      r1, r8
003c4da4  mov      r0, r7
003c4da8  bl       #0x337a88
003c4dac  mov      r0, r8
003c4db0  bl       #0x318254
003c4db4  mov      r0, r7
003c4db8  bl       #0x337888
003c4dbc  ldr      r1, [pc, #0x1b8]
003c4dc0  add      r8, sp, #0x14
003c4dc4  add      r2, sp, #0xc
003c4dc8  add      r1, pc, r1
003c4dcc  mov      r0, r8
003c4dd0  bl       #0x3140ec
003c4dd4  mov      r1, r8
003c4dd8  mov      r0, r7
003c4ddc  bl       #0x337a88
003c4de0  mov      r0, r8
003c4de4  bl       #0x318254
003c4de8  movw     r3, #0x241
003c4dec  str      r3, [r4, #0x520]
003c4df0  mov      r0, r4
003c4df4  ldr      r3, [r4]
003c4df8  mov      lr, pc
003c4dfc  ldr      pc, [r3, #0x28]
003c4e00  cmp      r0, #0
003c4e04  ldrne    r3, [r4, #0x520]
003c4e08  mov      r1, sl
003c4e0c  ldr      r0, [r4, #0x378]
003c4e10  orrne    r3, r3, #0x2000
003c4e14  strne    r3, [r4, #0x520]
003c4e18  bl       #0x4052bc
003c4e1c  ldr      r3, [r4, #0x378]
003c4e20  mov      r2, #1
003c4e24  mov      r0, r4
003c4e28  strb     r2, [r3, #8]
003c4e2c  bl       #0x3a4068
003c4e30  ldrb     r3, [r4, #0x53a]
003c4e34  cmp      r3, #0
003c4e38  beq      #0x3c4ee8
003c4e3c  add      r0, r4, #0x490
003c4e40  add      r0, r0, #0xc
003c4e44  mov      r1, #0
003c4e48  bl       #0x3c948c
003c4e4c  ldr      r3, [r4, #0x4e8]
003c4e50  cmn      r3, #1
003c4e54  beq      #0x3c4f14
003c4e58  ldr      r0, [r4, #0x2dc]
003c4e5c  cmp      r0, #0
003c4e60  beq      #0x3c4e7c
003c4e64  mov      ip, #0
003c4e68  mov      r1, ip
003c4e6c  movw     r2, #0x51c
003c4e70  mov      r3, #3
003c4e74  str      ip, [sp]
003c4e78  bl       #0x46ece8
003c4e7c  mov      r0, r4
003c4e80  bl       #0x3bc6b8
003c4e84  mov      r0, r4
003c4e88  bl       #0x3a40e4
003c4e8c  mov      r0, r4
003c4e90  bl       #0x3a40b0
003c4e94  add      r0, r4, #0x560
003c4e98  bl       #0x3e0af8
003c4e9c  mov      r0, r4
003c4ea0  mov      r1, #0x2a
003c4ea4  mov      r2, #0
003c4ea8  bl       #0x3a4d5c
003c4eac  mov      r0, r4
003c4eb0  mov      r1, #0x2c
003c4eb4  mov      r2, #0
003c4eb8  bl       #0x3a4d5c
003c4ebc  mov      r2, #0
003c4ec0  mov      r0, r4
003c4ec4  mov      r1, #0x2b
003c4ec8  bl       #0x3a4d5c
003c4ecc  ldr      r3, [r5, r6]
003c4ed0  ldr      r2, [sp, #0x44]
003c4ed4  ldr      r3, [r3]
003c4ed8  cmp      r2, r3
003c4edc  bne      #0x3c4f68
003c4ee0  add      sp, sp, #0x4c
003c4ee4  pop      {r4, r5, r6, r7, r8, sl, pc}
003c4ee8  add      r0, r4, #0x4f0
003c4eec  add      r0, r0, #0xc
003c4ef0  mvn      r1, #0
003c4ef4  bl       #0x3c0b50
003c4ef8  add      r0, r4, #0x490
003c4efc  add      r0, r0, #0xc
003c4f00  mov      r1, #0
003c4f04  bl       #0x3c948c
003c4f08  ldr      r3, [r4, #0x4e8]
003c4f0c  cmn      r3, #1
003c4f10  bne      #0x3c4e58
003c4f14  ldr      r3, [r4]
003c4f18  mov      r0, r4
003c4f1c  mov      lr, pc
003c4f20  ldr      pc, [r3, #0x28]
003c4f24  subs     r7, r0, #0
003c4f28  bne      #0x3c4e58
003c4f2c  ldr      r3, [pc, #0x4c]
003c4f30  ldr      r1, [pc, #0x4c]
003c4f34  ldr      r2, [pc, #0x4c]
003c4f38  ldr      r3, [r5, r3]
003c4f3c  add      r1, pc, r1
003c4f40  add      r2, pc, r2
003c4f44  ldr      r0, [r3, #0x2c]
003c4f48  bl       #0x4c4bdc
003c4f4c  mov      r2, r7
003c4f50  mov      r1, r0
003c4f54  mov      r3, #0x2e
003c4f58  add      r0, r4, #0x3b4
003c4f5c  str      r7, [sp]
003c4f60  bl       #0x3dbe24
003c4f64  b        #0x3c4e58
003c4f68  bl       #0x30e310
003c4f6c  subseq   pc, ip, r0, lsr sp
003c4f70  andeq    r4, r0, ip, lsr #1
003c4f74  andeq    r0, r0, r4, lsl #17
003c4f78  ldrheq   r0, [r0], #-0xc
003c4f7c  subseq   r0, r0, r0, lsr r1
003c4f80  strdeq   r3, r4, [r0], -r4
003c4f84  subeq    ip, pc, r4, lsl r8
003c4f88  subeq    pc, pc, r8, asr #31

_ZN9Character14CancelSneakingEv @003bc6b8, bytes204, sha256=d6373ab73d784aeec6a20fb1b71b48d3657f1e0d1dbb442d47c1d3311ef059a4
003bc6b8  push     {r4, r5, r6, lr}
003bc6bc  ldr      r3, [r0]
003bc6c0  mov      r5, r0
003bc6c4  mov      lr, pc
003bc6c8  ldr      pc, [r3, #0x28]
003bc6cc  ldr      r4, [pc, #0xa8]
003bc6d0  cmp      r0, #0
003bc6d4  add      r4, pc, r4
003bc6d8  bne      #0x3bc760
003bc6dc  mov      r0, r5
003bc6e0  bl       #0x3bc690
003bc6e4  cmp      r0, #0
003bc6e8  bne      #0x3bc6f0
003bc6ec  pop      {r4, r5, r6, pc}
003bc6f0  mov      r0, r5
003bc6f4  bl       #0x3bc5fc
003bc6f8  ldr      r2, [r0, #4]
003bc6fc  cmp      r2, #0
003bc700  beq      #0x3bc6ec
003bc704  ldr      r1, [pc, #0x74]
003bc708  ldr      r0, [r0, #8]
003bc70c  mov      r3, #0x4c
003bc710  ldr      ip, [r4, r1]
003bc714  ldr      r1, [r0]
003bc718  ldr      ip, [ip]
003bc71c  mla      r1, r3, r1, ip
003bc720  ldr      r1, [r1, #0x1c]
003bc724  ands     r1, r1, #0x2000000
003bc728  movne    r1, #0
003bc72c  bne      #0x3bc754
003bc730  mov      r4, r3
003bc734  add      r1, r1, #1
003bc738  cmp      r1, r2
003bc73c  beq      #0x3bc6ec
003bc740  ldr      r3, [r0, r1, lsl #2]
003bc744  mla      r3, r4, r3, ip
003bc748  ldr      r3, [r3, #0x1c]
003bc74c  tst      r3, #0x2000000
003bc750  beq      #0x3bc734
003bc754  add      r0, r5, #0x3c8
003bc758  pop      {r4, r5, r6, lr}
003bc75c  b        #0x3d84e0
003bc760  add      r0, r5, #0x560
003bc764  mov      r1, #0x92
003bc768  mov      r2, #0
003bc76c  bl       #0x3e101c
003bc770  mov      r3, #1
003bc774  strb     r3, [r5, #0x415]
003bc778  b        #0x3bc6dc
003bc77c  ldrheq   r8, [sp], #-0x3c
003bc780  andeq    r4, r0, ip, lsl r4

_ZNK9Character10IsSneakingEv @003bc690, bytes40, sha256=a676ab62253174c6e16ef64e181800290d6dd78d8b8b3e0c34054651dbfb76bb
003bc690  add      r1, r0, #0xff0
003bc694  push     {r4, lr}
003bc698  add      r1, r1, #4
003bc69c  mov      r2, #0xc6
003bc6a0  add      r0, r0, #0x560
003bc6a4  bl       #0x3dedb4
003bc6a8  cmp      r0, #0
003bc6ac  movle    r0, #0
003bc6b0  movgt    r0, #1
003bc6b4  pop      {r4, pc}

_ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi @003dedb4, bytes292, sha256=c227dbb320f18ab04022f3d61d90e90285652215e56bce8c89c8146aa0fa706f
003dedb4  str      lr, [sp, #-4]!
003dedb8  ldr      r3, [pc, #0xf0]
003dedbc  cmp      r2, #0
003dedc0  sub      sp, sp, #0xc
003dedc4  add      r3, pc, r3
003dedc8  blt      #0x3dedfc
003dedcc  cmp      r2, #0xdf
003dedd0  ble      #0x3dee20
003dedd4  ldr      r2, [pc, #0xd8]
003dedd8  ldr      r2, [r3, r2]
003deddc  ldr      r2, [r2]
003dede0  cmp      r2, #2
003dede4  beq      #0x3dee10
003dede8  cmp      r2, #1
003dedec  beq      #0x3dee78
003dedf0  mvn      r0, #0
003dedf4  add      sp, sp, #0xc
003dedf8  ldm      sp!, {pc}
003dedfc  ldr      r2, [pc, #0xb0]
003dee00  ldr      r2, [r3, r2]
003dee04  ldr      r2, [r2]
003dee08  cmp      r2, #2
003dee0c  bne      #0x3dee38
003dee10  mov      r3, #0
003dee14  str      r3, [r3]
003dee18  mvn      r0, #0
003dee1c  b        #0x3dedf4
003dee20  ldr      r0, [pc, #0x90]
003dee24  ldr      r3, [r3, r0]
003dee28  ldr      r3, [r3, r2, lsl #2]
003dee2c  add      r1, r1, r3
003dee30  ldr      r0, [r1, #4]
003dee34  b        #0x3dedf4
003dee38  cmp      r2, #1
003dee3c  bne      #0x3dedf0
003dee40  ldr      r0, [pc, #0x74]
003dee44  ldr      r1, [pc, #0x74]
003dee48  ldr      r2, [pc, #0x74]
003dee4c  ldr      r0, [r3, r0]
003dee50  ldr      r3, [pc, #0x70]
003dee54  movw     ip, #0x103
003dee58  add      r1, pc, r1
003dee5c  add      r0, r0, #0xa8
003dee60  add      r2, pc, r2
003dee64  add      r3, pc, r3
003dee68  str      ip, [sp]
003dee6c  bl       #0x30e004
003dee70  mvn      r0, #0
003dee74  b        #0x3dedf4
003dee78  ldr      r0, [pc, #0x3c]
003dee7c  ldr      r1, [pc, #0x48]
003dee80  ldr      r2, [pc, #0x48]
003dee84  ldr      r0, [r3, r0]
003dee88  ldr      r3, [pc, #0x44]
003dee8c  mov      ip, #0x104
003dee90  add      r1, pc, r1
003dee94  add      r0, r0, #0xa8
003dee98  add      r2, pc, r2
003dee9c  add      r3, pc, r3
003deea0  str      ip, [sp]
003deea4  bl       #0x30e004
003deea8  mvn      r0, #0
003deeac  b        #0x3dedf4
003deeb0  subseq   r5, fp, ip, asr #25
003deeb4  andeq    r3, r0, r0, asr #19
003deeb8  andeq    r2, r0, r8, lsr #5
003deebc  andeq    r1, r0, r0, asr #19
003deec0  subeq    pc, sp, r0, lsl #11
003deec4  strheq   r6, [lr], #-0xe0
003deec8  subeq    r6, lr, ip, asr #28
003deecc  subeq    pc, sp, r8, asr #10
003deed0  subeq    r6, lr, r8, lsl #29
003deed4  subeq    r6, lr, r4, lsl lr

_ZNK9Character18GetCharSkillListIdEv @003bc5c0, bytes60, sha256=ae0ccd8e8da06e96819b3c7d77bba945702d41a97bf35ff5530b54a83a72bc20
003bc5c0  movw     r3, #0x1068
003bc5c4  ldr      r0, [r0, r3]
003bc5c8  ldr      r3, [pc, #0x24]
003bc5cc  cmp      r0, #0
003bc5d0  add      r3, pc, r3
003bc5d4  blt      #0x3bc5ec
003bc5d8  ldr      r2, [pc, #0x18]
003bc5dc  ldr      r3, [r3, r2]
003bc5e0  ldr      r3, [r3]
003bc5e4  cmp      r0, r3
003bc5e8  bxlt     lr
003bc5ec  mov      r0, #3
003bc5f0  bx       lr
003bc5f4  subseq   r8, sp, r0, asr #9
003bc5f8  andeq    r2, r0, r8, ror sp

_ZNK9Character16GetCharSkillListEv @003bc5fc, bytes48, sha256=7711762aa425c1b5e34dd0b9ce1f2e0d9316ef3d7c0c7ad43a51b30e0dcd9210
003bc5fc  ldr      r3, [pc, #0x20]
003bc600  ldr      r2, [pc, #0x20]
003bc604  push     {r4, lr}
003bc608  add      r3, pc, r3
003bc60c  ldr      r2, [r3, r2]
003bc610  ldr      r4, [r2]
003bc614  bl       #0x3bc5c0
003bc618  mov      r3, #0xc
003bc61c  mla      r0, r3, r0, r4
003bc620  pop      {r4, pc}
003bc624  subseq   r8, sp, r8, lsl #9
003bc628  andeq    r1, r0, r8, asr #3

_ZNK9Character12GetCharSkillEi @003bc784, bytes220, sha256=dc2d085f0cc0fd467f8bcf96357f3c2e4fe3bd99aaf38585eab36589e77020ad
003bc784  push     {r4, r5, r6, lr}
003bc788  sub      sp, sp, #8
003bc78c  mov      r5, r1
003bc790  bl       #0x3bc5c0
003bc794  ldr      r4, [pc, #0xa4]
003bc798  ldr      r3, [pc, #0xa4]
003bc79c  mov      r6, #0xc
003bc7a0  add      r4, pc, r4
003bc7a4  ldr      r3, [r4, r3]
003bc7a8  cmp      r5, #0
003bc7ac  ldr      r3, [r3]
003bc7b0  mla      r6, r6, r0, r3
003bc7b4  blt      #0x3bc7c4
003bc7b8  ldr      r3, [r6, #4]
003bc7bc  cmp      r5, r3
003bc7c0  blt      #0x3bc7e8
003bc7c4  ldr      r3, [pc, #0x7c]
003bc7c8  ldr      r3, [r4, r3]
003bc7cc  ldr      r3, [r3]
003bc7d0  cmp      r3, #2
003bc7d4  moveq    r3, #0
003bc7d8  streq    r3, [r3]
003bc7dc  beq      #0x3bc7e8
003bc7e0  cmp      r3, #1
003bc7e4  beq      #0x3bc80c
003bc7e8  ldr      r3, [pc, #0x5c]
003bc7ec  ldr      r2, [r6, #8]
003bc7f0  mov      r0, #0x4c
003bc7f4  ldr      r3, [r4, r3]
003bc7f8  ldr      r2, [r2, r5, lsl #2]
003bc7fc  ldr      r3, [r3]
003bc800  mla      r0, r0, r2, r3
003bc804  add      sp, sp, #8
003bc808  pop      {r4, r5, r6, pc}
003bc80c  ldr      r0, [pc, #0x3c]
003bc810  ldr      r1, [pc, #0x3c]
003bc814  ldr      r2, [pc, #0x3c]
003bc818  ldr      r0, [r4, r0]
003bc81c  ldr      r3, [pc, #0x38]
003bc820  mov      ip, #0x3d
003bc824  add      r1, pc, r1
003bc828  add      r2, pc, r2
003bc82c  add      r3, pc, r3
003bc830  add      r0, r0, #0xa8
003bc834  str      ip, [sp]
003bc838  bl       #0x30e004
003bc83c  b        #0x3bc7e8
003bc840  ldrsheq  r8, [sp], #-0x20
003bc844  andeq    r1, r0, r8, asr #3
003bc848  andeq    r3, r0, r0, asr #19
003bc84c  andeq    r4, r0, ip, lsl r4
003bc850  andeq    r1, r0, r0, asr #19
003bc854  ldrheq   r1, [r0], #-0xb4
003bc858  subseq   r7, r0, r0, lsr #31
003bc85c  ldrsbeq  r7, [r0], #-0xf4

_ZN6CharAI14AI_CancelSkillEj @003d84e0, bytes244, sha256=b304aa5ca13b9b51caf272960f8e93cfcad7b3fd364762b80130a6c50f6f0924
003d84e0  push     {r4, r5, lr}
003d84e4  mov      r4, r0
003d84e8  ldr      r2, [r0, #0xb4]
003d84ec  ldr      r0, [r0, #0xb8]
003d84f0  ldr      r3, [pc, #0xc4]
003d84f4  sub      sp, sp, #0xc
003d84f8  rsb      r0, r2, r0
003d84fc  cmp      r1, r0, asr #2
003d8500  mov      r5, r1
003d8504  add      r3, pc, r3
003d8508  blo      #0x3d8530
003d850c  ldr      r1, [pc, #0xac]
003d8510  ldr      r1, [r3, r1]
003d8514  ldr      r1, [r1]
003d8518  cmp      r1, #2
003d851c  moveq    r3, #0
003d8520  streq    r3, [r3]
003d8524  beq      #0x3d8530
003d8528  cmp      r1, #1
003d852c  beq      #0x3d8584
003d8530  ldr      r3, [r2, r5, lsl #2]
003d8534  cmp      r3, #0
003d8538  beq      #0x3d8554
003d853c  ldr      r0, [r4, #4]
003d8540  mov      r1, r5
003d8544  bl       #0x3bc784
003d8548  ldr      r3, [r0, #0x48]
003d854c  cmp      r3, #1
003d8550  beq      #0x3d855c
003d8554  add      sp, sp, #0xc
003d8558  pop      {r4, r5, pc}
003d855c  ldr      r3, [r4, #0xb4]
003d8560  ldr      r0, [r3, r5, lsl #2]
003d8564  bl       #0x3db16c
003d8568  cmp      r0, #0
003d856c  beq      #0x3d8554
003d8570  ldr      r3, [r4, #0xb4]
003d8574  ldr      r0, [r3, r5, lsl #2]
003d8578  add      sp, sp, #0xc
003d857c  pop      {r4, r5, lr}
003d8580  b        #0x3da8b8
003d8584  ldr      r0, [pc, #0x38]
003d8588  ldr      r1, [pc, #0x38]
003d858c  ldr      r2, [pc, #0x38]
003d8590  ldr      r0, [r3, r0]
003d8594  ldr      r3, [pc, #0x34]
003d8598  add      r2, pc, r2
003d859c  movw     ip, #0x122
003d85a0  add      r1, pc, r1
003d85a4  add      r0, r0, #0xa8
003d85a8  add      r3, pc, r3
003d85ac  str      ip, [sp]
003d85b0  bl       #0x30e004
003d85b4  ldr      r2, [r4, #0xb4]
003d85b8  b        #0x3d8530
003d85bc  subseq   ip, fp, ip, lsl #11
003d85c0  andeq    r3, r0, r0, asr #19
003d85c4  andeq    r1, r0, r0, asr #19
003d85c8  subeq    r5, lr, r8, lsr lr
003d85cc  subeq    sp, lr, r8, ror #3
003d85d0  subeq    sp, lr, r0, lsl #3

_ZN9Character26RemoveMultiplayerHighlightEv @003a4068, bytes72, sha256=55fe16c35ef299a3a3d39441c7d9f6414593f6c30e1a303a8548eb7982a3aaa4
003a4068  push     {r4, r5, r6, lr}
003a406c  movw     r5, #0x14a0
003a4070  ldr      r2, [r0, r5]
003a4074  ldr      r3, [pc, #0x2c]
003a4078  mov      r4, r0
003a407c  cmp      r2, #0
003a4080  add      r3, pc, r3
003a4084  beq      #0x3a40a4
003a4088  ldr      r2, [pc, #0x1c]
003a408c  add      r1, r0, #0x1480
003a4090  add      r1, r1, #0x20
003a4094  ldr      r0, [r3, r2]
003a4098  bl       #0x494978
003a409c  mov      r3, #0
003a40a0  str      r3, [r4, r5]
003a40a4  pop      {r4, r5, r6, pc}
003a40a8  subseq   r0, pc, r0, lsl sl
003a40ac  andeq    r1, r0, r8, lsl #22

_ZN9Character14DisableStateFXEv @003a40e4, bytes52, sha256=5cee7994f802b8f65bf5fc90858b61b248328747a20828b4911995e3de62d50e
003a40e4  movw     r3, #0x148c
003a40e8  ldr      r2, [r0, r3]
003a40ec  ldr      r3, [pc, #0x1c]
003a40f0  cmp      r2, #0
003a40f4  add      r3, pc, r3
003a40f8  bxeq     lr
003a40fc  ldr      r2, [pc, #0x10]
003a4100  add      r1, r0, #0x1480
003a4104  add      r1, r1, #0xc
003a4108  ldr      r0, [r3, r2]
003a410c  b        #0x494978

_ZN9Character13DisableSelfFXEv @003a40b0, bytes52, sha256=6c1bde33e2bcab23cc7e4900b7203df67156985ac591ee624d1336fec80273fd
003a40b0  movw     r3, #0x1484
003a40b4  ldr      r2, [r0, r3]
003a40b8  ldr      r3, [pc, #0x1c]
003a40bc  cmp      r2, #0
003a40c0  add      r3, pc, r3
003a40c4  bxeq     lr
003a40c8  ldr      r2, [pc, #0x10]
003a40cc  add      r1, r0, #0x1480
003a40d0  add      r1, r1, #4
003a40d4  ldr      r0, [r3, r2]
003a40d8  b        #0x494978
003a40dc  ldrsbeq  r0, [pc], #-0x90
003a40e0  andeq    r1, r0, r8, lsl #22

_ZN14CharProperties20PROPS_RemoveAllBuffsEv @003e0af8, bytes372, sha256=e4cd4ad28f349467bbec59eaf964f82b45950f5ee9ddd666a8587048c255165a
003e0af8  push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e0afc  ldr      r2, [pc, #0x160]
003e0b00  sub      sp, sp, #0x34
003e0b04  add      r3, r0, #0xe10
003e0b08  add      r2, pc, r2
003e0b0c  str      r2, [sp, #8]
003e0b10  ldr      r2, [pc, #0x150]
003e0b14  add      r3, r3, #8
003e0b18  str      r3, [sp, #4]
003e0b1c  ldr      sl, [r0, #0xe20]
003e0b20  mov      r7, r0
003e0b24  add      sb, sp, #0x20
003e0b28  add      r4, sp, #0x10
003e0b2c  str      r2, [sp, #0xc]
003e0b30  ldr      r3, [sp, #4]
003e0b34  cmp      r3, sl
003e0b38  beq      #0x3e0bec
003e0b3c  add      r5, sl, #0x34
003e0b40  ldm      r5, {r0, r1, r2, r3}
003e0b44  stm      sb, {r0, r1, r2, r3}
003e0b48  add      r0, sl, #0x44
003e0b4c  mov      r1, sb
003e0b50  bl       #0x3de870
003e0b54  subs     r8, r0, #0
003e0b58  beq      #0x3e0ba8
003e0b5c  mov      r6, #0
003e0b60  ldm      r5, {r0, r1, r2, r3}
003e0b64  stm      r4, {r0, r1, r2, r3}
003e0b68  mov      r1, r6
003e0b6c  mov      r0, r4
003e0b70  bl       #0x3de8b4
003e0b74  ldr      r3, [sp, #0x10]
003e0b78  ldr      r0, [r7, #4]
003e0b7c  add      r6, r6, #1
003e0b80  ldr      fp, [r3]
003e0b84  add      r0, r0, #0x3b4
003e0b88  ldr      r1, [fp, #0x388]
003e0b8c  bl       #0x3db2d8
003e0b90  mov      r0, fp
003e0b94  bl       #0x4c5740
003e0b98  mov      r0, fp
003e0b9c  bl       #0x310440
003e0ba0  cmp      r6, r8
003e0ba4  bne      #0x3e0b60
003e0ba8  ldr      r2, [sp, #8]
003e0bac  ldr      r3, [sp, #0xc]
003e0bb0  add      r1, sl, #0x18
003e0bb4  ldr      r0, [r2, r3]
003e0bb8  bl       #0x494978
003e0bbc  ldr      r2, [sl, #0xc]
003e0bc0  cmp      r2, #0
003e0bc4  bne      #0x3e0bd0
003e0bc8  b        #0x3e0c30
003e0bcc  mov      r2, r3
003e0bd0  ldr      r3, [r2, #8]
003e0bd4  cmp      r3, #0
003e0bd8  bne      #0x3e0bcc
003e0bdc  ldr      r3, [sp, #4]
003e0be0  mov      sl, r2
003e0be4  cmp      r3, sl
003e0be8  bne      #0x3e0b3c
003e0bec  ldr      r3, [r7, #0xe28]
003e0bf0  cmp      r3, #0
003e0bf4  beq      #0x3e0c1c
003e0bf8  ldr      r0, [sp, #4]
003e0bfc  ldr      r1, [r7, #0xe1c]
003e0c00  bl       #0x3e0ab8
003e0c04  ldr      r2, [sp, #4]
003e0c08  mov      r3, #0
003e0c0c  str      r3, [r7, #0xe28]
003e0c10  str      r2, [r7, #0xe24]
003e0c14  str      r2, [r7, #0xe20]
003e0c18  str      r3, [r7, #0xe1c]
003e0c1c  mov      r0, r7
003e0c20  mov      r1, #1
003e0c24  bl       #0x3e0810
003e0c28  add      sp, sp, #0x34
003e0c2c  pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e0c30  ldr      r3, [sl, #4]
003e0c34  ldr      r1, [r3, #0xc]
003e0c38  cmp      r1, sl
003e0c3c  bne      #0x3e0c58
003e0c40  mov      sl, r3
003e0c44  ldr      r3, [r3, #4]
003e0c48  ldr      r2, [r3, #0xc]
003e0c4c  cmp      r2, sl
003e0c50  beq      #0x3e0c40
003e0c54  ldr      r2, [sl, #0xc]
003e0c58  cmp      r3, r2
003e0c5c  movne    sl, r3
003e0c60  b        #0x3e0b30
003e0c64  subseq   r3, fp, r8, lsl #31
003e0c68  andeq    r1, r0, r8, lsl #22

_ZN9CharacterC2EN10ObjectBase6GO_IDSE @003a9340, bytes1448, sha256=ef72da53c9a0ec2014a3fbcf81a04d57cc49859415d822fd932ca11ff0e77ceb
003a9340  push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a9344  add      ip, r0, #0x374
003a9348  sub      sp, sp, #0x3c
003a934c  mov      r4, r0
003a9350  str      ip, [sp, #0xc]
003a9354  bl       #0x38c398
003a9358  ldr      ip, [sp, #0xc]
003a935c  add      r5, r4, #0x4f0
003a9360  add      r5, r5, #0xc
003a9364  mov      r0, ip
003a9368  bl       #0x404db8
003a936c  add      r0, r4, #0x3b4
003a9370  str      r0, [sp, #0x20]
003a9374  add      r0, r4, #0x37c
003a9378  bl       #0x3ff330
003a937c  add      r2, r4, #0x490
003a9380  add      r1, r4, #0x3c8
003a9384  add      r2, r2, #0xc
003a9388  ldr      r0, [sp, #0x20]
003a938c  str      r1, [sp, #0x1c]
003a9390  str      r2, [sp, #0x14]
003a9394  bl       #0x3dbb0c
003a9398  ldr      r0, [sp, #0x1c]
003a939c  bl       #0x3cebf0
003a93a0  ldr      r0, [sp, #0x14]
003a93a4  bl       #0x3c8ff4
003a93a8  add      r3, r4, #0x560
003a93ac  mov      r0, r5
003a93b0  str      r3, [sp, #0x18]
003a93b4  ldr      sb, [pc, #0x50c]
003a93b8  bl       #0x3c1b58
003a93bc  ldr      r0, [sp, #0x18]
003a93c0  bl       #0x3df084
003a93c4  ldr      lr, [pc, #0x500]
003a93c8  add      sb, pc, sb
003a93cc  mov      r8, #0
003a93d0  ldr      lr, [sb, lr]
003a93d4  mov      fp, #1
003a93d8  mvn      r6, #0
003a93dc  add      sl, lr, #0x324
003a93e0  str      sl, [sp, #0x34]
003a93e4  add      sl, lr, #0x180
003a93e8  str      sl, [sp, #0x10]
003a93ec  add      sl, lr, #0x1f4
003a93f0  str      sl, [sp, #0x24]
003a93f4  add      sl, lr, #0x220
003a93f8  str      sl, [sp, #0x28]
003a93fc  add      sl, lr, #0x230
003a9400  str      sl, [sp, #0x2c]
003a9404  add      r0, lr, #8
003a9408  add      r1, lr, #0x15c
003a940c  add      r2, lr, #0x168
003a9410  add      sl, lr, #0x304
003a9414  str      sl, [sp, #0x30]
003a9418  stm      r4, {r0, r1}
003a941c  str      r2, [r4, #0x24]
003a9420  ldr      r0, [sp, #0x10]
003a9424  add      lr, lr, #0x314
003a9428  add      r7, r4, #0x1380
003a942c  str      r0, [r4, #0x374]
003a9430  ldr      r1, [sp, #0x24]
003a9434  add      r3, r7, #0x18
003a9438  movw     sl, #0x13a8
003a943c  str      r1, [r4, #0x37c]
003a9440  ldr      r2, [sp, #0x28]
003a9444  add      r7, r7, #0x30
003a9448  str      r2, [r4, #0x3b4]
003a944c  ldr      r0, [sp, #0x2c]
003a9450  str      r0, [r4, #0x3c8]
003a9454  ldr      r1, [sp, #0x30]
003a9458  str      lr, [r4, #0x4fc]
003a945c  mov      r0, r3
003a9460  str      r1, [r4, #0x49c]
003a9464  ldr      r2, [sp, #0x34]
003a9468  mov      r1, #0x10
003a946c  str      r2, [r4, #0x560]
003a9470  movw     r2, #0x1394
003a9474  strb     r8, [r4, r2]
003a9478  movw     r2, #0x1395
003a947c  strb     r8, [r4, r2]
003a9480  movw     r2, #0x1396
003a9484  strb     fp, [r4, r2]
003a9488  movw     r2, #0x1397
003a948c  strb     r6, [r4, r2]
003a9490  movw     r2, #0x13ac
003a9494  str      r3, [r4, r2]
003a9498  str      r3, [r4, sl]
003a949c  bl       #0x31167c
003a94a0  ldr      r3, [r4, sl]
003a94a4  mov      sl, #0x13c0
003a94a8  mov      r0, r7
003a94ac  strb     r8, [r3]
003a94b0  movw     r3, #0x13c4
003a94b4  str      r7, [r4, r3]
003a94b8  mov      r1, #0x10
003a94bc  str      r7, [r4, sl]
003a94c0  bl       #0x31167c
003a94c4  ldr      r2, [r4, sl]
003a94c8  add      r7, r4, sl
003a94cc  add      r3, r7, #0xc
003a94d0  strb     r8, [r2]
003a94d4  movw     r2, #0x13c8
003a94d8  strh     r6, [r4, r2]
003a94dc  movw     r2, #0x13ca
003a94e0  strh     r6, [r4, r2]
003a94e4  movw     sl, #0x13dc
003a94e8  movw     r2, #0x13e0
003a94ec  str      r3, [r4, r2]
003a94f0  mov      r0, r3
003a94f4  str      r3, [r4, sl]
003a94f8  mov      r1, #0x10
003a94fc  bl       #0x31167c
003a9500  ldr      r3, [r4, sl]
003a9504  add      r7, r7, #0x28
003a9508  movw     sl, #0x13f8
003a950c  strb     r8, [r3]
003a9510  movw     r3, #0x13e4
003a9514  strb     fp, [r4, r3]
003a9518  movw     r3, #0x13fc
003a951c  str      r7, [r4, r3]
003a9520  mov      r0, r7
003a9524  str      r7, [r4, sl]
003a9528  mov      r1, #0x10
003a952c  bl       #0x31167c
003a9530  ldr      r3, [r4, sl]
003a9534  add      r7, r4, #0x1400
003a9538  movw     sl, #0x1410
003a953c  strb     r8, [r3]
003a9540  movw     r3, #0x1414
003a9544  str      r7, [r4, r3]
003a9548  mov      r0, r7
003a954c  str      r7, [r4, sl]
003a9550  mov      r1, #0x10
003a9554  bl       #0x31167c
003a9558  ldr      r3, [r4, sl]
003a955c  add      r7, r7, #0x18
003a9560  movw     sl, #0x1428
003a9564  strb     r8, [r3]
003a9568  movw     r3, #0x142c
003a956c  str      r7, [r4, r3]
003a9570  mov      r0, r7
003a9574  str      r7, [r4, sl]
003a9578  mov      r1, #0x10
003a957c  bl       #0x31167c
003a9580  ldr      r2, [r4, sl]
003a9584  mov      r3, #0
003a9588  mov      r1, #0xbf000000
003a958c  strb     r8, [r2]
003a9590  movw     r2, #0x14a8
003a9594  strb     r6, [r4, r2]
003a9598  movw     r2, #0x1430
003a959c  strb     fp, [r4, r2]
003a95a0  movw     r2, #0x1434
003a95a4  str      r8, [r4, r2]
003a95a8  movw     r2, #0x1438
003a95ac  str      r8, [r4, r2]
003a95b0  movw     r2, #0x1448
003a95b4  strb     fp, [r4, r2]
003a95b8  movw     r2, #0x1449
003a95bc  strb     r8, [r4, r2]
003a95c0  movw     r2, #0x144c
003a95c4  str      r8, [r4, r2]
003a95c8  movw     r2, #0x1450
003a95cc  str      r3, [r4, r2]
003a95d0  movw     r2, #0x1454
003a95d4  str      r3, [r4, r2]
003a95d8  movw     r2, #0x1458
003a95dc  str      r3, [r4, r2]
003a95e0  movw     r2, #0x145c
003a95e4  str      r3, [r4, r2]
003a95e8  movw     r2, #0x1460
003a95ec  str      r3, [r4, r2]
003a95f0  movw     r2, #0x1464
003a95f4  str      r3, [r4, r2]
003a95f8  movw     r2, #0x1468
003a95fc  str      r3, [r4, r2]
003a9600  movw     r2, #0x146c
003a9604  str      r3, [r4, r2]
003a9608  movw     r2, #0x1470
003a960c  str      r3, [r4, r2]
003a9610  movw     r2, #0x1474
003a9614  str      r3, [r4, r2]
003a9618  movw     r2, #0x1478
003a961c  str      r3, [r4, r2]
003a9620  movw     r2, #0x147c
003a9624  str      r3, [r4, r2]
003a9628  mov      r2, #0x1480
003a962c  strb     r8, [r4, r2]
003a9630  movw     r2, #0x1481
003a9634  strb     r8, [r4, r2]
003a9638  movw     r2, #0x1484
003a963c  str      r8, [r4, r2]
003a9640  movw     r2, #0x1488
003a9644  str      r8, [r4, r2]
003a9648  movw     r2, #0x148c
003a964c  str      r8, [r4, r2]
003a9650  movw     r2, #0x1490
003a9654  str      r8, [r4, r2]
003a9658  movw     r2, #0x1494
003a965c  str      r8, [r4, r2]
003a9660  movw     r2, #0x1498
003a9664  str      r6, [r4, r2]
003a9668  movw     r2, #0x149c
003a966c  str      r8, [r4, r2]
003a9670  movw     r2, #0x14a0
003a9674  str      r8, [r4, r2]
003a9678  movw     r2, #0x14a4
003a967c  str      r8, [r4, r2]
003a9680  movw     r2, #0x14aa
003a9684  strh     r8, [r4, r2]
003a9688  movw     r2, #0x14ac
003a968c  strb     r8, [r4, r2]
003a9690  movw     r2, #0x14d8
003a9694  str      r3, [r4, r2]
003a9698  add      r1, r1, #0x800000
003a969c  movw     r2, #0x14fc
003a96a0  str      r1, [r4, r2]
003a96a4  movw     r2, #0x1504
003a96a8  str      r6, [r4, r2]
003a96ac  movw     r2, #0x14ad
003a96b0  strb     r8, [r4, r2]
003a96b4  movw     r2, #0x14b0
003a96b8  str      r3, [r4, r2]
003a96bc  movw     r2, #0x14b4
003a96c0  str      r3, [r4, r2]
003a96c4  movw     r2, #0x14b8
003a96c8  str      r3, [r4, r2]
003a96cc  movw     r2, #0x14bc
003a96d0  str      r3, [r4, r2]
003a96d4  mov      r2, #0x14c0
003a96d8  str      r3, [r4, r2]
003a96dc  movw     r2, #0x14c4
003a96e0  str      r3, [r4, r2]
003a96e4  movw     r3, #0x14c8
003a96e8  strb     r8, [r4, r3]
003a96ec  movw     r3, #0x14ca
003a96f0  strh     r6, [r4, r3]
003a96f4  movw     r3, #0x14cc
003a96f8  str      r8, [r4, r3]
003a96fc  movw     r3, #0x14d0
003a9700  strh     r8, [r4, r3]
003a9704  movw     r3, #0x14d4
003a9708  str      r8, [r4, r3]
003a970c  movw     r3, #0x14dc
003a9710  strb     r8, [r4, r3]
003a9714  movw     r3, #0x14e4
003a9718  strb     r8, [r4, r3]
003a971c  movw     r3, #0x14e5
003a9720  strb     r8, [r4, r3]
003a9724  movw     r3, #0x14e8
003a9728  str      r8, [r4, r3]
003a972c  movw     r3, #0x14ec
003a9730  str      r8, [r4, r3]
003a9734  add      r7, r4, #0x1500
003a9738  movw     r3, #0x14f0
003a973c  add      r0, r4, #0x1a40
003a9740  strb     r8, [r4, r3]
003a9744  add      r0, r0, #8
003a9748  mov      r3, #0x1500
003a974c  add      r7, r7, #8
003a9750  str      r6, [r4, r3]
003a9754  str      r0, [sp, #0x10]
003a9758  mov      r0, r7
003a975c  bl       #0x3a6a24
003a9760  ldr      r0, [sp, #0x10]
003a9764  bl       #0x3a6a24
003a9768  add      r0, r4, #0x304
003a976c  mov      r1, r4
003a9770  strb     fp, [r4, #0x28]
003a9774  bl       #0x4a191c
003a9778  strb     fp, [r4, #0x1c4]
003a977c  strb     fp, [r4, #0x85]
003a9780  mov      r0, #0x10
003a9784  mov      r1, r8
003a9788  bl       #0x310570
003a978c  ldr      r3, [pc, #0x13c]
003a9790  ldr      ip, [sp, #0xc]
003a9794  mov      r6, r0
003a9798  ldr      r3, [sb, r3]
003a979c  cmp      ip, r8
003a97a0  strb     r8, [r6, #0xa]
003a97a4  add      r3, r3, #8
003a97a8  str      r8, [r0, #0xc]
003a97ac  stm      r0, {r3, ip}
003a97b0  strb     r8, [r6, #8]
003a97b4  strb     r8, [r6, #9]
003a97b8  beq      #0x3a986c
003a97bc  mov      r0, ip
003a97c0  mov      r1, r6
003a97c4  bl       #0x404e10
003a97c8  ldr      r3, [r4, #0x378]
003a97cc  ldr      r0, [sp, #0x20]
003a97d0  mov      r1, r4
003a97d4  str      r4, [r3, #0xc]
003a97d8  bl       #0x3db480
003a97dc  ldr      r0, [sp, #0x1c]
003a97e0  mov      r1, r4
003a97e4  bl       #0x3cb7c0
003a97e8  ldr      r0, [sp, #0x14]
003a97ec  mov      r1, r4
003a97f0  bl       #0x3c9890
003a97f4  mov      r0, r5
003a97f8  mov      r1, r4
003a97fc  bl       #0x3c1600
003a9800  ldr      r0, [sp, #0x18]
003a9804  mov      r1, r4
003a9808  bl       #0x3dec0c
003a980c  mov      r6, #0
003a9810  str      r4, [r4, #0x380]
003a9814  mov      r1, r6
003a9818  mov      r0, r5
003a981c  add      r6, r6, #1
003a9820  bl       #0x3c7318
003a9824  cmp      r6, #0x14
003a9828  bne      #0x3a9814
003a982c  mov      r1, #0
003a9830  movw     r2, #0x14e0
003a9834  str      r1, [r4, r2]
003a9838  mvn      r3, #0
003a983c  movw     r2, #0x14f4
003a9840  str      r3, [r4, r2]
003a9844  str      r7, [r4, #0x100]
003a9848  ldr      sl, [sp, #0x10]
003a984c  movw     r2, #0x14f8
003a9850  mov      r0, r4
003a9854  str      sl, [r4, #0x104]
003a9858  str      r3, [r4, r2]
003a985c  mov      r3, #1
003a9860  strb     r3, [r4, #0xf8]
003a9864  add      sp, sp, #0x3c
003a9868  pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a986c  ldr      r3, [pc, #0x60]
003a9870  ldr      r3, [sb, r3]
003a9874  ldr      r3, [r3]
003a9878  cmp      r3, #2
003a987c  streq    ip, [r4, #0x374]
003a9880  beq      #0x3a97bc
003a9884  cmp      r3, #1
003a9888  bne      #0x3a97bc
003a988c  ldr      r0, [pc, #0x44]
003a9890  ldr      r1, [pc, #0x44]
003a9894  ldr      r2, [pc, #0x44]
003a9898  ldr      r0, [sb, r0]
003a989c  ldr      r3, [pc, #0x40]
003a98a0  mov      lr, #0x44
003a98a4  add      r1, pc, r1
003a98a8  add      r0, r0, #0xa8
003a98ac  add      r2, pc, r2
003a98b0  add      r3, pc, r3
003a98b4  str      ip, [sp, #0xc]
003a98b8  str      lr, [sp]
003a98bc  bl       #0x30e004
003a98c0  ldr      ip, [sp, #0xc]
003a98c4  b        #0x3a97bc
003a98c8  subseq   fp, lr, r8, asr #13
003a98cc  andeq    r2, r0, r8, lsl #28
003a98d0  andeq    r2, r0, r4, lsr #21
003a98d4  andeq    r3, r0, r0, asr #19
003a98d8  andeq    r1, r0, r0, asr #19
003a98dc  subseq   r4, r1, r4, lsr fp
003a98e0  subseq   sb, r1, r4, lsl ip
003a98e4  subseq   sb, r1, r0, lsr #24

_ZN9CharacterC1EN10ObjectBase6GO_IDSE @003aa1b4, bytes1448, sha256=85471dbc43082bce2a8f0fd429f0764fb55948ce47c3396dfca28933d701e6e6
003aa1b4  push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003aa1b8  add      ip, r0, #0x374
003aa1bc  sub      sp, sp, #0x3c
003aa1c0  mov      r4, r0
003aa1c4  str      ip, [sp, #0xc]
003aa1c8  bl       #0x38c398
003aa1cc  ldr      ip, [sp, #0xc]
003aa1d0  add      r5, r4, #0x4f0
003aa1d4  add      r5, r5, #0xc
003aa1d8  mov      r0, ip
003aa1dc  bl       #0x404db8
003aa1e0  add      r0, r4, #0x3b4
003aa1e4  str      r0, [sp, #0x20]
003aa1e8  add      r0, r4, #0x37c
003aa1ec  bl       #0x3ff330
003aa1f0  add      r2, r4, #0x490
003aa1f4  add      r1, r4, #0x3c8
003aa1f8  add      r2, r2, #0xc
003aa1fc  ldr      r0, [sp, #0x20]
003aa200  str      r1, [sp, #0x1c]
003aa204  str      r2, [sp, #0x14]
003aa208  bl       #0x3dbb0c
003aa20c  ldr      r0, [sp, #0x1c]
003aa210  bl       #0x3cebf0
003aa214  ldr      r0, [sp, #0x14]
003aa218  bl       #0x3c8ff4
003aa21c  add      r3, r4, #0x560
003aa220  mov      r0, r5
003aa224  str      r3, [sp, #0x18]
003aa228  ldr      sb, [pc, #0x50c]
003aa22c  bl       #0x3c1b58
003aa230  ldr      r0, [sp, #0x18]
003aa234  bl       #0x3df084
003aa238  ldr      lr, [pc, #0x500]
003aa23c  add      sb, pc, sb
003aa240  mov      r8, #0
003aa244  ldr      lr, [sb, lr]
003aa248  mov      fp, #1
003aa24c  mvn      r6, #0
003aa250  add      sl, lr, #0x324
003aa254  str      sl, [sp, #0x34]
003aa258  add      sl, lr, #0x180
003aa25c  str      sl, [sp, #0x10]
003aa260  add      sl, lr, #0x1f4
003aa264  str      sl, [sp, #0x24]
003aa268  add      sl, lr, #0x220
003aa26c  str      sl, [sp, #0x28]
003aa270  add      sl, lr, #0x230
003aa274  str      sl, [sp, #0x2c]
003aa278  add      r0, lr, #8
003aa27c  add      r1, lr, #0x15c
003aa280  add      r2, lr, #0x168
003aa284  add      sl, lr, #0x304
003aa288  str      sl, [sp, #0x30]
003aa28c  stm      r4, {r0, r1}
003aa290  str      r2, [r4, #0x24]
003aa294  ldr      r0, [sp, #0x10]
003aa298  add      lr, lr, #0x314
003aa29c  add      r7, r4, #0x1380
003aa2a0  str      r0, [r4, #0x374]
003aa2a4  ldr      r1, [sp, #0x24]
003aa2a8  add      r3, r7, #0x18
003aa2ac  movw     sl, #0x13a8
003aa2b0  str      r1, [r4, #0x37c]
003aa2b4  ldr      r2, [sp, #0x28]
003aa2b8  add      r7, r7, #0x30
003aa2bc  str      r2, [r4, #0x3b4]
003aa2c0  ldr      r0, [sp, #0x2c]
003aa2c4  str      r0, [r4, #0x3c8]
003aa2c8  ldr      r1, [sp, #0x30]
003aa2cc  str      lr, [r4, #0x4fc]
003aa2d0  mov      r0, r3
003aa2d4  str      r1, [r4, #0x49c]
003aa2d8  ldr      r2, [sp, #0x34]
003aa2dc  mov      r1, #0x10
003aa2e0  str      r2, [r4, #0x560]
003aa2e4  movw     r2, #0x1394
003aa2e8  strb     r8, [r4, r2]
003aa2ec  movw     r2, #0x1395
003aa2f0  strb     r8, [r4, r2]
003aa2f4  movw     r2, #0x1396
003aa2f8  strb     fp, [r4, r2]
003aa2fc  movw     r2, #0x1397
003aa300  strb     r6, [r4, r2]
003aa304  movw     r2, #0x13ac
003aa308  str      r3, [r4, r2]
003aa30c  str      r3, [r4, sl]
003aa310  bl       #0x31167c
003aa314  ldr      r3, [r4, sl]
003aa318  mov      sl, #0x13c0
003aa31c  mov      r0, r7
003aa320  strb     r8, [r3]
003aa324  movw     r3, #0x13c4
003aa328  str      r7, [r4, r3]
003aa32c  mov      r1, #0x10
003aa330  str      r7, [r4, sl]
003aa334  bl       #0x31167c
003aa338  ldr      r2, [r4, sl]
003aa33c  add      r7, r4, sl
003aa340  add      r3, r7, #0xc
003aa344  strb     r8, [r2]
003aa348  movw     r2, #0x13c8
003aa34c  strh     r6, [r4, r2]
003aa350  movw     r2, #0x13ca
003aa354  strh     r6, [r4, r2]
003aa358  movw     sl, #0x13dc
003aa35c  movw     r2, #0x13e0
003aa360  str      r3, [r4, r2]
003aa364  mov      r0, r3
003aa368  str      r3, [r4, sl]
003aa36c  mov      r1, #0x10
003aa370  bl       #0x31167c
003aa374  ldr      r3, [r4, sl]
003aa378  add      r7, r7, #0x28
003aa37c  movw     sl, #0x13f8
003aa380  strb     r8, [r3]
003aa384  movw     r3, #0x13e4
003aa388  strb     fp, [r4, r3]
003aa38c  movw     r3, #0x13fc
003aa390  str      r7, [r4, r3]
003aa394  mov      r0, r7
003aa398  str      r7, [r4, sl]
003aa39c  mov      r1, #0x10
003aa3a0  bl       #0x31167c
003aa3a4  ldr      r3, [r4, sl]
003aa3a8  add      r7, r4, #0x1400
003aa3ac  movw     sl, #0x1410
003aa3b0  strb     r8, [r3]
003aa3b4  movw     r3, #0x1414
003aa3b8  str      r7, [r4, r3]
003aa3bc  mov      r0, r7
003aa3c0  str      r7, [r4, sl]
003aa3c4  mov      r1, #0x10
003aa3c8  bl       #0x31167c
003aa3cc  ldr      r3, [r4, sl]
003aa3d0  add      r7, r7, #0x18
003aa3d4  movw     sl, #0x1428
003aa3d8  strb     r8, [r3]
003aa3dc  movw     r3, #0x142c
003aa3e0  str      r7, [r4, r3]
003aa3e4  mov      r0, r7
003aa3e8  str      r7, [r4, sl]
003aa3ec  mov      r1, #0x10
003aa3f0  bl       #0x31167c
003aa3f4  ldr      r2, [r4, sl]
003aa3f8  mov      r3, #0
003aa3fc  mov      r1, #0xbf000000
003aa400  strb     r8, [r2]
003aa404  movw     r2, #0x14a8
003aa408  strb     r6, [r4, r2]
003aa40c  movw     r2, #0x1430
003aa410  strb     fp, [r4, r2]
003aa414  movw     r2, #0x1434
003aa418  str      r8, [r4, r2]
003aa41c  movw     r2, #0x1438
003aa420  str      r8, [r4, r2]
003aa424  movw     r2, #0x1448
003aa428  strb     fp, [r4, r2]
003aa42c  movw     r2, #0x1449
003aa430  strb     r8, [r4, r2]
003aa434  movw     r2, #0x144c
003aa438  str      r8, [r4, r2]
003aa43c  movw     r2, #0x1450
003aa440  str      r3, [r4, r2]
003aa444  movw     r2, #0x1454
003aa448  str      r3, [r4, r2]
003aa44c  movw     r2, #0x1458
003aa450  str      r3, [r4, r2]
003aa454  movw     r2, #0x145c
003aa458  str      r3, [r4, r2]
003aa45c  movw     r2, #0x1460
003aa460  str      r3, [r4, r2]
003aa464  movw     r2, #0x1464
003aa468  str      r3, [r4, r2]
003aa46c  movw     r2, #0x1468
003aa470  str      r3, [r4, r2]
003aa474  movw     r2, #0x146c
003aa478  str      r3, [r4, r2]
003aa47c  movw     r2, #0x1470
003aa480  str      r3, [r4, r2]
003aa484  movw     r2, #0x1474
003aa488  str      r3, [r4, r2]
003aa48c  movw     r2, #0x1478
003aa490  str      r3, [r4, r2]
003aa494  movw     r2, #0x147c
003aa498  str      r3, [r4, r2]
003aa49c  mov      r2, #0x1480
003aa4a0  strb     r8, [r4, r2]
003aa4a4  movw     r2, #0x1481
003aa4a8  strb     r8, [r4, r2]
003aa4ac  movw     r2, #0x1484
003aa4b0  str      r8, [r4, r2]
003aa4b4  movw     r2, #0x1488
003aa4b8  str      r8, [r4, r2]
003aa4bc  movw     r2, #0x148c
003aa4c0  str      r8, [r4, r2]
003aa4c4  movw     r2, #0x1490
003aa4c8  str      r8, [r4, r2]
003aa4cc  movw     r2, #0x1494
003aa4d0  str      r8, [r4, r2]
003aa4d4  movw     r2, #0x1498
003aa4d8  str      r6, [r4, r2]
003aa4dc  movw     r2, #0x149c
003aa4e0  str      r8, [r4, r2]
003aa4e4  movw     r2, #0x14a0
003aa4e8  str      r8, [r4, r2]
003aa4ec  movw     r2, #0x14a4
003aa4f0  str      r8, [r4, r2]
003aa4f4  movw     r2, #0x14aa
003aa4f8  strh     r8, [r4, r2]
003aa4fc  movw     r2, #0x14ac
003aa500  strb     r8, [r4, r2]
003aa504  movw     r2, #0x14d8
003aa508  str      r3, [r4, r2]
003aa50c  add      r1, r1, #0x800000
003aa510  movw     r2, #0x14fc
003aa514  str      r1, [r4, r2]
003aa518  movw     r2, #0x1504
003aa51c  str      r6, [r4, r2]
003aa520  movw     r2, #0x14ad
003aa524  strb     r8, [r4, r2]
003aa528  movw     r2, #0x14b0
003aa52c  str      r3, [r4, r2]
003aa530  movw     r2, #0x14b4
003aa534  str      r3, [r4, r2]
003aa538  movw     r2, #0x14b8
003aa53c  str      r3, [r4, r2]
003aa540  movw     r2, #0x14bc
003aa544  str      r3, [r4, r2]
003aa548  mov      r2, #0x14c0
003aa54c  str      r3, [r4, r2]
003aa550  movw     r2, #0x14c4
003aa554  str      r3, [r4, r2]
003aa558  movw     r3, #0x14c8
003aa55c  strb     r8, [r4, r3]
003aa560  movw     r3, #0x14ca
003aa564  strh     r6, [r4, r3]
003aa568  movw     r3, #0x14cc
003aa56c  str      r8, [r4, r3]
003aa570  movw     r3, #0x14d0
003aa574  strh     r8, [r4, r3]
003aa578  movw     r3, #0x14d4
003aa57c  str      r8, [r4, r3]
003aa580  movw     r3, #0x14dc
003aa584  strb     r8, [r4, r3]
003aa588  movw     r3, #0x14e4
003aa58c  strb     r8, [r4, r3]
003aa590  movw     r3, #0x14e5
003aa594  strb     r8, [r4, r3]
003aa598  movw     r3, #0x14e8
003aa59c  str      r8, [r4, r3]
003aa5a0  movw     r3, #0x14ec
003aa5a4  str      r8, [r4, r3]
003aa5a8  add      r7, r4, #0x1500
003aa5ac  movw     r3, #0x14f0
003aa5b0  add      r0, r4, #0x1a40
003aa5b4  strb     r8, [r4, r3]
003aa5b8  add      r0, r0, #8
003aa5bc  mov      r3, #0x1500
003aa5c0  add      r7, r7, #8
003aa5c4  str      r6, [r4, r3]
003aa5c8  str      r0, [sp, #0x10]
003aa5cc  mov      r0, r7
003aa5d0  bl       #0x3a6a24
003aa5d4  ldr      r0, [sp, #0x10]
003aa5d8  bl       #0x3a6a24
003aa5dc  add      r0, r4, #0x304
003aa5e0  mov      r1, r4
003aa5e4  strb     fp, [r4, #0x28]
003aa5e8  bl       #0x4a191c
003aa5ec  strb     fp, [r4, #0x1c4]
003aa5f0  strb     fp, [r4, #0x85]
003aa5f4  mov      r0, #0x10
003aa5f8  mov      r1, r8
003aa5fc  bl       #0x310570
003aa600  ldr      r3, [pc, #0x13c]
003aa604  ldr      ip, [sp, #0xc]
003aa608  mov      r6, r0
003aa60c  ldr      r3, [sb, r3]
003aa610  cmp      ip, r8
003aa614  strb     r8, [r6, #0xa]
003aa618  add      r3, r3, #8
003aa61c  str      r8, [r0, #0xc]
003aa620  stm      r0, {r3, ip}
003aa624  strb     r8, [r6, #8]
003aa628  strb     r8, [r6, #9]
003aa62c  beq      #0x3aa6e0
003aa630  mov      r0, ip
003aa634  mov      r1, r6
003aa638  bl       #0x404e10
003aa63c  ldr      r3, [r4, #0x378]
003aa640  ldr      r0, [sp, #0x20]
003aa644  mov      r1, r4
003aa648  str      r4, [r3, #0xc]
003aa64c  bl       #0x3db480
003aa650  ldr      r0, [sp, #0x1c]
003aa654  mov      r1, r4
003aa658  bl       #0x3cb7c0
003aa65c  ldr      r0, [sp, #0x14]
003aa660  mov      r1, r4
003aa664  bl       #0x3c9890
003aa668  mov      r0, r5
003aa66c  mov      r1, r4
003aa670  bl       #0x3c1600
003aa674  ldr      r0, [sp, #0x18]
003aa678  mov      r1, r4
003aa67c  bl       #0x3dec0c
003aa680  mov      r6, #0
003aa684  str      r4, [r4, #0x380]
003aa688  mov      r1, r6
003aa68c  mov      r0, r5
003aa690  add      r6, r6, #1
003aa694  bl       #0x3c7318
003aa698  cmp      r6, #0x14
003aa69c  bne      #0x3aa688
003aa6a0  mov      r1, #0
003aa6a4  movw     r2, #0x14e0
003aa6a8  str      r1, [r4, r2]
003aa6ac  mvn      r3, #0
003aa6b0  movw     r2, #0x14f4
003aa6b4  str      r3, [r4, r2]
003aa6b8  str      r7, [r4, #0x100]
003aa6bc  ldr      sl, [sp, #0x10]
003aa6c0  movw     r2, #0x14f8
003aa6c4  mov      r0, r4
003aa6c8  str      sl, [r4, #0x104]
003aa6cc  str      r3, [r4, r2]
003aa6d0  mov      r3, #1
003aa6d4  strb     r3, [r4, #0xf8]
003aa6d8  add      sp, sp, #0x3c
003aa6dc  pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aa6e0  ldr      r3, [pc, #0x60]
003aa6e4  ldr      r3, [sb, r3]
003aa6e8  ldr      r3, [r3]
003aa6ec  cmp      r3, #2
003aa6f0  streq    ip, [r4, #0x374]
003aa6f4  beq      #0x3aa630
003aa6f8  cmp      r3, #1
003aa6fc  bne      #0x3aa630
003aa700  ldr      r0, [pc, #0x44]
003aa704  ldr      r1, [pc, #0x44]
003aa708  ldr      r2, [pc, #0x44]
003aa70c  ldr      r0, [sb, r0]
003aa710  ldr      r3, [pc, #0x40]
003aa714  mov      lr, #0x44
003aa718  add      r1, pc, r1
003aa71c  add      r0, r0, #0xa8
003aa720  add      r2, pc, r2
003aa724  add      r3, pc, r3
003aa728  str      ip, [sp, #0xc]
003aa72c  str      lr, [sp]
003aa730  bl       #0x30e004
003aa734  ldr      ip, [sp, #0xc]
003aa738  b        #0x3aa630
003aa73c  subseq   sl, lr, r4, asr r8
003aa740  andeq    r2, r0, r8, lsl #28
003aa744  andeq    r2, r0, r4, lsr #21
003aa748  andeq    r3, r0, r0, asr #19
003aa74c  andeq    r1, r0, r0, asr #19
003aa750  subseq   r3, r1, r0, asr #25
003aa754  subseq   r8, r1, r0, lsr #27
003aa758  subseq   r8, r1, ip, lsr #27

_ZN6CharAIC2Ev @003cebf0, bytes352, sha256=90203efdaeb5c4e378736be80a95ab30a278cf81e2ccbe097e43a20f34db4009
003cebf0  ldr      r3, [pc, #0x14c]
003cebf4  ldr      r2, [pc, #0x14c]
003cebf8  push     {r4, r5, lr}
003cebfc  add      r3, pc, r3
003cec00  ldr      r2, [r3, r2]
003cec04  mov      r4, r0
003cec08  mov      r1, #0
003cec0c  add      r2, r2, #8
003cec10  str      r2, [r4]
003cec14  ldr      r2, [pc, #0x130]
003cec18  mov      r0, #1
003cec1c  mvn      ip, #0
003cec20  mov      r5, r4
003cec24  strb     r0, [r4, #0x55]
003cec28  str      r1, [r4, #8]
003cec2c  str      r1, [r4, #0xc]
003cec30  strb     r1, [r4, #0x18]
003cec34  str      r1, [r4, #0x1c]
003cec38  str      r1, [r4, #0x20]
003cec3c  strb     r1, [r4, #0x24]
003cec40  str      r1, [r4, #0x28]
003cec44  strb     r1, [r4, #0x2c]
003cec48  str      r1, [r4, #0x30]
003cec4c  str      r1, [r4, #0x34]
003cec50  str      r1, [r4, #0x3c]
003cec54  str      r1, [r4, #0x40]
003cec58  str      r1, [r4, #0x44]
003cec5c  strb     r1, [r4, #0x49]
003cec60  strb     r0, [r4, #0x4a]
003cec64  strb     r0, [r4, #0x4b]
003cec68  strb     r1, [r4, #0x4c]
003cec6c  strb     r0, [r4, #0x4d]
003cec70  str      r1, [r4, #0x50]
003cec74  strb     r0, [r4, #0x54]
003cec78  str      r1, [r4, #0x58]
003cec7c  mov      r0, r4
003cec80  str      r1, [r4, #0x60]
003cec84  str      ip, [r4, #0x10]
003cec88  str      ip, [r4, #0x14]
003cec8c  str      ip, [r4, #0x38]
003cec90  strb     r1, [r5, #0x5c]!
003cec94  str      r5, [r4, #0x68]
003cec98  str      r5, [r4, #0x64]
003cec9c  str      r1, [r4, #0x6c]
003ceca0  str      r1, [r4, #0x80]
003ceca4  strb     r1, [r0, #0x7c]!
003ceca8  ldr      r5, [r3, r2]
003cecac  mov      r2, r4
003cecb0  str      r0, [r4, #0x88]
003cecb4  str      r0, [r4, #0x84]
003cecb8  str      r1, [r4, #0x8c]
003cecbc  str      r1, [r4, #0x98]
003cecc0  add      r0, r4, #0xac
003cecc4  strb     r1, [r2, #0x94]!
003cecc8  str      r2, [r4, #0xa0]
003ceccc  str      r0, [r4, #0xb0]
003cecd0  str      ip, [r4, #0xcc]
003cecd4  strb     r1, [r4, #0xd1]
003cecd8  str      r2, [r4, #0x9c]
003cecdc  str      r1, [r4, #0xa4]
003cece0  str      r0, [r4, #0xac]
003cece4  str      r1, [r4, #0xb4]
003cece8  str      r1, [r4, #0xb8]
003cecec  str      r1, [r4, #0xbc]
003cecf0  str      r1, [r4, #0xc0]
003cecf4  str      r1, [r4, #0xc4]
003cecf8  str      r1, [r4, #0xc8]
003cecfc  strb     r1, [r4, #0xd0]
003ced00  ldr      r1, [r5, #0x18]
003ced04  ldr      r2, [r5, #0x10]
003ced08  sub      sp, sp, #0xc
003ced0c  sub      r3, r1, #4
003ced10  cmp      r2, r3
003ced14  str      r4, [sp, #4]
003ced18  beq      #0x3ced38
003ced1c  str      r4, [r2]
003ced20  ldr      r3, [r5, #0x10]
003ced24  add      r3, r3, #4
003ced28  str      r3, [r5, #0x10]
003ced2c  mov      r0, r4
003ced30  add      sp, sp, #0xc
003ced34  pop      {r4, r5, pc}
003ced38  add      r0, sp, #4
003ced3c  bl       #0x3ce810
003ced40  b        #0x3ced2c

_ZN6CharAIC1Ev @003ced50, bytes352, sha256=e4a3df105876c622bb394d7f12bb43ac915bf8ba4927708337bf8ba786323919
003ced50  ldr      r3, [pc, #0x14c]
003ced54  ldr      r2, [pc, #0x14c]
003ced58  push     {r4, r5, lr}
003ced5c  add      r3, pc, r3
003ced60  ldr      r2, [r3, r2]
003ced64  mov      r4, r0
003ced68  mov      r1, #0
003ced6c  add      r2, r2, #8
003ced70  str      r2, [r4]
003ced74  ldr      r2, [pc, #0x130]
003ced78  mov      r0, #1
003ced7c  mvn      ip, #0
003ced80  mov      r5, r4
003ced84  strb     r0, [r4, #0x55]
003ced88  str      r1, [r4, #8]
003ced8c  str      r1, [r4, #0xc]
003ced90  strb     r1, [r4, #0x18]
003ced94  str      r1, [r4, #0x1c]
003ced98  str      r1, [r4, #0x20]
003ced9c  strb     r1, [r4, #0x24]
003ceda0  str      r1, [r4, #0x28]
003ceda4  strb     r1, [r4, #0x2c]
003ceda8  str      r1, [r4, #0x30]
003cedac  str      r1, [r4, #0x34]
003cedb0  str      r1, [r4, #0x3c]
003cedb4  str      r1, [r4, #0x40]
003cedb8  str      r1, [r4, #0x44]
003cedbc  strb     r1, [r4, #0x49]
003cedc0  strb     r0, [r4, #0x4a]
003cedc4  strb     r0, [r4, #0x4b]
003cedc8  strb     r1, [r4, #0x4c]
003cedcc  strb     r0, [r4, #0x4d]
003cedd0  str      r1, [r4, #0x50]
003cedd4  strb     r0, [r4, #0x54]
003cedd8  str      r1, [r4, #0x58]
003ceddc  mov      r0, r4
003cede0  str      r1, [r4, #0x60]
003cede4  str      ip, [r4, #0x10]
003cede8  str      ip, [r4, #0x14]
003cedec  str      ip, [r4, #0x38]
003cedf0  strb     r1, [r5, #0x5c]!
003cedf4  str      r5, [r4, #0x68]
003cedf8  str      r5, [r4, #0x64]
003cedfc  str      r1, [r4, #0x6c]
003cee00  str      r1, [r4, #0x80]
003cee04  strb     r1, [r0, #0x7c]!
003cee08  ldr      r5, [r3, r2]
003cee0c  mov      r2, r4
003cee10  str      r0, [r4, #0x88]
003cee14  str      r0, [r4, #0x84]
003cee18  str      r1, [r4, #0x8c]
003cee1c  str      r1, [r4, #0x98]
003cee20  add      r0, r4, #0xac
003cee24  strb     r1, [r2, #0x94]!
003cee28  str      r2, [r4, #0xa0]
003cee2c  str      r0, [r4, #0xb0]
003cee30  str      ip, [r4, #0xcc]
003cee34  strb     r1, [r4, #0xd1]
003cee38  str      r2, [r4, #0x9c]
003cee3c  str      r1, [r4, #0xa4]
003cee40  str      r0, [r4, #0xac]
003cee44  str      r1, [r4, #0xb4]
003cee48  str      r1, [r4, #0xb8]
003cee4c  str      r1, [r4, #0xbc]
003cee50  str      r1, [r4, #0xc0]
003cee54  str      r1, [r4, #0xc4]
003cee58  str      r1, [r4, #0xc8]
003cee5c  strb     r1, [r4, #0xd0]
003cee60  ldr      r1, [r5, #0x18]
003cee64  ldr      r2, [r5, #0x10]
003cee68  sub      sp, sp, #0xc
003cee6c  sub      r3, r1, #4
003cee70  cmp      r2, r3
003cee74  str      r4, [sp, #4]
003cee78  beq      #0x3cee98
003cee7c  str      r4, [r2]
003cee80  ldr      r3, [r5, #0x10]
003cee84  add      r3, r3, #4
003cee88  str      r3, [r5, #0x10]
003cee8c  mov      r0, r4
003cee90  add      sp, sp, #0xc
003cee94  pop      {r4, r5, pc}
003cee98  add      r0, sp, #4
003cee9c  bl       #0x3ce810
003ceea0  b        #0x3cee8c
003ceea4  subseq   r5, ip, r4, lsr sp
003ceea8  andeq    r4, r0, ip, asr #12
003ceeac  andeq    r4, r0, ip, lsr #19

_ZN6CharAI6UpdateEv @003cfbf4, bytes372, sha256=ac1373709c4ba656b3deba4c9e5f75b3677a7f526e009e748acc2880f884dcfe
003cfbf4  push     {r4, r5, r6, lr}
003cfbf8  mov      r5, r0
003cfbfc  ldr      r0, [pc, #0x140]
003cfc00  ldr      r4, [pc, #0x140]
003cfc04  add      r0, pc, r0
003cfc08  bl       #0x3136b4
003cfc0c  ldrb     r3, [r5, #0x18]
003cfc10  add      r4, pc, r4
003cfc14  cmp      r3, #0
003cfc18  bne      #0x3cfc44
003cfc1c  ldr      r6, [r5, #4]
003cfc20  ldr      r3, [r6, #0x378]
003cfc24  ldrb     r2, [r3, #9]
003cfc28  cmp      r2, #0
003cfc2c  bne      #0x3cfc60
003cfc30  ldr      r2, [pc, #0x114]
003cfc34  ldr      r2, [r4, r2]
003cfc38  ldrb     r2, [r2]
003cfc3c  cmp      r2, #0
003cfc40  beq      #0x3cfc54
003cfc44  ldr      r0, [pc, #0x104]
003cfc48  add      r0, pc, r0
003cfc4c  pop      {r4, r5, r6, lr}
003cfc50  b        #0x3136b8
003cfc54  ldrb     r3, [r3, #8]
003cfc58  cmp      r3, #0
003cfc5c  bne      #0x3cfc44
003cfc60  ldr      r3, [r6, #0x520]
003cfc64  tst      r3, #0x100
003cfc68  beq      #0x3cfc44
003cfc6c  ldr      r3, [r6]
003cfc70  mov      r0, r6
003cfc74  mov      lr, pc
003cfc78  ldr      pc, [r3, #0xc4]
003cfc7c  cmp      r0, #0
003cfc80  beq      #0x3cfc90
003cfc84  ldrb     r3, [r6, #0x2ee]
003cfc88  cmp      r3, #0
003cfc8c  bne      #0x3cfd34
003cfc90  ldr      r6, [pc, #0xbc]
003cfc94  ldr      r3, [r5, #4]
003cfc98  mov      r2, #1
003cfc9c  add      r6, pc, r6
003cfca0  ldr      r4, [pc, #0xb0]
003cfca4  strb     r2, [r3, #0x88]
003cfca8  mov      r0, r6
003cfcac  bl       #0x3136b4
003cfcb0  mov      r0, r5
003cfcb4  bl       #0x3cb908
003cfcb8  add      r4, pc, r4
003cfcbc  mov      r0, r6
003cfcc0  ldr      r6, [pc, #0x94]
003cfcc4  bl       #0x3136b8
003cfcc8  mov      r0, r4
003cfccc  bl       #0x3136b4
003cfcd0  mov      r0, r5
003cfcd4  bl       #0x3cc5a4
003cfcd8  add      r6, pc, r6
003cfcdc  mov      r0, r4
003cfce0  ldr      r4, [pc, #0x78]
003cfce4  bl       #0x3136b8
003cfce8  mov      r0, r6
003cfcec  bl       #0x3136b4
003cfcf0  mov      r0, r5
003cfcf4  bl       #0x3cf3f0
003cfcf8  add      r4, pc, r4
003cfcfc  mov      r0, r6
003cfd00  bl       #0x3136b8
003cfd04  mov      r0, r4
003cfd08  bl       #0x3136b4
003cfd0c  mov      r0, r5
003cfd10  ldr      r3, [r5]
003cfd14  mov      lr, pc
003cfd18  ldr      pc, [r3, #0x18]
003cfd1c  mov      r0, r4
003cfd20  bl       #0x3136b8
003cfd24  ldr      r0, [pc, #0x38]
003cfd28  add      r0, pc, r0
003cfd2c  pop      {r4, r5, r6, lr}
003cfd30  b        #0x3136b8
003cfd34  ldrb     r3, [r6, #0x2f0]
003cfd38  cmp      r3, #0
003cfd3c  beq      #0x3cfc44
003cfd40  b        #0x3cfc90
003cfd44  strdeq   r5, r6, [pc], #-0x7c
003cfd48  subseq   r4, ip, r0, lsl #29
003cfd4c  andeq    r3, r0, r0, asr r6
003cfd50  strheq   r5, [pc], #-0x78
003cfd54  subeq    r5, pc, ip, ror r7
003cfd58  subeq    r5, pc, r8, ror r7
003cfd5c  subeq    r5, pc, r0, ror r7
003cfd60  subeq    r5, pc, r8, ror #14
003cfd64  ldrdeq   r5, r6, [pc], #-0x68

_ZN6CharAI8OnUpdateEv @003d1050, bytes364, sha256=b7f3702aa3d33a84667fd56c9fd44b1694acd7a557b9bd301dcb978a51d8ebde
003d1050  push     {r4, lr}
003d1054  ldr      r3, [r0, #0x1c]
003d1058  sub      sp, sp, #0x10
003d105c  mov      r4, r0
003d1060  cmp      r3, #0
003d1064  beq      #0x3d1078
003d1068  mov      r0, r3
003d106c  ldr      r3, [r3]
003d1070  mov      lr, pc
003d1074  ldr      pc, [r3, #0x18]
003d1078  ldr      r0, [r4, #4]
003d107c  mov      r1, #0
003d1080  add      r0, r0, #0x4f0
003d1084  add      r0, r0, #0xc
003d1088  bl       #0x3c0260
003d108c  cmp      r0, #0
003d1090  beq      #0x3d10c8
003d1094  ldr      r3, [r4, #4]
003d1098  ldr      r2, [r3, #0x408]
003d109c  cmp      r2, #0
003d10a0  beq      #0x3d10f4
003d10a4  ldr      r4, [r3, #0x2d8]
003d10a8  mov      r0, r3
003d10ac  bl       #0x38c600
003d10b0  cmp      r4, #0
003d10b4  beq      #0x3d10c0
003d10b8  mov      r0, r4
003d10bc  bl       #0x4713d0
003d10c0  add      sp, sp, #0x10
003d10c4  pop      {r4, pc}
003d10c8  ldr      r0, [r4, #4]
003d10cc  add      r0, r0, #0x4f0
003d10d0  add      r0, r0, #0xc
003d10d4  bl       #0x3c0230
003d10d8  cmp      r0, #0
003d10dc  ldreq    r3, [r4, #4]
003d10e0  beq      #0x3d10a4
003d10e4  ldr      r3, [r4, #4]
003d10e8  ldr      r2, [r3, #0x408]
003d10ec  cmp      r2, #0
003d10f0  bne      #0x3d10a4
003d10f4  ldr      r2, [r3, #0x418]
003d10f8  cmp      r2, #0
003d10fc  bne      #0x3d10a4
003d1100  ldrb     r2, [r3, #0x2ee]
003d1104  cmp      r2, #0
003d1108  bne      #0x3d10c0
003d110c  mov      r0, r3
003d1110  ldr      r3, [r3]
003d1114  mov      lr, pc
003d1118  ldr      pc, [r3, #0xc4]
003d111c  cmp      r0, #0
003d1120  beq      #0x3d10c0
003d1124  ldr      r0, [r4, #4]
003d1128  bl       #0x38c790
003d112c  ldr      r3, [r4, #4]
003d1130  mov      r0, r3
003d1134  ldr      r3, [r3]
003d1138  mov      lr, pc
003d113c  ldr      pc, [r3, #0x34]
003d1140  cmp      r0, #0
003d1144  bne      #0x3d10c0
003d1148  ldr      r0, [r4, #4]
003d114c  ldrb     r3, [r0, #0x85]
003d1150  cmp      r3, #0
003d1154  bne      #0x3d10c0
003d1158  add      r1, r0, #0x1440
003d115c  mov      r2, #1
003d1160  add      r1, r1, #0x10
003d1164  bl       #0x393db4
003d1168  ldr      r3, [r4, #4]
003d116c  ldr      r2, [r3, #0x2d8]
003d1170  cmp      r2, #0
003d1174  beq      #0x3d10c0
003d1178  ldr      r0, [r2, #8]
003d117c  cmp      r0, #0
003d1180  beq      #0x3d10c0
003d1184  movw     r2, #0x1450
003d1188  ldr      lr, [r3, r2]
003d118c  movw     r2, #0x1454
003d1190  ldr      ip, [r3, r2]
003d1194  movw     r2, #0x1458
003d1198  ldr      r2, [r3, r2]
003d119c  ldr      r3, [r0]
003d11a0  add      r1, sp, #4
003d11a4  ldr      r3, [r3, #0xa4]
003d11a8  str      lr, [sp, #4]
003d11ac  str      ip, [sp, #8]
003d11b0  str      r2, [sp, #0xc]
003d11b4  blx      r3
003d11b8  b        #0x3d10c0

_ZN11AISExternal8OnUpdateEv @003dce64, bytes64, sha256=47fc8c7829030778675408d02e0b1e3773f59a620bad5d373201c65fb0e61dc6
003dce64  push     {r4, lr}
003dce68  mov      r4, r0
003dce6c  bl       #0x3dc798
003dce70  ldr      r3, [r4, #0xb8]
003dce74  tst      r3, #1
003dce78  beq      #0x3dce8c
003dce7c  ldr      r1, [pc, #0x1c]
003dce80  mov      r0, r4
003dce84  add      r1, pc, r1
003dce88  bl       #0x37c514
003dce8c  mov      r0, r4
003dce90  bl       #0x3d8eb4
003dce94  mov      r0, r4
003dce98  pop      {r4, lr}
003dce9c  b        #0x3d8ea0
003dcea0  subeq    r8, lr, r4, lsl #25

_ZN10AISDefault8OnUpdateEv @003dc798, bytes64, sha256=b4b2accbca71efc2e95e37357589e0c2e722873cac3f1395065000d8561a8913
003dc798  push     {r4, lr}
003dc79c  ldr      r3, [r0, #0xbc]
003dc7a0  mov      r4, r0
003dc7a4  cmp      r3, #0xc7
003dc7a8  bhi      #0x3dc7b0
003dc7ac  pop      {r4, pc}
003dc7b0  ldr      r0, [r0, #0x98]
003dc7b4  mov      r3, #0
003dc7b8  str      r3, [r4, #0xbc]
003dc7bc  add      r0, r0, #0x3c8
003dc7c0  mov      r1, #0x3e8
003dc7c4  bl       #0x3cb748
003dc7c8  ldr      r3, [r4, #0x98]
003dc7cc  ldr      r0, [r3, #0x378]
003dc7d0  pop      {r4, lr}
003dc7d4  b        #0x40559c

_ZN6CharAI15UpdateAllSkillsEv @003d8894, bytes180, sha256=7c3efb393b1c872b2992c91f04214cbf333d09330a5fd26d84e14c1b0fef58c3
003d8894  push     {r4, r5, r6, lr}
003d8898  mov      r5, r0
003d889c  ldr      r0, [r0, #4]
003d88a0  add      r0, r0, #0x4f0
003d88a4  add      r0, r0, #0xc
003d88a8  bl       #0x3c02e8
003d88ac  cmp      r0, #0
003d88b0  beq      #0x3d88b8
003d88b4  pop      {r4, r5, r6, pc}
003d88b8  ldr      r0, [r5, #4]
003d88bc  add      r0, r0, #0x4f0
003d88c0  add      r0, r0, #0xc
003d88c4  bl       #0x3c0334
003d88c8  subs     r4, r0, #0
003d88cc  bne      #0x3d88b4
003d88d0  ldr      r3, [r5, #0xb4]
003d88d4  ldr      r6, [r5, #0xb8]
003d88d8  rsb      r6, r3, r6
003d88dc  asrs     r6, r6, #2
003d88e0  bne      #0x3d88ec
003d88e4  b        #0x3d8908
003d88e8  ldr      r3, [r5, #0xb4]
003d88ec  ldr      r0, [r3, r4, lsl #2]
003d88f0  add      r4, r4, #1
003d88f4  cmp      r0, #0
003d88f8  beq      #0x3d8900
003d88fc  bl       #0x3dabd0
003d8900  cmp      r4, r6
003d8904  bne      #0x3d88e8
003d8908  ldr      r3, [r5, #0xc0]
003d890c  ldr      r6, [r5, #0xc4]
003d8910  rsb      r6, r3, r6
003d8914  asrs     r6, r6, #2
003d8918  beq      #0x3d88b4
003d891c  mov      r4, #0
003d8920  b        #0x3d8928
003d8924  ldr      r3, [r5, #0xc0]
003d8928  ldr      r0, [r3, r4, lsl #2]
003d892c  add      r4, r4, #1
003d8930  cmp      r0, #0
003d8934  beq      #0x3d893c
003d8938  bl       #0x3dabd0
003d893c  cmp      r4, r6
003d8940  bne      #0x3d8924
003d8944  pop      {r4, r5, r6, pc}

_ZN6CharAI12UpdateSkillsEv @003d8a04, bytes148, sha256=82796a85dc2dab42ac52b4442a3c5c52f932e580855257ccff2326db8600f389
003d8a04  push     {r4, r5, r6, lr}
003d8a08  mov      r4, r0
003d8a0c  ldr      r0, [r0, #4]
003d8a10  ldr      r5, [pc, #0x78]
003d8a14  add      r0, r0, #0x4f0
003d8a18  add      r0, r0, #0xc
003d8a1c  bl       #0x3c02e8
003d8a20  cmp      r0, #0
003d8a24  add      r5, pc, r5
003d8a28  beq      #0x3d8a30
003d8a2c  pop      {r4, r5, r6, pc}
003d8a30  ldr      r0, [r4, #4]
003d8a34  add      r0, r0, #0x4f0
003d8a38  add      r0, r0, #0xc
003d8a3c  bl       #0x3c0334
003d8a40  cmp      r0, #0
003d8a44  bne      #0x3d8a2c
003d8a48  ldr      r3, [pc, #0x44]
003d8a4c  mov      r2, r4
003d8a50  ldr      r0, [r4, #4]
003d8a54  ldr      r1, [r5, r3]
003d8a58  bl       #0x3bbe40
003d8a5c  ldr      r0, [r4, #4]
003d8a60  mvn      r1, #0
003d8a64  bl       #0x3bb98c
003d8a68  ldr      r2, [r4, #0xc4]
003d8a6c  ldr      r3, [r4, #0xc0]
003d8a70  rsb      r2, r3, r2
003d8a74  cmp      r0, r2, asr #2
003d8a78  bhs      #0x3d8a2c
003d8a7c  ldr      r0, [r3, r0, lsl #2]
003d8a80  cmp      r0, #0
003d8a84  beq      #0x3d8a2c
003d8a88  pop      {r4, r5, r6, lr}
003d8a8c  b        #0x3dabd0
003d8a90  subseq   ip, fp, ip, rrx
003d8a94  strdeq   r3, r4, [r0], -r8

_ZN13PlayerManager17_ManageCharactersEv @0037280c, bytes5072, sha256=da2fe6a4ee1363f68a3f0a7b23aa851bd48d5bcbcfe4220e925de109a84ad7ce
00373448  add      r0, r4, #0x3d8
0037344c  bl       #0x814f70
00373450  cmp      r0, #0
00373454  beq      #0x3738a8
00373458  ldr      r1, [r4, #0x3f8]
0037345c  cmp      r1, #0
00373460  beq      #0x3739d0
00373464  ldr      r2, [r4, #0x3fc]
00373468  cmp      r2, #0
0037346c  ble      #0x3739d0
00373470  add      sb, sp, #0xc4
00373474  mov      r0, sb
00373478  bl       #0x30e868
0037347c  mov      sl, #0
00373480  ldrsb    r2, [sb, sl]
00373484  mov      r1, sl
00373488  mov      r0, r5
0037348c  cmn      r2, #1
00373490  mvnlt    lr, #0
00373494  strblt   lr, [sb, sl]
00373498  mvnlt    r2, #0
0037349c  add      sl, sl, #1
003734a0  bl       #0x3bbe54
003734a4  cmp      sl, #3
003734a8  bne      #0x373480
003734ac  add      r0, r4, #0x400
003734b0  bl       #0x814f70
003734b4  cmp      r0, #0
003734b8  beq      #0x3738c4
003734bc  movw     r3, #0x14e8
003734c0  ldr      r3, [r5, r3]
003734c4  cmp      r3, #0
003734c8  beq      #0x3734fc
003734cc  ldr      r3, [r3, #0x84]
003734d0  cmp      r3, #0x1e
003734d4  bls      #0x3734fc
003734d8  ldr      r3, [pc, #0x240]
003734dc  ldr      r3, [r8, r3]
003734e0  ldr      r3, [r3]
003734e4  cmp      r3, #2
003734e8  moveq    r3, #0
003734ec  streq    r3, [r3]
003734f0  beq      #0x3734fc
003734f4  cmp      r3, #1
003734f8  beq      #0x373b04
003734fc  ldr      r1, [r4, #0x420]
00373500  cmp      r1, #0
00373504  beq      #0x37351c
00373508  ldr      r2, [r4, #0x424]
0037350c  cmp      r2, #0
00373510  ble      #0x37351c
00373514  add      r0, sp, #0xd4
00373518  bl       #0x30e868
0037351c  mov      sl, #0
00373520  add      r3, sp, #0xd4
00373524  str      r4, [sp, #0x14]
00373528  mov      r1, sl
0037352c  movw     sb, #0x14e8
00373530  mov      r4, r3
00373534  ldr      r3, [r5, sb]
00373538  cmp      r3, #0
0037353c  beq      #0x373568
00373540  ldr      r3, [r3, #0x84]
00373544  cmp      sl, r3
00373548  bhs      #0x373568
0037354c  ldrsb    r2, [r4, sl]
00373550  cmp      r2, #0
00373554  blt      #0x373568
00373558  mov      r1, sl
0037355c  mov      r0, r5
00373560  bl       #0x3bbebc
00373564  mov      r1, #1
00373568  add      sl, sl, #1
0037356c  cmp      sl, #0x1e
00373570  bne      #0x373534
00373574  cmp      r1, #0
00373578  ldr      r4, [sp, #0x14]
0037357c  bne      #0x37396c
