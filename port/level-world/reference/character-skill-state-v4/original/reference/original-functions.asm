
# _ZN7CSSkill6OnBlurEiP9CharacterP16CharStateMachinei
003c434c: push     {r4, r5, r6, r7, r8, lr}
003c4350: ldr      r5, [pc, #0x118]
003c4354: ldr      r6, [pc, #0x118]
003c4358: ldr      r1, [pc, #0x118]
003c435c: add      r5, pc, r5
003c4360: ldr      r3, [r5, r6]
003c4364: ldr      r8, [r5, r1]
003c4368: sub      sp, sp, #0x28
003c436c: ldr      r3, [r3]
003c4370: mov      r0, r8
003c4374: mov      r4, r2
003c4378: str      r3, [sp, #0x24]
003c437c: bl       #0x337888
003c4380: ldr      r1, [pc, #0xf4]
003c4384: add      r7, sp, #0xc
003c4388: add      r2, sp, #8
003c438c: add      r1, pc, r1
003c4390: mov      r0, r7
003c4394: bl       #0x3140ec
003c4398: mov      r1, r7
003c439c: mov      r0, r8
003c43a0: bl       #0x337a88
003c43a4: mov      r0, r7
003c43a8: bl       #0x318254
003c43ac: add      r0, r4, #0x3c8
003c43b0: bl       #0x3d49c4
003c43b4: mov      r0, r4
003c43b8: bl       #0x3938f8
003c43bc: mov      r0, r4
003c43c0: mov      r1, #0x1f
003c43c4: mov      r2, #0
003c43c8: bl       #0x3a4d5c
003c43cc: ldr      r3, [r4, #0x528]
003c43d0: tst      r3, #0x100
003c43d4: bne      #0x3c4414
003c43d8: ldr      r0, [r4, #0x2dc]
003c43dc: cmp      r0, #0
003c43e0: beq      #0x3c43e8
003c43e4: bl       #0x46eb20
003c43e8: mov      r0, r4
003c43ec: bl       #0x3a3064
003c43f0: cmp      r0, #0
003c43f4: bne      #0x3c4440
003c43f8: ldr      r3, [r5, r6]
003c43fc: ldr      r2, [sp, #0x24]
003c4400: ldr      r3, [r3]
003c4404: cmp      r2, r3
003c4408: bne      #0x3c446c
003c440c: add      sp, sp, #0x28
003c4410: pop      {r4, r5, r6, r7, r8, pc}
003c4414: mov      ip, #0
003c4418: mov      r2, ip
003c441c: mov      r1, #0xa
003c4420: mov      r3, #0x30
003c4424: add      r0, r4, #0x3b4
003c4428: str      ip, [sp]
003c442c: bl       #0x3dbe24
003c4430: mov      r0, r4
003c4434: bl       #0x3a3064
003c4438: cmp      r0, #0
003c443c: beq      #0x3c43f8
003c4440: mov      r0, r4
003c4444: bl       #0x3a3144
003c4448: cmp      r0, #0
003c444c: bne      #0x3c43f8
003c4450: mov      r0, r4
003c4454: bl       #0x3a3158
003c4458: cmp      r0, #0
003c445c: ldreq    r3, [r4, #0x520]
003c4460: biceq    r3, r3, #0x10000
003c4464: streq    r3, [r4, #0x520]
003c4468: b        #0x3c43f8
003c446c: bl       #0x30e310
003c4470: subseq   r0, sp, r4, lsr r7
003c4474: andeq    r4, r0, ip, lsr #1
003c4478: andeq    r0, r0, r4, lsl #17
003c447c: subseq   r0, r0, r4, asr #21

# _ZN7CSSkill7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c4480: push     {r4, r5, r6, r7, r8, lr}
003c4484: ldr      r5, [pc, #0x128]
003c4488: ldr      r7, [pc, #0x128]
003c448c: ldr      r1, [pc, #0x128]
003c4490: add      r5, pc, r5
003c4494: ldr      r3, [r5, r7]
003c4498: ldr      r8, [r5, r1]
003c449c: sub      sp, sp, #0x20
003c44a0: ldr      r3, [r3]
003c44a4: mov      r0, r8
003c44a8: mov      r4, r2
003c44ac: str      r3, [sp, #0x1c]
003c44b0: bl       #0x337888
003c44b4: ldr      r1, [pc, #0x104]
003c44b8: add      r6, sp, #4
003c44bc: mov      r2, sp
003c44c0: add      r1, pc, r1
003c44c4: mov      r0, r6
003c44c8: bl       #0x3140ec
003c44cc: mov      r1, r6
003c44d0: mov      r0, r8
003c44d4: bl       #0x337a88
003c44d8: mov      r0, r6
003c44dc: bl       #0x318254
003c44e0: ldr      r3, [r4, #0x528]
003c44e4: movw     r2, #0x6341
003c44e8: str      r2, [r4, #0x520]
003c44ec: bic      r3, r3, #0x140
003c44f0: str      r3, [r4, #0x528]
003c44f4: mov      r2, #0
003c44f8: mov      r0, r4
003c44fc: mov      r1, #0x1e
003c4500: bl       #0x3a4d5c
003c4504: add      r0, r4, #0x4f0
003c4508: add      r0, r0, #0xc
003c450c: mvn      r1, #0
003c4510: bl       #0x3c0b50
003c4514: add      r0, r4, #0x490
003c4518: add      r0, r0, #0xc
003c451c: mov      r1, #0x3f800000
003c4520: bl       #0x3c93fc
003c4524: mov      r3, #0
003c4528: strb     r3, [r4, #0x412]
003c452c: mov      r0, r4
003c4530: bl       #0x3bc6b8
003c4534: ldrb     r3, [r4, #0x554]
003c4538: ldr      r0, [r4, #0x2dc]
003c453c: cmp      r3, #0
003c4540: ldrne    r3, [r4, #0x528]
003c4544: orrne    r3, r3, #0x100
003c4548: strne    r3, [r4, #0x528]
003c454c: cmp      r0, #0
003c4550: beq      #0x3c4558
003c4554: bl       #0x46eae0
003c4558: mov      r0, r4
003c455c: bl       #0x3a3064
003c4560: cmp      r0, #0
003c4564: bne      #0x3c4584
003c4568: ldr      r3, [r5, r7]
003c456c: ldr      r2, [sp, #0x1c]
003c4570: ldr      r3, [r3]
003c4574: cmp      r2, r3
003c4578: bne      #0x3c45b0
003c457c: add      sp, sp, #0x20
003c4580: pop      {r4, r5, r6, r7, r8, pc}
003c4584: mov      r0, r4
003c4588: bl       #0x3a3144
003c458c: cmp      r0, #0
003c4590: bne      #0x3c4568
003c4594: mov      r0, r4
003c4598: bl       #0x3a3158
003c459c: cmp      r0, #0
003c45a0: ldreq    r3, [r4, #0x520]
003c45a4: orreq    r3, r3, #0x10000
003c45a8: streq    r3, [r4, #0x520]
003c45ac: b        #0x3c4568
003c45b0: bl       #0x30e310
003c45b4: subseq   r0, sp, r0, lsl #12
003c45b8: andeq    r4, r0, ip, lsr #1
003c45bc: andeq    r0, r0, r4, lsl #17

# _ZN7CSSkill7OnEventEiP9CharacterP16CharStateMachineiPv
003c0ac8: push     {r4, lr}
003c0acc: ldr      r3, [sp, #8]
003c0ad0: mov      r4, r2
003c0ad4: cmp      r3, #0x28
003c0ad8: bne      #0x3c0afc
003c0adc: ldr      r1, [pc, #0x1c]
003c0ae0: ldr      r0, [sp, #0xc]
003c0ae4: add      r1, pc, r1
003c0ae8: bl       #0x30e31c
003c0aec: cmp      r0, #0
003c0af0: ldreq    r3, [r4, #0x520]
003c0af4: orreq    r3, r3, #0x8000
003c0af8: streq    r3, [r4, #0x520]
003c0afc: pop      {r4, pc}
003c0b00: subseq   r4, r0, r4, ror r0

# _ZN7CSSkill8OnUpdateEiP9CharacterP16CharStateMachine
003c0018: bx       lr

# _ZN7CSSkill6OnInitEiP9CharacterP16CharStateMachine
003c8438: push     {r4, r5, r6, r7, r8, lr}
003c843c: add      r5, r2, #0x4f0
003c8440: add      r5, r5, #0xc
003c8444: sub      sp, sp, #0x50
003c8448: mov      r4, #0
003c844c: mov      r6, r1
003c8450: mov      r0, r5
003c8454: mov      r2, #0x22
003c8458: mov      r3, #3
003c845c: str      r4, [sp, #0x48]
003c8460: str      r4, [sp, #0x4c]
003c8464: str      r4, [sp]
003c8468: str      r4, [sp, #4]
003c846c: ldr      r8, [pc, #0x138]
003c8470: bl       #0x3c7b18
003c8474: mov      r0, r5
003c8478: mov      r1, r6
003c847c: movw     r2, #0xc358
003c8480: mov      r3, #0xc
003c8484: str      r4, [sp, #0x40]
003c8488: str      r4, [sp, #0x44]
003c848c: str      r4, [sp]
003c8490: str      r4, [sp, #4]
003c8494: bl       #0x3c7b18
003c8498: ldr      r3, [pc, #0x110]
003c849c: add      r8, pc, r8
003c84a0: mov      r0, r5
003c84a4: ldr      r7, [r8, r3]
003c84a8: mov      r1, r6
003c84ac: movw     r2, #0xc351
003c84b0: mov      r3, #4
003c84b4: str      r7, [sp, #0x38]
003c84b8: str      r7, [sp]
003c84bc: str      r4, [sp, #0x3c]
003c84c0: str      r4, [sp, #4]
003c84c4: bl       #0x3c7b18
003c84c8: mov      r0, r5
003c84cc: mov      r1, r6
003c84d0: movw     r2, #0xc354
003c84d4: mov      r3, #5
003c84d8: str      r7, [sp, #0x30]
003c84dc: str      r7, [sp]
003c84e0: str      r4, [sp, #0x34]
003c84e4: str      r4, [sp, #4]
003c84e8: bl       #0x3c7b18
003c84ec: mov      r0, r5
003c84f0: mov      r1, r6
003c84f4: movw     r2, #0xc355
003c84f8: mov      r3, #6
003c84fc: str      r7, [sp]
003c8500: str      r7, [sp, #0x28]
003c8504: str      r4, [sp, #0x2c]
003c8508: str      r4, [sp, #4]
003c850c: bl       #0x3c7b18
003c8510: ldr      r3, [pc, #0x9c]
003c8514: mov      r0, r5
003c8518: mov      r1, r6
003c851c: ldr      r7, [r8, r3]
003c8520: movw     r2, #0xc35a
003c8524: mov      r3, #0xb
003c8528: str      r7, [sp, #0x20]
003c852c: str      r4, [sp, #0x24]
003c8530: str      r7, [sp]
003c8534: str      r4, [sp, #4]
003c8538: bl       #0x3c7b18
003c853c: mov      r0, r5
003c8540: mov      r1, r6
003c8544: movw     r2, #0xc35c
003c8548: mov      r3, #9
003c854c: str      r7, [sp, #0x18]
003c8550: str      r4, [sp, #0x1c]
003c8554: str      r7, [sp]
003c8558: str      r4, [sp, #4]
003c855c: bl       #0x3c7b18
003c8560: mov      r0, r5
003c8564: mov      r1, r6
003c8568: movw     r2, #0xc35d
003c856c: mov      r3, #8
003c8570: str      r7, [sp, #0x10]
003c8574: str      r4, [sp, #0x14]
003c8578: str      r7, [sp]
003c857c: str      r4, [sp, #4]
003c8580: bl       #0x3c7b18
003c8584: mov      r0, r5
003c8588: mov      r1, r6
003c858c: movw     r2, #0xc35b
003c8590: mov      r3, #0xa
003c8594: str      r7, [sp]
003c8598: stmib    sp, {r4, r7}
003c859c: str      r4, [sp, #0xc]
003c85a0: bl       #0x3c7b18
003c85a4: add      sp, sp, #0x50
003c85a8: pop      {r4, r5, r6, r7, r8, pc}
003c85ac: ldrsheq  ip, [ip], #-0x54
003c85b0: strheq   r4, [r0], -ip
003c85b4: andeq    r3, r0, ip, asr #9

# _ZN16CharStateMachine16SM_SetSkillStateEjbPvb
003c6670: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c6674: mov      r4, r0
003c6678: ldr      r0, [r0, #4]
003c667c: mov      sb, r2
003c6680: mov      r6, r3
003c6684: mov      r7, r1
003c6688: ldrb     r8, [sp, #0x20]
003c668c: bl       #0x3bc784
003c6690: ldr      r5, [pc, #0x7c]
003c6694: ldr      r3, [pc, #0x7c]
003c6698: ldr      r1, [pc, #0x7c]
003c669c: add      r5, pc, r5
003c66a0: ldr      r3, [r5, r3]
003c66a4: ldr      r2, [pc, #0x74]
003c66a8: ldr      sl, [r0, #4]
003c66ac: add      r1, pc, r1
003c66b0: ldr      r0, [r3, #0x2c]
003c66b4: add      r2, pc, r2
003c66b8: bl       #0x4c4bdc
003c66bc: ands     r0, r0, #0x200000
003c66c0: bne      #0x3c6708
003c66c4: add      sl, r0, sl
003c66c8: cmp      r8, #0
003c66cc: str      sl, [r4, #0x28]
003c66d0: str      r7, [r4, #0x54]
003c66d4: strb     sb, [r4, #0x58]
003c66d8: bne      #0x3c66f0
003c66dc: mov      r0, r4
003c66e0: mov      r2, r6
003c66e4: movw     r1, #0xc355
003c66e8: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003c66ec: b        #0x3c5684
003c66f0: mov      r0, r4
003c66f4: mov      r3, r6
003c66f8: mov      r1, #6
003c66fc: movw     r2, #0xc355
003c6700: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003c6704: b        #0x3c1938
003c6708: ldr      r0, [r4, #4]
003c670c: bl       #0x3a53e0
003c6710: b        #0x3c66c4
003c6714: ldrsheq  lr, [ip], #-0x34
003c6718: strdeq   r3, r4, [r0], -r4
003c671c: subeq    lr, pc, ip, lsl #10
003c6720: subeq    lr, pc, r4, lsl r5
